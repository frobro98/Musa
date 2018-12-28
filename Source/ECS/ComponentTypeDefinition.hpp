#pragma once

#include "EngineCore/PlatformDefinitions.h"

#include <type_traits>
WALL_WRN_PUSH
#include <bitset>
WALL_WRN_POP

#include "EngineCore/Types.h"
#include "EngineCore/Algorithms.hpp"
#include "Utilities/TemplateUtils.hpp"
#include "EngineCore/HashBasicTypes.h"
#include "Utilities/CompileTimeCounter.hpp"

namespace Musa
{

namespace Internal
{
class ComponentTypeCounter
{
public:
	static uint32 GetNewID() { return counter++; }

	static uint32 counter;
};

static constexpr size_t TypeStorageBytes = 64;
}

using ComponentSet = std::bitset<Internal::TypeStorageBytes>;

struct ComponentType
{
	uint32 bitIndex;

	constexpr ComponentType(uint32 shift)
		: bitIndex(shift)
	{
		Assert(shift < Internal::TypeStorageBytes);
	}

	constexpr ComponentType(const ComponentType& other)
		: bitIndex(other.bitIndex)
	{
	}

	constexpr ComponentType& operator=(const ComponentType& other)
	{
		if (this != &other)
		{
			bitIndex = other.bitIndex;
		}

		return *this;
	}

	friend constexpr bool operator==(const ComponentType& lhs, const ComponentType& rhs)
	{
		return lhs.bitIndex == rhs.bitIndex;
	}

	friend constexpr bool operator!=(const ComponentType& lhs, const ComponentType& rhs)
	{
		return lhs.bitIndex != rhs.bitIndex;
	}
};

class ComponentGroupDescription
{
public:
	ComponentGroupDescription() = default;

private:
	template <typename Type>
	void AddComponent()
	{
		ComponentType cType = Type::GetComponentType();
		componentSet.set(cType.bitIndex);
	}

public:
	template <typename... Types>
	void AddComponents()
	{
		[[maybe_unused]] int dummy[] = {0, (AddComponent<Types>(), 0)...};
		hash = fnv(componentSet.to_string().c_str(), componentSet.size());
	}

	void AddComponentWithType(ComponentType type)
	{
		componentSet.set(type.bitIndex);
	}

	bool ContainsType(ComponentType type)
	{
		return componentSet[type.bitIndex];
	}

	bool ContainsTypes(const ComponentGroupDescription& types)
	{
		return (types.componentSet & componentSet) == types.componentSet;
	}

	uint32 GetHash()
	{
		return hash;
	}

	friend bool operator==(const ComponentGroupDescription& lhs, const ComponentGroupDescription& rhs)
	{
		return lhs.componentSet == rhs.componentSet;
	}

	friend bool operator!=(const ComponentGroupDescription& lhs, const ComponentGroupDescription& rhs)
	{
		return lhs.componentSet != rhs.componentSet;
	}

private:
	ComponentSet componentSet;
	uint32 hash;
	uint32 pad[1] = {};
};

template <typename... Components>//, typename = std::enable_if_t<all_base_of_v<Component, Components...>>>
ComponentGroupDescription CreateComponentDescription()
{
	ComponentGroupDescription desc;
	desc.AddComponents<Components...>();
	return desc;
}

}

#define DECLARE_COMPONENT(type)						\
static const Musa::ComponentType component_##type;	\
static Musa::ComponentType GetComponentType()		\
{													\
	return component_##type;						\
}

#define DEFINE_COMPONENT(type) \
const Musa::ComponentType type::component_##type = Musa::ComponentType(Musa::Internal::ComponentTypeCounter::counter++);


inline uint32 GetHash(Musa::ComponentGroupDescription group)
{
	return group.GetHash();
}

inline uint32 GetHash(Musa::ComponentType type)
{
	return type.bitIndex;
}


