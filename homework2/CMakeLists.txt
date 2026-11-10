cmake_minimum_required(VERSION 3.20)

project(FizKat LANGUAGES CXX)

add_executable(fleet
    src/main.cpp
    src/items/Inventory.cpp
    src/items/Transport.cpp
    src/items/EnergyConsumer.cpp
    src/items/Things.cpp
    src/fleet/Registry.cpp
    src/fleet/ServiceCenter.cpp
    src/fleet/Fleet.cpp
    src/console/Commands.cpp
    src/console/Console.cpp
    src/items/Inventory.hpp
    src/items/Transport.hpp
    src/items/EnergyConsumer.hpp
    src/items/Things.hpp
    src/fleet/Registry.hpp
    src/fleet/ServiceCenter.hpp
    src/fleet/Fleet.hpp
    src/console/Commands.hpp
    src/console/Console.hpp
    src/common/BoundedValue.hpp
    src/common/Errors.hpp
)

target_include_directories(fleet PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

set_target_properties(fleet PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
    target_compile_options(fleet PRIVATE /W4 /utf-8)
else()
    target_compile_options(fleet PRIVATE -Wall -Wextra -Wpedantic)
endif()
