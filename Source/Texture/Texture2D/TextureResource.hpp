// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class Texture;

// TODO - Currently, if sampler information is changed on the texture, it doesn't recreate
// the sampler at all. 
struct TextureResource
{
	void UpdateTextureResource(const Texture& tex);

	UniquePtr<NativeTexture> texResource;
	UniquePtr<NativeSampler> samplerResource;
};
