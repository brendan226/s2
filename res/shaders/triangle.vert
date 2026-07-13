struct VSInput {
    float2 pos   : POSITION;
    float3 color : COLOR;
};

struct VSOutput {
    float4 pos   : SV_Position;
    float3 color : COLOR;
};

VSOutput main(VSInput input)
{
    VSOutput o;
    o.pos   = float4(input.pos, 0.0, 1.0);
    o.color = input.color;
    return o;
}