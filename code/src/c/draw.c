#include "draw.h"
//len between poles: 80 016 000m

// void snake_to_pixels(const canvas_t *canvas, const color_t *color, vessel_t *vessel){
//   //TODO
// }

// void draw_path_snake(const canvas_t *canvas, const color_t *color, vessel_t *vessel){
//   SDL_FRect *rect = canvas->dst_rect;
//   SDL_Renderer *rend = canvas->renderer;
//
//   // const SDL_FPoint points[path->len];
//
//   double equator = (rect->h / 2.0f) + rect->y;
//   double prime_mer = (rect->w / 2.0f) + rect->x;
//   double long_scale = rect->h/EARTH_POLAR_DIST; 
//   double lat_scale = rect->w/EARTH_CIRC_EQ;
//   
//   SDL_SetRenderDrawColorFloat(
//     rend,
//     color->r,
//     color->g,
//     color->b,
//     color->a
//   );
//
  //make sure correct len TODO
  // path_node old = path->nodes[0];
  // for (int i = 1; i < path->len; i++){
  //   path_node new = path->nodes[i];

    //TODO:
    //need array of positions from origin
    //1. instruction to vessel: a set of moves delta x delta y
    //
    //2. vessel moves according to this
    //    2a. saves local positions as array element (positions from local origin)
    //    2b. saves geodetic coords as array element 
    //3. each frame, convert these positions to pixels and draw them
    //
    //TODO:
    //path is a bad DS.
    //Need:
    //  list of geodetic points
    //  list of meters from origin (non null island origin can be calc'd by adding a certain amount of meters.)
    //  list of move instructions
    //  list of SDL_FPoint (for drawing)

  //   SDL_RenderLine(rend, old.x, old.y, new.x, new.y);
  //   path_node old = new;
  // }

  // float x = (float) vessel->pos_local.x;
  // float y = (float) vessel->pos_local.y;
  // SDL_RenderLine(rend, old.x, old.y, x, y);
// }


//TODO: I suspect this function can be optimized alot
// void centered_to_window(SDL_Window *window, float zoom, float *x, float *y){
//   int w;
//   int h;
//   SDL_GetWindowSize(window, &w, &h);
//   //FIXME
// }

//TODO: function that translates window coords to centered coords

// void draw_entity(SDL_Renderer *renderer, SDL_Window *window, float zoom, float xpos, float ypos){
//   float x = xpos;
//   float y = ypos;
//
//     filledCircleRGBA(
//       renderer, 
//       x, y, 
//       POINT_SIZE, 
//       40.0f, 200.0f, 40.0f, 255.0f
//     );
// }

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

//TODO
//use canvas as arg
// void draw_vessel_snake(vessel_t *vessel){
//   SDL_FRect *rect = (vessel->canvas).dst_rect;
//   SDL_Renderer *rend = (vessel->canvas).renderer;
//   
//   //how many pixels is a degree
//   float long_scale = rect->h/180.0f;
//   float lat_scale = rect->w/360.0f;
//
//   //equator in the middle between N and S
//   float equator = (rect->h / 2.0f) + rect->y;
//
//   //prime meridian in the middle between E and W
//   float prime_mer = (rect->w / 2.0f) + rect->x;
//
//   //aquire degree position of local pos
//   double pos_lat = (vessel->pos_local.x / 110600) + vessel->start_geod.deg_lat;
//   double pos_long = (vessel->pos_local.y / 111300) + vessel->start_geod.deg_long;
//
//   double x = prime_mer + (pos_lat * lat_scale);
//   double y = equator - (pos_long * long_scale);
//
//   //centered_to_window(window, zoom, &x, &y);
//   filledCircleRGBA(
//     rend, 
//     x, y, 
//     POINT_SIZE, 
//     vessel->color.r, 
//     vessel->color.g, 
//     vessel->color.b, 
//     vessel->color.a
//   );
// }

void draw_vessel_snake(canvas_t *canvas, vessel_t *vessel){
  SDL_FPoint p = local_to_pixels_snake(canvas->dst_rect, vessel->pos_snake);

  filledCircleRGBA(
    canvas->renderer, 
    p.x, p.y, 
    POINT_SIZE, 
    0.0f,
    255.0f,
    0.0f,
    255.0f
  );
}

