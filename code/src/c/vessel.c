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

move_order_t create_random_move_order(int len, int scale){
  move_order_t order;
  order.deltas = malloc(len * sizeof(point_local));
  order.len = 0;

  //rand
  point_local p;
  double dx;
  double dy;

  for (int i = 0; i < len; i++){
    dx = ((double) (SDL_rand(200.0) - 100.0) * scale);
    dy = ((double) (SDL_rand(200.0) - 100.0) * scale);
    p.x = dx;
    p.y = dy;

    order.deltas[i] = p;
  }

  return order;
}

void destroy_move_order(move_order_t order){
  free(order.deltas);
  // free(order);
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

vessel_t *launch_vessel(point_geod startp, int path_len){
  vessel_t *vessel = malloc(sizeof(vessel_t));

  //what lat/long the vessel is launched at
  vessel->start_geod = startp;
  vessel->pos_geod = startp;
  vessel->zone = utm_zone_from_geod(startp);

  //NOTE: will place the vessel at the correct starting point
  point_tm_grid local_point = geod_to_utm_grid(startp);
  point_local p = {local_point.x, local_point.y};

  vessel->start_snake = p;
  vessel->pos_snake = p;

  vessel->steps_snake = 1;
  vessel->steps_utm = 1;

  return vessel;
}

void destroy_vessel(vessel_t *vessel){
  // free(vessel->local_path);
  // free(vessel->geod_path);
  // free(vessel->local_path_pixels);
  // free(vessel->geod_path_pixels);
  free(vessel);
}

void move_vessel_snake(vessel_t *vessel, point_local delta){
  int idx = vessel->steps_snake;
  point_local p;
  p.x = vessel->pos_snake.x + delta.x;
  p.y = vessel->pos_snake.y + delta.y;

  vessel->pos_snake = p;
  vessel->steps_snake++;
}



void move_vessel_utm(vessel_t *vessel, point_local delta){
  point_tm_grid p = geod_to_utm_grid(vessel->pos_geod);
  p.x = p.x + delta.x;
  p.y = p.y + delta.y;
  
  //TODO: should detect if vessel transfers zone
  //current solution is likely not working
  point_geod newpos = utm_grid_to_geod(p, vessel->zone);
  vessel->steps_utm++;
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
