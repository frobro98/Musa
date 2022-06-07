

#pragma once

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


#define MOVE(value) static_cast<remove_reference_t<decltype(value)> &&>(value)
#define FORWARD(type, value) static_cast<type &&>(value)
