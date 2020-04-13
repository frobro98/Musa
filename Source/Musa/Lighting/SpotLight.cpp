#include "SpotLight.hpp"

#include "SpotLight.hpp"
#include "Math/MathFunctions.hpp"

constexpr float32 RadAdjustment = .001f;
constexpr float32 MaxInnerRad = Math::DegreesToRadians(89.f);
constexpr float32 MaxOuterRad = MaxInnerRad + .001f;

LightDescription SpotLight::GetLightDescription()
{
	worldLight = worldTransform.GetInverse();

	const float32 innerRad = Math::DegreesToRadians(innerAngleDegrees);
	const float32 outerRad = Math::DegreesToRadians(outerAngleDegrees);

	const float32 clampedInnerRad = Math::Clamp(innerRad, 0.f, MaxInnerRad);
	const float32 clampedOuterRad = Math::Clamp(outerRad, clampedInnerRad + RadAdjustment, MaxOuterRad);

	const float32 cosInner = Math::Cos(clampedInnerRad);
	const float32 cosOuter = Math::Cos(clampedOuterRad);

	LightDescription info = {};
	info.direction = -GetDirection();
	info.cosOuterAngle = cosOuter;
	info.inverseCosDiff = 1 / (cosInner - cosOuter);
	info.position = position;
	info.radius = attenuationRadius;

	return info;
}
