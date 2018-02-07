cmake_minimum_required(VERSION 3.8)
project(ced)

# Directories
set(LIBDIR "libs")
set(BUILDDIR "build")
set(SRCDIR "src")
set(INCLUDEDIRS "include")

# Include dirs
# NOTE: this comes before add_subdirectory()
include_directories(
	${INCLUDEDIRS}
	${LIBDIR}/include
)

# Compiler flags
#add_compile_options(-Wall -Wextra -Wpedantic)

# Subdirs
add_subdirectory(${SRCDIR}/core)
add_subdirectory(${SRCDIR}/gui)

# Executables
add_executable(debug ${SRCDIR}/main.c)

target_link_libraries(debug 
	# Glfw dependencies
	GL
	m
	X11
	pthread
	Xrandr
	Xi
	dl
	Xinerama
	rt
	Xxf86vm
	Xcursor
	# / Glfw deps

	gui
	core

	# placing this before gui and core causes ld error. Right now
	# this config works. Keep this at the bottom of the list.
	${CMAKE_SOURCE_DIR}/${LIBDIR}/libglfw3.a
) 
