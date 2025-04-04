cmake_minimum_required(VERSION 3.16)
project(vessel_tracker)

set(IMGUI_SRC
  imgui.cpp
  imgui_demo.cpp
  imgui_draw.cpp
  imgui_impl_sdl3.cpp
  imgui_impl_sdlrenderer3.cpp
  imgui_tables.cpp
  imgui_widgets.cpp
)
set(IMGUI_H 
  imconfig.h
  imgui.h
  imgui_impl_sdl3.h
  imgui_impl_sdlrenderer3.h
  imgui_internal.h
  imstb_rectpack.h
  imstb_textedit.h
  imstb_truetype.h
)
target_sources(vessel_tracker PRIVATE ${IMGUI_SRC} ${IMGUI_H})
