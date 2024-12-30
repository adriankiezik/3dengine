cmake_minimum_required(VERSION 3.10)

project(3dengine)
set(CMAKE_CXX_STANDARD 17)

# Jolt Physics options
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(INTERPROCEDURAL_OPTIMIZATION ON CACHE BOOL "Enable interprocedural optimizations" FORCE)
set(USE_STATIC_MSVC_RUNTIME_LIBRARY ON CACHE BOOL "Use static MSVC runtime library" FORCE)
set(TARGET_UNIT_TESTS OFF CACHE BOOL "Build unit tests" FORCE)
set(TARGET_HELLO_WORLD OFF CACHE BOOL "Build hello world app" FORCE)
set(TARGET_PERFORMANCE_TEST OFF CACHE BOOL "Build performance test app" FORCE)
set(TARGET_SAMPLES OFF CACHE BOOL "Build samples" FORCE)

find_package(OpenGL REQUIRED)

# Include header only libs
include_directories(include)
include_directories(lib/imgui)
include_directories(lib/imgui/backends)
include_directories(lib/lua/src)
include_directories(lib/sol/include)
include_directories(lib/jolt)

# Add submodule libs
add_subdirectory(lib/glfw)
add_subdirectory(lib/assimp)
add_subdirectory(lib/json)
add_subdirectory(lib/jolt/Build)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCES "lib/imgui/*.cpp")
file(GLOB LUA_SOURCES "lib/lua/src/*.c")

add_library(glad src/glad.c)
add_library(lua STATIC ${LUA_SOURCES})

add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE EDITOR_MODE)

target_link_libraries(${PROJECT_NAME} 
    glad 
    glfw 
    assimp 
    lua 
    nlohmann_json::nlohmann_json
    Jolt
    ${OPENGL_LIBRARIES}
)

if(APPLE)
    set(APP_BUNDLE_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app")
    set(APP_ICON_PATH "${APP_BUNDLE_DIR}/Contents/Resources/${PROJECT_NAME}.icns")

    file(MAKE_DIRECTORY "${APP_BUNDLE_DIR}/Contents/MacOS")
    file(MAKE_DIRECTORY "${APP_BUNDLE_DIR}/Contents/Resources")

    # Copy the executable into the MacOS directory
    set(EXECUTABLE_PATH "${APP_BUNDLE_DIR}/Contents/MacOS/${PROJECT_NAME}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${EXECUTABLE_PATH}
    )

    # Create the Info.plist file
    configure_file(resources/Info.plist.in ${APP_BUNDLE_DIR}/Contents/Info.plist @ONLY)
    # Create icon file
    configure_file(resources/${PROJECT_NAME}.icns ${APP_ICON_PATH} COPYONLY)
    
    message(STATUS "Building for macOS: ${PROJECT_NAME}.app bundle will be created.")
else()
    message(STATUS "Not building for macOS: Skipping .app bundle creation.")
endif()
