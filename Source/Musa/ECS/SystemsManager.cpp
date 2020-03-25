
#include "SystemsManager.hpp"
#include "System.hpp"
#include "Internal/SystemHierarchyRoot.hpp"
#include "Internal/EntityPool.hpp"
#include "ECS/Entity.hpp"

namespace Musa
{
SystemManager& SystemManager::Instance()
{
	static SystemManager smInstance;
	return smInstance;
}

void SystemManager::Initialize()
{
	systemDependencyTree.Insert(new Internal::SystemHierarchyRoot, nullptr);
}

void SystemManager::AddSystem(System& system)
{
	// TODO - Adding a system to the root is reverse ordering. Can't do this by default because that's not how people would assume it works
	systemDependencyTree.Insert(&system, systemDependencyTree.GetRoot());
}

void SystemManager::AddSystemDependentOn(System& system, System& dependingSystem)
{
	systemDependencyTree.Insert(&system, &dependingSystem);
}

System* SystemManager::FindSystemOfType(SystemType type)
{
	return systemsMap[type];
}

void SystemManager::RemoveSystemFromHierarchy([[maybe_unused]] System* system)
{
	// Remove the single system from the tree. Must be done at the PCSTree level
}

void SystemManager::AddEntityToProcess(const Entity& entity)
{
	dirtyEntities.Add(&entity);
}

void SystemManager::ProcessDirtyEntities()
{
	for (const auto& entity : dirtyEntities)
	{
		Assert(entity != nullptr);
		AddEntityToRequiredSystems(*entity);
	}
}

void SystemManager::TickSystems(float deltaTime)
{
	for (auto system : systemDependencyTree)
	{
		system->Tick(deltaTime);
	}
}

void SystemManager::AddEntityToRequiredSystems(const Entity& entity)
{
	ComponentSet groupOnEntity = GetComponentDescriptionOf(entity);
	for (auto system : systemDependencyTree)
	{
		ComponentSet desc = system->GetRequiredComponents();
		if (groupOnEntity.ContainsTypes(desc))
		{
			system->AssociateEntity(entity);
		}
	}
}


SystemManager& GetSystemsManager()
{
	return SystemManager::Instance();
}

}