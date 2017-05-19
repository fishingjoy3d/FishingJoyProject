Shader "FishShader/Dissolve"{ 
Properties {
	_Color ("Main Color", Color) = (1,1,1,1)
	_SpecColor ("Specular Color", Color) = (0.5, 0.5, 0.5, 1)
	_Shininess ("Shininess", Range (0.03, 1)) = 0.078125
	_BlendFactor ("_BlendFactor", Range (0, 1)) = 0.0
	_StartAmount("StartAmount", float) = 0.1
	_Illuminate ("Illuminate", Range (0, 1)) = 0.5
	_Tile("Tile", float) = 1
	_DissColor ("DissColor", Color) = (1,1,1,1)
	_ColorAnimate ("ColorAnimate", vector) = (1,1,1,1)
	_MainTex ("Base (RGB) Gloss (A)", 2D) = "white" {}
	_RefracTex("_RefracTex", 2D) = "white"{}
	_EffectTex ("DissolveSrc", 2D) = "white" {}

}
SubShader { 
	Tags { "RenderType"="Opaque" }
	LOD 400
	cull off
	
	
CGPROGRAM
#pragma target 3.0
#pragma surface surf FishPS



sampler2D _MainTex;
sampler2D _EffectTex;
sampler2D _RefracTex;

fixed4 _Color;
half4 _DissColor;
half _Shininess;
half _BlendFactor;
static half3 Color = float3(1,1,1);
half4 _ColorAnimate;
half _Illuminate;
half _Tile;
half _StartAmount;



struct Input {
	float2 uv_MainTex;
	float2 uv_BumpMap;
	float3 viewDir;
	float3 worldPos;
	float2 uvDissolveSrc;
};

void vert (inout appdata_full v, out Input o) {}

half4 LightingFishPS (SurfaceOutput s, half3 lightDir, half3 viewDir, half atten)
{
	return float4(s.Albedo, s.Alpha);
}

void surf (Input IN, inout SurfaceOutput o) {

	half4 c = tex2D (_MainTex, IN.uv_MainTex);
	half4 r = tex2D(_RefracTex, IN.worldPos.xz * 0.008f);

	const float DIFFUSE_SCALING = 1.0f;
	float dotn = max(dot(o.Normal, float3(0, 1, 0)), 0);
	c.rgb *= DIFFUSE_SCALING;
	c.rgb += c.rgb * dotn * (1 - DIFFUSE_SCALING);
	r.rgb *= dotn * 4.0 * c.rgb;
	o.Albedo = r.rgb + c.rgb;

	o.Alpha = c.a;
	half rim = 1.0 - saturate(dot (normalize(IN.viewDir), o.Normal));
	o.Albedo += float3(0.2f, 0.2f, 0.2f) * pow (rim, 3);
	
	
	float ClipTex = tex2D (_EffectTex, IN.uv_MainTex/_Tile).r ;
	float ClipAmount = ClipTex - _BlendFactor;
	float Clip = 0;
	//float4 DematBump =  float4(1, 1, 1, 1);
	//o.Normal = float3(1, 1, 1);
if (_BlendFactor > 0)
{
	if (ClipAmount <0)
	{
		Clip = 1; //clip(-0.1);
	
	}
	 else
	 {
	
		if (ClipAmount < _StartAmount)
		{
			if (_ColorAnimate.x == 0)
				Color.x = _DissColor.x;
			else
				Color.x = ClipAmount/_StartAmount;
          
			if (_ColorAnimate.y == 0)
				Color.y = _DissColor.y;
			else
				Color.y = ClipAmount/_StartAmount;
          
			if (_ColorAnimate.z == 0)
				Color.z = _DissColor.z;
			else
				Color.z = ClipAmount/_StartAmount;

			o.Albedo  = (o.Albedo *((Color.x+Color.y+Color.z))* Color*((Color.x+Color.y+Color.z)))/(1 - _Illuminate);
			//o.Normal = float3(1, 1, 1);
		
		}
	 }
 }

 
if (Clip == 1)
{
clip(-0.1);
}

   
	//////////////////////////////////
	//
	//o.Gloss = c.a;
	//o.Alpha = c.a * _Color.a;
	//o.Specular = _Shininess;
	
}
ENDCG
}

FallBack "Specular"
}
