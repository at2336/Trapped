#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include "SDL_mixer.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image)
{
	SDL_Surface *surface = IMG_Load(image);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}


void drawSprite(GLint texture, float x, float y, float xChange = 0, float yChange = 0)
{
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	glBindTexture(GL_TEXTURE_2D, texture);
		float vertices[] = { x, y - 6, x + (-x / x) * -2, y, x, y, x, y - 6, x + (-x / x) * -2, y - 6, x + (-x / x) * -2, y };
		float texCoords[] = { 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
}

void drawSpriteBall(GLint texture, float x, float y, float xChange = 0, float yChange = 0)
{
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	glBindTexture(GL_TEXTURE_2D, texture);

	float vertices[] = { -1 * x + xChange, -1 * y + yChange, x + xChange, y + yChange, -1 * x + xChange, y + yChange, 
		-1 * x + xChange, -1 * y + yChange, x + xChange, -1 * y + yChange, x + xChange, y + yChange };
	float texCoords[] = { 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Chunk *someSound;
	someSound = Mix_LoadWAV("some_sound.wav");
	Mix_Music *music;
	music = Mix_LoadMUS("music.mp3");

	Mix_PlayMusic(music, -1);

	glViewport(0, 0, 800, 600);

	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	GLuint pongBlock = LoadTexture("white.png");
	GLuint playerWin = LoadTexture("win.jpg");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	projectionMatrix.setOrthoProjection(-20.0, 20.0, -20.0f, 20.0f, -1.0f, 1.0f);

	glUseProgram(program.programID);

	float lastFrameTicks = 0.0f;

	//could create an entity here
	float player1Y = 16;
	float player2Y = 16;
	float movePlayer1 = 0;
	float movePlayer2 = 0;
	float ballx = 1;
	float bally = 1;
	float moveBallx = 1;
	float moveBally = 0;
	float win = 0;;

	SDL_Event event;
	bool done = false;
	while (!done) {

		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;


		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
			{

			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
			{
				if (player2Y > -13)
				{
					movePlayer2 = 54 * elapsed;
					player2Y = player2Y - movePlayer2;
				}
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_UP)
			{
				if (player2Y < 19)
				{
					movePlayer2 = 54 * elapsed;
					player2Y = player2Y + movePlayer2;
				}
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_W)
			{
				if (player1Y < 19)
				{
					movePlayer1 = 54 * elapsed;
					player1Y = player1Y + movePlayer1;
				}
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_S)
			{
				if (player1Y > -13)
				{
					movePlayer1 = 54 * elapsed;
					player1Y = player1Y - movePlayer1;
				}
			}
		}
		SDL_GL_SwapWindow(displayWindow);

		glBindTexture(GL_TEXTURE_2D, pongBlock);

		glClear(GL_COLOR_BUFFER_BIT);

		if (ballx > 0 && win != 1)
		{
			moveBallx += elapsed * 12;
		}
		else if (ballx < 0 && win != 1)
		{
			moveBallx -= elapsed * 12;
		}
		if (bally > 0 && win != 1)
		{
			moveBally += sin(45) * elapsed * 12;
		}
		if (bally < 0 && win != 1)
		{
			moveBally -= sin(45) * elapsed * 12;
		}

		drawSpriteBall(pongBlock, 1, 1, moveBallx, moveBally);
		drawSprite(pongBlock, -19, player1Y);
		drawSprite(pongBlock, 17, player2Y);

		//glClearColor(0.4f, 0.2f, 0.4f, 1.0f);

		//glGenTextures(1, &plane1);


		if (moveBallx >= 16 && moveBally < player2Y && (moveBally > player2Y - 6) && win != 1)
		{
			moveBallx--;
			ballx = -1;
			Mix_PlayChannel(-1, someSound, 0);
		}
		else if (moveBallx <= -16 && moveBally < player1Y && (moveBally > player1Y - 6) && win != 1)
		{
			ballx = 1;
			moveBallx++;
			Mix_PlayChannel(-1, someSound, 0);
		}
		else if (moveBallx > 19)
		{
			moveBallx = 19;
			moveBally = 0;
			win = 1;
		}
		else if (moveBallx < -19)
		{
			moveBallx = -19;
			moveBally = 0;
			Mix_PlayChannel(-1, someSound, 0);
			win = 1;
		}
		else if (moveBallx < 16 && moveBally >= 19 && win != 1)
		{
			bally = bally * -1;
			Mix_PlayChannel(-1, someSound, 0);
		}
		else if (moveBallx > -16 && moveBally >= 19 && win != 1)
		{
			bally = bally * -1;
			Mix_PlayChannel(-1, someSound, 0);
		}
		else if (moveBallx < 16 && moveBally <= -19 && win != 1)
		{
			bally = bally * -1;
			Mix_PlayChannel(-1, someSound, 0);
		}
		else if (moveBallx > -16 && moveBally <= -19 && win != 1)
		{
			bally = bally * -1;
			Mix_PlayChannel(-1, someSound, 0);
		}
		if (win == 1)
		{
			drawSpriteBall(playerWin, 5, 5);
		}

	}

	SDL_Quit();
	return 0;
}
