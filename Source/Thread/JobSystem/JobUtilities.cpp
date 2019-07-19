
#include "JobUtilities.hpp"

namespace
{
thread_local Job* activeParentJob = nullptr;
}

void SetActiveJobParent(Job& job)
{
	activeParentJob = &job;
}

Job& GetActiveJobParent()
{
	return *activeParentJob;
}