cmake_minimum_required(VERSION 3.16)
project(ImGui)

set(IMGUI_SRC
  imgui.cpp
  imgui_demo.cpp
  imgui_draw.cpp
  imgui_impl_sdl3.cpp
  imgui_impl_sdlrenderer3.cpp
  imgui_tables.cpp
  imgui_widgets.cpp
)

set(IMGUI_HEADERS
  imconfig.h
  imgui.h
  imgui_impl_sdl3.h
  imgui_impl_sdlrenderer3.h
  imgui_internal.h
  imstb_rectpack.h
  imstb_textedit.h
  imstb_truetype.h
)

add_library(ImGui STATIC ${IMGUI_SRC} ${IMGUI_HEADERS})

target_include_directories(ImGui PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/vendored/SDL/include
)

target_link_libraries(ImGui PUBLIC SDL3::SDL3)
