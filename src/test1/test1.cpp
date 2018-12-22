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

    for (int idx = 0; idx < 4; idx++)
    {
        //TODO: This will be moved into fixtures 
        ph_rigidbody_set_mass(bodies[idx], idx * 500);
        ph_rigidbody_set_position(bodies[idx], { (float)idx, 10, 0 });
    }

    ph_world_destroy_rigidbody(world, bodies[2]);
    ph_world_destroy_rigidbody(world, bodies[0]);
}
