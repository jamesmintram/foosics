#pragma once

struct ph_world;

struct ph_alloc;
struct ph_rigidbody;

// Lifecycle
ph_world       *ph_world_create(ph_alloc *i_allocator);

// Rigidbody 
ph_rigidbody   *ph_world_create_rigidbody(ph_world *world);
void			ph_world_destroy_rigidbody(ph_world *world, ph_rigidbody *i_rigidBody);