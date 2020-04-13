#include "DirectoryLocations.h"
#include "String/CStringUtilities.hpp"


String EngineExeFullPath()
{
	HMODULE hCurrentModule;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&EngineAssetPath, &hCurrentModule);
	tchar pathResult[512] = "";
	GetModuleFileName(hCurrentModule, pathResult, (DWORD)ArraySize(pathResult));
	size_t pathLen = Strlen(pathResult);
	if (pathLen > 0)
	{
		String temp(pathResult);
		temp.Replace("\\", "/");
		Strcpy(pathResult, 512, *temp);

		--pathLen;
		for (; pathLen > 0; pathLen--)
		{
			if (pathResult[pathLen - 1] == '/' || pathResult[pathLen - 1] == '\\')
			{
				break;
			}
		}
	}
	pathResult[pathLen] = '\0';

	return pathResult;
}
