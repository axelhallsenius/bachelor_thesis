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

move_order_t *create_random_move_order(int len, int scale){
  move_order_t *order = malloc(sizeof(move_order_t));
  order->deltas = malloc(len * sizeof(point_local));
  order->len = len;


  //rand
  double dx;
  double dy;

  for (int i = 0; i < len; i++){
    dx = ((SDL_rand(200) - 100));
    dy = ((SDL_rand(200) - 100));
    dx = dx * scale;
    dy = dy * scale;

    point_local p = {dx, dy};

    order->deltas[i] = p;
    // printf("delta: %lf,%lf\n", order->deltas[0].x,order->deltas[4].x);
  }
  return order;
}

void destroy_move_order(move_order_t *order){
  free(order->deltas);
  free(order);
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

  // point_tm_grid local_point = geod_to_utm_grid(startp);
  //TODO: make snake spawn anywhere
  point_local p = {0.0, 0.0};

  

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
  
  vessel->pos_geod = newpos;
  // TODO: define headless mode for measuring without seeing
  // #ifdef HEADLESS=OFF
  // cant do that here, we dont have the rect
  // vessel->geod_path_pixels = geod_to_pixels(newpos);
  // #endif
}

//translate orders to degree movement
//call when updating order
point_geod *make_path_utm(point_geod start, move_order_t *order){
  point_geod *path = malloc(order->len * sizeof(point_geod));

  point_geod tmp_geod;
  utm_zone zone;
  point_tm_grid prev;
  point_tm_grid curr = geod_to_utm_grid(start);
  path[0] = start;
  for (int i = 1;i < order->len;i++) {
    //TODO: check if this works
    zone = utm_zone_from_geod(path[i-1]);
    prev = curr;
    curr.x = prev.x + ((order->deltas)[i].x);
    curr.y = prev.y + ((order->deltas)[i].y);
    // printf("geo: prev.x (%lf) + orderdeltas(%lf) = %lf\n", prev.x, (order->deltas)[i].x, curr.x);
    // printf("geo: prev.y (%lf) + orderdeltas(%lf) = %lf\n", prev.y, (order->deltas)[i].y, curr.y);

    //checking hemisphere
    // if (zone.hemisphere == HEM_N){
    //   if (curr.y < 0){
    //     zone.hemisphere = HEM_S;
    //   }
    //   else if (curr.y > UTM_FN_S){
    //   //TODO: Polar transfer
    //   }
    // }
    // }
    // if (zone.hemisphere == HEM_S){
    //   if (curr.y > UTM_FN_S){
    //     zone.hemisphere = HEM_N;
    //   }
    //   else if (curr.y < 0){
    //   //TODO: Polar transfer
    //   }
    // }


    tmp_geod = utm_grid_to_geod(curr, zone);
    zone = utm_zone_from_geod(tmp_geod);
    curr = geod_to_utm_grid(tmp_geod);

    path[i] = utm_grid_to_geod(curr, zone);
    // printf("geod converted lat%lf,long%lf\n", path[i].deg_lat,path[i].deg_long);
  }

  return path;
}

void update_vessel_pos(vessel_t *vessel, point_geod g, point_local l){
  vessel->pos_geod = g;
  vessel->pos_snake = l;
}

//call when updating order
point_local *make_path_snake(point_local start, move_order_t *order){
  point_local *path = malloc(order->len * sizeof(point_local));

  // point_tm_grid tmp = geod_to_utm_grid(start);
  // point_local prev;
  // point_local curr = {tmp.x, tmp.y};
  point_local prev;
  point_local curr = start;
  // path[0] = start;
  path[0] = curr;
  for (int i = 1;i < order->len;i++) {
    prev = curr;
    // printf("delta: %lf,%lf\n",(order->deltas)[i].x, (order->deltas)[i].y);
    // printf("old pos: %lf",prev.x);
    // printf(",%lf\n",prev.y);
    
    curr.x = prev.x + ((order->deltas)[i].x);
    // printf("new pos: %lf",curr.x);
    curr.y = prev.y + ((order->deltas)[i].y);
    // printf(",%lf\n",curr.y);

    // printf("snake: prev.x (%lf) + orderdeltas(%lf) = %lf\n", prev.x, (order->deltas)[i].x, curr.x);
    // printf("snake: prev.y (%lf) + orderdeltas(%lf) = %lf\n", prev.y, (order->deltas)[i].y, curr.y);
    path[i] = curr;
  }

  return path;
}
