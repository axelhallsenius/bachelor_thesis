#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "shapes.h"
#include "SDL3_gfx/SDL3_gfxPrimitives.h"

#define POINT_SIZE 6;

void draw_entity(SDL_Renderer *renderer, int xpos, int ypos, entity_type type){
  if(type == vessel){
    filledCircleRGBA(
      renderer, 
      xpos, ypos, 
      6, 
      40.0f, 200.0f, 40.0f, 255.0f
    );
  }
  else if(type == observer){
    filledCircleRGBA(
      renderer, 
      xpos, ypos, 
      6, 
      200.0f, 40.0f, 40.0f, 255.0f
    );
  }
  else {
    printf("\nError: entity_type not recognized.");
    exit(1);
  }
}

void draw_grid(SDL_Renderer *renderer, SDL_Window *window, Uint32 scale){
  //corresponds to color #c0c0c0
  SDL_SetRenderDrawColorFloat(renderer, 0.753f, 0.753f, 0.753f, 1.0f);
  int w;
  int h;
  // SDL_GetWindowSizeInPixels(window, &w, &h);
  //TODO: right now just renders a cross

	SDL_RenderLine(renderer, 0, 100, 200, 100);;
	SDL_RenderLine(renderer, 100, 0, 100, 200);;
  
	// SDL_RenderLine(renderer, 0, (*h)/2, *w, (*h)/2);;
	// SDL_RenderLine(renderer, (*w)/2, 0, (*w)/2, *h);;

}

