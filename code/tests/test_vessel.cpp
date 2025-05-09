#include <limits.h>
#include "gtest/gtest.h"
#include "../src/c/draw.h"

namespace {
TEST(ProjDiffTest, Short){
  int ORDER_LEN = 100;
  int ORDER_SCALE = 10;

  SDL_FRect test_rect;
  test_rect.w = 1000.0f;
  test_rect.h = 1000.0f;
  test_rect.x = 1000.0f;
  test_rect.y = 1000.0f;
  point_geod null_island = {0.0, 0.0};
  move_order_t *move_order = create_random_move_order(ORDER_LEN, ORDER_SCALE);
  vessel_t *vessel = launch_vessel(null_island, move_order->len);
  point_geod *geod_path = make_path_utm(vessel->pos_geod, move_order);
  point_local *local_path = make_path_snake(vessel->pos_snake, move_order);
  point_geod g = geod_path[move_order->len - 1];
  point_local l = local_path[move_order->len - 1];
  update_vessel_pos(vessel, g, l);

  float max_dx = 0.0f, max_dy = 0.0f;
  for (int i = 0; i < move_order->len; i++) {
    SDL_FPoint pixel_val_geod = geod_to_pixels(&test_rect, geod_path[i]);
    SDL_FPoint pixel_val_vessel = local_to_pixels_snake(&test_rect, local_path[i]);
    SCOPED_TRACE("step=" + std::to_string(i));
    SCOPED_TRACE(
        "\ncurr order:" 
      // + std::to_string(move_order->deltas[i-1].x) + "," + std::to_string(move_order->deltas[i-1].y)  
      // + "\ncurr order:" 
      + std::to_string(move_order->deltas[i].x) + "," + std::to_string(move_order->deltas[i].y)  
      + "\ngeod: "
      + std::to_string(geod_path[i].deg_lat) + "," + std::to_string(geod_path[i].deg_long)  
      + "\nlocal: "
      + std::to_string(local_path[i].y) + "," + std::to_string(local_path[i].x)  
    );
    SDL_FPoint pg = geod_to_pixels(&test_rect, geod_path[i]);
    SDL_FPoint pl = local_to_pixels_snake(&test_rect, local_path[i]);
    float dx = fabsf(pg.x - pl.x);
    float dy = fabsf(pg.y - pl.y);
    max_dx = std::max(max_dx, dx);
    max_dy = std::max(max_dy, dy);
    // if (i > 100){
    //   EXPECT_NE(pixel_val_geod.x, pixel_val_vessel.x) << "Not expected to be exactly equal.";
    //   EXPECT_NE(pixel_val_geod.y, pixel_val_vessel.y) << "Not expected to be exactly equal.";
    // }
    EXPECT_NEAR(pixel_val_geod.x, pixel_val_vessel.x, 10.0) << "Not expected to be too far apart";
    EXPECT_NEAR(pixel_val_geod.y, pixel_val_vessel.y, 10.0) << "Not expected to be too far apart";
  }

  destroy_move_order(move_order);
  destroy_vessel(vessel);
  free(local_path);
  free(geod_path);
}

TEST(ProjDiffTest, Medium){
  int ORDER_LEN = 100;
  int ORDER_SCALE = 1000;
  SDL_FRect test_rect;
  test_rect.w = 10000.0f;
  test_rect.h = 10000.0f;
  test_rect.x = 10000.0f;
  test_rect.y = 10000.0f;
  point_geod null_island = {0.0, 0.0};
  move_order_t *move_order = create_random_move_order(ORDER_LEN, ORDER_SCALE);
  vessel_t *vessel = launch_vessel(null_island, move_order->len);
  point_geod *geod_path = make_path_utm(vessel->pos_geod, move_order);
  point_local *local_path = make_path_snake(vessel->pos_snake, move_order);
  point_geod g = geod_path[move_order->len - 1];
  point_local l = local_path[move_order->len - 1];
  update_vessel_pos(vessel, g, l);

  float max_dx = 0.0f, max_dy = 0.0f;
  for (int i = 0; i < move_order->len; i++) {
    SDL_FPoint pixel_val_geod = geod_to_pixels(&test_rect, geod_path[i]);
    SDL_FPoint pixel_val_vessel = local_to_pixels_snake(&test_rect, local_path[i]);
    SCOPED_TRACE("step=" + std::to_string(i));
    SCOPED_TRACE(
        "\ncurr order:" 
      + std::to_string(move_order->deltas[i].x) + "," + std::to_string(move_order->deltas[i].y)  
      + "\ngeod: "
      + std::to_string(geod_path[i].deg_lat) + "," + std::to_string(geod_path[i].deg_long)  
      + "\nlocal: "
      + std::to_string(local_path[i].y) + "," + std::to_string(local_path[i].x)  
    );SDL_FPoint pg = geod_to_pixels(&test_rect, geod_path[i]);
    SDL_FPoint pl = local_to_pixels_snake(&test_rect, local_path[i]);
    float dx = fabsf(pg.x - pl.x);
    float dy = fabsf(pg.y - pl.y);
    max_dx = std::max(max_dx, dx);
    max_dy = std::max(max_dy, dy);

    if (i > 50){

      // EXPECT_NE(pixel_val_geod.x, pixel_val_vessel.x) << "Not expected to be exactly equal.";
      // EXPECT_NE(pixel_val_geod.y, pixel_val_vessel.y) << "Not expected to be exactly equal.";
    }
    EXPECT_NEAR(pixel_val_geod.x, pixel_val_vessel.x, 5.0) << "Not expected to be too far apart";
    EXPECT_NEAR(pixel_val_geod.y, pixel_val_vessel.y, 5.0) << "Not expected to be too far apart";
  }

  destroy_move_order(move_order);
  destroy_vessel(vessel);
  free(local_path);
  free(geod_path);
}

}
