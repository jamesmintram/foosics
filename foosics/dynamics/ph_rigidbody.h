#pragma once

struct ph_alloc;

struct ph_rigidbody
{
	ph_rigidbody *next;
	ph_rigidbody *prev;
};

// Lifecycle
ph_rigidbody *ph_rigidbody_create(ph_alloc *i_allocator);
void ph_rigidbody_destroy(ph_alloc *i_allocator, ph_rigidbody *i_rigidBody);

ph_rigidbody *ph_rigidbody_list_add(ph_rigidbody *head, ph_rigidbody *newRigidBody);
ph_rigidbody *ph_rigidbody_list_remove(ph_rigidbody *head, ph_rigidbody *oldRigidBody);
