

#pragma once

template<typename T, T val>
struct integral_constant
{
	static constexpr T value = val;

	using value_type = T;
	using type = integral_constant;

	constexpr operator value_type() const noexcept
	{
		return value;
	}

	NODISCARD constexpr value_type operator()() const noexcept
	{
		return value;
	}
};

template<bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

/**** remove_* definitions ****/
//////////////////////////////////////////////////////////////////////////
// remove_const
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct remove_const
{
	using type = T;
};

template<typename T>
struct remove_const<const T>
{
	using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

//////////////////////////////////////////////////////////////////////////
// remove_volatile
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct remove_volatile
{
	using type = T;
};

template<typename T>
struct remove_volatile<volatile T>
{
	using type = T;
};

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

//////////////////////////////////////////////////////////////////////////
// remove_cv
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct remove_cv
{
	using type = T;
};

template<typename T>
struct remove_cv<const T>
{
	using type = T;
};

template<typename T>
struct remove_cv<volatile T>
{
	using type = T;
};

template<typename T>
struct remove_cv<const volatile T>
{
	using type = T;
};

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

//////////////////////////////////////////////////////////////////////////
// remove_reference
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct remove_reference
{
	using type = T;
};

template<typename T>
struct remove_reference<T&>
{
	using type = T;
};

template<typename T>
struct remove_reference<T&&>
{
	using type = T;
};

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

//////////////////////////////////////////////////////////////////////////
// conditional
//////////////////////////////////////////////////////////////////////////
template<bool Test,
	typename T0, typename T1>
	struct conditional
{
	using type = T1;
};

template<typename T0, typename T1>
struct conditional<true, T0, T1>
{
	using type = T0;
};

template<bool Test,
	typename T0, typename T1>
	using conditional_t = typename conditional<Test, T0, T1>::type;

template<typename... Args>
using void_t = void;


#define MOVE(value) static_cast<remove_reference_t<decltype(value)> &&>(value)
#define FORWARD(type, value) static_cast<type &&>(value)
