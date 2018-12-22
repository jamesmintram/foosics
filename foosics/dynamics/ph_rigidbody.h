#pragma once

#include <stdint.h>

#include "../common/ph_vec3.h"
#include "../common/ph_transform.h"

struct ph_alloc;

struct ph_rigidbody
{
    ph_transform transform;
    ph_vec3 velocity;

    // Accumulators
    ph_vec3 force;
    ph_vec3 accell;

    float   inv_mass;	//TODO: Derived from fixtures

    ph_rigidbody *next;
    ph_rigidbody *prev;
};


// Dynamics
void            ph_rigidbody_set_mass(ph_rigidbody *i_rigidBody, float i_mass);
void            ph_rigidbody_set_position(ph_rigidbody *i_rigidBody, ph_vec3 const& i_pos);


void            ph_rigidbody_add_accell(ph_rigidbody *i_rigidBody, ph_vec3 const& i_accell);
void            ph_rigidbody_add_force(ph_rigidbody *i_rigidBody, ph_vec3 const& i_force);

// Internal
void            ph_rigidbody_integrate(ph_rigidbody *i_rigidBodies, uint32_t count, float i_timestep);

// Lifecycle
ph_rigidbody   *ph_rigidbody_create(ph_alloc *i_allocator);
void            ph_rigidbody_destroy(ph_alloc *i_allocator, ph_rigidbody *i_rigidBody);

ph_rigidbody   *ph_rigidbody_list_add(ph_rigidbody *head, ph_rigidbody *newRigidBody);
ph_rigidbody   *ph_rigidbody_list_remove(ph_rigidbody *head, ph_rigidbody *oldRigidBody);
