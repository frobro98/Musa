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

	bool IsValid() const;
	operator bool() const;

	[[nodiscard]] OwnedType* Get() const;
	[[nodiscard]] OwnedType* Release();
	void Reset(OwnedType* newPtr = nullptr);

	void Swap(UniquePtr& other);

private:
	void ReleaseOwnedPtr();

private:
	OwnedType* ptr = nullptr;
};

template <typename Type>
inline void Swap(UniquePtr<Type>& lhs, UniquePtr<Type>& rhs)
{
	lhs.Swap(rhs);
}

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
	ReleaseOwnedPtr();
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
		ReleaseOwnedPtr();
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
inline bool UniquePtr<OwnedType>::IsValid() const
{
	return ptr != nullptr;
}

template<typename OwnedType>
UniquePtr<OwnedType>::operator bool() const
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
	ReleaseOwnedPtr();
	ptr = newPtr;
}

template<typename OwnedType>
inline void UniquePtr<OwnedType>::Swap(UniquePtr& other)
{
	OwnedType* tmp = ptr;
	ptr = other.ptr;
	other.ptr = tmp;
}

template<typename OwnedType>
inline void UniquePtr<OwnedType>::ReleaseOwnedPtr()
{
	delete ptr;
	ptr = nullptr;
}

template<typename OwnedType>
inline OwnedType* UniquePtr<OwnedType>::Release()
{
	OwnedType* p = ptr;
	ptr = nullptr;
	return p;
}

