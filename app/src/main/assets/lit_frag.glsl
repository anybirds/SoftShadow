#version 320 es
precision highp float;
precision highp int;
precision highp sampler2D;

struct Light {
    vec3 _AMBIENT;
    vec3 _DIFFUSE;
    vec3 _SPECULAR;
    mat4 _WLP; // World Light Projection
    vec3 _DIR;
    vec2 _AREA; // area light width and height in normalized light space
    // float _NEAR;
};

in vec3 _FRAG_POS;
in vec3 _FRAG_NORM;
in vec2 _FRAG_UV;

uniform mat4 _CAM;
uniform sampler2D _MAIN_TEX;
uniform highp sampler2DShadow _SHADOW_MAP;
uniform sampler2D _HSM;
uniform highp isampler2D _VSM;
uniform Light _LIGHT;
uniform vec3 _AMBIENT;
uniform vec3 _DIFFUSE;
uniform vec3 _SPECULAR;
uniform float _SHININESS;

out vec4 _FRAG_COLOR;

vec2 SearchArea(float receiver, vec2 uv, vec2 tsize) {
    vec2 area = _LIGHT._AREA;
    float level = ceil(log2(max(area.x / tsize.x, area.y / tsize.y))) + 1.0; // need to clamp between 0 ~ HSM_MAX_LEVEL
    float minDepth = textureLod(_HSM, uv, level).r;
    area = (minDepth / receiver) * area;

    /*
    for (;;) {
        float level = ceil(log2(max(area.x / tsize.x, area.y / tsize.y))); // need to clamp between 0 ~ HSM_MAX_LEVEL
        float minDepth = textureLod(_HSM, uv, level).r;
        vec2 nextArea = (minDepth / receiver) * area;
        if (area == nextArea) {
            break;
        }
        area = nextArea;
    }
    */
    return area;
}


float BlockerSearch(ivec2 kernel, float receiver, vec2 vsm_uv, vec2 vsm_tsize) {
    // vec2 area = (1.0 - _LIGHT._NEAR / receiver) * _LIGHT._AREA;
    // ivec2 kernel = ivec2(ceil(SearchArea(receiver, uv, tsize) / tsize));

    float zsum = 0.0;
    float cnt = 0.0;

    vec2 size = vec2(kernel - ivec2(1)) * vsm_tsize * 0.5;
    float left = vsm_uv.x - size.x - vsm_tsize.x;
    float right = vsm_uv.x + size.x;
    float top = vsm_uv.y - size.y - vsm_tsize.y;
    float bottom = vsm_uv.y + size.y;

    vec2 avg = (vec2(ivec2(texture(_VSM, vec2(right, bottom))) - ivec2(texture(_VSM, vec2(right, top))) - ivec2(texture(_VSM, vec2(left, bottom))) + ivec2(texture(_VSM, vec2(left, top)))) / 16384.0) / float(kernel.x * kernel.y);
    float E = 0.5 + avg.x;
    float E2 = 0.5 + avg.y;

    float V = max(E2 - E * E, pow(2.0, -14.0));

    if (receiver <= E) {
        return receiver;
    }

    float P = clamp(V / ( V + (receiver - E) * (receiver - E)), 0.0, 1.0);
    return min((E - P * receiver) / (1.0 - P), receiver);
}

ivec4 mixInt(ivec4 l, ivec4 r, float t) {
    vec4 lh = vec4(l / 65536);
    vec4 ll = vec4(l - ivec4(lh) * 65536);
    vec4 rh = vec4(r / 65536);
    vec4 rl = vec4(r - ivec4(rh) * 65536);

    vec4 hInterp = mix(lh, rh, t);
    vec4 lInterp = mix(ll, rl, t);
    return ivec4(floor(hInterp)) * 65536 + ivec4(fract(hInterp) * 65536.0) + ivec4(floor(lInterp));
}

ivec4 textureBilinear(highp isampler2D tex, vec2 uv) {
    vec2 size = vec2(textureSize(tex, 0));
    vec2 tsize = vec2(1.0) / vec2(textureSize(tex, 0));

    ivec4 p0q0 = texture(tex, uv);
    ivec4 p1q0 = texture(tex, uv + vec2(tsize.x, 0.0));
    ivec4 p0q1 = texture(tex, uv + vec2(0.0, tsize.y));
    ivec4 p1q1 = texture(tex, uv + tsize);

    float a = fract(uv.x * size.x);

    ivec4 pInterp_q0 = mixInt(p0q0, p1q0, a);
    ivec4 pInterp_q1 = mixInt(p0q1, p1q1, a);

    float b = fract(uv.y * size.y);
    return mixInt(pInterp_q0, pInterp_q1, b);
}

