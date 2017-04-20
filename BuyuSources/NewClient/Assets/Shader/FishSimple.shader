Shader "FishShader/Simple" {
	Properties {
		_MainTex ("Base (RGB)", 2D) = "white" {}
		_RefracTex ("折射贴图", 2D) = "black" {}
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		CGPROGRAM
		#pragma surface surf FishPS

		sampler2D	_MainTex;
		sampler2D	_RefracTex;
		struct Input {
			float2 uv_MainTex;
			float3 viewDir;
			float3 worldPos;
			float4 screenPos;
		};
		half4 LightingFishPS (SurfaceOutput s, half3 lightDir, half3 viewDir, half atten)
		{
			return float4(s.Albedo, s.Alpha);
		}
		void surf (Input IN, inout SurfaceOutput o) {
			{
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
			}
		}
		ENDCG
	} 
	FallBack "Diffuse"
}
