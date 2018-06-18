// EngineXGL.cpp : Defines the entry point for the console application.
//

// It nees to be here I don't know why
#include "stdafx.h"
#include "EngineXGL.h"
#include "../EngineXGL/GameInterface.h"

const int kViewportWidth = 800;
const int kViewportHeight = 600;
const float MS2SEC = 1.0f/1000.0f;


#define ATTRIB_POINT 0
#define countof(x) (sizeof(x) / sizeof(0[x]))

static GLuint CompileShader(GLenum type, const GLchar* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint param;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &param);

	if (!param)
	{
		GLchar log[4096];
		glGetShaderInfoLog(shader, sizeof(log), NULL, log);
		printf("error: %s: %s\n", (type == GL_FRAGMENT_SHADER) ? "frag" : "vert", (char*)log);
		//exAssert(false);
	}

	return shader;
}

static GLuint LinkProgram(GLuint vert, GLuint frag)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	GLint param;
	glGetProgramiv(program, GL_LINK_STATUS, &param);

	if (!param)
	{
		GLchar log[4096];
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		printf("error: link: %s\n", (char*)log);
		//exAssert(false);
	}

	return program;
}

const float SQUARE[] = {
	-1.0f,  1.0f,
	-1.0f, -1.0f,
	1.0f,  1.0f,
	1.0f, -1.0f
};

struct GraphicsContext
{
	GLuint mProgram;
	GLint mUniformAngle;
	GLuint mVBOPoint;
	GLuint mVAOPoint;
	float mAngle;
};

static GraphicsContext gc;

void GL_IgnoreError()
{
	glGetError();
}

void exEngine::Run(exGameInterface* pGameInterface)
{
	mGame = pGameInterface;
	Initialize();

	Loop();
}

void exEngine::Initialize()
{
	SDL_Init(SDL_INIT_VIDEO);

	const char* szWindowName = "WINDOW NAME"; // mGame->GetWindowName();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// mWindow = SDL_CreateWindow((szWindowName != nullptr) ? szWindowName : gWindowNameDefault,
	mWindow = SDL_CreateWindow(szWindowName,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, kViewportWidth, kViewportHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);

	//exAssert(mWindow != nullptr);

	SDL_GLContext glcontext = SDL_GL_CreateContext(mWindow);

	glewExperimental = GL_TRUE;
	GLenum res = glewInit();

	// glewInit will cause an error, ignore it
	GL_IgnoreError();

	if (res != GLEW_OK)
	{
		// glewGetErrorString( res )
		// exAssert(false);
	}

	// this makes our buffer swap synchronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	const GLchar *vert_shader =
		"#version 330\n"
		"layout(location = 0) in vec2 point;\n"
		"uniform float angle;\n"
		"void main() {\n"
		"    mat2 rotate = mat2(cos(angle), -sin(angle),\n"
		"                       sin(angle), cos(angle));\n"
		"    gl_Position = vec4(0.75 * rotate * point, 0.0, 1.0);\n"
		"}\n";
	const GLchar *frag_shader =
		"#version 330\n"
		"out vec4 color;\n"
		"void main() {\n"
		"    color = vec4(1.0, 0.15, 0.15, 0);\n"
		"}\n";

	// compile and link OpenGL program
	GLuint vert = CompileShader(GL_VERTEX_SHADER, vert_shader);
	GLuint frag = CompileShader(GL_FRAGMENT_SHADER, frag_shader);

	gc.mProgram = LinkProgram(vert, frag);
	gc.mUniformAngle = glGetUniformLocation(gc.mProgram, "angle");

	glDeleteShader(frag);
	glDeleteShader(vert);

	// prepare vertex buffer object (VBO)
	glGenBuffers(1, &gc.mVBOPoint);
	glBindBuffer(GL_ARRAY_BUFFER, gc.mVBOPoint);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SQUARE), SQUARE, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// prepare vertex array object (VAO)
	glGenVertexArrays(1, &gc.mVAOPoint);
	glBindVertexArray(gc.mVAOPoint);
	glBindBuffer(GL_ARRAY_BUFFER, gc.mVAOPoint);
	glVertexAttribPointer(ATTRIB_POINT, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ATTRIB_POINT);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void ToColor(exColor& colorInt, exColorF& colorF) 
{
	colorF.mColor[0] = colorInt.mColor[0] / 255.0f;
	colorF.mColor[1] = colorInt.mColor[1] / 255.0f;
	colorF.mColor[2] = colorInt.mColor[2] / 255.0f;
	colorF.mColor[3] = colorInt.mColor[3] / 255.0f;
}

void exEngine::OnFrame(float fDeltaT)
{
	ConsumeEvents();

	mGame->Run(fDeltaT);

	gc.mAngle += 1.0f * fDeltaT;
	if (gc.mAngle > 2.0f * M_PI)
		gc.mAngle -= 2.0f * M_PI;

	exColor clearColor;
	mGame->GetClearColor(clearColor);

	exColorF clearColorF;
	ToColor(clearColor, clearColorF);

	glClearColor(clearColorF.mColor[0], clearColorF.mColor[1], clearColorF.mColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(gc.mProgram);
	glUniform1f(gc.mUniformAngle, gc.mAngle);
	glBindVertexArray(gc.mVAOPoint);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, countof(SQUARE) / 2);
	glBindVertexArray(0);
	glUseProgram(0);

	SDL_GL_SwapWindow(mWindow);
}

void exEngine::ConsumeEvents()
{
	SDL_PumpEvents();

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			exit(0);
		}

		mGame->OnEvent(&event);
	}

	mGame->OnEventsConsumed();
}

void exEngine::Loop()
{
	const float fRateRequired = (1 / 60.0f);			// 60 FPS

	unsigned int uLastTicks = SDL_GetTicks();

	while (1)
	{
		unsigned int uNowTicks = SDL_GetTicks();
		unsigned int uFrameTicks = uNowTicks - uLastTicks;

		float fDeltaT = uFrameTicks * MS2SEC;

		if (fDeltaT < fRateRequired)
		{
			// maybe sleep?

			continue;
		}

		OnFrame(fDeltaT);

		uLastTicks = uNowTicks;
	}

	

	
}