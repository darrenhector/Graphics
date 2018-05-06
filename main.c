#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdatomic.h>
#include <time.h>
#include "glew.h"
#include "SDL.h"

typedef struct SDL_State
{
	SDL_Window *window;
	SDL_GLContext *context;
	int ticks,fps,max_fps,active;
}SDL_State;

typedef struct Point2d
{
	GLfloat x,y;
}Point2d;
/*189*/
enum
{
	vertex_count = 4, index_count = 8
};

static const Point2d points[vertex_count] = 
{
	{-0.5f, 0.5f},
	{ 0.5f, 0.5f},
	{ 0.5f,-0.5f},
	{-0.5f,-0.5f}
};

static const GLuint indices[index_count] =
{
	0,1,1,2,2,3,3,0
};

static char title[128] = "";

static const char* vertex_code = "#version 430\n"
	"layout(location = 0) in vec2 aPos;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(aPos,0,1.0f);\n"
	"}\n";

static const char* fragment_code = "#version 430\n"
	"layout(location = 0) out vec4 Color;\n"
	"void main()\n"
	"{\n"
	"Color = vec4(1,1,0,0);\n"
	"}\n";

GLuint create_shader(GLenum shader_type,const GLchar* shader_code);

GLint shader_status(GLuint shader,GLenum pname);

GLchar* shader_log(GLuint shader);

GLuint create_program(GLuint vertex_shader,GLuint fragment_shader);

GLint program_status(GLuint program,GLenum pname);

GLchar* program_log(GLuint program);

void handle_event(SDL_State* state);

void frame_rate(SDL_State* state);

SDL_State* InitSDL(int x,int y,const char* title);

void CloseSDL(SDL_State* state);
int main(int argc,char* argv[])
{
	
	SDL_State *state = NULL;
	GLenum err = 0;
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	
	GLuint program = 0;
	GLuint vertex_shader = 0;
	GLuint fragment_shader = 0;
	
	state = InitSDL(640,480,argv[argc-1]);
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(points[0]), points, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
	
	glGenBuffers(1,&ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	
	vertex_shader = create_shader(GL_VERTEX_SHADER,vertex_code);
	fragment_shader = create_shader(GL_FRAGMENT_SHADER,fragment_code);
	program = create_program(vertex_shader,fragment_shader);
	
	
	while(state->active)
	{
		glClearColor(1,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
		handle_event(state);
		
		glBindVertexArray(vao);
		glUseProgram(program);
		glDrawElements(GL_LINES,index_count,GL_UNSIGNED_INT,NULL);
		glUseProgram(0);
		glBindVertexArray(0);
		
		SDL_GL_SwapWindow(state->window);
		frame_rate(state);
	}
	
	glDeleteProgram(program);
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);	
	
	glDeleteBuffers(1,&ebo);
	glDeleteBuffers(1,&vbo);
	glDeleteVertexArrays(1,&vao);
	err = glGetError();
	CloseSDL(state);
	return err;
}

GLuint create_shader(GLenum shader_type,const GLchar* shader_code)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_code, NULL);
	glCompileShader(shader);
	
	if(shader_status(shader,GL_COMPILE_STATUS)  == GL_FALSE)
	{
		GLchar* log = shader_log(shader);
		fprintf(stderr,"%s\n",log);
		free(log);
		exit(1);
	}
	return shader;
}

GLint shader_status(GLuint shader,GLenum pname)
{
	GLint status = GL_FALSE;
	glGetShaderiv(shader,pname,&status);
	return status;
}

GLchar* shader_log(GLuint shader)
{
	GLchar* log = NULL;
	GLint length = shader_status(shader,GL_INFO_LOG_LENGTH);
	log = (GLchar*)malloc(length * sizeof(*log));
	glGetShaderInfoLog(shader,length,NULL,log);
	return log; 
}

GLuint create_program(GLuint vertex_shader,GLuint fragment_shader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program,vertex_shader);
	glAttachShader(program,fragment_shader);
	glLinkProgram(program);
	
	if(program_status(program,GL_LINK_STATUS)  == GL_FALSE)
	{
		GLchar* log = program_log(program);
		fprintf(stderr,"%s\n",log);
		free(log);
		exit(1);
	}
	
	return program;
}

GLint program_status(GLuint program,GLenum pname)
{
	GLint status = GL_FALSE;
	glGetProgramiv(program,pname,&status);
	return status;
}

GLchar* program_log(GLuint program)
{
	GLchar* log = NULL;
	GLint length = program_status(program,GL_INFO_LOG_LENGTH);
	log = (GLchar*)malloc(length * sizeof(*log));
	glGetProgramInfoLog(program,length,NULL,log);
	return log; 
}

void handle_event(SDL_State* state)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
		{
			state->active = 0;
		}
		if(event.type == SDL_KEYDOWN)
		{
			if(event.key.keysym.sym == SDLK_ESCAPE)
			{
				state->active = 0;
			}
			if(event.key.keysym.sym == SDLK_v)
			{
				SDL_GL_SetSwapInterval(!SDL_GL_GetSwapInterval());

			}
			printf("%s\n", SDL_GetKeyName(event.key.keysym.sym) );
		}
	}
}

void frame_rate(SDL_State* state)
{
	if( (state->ticks + CLOCKS_PER_SEC) < clock())
	{
		if(state->fps > state->max_fps) state->max_fps = state->fps;
		snprintf(title,128,"FPS: %d    MAX FPS: %d",state->fps,state->max_fps);
		SDL_SetWindowTitle(state->window,title);
		state->fps = 1; state->ticks += CLOCKS_PER_SEC;
	}
	else
	{
		++state->fps;
	}
}

SDL_State* InitSDL(int w,int h,const char* window_title)
{
	SDL_Window* window = NULL;
	SDL_GLContext* context = NULL;
	SDL_State* state = NULL;
	
	SDL_Init(SDL_INIT_VIDEO);	
	window = SDL_CreateWindow(window_title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,SDL_WINDOW_OPENGL);
	if(window == NULL)
	{
		fprintf(stderr,"%s\n",SDL_GetError());
		exit(1);
	}
	
	context =  SDL_GL_CreateContext(window);
	if(context == NULL)
	{
		fprintf(stderr,"%s\n",SDL_GetError());
		exit(1);
	}
	
	SDL_GL_SetSwapInterval(SDL_TRUE);
	glewInit();
	
	state = (SDL_State*)calloc(1 , sizeof(*state));
	
	if(state == NULL)
	{
		perror(__FILE__);
		exit(1);
	}
	state->window = window;
	state->context = context;
	state->active = SDL_TRUE;
	return state;
	
}

void CloseSDL(SDL_State* state)
{
	SDL_GL_DeleteContext(state->context);
	SDL_DestroyWindow(state->window);
	SDL_Quit();
	printf("MAX FPS: %d\n",state->max_fps);
	free(state);
}
