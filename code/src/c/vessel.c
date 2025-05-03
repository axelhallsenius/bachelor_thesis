// #include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "vessel.h"


double simpl_lat_to_m(double deg){
  return deg * 110600;
}
double simpl_lon_to_m(double deg){
  return deg * 111300;
}




//FIXME: somewhat ugly code
void append_to_path_local(path_t *path, double x, double y){
  path_node node;
  node.x = x;
  node.y = y;
  path->nodes[path->len] = node;
  path->len++;
}

vessel_t *launch_vessel(canvas_t *canvas, point_geod startp, float rgba[4]){
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

  vessel->canvas.renderer = canvas->renderer;
  vessel->canvas.dst_rect = canvas->dst_rect;

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






point_local snake_move_vessel_m(vessel_t *vessel, point_local p){
  point_local old;
  old.x = vessel -> pos_local.x;
  old.y = vessel -> pos_local.y;

  vessel->pos_local.x = vessel->pos_local.x + p.x;
  vessel->pos_local.y = vessel->pos_local.y + p.y;

  return old;
  // snake_draw_path(vessel, old);
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
