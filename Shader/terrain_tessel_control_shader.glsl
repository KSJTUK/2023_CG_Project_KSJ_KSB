#version 460 core

// 패치는 사각형으로 받음
layout (vertices=4) out;

// 정점 셰이더로 부터 텍스쳐, 노멀 데이터 받아오기
in vec2 vs_out_tex[];
in vec3 vs_out_normal[];
in vec3 vs_out_fragPosition[];

// 테셀레이션 평가 TES 쉐이더로 데이터 이전
out vec2 tcs_out_tex[];
out vec3 tcs_out_normal[];
out vec3 tcs_out_fragPosition[];

// 최소 테셀레이션 레벨과 최대 테셀레이션 레벨을 정의
const int minTesselLevel = 2;
const int maxTesselLevel = 32;

// 최소 테셀레이션 레벨과 최대 테셀레이션 레벨을 적용할 거리 정의
// 최소거리 -> 최대 테셀레이션 레벨, 최대거리 -> 최소 테셀레이션 레벨
const float minDist = 20;
const float maxDist = 800;

const int tessLevel = 64;

void main()
{
    // 다음 셰이더로 데이터 전송
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tcs_out_tex[gl_InvocationID] = vs_out_tex[gl_InvocationID];
    tcs_out_normal[gl_InvocationID] = vs_out_normal[gl_InvocationID];
    tcs_out_fragPosition[gl_InvocationID] = vs_out_fragPosition[gl_InvocationID];

    if (gl_InvocationID == 0) {
        // 컨트롤 포인트들을 받아오기
        vec4 p00 = gl_in[0].gl_Position;
        vec4 p01 = gl_in[1].gl_Position;
        vec4 p10 = gl_in[2].gl_Position;
        vec4 p11 = gl_in[3].gl_Position;

        // 카메라 공간으로 이전된 컨트롤 포인트들의 z값과 거리를 비교하여
        // 직선의 방정식으로 거리 비율을 구함 (0~1)사이 값 clamp를 사용해 값이 밖으로 나가지 않도록함
        float distance00 = clamp((abs(p00.z) - minDist) / (maxDist - minDist), 0.0, 1.0 );
        float distance01 = clamp((abs(p01.z) - minDist) / (maxDist - minDist), 0.0, 1.0 );
        float distance10 = clamp((abs(p10.z) - minDist) / (maxDist - minDist), 0.0, 1.0 );
        float distance11 = clamp((abs(p11.z) - minDist) / (maxDist - minDist), 0.0, 1.0 );

        // 테셀레이션 레벨을 거리 비율로 선형 보간함
        float tessLevel0 = mix(maxTesselLevel, minTesselLevel, min(distance10, distance00) );
        float tessLevel1 = mix(maxTesselLevel, minTesselLevel, min(distance00, distance01) );
        float tessLevel2 = mix(maxTesselLevel, minTesselLevel, min(distance01, distance11) );
        float tessLevel3 = mix(maxTesselLevel, minTesselLevel, min(distance11, distance10) );

        // 테셀레이션 레벨 적용
        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        // 내부 테셀레이션 레벨은 각 엣지들의 테셀레이션 레벨들 중에서 큰 값들을 사용함
        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}