#pragma once

#include "UI/UIWidget.hpp"

class UIWindow : public UIWidget
{
public:



	inline void SetDim(uint32 sizeX, uint32 sizeY)
	{
		dimensionX = sizeX;
		dimensionY = sizeY;
	}
	inline uint32 GetDimX() const { return dimensionX; }
	inline uint32 GetDimY() const { return dimensionY; }

private:
	uint32 dimensionX;
	uint32 dimensionY;
};

