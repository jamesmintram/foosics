#pragma once

#include <stdint.h>

#include "../common/ph_vec3.h"

struct ph_rigidbody;

struct ph_contact {
    ph_rigidbody *a;
    ph_rigidbody *b;

    //TODO: Reference the fixtures involved?

    ph_vec3 contact_normal;
    float restitution;

    float penetration;
};

const uint32_t kMaxContactsCount = 64;

struct ph_contact_collection
{
    ph_contact contacts[kMaxContactsCount];
    uint32_t count;
};

void        ph_contact_collection_reset(ph_contact_collection  *i_collection);
ph_contact *ph_contact_collection_create(ph_contact_collection  *i_collection);
