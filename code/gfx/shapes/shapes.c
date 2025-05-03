// #include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "shapes.h"

#define EARTH_CIRC_EQ 40075016.0
#define EARTH_POLAR_DIST 20004000.0

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

void draw_entity(SDL_Renderer *renderer, SDL_Window *window, float zoom, float xpos, float ypos){
  float x = xpos;
  float y = ypos;

    filledCircleRGBA(
      renderer, 
      x, y, 
      POINT_SIZE, 
      40.0f, 200.0f, 40.0f, 255.0f
    );
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

//FIXME: segs
void draw_vessel_snake(vessel_t *vessel){
  SDL_FRect *rect = vessel->canvas.dst_rect;
  SDL_Renderer *rend = vessel->canvas.renderer;
  //how many pixels is a degree
  float long_scale = rect->h/180.0f;
  float lat_scale = rect->w/360.0f;
  //equator in the middle between N and S
  float equator = (rect->h / 2.0f) + rect->y;
  //prime meridian in the middle between E and W
  float prime_mer = (rect->w / 2.0f) + rect->x;

  //aquire degree position of local pos
  double pos_lat = (vessel->pos_local.x / 110600) + vessel->start_geod.deg_lat;
  double pos_long = (vessel->pos_local.y / 111300) + vessel->start_geod.deg_long;

  double x = prime_mer + (pos_lat * lat_scale);
  double y = equator - (pos_long * long_scale);

  //centered_to_window(window, zoom, &x, &y);
  filledCircleRGBA(
    rend, 
    x, y, 
    POINT_SIZE, 
    vessel->color.r, 
    vessel->color.g, 
    vessel->color.b, 
    vessel->color.a
  );
}

//TODO:
void deg_to_pixel(SDL_FRect *rect, double x, double y){
  float equator = rect->h/2.0f + rect->y;
  float prime_mer = rect->w/2.0f + rect->x;
}
void m_to_pixel(SDL_FRect *rect, double x, double y){
  float equator = rect->h/2.0f + rect->y;
  float prime_mer = rect->w/2.0f + rect->x;

  double lat_scale = rect->w/EARTH_CIRC_EQ;
  double long_scale = rect->h/EARTH_POLAR_DIST;
}


// will likely print wierd
void draw_example_point_tm(SDL_Renderer *rend, SDL_FRect *rect, point_geod p){
  double x;
  double y;
  geod_to_pixels(rect, p, &x, &y);
  filledCircleRGBA(
    rend, 
    x, y, 
    POINT_SIZE, 
    60, 60, 200, 255
  );
}

//FIXME: somewhat ugly code
void append_to_path_local(path_t *path, double x, double y){
  path_node node;
  node.x = x;
  node.y = y;
  path->nodes[path->len] = node;
  path->len++;
}

vessel_t *launch_vessel(point_geod startp, float rgba[4]){
  vessel_t *vessel = calloc(1, sizeof(vessel_t));

  //what lat/long the vessel is launched at
  vessel -> start_geod.deg_long = startp.deg_long;
  vessel -> start_geod.deg_lat = startp.deg_lat;

  //the vessel will always start at 0,0 from its own
  //point of view
  vessel -> pos_local.x = 0;
  vessel -> pos_local.y = 0;

  vessel -> pos_geod.deg_lat = startp.deg_lat;
  vessel -> pos_geod.deg_long = startp.deg_long;

  // geod_to_utm_grid()

  //assigning colors to vessel
  vessel -> color.r = rgba[0];
  vessel -> color.g = rgba[1];
  vessel -> color.b = rgba[2];
  vessel -> color.a = rgba[3];

  //malloc its path
  //NOTE: not dynamic
  // vessel->path = calloc(1, sizeof(path_t));

  //first node visited will always be 0,0
  // append_to_path_local(vessel->path, 0, 0);
  return vessel;
}


void destroy_vessel(vessel_t *vessel){
  // free(vessel->path);
  //watch out for pointers to this
  free(vessel);
}



//This function could be saved as draw_path_linear or something
//FIXME: this is really inefficient
void snake_draw_path(vessel_t *vessel, point_local old){
  SDL_FRect *rect = vessel->canvas.dst_rect;
  SDL_Renderer *rend = vessel->canvas.renderer;

  double equator = (rect->h / 2.0f) + rect->y;
  double prime_mer = (rect->w / 2.0f) + rect->x;
  double long_scale = rect->h/EARTH_POLAR_DIST; 
  double lat_scale = rect->w/EARTH_CIRC_EQ;
  //len between poles: 80 016 000m
  
  //pick color
  SDL_SetRenderDrawColorFloat(
    rend,
    vessel->color.r,
    vessel->color.g,
    vessel->color.b,
    vessel->color.a
  );

  float x = (float) vessel->pos_local.x;
  float y = (float) vessel->pos_local.y;
  SDL_RenderLine(rend, old.x, old.y, x, y);
}


void snake_move_vessel_m(vessel_t *vessel, point_local p){
  point_local old;
  old.x = vessel -> pos_local.x;
  old.y = vessel -> pos_local.y;

  vessel->pos_local.x = vessel->pos_local.x + p.x;
  vessel->pos_local.y = vessel->pos_local.y + p.y;

  snake_draw_path(vessel, old);
  // draw from last pos to current pos
  // append_to_path_local(vessel->path, vessel->local_x, vessel->local_y);
}

void snake_move_vessel_deg(vessel_t *vessel, point_geod p){

  point_local lp;
  lp.x = 110600 * p.deg_long;
  lp.y = 111300 * p.deg_lat;

  snake_move_vessel_m(vessel,lp);
}

void utm_move_vessel_m(vessel_t *vessel, point_local dst){
  // path->nodes[path->len].x = 0.0; //TODO
  // path->nodes[path->len].y = 0.0; //TODO
  // path->len++;
}


void draw_path_sphere(SDL_Renderer *rend, SDL_FRect *rect,  path_t *path){
  
}
