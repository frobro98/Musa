#pragma once

#include "GameObject/GameObject.h"
#include "Math/Matrix.h"
#include "Graphics/UniformBuffer.h"

class VulkanDevice;
struct NativeUniformBuffer;

struct LightDescription
{
	Vector position;
	Color color;
	Vector direction;
	float cosOuterAngle; // Because shader is doing dot product using this information, the outer angle is kept in cosine space, allowing the shader to do angle calculations
	float inverseCosDiff; // 1 / (innerCos - outerCos)
	float radius;
};

class Light : public GameObject
{
public:
	Light();

	virtual LightDescription GetLightDescription() = 0;

	inline Vector GetDirection() const
	{
		return Vector(-worldLight[m2], -worldLight[m6], -worldLight[m10]);
	}
	inline const Matrix& GetWorldLightTransform() const
	{
		return worldLight;
	}

	inline NativeUniformBuffer& GetLightBuffer() { return *lightInformationBuffer; }

	Color lightColor;

protected:
	Matrix worldLight;
	NativeUniformBuffer* lightInformationBuffer;
};