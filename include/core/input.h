#pragma once
#include "gui/window.h"

enum inpev_type {
	INSERT,
	EDIT,
	MOVE,
	NOP
};

union inpev_data {
	struct {
		char c;
	} insert;
};

typedef struct inpev{
	enum inpev_type type;
	union inpev_data data;
} inpev;


/** Primary window key callback.*/
void inp_onkey(window* win, int key, int action, int mods);

