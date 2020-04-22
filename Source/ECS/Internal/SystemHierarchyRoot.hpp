#pragma once

// TODO - Determine later on, as development progresses, if the SceneRoot and/or the SystemHierarchyRoot are necessary

#include "ECS/System.hpp"

namespace Musa
{
namespace Internal
{

class SystemHierarchyRoot : public System
{
public:
	virtual void Tick(float /*tick*/) override {};
//	virtual ComponentSet GetRequiredComponents() const override { return ComponentSet(); }
	virtual void AssociateEntity(const Entity& /*entity*/) override {}
};

}
}
