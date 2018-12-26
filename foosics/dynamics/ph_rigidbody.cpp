#include "ph_rigidbody.h"

#include "../ph_alloc.h"

#include <float.h>

void 
ph_rigidbody_set_radius(ph_rigidbody *i_rigidBody, float i_radius)
{
    i_rigidBody->radius = i_radius;
}

void 
ph_rigidbody_set_mass(ph_rigidbody *i_rigidBody, float i_mass)
{
    if (i_mass == 0.0f)
    {
        //TODO: Warn
    }

    float inverse_mass = (i_mass > 0.0f)
        ? 1.0f / i_mass
        : 0.0f;

    i_rigidBody->inv_mass = inverse_mass;
}


//@@ Dynamics

void 
ph_rigidbody_set_position(ph_rigidbody *i_rigidBody, ph_vec3 const& i_pos)
{
    i_rigidBody->transform.position = i_pos;
}

void 
ph_rigidbody_add_accell(ph_rigidbody *i_rigidBody, ph_vec3 const& i_accell)
{
    ph_vec3_add(i_rigidBody->accell, i_rigidBody->accell, i_accell);

}

void 
ph_rigidbody_add_force(ph_rigidbody *i_rigidBody, ph_vec3 const& i_force)
{
    ph_vec3_add(i_rigidBody->force, i_rigidBody->force, i_force);
}


//@@ Internal

void 
ph_rigidbody_integrate(ph_rigidbody *i_rigidBodies, uint32_t count, float i_timestep)
{
    for (uint32_t idx = 0; idx < count; idx++)
    {
        ph_rigidbody &body = i_rigidBodies[idx];
        ph_transform &xform = body.transform;

        ph_vec3 accell;
        ph_vec3_mul(accell, body.force, body.inv_mass);
        ph_vec3_add(body.accell, body.accell, accell);

        ph_vec3_add_scaled(body.velocity, body.velocity, body.accell, i_timestep);
        ph_vec3_add_scaled(xform.position, xform.position, body.velocity, i_timestep);

        // Reset force
        body.force = ph_vec3_zero;
        body.accell = ph_vec3_zero;
    }
}


//@@ Lifecycle

ph_rigidbody*
ph_rigidbody_create(ph_alloc *i_allocator)
{
    ph_rigidbody *newBody = (ph_rigidbody*)ph_alloc_allocate(i_allocator, sizeof(ph_rigidbody));

    newBody->next = nullptr;
    newBody->prev = nullptr;

    newBody->force = ph_vec3_zero;
    newBody->accell = ph_vec3_zero;

    newBody->velocity = ph_vec3_zero;
    
    //TEMP
    newBody->inv_mass = 0.0f;
    newBody->radius = 0.2f;

    newBody->transform = ph_transform_zero;

    return newBody;
}

void 
ph_rigidbody_destroy(ph_alloc *i_allocator, ph_rigidbody *i_rigidBody)
{
    ph_alloc_free(i_allocator, i_rigidBody);
}

ph_rigidbody*
ph_rigidbody_list_add(ph_rigidbody *head, ph_rigidbody *newRigidBody)
{
    // First item added to the list?
    if (head == nullptr)
    {
        return newRigidBody;
    }

    // Link
    head->prev = newRigidBody;
    newRigidBody->next = head;

    // New item now at head of list
    return newRigidBody;
}

ph_rigidbody* 
ph_rigidbody_list_remove(ph_rigidbody *head, ph_rigidbody *oldRigidBody)
    {
    // Have we just removed the head?
    if (oldRigidBody->prev == nullptr)
    {
        return head->next;
    }

    // Unlink
    ph_rigidbody *prev = oldRigidBody->prev;
    oldRigidBody->prev->next = oldRigidBody->next;

    // Check we are not the last in the list
    if (oldRigidBody->next != nullptr)
    {
        oldRigidBody->next->prev = prev;
    }

    // Head has not changed
    return head;
}
