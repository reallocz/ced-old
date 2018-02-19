# core
add_library( core
	# Data structures
	ds/strbuf.c

	# Font
	font/font.c

	# Document
	document/line.c
	document/document.c
	document/doccmd.c

	# Input
	input/input.c
	input/keyboard.c

	# Rendering
	rendering/renderer.c
	rendering/line_renderer.c
)

