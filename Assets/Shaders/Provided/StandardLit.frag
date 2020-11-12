
#include "Common/MusaFSInput.hs"
#include "Common/MusaGBufferOutput.hs"

void main()
{
	MusaFSInput inputs = GetInputs();
	GBufferOutData gbufferData;
	
	gbufferData.position = vec4(inputs.interpPos, 1);
	gbufferData.normal = vec4(inputs.interpNorm, 0);
	gbufferData.diffuse = texture(mainTexture, inputs.interpUV) * materialProperties.diffuse;
	OutputToGBuffer(gbufferData);
}
