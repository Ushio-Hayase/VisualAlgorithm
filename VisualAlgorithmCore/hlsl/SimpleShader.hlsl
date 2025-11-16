cbuffer cbperObject : register(b0)
{
    row_major matrix matrixWorld;
    row_major matrix matrixView;
    row_major matrix matrixProj;

    row_major float3x3 worldInvTranspose
}

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texture : TEXCOORD0;
    float3 normal : NORMAL;
}

struct PS_INPUT
{
    float4 position : SV_POSTION;
    float3 normal : NORMALW;
}

PS_INPUT VS(VS_INPUT vin)
{
    PS_OUTPUT out = (PS_INPUT)0;

    // 최종 행렬 계산
    matrix wvp = mul(mul(worldMatrix, viewMatrix), projectionMatrix);

    out.position = mul(float4(vin.position, 1.0f), wvp);
    
    out.normal = input.normal;

    return output;
}

float4 PS(PS_INPUT pin) : SV_TARGET
{
    // 지금은 간단하게 법선 벡터 방향에 따라 색을 결정 (간단한 조명 효과 흉내)
    float3 lightDir = normalize(float3(0.5f, -0.5f, 0.5f));
    float lightAmount = saturate(dot(input.normal, -lightDir)) * 0.5f + 0.5f;

    // 최종 색상 반환 (RGBA)
    return float4(lightAmount, lightAmount, lightAmount, 1.0f);
}

    