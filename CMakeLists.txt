cmake_minimum_required(VERSION 3.10)
project(bustelo)

# Set the source files
set(SRC_CLI src/cli.c src/molido.c)
set(SRC_GUI src/gui.c src/molido.c)

# Set the libraries
set(LIBS_CLI -lm)
set(LIBS_GUI -lm -lgdi32 -lwinmm)

# Set the target executable
set(TARGET bustelo)

# Add the executable targets
add_executable(${TARGET}-cli ${SRC_CLI})
add_executable(${TARGET}-gui ${SRC_GUI})

# Include directory for Raylib
set(RAYLIB_INCLUDE_DIR "C:/w64DevKit/x86_64-w64-mingw32/include/")

# Add the include directory for Raylib
include_directories(${RAYLIB_INCLUDE_DIR})

# Set the libraries for GUI target
set(LIBS_GUI ${LIBS_GUI} raylib)

# Link libraries to the targets
target_link_libraries(${TARGET}-cli ${LIBS_CLI})
target_link_libraries(${TARGET}-gui ${LIBS_GUI})

# Specify the output directories
set(BUILD_DIR "${CMAKE_SOURCE_DIR}/build")

set_target_properties(${TARGET}-cli ${TARGET}-gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${BUILD_DIR}/bin"
)

# Create a custom target for running the CLI executable
add_custom_target(run-cli
    COMMAND ${BUILD_DIR}/bin/${TARGET}-cli data/avatar.png
    DEPENDS ${TARGET}-cli
)

# Create a custom target for running the GUI executable
add_custom_target(run-gui
    COMMAND ${BUILD_DIR}/bin/${TARGET}-gui data/avatar.png
    DEPENDS ${TARGET}-gui
)

# Create aliases for convenience
add_custom_target(cr DEPENDS ${TARGET}-cli run-cli)
add_custom_target(gr DEPENDS ${TARGET}-gui run-gui)

# Clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${BUILD_DIR}
)
