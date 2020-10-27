
#define VIEW_BINDING 3

#include "Common/LightFSInput.hs"
#include "Common/GBufferResources.hs"
#include "Common/View.hs"
#include "Common/LightProperties.hs"
#include "Common/OutputColor.hs"

void main()
{
	GBufferData gbufferData = GetGBufferData(GetGBufferCoords());
	
	
	// Blinn Shading
	const float specular_power = 128.0;
	
	// Normalize the incoming N, L and V vectors
    vec3 N = normalize(gbufferData.normal);
    vec3 L = normalize(light.position.xyz - gbufferData.position);
    vec3 V = normalize(view.position.xyz - gbufferData.position);
    vec3 H = normalize(L + V);


	float difference = max(dot(N, L), 0.0);
	vec3 diffuse = difference * gbufferData.diffuse.rgb;

	float specularValue = pow(max(dot(N, H), 0.0), specular_power);
    vec3 specular = light.color.rgb * specularValue;

    // Write final color to the framebuffer
    OutputColor(vec4(diffuse + specular, 1));
}
