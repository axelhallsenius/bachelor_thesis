#include "translation.h"
#include <math.h>

win_coords cent_to_win(SDL_Window *window, float x, float y){
  win_coords coords;
  int win_w;
  int win_h;
  SDL_GetWindowSize(window, &win_w, &win_h);

  coords.x = floor(x + ((float)win_w / 2));
  coords.y = floor(((float)win_h / 2) - y);

  return coords;
}



win_coords deg_to_win_snake(SDL_Window *window, lat_lon_snake *deg){
  float cart_x = fmod(deg->lon, 180.0f);
  float cart_y = fmod(deg->lat, 90.0f);

  return cent_to_win(window, cart_x, cart_y);
}

void move_xy_snake(lat_lon_snake *pos, lat_lon_snake *course){
  pos->lon = fmod(pos->lon + course->lon, 180.0f);
  pos->lat = fmod(pos->lat + course->lat, 90.0f);
}
