cmake_minimum_required(VERSION 3.15)
project(AdvancedCache VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(nlohmann_json REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    nlohmann_json::nlohmann_json
    protobuf::libprotobuf
    Threads::Threads
)

# Set compile options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

# Install configuration
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)