#include "ph_rigidbody.h"

#include "../ph_alloc.h"

ph_rigidbody *ph_rigidbody_create(ph_alloc *i_allocator)
{
	ph_rigidbody *newBody = (ph_rigidbody*)ph_alloc_allocate(i_allocator, sizeof(ph_rigidbody));

	newBody->next = nullptr;
	newBody->prev = nullptr;

	return newBody;
}

void ph_rigidbody_destroy(ph_alloc *i_allocator, ph_rigidbody *i_rigidBody)
{
	ph_alloc_free(i_allocator, i_rigidBody);
}

ph_rigidbody *ph_rigidbody_list_add(ph_rigidbody *head, ph_rigidbody *newRigidBody)
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

ph_rigidbody * ph_rigidbody_list_remove(ph_rigidbody *head, ph_rigidbody *oldRigidBody)
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
