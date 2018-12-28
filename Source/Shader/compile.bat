@echo off

set glslang=C:\VulkanSDK\1.0.39.1\Bin\glslangValidator.exe
echo %glslang%

if exist "%glslang%" (
	rem Vertex shaders
 	for /R %%f in (*.vert) do (
		%glslang% -V %%f -o "%%~pf%%~nf.vs.spv"
		
		if errorlevel 1 goto :break
	)
	rem Fragment shaders 
	for /R %%f in (*.frag) do (
		%glslang% -V %%f -o "%%~pf%%~nf.fs.spv"
		if errorlevel 1 goto :break
	)
	rem Compute shaders
	for /R %%f in (*.comp) do (
		%glslang% -V %%f -o "%%~pf%%~nf.cs.spv"
		if errorlevel 1 goto :break
	)
)

:break