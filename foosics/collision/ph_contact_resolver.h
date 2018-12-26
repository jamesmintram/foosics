#pragma once

#include <stdint.h>

struct ph_contact_collection;

void ph_resolve_collection(ph_contact_collection *i_contacts, float i_timestep, uint32_t iterations);