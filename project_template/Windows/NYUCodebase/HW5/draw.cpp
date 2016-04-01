#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "draw.h"
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


	GLuint Draw::LoadTexture(const char *image)
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

	void Draw::DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY)
	{
		float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
		float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
		float spriteWidth = 1.0 / (float)spriteCountX;
		float spriteHeight = 1.0 / (float)spriteCountY;
		GLfloat texCoords[] = {
			u, v + spriteHeight,
			u + spriteWidth, v,
			u, v,
			u, v + spriteHeight,
			u + spriteWidth, v + spriteHeight,
			u + spriteWidth, v
		};
		float vertices[] = { -2.0, 1.0, -1.0, 3.0, -2.0, 3.0, -2.0, 1.0, -1.0, 1.0, -1.0, 3.0 };

		GLuint player = LoadTexture("spritesheet.png");
		glBindTexture(GL_TEXTURE_2D, player);

		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program->positionAttribute);
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program->positionAttribute);
		glDisableVertexAttribArray(program->texCoordAttribute);
	}

	void Draw::DrawMap() {
		vector<float> vertexData;
		vector<float> texCoordData;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, sheet->getID());
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		//glTranslatef((-TILE_SIZE * mapWidth * 0.1f), (TILE_SIZE * mapHeight * 0.5f), 0.0f);

		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				if (levelData[y][x] != 0) {

					float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
					float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
					float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
					float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
					vertexData.insert(vertexData.end(), {
						TILE_SIZE * x, -TILE_SIZE * y,
						TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
						(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
						(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
					});
					texCoordData.insert(texCoordData.end(), { u, v,
						u, v + (spriteHeight),
						u + spriteWidth, v + (spriteHeight),
						u + spriteWidth, v
					});
				}
			}
		}

		glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
		glEnableClientState(GL_VERTEX_ARRAY);

		glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDrawArrays(GL_QUADS, 0, vertexData.size() / 2);

		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glPopMatrix();
	}

	void Draw::DrawMap(ShaderProgram *program, int index, int spriteCountX, int spriteCountY)
	{
		float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
		float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
		float spriteWidth = 1.0 / (float)spriteCountX;
		float spriteHeight = 1.0 / (float)spriteCountY;
		GLfloat texCoords[] = {
			u, v + spriteHeight,
			u + spriteWidth, v,
			u, v,
			u, v + spriteHeight,
			u + spriteWidth, v + spriteHeight,
			u + spriteWidth, v
		};
		float vertices[] = { -2.0, 1.0, -1.0, 3.0, -2.0, 3.0, -2.0, 1.0, -1.0, 1.0, -1.0, 3.0 };

		GLuint player = LoadTexture("spritesheet.png");
		glBindTexture(GL_TEXTURE_2D, player);

		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program->positionAttribute);
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program->positionAttribute);
		glDisableVertexAttribArray(program->texCoordAttribute);
	}

	void Draw::drawSprite(GLint texture, float x, float y)
	{
		ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

		glBindTexture(GL_TEXTURE_2D, texture);
		float vertices[] = { -2.0, 1.0, -1.0, 3.0, -2.0, 3.0, -2.0, 1.0, -1.0, 1.0, -1.0, 3.0 };
		float texCoords[] = { 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
	}