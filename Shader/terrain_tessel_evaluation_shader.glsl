#version 460 core

layout (quads, fractional_odd_spacing, ccw) in;
//layout (quads, equal_spacing, ccw) in;

in vec2 tcs_out_tex[];
in vec3 tcs_out_normal[];
in vec3 tcs_out_fragPosition[];

out vec2 tes_out_tex;
out vec3 tes_out_normal;
out vec3 tes_out_fragPosition;

out float height;
out float heightFactor;

uniform sampler2D heightMap;
uniform mat4 perspective;
uniform mat4 view;

vec4 heightNormal = vec4(0.0f, 1.0f, 0.0f, 0.0f);
const float patchSize = 20.0f;

vec4 calcPostition(float u, float v, float height)
{ 
    // 컨트롤 포인트
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // 사각형이므로 두 엣지들의 외적값으로 노멀을 계산
//    vec4 uVec = p01 - p00;
//    vec4 vVec = p10 - p00;
//    vec4 normal = normalize( vec4(cross(uVec.xyz, vVec.xyz), 0) );


    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    return (p1 - p0) * v + p0 + heightNormal * height;
}

vec3 calcNormal(float u, float v)
{
    vec3 norm00 = tcs_out_normal[0];
    vec3 norm01 = tcs_out_normal[1];
    vec3 norm10 = tcs_out_normal[2];
    vec3 norm11 = tcs_out_normal[3];

    vec3 norm0 = (norm01 - norm00) * u + norm00;
    vec3 norm1 = (norm11 - norm10) * u + norm10;

    return normalize((norm1 - norm0) * v + norm0);
}

vec3 calcFragPos(float u, float v, float height)
{
    vec3 fp00 = tcs_out_fragPosition[0];
    vec3 fp01 = tcs_out_fragPosition[1];
    vec3 fp10 = tcs_out_fragPosition[2];
    vec3 fp11 = tcs_out_fragPosition[3];

    vec3 fp0 = (fp01 - fp00) * u + fp00;
    vec3 fp1 = (fp11 - fp10) * u + fp10;
    return (fp1 - fp0) * v + fp0 + vec3(heightNormal) * height;
}

void main()
{
    // 가중치 (선형 보간할 계수들)를 받아옴
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // 컨트롤 포인트들의 텍스쳐 좌표들
    vec2 t00 = tcs_out_tex[0];
    vec2 t01 = tcs_out_tex[1];
    vec2 t10 = tcs_out_tex[2];
    vec2 t11 = tcs_out_tex[3];

    // 새로 생성될 프리미티브의 텍스쳐 좌표를 선형보간하여 계산함
    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 texCoord = (t1 - t0) * v + t0;

    // 하이트 맵에서 텍스쳐 색상을 받아옴
    heightFactor = 64.0f;
    vec4 textureFrag = texture(heightMap, texCoord);
    height = max(max(textureFrag.x, textureFrag.y), textureFrag.z) * heightFactor - heightFactor / 2.0f;

    vec4 p = calcPostition(u, v, height);
    tes_out_fragPosition = calcFragPos(u, v, height);

    gl_Position = perspective * view * p;

    tes_out_tex = texCoord;
//    tes_out_normal = calcNormal(u, v);
    tes_out_normal = vec3(heightNormal);
}