@echo off

set glslang=C:\VulkanSDK\1.0.39.1\Bin\glslangValidator.exe
echo %glslang%

if exist "%glslang%" (
	rem Vertex shaders
 	for /R %%f in (*.vert) do (
		rem echo "%~dp0\Generated\%%~nf.vs.spv"
		%glslang% -V %%f -o "%~dp0\Generated\%%~nf.vs.spv"
		
		if errorlevel 1 goto :break
	)
	rem Fragment shaders 
	for /R %%f in (*.frag) do (
		rem echo "%~dp0\Generated\%%~nf.fs.spv"
		%glslang% -V %%f -o "%~dp0\Generated\%%~nf.fs.spv"
		if errorlevel 1 goto :break
	)
	rem Compute shaders
	for /R %%f in (*.comp) do (
		%glslang% -V %%f -o "%~dp0\Generated\%%~nf.cs.spv"
		if errorlevel 1 goto :break
	)
)

:break