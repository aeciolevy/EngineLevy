// EngineXGL.cpp : Defines the entry point for the console application.
//

// It nees to be here I don't know why
#include "stdafx.h"
#include "../EngineXGL/Engine/EngineXGL.h"
#include "../EngineXGL/Game/Header/GameInterface.h"
#include "Engine\exMatrix4.h"
#include "Game\Header\Output.h"

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

float SQUARE[] = {
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
	mGame->Initialize(this);
	Initialize();
	InitializeShaders();
	Loop();

}


void exEngine::InitializeShaders()
{
	glEnable(GL_DEPTH_TEST);

	// updates to the shaders so they they'll A) draw in the correct space B) we can control the colour
	const GLchar *vert_shader =
		"#version 330\n"
		"layout(location = 0) in vec2 point;\n"
		"uniform mat4 model, view, proj;\n"
		"uniform vec2 size;\n"
		"void main() {\n"
		"    gl_Position = proj * view * model * vec4(size * point, 0.0, 1.0);\n"
		"}\n";
	const GLchar *frag_shader =
		"#version 330\n"
		"layout(location = 0) out vec4 color;\n"
		"uniform vec3 in_color;\n"
		"void main() {\n"
		"    color = vec4(in_color, 0);\n"
		"}\n";

	// compile and link OpenGL program
	GLuint vert = CompileShader(GL_VERTEX_SHADER, vert_shader);
	GLuint frag = CompileShader(GL_FRAGMENT_SHADER, frag_shader);

	gc.mProgram = LinkProgram(vert, frag);
	gc.mUniformAngle = glGetUniformLocation(gc.mProgram, "in_color");

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

	exColor clearColor;
	mGame->GetClearColor(clearColor);

	exColorF clearColorF;
	ToColor(clearColor, clearColorF);

	glClearColor(clearColorF.mColor[0], clearColorF.mColor[1], clearColorF.mColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mGame->Run(fDeltaT);

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

void exOrthographicProjectionMatrix(exMatrix4* pOut, float fWidth, float fHeight, float fNearPlane, float fFarPlane)
{
	// http://msdn.microsoft.com/en-us/library/windows/desktop/dd373965%28v=vs.85%29.aspx
	// http://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/orthographic-projection-matrix
	// https://blog.demofox.org/2017/03/31/orthogonal-projection-matrix-plainly-explained/

	pOut->m11 = 2.0f / fWidth;
	pOut->m12 = 0.0f;
	pOut->m13 = 0.0f;
	pOut->m14 = 0.0f;

	pOut->m21 = 0.0f;
	pOut->m22 = 2.0f / -fHeight;
	pOut->m23 = 0.0f;
	pOut->m24 = 0.0f;

	pOut->m31 = 0.0f;
	pOut->m32 = 0.0f;
	pOut->m33 = -2.0f / (fFarPlane - fNearPlane);
	pOut->m34 = 0.0f;

	pOut->m41 = -1.0f;
	pOut->m42 = 1.0f;
	pOut->m43 = -((fFarPlane + fNearPlane) / (fFarPlane - fNearPlane));
	pOut->m44 = 1.0f;
}

void exMakeTranslationMatrix(exMatrix4* pOut, const exVector2& v2Position)
{
	pOut->m11 = 1.0f;
	pOut->m22 = 1.0f;
	pOut->m33 = 1.0f;
	pOut->m44 = 1.0f;

	pOut->m12 = pOut->m13 = pOut->m14 = 0.0f;
	pOut->m21 = pOut->m23 = pOut->m24 = 0.0f;
	pOut->m31 = pOut->m32 = pOut->m34 = 0.0f;

	pOut->m41 = v2Position.x;	// identity matrix apart from this translation
	pOut->m42 = v2Position.y;
	pOut->m43 = 0.0f;
}

void exEngine::DrawBox(const exVector2& v2P1, const exVector2& v2P2, const exColor& color, int nLayer)
{
	// the three matrices we need
	exMatrix4 orthographicProjection;
	exMatrix4 model;
	exMatrix4 view;

	// projection matrix
	exOrthographicProjectionMatrix(&orthographicProjection, 800.0f, 600.0f, -100.0f, 100.0f);

	// view matrix (really just an identity matrix, maybe optimize this call?)
	exVector2 vPosition;
	vPosition = v2P1;
	exMakeTranslationMatrix(&view, vPosition);

	// positions of the uniforms
	int view_mat_location;
	int proj_mat_location;
	int model_mat_location;
	int size_vec_location;

	glUseProgram(gc.mProgram);

	float colorTest[3] = { 1.0f, 0.5f, 0.15f };

	glUniform3fv(gc.mUniformAngle, 1, &colorTest[0]);

	exVector2 objPosition;
	objPosition = v2P2;
	exMakeTranslationMatrix(&model, objPosition); // position of the object
	model.m43 = 2.0f; // this is the "layer", cast from int to float

	view_mat_location = glGetUniformLocation(gc.mProgram, "view");
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.ToFloatPtr());
	proj_mat_location = glGetUniformLocation(gc.mProgram, "proj");
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, orthographicProjection.ToFloatPtr());
	model_mat_location = glGetUniformLocation(gc.mProgram, "model");
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model.ToFloatPtr());
	size_vec_location = glGetUniformLocation(gc.mProgram, "size");
	float size[2] = {objPosition.x - vPosition.x, objPosition.y - vPosition.y };
	glUniform2fv(size_vec_location, 1, &size[0]);

	glUseProgram(gc.mProgram);
	glBindVertexArray(gc.mVAOPoint);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, countof(SQUARE) / 2);
	glBindVertexArray(0);
	glUseProgram(0);
}


