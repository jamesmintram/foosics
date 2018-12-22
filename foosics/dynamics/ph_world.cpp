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

void ph_world_step()
{
	/*#
	TODO:

	Force accum:		
		Gravity

	Integrate:
		Foreach object
			-> Calculate accell
			-> Add accell to vel
			-> Add vel to position
			-> Drag for accell and vel?
	*/
}
