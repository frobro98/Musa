#pragma once

class Model;
class Mesh;
class Material;
class RendererContext;

class ModelFactory
{
public:
	static Model* CreateModel(Mesh* mesh, Material* material);
	static void DestroyModel(Model* model);

private:
	ModelFactory() = default;
	~ModelFactory() = default;

	static ModelFactory& Instance();

private:
	RendererContext* renderer;

};