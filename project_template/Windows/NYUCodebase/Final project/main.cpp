#include "Matrix.h"
#include "vector"
#include "Game.h"

using namespace std;

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Music *music;
	music = Mix_LoadMUS("music.mp3");

	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	glViewport(0, 0, 800, 600);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	viewMatrix.Translate(-1.2, 1.2, 0);
	projectionMatrix.setOrthoProjection(-1.33, 1.33, -1.33, 1.33, -1.0, 1.0);
	glUseProgram(program.programID);

	program.setModelMatrix(modelMatrix);
	program.setProjectionMatrix(projectionMatrix);
	program.setViewMatrix(viewMatrix);
	float screenShakeValue = 2.0f;
	float screenShakeSpeed = 2.0f;
	float screenShakeIntensity = 2.0f;
	SDL_Event event;
	bool done = false;
	Game game;
	Mix_PlayMusic(music, -1);
	while (!done) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game.render();
		glLoadIdentity();
		SDL_GL_SwapWindow(displayWindow);
	}
	SDL_Quit();
	return 0;
}
