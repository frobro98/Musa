@echo off

pushd Tools

echo Converting Stanford Bunny
Converter.exe ../Assets/Fbx/stanford-bunny.fbx > bunny.log

echo Converting War Bear
Converter.exe ../Assets/Fbx/warbear.fbx -t ../Assets/Fbx/polarbear.tga -c bc7 > warbear.log

echo Converting Buggy
Converter.exe ../Assets/Fbx/buggy.fbx -t ../Assets/Fbx/buggy_spec.tga -c bc7 > buggy.log

echo Converting Astroboy
Converter.exe ../Assets/Fbx/astroboy.fbx -c bc7 > astroboy.log

echo Converting Space Frigate
Converter.exe ../Assets/Fbx/space_frigate.fbx -c bc7 > frigate.log

echo Converting Geth Trooper
Converter.exe ../Assets/Fbx/Geth_Trooper.fbx -c bc7 > trooper.log

popd