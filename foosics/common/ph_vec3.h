#pragma once

struct ph_vec3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };
};

// Constants
extern ph_vec3 ph_vec3_zero;
extern ph_vec3 ph_vec3_up;

// Functions
void    ph_vec3_normalise(ph_vec3 &o_dest, ph_vec3 const& i_a);

void    ph_vec3_inverse(ph_vec3 &o_dest, ph_vec3 const& i_a);
float   ph_vec3_magnitude(ph_vec3 const& i_a);

float   ph_vec3_dot(ph_vec3 const& i_a, ph_vec3 const& i_b);

void    ph_vec3_sub(ph_vec3 &o_dest, ph_vec3 const& i_a, ph_vec3 const& i_b);
void    ph_vec3_add(ph_vec3 &o_dest, ph_vec3 const& i_a, ph_vec3 const& i_b);
void    ph_vec3_mul(ph_vec3 &o_dest, ph_vec3 const& i_a, float const i_scalar);

void    ph_vec3_add_scaled(ph_vec3 &o_dest, ph_vec3 const& i_a, ph_vec3 const& i_b, float const i_scale);