@echo off

pushd Tools

rem echo Converting Stanford Bunny
rem Converter.exe ../Assets/Fbx/stanford-bunny.fbx > bunny.log

rem echo Converting War Bear
rem Converter.exe ../Assets/Fbx/warbear.fbx -t ../Assets/Fbx/polarbear.tga -c bc7 > warbear.log

rem echo Converting Buggy
rem Converter.exe ../Assets/Fbx/buggy.fbx -t ../Assets/Fbx/buggy_spec.tga -c bc7 > buggy.log

echo Converting Astroboy
Converter.exe ../Assets/Fbx/astro-boy.fbx -c bc7 > astroboy.log

rem echo Converting Space Frigate
rem Converter.exe ../Assets/Fbx/space_frigate.fbx -c bc7 > frigate.log

echo Converting Geth Trooper
Converter.exe ../Assets/Fbx/geth-trooper.fbx -c bc7 > trooper.log

PAUSE

popd