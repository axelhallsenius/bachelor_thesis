#include "../shapes/shapes.h"
#include "../shapes/map.h"
#include <stdio.h>

#ifndef TEST_H
#define TEST_H

#ifdef __cplusplus
extern "C" {
#endif

void test_geod_draw(SDL_Renderer *rend, SDL_FRect *rect);
void test_meter_to_geod(SDL_Renderer *rend, SDL_FRect *rect);

#endif
#ifdef __cplusplus
}
#endif
