cmake_minimum_required(VERSION 3.10)
project(Springuins)

set(CMAKE_CXX_STANDARD 20)

function(bundle_macos_app target app_name assets_src dylibs)
    set(BUILD_DIR "${CMAKE_SOURCE_DIR}/Build")
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
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Build/Icon.icns" "${RESOURCES_DIR}/Icon.icns"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Build/Info.plist" "${APP_DIR}/Contents/Info.plist"
    )
endfunction()

if(APPLE AND CMAKE_GENERATOR STREQUAL "Xcode")
    add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE)
else()
    add_executable(${CMAKE_PROJECT_NAME})
endif()

if(APPLE)
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/SaplingEngine/Renderer/SproutMetal.mm")
    set_source_files_properties("${CMAKE_SOURCE_DIR}/SaplingEngine/Renderer/SproutMetal.mm"
                                PROPERTIES COMPILE_FLAGS "-x objective-c++")
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

set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/SaplingEngine/thirdparty/fmod")
set(FMOD_CORE_LIB_DIR "${FMOD_ROOT}/core/lib")
set(FMOD_STUDIO_LIB_DIR "${FMOD_ROOT}/studio/lib")

if(APPLE)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework QuartzCore"
        "-framework Metal"
        "-framework MetalKit"
        "${FMOD_CORE_LIB_DIR}/libfmod.dylib"
        "${FMOD_STUDIO_LIB_DIR}/libfmodstudio.dylib"
    )

    # copy FMOD dylibs next to the binary
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

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FMOD_CORE_LIB_DIR}/fmod.dll"
            "${FMOD_STUDIO_LIB_DIR}/fmodstudio.dll"
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
    )
endif()

# set asset path for debug builds
if(APPLE OR WIN32)
    target_compile_definitions(${CMAKE_PROJECT_NAME}
        PUBLIC 
            $<$<CONFIG:Debug>:ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Assets/">
            $<$<CONFIG:Debug>:DEBUG>
    )
endif()

# includes
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

# Xcode-only builds
if(APPLE AND CMAKE_GENERATOR STREQUAL "Xcode")
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Build/Info.plist"
    )

    if(CMAKE_CONFIGURATION_TYPES)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "${CMAKE_BINARY_DIR}/Release/${CMAKE_PROJECT_NAME}.app/Contents/Resources/Assets"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Assets"
                "${CMAKE_BINARY_DIR}/Release/${CMAKE_PROJECT_NAME}.app/Contents/Resources/Assets"
        )
    endif()
    
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/Build/Icon.icns"
            "${CMAKE_BINARY_DIR}/Release/${CMAKE_PROJECT_NAME}.app/Contents/Resources/Icon.icns"
    )
    
endif()

# non-Xcode macOS builds
if(APPLE AND NOT CMAKE_GENERATOR STREQUAL "Xcode" AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(FMOD_DYLIBS
        "${FMOD_CORE_LIB_DIR}/libfmod.dylib"
        "${FMOD_STUDIO_LIB_DIR}/libfmodstudio.dylib"
    )
    bundle_macos_app(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}" "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Assets" "${FMOD_DYLIBS}")
endif()

# windows builds
if (WIN32)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE $<IF:$<CONFIG:Release>,YES,NO>
    )

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Checking config: $<CONFIG>"
        COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Release>,copy_directory,echo>
            "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Assets"
            "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/Assets"
    )
endif()