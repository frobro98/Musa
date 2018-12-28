#pragma once

#include "ECS/System.hpp"
#include "Containers/Array.h"

namespace Musa
{
struct Entity;
}

class CameraSystem : public Musa::System
{
public:

	virtual void Tick(float tick) override;
	virtual Musa::ComponentGroupDescription GetRequiredComponents() const override;
	virtual void AssociateEntity(const Musa::Entity& entity) override;

private:
	struct CameraWork
	{
		const struct TransformComponent* transform;
		struct CameraComponent* camera;
	};

	void ProcessSingleCamera(CameraWork& work);
	void UpdatePosition(CameraWork& work);
	void CalculatePlaneDimentions(CameraWork& work);
	void CalculateFrustum(CameraWork& work);
	void CalculateFrustumNormals(CameraWork& work);
	void UpdateView(CameraWork& work);
	void UpdateProjection(CameraWork& work);
	void UpdateViewport(CameraWork& work);

	void UpdateOrientiationAndPosition(const TransformComponent& transform, CameraComponent& camera);

private:
	Array<CameraWork> componentWork;
};

DEFINE_SYSTEM(CameraSystem);