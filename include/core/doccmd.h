#pragma once
#include "mytypes.h"

enum doccmd_type {
	INSERT,
	MOVE,
	EDIT,
	SAVE,
	QUIT,
	NOP
};

union doccmd_cmd {
	char c; /**< Character for INSERT type */
	uint n; /**< Multiplier for MOVE type */
};

/**
 * Docment command
 */
typedef struct doccmd {
	uint flags;
	enum doccmd_type type;
	union doccmd_cmd cmd;
} doccmd;

/** Init a doccmd. */
doccmd dc_create(enum doccmd_type type);

