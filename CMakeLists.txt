cmake_minimum_required(VERSION 3.10)
project(Sapling)

set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(Sapling)

# Add source files
target_sources(Sapling PRIVATE
    SaplingEngine/main.cpp
    SaplingEngine/Core/Engine.cpp
    SaplingEngine/Core/Canopy/Scene.cpp
    SaplingEngine/Core/Input/Input.cpp
    SaplingEngine/Core/Seedbank/Assets.cpp
    SaplingEngine/ECS/Components/Component.cpp
    SaplingEngine/ECS/Entities/Entity.cpp
    SaplingEngine/ECS/Entities/EntityManager.cpp
    SaplingEngine/Physics/Physics.cpp
    SaplingEngine/Renderer/Sprout.mm
)

# Add header files
target_sources(Sapling PRIVATE
    SaplingEngine/Core/Canopy/Scene.hpp
    SaplingEngine/Core/Engine.hpp
    SaplingEngine/Core/Input/Input.hpp
    SaplingEngine/Core/Seedbank/Assets.hpp
    SaplingEngine/ECS/Components/Component.hpp
    SaplingEngine/ECS/Entities/Entity.hpp
    SaplingEngine/ECS/Entities/EntityManager.hpp
    SaplingEngine/Physics/Physics.hpp
    SaplingEngine/Renderer/Sprout.hpp
    SaplingEngine/Renderer/quad.h
)
set_source_files_properties(Sprout.mm PROPERTIES LANGUAGE CXX)
target_link_libraries(Sapling "-framework Cocoa -framework QuartzCore -framework Metal")
target_link_libraries(Sapling "-framework OpenGL")
	
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Assets/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC COMPONENTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Components/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PREFABS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Prefabs/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC SYSTEMS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/GameContent/Systems/")

# Add include directories
target_include_directories(Sapling PUBLIC
    ${CMAKE_SOURCE_DIR}/SaplingEngine
    /opt/homebrew/include
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}/thirdparty/stb
)
