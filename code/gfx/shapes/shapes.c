#include <stdio.h>
// #include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "shapes.h"

#define POINT_SIZE 4;

void testing_func(SDL_Renderer *renderer){
  Uint32 px = (Uint32) SDL_rand(400);
  Uint32 py = (Uint32) SDL_rand(400);
  if (!SDL_RenderPoint(renderer, px, py)){
    printf("Failed SDL_RenderPoint()\n");
  }
}

