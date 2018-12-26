#pragma once

#include <stdint.h>

struct ph_world;
struct ph_contact_collection;

void ph_contact_generate(ph_world const *i_world, ph_contact_collection *o_contacts);