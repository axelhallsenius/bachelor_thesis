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

void draw_vessel_snake(SDL_Renderer *rend, SDL_FRect *rect, vessel_t *vessel){
  float long_scale = rect->h/180.0f;
  float lat_scale = rect->w/360.0f;
  float equator = (rect->h / 2.0f) + rect->y;
  float prime_mer = (rect->w / 2.0f) + rect->x;

  //TODO: depends on projection
  double pos_lat = (vessel->local_x / 110600) + vessel->start_lat;
  double pos_long = (vessel->local_y / 111300) + vessel->start_long;

  double x = prime_mer + (pos_lat * lat_scale);
  double y = equator - (pos_long * long_scale);

  //centered_to_window(window, zoom, &x, &y);
  filledCircleRGBA(
    rend, 
    x, y, 
    POINT_SIZE, 
    200, 60, 60, 255
  );
}

//FIXME: somewhat ugly code
void append_to_path_local(path_t *path, double x, double y){
  path_node node;
  node.x = x;
  node.y = y;
  path->nodes[path->len] = node;
  path->len++;
  // printf("step %d: (%lf, %lf) visited\n",path->len, x,y);
}

vessel_t *launch_vessel(double start_long, double start_lat, float rgba[4]){
  vessel_t *vessel = calloc(1, sizeof(vessel_t));

  //what lat/long the vessel is launched at
  vessel -> start_long = start_long;
  vessel -> start_lat = start_lat;

  //the vessel will always start at 0,0 from its own
  //point of view
  vessel -> local_x = 0;
  vessel -> local_y = 0;

  vessel -> sphere_lat = start_lat;
  vessel -> sphere_long = start_long;

  //assigning colors to vessel
  for (int i = 0;i < 4;i++) {
    vessel -> rgba[i] = rgba[i];
  }

  //malloc its path
  //NOTE: not dynamic
  vessel->path = calloc(1, sizeof(path_t));

  //first node visited will always be 0,0
  append_to_path_local(vessel->path, 0, 0);
  return vessel;
}


void destroy_vessel(vessel_t *vessel){
  free(vessel->path);
  //watch out for pointers to this
  free(vessel);
}

//NOTE: naive
void move_vessel_deg(vessel_t *vessel, double move_x, double move_y){
  vessel->local_x = vessel->local_x + (110600 * move_x);
  vessel->local_y = vessel->local_y + (111300 * move_y);

  append_to_path_local(vessel->path, vessel->local_x, vessel->local_y);
}
//move_vessel_cart_deg

void move_vessel_m(vessel_t *vessel, double move_x, double move_y){
  vessel->local_x = vessel->local_x + move_x;
  vessel->local_y = vessel->local_y + move_y;

  append_to_path_local(vessel->path, vessel->local_x, vessel->local_y);
}

void move_sphere_m(vessel_t *vessel, double move_x, double move_y){
  //move x many meters in this direction, y in that
  //what is now the degree value on the sphere
  //save the value in the vessel->sphere_lat/long
}



//move_sphere_deg
//move_sphere_m

//move_geoid_deg
//move_geoid_m

//timea funktioner separat

//HERE: great circle navigation
//HERE: basic UV-mapping to a sphere
//HERE: rhumb line impl? Looks straight on a mercator proj
//but isn't straight over a geoid

//NOTE:
//Store coordinates in a common data structure
//ex: vessel:
//x, y from launch
//lat/long on a sphere
//lat/long on a geoid
//functions translate these coords onto the map
//Points will start to differ when they are moved 
//difference in projection and such will be in move functions

//TODO: Implement travel across poles and date line for snake
//Less silly:
//Make movement across a pole will make you end up on an equal distance to the meridian, but upside down

//IDEA: save orientation of the vessels matrix?
//
//movement algo for this: 
//
// 1. place reference (basis) matrix in current position of vessel
// 2. translate next waypoint's position 
//    (an x, y value from the current basis) ---> projection coords 
//  2a. x, y from null island for snake proj
//  2b. lat/long for sphere
//  2c. lat/long for WGS geoid
// 3. store the vessel's new ref?
// 4. draw it
//    translate from all 3 coord systems separately into pixels
//    give separate colors for comparison?
//
// this can be done similarly for all projections
//
// This is good if vessel reports position like:
// I went 2m right, and 4 m forward from where I last was.
//
// This assumes vessels bearing at the end of a waypoint.
// Can the bearing be assumed to be linear to vector? No.
// Can the vessel report its rotation?
// Saving rotation and transform as a quaternion (2d, so maybe that's just a x,y,w) is a good idea in this case 
// More classic gamedev programming there
//
// Write about this, but do not implement it


// ALTERNATIVE:
// next waypoints are always given as cardinal directions from the vessel position.
// supplying more than max/min of the map will result in 
// supplied val until vessel reaches pole. The vessel will then "teleport" 
// to a negated map value x 
// and the rest of the y value will be negated

//This function could be saved as draw_path_linear or something
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



void draw_path_sphere(SDL_Renderer *rend, SDL_FRect *rect,  path_t *path){
  
}
