#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"

using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

Entity::Entity(float x, float y, float width, float height, string type)
	: x(x), y(y), width(width), height(height), type(type)
{
	XVelo = 0;
	YVelo = 0;
};
	

void Entity::setX(float x)
{
	x = x;
}
void Entity::setY(float y)
{
	y = y;
}
void Entity::setXVelo(float x)
{
	XVelo = x;
}
void Entity::setYVelo(float y)
{
	YVelo = y;
}
float Entity::getX()
{
	return x;
}
float Entity::getY()
{
	return y;
}
float Entity::getXVelo()
{
	return XVelo;
}
float Entity::getYVelo()
{
	return YVelo;
}
void Entity::setCollideTop(bool collide)
{
	collideTop = collide;
}

void Entity::setCollideBottom(bool collide)
{
	collideBottom = collide;
}

void Entity::setCollideRight(bool collide)
{
	collideRight = collide;
}

void Entity::setCollideLeft(bool collide)
{
	collideLeft = collide;
}
void Entity::drawPlayer()
{
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	if (!deadOrWon)
		sprite.DrawSpriteSheetSprite(&program, 29, 30, 30);
}

GLuint Entity::LoadTexture(const char *image)
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

string Entity::getType()
{
	return type;
}

void Entity::DrawSpriteSheetSprite(ShaderProgram *program)
{
	int index;
	if (type == "player")
	{
		index = 20;
		cout << "player1";
	}
	else if (type == "rock")
	{
		index = 283;
	}
	else if (type == "enemy")
	{
		index = 235;
	}
	int spriteCountX = 30;
	int spriteCountY = 30;
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
	float vertices[] = { x, y, x + 1, y + 2, x, y + 2, x, y, x + 1, y, x + 1, y + 2 };

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