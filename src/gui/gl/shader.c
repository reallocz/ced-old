#include "glad/glad.h"
#include "gui/gl/shader.h"
#include "core/strbuf.h"
#include "flags.h"
#include <stdio.h>
#include <stdlib.h>

static int _shader_compilesuccess(int id, const char* name)
{
	char infolog[512];
	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(id, 512, NULL, infolog);
		printf("E: %s(id=%d, name=%s): %s\n", __func__, id, name, infolog);
		return 0;
	}
	printf("%s(id=%d, name=%s): compiled successfully.\n", __func__, id, name);
	return 1;
}


static int _shader_linksuccess(int id)
{
	char infolog[512];
	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if(! success) {
		glGetProgramInfoLog(id, 512, NULL, infolog);
		printf("E: %s(id=%d): link failed: %s\n", __func__, id, infolog);
		exit(1);
	}
	printf("%s(id=%d): link successful.\n", __func__, id);
	return 1;
}

static int _shader_create(GLuint type)
{
	int id;
	id = glCreateShader(type);
	if(!id) {
		printf("E: %s: failed to create shader.\n", __func__);
		exit(1);
	}
	return id;
}

static void _shader_compile(int id, const char* path)
{
	strbuf sb = sb_createfrom_file(path);
	if(sb.len == 0) {
		printf("E: %s(id=%d, path=%s): sb.len is 0.\n", __func__, id, path);
		sb_destroy(&sb);
		return;
	} else {
		const char* data = sb_get_cstr(&sb);
		glShaderSource(id, 1, &data, NULL);
		glCompileShader(id);
	}
	sb_destroy(&sb);
}

shader shader_create(const char* vpath, const char* fpath)
{
	shader s = {.id = 0, .flags = 0};

	// Create shaders
	int vs, fs;
	vs = _shader_create(GL_VERTEX_SHADER);
	fs = _shader_create(GL_FRAGMENT_SHADER);
	_shader_compile(vs, vpath);
	_shader_compilesuccess(vs, vpath);
	_shader_compile(fs, fpath);
	_shader_compilesuccess(fs, fpath);

	// Create program
	int pid = glCreateProgram();
	glAttachShader(pid, vs);
	glAttachShader(pid, fs);
	glLinkProgram(pid);
	_shader_linksuccess(pid);
	glDeleteShader(vs);
	glDeleteShader(fs);

	s.id = pid;
	flag_set(&s.flags, f_init);
	return s;
}


void shader_use(shader* sh)
{
	if(! flag_isset(sh->flags, f_init)) {
		printf("E: %s: uninitialized shader\n", __func__);
		return;
	} else {
		glUseProgram(sh->id);
	}
}

