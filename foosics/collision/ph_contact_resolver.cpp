#include "ph_contact_resolver.h"

#include "ph_contact.h"
#include "../dynamics/ph_rigidbody.h"

#include <float.h>

static float 
calculate_separating_velocity(ph_contact &i_contact)
{
    ph_vec3 vel = i_contact.a->velocity;
    if (i_contact.b != nullptr) ph_vec3_sub(vel, vel, i_contact.b->velocity);

    float const result = ph_vec3_dot(vel, i_contact.contact_normal);
    return result;
}

static void 
resolve_interpenetration(ph_contact &i_contact)
{
    //TODO:
}

static void 
apply_impulse(ph_rigidbody &i_body, ph_vec3 const &i_impulse_per_inv_mass, float const i_inv_mass_factor)
{
    ph_vec3 delta_vel_a;
    ph_vec3_mul(delta_vel_a, i_impulse_per_inv_mass, i_body.inv_mass * i_inv_mass_factor);

    ph_vec3 new_vel_a;
    ph_vec3_add(new_vel_a, i_body.velocity, delta_vel_a);

    i_body.velocity = new_vel_a;
}

static void 
resolve_velocity(ph_contact &i_contact)
{
    float const separating_vel = calculate_separating_velocity(i_contact);

    if (separating_vel >= 0) return;

    float const new_separating_vel = -separating_vel * i_contact.restitution;
    float const delta_vel = new_separating_vel - separating_vel;

    float const total_inv_mass = (i_contact.b != nullptr)
        ? i_contact.a->inv_mass + i_contact.b->inv_mass
        : i_contact.a->inv_mass;

    if (total_inv_mass <= 0) return;

    float const impulse = delta_vel / total_inv_mass;

    ph_vec3 impulse_per_inv_mass;
    ph_vec3_mul(impulse_per_inv_mass, i_contact.contact_normal, impulse);

    apply_impulse(*i_contact.a, impulse_per_inv_mass, 1.0f);

    if (i_contact.b != nullptr)
    {
        apply_impulse(*i_contact.b, impulse_per_inv_mass, -1.0f);
    }
}

static void 
resolve_contact(ph_contact &i_contact, float i_timestep)
{
    resolve_velocity(i_contact);
    resolve_interpenetration(i_contact);
}

void 
ph_resolve_collection(ph_contact_collection *i_contacts, float i_timestep, uint32_t iterations)
{
    for (uint32_t iteration = 0; iteration < iterations; iteration++)
    {
        uint32_t const count = i_contacts->count;

        float max_inbound = FLT_MAX;
        uint32_t max_idx = count;

        for (uint32_t cIdx = 0; cIdx < count; cIdx++)
        {
            ph_contact &contact = i_contacts->contacts[cIdx];
            
            float const separating_vel = calculate_separating_velocity(contact);
            bool const is_interesting = separating_vel < 0 || contact.penetration > 0;
           
            if (separating_vel < max_inbound && is_interesting)
            {
                max_idx = cIdx;
                max_inbound = separating_vel;
            }
        }

        if (max_idx == count)
        {
            break;
        }

        ph_contact &selected_contact = i_contacts->contacts[max_idx];
        resolve_contact(selected_contact, i_timestep);
    }
}
