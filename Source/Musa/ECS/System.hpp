#pragma once

#include "PCSTree/PCSNode.h"
#include "ECS/SystemTypeDefinitions.hpp"
#include "ECS/ComponentTypeDefinition.hpp"

namespace Musa
{
class System;
struct Entity;

namespace Internal
{
System* GetSystemWithType(SystemType type);
}

class System : public PCSNode<System>
{
public:
	virtual void Tick(float tick) = 0;
	virtual void OnDestruction() {};
	virtual void AssociateEntity(const Entity& entity) = 0;
//	[[nodiscard]] virtual ComponentSet GetRequiredComponents() const = 0;
};

template <class Sys>
Sys* GetSystem()
{
	SystemType type = GetSystemType<Sys>();
	return reinterpret_cast<Sys*>(Internal::GetSystemWithType(type));
}

template <class Sys, typename... Args>
Sys* CreateSystem(Args&&... args)
{
	Sys* system = new Sys(std::forward<Args>(args)...);
	// TODO - probably should register system with the system manager to associate it with its required components
	return system;
}

void DestroySystem(System* system);
}