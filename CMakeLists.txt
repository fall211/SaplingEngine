cmake_minimum_required(VERSION 3.10)
project(Sapling)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/std:c++17)
endif()


# Add executable
add_executable(Sapling)

# Source files
file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.hpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/*.hpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/*.cpp"
)
file(GLOB_RECURSE GAME_CONTENT_SOURCES 
    "${CMAKE_SOURCE_DIR}/GameContent/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/GameContent/**/*.hpp"
    "${CMAKE_SOURCE_DIR}/GameContent/*.cpp"
    "${CMAKE_SOURCE_DIR}/GameContent/*.hpp"
)

target_sources(Sapling PRIVATE ${ENGINE_SOURCES} ${GAME_CONTENT_SOURCES})

# Handle .mm files (macOS-specific)
if(APPLE)
    file(GLOB_RECURSE OBJC_SOURCES "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.mm")
    target_sources(Sapling PRIVATE ${OBJC_SOURCES})
    set_source_files_properties(${OBJC_SOURCES} PROPERTIES LANGUAGE CXX)
endif()

# FMOD setup
set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/fmod")
set(FMOD_INCLUDE_DIR "${FMOD_ROOT}/core/inc")
set(FMOD_STUDIO_INCLUDE_DIR "${FMOD_ROOT}/studio/inc")

if(APPLE)
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

    add_custom_command(TARGET Sapling POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FMOD_CORE_LIB_DIR}/libfmod.dylib"
            "${FMOD_STUDIO_LIB_DIR}/libfmodstudio.dylib"
            $<TARGET_FILE_DIR:Sapling>
    )

    set_target_properties(Sapling PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@executable_path"
    )

elseif(WIN32)
    set(FMOD_CORE_LIB_DIR "${FMOD_ROOT}/core/lib/x86")
    set(FMOD_STUDIO_LIB_DIR "${FMOD_ROOT}/studio/lib/x86")

    target_link_libraries(Sapling PRIVATE
        "${FMOD_CORE_LIB_DIR}/fmod_vc.lib"
        "${FMOD_STUDIO_LIB_DIR}/fmodstudio_vc.lib"
    )

    # Copy FMOD DLLs after build
    add_custom_command(TARGET Sapling POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FMOD_CORE_LIB_DIR}/fmod.dll"
            "${FMOD_STUDIO_LIB_DIR}/fmodstudio.dll"
            $<TARGET_FILE_DIR:Sapling>
    )
endif()

# Include directories
target_include_directories(Sapling PUBLIC
    ${CMAKE_SOURCE_DIR}/SaplingEngine
    ${FMOD_INCLUDE_DIR}
    ${FMOD_STUDIO_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}/thirdparty/stb
    ${CMAKE_SOURCE_DIR}/GameContent
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
    ${CMAKE_SOURCE_DIR}/thirdparty/fmod/studio/inc
    ${CMAKE_SOURCE_DIR}/thirdparty/fmod/core/inc
    ${CMAKE_SOURCE_DIR}/GameContent/Scenes
    ${CMAKE_SOURCE_DIR}/GameContent/Systems
    ${CMAKE_SOURCE_DIR}/GameContent/Components
    ${CMAKE_SOURCE_DIR}/GameContent/Prefabs
)

# Define asset paths
target_compile_definitions(Sapling PUBLIC 
    ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Assets/"
    SPRITES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Assets/Sprites/"
    COMPONENTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Components/"
    PREFABS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Prefabs/"
    SYSTEMS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Systems/"
)

# Debugging Info
message(STATUS "FMOD Core Library Dir: ${FMOD_CORE_LIB_DIR}")
message(STATUS "FMOD Studio Library Dir: ${FMOD_STUDIO_LIB_DIR}")
