#include "CameraSystem.hpp"
#include "ECS/Components/CameraComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/ComponentTypeDefinition.hpp"
#include "ECS/Entity.hpp"

using namespace Musa;

void CameraSystem::Tick(float tick)
{
	UNUSED(tick);
	for (auto& work : componentWork)
	{
		ProcessSingleCamera(work);
	}
}

ComponentGroupDescription CameraSystem::GetRequiredComponents() const
{
	ComponentGroupDescription group;
	group.AddComponents<CameraComponent, TransformComponent>();
	return group;
}

void CameraSystem::AssociateEntity(const Musa::Entity& entity)
{
	ComponentGroupDescription entityComponents = Musa::GetComponentDescriptionOf(entity);
	Assert(entityComponents.ContainsTypes(GetRequiredComponents()));
	CameraWork work = 
	{ 
		GetComponentFrom<TransformComponent>(entity), // const transform
		GetComponentFrom<CameraComponent>(entity)	  // camera
	};
	Assert(work.transform != nullptr);
	componentWork.Add(work);
}

void CameraSystem::UpdateOrientiationAndPosition(const TransformComponent& transform, CameraComponent& camera)
{
	camera.position = transform.GetPosition();
	
	camera.forward = camera.position - camera.lookAt;
	camera.forward.Normalize();
	camera.right = camera.up.Cross(camera.forward);
	camera.right.Normalize();
	camera.up = camera.forward.Cross(camera.right);
	camera.up.Normalize();

}

void CameraSystem::ProcessSingleCamera(CameraWork& work) 
{
	UpdatePosition(work);
	UpdateViewport(work);
	CalculatePlaneDimentions(work);
	CalculateFrustum(work);
	UpdateProjection(work);
	UpdateView(work);
}

void CameraSystem::UpdatePosition(CameraWork& work)
{
	const TransformComponent* transform = work.transform;
	CameraComponent* camera = work.camera;
	//if (transform->WasUpdatedThisFrame())
	{
		UpdateOrientiationAndPosition(*transform, *camera);
	}
}

void CameraSystem::CalculatePlaneDimentions(CameraWork& work)
{
	CameraComponent* cam = work.camera;

	cam->nearDimensions.height = 2.f * Math::Tan(Math::DegreesToRadians(cam->fov) * .5f) * cam->nearPlane;
	cam->nearDimensions.width = cam->nearDimensions.height * cam->aspectRatio;
	cam->farDimensions.height = 2.f * Math::Tan(Math::DegreesToRadians(cam->fov) * .5f) * cam->farPlane;
	cam->farDimensions.width = cam->farDimensions.height * cam->aspectRatio;
}

void CameraSystem::CalculateFrustum(CameraWork& work)
{
	CameraComponent* cam = work.camera;
	Vector position = cam->position;

	Vector fwd = cam->forward;
	Vector right = cam->right;
	Vector up = cam->up;
	float nearPlane = cam->nearPlane;
	float farPlane = cam->farPlane;
	float nearWidth = cam->nearDimensions.width;
	float nearHeight = cam->nearDimensions.height;
	float farWidth = cam->farDimensions.width;
	float farHeight = cam->farDimensions.height;

	cam->frustum.nearTopLeft = position - fwd * nearPlane + up * nearHeight * 0.5f - right * nearWidth * 0.5f;
	cam->frustum.nearTopRight = position - fwd * nearPlane + up * nearHeight * 0.5f + right * nearWidth * 0.5f;
	cam->frustum.nearBottomLeft = position - fwd * nearPlane - up * nearHeight * 0.5f - right * nearWidth * 0.5f;
	cam->frustum.nearBottomRight = position - fwd * nearPlane - up * nearHeight * 0.5f + right * nearWidth * 0.5f;

	cam->frustum.farTopLeft = position - fwd * farPlane + up * farHeight * 0.5f - right * farWidth * 0.5f;
	cam->frustum.farTopRight = position - fwd * farPlane + up * farHeight * 0.5f + right * farWidth * 0.5f;
	cam->frustum.farBottomLeft = position - fwd * farPlane - up * farHeight * 0.5f - right * farWidth * 0.5f;
	cam->frustum.farBottomRight = position - fwd * farPlane - up * farHeight * 0.5f + right * farWidth * 0.5f;

	CalculateFrustumNormals(work);
}

