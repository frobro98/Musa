#include "System.hpp"
#include "SystemsManager.hpp"

namespace Musa
{
namespace Internal
{
System* GetSystemWithType(SystemType type)
{
	return GetSystemsManager().FindSystemOfType(type);
}
}

}