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
//
// //FIXME: somewhat ugly code
// void append_to_path_local(path_t *path, local_path){
//   path_node node;
//   node.x = x;
//   node.y = y;
//   path->nodes[path->len] = node;
//   path->len++;
// }

vessel_t *launch_vessel(canvas_t *canvas, point_geod startp, int path_len){
  vessel_t *vessel = calloc(1, sizeof(vessel_t));

  //what lat/long the vessel is launched at
  vessel->start_geod = startp;
  //TODO: dynamically?
  //can realloc if given more at runtime
  vessel->local_path = calloc(path_len, sizeof(point_local));
  vessel->geod_path  = calloc(path_len, sizeof(point_geod));
  vessel->geod_path[0] = startp;

  vessel->pos_geod = startp;

  // vessel -> pos_geod.deg_lat = startp.deg_lat;
  // vessel -> pos_geod.deg_long = startp.deg_long;
  vessel->local_steps = 1;
  vessel->geod_steps = 1;


  // geod_to_utm_grid()

  //assigning colors to vessel
  // vessel -> color = color;
  // vessel -> color.r = rgba[0];
  // vessel -> color.g = rgba[1];
  // vessel -> color.b = rgba[2];
  // vessel -> color.a = rgba[3];

  //malloc its path
  //NOTE: not dynamic
  // vessel->path = calloc(1, sizeof(path_t));

  //first node visited will always be 0,0
  // append_to_path_local(vessel->path, 0, 0);
  return vessel;
}

void destroy_vessel(vessel_t *vessel){
  free(vessel->local_path);
  free(vessel->geod_path);
  free(vessel->local_path_pixels);
  free(vessel->geod_path_pixels);
  //watch out for pointers to this
  free(vessel);
}

//FIXME: will segfault now
void move_vessel_snake(vessel_t *vessel, point_local delta){
  int idx = vessel->local_steps;
  point_local p;
  p.x = vessel->local_path[idx-1].x + delta.x;
  p.y = vessel->local_path[idx-1].y + delta.y;

  vessel->local_path[idx] = p;
  vessel->local_steps++;
  //TODO: populate pixel point array
}

//make move orders into positions, and draw inbetween them
void track_vessel_snake(canvas_t * canvas, vessel_t *vessel, move_order_t *order){
  for (int i = 0; i < order->len; i++){
    move_vessel_snake(vessel, (order->deltas)[i]);
  }
}

void move_vessel_utm(vessel_t *vessel, point_local delta){
  point_tm_grid p = geod_to_utm_grid(vessel->pos_geod);
  p.x = p.x + delta.x;
  p.y = p.y + delta.y;
  
  //TODO: should detect if vessel transfers zone
  //current solution is likely not working
  point_geod newpos = utm_grid_to_geod(p, vessel->zone);
  vessel->geod_path[vessel->geod_steps] = newpos;
  vessel->geod_steps++;
  vessel->zone = utm_zone_from_geod(newpos);
  
  // TODO: define headless mode for measuring without seeing
  // #ifdef HEADLESS=OFF
  // cant do that here, we dont have the rect
  // vessel->geod_path_pixels = geod_to_pixels(newpos);
  // #endif
}

//
// point_local snake_move_vessel_m(vessel_t *vessel, point_local p){
//   point_local old;
//   old.x = vessel -> pos_local.x;
//   old.y = vessel -> pos_local.y;
//
//   vessel->pos_local.x = vessel->pos_local.x + p.x;
//   vessel->pos_local.y = vessel->pos_local.y + p.y;
//
//   return old;
//   // snake_draw_path(vessel, old);
//   // draw from last pos to current pos
//   // NOTE - This doesn't work.
//   // append_to_path_local(vessel->local_path, vessel->pos_local);
//   vessel->local_path[]
// }

// void snake_move_vessel_deg(vessel_t *vessel, point_geod p){
//
//   point_local lp;
//   lp.x = 110600 * p.deg_long;
//   lp.y = 111300 * p.deg_lat;
//
//   snake_move_vessel_m(vessel,lp);
// }

void utm_move_vessel_m(vessel_t *vessel, point_local dst){
  // path->nodes[path->len].x = 0.0; //TODO
  // path->nodes[path->len].y = 0.0; //TODO
  // path->len++;
}
