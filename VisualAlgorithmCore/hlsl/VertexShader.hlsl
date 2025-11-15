cbuffer cbperObject : register(b0)
{
    row_major float4x4 matrixWorld;
    row_major float4x4 matrixView;
    row_major float4x4 matrixProj;

    row_major float3x3 worldInvTranspose
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
}

struct VS_OUTPUT
{
    float4 position : SV_POSTION;
    float3 normal : NORMALW;
}

VS_OUTPUT VS(VS_INPUT vin)
{
    VS_OUTPUT vout;

    float4 posW = mul(float4(vin.position, 1.0f), matrixWorld);
    float4 posV = mul(posW, matrixView);
    vout.position = mul(posV, matrixProj);

    float3 nW = mulP(vin.normal, worldInvTranspose);
    vout.normal = normalize(nW);

    return vout;
}
    