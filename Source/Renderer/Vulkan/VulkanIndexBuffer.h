#pragma once

#include "VulkanAbstractions.h"
#include "Containers/Array.h"

struct Face;

class VulkanIndexBuffer
{
public:
	VulkanIndexBuffer(const Array<Face>& indices);
	~VulkanIndexBuffer();
	VulkanIndexBuffer(const VulkanIndexBuffer&) = delete;
	VulkanIndexBuffer& operator=(const VulkanIndexBuffer&) = delete;

	void Bind(class VulkanCommandBuffer* cmdBuffer) const;

	inline VulkanBuffer* GetBuffer() { return &indexBuffer; }

private:
	VulkanBuffer indexBuffer;

};

typedef VulkanIndexBuffer IndexBuffer;