void draw_vessel_utm(canvas_t *canvas, vessel_t *vessel){
  SDL_FPoint p = geod_to_pixels(canvas->dst_rect, vessel->pos_geod);
  filledCircleRGBA(
    canvas->renderer, 
    p.x, p.y, 
    POINT_SIZE, 
    255.0f,
    0.0f,
    0.0f,
    255.0f
  );
}
// void geod_to_pixels(SDL_FRect *rect, point_geod p, double *x, double *y){
//   double long_scale = rect->h/180.0;
//   double lat_scale  = rect->w/360.0;
//   double equator    = (rect->h / 2.0) + rect->y;
//   double prime_mer  = (rect->w / 2.0) + rect->x;
//   double pos_lat    = p.deg_lat * lat_scale;
//   double pos_long   = p.deg_long * long_scale;
//
//   *y = equator - pos_lat;
//   *x = prime_mer + pos_long;
// }

SDL_FPoint geod_to_pixels(SDL_FRect *rect, point_geod p){
  double long_scale = rect->h/180.0;
  double lat_scale  = rect->w/360.0;
  double equator    = (rect->h / 2.0) + rect->y;
  double prime_mer  = (rect->w / 2.0) + rect->x;
  double pos_lat    = p.deg_lat * lat_scale;
  double pos_long   = p.deg_long * long_scale;

  SDL_FPoint pixp;
  pixp.y = equator - pos_lat;
  pixp.x = prime_mer + pos_long;

  return pixp;
}

SDL_FPoint local_to_pixels_snake(SDL_FRect *rect, point_local p){
  double long_scale = rect->h/EARTH_POLAR_DIST;
  double lat_scale  = rect->w/EARTH_CIRC_EQ;
  double equator    = (rect->h / 2.0) + rect->y;
  double prime_mer  = (rect->w / 2.0) + rect->x;
  double pos_lat    = p.y * lat_scale;
  double pos_long   = p.x * long_scale;

  SDL_FPoint pixp;
  pixp.y = equator - pos_lat;
  pixp.x = prime_mer + pos_long;

  return pixp;
}



//make move orders into positions, and draw inbetween them
void track_vessel_snake(canvas_t *canvas, vessel_t *vessel, move_order_t *order, SDL_FPoint *pixel_path){
  // pixel_path = realloc(pixel_path, order->len * sizeof(SDL_FPoint));

  // pixel_path[0] = local_to_pixels_snake(canvas->dst_rect, vessel->pos_snake);
  // SDL_FPoint pos;
  //TODO:test for length
  for (int i = 0; i < order->len; i++){
    // int idx = vessel->steps_snake;
    move_vessel_snake(vessel, (order->deltas)[i]);

    pixel_path[i] = local_to_pixels_snake(
      canvas->dst_rect,
      vessel->pos_snake);

  }
  //NOTE: possible to draw lines one by one, since the drawing is single threaded
  //in case of multithread: make sure map renders first
}

void track_vessel_utm(canvas_t *canvas, vessel_t *vessel, move_order_t *order, SDL_FPoint *pixel_path){
  // SDL_FPoint pixel_path[order->len];
  // pixel_path = realloc(pixel_path, order->len * sizeof(SDL_FPoint));
  // pixel_path[0] = geod_to_pixels(canvas->dst_rect, vessel->pos_geod);
  for(int i = 0; i < order->len; i++){
    //TODO: zone transfer
    move_vessel_utm(vessel, (order->deltas)[i]);

    pixel_path[i] = geod_to_pixels(
      canvas->dst_rect,
      vessel->pos_geod
    );

  }

  SDL_SetRenderDrawColorFloat(canvas->renderer, 1.0f, 0.0f, 0.0f, 1.0f);
  SDL_RenderLines(canvas->renderer, pixel_path, order->len);
}

//NOTE:could do this inside the track function
void draw_vessel_path(canvas_t *canvas, color_t color, SDL_FPoint *pixel_path, int len){
  //TODO: color thing

  SDL_RenderLines(canvas->renderer, pixel_path, len);
}



//
// //TODO:
// void deg_to_pixel(SDL_FRect *rect, double x, double y){
//   float equator = rect->h/2.0f + rect->y;
//   float prime_mer = rect->w/2.0f + rect->x;
// }
// void m_to_pixel(SDL_FRect *rect, double x, double y){
//   float equator = rect->h/2.0f + rect->y;
//   float prime_mer = rect->w/2.0f + rect->x;
//
//   // double lat_scale = rect->w/EARTH_CIRC_EQ;
//   // double long_scale = rect->h/EARTH_POLAR_DIST;
// }


// will likely print wierd
void draw_example_point_tm(SDL_Renderer *rend, SDL_FRect *rect, point_geod p){
  double x;
  double y;
  // geod_to_pixels(rect, p, &x, &y);
  filledCircleRGBA(
    rend, 
    x, y, 
    POINT_SIZE, 
    60, 60, 200, 255
  );
}