vec4 momentBilinear(highp isampler2D tex, float left, float right, float top, float bottom) {
    vec2 size = vec2(textureSize(tex, 0));
    vec2 tsize = vec2(1.0) / vec2(textureSize(tex, 0));

    vec4 p0q0 = vec4(texture(tex, vec2(right, bottom)) - texture(tex, vec2(right, top)) - texture(tex, vec2(left, bottom)) + texture(tex, vec2(left, top))) / 16384.0;
    vec4 p1q0 = vec4(texture(tex, vec2(right + tsize.x, bottom)) - texture(tex, vec2(right + tsize.x, top)) - texture(tex, vec2(left + tsize.x, bottom)) + texture(tex, vec2(left + tsize.x, top))) / 16384.0;
    vec4 p0q1 = vec4(texture(tex, vec2(right, bottom + tsize.y)) - texture(tex, vec2(right, top + tsize.y)) - texture(tex, vec2(left, bottom + tsize.y)) + texture(tex, vec2(left, top + tsize.y))) / 16384.0;
    vec4 p1q1 = vec4(texture(tex, vec2(right, bottom) + tsize) - texture(tex, vec2(right, top) + tsize) - texture(tex, vec2(left, bottom) + tsize) + texture(tex, vec2(left, top) + tsize)) / 16384.0;

    float a = fract(left * size.x);

    vec4 pInterp_q0 = mix(p0q0, p1q0, a);
    vec4 pInterp_q1 = mix(p0q1, p1q1, a);

    float b = fract(top * size.y);
    return mix(pInterp_q0, pInterp_q1, b);
}

float Visibility(vec3 N, vec3 L) {
    vec4 _LIGHT_FRAG_POS = _LIGHT._WLP * vec4(_FRAG_POS, 1.0);
    vec3 _NORM_FRAG_POS = _LIGHT_FRAG_POS.xyz / _LIGHT_FRAG_POS.w;
    float receiver = _NORM_FRAG_POS.z * 0.5 + 0.5;
    vec2 uv = _NORM_FRAG_POS.xy * 0.5 + 0.5;
    vec2 tsize = vec2(1.0) / vec2(textureSize(_SHADOW_MAP, 0));
    vec2 vsm_uv = uv * (vec2(textureSize(_SHADOW_MAP, 0)) / vec2(textureSize(_VSM, 0))) + vec2(1.0) / vec2(textureSize(_SHADOW_MAP, 0));
    vec2 vsm_tsize = vec2(1.0) / vec2(textureSize(_VSM, 0));
    float bias = max(0.005 * (1.0 - dot(N, L)), 0.001);

    vec2 area = _LIGHT._AREA;
    ivec2 kernel = ivec2(ceil(area / vsm_tsize));
    float level = ceil(log2(max(area.x / tsize.x, area.y / tsize.y))); // need to clamp between 0 ~ HSM_MAX_LEVEL
    vec2 depth = vec2(textureLod(_HSM, uv, level));
    // how to remove branch?
    /*if (receiver > depth.y + bias) {
        return 0.0;
    } else */{
        // PCSS

        float zavg = BlockerSearch(kernel, receiver, vsm_uv, vsm_tsize);
        vec2 penumbra = ((receiver / zavg) - 1.0) * _LIGHT._AREA;
        ivec2 kernel = ivec2(max(vec2(4.0), ceil(penumbra / vsm_tsize)));
        // return float(kernel.x) / 10.0;
        // vec2 filter = _LIGHT._NEAR / receiver * penumbra;
        // ivec2 kernel = ivec2(20);
/*
        if (abs(zavg - receiver) < 0.02) {
            float ret = 0.0;
            for (int i=-1; i<=1; i++) {
                for (int j=-1; j<=1; j++) {
                    ret += texture(_SHADOW_MAP, vec3(uv.x + float(i) * tsize.x, uv.y + float(j) * tsize.y, receiver - bias));
                }
            }
            return ret / 9.0;
        } else */{
            ivec2 kernel = ivec2(clamp(ceil(penumbra / vsm_tsize), vec2(3.0), vec2(25.0)));
            vec2 size = vec2(kernel - ivec2(1)) * vsm_tsize * 0.5;

            float left = vsm_uv.x - size.x - vsm_tsize.x;
            float right = vsm_uv.x + size.x;
            float top = vsm_uv.y - size.y - vsm_tsize.y;
            float bottom = vsm_uv.y + size.y;

            vec2 avg = (vec2(momentBilinear(_VSM, left, right, top, bottom))) / float(kernel.x * kernel.y);
            // vec2 avg = (vec2(textureBilinear(_VSM, vec2(right, bottom)) - textureBilinear(_VSM, vec2(right, top)) - textureBilinear(_VSM, vec2(left, bottom)) + textureBilinear(_VSM, vec2(left, top))) / 16384.0) / float(kernel.x * kernel.y);
            float E = 0.5 + avg.x;
/*
            if (abs(E - receiver) < 0.003) {
                return 1.0;
            }
*/
            float E2 = 0.5 + avg.y;
            float V = max(E2 - E * E, pow(2.0, -16.0));

            if (receiver <= E) {
                return 1.0;
            }
            float P = clamp(V / (V + (receiver - E) * (receiver - E)), 0.0, 1.0);
            return P;
        }
    }

    /*
    // PCF
    float ret = 0.0;
    for (int i=-1; i<=1; i++) {
        for (int j=-1; j<=1; j++) {
            float blocker = texture(_SHADOW_MAP, uv + vec2(i, j) * tsize).r;
            if (blocker + bias >= receiver) {
                ret += 1.0;
            }
        }
    }
    return ret / 9.0;
    */
}

