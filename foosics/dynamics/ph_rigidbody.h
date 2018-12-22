#pragma once

#include "../common/ph_vec3.h"
#include "../common/ph_transform.h"

struct ph_alloc;

struct ph_rigidbody
{
	ph_rigidbody *next;
	ph_rigidbody *prev;

	ph_vec3 force;
	ph_vec3 velocity;

	ph_transform transform;

	float   inv_mass;	//Derived from fixtures
};

void ph_rigidbody_integrate(ph_rigidbody *i_rigidBody, float i_timestep);

// Dynamics
void ph_rigidbody_add_force(ph_rigidbody *i_rigidBody, ph_vec3 const& i_force);

// Lifecycle
ph_rigidbody *ph_rigidbody_create(ph_alloc *i_allocator);
void ph_rigidbody_destroy(ph_alloc *i_allocator, ph_rigidbody *i_rigidBody);

ph_rigidbody *ph_rigidbody_list_add(ph_rigidbody *head, ph_rigidbody *newRigidBody);
ph_rigidbody *ph_rigidbody_list_remove(ph_rigidbody *head, ph_rigidbody *oldRigidBody);
