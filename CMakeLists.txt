cmake_minimum_required(VERSION 3.19)
project(term_game_rpg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_COLOR_DIAGNOSTICS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
endif()

option(TERMGAMERPG_UNITY_BUILD "Enable unity build" OFF)
if (TERMGAMERPG_UNITY_BUILD)
	set(CMAKE_UNITY_BUILD ON)
else()
	unset(CMAKE_UNITY_BUILD)
endif()

include_directories(flecs/include)
add_compile_definitions(FLECS_CUSTOM_BUILD FLECS_SYSTEM FLECS_CPP
	FLECS_APP FLECS_EXPR)
#	FLECS_LOG
#	FLECS_META FLECS_META_C  FLECS_DOC)
add_library(flecs STATIC flecs/flecs.c)
set_target_properties(flecs PROPERTIES ENABLE_EXPORTS ON)


include_directories(include)


file(GLOB_RECURSE src_files "src/*.cpp")
file(GLOB_RECURSE include_files "include/*.cpp")

add_executable(game
	${src_files}
	${src_files}
)

target_link_libraries(game PUBLIC curses flecs)

