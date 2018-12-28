#include "MeshManager.h"
#include "Mesh.h"
#include "PlatformDefinitions.h"
#include "File/FileCore/File.h"
#include "ModelFileHeader.h"
#include "DirectoryLocations.h"
#include "Camera/Camera.h"

Mesh* MeshManager::LoadPrimitive(Primitive primitiveMesh)
{
	switch (primitiveMesh)
	{
	case Primitive::Sphere:
		return Instance().LoadSpherePrimitive();
	case Primitive::Box:
		return Instance().LoadBoxPrimitive();
	case Primitive::Plane:
		return Instance().LoadPlanePrimitive();
	case Primitive::Pyramid:
		return Instance().LoadPyramidPrimitive();
	default:
		assert(false);
	}

	return nullptr;
}

Mesh* MeshManager::LoadFrustumForCamera(Camera * camera)
{
	return Instance().LoadCameraFrustum(camera);
}

Mesh* MeshManager::LoadMeshFromPak(uint8* modelData, const char * modelName)
{
	return Instance().LoadMeshInternalPak(modelData, modelName);
}

Mesh* MeshManager::LoadMeshFromPak(uint8 * modelData, uint8 * skinningData, const char * modelName)
{
	return Instance().LoadMeshInternalPak(modelData, skinningData, modelName);
}

void MeshManager::UnloadMesh(const char * modelName)
{
	Instance().UnloadMeshInternal(modelName);
}

Mesh* MeshManager::FindMesh(const char * modelName)
{
	MeshNode* current = Instance().head;
	while (current != nullptr)
	{
		if (strcmp(modelName, current->meshName) == 0)
		{
			return current->mesh;
		}

		current = current->next;
	}
	return nullptr;
}

Mesh* MeshManager::LoadCameraFrustum(Camera* cam)
{
	Vertex vertices[9];
	Face faces[8];

	cam->Update();
	Frustum camFrustum = cam->GetCameraFrustum();

	faces[0].v0 = 0;
	faces[0].v1 = 1;
	faces[0].v2 = 2;

	faces[1].v0 = 0;
	faces[1].v1 = 2;
	faces[1].v2 = 3;

	faces[2].v0 = 0;
	faces[2].v1 = 3;
	faces[2].v2 = 4;

	faces[3].v0 = 0;
	faces[3].v1 = 4;
	faces[3].v2 = 1;

	// far
	faces[4].v0 = 1;
	faces[4].v1 = 2;
	faces[4].v2 = 3;

	// far
	faces[5].v0 = 3;
	faces[5].v1 = 4;
	faces[5].v2 = 1;

	// near
	faces[6].v0 = 5;
	faces[6].v1 = 6;
	faces[6].v2 = 7;

	// near
	faces[7].v0 = 7;
	faces[7].v1 = 8;
	faces[7].v2 = 5;

	Vector pos = cam->GetPosition();
	vertices[0].x = pos.x;
	vertices[0].y = pos.y;
	vertices[0].z = pos.z;
	vertices[0].nx = 0.000000f;
	vertices[0].ny = 0.000000f;
	vertices[0].nz = 0.000000f;
	vertices[0].u = 0.000000f;
	vertices[0].v = 0.000000f;

	vertices[1].x = camFrustum.farTopRight.x;
	vertices[1].y = camFrustum.farTopRight.y;
	vertices[1].z = camFrustum.farTopRight.z;
	vertices[1].nx = 0.000000f;
	vertices[1].ny = 0.000000f;
	vertices[1].nz = 0.000000f;
	vertices[1].u = 0.000000f;
	vertices[1].v = 0.000000f;

	vertices[2].x = camFrustum.farTopLeft.x;
	vertices[2].y = camFrustum.farTopLeft.y;
	vertices[2].z = camFrustum.farTopLeft.z;
	vertices[2].nx = 0.000000f;
	vertices[2].ny = 0.000000f;
	vertices[2].nz = 0.000000f;
	vertices[2].u = 0.000000f;
	vertices[2].v = 0.000000f;

	vertices[3].x = camFrustum.farBottomLeft.x;
	vertices[3].y = camFrustum.farBottomLeft.y;
	vertices[3].z = camFrustum.farBottomLeft.z;
	vertices[3].nx = 0.000000f;
	vertices[3].ny = 0.000000f;
	vertices[3].nz = 0.000000f;
	vertices[3].u = 0.000000f;
	vertices[3].v = 0.000000f;

	vertices[4].x = camFrustum.farBottomRight.x;
	vertices[4].y = camFrustum.farBottomRight.y;
	vertices[4].z = camFrustum.farBottomRight.z;
	vertices[4].nx = 0.000000f;
	vertices[4].ny = 0.000000f;
	vertices[4].nz = 0.000000f;
	vertices[4].u = 0.000000f;
	vertices[4].v = 0.000000f;

	vertices[5].x = camFrustum.nearTopRight.x;
	vertices[5].y = camFrustum.nearTopRight.y;
	vertices[5].z = camFrustum.nearTopRight.z;
	vertices[5].nx = 0.000000f;
	vertices[5].ny = 0.000000f;
	vertices[5].nz = 0.000000f;
	vertices[5].u = 0.000000f;
	vertices[5].v = 0.000000f;

	vertices[6].x = camFrustum.nearTopLeft.x;
	vertices[6].y = camFrustum.nearTopLeft.y;
	vertices[6].z = camFrustum.nearTopLeft.z;
	vertices[6].nx = 0.000000f;
	vertices[6].ny = 0.000000f;
	vertices[6].nz = 0.000000f;
	vertices[6].u = 0.000000f;
	vertices[6].v = 0.000000f;

	vertices[7].x = camFrustum.nearBottomLeft.x;
	vertices[7].y = camFrustum.nearBottomLeft.y;
	vertices[7].z = camFrustum.nearBottomLeft.z;
	vertices[7].nx = 0.000000f;
	vertices[7].ny = 0.000000f;
	vertices[7].nz = 0.000000f;
	vertices[7].u = 0.000000f;
	vertices[7].v = 0.000000f;

	vertices[8].x = camFrustum.nearBottomRight.x;
	vertices[8].y = camFrustum.nearBottomRight.y;
	vertices[8].z = camFrustum.nearBottomRight.z;
	vertices[8].nx = 0.000000f;
	vertices[8].ny = 0.000000f;
	vertices[8].nz = 0.000000f;
	vertices[8].u = 0.000000f;
	vertices[8].v = 0.000000f;

	Mesh* mesh = new Mesh(vertices, faces);
	MeshNode* node = new MeshNode;
	node->meshName = "Frustum";
	node->mesh = mesh;
	node->next = head;
	head = node;

	return mesh;
}

