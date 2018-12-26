#pragma comment(lib, "d3d9.lib")

#include "../../foosics/debug/ph_world_renderer.h"

#include "../../foosics/ph_alloc.h"
#include "../../foosics/dynamics/ph_world.h"
#include "../../foosics/dynamics/ph_rigidbody.h"

ph_alloc crtAllocator;
ph_world *world;

void demoDraw(IDebugRenderer *worldRenderer)
{
    ph_world_step(world);
    ph_world_debug_render(worldRenderer, world);
}


// Add this

void demoInit()
{


    ph_alloc_crt_init(&crtAllocator);
    world = ph_world_create(&crtAllocator);

    ph_rigidbody *bodies[] =
    {
        ph_world_create_rigidbody(world),
        ph_world_create_rigidbody(world),
        ph_world_create_rigidbody(world),
        ph_world_create_rigidbody(world)
    };

    ph_rigidbody_set_position(bodies[0], { 4, 2, 0 });
    bodies[0]->inv_mass = 1.0f;
    bodies[0]->restitution = 0.0f;

    ph_rigidbody_set_position(bodies[1], { 4, 8, 0 });
    bodies[1]->restitution = 1.0f;

    ph_rigidbody_set_position(bodies[2], { 4, 16, 0 });
    bodies[2]->restitution = 1.0f;

    ph_rigidbody_set_position(bodies[3], { 4, 18, 0 });
    bodies[3]->restitution = 0.0f;
}
