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

extern ph_vec3 ph_vec3_zero;