
#include "Common/MusaFSInput.hs"
#include "Common/MusaGBufferOutput.hs"

void main()
{
	MusaFSInput inputs = GetInputs();
	GBufferOutData gbufferData;
	
	gbufferData.position = vec4(0, 0, 0, 1);
	gbufferData.normal = vec4(0, 0, 0, 0);
	gbufferData.diffuse = materialProperties.diffuse;
	OutputToGBuffer(gbufferData);
}