void main() {

    vec3 N = normalize(_FRAG_NORM);
    vec3 L = normalize( _LIGHT._DIR);
    vec3 V = normalize(vec3(_CAM[3]) - _FRAG_POS);
    vec3 H = normalize(V + L);

    vec3 I = clamp(
    _LIGHT._AMBIENT * _AMBIENT +
    Visibility(N, L) * (
    _LIGHT._DIFFUSE * _DIFFUSE * max(0.0, dot(N, L)) +
    _LIGHT._SPECULAR * _SPECULAR * max(0.0, pow(dot(N, H), _SHININESS))
    ),
    0.0, 1.0);

    _FRAG_COLOR = texture(_MAIN_TEX, _FRAG_UV) * vec4(I, 1.0);

    /*
    vec3 N = normalize(_FRAG_NORM);
    vec3 L = normalize( _LIGHT._DIR);
    _FRAG_COLOR = vec4(Visibility(N, L), 0.0, 0.0, 1.0);
    */
    // _FRAG_COLOR = 0.2 + 0.02 * vec4(texture(_VSM, _FRAG_UV).r, 0.0, 0.0, 1.0);
    ivec2 coord = ivec2(floor(_FRAG_UV * vec2(textureSize(_VSM, 0))));
    // _FRAG_COLOR = vec4(1.0);
    // _FRAG_COLOR = vec4(texture(_SHADOW_MAP, _FRAG_UV).r, 0.0, 0.0, 1.0);

    /*
    vec4 sam = vec4(texelFetch(_VSM, coord, 0).r, texelFetch(_VSM, coord - ivec2(1, 0), 0).r, texelFetch(_VSM, coord - ivec2(0, 1), 0).r, texelFetch(_VSM, coord - ivec2(1, 1), 0).r);
    vec4 logSample = floor(log2(sam));
    vec4 level = max(vec4(0.0), logSample + vec4(15.0));
    float result = 1024.0 * (
    (level.x + (sam.x - min(level.x, 1.0) * pow(2.0, logSample.x)) / pow(2.0, max(-14.0, logSample.x))) -
    (level.y + (sam.y - min(level.y, 1.0) * pow(2.0, logSample.y)) / pow(2.0, max(-14.0, logSample.y))) -
    (level.z + (sam.z - min(level.z, 1.0) * pow(2.0, logSample.z)) / pow(2.0, max(-14.0, logSample.z))) +
    (level.w + (sam.w - min(level.w, 1.0) * pow(2.0, logSample.w)) / pow(2.0, max(-14.0, logSample.w)))
    );
    _FRAG_COLOR = vec4(0.5 * level.x, 0.0, 0.0, 1.0);
    */
/*
    _FRAG_COLOR = vec4(
    0.5 +
    float(
    (texelFetch(_VSM, coord, 0).r -
    texelFetch(_VSM, coord - ivec2(1, 0), 0).r -
    texelFetch(_VSM, coord - ivec2(0, 1), 0).r +
    texelFetch(_VSM, coord - ivec2(1, 1), 0).r)) / 16384.0
    ,
    0.0, 0.0, 1.0);
*/
}