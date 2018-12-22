#include "ph_world.h"
#include "ph_world_internal.h"

#include "ph_rigidbody.h"
#include "../ph_alloc.h"

//@@ Lifecycle

ph_world *ph_world_create(ph_alloc *i_allocator)
{
    ph_world *newWorld = (ph_world*)ph_alloc_allocate(i_allocator, sizeof(ph_world));

    newWorld->allocator = i_allocator;
    newWorld->head = nullptr;

    return newWorld;
}

//@@ Rigidbody

ph_rigidbody *ph_world_create_rigidbody(ph_world *world)
{
    ph_rigidbody *newBody = ph_rigidbody_create(world->allocator);
	
    world->head = ph_rigidbody_list_add(world->head, newBody);

    return newBody;
}

void ph_world_destroy_rigidbody(ph_world *world, ph_rigidbody *i_rigidBody)
{
    world->head = ph_rigidbody_list_remove(world->head, i_rigidBody);

    ph_rigidbody_destroy(world->allocator, i_rigidBody);
}

const float kMinVelocity = 0.01f;

bool ph_calc_drag_coefficient(ph_vec3 &o_dest, ph_vec3 const& i_velocity)
{
    const float kK1 = 1.01f;
    const float kK2 = 1.01f;

    float drag_coeff = ph_vec3_magnitude(i_velocity);
    drag_coeff = kK1 * drag_coeff + kK2 * drag_coeff * drag_coeff;

    if (drag_coeff < kMinVelocity)
    {
        return false;
    } 

    ph_vec3 drag_vector;
    ph_vec3_normalise(drag_vector, i_velocity);
    ph_vec3_mul(o_dest, drag_vector, -drag_coeff);
        
    return true;
}

void ph_world_step(ph_world *world)
{
    ph_rigidbody *body = world->head;

    ph_vec3 gravity = { 0, -9.81f, 0 };
    float timestep = 1.0f / 250;

    while (body != nullptr) 
    {
        //Figure out drag
        ph_vec3 drag_vector;
        if (ph_calc_drag_coefficient(drag_vector, body->velocity))
        {
            ph_rigidbody_add_force(body, drag_vector);
        }
        
        
        ph_rigidbody_add_accell(body, gravity);
        ph_rigidbody_integrate(body, 1, timestep);

        body = body->next;
    }
}
