cmake_minimum_required(VERSION 3.16)

project("sfml-Gamedev" CXX)
include(cmake/StandardProjectSettings.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if (ENABLE_BUILD_WITH_TIME_TRACE)
    add_compile_definitions(project_options INTERFACE -ftime-trace)
  endif()
endif()
 
# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

if(CPP_STARTER_USE_IMGUI)
  set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES}
                           imgui-sfml/2.1@bincrafters/stable)

  # set(CONAN_EXTRA_OPTIONS ${CONAN_EXTRA_OPTIONS} sfml:shared=False
  # sfml:graphics=True sfml:audio=False sfml:window=True
  # libalsa:disable_python=True)
endif()

# if(CPP_STARTER_USE_IMGUI)
#   set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES}
#                            lua/5.4.3@zimmerk/stable)
# endif()

include(cmake/Conan.cmake)
run_conan()

add_subdirectory(src)

# list of files for which we add a copy rule
set(data_SHADOW scripts)

foreach(item IN LISTS data_SHADOW)
  message(STATUS ${item})
    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/res/${item}"
      COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/res/${item}" "${CMAKE_CURRENT_BINARY_DIR}/res/${item}"
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/res/${item}"
      )
endforeach()

