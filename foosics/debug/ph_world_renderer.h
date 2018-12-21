#pragma once

class IDebugRenderer;

struct ph_world;

void ph_world_debug_render(IDebugRenderer *renderer, ph_world *world);