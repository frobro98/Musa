
#include "PlayerInputContext.hpp"

PlayerInputContext MakeInputContext(const StringView& name)
{
	PlayerInputContext context = {};
	context.contextName = String(*name, name.Length());
	context.nameHash = GetHash(name);
	return context;
}
