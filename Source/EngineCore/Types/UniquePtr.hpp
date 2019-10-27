#pragma once

#include <type_traits>

template <typename OwnedType>
class UniquePtr
{
	static_assert(!std::is_array_v<OwnedType>, "Cannot use dynamic arrays in unique ptr");
public:
	UniquePtr() = default;
	UniquePtr(OwnedType* owningPtr);
	~UniquePtr();

	UniquePtr(UniquePtr&&);
	UniquePtr& operator=(UniquePtr&&);

	UniquePtr(const UniquePtr&) = delete;
	UniquePtr& operator=(const UniquePtr&) = delete;

	OwnedType& operator*() const;
	OwnedType* operator->() const;

	operator bool() const;

	OwnedType* Get() const;
	void Reset(OwnedType* newPtr);
	void Release();

private:
	OwnedType* ptr = nullptr;
};

template<typename OwnedType, typename... Args>
inline UniquePtr<OwnedType> MakeUnique(Args&&... args)
{
	return UniquePtr<OwnedType>(new OwnedType(std::forward<Args>(args)...));
}

template<typename OwnedType>
inline UniquePtr<OwnedType>::UniquePtr(OwnedType* owningPtr)
	: ptr(owningPtr)
{
}

template<typename OwnedType>
inline UniquePtr<OwnedType>::~UniquePtr()
{
	Release();
}

template<typename OwnedType>
inline UniquePtr<OwnedType>::UniquePtr(UniquePtr&& other)
{
	ptr = other.ptr;
	other.ptr = nullptr;
}

template<typename OwnedType>
inline UniquePtr<OwnedType>& UniquePtr<OwnedType>::operator=(UniquePtr&& other)
{
	if (this != &other)
	{
		Release();
		ptr = other.ptr;
		other.ptr = nullptr;
	}

	return *this;
}

template<typename OwnedType>
inline OwnedType& UniquePtr<OwnedType>::operator*() const
{
	return *ptr;
}

template<typename OwnedType>
inline OwnedType* UniquePtr<OwnedType>::operator->() const
{
	return ptr;
}

template<typename OwnedType>
inline UniquePtr<OwnedType>::operator bool() const
{
	return ptr != nullptr;
}

template<typename OwnedType>
inline OwnedType* UniquePtr<OwnedType>::Get() const
{
	return ptr;
}

template<typename OwnedType>
inline void UniquePtr<OwnedType>::Reset(OwnedType* newPtr)
{
	Release();
	ptr = newPtr;
}

template<typename OwnedType>
inline void UniquePtr<OwnedType>::Release()
{
	delete ptr;
	ptr = nullptr;
}

