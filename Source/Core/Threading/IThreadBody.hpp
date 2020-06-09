#pragma once

class IThreadBody
{
public:
	virtual ~IThreadBody() {}

	virtual void ThreadStart() {}
	virtual void ThreadBody() = 0;
	virtual void ThreadStop() {}
};
