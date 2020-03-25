#pragma once

#include "EngineCore/PlatformDefinitions.h"

#include <type_traits>
WALL_WRN_PUSH
#include <bitset>
WALL_WRN_POP

#include "Types/Intrinsics.hpp"
#include "Algorithms.hpp"
#include "HashBasicTypes.h"
#include "Utilities/TemplateUtils.hpp"
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

struct ComponentType
{
	size_t typeSize;
	uint32 bitIndex;

	constexpr ComponentType(size_t compTypeSize, uint32 shift)
		: typeSize(compTypeSize),
		bitIndex(shift)
	{
		//Assert(typeSize >= sizeof(Component));
		Assert(shift < Internal::TypeStorageBytes);
	}

	constexpr ComponentType(const ComponentType& other)
		: typeSize(other.typeSize),
		bitIndex(other.bitIndex)
	{
	}

	friend constexpr bool operator==(const ComponentType& lhs, const ComponentType& rhs)
	{
		return lhs.typeSize == rhs.typeSize && lhs.bitIndex == rhs.bitIndex;
	}

	friend constexpr bool operator!=(const ComponentType& lhs, const ComponentType& rhs)
	{
		return lhs.typeSize != rhs.typeSize && lhs.bitIndex != rhs.bitIndex;
	}
};

struct ComponentSet
{
	bool ContainsType(const ComponentType& type)
	{
		return set[type.bitIndex];
	}

	bool ContainsTypes(const ComponentSet& types)
	{
		return (types.set & set) == types.set;
	}

	std::bitset<Internal::TypeStorageBytes> set;
	uint32 hash;
};

inline bool operator==(const ComponentSet& lhs, const ComponentSet& rhs)
{
	return lhs.set == rhs.set;
}

inline bool operator!=(const ComponentSet& lhs, const ComponentSet& rhs)
{
	return lhs.set != rhs.set;
}

template <typename Type>
inline void AddComponentTo(ComponentSet& desc)
{
	ComponentType cType = Type::GetComponentType();
	desc.set.set(cType.bitIndex);
}

template <typename... Types>
inline void AddComponentsTo(ComponentSet& desc)
{
	[[maybe_unused]] int dummy[] = { 0, (AddComponentTo<Types>(desc), 0)... };
	desc.hash = fnv(desc.set.to_string().c_str(), (uint32)desc.set.size());
}

inline void AddComponentWithTypeTo(ComponentType type, ComponentSet& desc)
{
	desc.set.set(type.bitIndex);
}

template <typename CompType>
inline void AddComponentWithTypeTo(ComponentSet& desc)
{
	ComponentType type = CompType::GetComponentType();
	desc.set.set(type.bitIndex);
}

template <typename... Components, typename = std::enable_if_t<all_base_of_v<Musa::Component, Components...>>>
inline ComponentSet CreateComponentDescription()
{
	ComponentSet desc;
	AddComponentsTo<Components...>(desc);
	return desc;
}

}

// TODO - For scene loading, this won't work because each set index isn't reproducible. Not a good solution in the end sadly...
#define DECLARE_COMPONENT(type)							\
static const Musa::ComponentType component_##type;		\
static inline Musa::ComponentType GetComponentType()	\
{														\
	return component_##type;							\
}

#define DEFINE_COMPONENT(type) \
const Musa::ComponentType type::component_##type = Musa::ComponentType(sizeof(type), Musa::Internal::ComponentTypeCounter::counter++);


inline uint32 GetHash(Musa::ComponentSet group)
{
	return group.hash;
}

inline uint32 GetHash(Musa::ComponentType type)
{
	return type.bitIndex;
}


