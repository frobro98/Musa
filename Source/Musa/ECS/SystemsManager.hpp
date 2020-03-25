#pragma once

#include "EngineCore/Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"
#include "PCSTree/PCSTree.h"
#include "ECS/System.hpp"
#include "Containers/Map.h"

namespace Musa
{

struct Entity;

// Probably will have to change to not a singleton down the line
class SystemManager final
{
public:
	void Initialize();

	void AddSystem(System& system);
	// TODO - This method needs to have a better name
	void AddSystemDependentOn(System& system, System& dependentSystem);
	System* FindSystemOfType(SystemType type);
	void RemoveSystemFromHierarchy(System* system);

	void AddEntityToProcess(const Entity& entity);

	void ProcessDirtyEntities();
	void TickSystems(float deltaTime);

	static SystemManager& Instance();
private:
	SystemManager() = default;

	void AddEntityToRequiredSystems(const Entity& entity);

private:
	DynamicArray<const Entity*> dirtyEntities;
	PCSTree<System> systemDependencyTree;
	Map<SystemType, System*> systemsMap;
	Map<ComponentSet, DynamicArray<System*>> componentsToRelevantSystems;
};

SystemManager& GetSystemsManager();

}