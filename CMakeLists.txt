cmake_minimum_required(VERSION 3.10)

function(bundle_macos_app target app_name assets_src dylibs)
    set(APP_DIR "${CMAKE_BINARY_DIR}/${app_name}.app")
    set(MACOS_DIR "${APP_DIR}/Contents/MacOS")
    set(RESOURCES_DIR "${APP_DIR}/Contents/Resources")

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${MACOS_DIR}"
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@executable_path"
    )

    foreach(dylib ${dylibs})
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dylib}" "${MACOS_DIR}"
        )
    endforeach()

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCES_DIR}/Assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${assets_src}" "${RESOURCES_DIR}/Assets"
    )
endfunction()


project(Sapling)

set(CMAKE_CXX_STANDARD 20)

add_executable(Sapling)

if(APPLE)
    target_sources(Sapling PRIVATE "${CMAKE_SOURCE_DIR}/SaplingEngine/Renderer/SproutMetal.mm")
    set_source_files_properties("${CMAKE_SOURCE_DIR}/SaplingEngine/Renderer/SproutMetal.mm"
                                PROPERTIES
                                COMPILE_FLAGS "-x objective-c++")
endif()


file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_SOURCE_DIR}/GameContent/main.cpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.hpp"
)
file(GLOB_RECURSE GAME_CONTENT_SOURCES 
    "${CMAKE_SOURCE_DIR}/GameContent/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/GameContent/**/*.hpp"
)

target_sources(Sapling PRIVATE ${ENGINE_SOURCES} ${GAME_CONTENT_SOURCES})

# FMOD setup
set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/fmod")
set(FMOD_CORE_LIB_DIR "${FMOD_ROOT}/core/lib")
set(FMOD_STUDIO_LIB_DIR "${FMOD_ROOT}/studio/lib")

if(APPLE)
    set(FMOD_CORE_LIB_DIR "${FMOD_ROOT}/core/lib")
    set(FMOD_STUDIO_LIB_DIR "${FMOD_ROOT}/studio/lib")

    target_link_libraries(Sapling PRIVATE
        "-framework Cocoa"
        "-framework QuartzCore"
        "-framework Metal"
        "-framework MetalKit"
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



# Set RPATH for the executable
if(APPLE)
    set_target_properties(Sapling PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@executable_path"
    )
endif()

if(APPLE)
    target_compile_definitions(Sapling
        PUBLIC 
            $<$<CONFIG:Debug>:ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Assets/">
    )
elseif(WIN32)
    target_compile_definitions(Sapling
        PUBLIC 
            $<$<CONFIG:Debug>:ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Assets/">
    )
endif()



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


if(APPLE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(FMOD_DYLIBS
        "${FMOD_CORE_LIB_DIR}/libfmod.dylib"
        "${FMOD_STUDIO_LIB_DIR}/libfmodstudio.dylib"
    )
    bundle_macos_app(Sapling "Sapling" "${CMAKE_SOURCE_DIR}/GameContent/Assets" "${FMOD_DYLIBS}")
endif()

if (WIN32)
    add_custom_command(TARGET Sapling POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Checking config: $<CONFIG>"
        COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Release>,copy_directory,echo>
            "${CMAKE_SOURCE_DIR}/GameContent/Assets"
            "$<TARGET_FILE_DIR:Sapling>/Assets"
    )

    set_target_properties(Sapling PROPERTIES
        WIN32_EXECUTABLE $<IF:$<CONFIG:Release>,YES,NO>
    )


    target_compile_definitions(Sapling
        PUBLIC 
            $<$<NOT:$<CONFIG:Release>>:DEBUG>
    )


endif()

