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


project(Springuins)

set(CMAKE_CXX_STANDARD 20)

add_executable(${CMAKE_PROJECT_NAME})

if(APPLE)
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/SaplingEngine/Renderer/SproutMetal.mm")
    set_source_files_properties("${CMAKE_SOURCE_DIR}/SaplingEngine/Renderer/SproutMetal.mm"
                                PROPERTIES
                                COMPILE_FLAGS "-x objective-c++")
endif()


file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/main.cpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/SaplingEngine/**/*.hpp"
)
file(GLOB_RECURSE GAME_CONTENT_SOURCES 
    "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/**/*.hpp"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${ENGINE_SOURCES} ${GAME_CONTENT_SOURCES})

# FMOD setup
set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/SaplingEngine/thirdparty/fmod")
set(FMOD_CORE_LIB_DIR "${FMOD_ROOT}/core/lib")
set(FMOD_STUDIO_LIB_DIR "${FMOD_ROOT}/studio/lib")

if(APPLE)
    set(FMOD_CORE_LIB_DIR "${FMOD_ROOT}/core/lib")
    set(FMOD_STUDIO_LIB_DIR "${FMOD_ROOT}/studio/lib")

    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework QuartzCore"
        "-framework Metal"
        "-framework MetalKit"
        "${FMOD_CORE_LIB_DIR}/libfmod.dylib"
        "${FMOD_STUDIO_LIB_DIR}/libfmodstudio.dylib"
    )

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FMOD_CORE_LIB_DIR}/libfmod.dylib"
            "${FMOD_STUDIO_LIB_DIR}/libfmodstudio.dylib"
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
    )

    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@executable_path"
    )

elseif(WIN32)
    set(FMOD_CORE_LIB_DIR "${FMOD_ROOT}/core/lib/x86")
    set(FMOD_STUDIO_LIB_DIR "${FMOD_ROOT}/studio/lib/x86")

    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        "${FMOD_CORE_LIB_DIR}/fmod_vc.lib"
        "${FMOD_STUDIO_LIB_DIR}/fmodstudio_vc.lib"
    )

    # Copy FMOD DLLs after build
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FMOD_CORE_LIB_DIR}/fmod.dll"
            "${FMOD_STUDIO_LIB_DIR}/fmodstudio.dll"
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
    )
endif()



# Set RPATH for the executable
if(APPLE)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@executable_path"
    )
endif()

if(APPLE)
    target_compile_definitions(${CMAKE_PROJECT_NAME}
        PUBLIC 
            $<$<CONFIG:Debug>:ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Assets/">
            $<$<CONFIG:Debug>:DEBUG>
    )
elseif(WIN32)
    target_compile_definitions(${CMAKE_PROJECT_NAME}
        PUBLIC 
            $<$<CONFIG:Debug>:ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Assets/">
            $<$<CONFIG:Debug>:DEBUG>
    )
endif()



# Add include directories
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/SaplingEngine
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Core
    ${CMAKE_SOURCE_DIR}/SaplingEngine/ECS
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Utility
    ${CMAKE_SOURCE_DIR}/SaplingEngine/Renderer
    ${CMAKE_SOURCE_DIR}/SaplingEngine/thirdparty
    ${CMAKE_SOURCE_DIR}/SaplingEngine/thirdparty/stb
    ${CMAKE_SOURCE_DIR}/SaplingEngine/thirdparty/fmod/studio/inc
    ${CMAKE_SOURCE_DIR}/SaplingEngine/thirdparty/fmod/core/inc
    ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Scenes
    ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Systems
    ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Components
    ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Prefabs
    /opt/homebrew/include
)


if(APPLE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(FMOD_DYLIBS
        "${FMOD_CORE_LIB_DIR}/libfmod.dylib"
        "${FMOD_STUDIO_LIB_DIR}/libfmodstudio.dylib"
    )
    bundle_macos_app(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}" "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Assets" "${FMOD_DYLIBS}")
endif()

if (WIN32)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Checking config: $<CONFIG>"
        COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Release>,copy_directory,echo>
            "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Assets"
            "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/Assets"
    )

    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE $<IF:$<CONFIG:Release>,YES,NO>
    )


    target_compile_definitions(${CMAKE_PROJECT_NAME}
        PUBLIC 
            $<$<NOT:$<CONFIG:Release>>:DEBUG>
    )


endif()

