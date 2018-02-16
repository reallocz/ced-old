# gui
add_library( gui
	# external
	glad.c

	# GL
	gl/shader.c

	window.c
	font.c
)

# Ignore flags for external deps
set_source_files_properties(glad.c PROPERTIES COMPILE_FLAGS -Wno-pedantic -Wno-error -Wno-everything)
