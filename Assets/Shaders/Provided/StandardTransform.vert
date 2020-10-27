
#include "Common/MusaVSInputs.hs"
#include "Common/MusaVSToFS.hs"

void main()
{
	MusaVSOutput vsOutput;
	vsOutput.pos = GetWorldPosition();
	vsOutput.norm = GetWorldNormal();
	vsOutput.tan = GetWorldTangent();
	vsOutput.uv = GetUVCoordinates();
	SetVSToFSOutput(vsOutput);
	
	GLSLOutputPosition(GetClipSpacePosition());
}