#include "ph_vec3.h"

#include <math.h>

ph_vec3 ph_vec3_zero = { 0 };

float ph_vec3_magnitude(ph_vec3 const& i_a)
{
    float sqr_mag = 
        i_a.x * i_a.x +
        i_a.y * i_a.y +
        i_a.z * i_a.z;

    return sqrtf(sqr_mag);
}

void ph_vec3_add(ph_vec3 &o_dest, ph_vec3 const& i_a, ph_vec3 const& i_b)
{
    o_dest.x = i_a.x + i_b.x;
    o_dest.y = i_a.y + i_b.y;
    o_dest.z = i_a.z + i_b.z;
}

void ph_vec3_mul(ph_vec3 &o_dest, ph_vec3 const& i_a, float const i_b)
{
    o_dest.x = i_a.x * i_b;
    o_dest.y = i_a.y * i_b;
    o_dest.z = i_a.z * i_b;
}

void ph_vec3_add_scaled(ph_vec3 &o_dest, ph_vec3 const& i_a, ph_vec3 const& i_b, float const i_scale)
{
    o_dest.x = i_a.x + i_b.x * i_scale;
    o_dest.y = i_a.y + i_b.y * i_scale;
    o_dest.z = i_a.z + i_b.z * i_scale;
}