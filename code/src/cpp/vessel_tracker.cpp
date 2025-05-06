//#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3_image/SDL_image.h>

#define ORDER_LEN 10
#define ORDER_SCALE 10

#include "vessel.h"
#include "draw.h"
// #include "shapes/map.h"
// #include "translation/translation.h"
// #include "test/test.h"

typedef enum {
  snake_vessel,
  utm_vessel,
  compare
}view_t;

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

int main(int, char**)
{
  // Setup SDL
  // [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop starts would likely be your SDL_AppInit() function]
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
  char *svg_path;
  
  SDL_asprintf(&svg_path, "%sassets/maps/World_map_nations.svg", SDL_GetBasePath());  /* allocate a string of the full file path */
  surface = IMG_Load(svg_path); //NOTE: consider using IMG_LoadTexture instead of going the surface route
  if (!surface) {
    SDL_Log("Couldn't load svg: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_free(svg_path);  /* done with this, the file is loaded. */

  texture_width = surface->w;
  texture_height = surface->h;

  SDL_Texture *map_texture = SDL_CreateTextureFromSurface(renderer, surface);
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
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();
  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);

  // Load Fonts
  io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 24.0f);

   // Our state
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

  int grid_scale = 100;
  float zoom = 1.0f;
  int pan_x = 0;
  int pan_y = 0;
  static int win_w;
  static int win_h;
  // static projection_t projection;
  static view_t view;
  //TODO: test for possible incorrect access to dst_rect
  SDL_FRect dst_rect;
  
  // Define destination rectangle
  float red_color[4] = {0.760f, 0.213f, 0.213f, 1.00f};
  point_geod null_island = {0.0, 0.0};

  canvas_t canvas = {renderer, &dst_rect};

  move_order_t *move_order = create_random_move_order(ORDER_LEN, ORDER_SCALE);

  vessel_t *vessel = launch_vessel(null_island, move_order->len);

  SDL_FPoint *snake_path = (SDL_FPoint *) malloc(move_order->len * sizeof(SDL_FPoint));
  SDL_FPoint *utm_path = (SDL_FPoint *) malloc(move_order->len * sizeof(SDL_FPoint));

  // Main loop
  bool done = false;
#ifdef __EMSCRIPTEN__
  // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
  // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
  io.IniFilename = nullptr;
  EMSCRIPTEN_MAINLOOP_BEGIN
#else
  while (!done)
#endif
  {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    // [If using SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your SDL_AppEvent() function]
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

    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppIterate() function]
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
      ImGui::Begin("Options");                          // Create a window called "Hello, world!" and append into it.

      ImGui::Checkbox("Show Grid", &show_grid);      // Edit bools storing our window open/close state

      ImGui::Checkbox("Show Vessel", &show_vessel);      // Edit bools storing our window open/close state
      ImGui::Checkbox("Show Vessel Path", &show_vessel_path);      // Edit bools storing our window open/close state
      
      // ImGui::Checkbox("Show Observer", &show_observer);      // Edit bools storing our window open/close state
      // ImGui::Checkbox("Show Observer Path", &show_observer_path);      // Edit bools storing our window open/close state
      // ImGui::Checkbox("Show Test Point", &show_test_point);      // Edit bools storing our window open/close state
      
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
      // if(ImGui::RadioButton("Transverse Mercator", projection == t_merc)) { 
      //   projection = t_merc; 
      // }

      ImGui::SliderFloat("Zoom Level", &zoom, 0.10f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
      ImGui::Text("\n");

      // if (ImGui::Button("Move Vessel Regular")){
      //   // point_geod pg = {3.0, 2.0};
      //   // snake_move_vessel_deg(vessel, pg);
      // }
        float scale_factor = ((float) win_h) / ((float) texture_height);
        dst_rect.w = (((float)texture_width) * scale_factor) * zoom;
        dst_rect.h = (((float)texture_height) * scale_factor) * zoom;
        dst_rect.x = ((win_w - dst_rect.w) / 2.0f) + pan_x;
        dst_rect.y = ((win_h - dst_rect.h) / 2.0f) + pan_y;
        SDL_RenderTexture(renderer, map_texture, NULL, &dst_rect);
 

      if (ImGui::Button("Move Vessel Rand")){
        
        destroy_move_order(move_order);
        move_order = create_random_move_order(ORDER_LEN, ORDER_SCALE);
        track_vessel_utm(&canvas, vessel, move_order, utm_path);
        track_vessel_snake(&canvas, vessel, move_order, snake_path);


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
        // draw_vessel_snake(vessel);
        // draw_vessel_snake(renderer, &dst_rect, vessel);
        SDL_SetRenderDrawColorFloat(renderer, 1.0f, 0.0f, 0.0f, 1.0f);
        draw_vessel_utm(&canvas, vessel);
        SDL_RenderLines(renderer, utm_path, move_order->len);
      }
    }
    if (view == snake_vessel || view == compare) {
      if (show_grid) {
      }
      if (show_vessel) {
        printf("%d", move_order->deltas[0].x,move_order->deltas[4].x);
        SDL_SetRenderDrawColorFloat(renderer, 0.0f, 1.0f, 0.0f, 1.0f);
        draw_vessel_snake(&canvas, vessel);
        // SDL_SetRenderDrawColorFloat(canvas->renderer, 0.0f, 1.0f, 0.0f, 1.0f);
        SDL_RenderLines(renderer, snake_path, move_order->len);
      }
    }

    // Rendering
    ImGui::Render();


    // if (show_vessel_path){
    //   draw_path(renderer, &dst_rect, vessel);
    // }
    if (show_test_point) {
      // test_geod_draw(renderer,&dst_rect);
      // test_meter_to_geod(renderer, &dst_rect);
      // test_geod_to_meter(renderer, &dst_rect);
    }
    
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
  }
#ifdef __EMSCRIPTEN__
  EMSCRIPTEN_MAINLOOP_END;
#endif

  // Cleanup
  // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppQuit() function]
  destroy_vessel(vessel);
  destroy_move_order(move_order);
  free(snake_path);
  free(utm_path);
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyTexture(map_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
