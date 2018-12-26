#include "ph_contact_generator.h"

#include "ph_contact.h"

#include "../dynamics/ph_world_internal.h"
#include "../dynamics/ph_rigidbody.h"

void 
ph_contact_generate(ph_world const *i_world, ph_contact_collection *o_contacts)
{
    ph_contact_collection_reset(o_contacts);

    //Check against ground plane
    {
        ph_rigidbody *body = i_world->head;
        while (body != nullptr)
        {
            ph_vec3 const &pos = body->transform.position;

            if (pos.y < body->radius)
            {
                ph_contact *contact = ph_contact_collection_create(o_contacts);
                contact->a = body;
                contact->b = nullptr;
                contact->restitution = 0.4f;
                contact->contact_normal = ph_vec3_up;
                contact->penetration = -pos.y + body->radius;
                contact->restitution = body->restitution;
            }

            body = body->next;
        }
    }

    //Check against each other
    {
        ph_rigidbody *body_a = i_world->head;
        while (body_a != nullptr)
        {
            ph_vec3 const &pos_a = body_a->transform.position;
            
            
            for (ph_rigidbody *body_b = i_world->head; body_b != nullptr; body_b = body_b->next)
            {
                ph_vec3 const &pos_b = body_b->transform.position;
                
                float const radius_total = body_a->radius + body_b->radius;
                ph_vec3 dist_vec;
                ph_vec3_sub(dist_vec, pos_a, pos_b);
                
                float const centroid_distance = ph_vec3_magnitude(dist_vec);
                
                if (centroid_distance == 0) continue;

                float const penetration = radius_total - centroid_distance;

                if (penetration >= 0)
                {
                    ph_vec3_normalise(dist_vec, dist_vec);

                    ph_contact *contact = ph_contact_collection_create(o_contacts);
                    contact->a = body_a;
                    contact->b = body_b;
                    contact->contact_normal = dist_vec;
                    contact->penetration = penetration;
                    contact->restitution = (body_a->restitution + body_b->restitution) / 2.0f;
                }
            }

            body_a = body_a->next;
        }
    }
}
