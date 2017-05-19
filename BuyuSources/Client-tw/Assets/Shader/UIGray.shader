Shader "FishShader/UIGray" {
	Properties {
		_MainTex ("Base (RGB)", 2D) = "white" {}
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		CGPROGRAM
		#pragma surface surf FishPS

		sampler2D	_MainTex;
		struct Input 
		{
			float2 uv_MainTex;
		};
		half4 LightingFishPS (SurfaceOutput s, half3 lightDir, half3 viewDir, half atten)
		{
			return float4(s.Albedo, s.Alpha);
		}
		void surf (Input IN, inout SurfaceOutput o) 
		{
			half4 c = tex2D (_MainTex, IN.uv_MainTex);
			o.Albedo = dot(c.rgb, float3(0.299f, 0.587f, 0.114f));
			o.Alpha = c.a;
		}
		ENDCG
	} 
	FallBack "Diffuse"
}
