#pragma once

#ifdef FILE_EXPORT
#define FILE_API __declspec(dllexport)
#else
#define FILE_API __declspec(dllimport)
#endif
