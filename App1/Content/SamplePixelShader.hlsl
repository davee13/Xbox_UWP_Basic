struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 opos : TEXCOORD0;
	float3 color : COLOR0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{



	float4 cout = float4(input.opos.y*2,0,0,1);

	cout.xyz = float4(input.color, 1);// input.color;
	return float4(cout);
}
