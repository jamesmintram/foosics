#include "ph_vec3.h"

#include <math.h>
#include <float.h>

ph_vec3 ph_vec3_zero = { 0 };
ph_vec3 ph_vec3_up = { 0, 1, 0 };

void 
ph_vec3_normalise(ph_vec3 &o_dest, ph_vec3 const& i_a)
{
    float mag = ph_vec3_magnitude(i_a);
    if (mag < FLT_EPSILON)
    {
        //TODO: Assert
        return;
    }

    float inv_mag = 1.0f / mag;

    ph_vec3_mul(o_dest, i_a, inv_mag);
}

void 
ph_vec3_inverse(ph_vec3 &o_dest, ph_vec3 const& i_a)
{
    o_dest.x = -i_a.x;
    o_dest.y = -i_a.y;
    o_dest.z = -i_a.z;
}

float 
ph_vec3_sqr_magnitude(ph_vec3 const& i_a)
{
    return
        i_a.x * i_a.x +
        i_a.y * i_a.y +
        i_a.z * i_a.z;
}
float 
ph_vec3_magnitude(ph_vec3 const& i_a)
{
    float sqr_mag = ph_vec3_sqr_magnitude(i_a);
    return sqrtf(sqr_mag);
}

float 
ph_vec3_dot(ph_vec3 const& i_a, ph_vec3 const& i_b)
{
   return i_a.x * i_b.x + i_a.y * i_b.y + i_a.z * i_b.z;
}

void 
ph_vec3_sub(ph_vec3 &o_dest, ph_vec3 const& i_a, ph_vec3 const& i_b)
{
    o_dest.x = i_a.x - i_b.x;
    o_dest.y = i_a.y - i_b.y;
    o_dest.z = i_a.z - i_b.z;
}

void 
ph_vec3_add(ph_vec3 &o_dest, ph_vec3 const& i_a, ph_vec3 const& i_b)
{
    o_dest.x = i_a.x + i_b.x;
    o_dest.y = i_a.y + i_b.y;
    o_dest.z = i_a.z + i_b.z;
}

void 
ph_vec3_mul(ph_vec3 &o_dest, ph_vec3 const& i_a, float const i_scalar)
{
    o_dest.x = i_a.x * i_scalar;
    o_dest.y = i_a.y * i_scalar;
    o_dest.z = i_a.z * i_scalar;
}

void 
ph_vec3_add_scaled(ph_vec3 &o_dest, ph_vec3 const& i_a, ph_vec3 const& i_b, float const i_scale)
{
    o_dest.x = i_a.x + i_b.x * i_scale;
    o_dest.y = i_a.y + i_b.y * i_scale;
    o_dest.z = i_a.z + i_b.z * i_scale;
}