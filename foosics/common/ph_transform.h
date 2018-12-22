#pragma once

#include "ph_vec3.h"

struct ph_transform {
	ph_vec3 position;
	ph_vec3 rotation;
};

extern ph_transform ph_transform_zero;
