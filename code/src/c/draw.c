#include "draw.h"
//len between poles: 80 016 000m

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

SDL_FPoint geod_to_pixels(SDL_FRect *rect, point_geod p){
  double long_scale = rect->w/360.0;
  double lat_scale  = rect->h/180.0;
  double equator    = (rect->h / 2.0) + rect->y;
  double prime_mer  = (rect->w / 2.0) + rect->x;
  double pos_lat    = p.deg_lat * lat_scale;
  double pos_long   = p.deg_long * long_scale;

  SDL_FPoint pixp;
  pixp.y = (equator - pos_lat);
  // pixp.x = prime_mer + pos_long;
  pixp.x = fmod((prime_mer + pos_long), rect->w);

  return pixp;
}

SDL_FPoint local_to_pixels_snake(SDL_FRect *rect, point_local p){
  double long_scale = rect->w/EARTH_CIRC_EQ;
  double lat_scale  = rect->h/EARTH_POLAR_DIST;
  double equator    = (rect->h / 2.0) + rect->y;
  double prime_mer  = (rect->w / 2.0) + rect->x;
  double pos_lat    = p.y * lat_scale;
  double pos_long   = p.x * long_scale;

  SDL_FPoint pixp;
  pixp.y = equator - pos_lat;
  // pixp.x = prime_mer + pos_long;
  pixp.x = fmod((prime_mer + pos_long), rect->w);

  return pixp;
}

//run every frame loop
void render_geod_path(canvas_t *canvas, point_geod *path, int len){
  if (len > 1){
    SDL_FPoint curr = geod_to_pixels(canvas->dst_rect, path[0]);
    SDL_FPoint prev;

    for(int i = 1; i < len; i++){
      prev = curr;
      curr = geod_to_pixels(canvas->dst_rect,path[i]);

      SDL_RenderLine(canvas->renderer, prev.x, prev.y, curr.x, curr.y);
    }
  }
}

void render_snake_path(canvas_t *canvas, point_local *path, int len){
  if (len > 1){
    SDL_FPoint curr = local_to_pixels_snake(canvas->dst_rect, path[0]);
    SDL_FPoint prev;

    for(int i = 1; i < len; i++){
      prev = curr;
      curr = local_to_pixels_snake(canvas->dst_rect,path[i]);

      SDL_RenderLine(canvas->renderer, prev.x, prev.y, curr.x, curr.y);
    }
  }
}
