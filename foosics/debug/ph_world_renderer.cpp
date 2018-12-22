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
		0xFFFF0000,
		0xFF00FFFF,
	};

	//TODO: Render the ground plane as a grid

	//TODO: Foreach body, render a cube at its position

	vec3 lineStart = { -5, -2, 2 };
	vec3 linePoints[] = 
	{ 
		{  5, -2, 2 }, 
		{  5, 2, 2 },
		{ -5, 2, 2 },
		{ -5, -2, 2 },
	};

	renderer->SetPenColor(0xFFFFFFFF);// 222222);
	for (int x = 0; x < 11; x++) 
	{
		//for (int y = 0; y < 10; y++)
		//{
			vec3 vLineStart	= { x - 0.5f, 0,  -0.5f };
			vec3 vLineEnd	= { x - 0.5f, 0, 9.5f };

			renderer->SetPen((float*)&vLineStart);
			renderer->DrawLine((float*)&vLineEnd);

			vec3 hLineStart = { -0.5f, 0, x - 0.5f };
			vec3 hLineEnd	= { 9.5f, 0, x - 0.5f };

			renderer->SetPen((float*)&hLineStart);
			renderer->DrawLine((float*)&hLineEnd);
		//}
	}

	static float ctr = 0;
	vec3 cubePos = { 0, 0, 0 };
	vec3 cubeRot = { 0, ctr += 0.01f, 0 };

	renderer->SetPenColor(colors[0]);
	renderer->DrawCube((float*)&cubePos, (float*)&cubeRot);

	renderer->SetPenColor(colors[1]);
	cubePos.x = 5;
	//renderer->DrawCube((float*)&cubePos, (float*)&cubeRot);


	renderer->Render();
}

