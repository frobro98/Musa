
#include "VulkanShaderManager.h"
#include "VulkanShader.h"

VulkanShader* VulkanShaderManager::CreateShader()
{
	return new VulkanShader(Instance().logicalDevice);
}

void VulkanShaderManager::AddShader(VulkanShader * vertexShader, VulkanShader * fragShader, const tchar * shaderName)
{
	Instance().AddShaderInternal(vertexShader, fragShader, shaderName);
}

void VulkanShaderManager::AddShader(VulkanShader * vertexShader, VulkanShader * fragShader, const String & shaderName)
{
	Instance().AddShaderInternal(vertexShader, fragShader, shaderName);
}

void VulkanShaderManager::RemoveShader(const tchar * shaderName)
{
	Instance().RemoveShaderInternal(shaderName);
}

void VulkanShaderManager::RemoveShader(const String& shaderName)
{
	Instance().RemoveShaderInternal(shaderName);
}

VulkanShaders* VulkanShaderManager::FindShader(const char * shaderName)
{
	VulkanShaderNode* current = Instance().head;
	while (current != nullptr)
	{
		if (shaderName == current->shaderName)
		{
			return current->shaders;
		}
	
		current = current->next;
	}

	return nullptr;
}

VulkanShaderManager::~VulkanShaderManager()
{
}

VulkanShaderManager & VulkanShaderManager::Instance()
{
	static VulkanShaderManager vmInstance;
	return vmInstance;
}

void VulkanShaderManager::AddShaderInternal(VulkanShader * vertexShader, VulkanShader * fragShader, const tchar * shaderName)
{
	VulkanShaderNode* node = new VulkanShaderNode;
	node->shaderName = shaderName;
	node->shaders = new VulkanShaders;
	node->shaders->shaders[Stage_Vert] = vertexShader;
	node->shaders->shaders[Stage_Frag] = fragShader;
	
	node->next = head;
	head = node;
}

void VulkanShaderManager::AddShaderInternal(VulkanShader * vertexShader, VulkanShader * fragShader, const String & shaderName)
{
	VulkanShaderNode* node = new VulkanShaderNode;
	node->shaderName = shaderName;
	node->shaders = new VulkanShaders;
	node->shaders->shaders[Stage_Vert] = vertexShader;
	node->shaders->shaders[Stage_Frag] = fragShader;

	node->next = head;
	head = node;
}

void VulkanShaderManager::RemoveShaderInternal(const char * shaderName)
{
	VulkanShaderNode* current = head;
	while (current != nullptr)
	{
		if (current->next->shaderName == shaderName)
		{
			VulkanShaderNode* node = current->next;
			current->next = current->next->next;
			delete node->shaders;
			delete node;
			break;
		}
	
		current = current->next;
	}
}

void VulkanShaderManager::RemoveShaderInternal(const String & shaderName)
{
	VulkanShaderNode* current = head;
	while (current != nullptr)
	{
		if (current->next->shaderName == shaderName)
		{
			VulkanShaderNode* node = current->next;
			current->next = current->next->next;
			delete node->shaders;
			delete node;
			break;
		}

		current = current->next;
	}
}
