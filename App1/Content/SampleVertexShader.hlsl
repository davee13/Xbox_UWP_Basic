cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};




struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR0;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 opos : TEXCOORD0;
	float2 tex : TEXCOORD1;
};

PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;

	

	float4 pos = float4(input.pos*1.0, 1.0f);
	
	
	
	

	pos = mul(pos, world);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	
	
	//pos = pos * mul(pos, model);

	float4 opos = float4(pos.xyz, 1.0f);
	
	output.pos = pos;
	output.opos = opos;

	//output.color = input.color * UV.xyy;
	output.tex = input.color * float3(100, 100, 0);// UV.xyy;

	return output;
}
