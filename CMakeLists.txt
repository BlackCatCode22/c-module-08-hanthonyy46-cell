cmake_minimum_required(VERSION 3.15)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

# --- AUTOMATIC SFML DOWNLOAD (ZIP METHOD) ---
include(FetchContent)

# Build static to avoid needing .dll files
set(BUILD_SHARED_LIBS OFF)
# Disable modules we don't need right now to speed up the build
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)

FetchContent_Declare(
        sfml
        # Download the zip file instead of using Git
        URL https://github.com/SFML/SFML/archive/refs/tags/2.6.1.zip
)

FetchContent_MakeAvailable(sfml)
# -------------------------------------

add_executable(untitled "Retro Gmae .cpp")

# Link SFML libraries
target_link_libraries(untitled sfml-graphics sfml-window sfml-system)

# Link necessary Windows system libraries
# WE REMOVED "freetype" FROM THIS LIST because SFML handles it internally
if(WIN32)
    target_link_libraries(untitled winmm gdi32 opengl32)
endif()