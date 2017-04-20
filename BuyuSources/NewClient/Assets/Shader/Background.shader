Shader "FishShader/Background" {
	Properties {
		_MainTex("Base (RGB)", 2D) = "white" {}
		_Light("光照参数", Vector) = (1, 1, 1, 1)
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		
		CGPROGRAM
		#pragma surface surf Background
		sampler2D _MainTex;
		float4	  _Light;
		struct Input {
			float2 uv_MainTex;
		};
		half4 LightingBackground (SurfaceOutput s, half3 lightDir, half3 viewDir, half atten)
		{
			return float4(s.Albedo, s.Alpha);
		}
		void surf (Input IN, inout SurfaceOutput o){
			half4 c = tex2D (_MainTex, IN.uv_MainTex);
			o.Albedo = c.rgb * _Light.rgb * 0.85f;
			o.Alpha = c.a * _Light.a;
		}
		ENDCG
	} 
	FallBack "Diffuse"
}
