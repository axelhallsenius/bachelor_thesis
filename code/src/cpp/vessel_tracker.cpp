//#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3_image/SDL_image.h>

#include <time.h>
#define ORDER_LEN 100
#define ORDER_SCALE 100
#define MAX_PATHS 20

#include "map.h"
#include "vessel.h"
#include "draw.h"

typedef enum {
  snake_vessel,
  utm_vessel,
  compare
}view_t;

int main(int, char**)
{
  // Setup SDL
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    printf("Error: SDL_Init(): %s\n", SDL_GetError());
    return -1;
  }

  // Create window with SDL_Renderer graphics context
  Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
  SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL3+SDL_Renderer example", 1280, 720, window_flags);
  if (window == nullptr)
  {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return -1;
  }
  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
  SDL_SetRenderVSync(renderer, 1);
  if (renderer == nullptr)
  {
    SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
    return -1;
  }
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_ShowWindow(window);

  //create textures
  static int texture_width = 0;
  static int texture_height = 0;

  SDL_Surface *surface;
  // char *svg_path;
  // 
  // SDL_asprintf(&svg_path, "%sassets/maps/World_map_nations.svg", SDL_GetBasePath());  /* allocate a string of the full file path */
  // surface = IMG_Load(svg_path); //NOTE: consider using IMG_LoadTexture instead of going the surface route
  // if (!surface) {
  //   SDL_Log("Couldn't load svg: %s", SDL_GetError());
  //   return SDL_APP_FAILURE;
  // }
  //
  // SDL_free(svg_path);  /* done with this, the file is loaded. */

  char *map_path;
  SDL_asprintf(&map_path, "%sassets/maps/paleblue.jpg", SDL_GetBasePath());  /* allocate a string of the full file path */
  //
  // surface = IMG_Load(map_path); //NOTE: consider using IMG_LoadTexture instead of going the surface route
  // if (!surface) {
  //   SDL_Log("Couldn't load map: %s", SDL_GetError());
  //   return SDL_APP_FAILURE;
  // }
  // texture_width = surface->w;
  // texture_height = surface->h;
  texture_width = 8192;
  texture_height = 4096;

  SDL_Texture *map_texture = IMG_LoadTexture(renderer, map_path);
  // SDL_Texture *map_texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_free(map_path);
  if (!map_texture) {
    SDL_Log("Couldn't create static map_texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_DestroySurface(surface);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  
  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);

  // Load Fonts
  io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 24.0f);

   // state
  bool show_demo_window = false;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.816f, 0.922f, 0.988f, 1.00f);


  bool show_grid = false;
  bool show_vessel = true;
  bool show_observer = true;
  bool show_vessel_path = false;
  bool show_observer_path = false;
  bool show_hovering_legend = false;
  bool show_test_point = false;

  srand(time(NULL));

  int grid_scale = 100;
  float zoom = 1.0f;
  int pan_x = 0;
  int pan_y = 0;
  static int win_w;
  static int win_h;
  static view_t view;
  // Define destination rectangle
  SDL_FRect dst_rect;

  float red_color[4] = {0.760f, 0.213f, 0.213f, 1.00f};
  point_geod null_island = {0.0, 0.0};

  canvas_t canvas = {renderer, &dst_rect};

  move_order_t *move_order = create_random_move_order(ORDER_LEN, ORDER_SCALE);

  vessel_t *vessel = launch_vessel(null_island, move_order->len);

  point_geod *geod_path = make_path_utm(vessel->pos_geod, move_order);
  point_local *local_path = make_path_snake(vessel->pos_snake, move_order);

  point_geod g = geod_path[move_order->len - 1];
  point_local l = local_path[move_order->len - 1];
  update_vessel_pos(vessel, g, l);

  // Main loop
  bool done = false;

  while (!done)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      ImGui_ImplSDL3_ProcessEvent(&event);
      if (event.type == SDL_EVENT_QUIT)
        done = true;
      if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
        done = true;

      if (event.type == SDL_EVENT_KEY_DOWN){
        if      (event.key.key == SDLK_RIGHT)
          pan_x = pan_x - 20;
        else if (event.key.key == SDLK_LEFT)
          pan_x = pan_x + 20;
        else if (event.key.key == SDLK_UP)
          pan_y = pan_y + 20;
        else if (event.key.key == SDLK_DOWN)
          pan_y = pan_y - 20;
      }
    }

    // MAIN LOOP
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
    {
      SDL_Delay(10);
      continue;
    }
    SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    SDL_RenderClear(renderer);

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    SDL_GetWindowSize(window, &win_w, &win_h);
    static float f = 0.0f;
    static int counter = 0;

    {
      ImGui::Begin("Options");

      ImGui::Checkbox("Show Grid", &show_grid);
      ImGui::Checkbox("Show Vessel", &show_vessel);
      ImGui::Checkbox("Show Vessel Path", &show_vessel_path);
      
      //Projection selection
      ImGui::Text("View");
      if(ImGui::RadioButton("Naïve Measurement", view == snake_vessel)) { 
        view = snake_vessel; 
      }
      if(ImGui::RadioButton("Universal Transverse Mercator", view == utm_vessel)) { 
        view = utm_vessel; 
      }
      if(ImGui::RadioButton("Compare Paths", view == compare)) { 
        view = compare; 
      }

      ImGui::SliderFloat("Zoom Level", &zoom, 0.10f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
      ImGui::Text("\n");

      float scale_factor = ((float) win_h) / ((float) texture_height);
      dst_rect.w = (((float)texture_width) * scale_factor) * zoom;
      dst_rect.h = (((float)texture_height) * scale_factor) * zoom;
      dst_rect.x = ((win_w - dst_rect.w) / 2.0f) + pan_x;
      dst_rect.y = ((win_h - dst_rect.h) / 2.0f) + pan_y;
      SDL_RenderTexture(renderer, map_texture, NULL, &dst_rect);

      if (ImGui::Button("Move Vessel Rand")){
        
        srand(time(NULL));
        destroy_move_order(move_order);
        move_order = create_random_move_order(ORDER_LEN, ORDER_SCALE);
        free(geod_path);
        free(local_path);
        geod_path = make_path_utm(vessel->pos_geod, move_order);

        local_path = make_path_snake(vessel->pos_snake, move_order);
        point_geod g = geod_path[move_order->len - 1];
        point_local l = local_path[move_order->len - 1];
        update_vessel_pos(vessel, g, l);

      }

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      if (ImGui::Button("Quit")){
        break;
      }
      ImGui::End();
    }
    if(show_grid){
      draw_grid_utm(renderer, &dst_rect);
    }
        
    if(view == utm_vessel || view == compare){
      if (show_vessel) {
        SDL_SetRenderDrawColorFloat(renderer, 1.0f, 0.0f, 0.0f, 1.0f);
        render_geod_path(&canvas, geod_path, move_order->len);
        draw_vessel_utm(&canvas, vessel);
      }
    }
    if (view == snake_vessel || view == compare) {
      if (show_grid) {
      }
      if (show_vessel) {
        SDL_SetRenderDrawColorFloat(renderer, 0.0f, 1.0f, 0.0f, 1.0f);
        render_snake_path(&canvas, local_path, move_order->len);
        draw_vessel_snake(&canvas, vessel);
      }
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
  }

  // Cleanup

  destroy_vessel(vessel);
  destroy_move_order(move_order);

  free(geod_path);
  free(local_path);

  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyTexture(map_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
