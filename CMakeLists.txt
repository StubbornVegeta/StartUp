cmake_minimum_required (VERSION 3.11)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v3.0.0
)


FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(StartUp
  LANGUAGES CXX
  VERSION 1.0.0
)

find_package(Lua)
# include_directories(${LUA_INCLUDE_DIR})

add_executable(StartUp
    src/ui.cpp
    src/main.cpp
    src/config.cpp
    src/keymap.cpp
    src/util/modal.cpp
    src/util/base.cpp
    )

target_include_directories(StartUp PRIVATE  include include/util ${LUA_INCLUDE_DIR})


target_link_libraries(StartUp
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
    ${LUA_LIBRARIES}
)

if (EMSCRIPTEN) 
  string(APPEND CMAKE_CXX_FLAGS " -s USE_PTHREADS") 
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s ASYNCIFY") 
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s PROXY_TO_PTHREAD") 

  # foreach(file "index.html" "run_webassembly.py")
  #   configure_file("src/${file}" ${file})
  # endforeach(file)
endif() 
