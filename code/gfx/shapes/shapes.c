#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "shapes.h"
#include "SDL3_gfx/SDL3_gfxPrimitives.h"

double simpl_lat_to_m(double deg){
  return deg * 110600;
}
double simpl_lon_to_m(double deg){
  return deg * 111300;
}

//TODO: I suspect this function can be optimized alot
void centered_to_window(SDL_Window *window, float zoom, float *x, float *y){
  int w;
  int h;
  SDL_GetWindowSize(window, &w, &h);
  //FIXME
}

//TODO: function that translates window coords to centered coords

void draw_entity(SDL_Renderer *renderer, SDL_Window *window, float zoom, float xpos, float ypos, entity_type type){
  float x = xpos;
  float y = ypos;

  //centered_to_window(window, zoom, &x, &y);
  if(type == vessel_ent){
    filledCircleRGBA(
      renderer, 
      x, y, 
      POINT_SIZE, 
      40.0f, 200.0f, 40.0f, 255.0f
    );
  }
  else if(type == observer_ent){
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

void draw_grid(SDL_Renderer *renderer, SDL_FRect *dst_rect){
  SDL_SetRenderDrawColorFloat(renderer, GRID_RGBA[0], GRID_RGBA[1], GRID_RGBA[2], GRID_RGBA[3]);
  float lat_scale = dst_rect->h/LAT_ZONES;
  float lon_scale = dst_rect->w/LON_ZONES;

  float mid_y = (dst_rect->h / 2.0f) + dst_rect->y;
  float mid_x = (dst_rect->w / 2.0f) + dst_rect->x;

  for (float i = lon_scale; i < dst_rect->w; i += lon_scale){
    SDL_RenderLine(renderer, 0, mid_y+i, dst_rect->w, mid_y+i);
    SDL_RenderLine(renderer, 0, mid_y-i, dst_rect->w, mid_y-i);
  }
  for (float i = lat_scale; i < dst_rect->h; i += lat_scale){
    SDL_RenderLine(renderer, mid_x+i, 0, mid_x+i, dst_rect->h);
    SDL_RenderLine(renderer, mid_x-i, 0, mid_x-i, dst_rect->h);
  }

  SDL_SetRenderDrawColorFloat(renderer, 255, GRID_RGBA[1], GRID_RGBA[2], GRID_RGBA[3]);
  SDL_RenderLine(renderer, 0, mid_y, dst_rect->w, mid_y);
  SDL_RenderLine(renderer, mid_x, 0, mid_x, dst_rect->h);
}



void draw_grid_utm(SDL_Renderer *renderer, SDL_FRect *dst_rect){
  SDL_SetRenderDrawColorFloat(renderer, GRID_RGBA[0], GRID_RGBA[1], GRID_RGBA[2], GRID_RGBA[3]);
  float lon_scale = dst_rect->h/180.0f;
  float lat_scale = dst_rect->w/360.0f;

  float equator = (dst_rect->h / 2.0f) + dst_rect->y;
  float prime_mer = (dst_rect->w / 2.0f) + dst_rect->x;

  for (float i = 8 * lon_scale; i <= dst_rect->h; i += 8 * lon_scale){
    SDL_RenderLine(renderer, 0, equator+i, dst_rect->w, equator+i);
    SDL_RenderLine(renderer, 0, equator-i, dst_rect->w, equator-i);
  }


  for (float i = 6 * lat_scale; i <= dst_rect->w; i += 6 * lat_scale){
    SDL_RenderLine(renderer, prime_mer+i, 0, prime_mer+i, dst_rect->h);
    SDL_RenderLine(renderer, prime_mer-i, 0, prime_mer-i, dst_rect->h);
  }

  SDL_SetRenderDrawColorFloat(renderer, 255, GRID_RGBA[1], GRID_RGBA[2], GRID_RGBA[3]);
  //equator and pm
  SDL_RenderLine(renderer, 0, equator, dst_rect->w, equator);
  SDL_RenderLine(renderer, prime_mer, 0, prime_mer, dst_rect->h);

  //ns edges
  SDL_RenderLine(renderer, 0, equator+(90 * lon_scale), dst_rect->w, equator+(90 * lon_scale));
  SDL_RenderLine(renderer, 0, equator+(80 * lon_scale), dst_rect->w, equator+(80 * lon_scale));
  SDL_RenderLine(renderer, 0, equator-(84 * lon_scale), dst_rect->w, equator-(84 * lon_scale));
  SDL_RenderLine(renderer, 0, equator-(90 * lon_scale), dst_rect->w, equator-(90 * lon_scale));

  //ew edges
}

void draw_vessel(SDL_Renderer *rend, SDL_FRect *rect, vessel_t *vessel){
  float long_scale = rect->h/180.0f;
  float lat_scale = rect->w/360.0f;
  float equator = (rect->h / 2.0f) + rect->y;
  float prime_mer = (rect->w / 2.0f) + rect->x;

  //TODO: depends on projection
  double pos_lat = (vessel->current_x / 110600) + vessel->start_lat;
  double pos_long = (vessel->current_y / 111300) + vessel->start_long;

  double x = prime_mer + (pos_lat * lat_scale);
  double y = equator - (pos_long * long_scale);

  //centered_to_window(window, zoom, &x, &y);
  filledCircleRGBA(
    rend, 
    x, y, 
    POINT_SIZE, 
    200, 60, 60, 255
    // vessel->rgba[0], 
    // vessel->rgba[1], 
    // vessel->rgba[2], 
    // vessel->rgba[3]
  );
}

//FIXME: somewhat ugly code
void append_to_path(path_t *path, double x, double y){
  path_node node;
  node.x = x;
  node.y = y;
  path->nodes[path->len] = node;
  path->len++;
  // printf("step %d: (%lf, %lf) visited\n",path->len, x,y);
}

vessel_t *launch_vessel(double start_lon, double start_lat, float rgba[4]){
  vessel_t *vessel = calloc(1, sizeof(vessel_t));

  //what lat/long the vessel is launched at
  vessel -> start_long = start_lon;
  vessel -> start_lat = start_lat;

  //the vessel will always start at 0,0 from its own
  //point of view
  vessel -> current_x = 0;
  vessel -> current_y = 0;

  //assigning colors to vessel
  for (int i = 0;i < 4;i++) {
    vessel -> rgba[i] = rgba[i];
  }

  //malloc its path
  //NOTE: not dynamic
  vessel->path = calloc(1, sizeof(path_t));

  //first node visited will always be 0,0
  append_to_path(vessel->path, 0, 0);
  return vessel;
}


void destroy_vessel(vessel_t *vessel){
  free(vessel->path);
  //watch out for pointers to this
  free(vessel);
}

//NOTE: naive
void move_vessel_deg(vessel_t *vessel, double move_x, double move_y){
  vessel->current_x = vessel->current_x + (110600 * move_x);
  vessel->current_y = vessel->current_y + (111300 * move_y);

  append_to_path(vessel->path, vessel->current_x, vessel->current_y);
}

void move_vessel_m(vessel_t *vessel, double move_x, double move_y){
  vessel->current_x = vessel->current_x + move_x;
  vessel->current_y = vessel->current_y + move_y;

  append_to_path(vessel->path, vessel->current_x, vessel->current_y);
}

//FIXME: this is really inefficient
void draw_path(SDL_Renderer *rend, SDL_FRect *rect, vessel_t *vessel){
  double equator = (rect->h / 2.0f) + rect->y;
  double prime_mer = (rect->w / 2.0f) + rect->x;
  double long_scale = rect->h/20004000; 
  double lat_scale = rect->w/40075016;
  //len between poles: 80 016 000m
  //
  //y = equator - node.y * long_scale
  //x = prime_mer + node.x * lat_scale

  int len = vessel->path->len;
  SDL_FPoint points[len];

  for (int i = 0;i < len;i++) {
    //y = equator - node.y * long_scale
    //x = prime_mer + node.x * lat_scale
    //NOTE: naive solution
    path_node node = vessel->path->nodes[i];
    SDL_FPoint p;// = {(float) node.x, (float) node.y};
    p.x = (float) (prime_mer + node.x * lat_scale);
    p.y = (float) (equator - node.y * long_scale);
    points[i] = p;
  }

  //pick color

  SDL_SetRenderDrawColorFloat(rend,vessel->rgba[0],vessel->rgba[1],vessel->rgba[2],vessel->rgba[3]);
  SDL_RenderLines(rend, points, len);
}
