cmake_minimum_required(VERSION 3.16)
project(vessel_tracker)

set(SHAPES_SRC
  shapes.c
)
set(SHAPES_H 
  shapes.h
)
target_sources(vessel_tracker PRIVATE ${SHAPES_SRC} ${SHAPES_H})