Mesh* MeshManager::LoadSpherePrimitive()
{
	Vertex vertices[121];
	Face faces[200];

	// Faces
	{
		faces[0].v0 = 0;
		faces[0].v1 = 1;
		faces[0].v2 = 2;

		faces[1].v0 = 1;
		faces[1].v1 = 3;
		faces[1].v2 = 2;

		faces[2].v0 = 2;
		faces[2].v1 = 3;
		faces[2].v2 = 4;

		faces[3].v0 = 3;
		faces[3].v1 = 5;
		faces[3].v2 = 4;

		faces[4].v0 = 4;
		faces[4].v1 = 5;
		faces[4].v2 = 6;

		faces[5].v0 = 5;
		faces[5].v1 = 7;
		faces[5].v2 = 6;

		faces[6].v0 = 6;
		faces[6].v1 = 7;
		faces[6].v2 = 8;

		faces[7].v0 = 7;
		faces[7].v1 = 9;
		faces[7].v2 = 8;

		faces[8].v0 = 8;
		faces[8].v1 = 9;
		faces[8].v2 = 10;

		faces[9].v0 = 9;
		faces[9].v1 = 11;
		faces[9].v2 = 10;

		faces[10].v0 = 10;
		faces[10].v1 = 11;
		faces[10].v2 = 12;

		faces[11].v0 = 11;
		faces[11].v1 = 13;
		faces[11].v2 = 12;

		faces[12].v0 = 12;
		faces[12].v1 = 13;
		faces[12].v2 = 14;

		faces[13].v0 = 13;
		faces[13].v1 = 15;
		faces[13].v2 = 14;

		faces[14].v0 = 14;
		faces[14].v1 = 15;
		faces[14].v2 = 16;

		faces[15].v0 = 15;
		faces[15].v1 = 17;
		faces[15].v2 = 16;

		faces[16].v0 = 16;
		faces[16].v1 = 17;
		faces[16].v2 = 18;

		faces[17].v0 = 17;
		faces[17].v1 = 19;
		faces[17].v2 = 18;

		faces[18].v0 = 18;
		faces[18].v1 = 19;
		faces[18].v2 = 20;

		faces[19].v0 = 19;
		faces[19].v1 = 21;
		faces[19].v2 = 20;

		faces[20].v0 = 1;
		faces[20].v1 = 22;
		faces[20].v2 = 3;

		faces[21].v0 = 22;
		faces[21].v1 = 23;
		faces[21].v2 = 3;

		faces[22].v0 = 3;
		faces[22].v1 = 23;
		faces[22].v2 = 5;

		faces[23].v0 = 23;
		faces[23].v1 = 24;
		faces[23].v2 = 5;

		faces[24].v0 = 5;
		faces[24].v1 = 24;
		faces[24].v2 = 7;

		faces[25].v0 = 24;
		faces[25].v1 = 25;
		faces[25].v2 = 7;

		faces[26].v0 = 7;
		faces[26].v1 = 25;
		faces[26].v2 = 9;

		faces[27].v0 = 25;
		faces[27].v1 = 26;
		faces[27].v2 = 9;

		faces[28].v0 = 9;
		faces[28].v1 = 26;
		faces[28].v2 = 11;

		faces[29].v0 = 26;
		faces[29].v1 = 27;
		faces[29].v2 = 11;

		faces[30].v0 = 11;
		faces[30].v1 = 27;
		faces[30].v2 = 13;

		faces[31].v0 = 27;
		faces[31].v1 = 28;
		faces[31].v2 = 13;

		faces[32].v0 = 13;
		faces[32].v1 = 28;
		faces[32].v2 = 15;

		faces[33].v0 = 28;
		faces[33].v1 = 29;
		faces[33].v2 = 15;

		faces[34].v0 = 15;
		faces[34].v1 = 29;
		faces[34].v2 = 17;

		faces[35].v0 = 29;
		faces[35].v1 = 30;
		faces[35].v2 = 17;

		faces[36].v0 = 17;
		faces[36].v1 = 30;
		faces[36].v2 = 19;

		faces[37].v0 = 30;
		faces[37].v1 = 31;
		faces[37].v2 = 19;

		faces[38].v0 = 19;
		faces[38].v1 = 31;
		faces[38].v2 = 21;

		faces[39].v0 = 31;
		faces[39].v1 = 32;
		faces[39].v2 = 21;

		faces[40].v0 = 22;
		faces[40].v1 = 33;
		faces[40].v2 = 23;

		faces[41].v0 = 33;
		faces[41].v1 = 34;
		faces[41].v2 = 23;

		faces[42].v0 = 23;
		faces[42].v1 = 34;
		faces[42].v2 = 24;

		faces[43].v0 = 34;
		faces[43].v1 = 35;
		faces[43].v2 = 24;

		faces[44].v0 = 24;
		faces[44].v1 = 35;
		faces[44].v2 = 25;

		faces[45].v0 = 35;
		faces[45].v1 = 36;
		faces[45].v2 = 25;

		faces[46].v0 = 25;
		faces[46].v1 = 36;
		faces[46].v2 = 26;

		faces[47].v0 = 36;
		faces[47].v1 = 37;
		faces[47].v2 = 26;

		faces[48].v0 = 26;
		faces[48].v1 = 37;
		faces[48].v2 = 27;

		faces[49].v0 = 37;
		faces[49].v1 = 38;
		faces[49].v2 = 27;

		faces[50].v0 = 27;
		faces[50].v1 = 38;
		faces[50].v2 = 28;

		faces[51].v0 = 38;
		faces[51].v1 = 39;
		faces[51].v2 = 28;

		faces[52].v0 = 28;
		faces[52].v1 = 39;
		faces[52].v2 = 29;

		faces[53].v0 = 39;
		faces[53].v1 = 40;
		faces[53].v2 = 29;

		faces[54].v0 = 29;
		faces[54].v1 = 40;
		faces[54].v2 = 30;

		faces[55].v0 = 40;
		faces[55].v1 = 41;
		faces[55].v2 = 30;

		faces[56].v0 = 30;
		faces[56].v1 = 41;
		faces[56].v2 = 31;

		faces[57].v0 = 41;
		faces[57].v1 = 42;
		faces[57].v2 = 31;

		faces[58].v0 = 31;
		faces[58].v1 = 42;
		faces[58].v2 = 32;

		faces[59].v0 = 42;
		faces[59].v1 = 43;
		faces[59].v2 = 32;

		faces[60].v0 = 33;
		faces[60].v1 = 44;
		faces[60].v2 = 34;

		faces[61].v0 = 44;
		faces[61].v1 = 45;
		faces[61].v2 = 34;

		faces[62].v0 = 34;
		faces[62].v1 = 45;
		faces[62].v2 = 35;

		faces[63].v0 = 45;
		faces[63].v1 = 46;
		faces[63].v2 = 35;

		faces[64].v0 = 35;
		faces[64].v1 = 46;
		faces[64].v2 = 36;

		faces[65].v0 = 46;
		faces[65].v1 = 47;
		faces[65].v2 = 36;

		faces[66].v0 = 36;
		faces[66].v1 = 47;
		faces[66].v2 = 37;

		faces[67].v0 = 47;
		faces[67].v1 = 48;
		faces[67].v2 = 37;

		faces[68].v0 = 37;
		faces[68].v1 = 48;
		faces[68].v2 = 38;

		faces[69].v0 = 48;
		faces[69].v1 = 49;
		faces[69].v2 = 38;

		faces[70].v0 = 38;
		faces[70].v1 = 49;
		faces[70].v2 = 39;

		faces[71].v0 = 49;
		faces[71].v1 = 50;
		faces[71].v2 = 39;

		faces[72].v0 = 39;
		faces[72].v1 = 50;
		faces[72].v2 = 40;

		faces[73].v0 = 50;
		faces[73].v1 = 51;
		faces[73].v2 = 40;

		faces[74].v0 = 40;
		faces[74].v1 = 51;
		faces[74].v2 = 41;

		faces[75].v0 = 51;
		faces[75].v1 = 52;
		faces[75].v2 = 41;

		faces[76].v0 = 41;
		faces[76].v1 = 52;
		faces[76].v2 = 42;

		faces[77].v0 = 52;
		faces[77].v1 = 53;
		faces[77].v2 = 42;

		faces[78].v0 = 42;
		faces[78].v1 = 53;
		faces[78].v2 = 43;

		faces[79].v0 = 53;
		faces[79].v1 = 54;
		faces[79].v2 = 43;

		faces[80].v0 = 44;
		faces[80].v1 = 55;
		faces[80].v2 = 45;

		faces[81].v0 = 55;
		faces[81].v1 = 56;
		faces[81].v2 = 45;

		faces[82].v0 = 45;
		faces[82].v1 = 56;
		faces[82].v2 = 46;

		faces[83].v0 = 56;
		faces[83].v1 = 57;
		faces[83].v2 = 46;

		faces[84].v0 = 46;
		faces[84].v1 = 57;
		faces[84].v2 = 47;

		faces[85].v0 = 57;
		faces[85].v1 = 58;
		faces[85].v2 = 47;

		faces[86].v0 = 47;
		faces[86].v1 = 58;
		faces[86].v2 = 48;

		faces[87].v0 = 58;
		faces[87].v1 = 59;
		faces[87].v2 = 48;

		faces[88].v0 = 48;
		faces[88].v1 = 59;
		faces[88].v2 = 49;

		faces[89].v0 = 59;
		faces[89].v1 = 60;
		faces[89].v2 = 49;

		faces[90].v0 = 49;
		faces[90].v1 = 60;
		faces[90].v2 = 50;

		faces[91].v0 = 60;
		faces[91].v1 = 61;
		faces[91].v2 = 50;

		faces[92].v0 = 50;
		faces[92].v1 = 61;
		faces[92].v2 = 51;

		faces[93].v0 = 61;
		faces[93].v1 = 62;
		faces[93].v2 = 51;

		faces[94].v0 = 51;
		faces[94].v1 = 62;
		faces[94].v2 = 52;

		faces[95].v0 = 62;
		faces[95].v1 = 63;
		faces[95].v2 = 52;

		faces[96].v0 = 52;
		faces[96].v1 = 63;
		faces[96].v2 = 53;

		faces[97].v0 = 63;
		faces[97].v1 = 64;
		faces[97].v2 = 53;

		faces[98].v0 = 53;
		faces[98].v1 = 64;
		faces[98].v2 = 54;

		faces[99].v0 = 64;
		faces[99].v1 = 65;
		faces[99].v2 = 54;

		faces[100].v0 = 55;
		faces[100].v1 = 66;
		faces[100].v2 = 56;

		faces[101].v0 = 66;
		faces[101].v1 = 67;
		faces[101].v2 = 56;

		faces[102].v0 = 56;
		faces[102].v1 = 67;
		faces[102].v2 = 57;

		faces[103].v0 = 67;
		faces[103].v1 = 68;
		faces[103].v2 = 57;

		faces[104].v0 = 57;
		faces[104].v1 = 68;
		faces[104].v2 = 58;

		faces[105].v0 = 68;
		faces[105].v1 = 69;
		faces[105].v2 = 58;

		faces[106].v0 = 58;
		faces[106].v1 = 69;
		faces[106].v2 = 59;

		faces[107].v0 = 69;
		faces[107].v1 = 70;
		faces[107].v2 = 59;

		faces[108].v0 = 59;
		faces[108].v1 = 70;
		faces[108].v2 = 60;

		faces[109].v0 = 70;
		faces[109].v1 = 71;
		faces[109].v2 = 60;

		faces[110].v0 = 60;
		faces[110].v1 = 71;
		faces[110].v2 = 61;

		faces[111].v0 = 71;
		faces[111].v1 = 72;
		faces[111].v2 = 61;

		faces[112].v0 = 61;
		faces[112].v1 = 72;
		faces[112].v2 = 62;

		faces[113].v0 = 72;
		faces[113].v1 = 73;
		faces[113].v2 = 62;

		faces[114].v0 = 62;
		faces[114].v1 = 73;
		faces[114].v2 = 63;

		faces[115].v0 = 73;
		faces[115].v1 = 74;
		faces[115].v2 = 63;

		faces[116].v0 = 63;
		faces[116].v1 = 74;
		faces[116].v2 = 64;

		faces[117].v0 = 74;
		faces[117].v1 = 75;
		faces[117].v2 = 64;

		faces[118].v0 = 64;
		faces[118].v1 = 75;
		faces[118].v2 = 65;

		faces[119].v0 = 75;
		faces[119].v1 = 76;
		faces[119].v2 = 65;

		faces[120].v0 = 66;
		faces[120].v1 = 77;
		faces[120].v2 = 67;

		faces[121].v0 = 77;
		faces[121].v1 = 78;
		faces[121].v2 = 67;

		faces[122].v0 = 67;
		faces[122].v1 = 78;
		faces[122].v2 = 68;

		faces[123].v0 = 78;
		faces[123].v1 = 79;
		faces[123].v2 = 68;

		faces[124].v0 = 68;
		faces[124].v1 = 79;
		faces[124].v2 = 69;

		faces[125].v0 = 79;
		faces[125].v1 = 80;
		faces[125].v2 = 69;

		faces[126].v0 = 69;
		faces[126].v1 = 80;
		faces[126].v2 = 70;

		faces[127].v0 = 80;
		faces[127].v1 = 81;
		faces[127].v2 = 70;

		faces[128].v0 = 70;
		faces[128].v1 = 81;
		faces[128].v2 = 71;

		faces[129].v0 = 81;
		faces[129].v1 = 82;
		faces[129].v2 = 71;

		faces[130].v0 = 71;
		faces[130].v1 = 82;
		faces[130].v2 = 72;

		faces[131].v0 = 82;
		faces[131].v1 = 83;
		faces[131].v2 = 72;

		faces[132].v0 = 72;
		faces[132].v1 = 83;
		faces[132].v2 = 73;

		faces[133].v0 = 83;
		faces[133].v1 = 84;
		faces[133].v2 = 73;

		faces[134].v0 = 73;
		faces[134].v1 = 84;
		faces[134].v2 = 74;

		faces[135].v0 = 84;
		faces[135].v1 = 85;
		faces[135].v2 = 74;

		faces[136].v0 = 74;
		faces[136].v1 = 85;
		faces[136].v2 = 75;

		faces[137].v0 = 85;
		faces[137].v1 = 86;
		faces[137].v2 = 75;

		faces[138].v0 = 75;
		faces[138].v1 = 86;
		faces[138].v2 = 76;

		faces[139].v0 = 86;
		faces[139].v1 = 87;
		faces[139].v2 = 76;

		faces[140].v0 = 77;
		faces[140].v1 = 88;
		faces[140].v2 = 78;

		faces[141].v0 = 88;
		faces[141].v1 = 89;
		faces[141].v2 = 78;

		faces[142].v0 = 78;
		faces[142].v1 = 89;
		faces[142].v2 = 79;

		faces[143].v0 = 89;
		faces[143].v1 = 90;
		faces[143].v2 = 79;

		faces[144].v0 = 79;
		faces[144].v1 = 90;
		faces[144].v2 = 80;

		faces[145].v0 = 90;
		faces[145].v1 = 91;
		faces[145].v2 = 80;

		faces[146].v0 = 80;
		faces[146].v1 = 91;
		faces[146].v2 = 81;

		faces[147].v0 = 91;
		faces[147].v1 = 92;
		faces[147].v2 = 81;

		faces[148].v0 = 81;
		faces[148].v1 = 92;
		faces[148].v2 = 82;

		faces[149].v0 = 92;
		faces[149].v1 = 93;
		faces[149].v2 = 82;

		faces[150].v0 = 82;
		faces[150].v1 = 93;
		faces[150].v2 = 83;

		faces[151].v0 = 93;
		faces[151].v1 = 94;
		faces[151].v2 = 83;

		faces[152].v0 = 83;
		faces[152].v1 = 94;
		faces[152].v2 = 84;

		faces[153].v0 = 94;
		faces[153].v1 = 95;
		faces[153].v2 = 84;

		faces[154].v0 = 84;
		faces[154].v1 = 95;
		faces[154].v2 = 85;

		faces[155].v0 = 95;
		faces[155].v1 = 96;
		faces[155].v2 = 85;

		faces[156].v0 = 85;
		faces[156].v1 = 96;
		faces[156].v2 = 86;

		faces[157].v0 = 96;
		faces[157].v1 = 97;
		faces[157].v2 = 86;

		faces[158].v0 = 86;
		faces[158].v1 = 97;
		faces[158].v2 = 87;

		faces[159].v0 = 97;
		faces[159].v1 = 98;
		faces[159].v2 = 87;

		faces[160].v0 = 88;
		faces[160].v1 = 99;
		faces[160].v2 = 89;

		faces[161].v0 = 99;
		faces[161].v1 = 100;
		faces[161].v2 = 89;

		faces[162].v0 = 89;
		faces[162].v1 = 100;
		faces[162].v2 = 90;

		faces[163].v0 = 100;
		faces[163].v1 = 101;
		faces[163].v2 = 90;

		faces[164].v0 = 90;
		faces[164].v1 = 101;
		faces[164].v2 = 91;

		faces[165].v0 = 101;
		faces[165].v1 = 102;
		faces[165].v2 = 91;

		faces[166].v0 = 91;
		faces[166].v1 = 102;
		faces[166].v2 = 92;

		faces[167].v0 = 102;
		faces[167].v1 = 103;
		faces[167].v2 = 92;

		faces[168].v0 = 92;
		faces[168].v1 = 103;
		faces[168].v2 = 93;

		faces[169].v0 = 103;
		faces[169].v1 = 104;
		faces[169].v2 = 93;

		faces[170].v0 = 93;
		faces[170].v1 = 104;
		faces[170].v2 = 94;

		faces[171].v0 = 104;
		faces[171].v1 = 105;
		faces[171].v2 = 94;

		faces[172].v0 = 94;
		faces[172].v1 = 105;
		faces[172].v2 = 95;

		faces[173].v0 = 105;
		faces[173].v1 = 106;
		faces[173].v2 = 95;

		faces[174].v0 = 95;
		faces[174].v1 = 106;
		faces[174].v2 = 96;

		faces[175].v0 = 106;
		faces[175].v1 = 107;
		faces[175].v2 = 96;

		faces[176].v0 = 96;
		faces[176].v1 = 107;
		faces[176].v2 = 97;

		faces[177].v0 = 107;
		faces[177].v1 = 108;
		faces[177].v2 = 97;

		faces[178].v0 = 97;
		faces[178].v1 = 108;
		faces[178].v2 = 98;

		faces[179].v0 = 108;
		faces[179].v1 = 109;
		faces[179].v2 = 98;

		faces[180].v0 = 99;
		faces[180].v1 = 110;
		faces[180].v2 = 100;

		faces[181].v0 = 110;
		faces[181].v1 = 111;
		faces[181].v2 = 100;

		faces[182].v0 = 100;
		faces[182].v1 = 111;
		faces[182].v2 = 101;

		faces[183].v0 = 111;
		faces[183].v1 = 112;
		faces[183].v2 = 101;

		faces[184].v0 = 101;
		faces[184].v1 = 112;
		faces[184].v2 = 102;

		faces[185].v0 = 112;
		faces[185].v1 = 113;
		faces[185].v2 = 102;

		faces[186].v0 = 102;
		faces[186].v1 = 113;
		faces[186].v2 = 103;

		faces[187].v0 = 113;
		faces[187].v1 = 114;
		faces[187].v2 = 103;

		faces[188].v0 = 103;
		faces[188].v1 = 114;
		faces[188].v2 = 104;

		faces[189].v0 = 114;
		faces[189].v1 = 115;
		faces[189].v2 = 104;

		faces[190].v0 = 104;
		faces[190].v1 = 115;
		faces[190].v2 = 105;

		faces[191].v0 = 115;
		faces[191].v1 = 116;
		faces[191].v2 = 105;

		faces[192].v0 = 105;
		faces[192].v1 = 116;
		faces[192].v2 = 106;

		faces[193].v0 = 116;
		faces[193].v1 = 117;
		faces[193].v2 = 106;

		faces[194].v0 = 106;
		faces[194].v1 = 117;
		faces[194].v2 = 107;

		faces[195].v0 = 117;
		faces[195].v1 = 118;
		faces[195].v2 = 107;

		faces[196].v0 = 107;
		faces[196].v1 = 118;
		faces[196].v2 = 108;

		faces[197].v0 = 118;
		faces[197].v1 = 119;
		faces[197].v2 = 108;

		faces[198].v0 = 108;
		faces[198].v1 = 119;
		faces[198].v2 = 109;

		faces[199].v0 = 119;
		faces[199].v1 = 120;
		faces[199].v2 = 109;
	}

	// Vertices
	{
		vertices[0].x = -0.000000f;
		vertices[0].y = 0.000000f;
		vertices[0].z = 0.500000f;
		vertices[0].nx = -0.000000f;
		vertices[0].ny = 0.000000f;
		vertices[0].nz = 1.000000f;
		vertices[0].u = 0.000000f;
		vertices[0].v = 1.000000f;

		vertices[1].x = -0.000000f;
		vertices[1].y = 0.154509f;
		vertices[1].z = 0.475528f;
		vertices[1].nx = -0.000000f;
		vertices[1].ny = 0.309017f;
		vertices[1].nz = 0.951057f;
		vertices[1].u = 0.000000f;
		vertices[1].v = 0.900000f;

		vertices[2].x = -0.000000f;
		vertices[2].y = 0.000000f;
		vertices[2].z = 0.500000f;
		vertices[2].nx = -0.000000f;
		vertices[2].ny = 0.000000f;
		vertices[2].nz = 1.000000f;
		vertices[2].u = 0.100000f;
		vertices[2].v = 1.000000f;

		vertices[3].x = -0.090818f;
		vertices[3].y = 0.125000f;
		vertices[3].z = 0.475528f;
		vertices[3].nx = -0.181636f;
		vertices[3].ny = 0.250000f;
		vertices[3].nz = 0.951057f;
		vertices[3].u = 0.100000f;
		vertices[3].v = 0.900000f;

		vertices[4].x = -0.000000f;
		vertices[4].y = 0.000000f;
		vertices[4].z = 0.500000f;
		vertices[4].nx = -0.000000f;
		vertices[4].ny = 0.000000f;
		vertices[4].nz = 1.000000f;
		vertices[4].u = 0.200000f;
		vertices[4].v = 1.000000f;

		vertices[5].x = -0.146946f;
		vertices[5].y = 0.047746f;
		vertices[5].z = 0.475528f;
		vertices[5].nx = -0.293893f;
		vertices[5].ny = 0.095491f;
		vertices[5].nz = 0.951057f;
		vertices[5].u = 0.200000f;
		vertices[5].v = 0.900000f;

		vertices[6].x = -0.000000f;
		vertices[6].y = -0.000000f;
		vertices[6].z = 0.500000f;
		vertices[6].nx = -0.000000f;
		vertices[6].ny = -0.000000f;
		vertices[6].nz = 1.000000f;
		vertices[6].u = 0.300000f;
		vertices[6].v = 1.000000f;

		vertices[7].x = -0.146946f;
		vertices[7].y = -0.047746f;
		vertices[7].z = 0.475528f;
		vertices[7].nx = -0.293893f;
		vertices[7].ny = -0.095492f;
		vertices[7].nz = 0.951057f;
		vertices[7].u = 0.300000f;
		vertices[7].v = 0.900000f;

		vertices[8].x = -0.000000f;
		vertices[8].y = -0.000000f;
		vertices[8].z = 0.500000f;
		vertices[8].nx = -0.000000f;
		vertices[8].ny = -0.000000f;
		vertices[8].nz = 1.000000f;
		vertices[8].u = 0.400000f;
		vertices[8].v = 1.000000f;

		vertices[9].x = -0.090818f;
		vertices[9].y = -0.125000f;
		vertices[9].z = 0.475528f;
		vertices[9].nx = -0.181636f;
		vertices[9].ny = -0.250000f;
		vertices[9].nz = 0.951057f;
		vertices[9].u = 0.400000f;
		vertices[9].v = 0.900000f;

		vertices[10].x = 0.000000f;
		vertices[10].y = -0.000000f;
		vertices[10].z = 0.500000f;
		vertices[10].nx = 0.000000f;
		vertices[10].ny = -0.000000f;
		vertices[10].nz = 1.000000f;
		vertices[10].u = 0.500000f;
		vertices[10].v = 1.000000f;

		vertices[11].x = 0.000000f;
		vertices[11].y = -0.154509f;
		vertices[11].z = 0.475528f;
		vertices[11].nx = 0.000000f;
		vertices[11].ny = -0.309017f;
		vertices[11].nz = 0.951057f;
		vertices[11].u = 0.500000f;
		vertices[11].v = 0.900000f;

		vertices[12].x = 0.000000f;
		vertices[12].y = -0.000000f;
		vertices[12].z = 0.500000f;
		vertices[12].nx = 0.000000f;
		vertices[12].ny = -0.000000f;
		vertices[12].nz = 1.000000f;
		vertices[12].u = 0.600000f;
		vertices[12].v = 1.000000f;

		vertices[13].x = 0.090818f;
		vertices[13].y = -0.125000f;
		vertices[13].z = 0.475528f;
		vertices[13].nx = 0.181636f;
		vertices[13].ny = -0.250000f;
		vertices[13].nz = 0.951057f;
		vertices[13].u = 0.600000f;
		vertices[13].v = 0.900000f;

		vertices[14].x = 0.000000f;
		vertices[14].y = -0.000000f;
		vertices[14].z = 0.500000f;
		vertices[14].nx = 0.000000f;
		vertices[14].ny = -0.000000f;
		vertices[14].nz = 1.000000f;
		vertices[14].u = 0.700000f;
		vertices[14].v = 1.000000f;

		vertices[15].x = 0.146946f;
		vertices[15].y = -0.047746f;
		vertices[15].z = 0.475528f;
		vertices[15].nx = 0.293893f;
		vertices[15].ny = -0.095492f;
		vertices[15].nz = 0.951057f;
		vertices[15].u = 0.700000f;
		vertices[15].v = 0.900000f;

		vertices[16].x = 0.000000f;
		vertices[16].y = 0.000000f;
		vertices[16].z = 0.500000f;
		vertices[16].nx = 0.000000f;
		vertices[16].ny = 0.000000f;
		vertices[16].nz = 1.000000f;
		vertices[16].u = 0.800000f;
		vertices[16].v = 1.000000f;

		vertices[17].x = 0.146946f;
		vertices[17].y = 0.047746f;
		vertices[17].z = 0.475528f;
		vertices[17].nx = 0.293893f;
		vertices[17].ny = 0.095492f;
		vertices[17].nz = 0.951057f;
		vertices[17].u = 0.800000f;
		vertices[17].v = 0.900000f;

		vertices[18].x = 0.000000f;
		vertices[18].y = 0.000000f;
		vertices[18].z = 0.500000f;
		vertices[18].nx = 0.000000f;
		vertices[18].ny = 0.000000f;
		vertices[18].nz = 1.000000f;
		vertices[18].u = 0.900000f;
		vertices[18].v = 1.000000f;

		vertices[19].x = 0.090818f;
		vertices[19].y = 0.125000f;
		vertices[19].z = 0.475528f;
		vertices[19].nx = 0.181636f;
		vertices[19].ny = 0.250000f;
		vertices[19].nz = 0.951057f;
		vertices[19].u = 0.900000f;
		vertices[19].v = 0.900000f;

		vertices[20].x = -0.000000f;
		vertices[20].y = 0.000000f;
		vertices[20].z = 0.500000f;
		vertices[20].nx = -0.000000f;
		vertices[20].ny = 0.000000f;
		vertices[20].nz = 1.000000f;
		vertices[20].u = 1.000000f;
		vertices[20].v = 1.000000f;

		vertices[21].x = -0.000000f;
		vertices[21].y = 0.154509f;
		vertices[21].z = 0.475528f;
		vertices[21].nx = -0.000000f;
		vertices[21].ny = 0.309017f;
		vertices[21].nz = 0.951057f;
		vertices[21].u = 1.000000f;
		vertices[21].v = 0.900000f;

		vertices[22].x = -0.000000f;
		vertices[22].y = 0.293893f;
		vertices[22].z = 0.404509f;
		vertices[22].nx = -0.000000f;
		vertices[22].ny = 0.587785f;
		vertices[22].nz = 0.809017f;
		vertices[22].u = 0.000000f;
		vertices[22].v = 0.800000f;

		vertices[23].x = -0.172746f;
		vertices[23].y = 0.237764f;
		vertices[23].z = 0.404509f;
		vertices[23].nx = -0.345491f;
		vertices[23].ny = 0.475528f;
		vertices[23].nz = 0.809017f;
		vertices[23].u = 0.100000f;
		vertices[23].v = 0.800000f;

		vertices[24].x = -0.279509f;
		vertices[24].y = 0.090818f;
		vertices[24].z = 0.404509f;
		vertices[24].nx = -0.559017f;
		vertices[24].ny = 0.181636f;
		vertices[24].nz = 0.809017f;
		vertices[24].u = 0.200000f;
		vertices[24].v = 0.800000f;

		vertices[25].x = -0.279508f;
		vertices[25].y = -0.090818f;
		vertices[25].z = 0.404509f;
		vertices[25].nx = -0.559017f;
		vertices[25].ny = -0.181636f;
		vertices[25].nz = 0.809017f;
		vertices[25].u = 0.300000f;
		vertices[25].v = 0.800000f;

		vertices[26].x = -0.172746f;
		vertices[26].y = -0.237764f;
		vertices[26].z = 0.404509f;
		vertices[26].nx = -0.345491f;
		vertices[26].ny = -0.475528f;
		vertices[26].nz = 0.809017f;
		vertices[26].u = 0.400000f;
		vertices[26].v = 0.800000f;

		vertices[27].x = 0.000000f;
		vertices[27].y = -0.293893f;
		vertices[27].z = 0.404509f;
		vertices[27].nx = 0.000000f;
		vertices[27].ny = -0.587785f;
		vertices[27].nz = 0.809017f;
		vertices[27].u = 0.500000f;
		vertices[27].v = 0.800000f;

		vertices[28].x = 0.172746f;
		vertices[28].y = -0.237764f;
		vertices[28].z = 0.404509f;
		vertices[28].nx = 0.345492f;
		vertices[28].ny = -0.475528f;
		vertices[28].nz = 0.809017f;
		vertices[28].u = 0.600000f;
		vertices[28].v = 0.800000f;

		vertices[29].x = 0.279508f;
		vertices[29].y = -0.090818f;
		vertices[29].z = 0.404509f;
		vertices[29].nx = 0.559017f;
		vertices[29].ny = -0.181636f;
		vertices[29].nz = 0.809017f;
		vertices[29].u = 0.700000f;
		vertices[29].v = 0.800000f;

		vertices[30].x = 0.279508f;
		vertices[30].y = 0.090818f;
		vertices[30].z = 0.404509f;
		vertices[30].nx = 0.559017f;
		vertices[30].ny = 0.181636f;
		vertices[30].nz = 0.809017f;
		vertices[30].u = 0.800000f;
		vertices[30].v = 0.800000f;

		vertices[31].x = 0.172746f;
		vertices[31].y = 0.237764f;
		vertices[31].z = 0.404509f;
		vertices[31].nx = 0.345491f;
		vertices[31].ny = 0.475528f;
		vertices[31].nz = 0.809017f;
		vertices[31].u = 0.900000f;
		vertices[31].v = 0.800000f;

		vertices[32].x = -0.000000f;
		vertices[32].y = 0.293893f;
		vertices[32].z = 0.404509f;
		vertices[32].nx = -0.000000f;
		vertices[32].ny = 0.587785f;
		vertices[32].nz = 0.809017f;
		vertices[32].u = 1.000000f;
		vertices[32].v = 0.800000f;

		vertices[33].x = -0.000000f;
		vertices[33].y = 0.404509f;
		vertices[33].z = 0.293893f;
		vertices[33].nx = -0.000000f;
		vertices[33].ny = 0.809017f;
		vertices[33].nz = 0.587785f;
		vertices[33].u = 0.000000f;
		vertices[33].v = 0.700000f;

		vertices[34].x = -0.237764f;
		vertices[34].y = 0.327254f;
		vertices[34].z = 0.293893f;
		vertices[34].nx = -0.475528f;
		vertices[34].ny = 0.654509f;
		vertices[34].nz = 0.587785f;
		vertices[34].u = 0.100000f;
		vertices[34].v = 0.700000f;

		vertices[35].x = -0.384710f;
		vertices[35].y = 0.125000f;
		vertices[35].z = 0.293893f;
		vertices[35].nx = -0.769421f;
		vertices[35].ny = 0.250000f;
		vertices[35].nz = 0.587785f;
		vertices[35].u = 0.200000f;
		vertices[35].v = 0.700000f;

		vertices[36].x = -0.384710f;
		vertices[36].y = -0.125000f;
		vertices[36].z = 0.293893f;
		vertices[36].nx = -0.769421f;
		vertices[36].ny = -0.250000f;
		vertices[36].nz = 0.587785f;
		vertices[36].u = 0.300000f;
		vertices[36].v = 0.700000f;

		vertices[37].x = -0.237764f;
		vertices[37].y = -0.327254f;
		vertices[37].z = 0.293893f;
		vertices[37].nx = -0.475528f;
		vertices[37].ny = -0.654509f;
		vertices[37].nz = 0.587785f;
		vertices[37].u = 0.400000f;
		vertices[37].v = 0.700000f;

		vertices[38].x = 0.000000f;
		vertices[38].y = -0.404509f;
		vertices[38].z = 0.293893f;
		vertices[38].nx = 0.000000f;
		vertices[38].ny = -0.809017f;
		vertices[38].nz = 0.587785f;
		vertices[38].u = 0.500000f;
		vertices[38].v = 0.700000f;

		vertices[39].x = 0.237764f;
		vertices[39].y = -0.327254f;
		vertices[39].z = 0.293893f;
		vertices[39].nx = 0.475528f;
		vertices[39].ny = -0.654508f;
		vertices[39].nz = 0.587785f;
		vertices[39].u = 0.600000f;
		vertices[39].v = 0.700000f;

		vertices[40].x = 0.384710f;
		vertices[40].y = -0.125000f;
		vertices[40].z = 0.293893f;
		vertices[40].nx = 0.769421f;
		vertices[40].ny = -0.250000f;
		vertices[40].nz = 0.587785f;
		vertices[40].u = 0.700000f;
		vertices[40].v = 0.700000f;

		vertices[41].x = 0.384710f;
		vertices[41].y = 0.125000f;
		vertices[41].z = 0.293893f;
		vertices[41].nx = 0.769421f;
		vertices[41].ny = 0.250000f;
		vertices[41].nz = 0.587785f;
		vertices[41].u = 0.800000f;
		vertices[41].v = 0.700000f;

		vertices[42].x = 0.237764f;
		vertices[42].y = 0.327254f;
		vertices[42].z = 0.293893f;
		vertices[42].nx = 0.475528f;
		vertices[42].ny = 0.654509f;
		vertices[42].nz = 0.587785f;
		vertices[42].u = 0.900000f;
		vertices[42].v = 0.700000f;

		vertices[43].x = -0.000000f;
		vertices[43].y = 0.404509f;
		vertices[43].z = 0.293893f;
		vertices[43].nx = -0.000000f;
		vertices[43].ny = 0.809017f;
		vertices[43].nz = 0.587785f;
		vertices[43].u = 1.000000f;
		vertices[43].v = 0.700000f;

		vertices[44].x = -0.000000f;
		vertices[44].y = 0.475528f;
		vertices[44].z = 0.154508f;
		vertices[44].nx = -0.000000f;
		vertices[44].ny = 0.951057f;
		vertices[44].nz = 0.309017f;
		vertices[44].u = 0.000000f;
		vertices[44].v = 0.600000f;

		vertices[45].x = -0.279509f;
		vertices[45].y = 0.384710f;
		vertices[45].z = 0.154508f;
		vertices[45].nx = -0.559017f;
		vertices[45].ny = 0.769421f;
		vertices[45].nz = 0.309017f;
		vertices[45].u = 0.100000f;
		vertices[45].v = 0.600000f;

		vertices[46].x = -0.452254f;
		vertices[46].y = 0.146946f;
		vertices[46].z = 0.154508f;
		vertices[46].nx = -0.904509f;
		vertices[46].ny = 0.293893f;
		vertices[46].nz = 0.309017f;
		vertices[46].u = 0.200000f;
		vertices[46].v = 0.600000f;

		vertices[47].x = -0.452254f;
		vertices[47].y = -0.146946f;
		vertices[47].z = 0.154508f;
		vertices[47].nx = -0.904508f;
		vertices[47].ny = -0.293893f;
		vertices[47].nz = 0.309017f;
		vertices[47].u = 0.300000f;
		vertices[47].v = 0.600000f;

		vertices[48].x = -0.279508f;
		vertices[48].y = -0.384710f;
		vertices[48].z = 0.154508f;
		vertices[48].nx = -0.559017f;
		vertices[48].ny = -0.769421f;
		vertices[48].nz = 0.309017f;
		vertices[48].u = 0.400000f;
		vertices[48].v = 0.600000f;

		vertices[49].x = 0.000000f;
		vertices[49].y = -0.475528f;
		vertices[49].z = 0.154508f;
		vertices[49].nx = 0.000000f;
		vertices[49].ny = -0.951057f;
		vertices[49].nz = 0.309017f;
		vertices[49].u = 0.500000f;
		vertices[49].v = 0.600000f;

		vertices[50].x = 0.279509f;
		vertices[50].y = -0.384710f;
		vertices[50].z = 0.154508f;
		vertices[50].nx = 0.559017f;
		vertices[50].ny = -0.769421f;
		vertices[50].nz = 0.309017f;
		vertices[50].u = 0.600000f;
		vertices[50].v = 0.600000f;

		vertices[51].x = 0.452254f;
		vertices[51].y = -0.146946f;
		vertices[51].z = 0.154508f;
		vertices[51].nx = 0.904508f;
		vertices[51].ny = -0.293893f;
		vertices[51].nz = 0.309017f;
		vertices[51].u = 0.700000f;
		vertices[51].v = 0.600000f;

		vertices[52].x = 0.452254f;
		vertices[52].y = 0.146946f;
		vertices[52].z = 0.154508f;
		vertices[52].nx = 0.904508f;
		vertices[52].ny = 0.293893f;
		vertices[52].nz = 0.309017f;
		vertices[52].u = 0.800000f;
		vertices[52].v = 0.600000f;

		vertices[53].x = 0.279508f;
		vertices[53].y = 0.384711f;
		vertices[53].z = 0.154508f;
		vertices[53].nx = 0.559017f;
		vertices[53].ny = 0.769421f;
		vertices[53].nz = 0.309017f;
		vertices[53].u = 0.900000f;
		vertices[53].v = 0.600000f;

		vertices[54].x = -0.000000f;
		vertices[54].y = 0.475528f;
		vertices[54].z = 0.154508f;
		vertices[54].nx = -0.000000f;
		vertices[54].ny = 0.951057f;
		vertices[54].nz = 0.309017f;
		vertices[54].u = 1.000000f;
		vertices[54].v = 0.600000f;

		vertices[55].x = -0.000000f;
		vertices[55].y = 0.500000f;
		vertices[55].z = -0.000000f;
		vertices[55].nx = -0.000000f;
		vertices[55].ny = 1.000000f;
		vertices[55].nz = -0.000000f;
		vertices[55].u = 0.000000f;
		vertices[55].v = 0.500000f;

		vertices[56].x = -0.293893f;
		vertices[56].y = 0.404509f;
		vertices[56].z = -0.000000f;
		vertices[56].nx = -0.587785f;
		vertices[56].ny = 0.809017f;
		vertices[56].nz = -0.000000f;
		vertices[56].u = 0.100000f;
		vertices[56].v = 0.500000f;

		vertices[57].x = -0.475528f;
		vertices[57].y = 0.154508f;
		vertices[57].z = -0.000000f;
		vertices[57].nx = -0.951057f;
		vertices[57].ny = 0.309017f;
		vertices[57].nz = -0.000000f;
		vertices[57].u = 0.200000f;
		vertices[57].v = 0.500000f;

		vertices[58].x = -0.475528f;
		vertices[58].y = -0.154509f;
		vertices[58].z = -0.000000f;
		vertices[58].nx = -0.951057f;
		vertices[58].ny = -0.309017f;
		vertices[58].nz = -0.000000f;
		vertices[58].u = 0.300000f;
		vertices[58].v = 0.500000f;

		vertices[59].x = -0.293893f;
		vertices[59].y = -0.404509f;
		vertices[59].z = -0.000000f;
		vertices[59].nx = -0.587785f;
		vertices[59].ny = -0.809017f;
		vertices[59].nz = -0.000000f;
		vertices[59].u = 0.400000f;
		vertices[59].v = 0.500000f;

		vertices[60].x = 0.000000f;
		vertices[60].y = -0.500000f;
		vertices[60].z = -0.000000f;
		vertices[60].nx = 0.000000f;
		vertices[60].ny = -1.000000f;
		vertices[60].nz = -0.000000f;
		vertices[60].u = 0.500000f;
		vertices[60].v = 0.500000f;

		vertices[61].x = 0.293893f;
		vertices[61].y = -0.404508f;
		vertices[61].z = -0.000000f;
		vertices[61].nx = 0.587785f;
		vertices[61].ny = -0.809017f;
		vertices[61].nz = -0.000000f;
		vertices[61].u = 0.600000f;
		vertices[61].v = 0.500000f;

		vertices[62].x = 0.475528f;
		vertices[62].y = -0.154509f;
		vertices[62].z = -0.000000f;
		vertices[62].nx = 0.951056f;
		vertices[62].ny = -0.309017f;
		vertices[62].nz = -0.000000f;
		vertices[62].u = 0.700000f;
		vertices[62].v = 0.500000f;

		vertices[63].x = 0.475528f;
		vertices[63].y = 0.154509f;
		vertices[63].z = -0.000000f;
		vertices[63].nx = 0.951056f;
		vertices[63].ny = 0.309017f;
		vertices[63].nz = -0.000000f;
		vertices[63].u = 0.800000f;
		vertices[63].v = 0.500000f;

		vertices[64].x = 0.293892f;
		vertices[64].y = 0.404509f;
		vertices[64].z = -0.000000f;
		vertices[64].nx = 0.587785f;
		vertices[64].ny = 0.809017f;
		vertices[64].nz = -0.000000f;
		vertices[64].u = 0.900000f;
		vertices[64].v = 0.500000f;

		vertices[65].x = -0.000000f;
		vertices[65].y = 0.500000f;
		vertices[65].z = -0.000000f;
		vertices[65].nx = -0.000000f;
		vertices[65].ny = 1.000000f;
		vertices[65].nz = -0.000000f;
		vertices[65].u = 1.000000f;
		vertices[65].v = 0.500000f;

		vertices[66].x = -0.000000f;
		vertices[66].y = 0.475528f;
		vertices[66].z = -0.154509f;
		vertices[66].nx = -0.000000f;
		vertices[66].ny = 0.951057f;
		vertices[66].nz = -0.309017f;
		vertices[66].u = 0.000000f;
		vertices[66].v = 0.400000f;

		vertices[67].x = -0.279508f;
		vertices[67].y = 0.384710f;
		vertices[67].z = -0.154509f;
		vertices[67].nx = -0.559017f;
		vertices[67].ny = 0.769421f;
		vertices[67].nz = -0.309017f;
		vertices[67].u = 0.100000f;
		vertices[67].v = 0.400000f;

		vertices[68].x = -0.452254f;
		vertices[68].y = 0.146946f;
		vertices[68].z = -0.154509f;
		vertices[68].nx = -0.904509f;
		vertices[68].ny = 0.293893f;
		vertices[68].nz = -0.309017f;
		vertices[68].u = 0.200000f;
		vertices[68].v = 0.400000f;

		vertices[69].x = -0.452254f;
		vertices[69].y = -0.146946f;
		vertices[69].z = -0.154509f;
		vertices[69].nx = -0.904509f;
		vertices[69].ny = -0.293893f;
		vertices[69].nz = -0.309017f;
		vertices[69].u = 0.300000f;
		vertices[69].v = 0.400000f;

		vertices[70].x = -0.279508f;
		vertices[70].y = -0.384710f;
		vertices[70].z = -0.154509f;
		vertices[70].nx = -0.559017f;
		vertices[70].ny = -0.769421f;
		vertices[70].nz = -0.309017f;
		vertices[70].u = 0.400000f;
		vertices[70].v = 0.400000f;

		vertices[71].x = 0.000000f;
		vertices[71].y = -0.475528f;
		vertices[71].z = -0.154509f;
		vertices[71].nx = 0.000000f;
		vertices[71].ny = -0.951057f;
		vertices[71].nz = -0.309017f;
		vertices[71].u = 0.500000f;
		vertices[71].v = 0.400000f;

		vertices[72].x = 0.279509f;
		vertices[72].y = -0.384710f;
		vertices[72].z = -0.154509f;
		vertices[72].nx = 0.559017f;
		vertices[72].ny = -0.769421f;
		vertices[72].nz = -0.309017f;
		vertices[72].u = 0.600000f;
		vertices[72].v = 0.400000f;

		vertices[73].x = 0.452254f;
		vertices[73].y = -0.146946f;
		vertices[73].z = -0.154509f;
		vertices[73].nx = 0.904509f;
		vertices[73].ny = -0.293893f;
		vertices[73].nz = -0.309017f;
		vertices[73].u = 0.700000f;
		vertices[73].v = 0.400000f;

		vertices[74].x = 0.452254f;
		vertices[74].y = 0.146946f;
		vertices[74].z = -0.154509f;
		vertices[74].nx = 0.904509f;
		vertices[74].ny = 0.293893f;
		vertices[74].nz = -0.309017f;
		vertices[74].u = 0.800000f;
		vertices[74].v = 0.400000f;

		vertices[75].x = 0.279508f;
		vertices[75].y = 0.384711f;
		vertices[75].z = -0.154509f;
		vertices[75].nx = 0.559017f;
		vertices[75].ny = 0.769421f;
		vertices[75].nz = -0.309017f;
		vertices[75].u = 0.900000f;
		vertices[75].v = 0.400000f;

		vertices[76].x = -0.000000f;
		vertices[76].y = 0.475528f;
		vertices[76].z = -0.154509f;
		vertices[76].nx = -0.000000f;
		vertices[76].ny = 0.951057f;
		vertices[76].nz = -0.309017f;
		vertices[76].u = 1.000000f;
		vertices[76].v = 0.400000f;

		vertices[77].x = -0.000000f;
		vertices[77].y = 0.404509f;
		vertices[77].z = -0.293893f;
		vertices[77].nx = -0.000000f;
		vertices[77].ny = 0.809017f;
		vertices[77].nz = -0.587785f;
		vertices[77].u = 0.000000f;
		vertices[77].v = 0.300000f;

		vertices[78].x = -0.237764f;
		vertices[78].y = 0.327254f;
		vertices[78].z = -0.293893f;
		vertices[78].nx = -0.475528f;
		vertices[78].ny = 0.654509f;
		vertices[78].nz = -0.587785f;
		vertices[78].u = 0.100000f;
		vertices[78].v = 0.300000f;

		vertices[79].x = -0.384710f;
		vertices[79].y = 0.125000f;
		vertices[79].z = -0.293893f;
		vertices[79].nx = -0.769421f;
		vertices[79].ny = 0.250000f;
		vertices[79].nz = -0.587785f;
		vertices[79].u = 0.200000f;
		vertices[79].v = 0.300000f;

		vertices[80].x = -0.384710f;
		vertices[80].y = -0.125000f;
		vertices[80].z = -0.293893f;
		vertices[80].nx = -0.769421f;
		vertices[80].ny = -0.250000f;
		vertices[80].nz = -0.587785f;
		vertices[80].u = 0.300000f;
		vertices[80].v = 0.300000f;

		vertices[81].x = -0.237764f;
		vertices[81].y = -0.327254f;
		vertices[81].z = -0.293893f;
		vertices[81].nx = -0.475528f;
		vertices[81].ny = -0.654509f;
		vertices[81].nz = -0.587785f;
		vertices[81].u = 0.400000f;
		vertices[81].v = 0.300000f;

		vertices[82].x = 0.000000f;
		vertices[82].y = -0.404509f;
		vertices[82].z = -0.293893f;
		vertices[82].nx = 0.000000f;
		vertices[82].ny = -0.809017f;
		vertices[82].nz = -0.587785f;
		vertices[82].u = 0.500000f;
		vertices[82].v = 0.300000f;

		vertices[83].x = 0.237764f;
		vertices[83].y = -0.327254f;
		vertices[83].z = -0.293893f;
		vertices[83].nx = 0.475528f;
		vertices[83].ny = -0.654508f;
		vertices[83].nz = -0.587785f;
		vertices[83].u = 0.600000f;
		vertices[83].v = 0.300000f;

		vertices[84].x = 0.384710f;
		vertices[84].y = -0.125000f;
		vertices[84].z = -0.293893f;
		vertices[84].nx = 0.769421f;
		vertices[84].ny = -0.250000f;
		vertices[84].nz = -0.587785f;
		vertices[84].u = 0.700000f;
		vertices[84].v = 0.300000f;

		vertices[85].x = 0.384710f;
		vertices[85].y = 0.125000f;
		vertices[85].z = -0.293893f;
		vertices[85].nx = 0.769421f;
		vertices[85].ny = 0.250000f;
		vertices[85].nz = -0.587785f;
		vertices[85].u = 0.800000f;
		vertices[85].v = 0.300000f;

		vertices[86].x = 0.237764f;
		vertices[86].y = 0.327254f;
		vertices[86].z = -0.293893f;
		vertices[86].nx = 0.475528f;
		vertices[86].ny = 0.654509f;
		vertices[86].nz = -0.587785f;
		vertices[86].u = 0.900000f;
		vertices[86].v = 0.300000f;

		vertices[87].x = -0.000000f;
		vertices[87].y = 0.404509f;
		vertices[87].z = -0.293893f;
		vertices[87].nx = -0.000000f;
		vertices[87].ny = 0.809017f;
		vertices[87].nz = -0.587785f;
		vertices[87].u = 1.000000f;
		vertices[87].v = 0.300000f;

		vertices[88].x = -0.000000f;
		vertices[88].y = 0.293893f;
		vertices[88].z = -0.404509f;
		vertices[88].nx = -0.000000f;
		vertices[88].ny = 0.587785f;
		vertices[88].nz = -0.809017f;
		vertices[88].u = 0.000000f;
		vertices[88].v = 0.200000f;

		vertices[89].x = -0.172746f;
		vertices[89].y = 0.237764f;
		vertices[89].z = -0.404509f;
		vertices[89].nx = -0.345491f;
		vertices[89].ny = 0.475528f;
		vertices[89].nz = -0.809017f;
		vertices[89].u = 0.100000f;
		vertices[89].v = 0.200000f;

		vertices[90].x = -0.279508f;
		vertices[90].y = 0.090818f;
		vertices[90].z = -0.404509f;
		vertices[90].nx = -0.559017f;
		vertices[90].ny = 0.181636f;
		vertices[90].nz = -0.809017f;
		vertices[90].u = 0.200000f;
		vertices[90].v = 0.200000f;

		vertices[91].x = -0.279508f;
		vertices[91].y = -0.090818f;
		vertices[91].z = -0.404509f;
		vertices[91].nx = -0.559017f;
		vertices[91].ny = -0.181636f;
		vertices[91].nz = -0.809017f;
		vertices[91].u = 0.300000f;
		vertices[91].v = 0.200000f;

		vertices[92].x = -0.172746f;
		vertices[92].y = -0.237764f;
		vertices[92].z = -0.404509f;
		vertices[92].nx = -0.345491f;
		vertices[92].ny = -0.475528f;
		vertices[92].nz = -0.809017f;
		vertices[92].u = 0.400000f;
		vertices[92].v = 0.200000f;

		vertices[93].x = 0.000000f;
		vertices[93].y = -0.293893f;
		vertices[93].z = -0.404509f;
		vertices[93].nx = 0.000000f;
		vertices[93].ny = -0.587785f;
		vertices[93].nz = -0.809017f;
		vertices[93].u = 0.500000f;
		vertices[93].v = 0.200000f;

		vertices[94].x = 0.172746f;
		vertices[94].y = -0.237764f;
		vertices[94].z = -0.404509f;
		vertices[94].nx = 0.345492f;
		vertices[94].ny = -0.475528f;
		vertices[94].nz = -0.809017f;
		vertices[94].u = 0.600000f;
		vertices[94].v = 0.200000f;

		vertices[95].x = 0.279508f;
		vertices[95].y = -0.090818f;
		vertices[95].z = -0.404509f;
		vertices[95].nx = 0.559017f;
		vertices[95].ny = -0.181636f;
		vertices[95].nz = -0.809017f;
		vertices[95].u = 0.700000f;
		vertices[95].v = 0.200000f;

		vertices[96].x = 0.279508f;
		vertices[96].y = 0.090818f;
		vertices[96].z = -0.404509f;
		vertices[96].nx = 0.559017f;
		vertices[96].ny = 0.181636f;
		vertices[96].nz = -0.809017f;
		vertices[96].u = 0.800000f;
		vertices[96].v = 0.200000f;

		vertices[97].x = 0.172746f;
		vertices[97].y = 0.237764f;
		vertices[97].z = -0.404509f;
		vertices[97].nx = 0.345491f;
		vertices[97].ny = 0.475528f;
		vertices[97].nz = -0.809017f;
		vertices[97].u = 0.900000f;
		vertices[97].v = 0.200000f;

		vertices[98].x = -0.000000f;
		vertices[98].y = 0.293893f;
		vertices[98].z = -0.404509f;
		vertices[98].nx = -0.000000f;
		vertices[98].ny = 0.587785f;
		vertices[98].nz = -0.809017f;
		vertices[98].u = 1.000000f;
		vertices[98].v = 0.200000f;

		vertices[99].x = -0.000000f;
		vertices[99].y = 0.154508f;
		vertices[99].z = -0.475528f;
		vertices[99].nx = -0.000000f;
		vertices[99].ny = 0.309017f;
		vertices[99].nz = -0.951057f;
		vertices[99].u = 0.000000f;
		vertices[99].v = 0.100000f;

		vertices[100].x = -0.090818f;
		vertices[100].y = 0.125000f;
		vertices[100].z = -0.475528f;
		vertices[100].nx = -0.181636f;
		vertices[100].ny = 0.250000f;
		vertices[100].nz = -0.951057f;
		vertices[100].u = 0.100000f;
		vertices[100].v = 0.100000f;

		vertices[101].x = -0.146946f;
		vertices[101].y = 0.047746f;
		vertices[101].z = -0.475528f;
		vertices[101].nx = -0.293892f;
		vertices[101].ny = 0.095491f;
		vertices[101].nz = -0.951057f;
		vertices[101].u = 0.200000f;
		vertices[101].v = 0.100000f;

		vertices[102].x = -0.146946f;
		vertices[102].y = -0.047746f;
		vertices[102].z = -0.475528f;
		vertices[102].nx = -0.293892f;
		vertices[102].ny = -0.095491f;
		vertices[102].nz = -0.951057f;
		vertices[102].u = 0.300000f;
		vertices[102].v = 0.100000f;

		vertices[103].x = -0.090818f;
		vertices[103].y = -0.125000f;
		vertices[103].z = -0.475528f;
		vertices[103].nx = -0.181635f;
		vertices[103].ny = -0.250000f;
		vertices[103].nz = -0.951057f;
		vertices[103].u = 0.400000f;
		vertices[103].v = 0.100000f;

		vertices[104].x = 0.000000f;
		vertices[104].y = -0.154508f;
		vertices[104].z = -0.475528f;
		vertices[104].nx = 0.000000f;
		vertices[104].ny = -0.309017f;
		vertices[104].nz = -0.951057f;
		vertices[104].u = 0.500000f;
		vertices[104].v = 0.100000f;

		vertices[105].x = 0.090818f;
		vertices[105].y = -0.125000f;
		vertices[105].z = -0.475528f;
		vertices[105].nx = 0.181636f;
		vertices[105].ny = -0.250000f;
		vertices[105].nz = -0.951057f;
		vertices[105].u = 0.600000f;
		vertices[105].v = 0.100000f;

		vertices[106].x = 0.146946f;
		vertices[106].y = -0.047746f;
		vertices[106].z = -0.475528f;
		vertices[106].nx = 0.293892f;
		vertices[106].ny = -0.095491f;
		vertices[106].nz = -0.951057f;
		vertices[106].u = 0.700000f;
		vertices[106].v = 0.100000f;

		vertices[107].x = 0.146946f;
		vertices[107].y = 0.047746f;
		vertices[107].z = -0.475528f;
		vertices[107].nx = 0.293892f;
		vertices[107].ny = 0.095491f;
		vertices[107].nz = -0.951057f;
		vertices[107].u = 0.800000f;
		vertices[107].v = 0.100000f;

		vertices[108].x = 0.090818f;
		vertices[108].y = 0.125000f;
		vertices[108].z = -0.475528f;
		vertices[108].nx = 0.181635f;
		vertices[108].ny = 0.250000f;
		vertices[108].nz = -0.951057f;
		vertices[108].u = 0.900000f;
		vertices[108].v = 0.100000f;

		vertices[109].x = -0.000000f;
		vertices[109].y = 0.154508f;
		vertices[109].z = -0.475528f;
		vertices[109].nx = -0.000000f;
		vertices[109].ny = 0.309017f;
		vertices[109].nz = -0.951057f;
		vertices[109].u = 1.000000f;
		vertices[109].v = 0.100000f;

		vertices[110].x = 0.000000f;
		vertices[110].y = -0.000000f;
		vertices[110].z = -0.500000f;
		vertices[110].nx = 0.000000f;
		vertices[110].ny = -0.000000f;
		vertices[110].nz = -1.000000f;
		vertices[110].u = 0.000000f;
		vertices[110].v = -0.000000f;

		vertices[111].x = 0.000000f;
		vertices[111].y = -0.000000f;
		vertices[111].z = -0.500000f;
		vertices[111].nx = 0.000000f;
		vertices[111].ny = -0.000000f;
		vertices[111].nz = -1.000000f;
		vertices[111].u = 0.100000f;
		vertices[111].v = -0.000000f;

		vertices[112].x = 0.000000f;
		vertices[112].y = -0.000000f;
		vertices[112].z = -0.500000f;
		vertices[112].nx = 0.000000f;
		vertices[112].ny = -0.000000f;
		vertices[112].nz = -1.000000f;
		vertices[112].u = 0.200000f;
		vertices[112].v = -0.000000f;

		vertices[113].x = 0.000000f;
		vertices[113].y = 0.000000f;
		vertices[113].z = -0.500000f;
		vertices[113].nx = 0.000000f;
		vertices[113].ny = 0.000000f;
		vertices[113].nz = -1.000000f;
		vertices[113].u = 0.300000f;
		vertices[113].v = -0.000000f;

		vertices[114].x = 0.000000f;
		vertices[114].y = 0.000000f;
		vertices[114].z = -0.500000f;
		vertices[114].nx = 0.000000f;
		vertices[114].ny = 0.000000f;
		vertices[114].nz = -1.000000f;
		vertices[114].u = 0.400000f;
		vertices[114].v = -0.000000f;

		vertices[115].x = -0.000000f;
		vertices[115].y = 0.000000f;
		vertices[115].z = -0.500000f;
		vertices[115].nx = -0.000000f;
		vertices[115].ny = 0.000000f;
		vertices[115].nz = -1.000000f;
		vertices[115].u = 0.500000f;
		vertices[115].v = -0.000000f;

		vertices[116].x = -0.000000f;
		vertices[116].y = 0.000000f;
		vertices[116].z = -0.500000f;
		vertices[116].nx = -0.000000f;
		vertices[116].ny = 0.000000f;
		vertices[116].nz = -1.000000f;
		vertices[116].u = 0.600000f;
		vertices[116].v = -0.000000f;

		vertices[117].x = -0.000000f;
		vertices[117].y = 0.000000f;
		vertices[117].z = -0.500000f;
		vertices[117].nx = -0.000000f;
		vertices[117].ny = 0.000000f;
		vertices[117].nz = -1.000000f;
		vertices[117].u = 0.700000f;
		vertices[117].v = -0.000000f;

		vertices[118].x = -0.000000f;
		vertices[118].y = -0.000000f;
		vertices[118].z = -0.500000f;
		vertices[118].nx = -0.000000f;
		vertices[118].ny = -0.000000f;
		vertices[118].nz = -1.000000f;
		vertices[118].u = 0.800000f;
		vertices[118].v = -0.000000f;

		vertices[119].x = -0.000000f;
		vertices[119].y = -0.000000f;
		vertices[119].z = -0.500000f;
		vertices[119].nx = -0.000000f;
		vertices[119].ny = -0.000000f;
		vertices[119].nz = -1.000000f;
		vertices[119].u = 0.900000f;
		vertices[119].v = -0.000000f;

		vertices[120].x = 0.000000f;
		vertices[120].y = -0.000000f;
		vertices[120].z = -0.500000f;
		vertices[120].nx = 0.000000f;
		vertices[120].ny = -0.000000f;
		vertices[120].nz = -1.000000f;
		vertices[120].u = 1.000000f;
		vertices[120].v = -0.000000f;
	}

	Mesh* mesh = new Mesh(vertices, faces);
	MeshNode* node = new MeshNode;
	node->meshName = Mesh::SphereName;
	node->mesh = mesh;
	node->next = head;
	head = node;

	return mesh;
}

