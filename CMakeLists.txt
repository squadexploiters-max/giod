cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Mengubah nama project agar sesuai dengan mod Every End milikmu
project(EveryEndEffectApplyer VERSION 1.0.0 LANGUAGES CXX)

# Mencari Geode SDK secara otomatis di server GitHub Actions
find_package(geode REQUIRED)

# Mengumpulkan semua file source code di dalam folder src secara otomatis
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Menggunakan fungsi resmi Geode modern untuk merakit mod
add_geode_mod(
    gemini.every_end_effect_applyer
    VERSION 1.0.0
    SOURCES ${SOURCES}
)
