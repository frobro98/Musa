
#include "RenderSystem.hpp"
#include "ECS/Components/RenderComponent.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/ComponentTypeDefinition.hpp"
#include "ECS/Entity.hpp"

using namespace Musa;

void RenderSystem::Tick(float deltaTime)
{
	UNUSED(deltaTime);
}

// ComponentSet RenderSystem::GetRequiredComponents() const
// {
// 	ComponentSet group;
// 	AddComponentsTo<TransformComponent, RenderComponent>(group);
// 	return group;
// }

void RenderSystem::AssociateEntity(const Musa::Entity& /*entity*/)
{
// 	ComponentSet entityComponents = Musa::GetComponentDescriptionOf(entity);
// 	Assert(entityComponents.ContainsTypes(GetRequiredComponents()));
// 	RenderWork work =
// 	{
// 		GetComponentFrom<TransformComponent>(entity), // const transform
// 		GetComponentFrom<RenderComponent>(entity)	  // render
// 	};
// 	Assert(work.transform != nullptr);
// 	componentWork.Add(work);
}