Mesh* MeshManager::LoadBoxPrimitive()
{
	assert(false);
	return nullptr;
}

Mesh* MeshManager::LoadPlanePrimitive()
{
	Vertex verts[] = 
	{
		{0.5f, 0.5f, 0.f, 0, 0, 1, 0, 0},
		{-0.5f, 0.5f, 0.f, 0, 0, 1, 1, 0},
		{-0.5f, -0.5f, 0.f, 0, 0, 1, 1, 1},
		{0.5f, -0.5f, 0.f, 0, 0, 1, 0, 1}
	};

	Face faces[] = 
	{
		{1, 0, 2},
		{0, 3, 2}
	};

	Mesh* mesh = new Mesh(verts, faces);
	MeshNode* node = new MeshNode;
	node->meshName = Mesh::PlaneName;
	node->mesh = mesh;
	node->next = head;
	head = node;

	return mesh;
}

Mesh* MeshManager::LoadPyramidPrimitive()
{
	Face faces[] =
	{
		{ 2,1,0 },
		{ 0,3,2 },
		{ 6,5,4 },
		{ 6,7,5 },
		{ 10,9,8 },
		{ 10,11,9 },
		{ 14,13,12 },
		{ 14,15,13 },
		{ 18,17,16 },
		{ 16,19,18 },
		{ 22,21,20 },
		{ 20,23,22 }
	};

	Vertex  verts[] =
	{
		{ -0.25f,  0.0f, -0.25f, -0.6f,  0.6f, -0.6f, 0.0f, 0.0f }, 
		{ -0.25f, -0.0f, -0.25f, -0.6f, -0.6f, -0.6f, 0.0f, 1.0f },
		{  0.25f, -0.0f, -0.25f,  0.6f, -0.6f, -0.6f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, -0.0f, 0.6f,  0.6f, -0.6f, 1.0f, 0.0f },
		{ 0.25f, -0.0f, -0.25f, 0.6f, -0.6f, -0.6f, 0.0f, 1.0f },
		{ 0.25f, -0.0f,  0.25f, 0.6f, -0.6f,  0.6f, 1.0f, 1.0f },
		{ 0.0f,   1.0f, -0.0f,  0.6f,  0.6f, -0.6f, 0.0f, 0.0f },
		{ 0.25f,  0.0f,  0.25f, 0.6f,  0.6f,  0.6f, 1.0f, 0.0f }, 
		{  0.25f, -0.0f, 0.25f,  0.6f, -0.6f, 0.6f, 1.0f, 1.0f },
		{ -0.25f, -0.0f, 0.25f, -0.6f, -0.6f, 0.6f, 0.0f, 1.0f },
		{  0.0f,   1.0f, 0.0f,   0.6f,  0.6f, 0.6f, 1.0f, 0.0f },
		{ -0.0f,  1.0f,  0.0f,  -0.6f,  0.6f, 0.6f, 0.0f, 0.0f },
		{ -0.25f, -0.0f,  0.25f, -0.6f, -0.6f,  0.6f, 0.0f, 0.0f },
		{ -0.25f, -0.0f, -0.25f, -0.6f, -0.6f, -0.6f, 0.0f, 1.0f },
		{ -0.0f,  1.0f,  0.0f, -0.6f,  0.6f,  0.6f, 1.0f, 0.0f, },
		{ -0.0f,  1.0f, -0.0f, -0.6f,  0.6f, -0.6f, 1.0f, 1.0f },
		{ -0.25f, -0.0f,  0.25f,  -0.6f, -0.6f,  0.6f, 0.0f, 0.0f },
		{ 0.25f, -0.0f,   0.25f,   0.6f, -0.6f,  0.6f, 1.0f, 0.0f },
		{ 0.25f, -0.0f,  -0.25f,   0.6f, -0.6f, -0.6f, 1.0f, 1.0f },
		{ -0.25f, -0.0f, -0.25f, -0.6f, -0.6f, -0.6f, 0.0f, 1.0f, },
		{ -0.0f,  1.0f, -0.0f, -0.6f, 0.6f, -0.6f, 0.0f, 1.0f }, 
		{ 0.0f,  1.0f,  -0.0f,  0.6f, 0.6f, -0.6f, 1.0f, 1.0f }, 
		{ 0.0f,  1.0f,   0.0f,  0.6f, 0.6f,  0.6f, 1.0f, 0.0f },
		{ -0.0f,  1.0f,  0.0f, -0.6f, 0.6f,  0.6f,  0.0f, 0.0f, },

	};
	
	Mesh* mesh = new Mesh(verts, faces);
	MeshNode* node = new MeshNode;
	node->meshName = Mesh::PyramidName;
	node->mesh = mesh;
	node->next = head;
	head = node;

	return mesh;
}

