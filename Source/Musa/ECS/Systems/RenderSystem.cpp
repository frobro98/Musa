
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

ComponentGroupDescription RenderSystem::GetRequiredComponents() const
{
	ComponentGroupDescription group;
	group.AddComponents<TransformComponent, RenderComponent>();
	return group;
}

void RenderSystem::AssociateEntity(const Musa::Entity & entity)
{
	ComponentGroupDescription entityComponents = Musa::GetComponentDescriptionOf(entity);
	Assert(entityComponents.ContainsTypes(GetRequiredComponents()));
	RenderWork work =
	{
		GetComponentFrom<TransformComponent>(entity), // const transform
		GetComponentFrom<RenderComponent>(entity)	  // render
	};
	Assert(work.transform != nullptr);
	componentWork.Add(work);
}
