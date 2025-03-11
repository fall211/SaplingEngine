cmake_minimum_required(VERSION 3.10)
project(Sapling)

set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(Sapling)

file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_SOURCE_DIR}/SaplingEngine/main.cpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.hpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.mm"
)
target_sources(Sapling PRIVATE ${ENGINE_SOURCES})

file(GLOB_RECURSE GAME_CONTENT_SOURCES 
    "${CMAKE_SOURCE_DIR}/GameContent/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/GameContent/**/*.hpp"
    "${CMAKE_SOURCE_DIR}/GameContent/**/*.mm"
)
target_sources(Sapling PRIVATE ${GAME_CONTENT_SOURCES})




set_source_files_properties(Sprout.mm PROPERTIES LANGUAGE CXX)
# target_sources(Sapling PRIVATE Sprout.mm)

# FMOD setup
set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/fmod")
set(FMOD_CORE_LIB_DIR "${FMOD_ROOT}/core/lib")
set(FMOD_STUDIO_LIB_DIR "${FMOD_ROOT}/studio/lib")

target_link_libraries(Sapling PRIVATE
    "-framework Cocoa"
    "-framework QuartzCore"
    "-framework Metal"
    "-framework OpenGL"
    "${FMOD_CORE_LIB_DIR}/libfmod.dylib"
    "${FMOD_STUDIO_LIB_DIR}/libfmodstudio.dylib"
)

# Copy FMOD dylibs to the executable directory
add_custom_command(TARGET Sapling POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FMOD_CORE_LIB_DIR}/libfmod.dylib"
        "${FMOD_STUDIO_LIB_DIR}/libfmodstudio.dylib"
        $<TARGET_FILE_DIR:Sapling>
)

# Set RPATH for the executable
if(APPLE)
    set_target_properties(Sapling PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@executable_path"
    )
endif()


target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Assets/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC SPRITES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Assets/Sprites/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC COMPONENTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Components/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PREFABS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Prefabs/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC SYSTEMS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Systems/")

# Add include directories
target_include_directories(Sapling PUBLIC
    ${CMAKE_SOURCE_DIR}/SaplingEngine
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Audio
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Core
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Core/Canopy
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Core/Input
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Core/Seedbank
    ${CMAKE_SOURCE_DIR}/SaplingEngine/ECS
    ${CMAKE_SOURCE_DIR}/SaplingEngine/ECS/Components
    ${CMAKE_SOURCE_DIR}/SaplingEngine/ECS/Entities
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Physics
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Renderer
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}/thirdparty/stb
    ${CMAKE_SOURCE_DIR}/thirdparty/fmod/studio/inc
    ${CMAKE_SOURCE_DIR}/thirdparty/fmod/core/inc
    ${CMAKE_SOURCE_DIR}/GameContent/Scenes
    ${CMAKE_SOURCE_DIR}/GameContent/Systems
    ${CMAKE_SOURCE_DIR}/GameContent/Components
    ${CMAKE_SOURCE_DIR}/GameContent/Prefabs
    /opt/homebrew/include
)

message(STATUS "FMOD Core Library Dir: ${FMOD_CORE_LIB_DIR}")
message(STATUS "FMOD Studio Library Dir: ${FMOD_STUDIO_LIB_DIR}")