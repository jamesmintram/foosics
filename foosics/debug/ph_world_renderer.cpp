#include "ph_world_renderer.h"

#include "../dynamics/ph_world_internal.h"
#include "DebugRenderer.h"

//TEMP:
struct vec3
{
	float x;
	float y;
	float z;
};

void ph_world_debug_render(IDebugRenderer *renderer, ph_world *world)
{
	uint32_t colors[] = {
		0xFF0000FF,
		0xFF00FF00,
		0xFFFF0000
	};

	//TODO: Render the ground plane as a grid

	//TODO: Foreach body, render a cube at its position

	for (int t = 0; t < 10; t++)
	{
		float y = 2.0f * t - 1.0f;

		vec3 lineStart = { -100, y, 10.5f };
		vec3 lineEnd = { 100, y, 10.5f };

		renderer->SetPenColor(colors[t % 3]);
		renderer->SetPen((float*)&lineStart);
		renderer->DrawLine((float*)&lineEnd);
	}

	renderer->Render();
}

