#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "shapes.h"
#include "SDL3_gfx/SDL3_gfxPrimitives.h"

//TODO: I suspect this function can be optimized alot
void centered_to_window(SDL_Window *window, float zoom, float *x, float *y){
  int w;
  int h;
  SDL_GetWindowSize(window, &w, &h);
}

void draw_entity(SDL_Renderer *renderer, SDL_Window *window, float zoom, float xpos, float ypos, entity_type type){
  float x = xpos;
  float y = ypos;

  //centered_to_window(window, zoom, &x, &y);
  if(type == vessel){
    filledCircleRGBA(
      renderer, 
      x, y, 
      POINT_SIZE, 
      40.0f, 200.0f, 40.0f, 255.0f
    );
  }
  else if(type == observer){
    filledCircleRGBA(
      renderer, 
      x, y, 
      POINT_SIZE, 
      200.0f, 40.0f, 40.0f, 255.0f
    );
  }
  else {
    printf("\nError: entity_type not recognized.");
    exit(1);
  }
}

void draw_grid(SDL_Renderer *renderer, SDL_Window *window, float zoom){
  //corresponds to color #c0c0c0
  float scale = zoom * 100.0f;
  if (scale < 10.0f){
    printf("Error, grid scale too small.\n");
  }
  int w;
  int h;
  SDL_GetWindowSize(window, &w, &h);
  int mid_x = w/2;  
  int mid_y = h/2;  

  SDL_SetRenderDrawColorFloat(renderer, GRID_RGBA[0], GRID_RGBA[1], GRID_RGBA[2], GRID_RGBA[3]);
	// SDL_RenderLine(renderer, 0, mid_y, w, mid_y);
	// SDL_RenderLine(renderer, mid_x, 0, mid_x, h);

  for (float i = 0; i < w; i += scale){
    SDL_RenderLine(renderer, 0, mid_y+i, w, mid_y+i);
    SDL_RenderLine(renderer, 0, mid_y-i, w, mid_y-i);
  }
  for (float i = 0; i < h; i += scale){
    SDL_RenderLine(renderer, mid_x+i, 0, mid_x+i, h);
    SDL_RenderLine(renderer, mid_x-i, 0, mid_x-i, h);
  }

}

