// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Debugging/Assertion.hpp"

// TODO - To implement function correctly, there needs to be a lot of boiler plate. Not priority...
template <size_t size, size_t alignment = 4>
struct AlignedStorage
{
	struct Type
	{
		alignas(alignment) u8 data[size];
	};
};

template <size_t size, size_t alignment = 4>
using AlignedStorage_t = typename AlignedStorage<size, alignment>::Type;


constexpr size_t SmallBuffSize = sizeof(void*);

using FuncStorage = typename AlignedStorage_t<SmallBuffSize>;

template <typename Ret, typename... Params>
struct CallableBase
{
	CallableBase() = default;
	virtual ~CallableBase() = default;

	virtual Ret Invoke(Params&&...) = 0;
	virtual CallableBase* Copy(FuncStorage& storage) = 0;
	virtual CallableBase* Move(FuncStorage& storage) = 0;
};

template <typename Call, typename Ret, typename... Params>
struct Callable : CallableBase<Ret, Params...>
{
	using Base = CallableBase<Ret, Params...>;
	template <typename Func>
	Callable(Func&& f)
		: call(FORWARD(Func, f))
	{
	}

	virtual Base* Copy(FuncStorage& storage) override
	{
		if constexpr(sizeof(*this) <= sizeof(storage))
		{
			return new(std::addressof(storage)) Callable(call);
		}
		else
		{
			return new Callable(call);
		}
	}
	virtual Base* Move(FuncStorage& storage) override
	{

	}

	virtual Ret Invoke(Params&&... params) override
	{
		return std::invoke(call, params...);
	}

	Call call;
};

template <typename>
class Function;

template <typename Ret, typename... Params>
class Function<Ret(Params...)> 
{
public:
	template <typename Func, typename = std::enable_if_t<
		std::is_invocable_v<Func, Params...>>
	>
	Function(Func&& f)
	{
		using CallType = Callable<std::decay<Func>, Ret, Params...>;
		if constexpr (sizeof(CallType) <= sizeof(storage))
		{
			callable = new(std::addressof(storage)) CallType(
				FORWARD(Func, f)
			);
		}
		else
		{
			callable = new CallType(
				FORWARD(Func, f)
			);
		}
	}

	Function(const Function& func)
	{
		if (func.IsValid())
		{
			callable = func.callable->Copy(storage);
		}
	}

	Function(Function&& func)
	{
		if (func.IsValid())
		{
			if (func.callable == func.Store())
			{
				callable = func.callable->Move(storage);
			}
			else
			{
				callable = func.callable;
				func.callable = nullptr;
			}
		}
	}

	Ret operator()(Params&&... params)
	{
		Assert(callable);
		return callable->Invoke(params...);
	}

	bool IsValid() const
	{
		return callable != nullptr;
	}

private:
	void* Store()
	{
		return std::addressof(storage);
	}

private:
	CallableBase<Ret, Params...>* callable = nullptr;
	FuncStorage storage;
};
