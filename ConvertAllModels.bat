@echo off

pushd Tools

echo Converting War Bear
Converter.exe ../Assets/Fbx/warbear.fbx -t polarbear.tga -c bc7 > nul

echo Converting Buggy
Converter.exe ../Assets/Fbx/buggy.fbx -t buggy_spec.tga -c bc7 > nul

echo Converting Astroboy
Converter.exe ../Assets/Fbx/astroboy.fbx -c bc7 > nul

echo Converting Space Frigate
Converter.exe ../Assets/Fbx/space_frigate.fbx -c bc7 > nul

echo Converting Geth Trooper
Converter.exe ../Assets/Fbx/Geth_Trooper.fbx -c bc7 > nul

popd