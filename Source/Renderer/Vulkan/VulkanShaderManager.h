#pragma once

#include "Graphics.h"
#include "String/String.h"

struct VulkanShaders;
class VulkanShader;

class VulkanShaderManager
{
public:

public:
	static constexpr const char* DefaultShader = "_default-shader";

	static VulkanShader* CreateShader();

	static void AddShader(VulkanShader* vertexShader, VulkanShader* fragShader, const tchar* shaderName);
	static void AddShader(VulkanShader* vertexShader, VulkanShader* fragShader, const String& shaderName);
	static void RemoveShader(const tchar* shaderName);
	static void RemoveShader(const String& shaderName);

	static VulkanShaders* FindShader(const char* shaderName);

private:
	VulkanShaderManager() = default;
	~VulkanShaderManager();

	static VulkanShaderManager& Instance();
	void AddShaderInternal(VulkanShader* vertexShader, VulkanShader* fragShader, const tchar* shaderName);
	void AddShaderInternal(VulkanShader* vertexShader, VulkanShader* fragShader, const String& shaderName);
	void RemoveShaderInternal(const char* shaderName);
	void RemoveShaderInternal(const String& shaderName);

private:
	// This is because it needs a device. This needs to move or change because this isn't good
	friend class Renderer;

	struct VulkanShaderNode
	{
		VulkanShaders* shaders = nullptr;
		VulkanShaderNode* next = nullptr;
		String shaderName;
	};

	VulkanShaderNode* head = nullptr;
	class VulkanDevice* logicalDevice = nullptr;
};