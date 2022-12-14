
Texture2D shaderTexture;
SamplerState SampleType;

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 opos : TEXCOORD0;
	float2 tex : TEXCOORD1;
};

float4 main(PixelShaderInput input) : SV_TARGET
{

		float4 textureColor;


		// Sample the pixel color from the texture using the sampler at this texture coordinate location.
		textureColor = shaderTexture.Sample(SampleType, input.tex);

	//float4 cout = float4(input.opos.yxz*2,1);

	//cout.xyz = float4(input.tex, 1);// input.color;
	return float4(textureColor);
}
