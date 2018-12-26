#include "ph_contact_generator.h"

#include "ph_contact.h"

#include "../dynamics/ph_world_internal.h"
#include "../dynamics/ph_rigidbody.h"

void 
ph_contact_generate(ph_world const *i_world, ph_contact_collection *o_contacts)
{
    ph_contact_collection_reset(o_contacts);

    ph_rigidbody *body = i_world->head;

    while (body != nullptr)
    {
        ph_vec3 const &pos = body->transform.position;

        if (pos.y < 0) 
        {
            ph_contact *contact = ph_contact_collection_create(o_contacts);
            contact->a = body;
            contact->b = nullptr;
            contact->restitution = 1.0f;
            contact->contact_normal = ph_vec3_up;
            contact->penetration = -pos.y;
        }
         
        body = body->next;
    }
}
