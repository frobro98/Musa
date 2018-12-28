@echo off

pushd Tools

echo Converting Red Brick texture with tinted mipmapping
TextureConverter.exe -f ..\Assets\Textures\RedBrick.tga -genmips fft -o ..\Assets\Textures\RedBrick.tex

echo. 
echo Converting default engine texture
TextureConverter.exe -f ..\Assets\Textures\default.tga -o ..\Assets\Textures\default.tex

echo Converting default engine texture with BC1 compression
TextureConverter.exe -f ..\Assets\Textures\default.tga -c bc1 -o ..\Assets\Textures\defaultBC1.tex

echo Converting default engine texture with BC3 compression
TextureConverter.exe -f ..\Assets\Textures\default.tga -c bc3 -o ..\Assets\Textures\defaultBC3.tex

echo Converting default engine texture with BC7 compression
TextureConverter.exe -f ..\Assets\Textures\default.tga -c bc7 -o ..\Assets\Textures\defaultBC7.tex

echo.
echo Converting JPEG
TextureConverter.exe -f ..\Assets\Textures\RedBrick.jpg -genmips fft -o ..\Assets\Textures\RedBrick_jpg.tex

echo Converting PNG
TextureConverter.exe -f ..\Assets\Textures\RedBrick.bmp -genmips fft -o ..\Assets\Textures\RedBrick_bmp.tex

echo Converting BMP
TextureConverter.exe -f ..\Assets\Textures\RedBrick.png -genmips fft -o ..\Assets\Textures\RedBrick_png.tex

popd