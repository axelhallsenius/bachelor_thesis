#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../gui/imgui.h"
#include "gui/imgui_impl_sdl3.h"
#include "gui/imgui_impl_sdlrenderer3.h"
#include "shapes.h"

#define POINT_SIZE 4;

void draw_point(point *p, struct ImVec4 color){
  //
}

void testing_func(SDL_Renderer *renderer){
  Uint32 px = (Uint32) SDL_rand(400);
  Uint32 py = (Uint32) SDL_rand(400);
  if (!SDL_RenderPoint(renderer, px, py)){
    printf("Failed SDL_RenderPoint()\n");
  }
}

