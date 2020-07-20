#version 320 es
precision highp float;

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
uniform sampler2D _SHADOW_MAP;
uniform sampler2D _HSM;
uniform sampler2D _VSM;
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

float BlockerSearch(ivec2 kernel, float receiver, vec2 uv, vec2 tsize) {
    // vec2 area = (1.0 - _LIGHT._NEAR / receiver) * _LIGHT._AREA;
    // ivec2 kernel = ivec2(ceil(SearchArea(receiver, uv, tsize) / tsize));

    float zsum = 0.0;
    float cnt = 0.0;
    vec2 UV = uv - (vec2(kernel) - vec2(1.0)) * tsize * 0.5;
    for (int i=0; i<kernel.x; i++) {
        for (int j=0; j<kernel.y; j++) {
            float blocker = texture(_SHADOW_MAP, UV + vec2(i, j) * tsize).r;
            if (blocker < receiver) {
                zsum += blocker;
                cnt += 1.0;
            }
        }
    }
    return (cnt == 0.0) ? receiver : zsum / cnt;
}

float Visibility(vec3 N, vec3 L) {
    vec4 _LIGHT_FRAG_POS = _LIGHT._WLP * vec4(_FRAG_POS, 1.0);
    vec3 _NORM_FRAG_POS = _LIGHT_FRAG_POS.xyz / _LIGHT_FRAG_POS.w;
    float receiver = _NORM_FRAG_POS.z * 0.5 + 0.5;
    vec2 uv = _NORM_FRAG_POS.xy * 0.5 + 0.5;
    float bias = max(0.00025 * (1.0 - dot(N, L)), 0.0001);
    vec2 tsize = vec2(1.0) / vec2(textureSize(_SHADOW_MAP, 0));

    vec2 area = _LIGHT._AREA;
    ivec2 kernel = ivec2(ceil(area / tsize));
    float level = ceil(log2(max(area.x / tsize.x, area.y / tsize.y))); // need to clamp between 0 ~ HSM_MAX_LEVEL
    vec2 depth = vec2(textureLod(_HSM, uv, level));
    // how to remove branch?
    if (receiver > depth.y + bias) {
        return 0.0;
    } else {
    // PCSS
        float zavg = BlockerSearch(kernel, receiver, uv, tsize);
        vec2 penumbra = ((receiver / zavg) - 1.0) * _LIGHT._AREA;
        // vec2 filter = _LIGHT._NEAR / receiver * penumbra;
        ivec2 kernel = ivec2(max(vec2(1.0), ceil(penumbra / tsize)));
        vec2 UV = uv - (vec2(kernel) - vec2(1.0)) * tsize * 0.5;
        float ret = 0.0;
        for (int i=0; i<kernel.x; i++) {
            for (int j=0; j<kernel.y; j++) {
                float blocker = texture(_SHADOW_MAP, UV + vec2(i, j) * tsize).r;
                if (blocker + bias >= receiver) {
                    ret += 1.0;
                }
            }
        }
        return ret / float(kernel.x * kernel.y);
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
    /*
    vec3 N = normalize(_FRAG_NORM);
    vec3 L = normalize( _LIGHT._DIR);
    vec3 V = normalize(vec3(_CAM[3]) - _FRAG_POS);
    vec3 H = normalize(V + L);

    vec3 I = clamp(
    _LIGHT._AMBIENT * _AMBIENT +
    Visibility(N, L) * (
    _LIGHT._DIFFUSE * _DIFFUSE * dot(N, L) +
    _LIGHT._SPECULAR * _SPECULAR * max(0.0, pow(dot(N, H), _SHININESS))
    ),
    0.0, 1.0);
    */

    vec4 temp = texture(_VSM, _FRAG_UV) * 0.1 * 0.1 * 0.1 * 0.1;
    _FRAG_COLOR = vec4(temp.r, 0.0, 0.0, 1.0);

    // _FRAG_COLOR = texture(_MAIN_TEX, _FRAG_UV) * vec4(I, 1.0);
}