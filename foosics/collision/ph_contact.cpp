#include "ph_contact.h"

#include "../common/ph_assert.h"

void ph_contact_collection_reset(ph_contact_collection *i_collection)
{
    i_collection->count = 0;
}

ph_contact * ph_contact_collection_create(ph_contact_collection *i_collection)
{
	PH_ASSERT(i_collection->count < kMaxContactsCount);

    ph_contact *contact = &i_collection->contacts[i_collection->count];
    i_collection->count++;

    contact->a = nullptr;
    contact->b = nullptr;
    contact->contact_normal = ph_vec3_zero;
    contact->restitution = 1.0f;
    contact->penetration = 0.0f;

    return contact;
}
