option(CPP_STARTER_USE_IMGUI "Enable sfml imgui" ON)
option(CPP_STARTER_USE_LUA "Enable lua scripting" ON)

add_definitions( -DBUILD_DIR=${CMAKE_CURRENT_BINARY_DIR})

if(CPP_STARTER_USE_IMGUI)
  message("Using Dear ImGui")
  add_subdirectory(imgui)
endif()

add_executable(game main.cpp)
add_subdirectory(headers)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(LUA REQUIRED lua5.3)
include_directories(${LUA_INCLUDE_DIRS})

target_link_libraries(
  game PRIVATE project_options project_warnings CONAN_PKG::docopt.cpp
                CONAN_PKG::fmt CONAN_PKG::spdlog CONAN_PKG::imgui-sfml headers ${LUA_LIBRARIES})

target_include_directories(game PRIVATE  "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src/headers")

