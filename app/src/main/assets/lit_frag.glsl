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
};

in vec3 _FRAG_POS;
in vec3 _FRAG_NORM;
in vec2 _FRAG_UV;

uniform mat4 _CAM;
uniform sampler2D _MAIN_TEX;
uniform sampler2D _SHADOW_MAP;
uniform highp sampler2DShadow _SHADOW_MAP_PCF;
uniform sampler2D _HSM;
uniform highp isampler2D _VSM;
uniform Light _LIGHT;
uniform vec3 _AMBIENT;
uniform vec3 _DIFFUSE;
uniform vec3 _SPECULAR;
uniform float _SHININESS;

out vec4 _FRAG_COLOR;

/*
// search area optimization
vec2 SearchArea(float receiver, vec2 uv, vec2 tsize) {
    vec2 area = _LIGHT._AREA;
    float level = ceil(log2(max(area.x / tsize.x, area.y / tsize.y))) + 1.0; // need to clamp between 0 ~ HSM_MAX_LEVEL
    float minDepth = textureLod(_HSM, uv, level).r;
    area = (minDepth / receiver) * area;
    return area;
}
*/

vec2 momentBilinear(highp isampler2D tex, float left, float right, float top, float bottom) {
    vec2 size = vec2(textureSize(tex, 0));

    vec2 tsize = vec2(1.0) / vec2(textureSize(tex, 0));

    vec2 p0q0 = vec2(texture(tex, vec2(right, bottom)).xy - texture(tex, vec2(right, top)).xy - texture(tex, vec2(left, bottom)).xy + texture(tex, vec2(left, top)).xy) / 65536.0;
    vec2 p1q0 = vec2(texture(tex, vec2(right + tsize.x, bottom)).xy - texture(tex, vec2(right + tsize.x, top)).xy - texture(tex, vec2(left + tsize.x, bottom)).xy + texture(tex, vec2(left + tsize.x, top)).xy) / 65536.0;
    vec2 p0q1 = vec2(texture(tex, vec2(right, bottom + tsize.y)).xy - texture(tex, vec2(right, top + tsize.y)).xy - texture(tex, vec2(left, bottom + tsize.y)).xy + texture(tex, vec2(left, top + tsize.y)).xy) / 65536.0;
    vec2 p1q1 = vec2(texture(tex, vec2(right, bottom) + tsize).xy - texture(tex, vec2(right, top) + tsize).xy - texture(tex, vec2(left, bottom) + tsize).xy + texture(tex, vec2(left, top) + tsize).xy) / 65536.0;

    float a = fract(left * size.x);

    vec2 pInterp_q0 = mix(p0q0, p1q0, a);
    vec2 pInterp_q1 = mix(p0q1, p1q1, a);

    float b = fract(top * size.y);
    return mix(pInterp_q0, pInterp_q1, b);
}

float BlockerSearch(ivec2 kernel, float receiver, vec2 uv, vec2 tsize, vec2 hsm_depth) {
    // ivec2 kernel = ivec2(ceil(SearchArea(receiver, uv, tsize) / tsize));

    float zsum = 0.0;
    float cnt = 0.0;

    vec2 size = vec2(kernel - ivec2(1)) * tsize * 0.5;
    float left = uv.x - size.x - tsize.x;
    float right = uv.x + size.x;
    float top = uv.y - size.y - tsize.y;
    float bottom = uv.y + size.y;

    vec2 moment = (vec2(momentBilinear(_VSM, left, right, top, bottom))) / float(kernel.x * kernel.y);
    float E = 0.5 + moment.x;
    float E2 = 0.5 + moment.y;

    float V = max(E2 - E * E, pow(2.0, -18.0));

    float P = clamp(V / ( V + (receiver - E) * (receiver - E)), 0.0, 1.0);
    float invalid = float(receiver <= E);
    return invalid * receiver + (1.0 - invalid) * (clamp((E - P * receiver) / (1.0 - P), hsm_depth.x, receiver));
}

float Random(vec4 seed) {
    float dot_product = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float Visibility(vec3 N, vec3 L) {
    vec4 _LIGHT_FRAG_POS = _LIGHT._WLP * vec4(_FRAG_POS, 1.0);
    vec3 _NORM_FRAG_POS = _LIGHT_FRAG_POS.xyz / _LIGHT_FRAG_POS.w;
    float receiver = _NORM_FRAG_POS.z * 0.5 + 0.5;
    vec2 uv = _NORM_FRAG_POS.xy * 0.5 + 0.5;
    vec2 shadowMapSize = vec2(textureSize(_SHADOW_MAP, 0));
    vec2 shadowMapTexelSize = vec2(1.0) / shadowMapSize;
    vec2 area = _LIGHT._AREA;

    vec2 poissonDisk[16] = vec2[](
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.094184101, -0.92938870),
    vec2(0.34495938, 0.29387760),
    vec2(-0.91588581, 0.45771432),
    vec2(-0.81544232, -0.87912464),
    vec2(-0.38277543, 0.27676845),
    vec2(0.97484398, 0.75648379),
    vec2(0.44323325, -0.97511554),
    vec2(0.53742981, -0.47373420),
    vec2(-0.26496911, -0.41893023),
    vec2(0.79197514, 0.19090188),
    vec2(-0.24188840, 0.99706507),
    vec2(-0.81409955, 0.91437590),
    vec2(0.19984126, 0.78641367),
    vec2(0.14383161, -0.14100790)
    );

    float theta = 2.0 * 3.1415926535 * Random(vec4(_FRAG_POS, _FRAG_POS.z));
    mat2 rot = mat2(cos(theta), sin(theta), -sin(theta), cos(theta));

    // blocker search
    float zavg = 0.0;
    float cnt = 0.0;
    for (int i=0; i<16; i++) {
        vec2 tap = rot * poissonDisk[i];
        float depth = texelFetch(_SHADOW_MAP, ivec2(shadowMapSize * (uv + tap * area * 0.5)), 0).r;
        float test = float(receiver > depth);
        cnt += test;
        zavg += test * depth;
    }
    zavg = min(receiver, zavg / cnt);

    // penumbra width estimation
    vec2 penumbra = max(2.0 * shadowMapTexelSize, ((receiver / zavg) - 1.0) * _LIGHT._AREA);

    // PCF
    float ret = 0.0;
    for (int i=0; i<16; i++) {
        vec2 tap = rot * poissonDisk[i];
        ret += texture(_SHADOW_MAP_PCF, vec3(uv + tap * penumbra * 0.5, receiver));
        // ret += float(receiver < texelFetch(_SHADOW_MAP, ivec2(shadowMapSize * (uv + tap * penumbra * 0.5)), 0).r);
    }
    return ret / 16.0;
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
    // test vsm
    ivec2 coord = ivec2(floor(_FRAG_UV * vec2(textureSize(_VSM, 0))));
    _FRAG_COLOR = vec4(
    0.5 +
    float(
    (texelFetch(_VSM, coord, 0).r -
    texelFetch(_VSM, coord - ivec2(1, 0), 0).r -
    texelFetch(_VSM, coord - ivec2(0, 1), 0).r +
    texelFetch(_VSM, coord - ivec2(1, 1), 0).r)) / 65536.0
    ,
    0.0, 0.0, 1.0);
*/
}