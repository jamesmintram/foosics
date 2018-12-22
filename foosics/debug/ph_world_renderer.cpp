#include "ph_world_renderer.h"

#include "../dynamics/ph_world_internal.h"
#include "../dynamics/ph_rigidbody.h"

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

    renderer->SetPenColor(0xFFFFFFFF);// 222222);

    for (int x = 0; x < 11; x++) 
    {
        vec3 vLineStart = { x - 0.5f, 0,  -0.5f };
        vec3 vLineEnd = { x - 0.5f, 0, 9.5f };

        renderer->SetPen((float*)&vLineStart);
        renderer->DrawLine((float*)&vLineEnd);

        vec3 hLineStart = { -0.5f, 0, x - 0.5f };
        vec3 hLineEnd = { 9.5f, 0, x - 0.5f };

        renderer->SetPen((float*)&hLineStart);
        renderer->DrawLine((float*)&hLineEnd);
    }

	

    static float ctr = 0;
    renderer->SetPenColor(colors[0]);

    ph_rigidbody *body = world->head;

    while (body != nullptr) 
    {
        vec3 cubePos = { 0, 0, 0 };
        vec3 cubeRot = { 0, ctr, 0 };

        renderer->DrawCube(
            (float const*)&body->transform.position,
            (float const*)&body->transform.rotation);
		
        body = body->next;
    }

    renderer->Render();
}