Mesh* MeshManager::LoadMeshInternalPak(uint8* modelData, const char* modelName)
{
	ModelFileHeader* header = reinterpret_cast<ModelFileHeader*>(modelData);
	uint8* vertPtr = modelData + header->vertBufferOffset;
	uint8* facePtr = modelData + header->facesBufferOffset;

	Array<Vertex> vertices(header->numVerts);
	Array<Face> faces(header->numFaces);
	CollisionInfo* sphereInfo = new CollisionInfo;
	sphereInfo->boundingSphere = header->boundingSphere;

	Memcpy(vertices.GetData(), vertices.Size() * sizeof(Vertex), vertPtr, header->numVerts * sizeof(Vertex));
	Memcpy(faces.GetData(), faces.Size() * sizeof(Face), facePtr, header->numFaces * sizeof(Face));

	Mesh* mesh = new Mesh(std::move(vertices), std::move(faces), sphereInfo);
	MeshNode* node = new MeshNode;
	node->meshName = modelName;
	node->mesh = mesh;

	node->next = head;
	head = node;

	return mesh;
}

Mesh* MeshManager::LoadMeshInternalPak(uint8* modelData, uint8* skinningData, const char* modelName)
{
	ModelFileHeader* header = reinterpret_cast<ModelFileHeader*>(modelData);
	uint8* vertPtr = modelData + header->vertBufferOffset;
	uint8* facePtr = modelData + header->facesBufferOffset;
	

	Array<Vertex> vertices(header->numVerts);
	size_t verticesBytes = vertices.Size() * sizeof(Vertex);

	Array<VertexBoneWeights> skinningWeights(header->numVerts);
	size_t skinningWeightBytes = skinningWeights.Size() * sizeof(VertexBoneWeights);
	
	Array<Face> faces(header->numFaces);
	size_t facesBytes = faces.Size() * sizeof(Face);

	CollisionInfo* sphereInfo = new CollisionInfo;
	sphereInfo->boundingSphere = header->boundingSphere;

	Memcpy(vertices.GetData(), verticesBytes, vertPtr, verticesBytes);
	Memcpy(skinningWeights.GetData(), skinningWeightBytes, skinningData, skinningWeightBytes);
	Memcpy(faces.GetData(), facesBytes, facePtr, facesBytes);

	Mesh* mesh = new Mesh(std::move(vertices), std::move(faces), std::move(skinningWeights), sphereInfo);
	MeshNode* node = new MeshNode;
	node->meshName = modelName;
	node->mesh = mesh;

	node->next = head;
	head = node;

	return mesh;
}

void MeshManager::UnloadMeshInternal(const char* modelName)
{
	MeshNode* current = head;
	while (current != nullptr)
	{
		if (strcmp(current->next->meshName, modelName) == 0)
		{
			MeshNode* node = current->next;
			current->next = current->next->next;
			delete node->mesh;
			delete node;
			break;
		}

		current = current->next;
	}
}

MeshManager& MeshManager::Instance()
{
	static MeshManager mmInstance;
	return mmInstance;
}

MeshManager::~MeshManager()
{
	MeshNode* current = head;
	while (current != nullptr)
	{
		MeshNode* node = current->next;
		if (node != nullptr)
		{
			current->next = current->next->next;
			delete node->mesh;
			delete node;
		}

		current = current->next;
	}
}
