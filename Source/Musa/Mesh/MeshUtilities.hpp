#pragma once

#include "EngineCore/Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"

struct Vertex;
struct Face;

void GenerateMeshTangents(DynamicArray<Vertex>& verts, const DynamicArray<Face>& faces);