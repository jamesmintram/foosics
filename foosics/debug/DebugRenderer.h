#pragma once
#pragma once

#include <stdint.h>

class IDebugRenderer
{
public: 
	virtual void SetViewProj(float const *view, float const *projection) = 0;
	virtual void SetViewport(int x, int y, int w, int h) = 0;
	virtual void Render() = 0;
	virtual void SetPenColor(uint32_t newColor) = 0;
	virtual void SetPen(float *newPenPos) = 0;
	virtual void DrawPoint() = 0;
	virtual void DrawLine(float *endPoint) = 0;
	virtual void DrawCube(float const *pos, float const *rot) = 0;
};
