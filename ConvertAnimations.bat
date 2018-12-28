@echo off

pushd Tools

echo Converting humanoid...
call Converter.exe ../Assets/Fbx/humanoid.fbx > nul

echo Converting teddy...
call Converter.exe ../Assets/Fbx/teddy_TGA.fbx > nul

popd