void CameraSystem::CalculateFrustumNormals(CameraWork& work)
{
	CameraComponent* cam = work.camera;
	Vector A = cam->frustum.nearBottomLeft - cam->frustum.nearTopLeft;
	Vector B = cam->frustum.nearTopRight - cam->frustum.nearTopLeft;
	Vector C = cam->frustum.farTopLeft - cam->frustum.nearTopLeft;

	cam->frustum.nearPlaneNormal = A.Cross(B);
	cam->frustum.nearPlaneNormal.Normalize();

	cam->frustum.leftNormal = C.Cross(A);
	cam->frustum.leftNormal.Normalize();

	cam->frustum.topNormal = B.Cross(C);
	cam->frustum.topNormal.Normalize();

	A = cam->frustum.farBottomLeft - cam->frustum.farBottomRight;
	B = cam->frustum.farTopRight - cam->frustum.farBottomRight;
	C = cam->frustum.nearBottomRight - cam->frustum.farBottomRight;

	cam->frustum.farPlaneNormal = A.Cross(B);
	cam->frustum.farPlaneNormal.Normalize();

	cam->frustum.rightNormal = B.Cross(C);
	cam->frustum.rightNormal.Normalize();

	cam->frustum.bottomNormal = C.Cross(A);
	cam->frustum.bottomNormal.Normalize();
}

void CameraSystem::UpdateView(CameraWork& work)
{
	CameraComponent* cam = work.camera;
	cam->view[m0] = cam->right.x;
	cam->view[m1] = cam->up.x;
	cam->view[m2] = cam->forward.x;
	cam->view[m3] = 0.f;

	cam->view[m4] = cam->right.y;
	cam->view[m5] = cam->up.y;
	cam->view[m6] = cam->forward.y;
	cam->view[m7] = 0.f;

	cam->view[m8]  = cam->right.z;
	cam->view[m9]  = cam->up.z;
	cam->view[m10] = cam->forward.z;
	cam->view[m11] = 0.f;

	cam->view[m12] = -cam->position.Dot(cam->right);
	cam->view[m13] = -cam->position.Dot(cam->up);
	cam->view[m14] = -cam->position.Dot(cam->forward);
	cam->view[m15] = 1.f;
}

void CameraSystem::UpdateProjection(CameraWork& work)
{
	CameraComponent* cam = work.camera;
	float nearPlane = cam->nearPlane;
	float farPlane = cam->farPlane;
	cam->projection[m0] = 2.f * nearPlane / cam->nearDimensions.width;//1.f / (aspect * tanHalfFOV);
	cam->projection[m1] = 0;
	cam->projection[m2] = 0;
	cam->projection[m3] = 0;

	cam->projection[m4] = 0;
	cam->projection[m5] = 2.f * nearPlane / cam->nearDimensions.height;//1.f / tanHalfFOV;
	cam->projection[m6] = 0;
	cam->projection[m7] = 0;

	cam->projection[m8] = 0;
	cam->projection[m9] = 0;
	cam->projection[m10] = (farPlane + nearPlane) / (nearPlane - farPlane);//-(farPlane + nearPlane) / (farPlane - nearPlane);
	cam->projection[m11] = -1.f;

	cam->projection[m12] = 0;
	cam->projection[m13] = 0;
	cam->projection[m14] = (2.f * farPlane * nearPlane) / (nearPlane - farPlane);
	cam->projection[m15] = 0;
}

void CameraSystem::UpdateViewport(CameraWork& /*work*/)
{
}


