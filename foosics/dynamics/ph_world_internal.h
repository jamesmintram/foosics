#pragma once

struct ph_alloc;

struct ph_rigidbody;

struct ph_world
{
	ph_alloc *allocator;
	ph_rigidbody *head;
};