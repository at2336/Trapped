#include "Entity.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define FIXED_TIMESTEP 0.0166666f

Entity::Entity(float x, float y, float width, float height, string type)
: xPos(x), yPos(y), width(width), height(height), type(type)
{
	XVelo = 0;
	YVelo = 0;
	yAccel = 0.25;
};

float Entity::getWidth()
{
	return width;
}

float Entity::getHeight()
{
	return height;
}

void Entity::setX(float x)
{
	xPos = x;
}
void Entity::setY(float y)
{
	yPos = y;
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
	return xPos;
}
float Entity::getY()
{
	return yPos;
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

GLuint Entity::LoadTexture(const char *image)
{
	SDL_Surface *surface = IMG_Load(image);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
	return textureID;
}

string Entity::getType()
{
	return type;
}

void Entity::setFace(string direction)
{
	face = direction;
}

void Entity::DrawSpriteSheetSprite(ShaderProgram *program)
{
	int index;
	if (type == "player")
	{
		if (face == "up")
			index = 24;
		else if (face == "down")
			index = 19;
		else if (face == "right")
			index = 21;
		else if (face == "left")
			index = 22;
	}
	else if (type == "rock")
	{
		index = 281;
	}
	else if (type == "enemy")
	{
		index = 235;
	}
	else if (type == "win")
	{
		index = 762;
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
	float vertices[] = { xPos, -yPos - (12 * TILE_SIZE), xPos + (1 * TILE_SIZE), -yPos - (1 * TILE_SIZE) * 11, xPos, -yPos - (1 * TILE_SIZE) * 11, xPos, -yPos - (12 * TILE_SIZE),
		xPos + (1 * TILE_SIZE), -yPos - (12 * TILE_SIZE), xPos + (1 * TILE_SIZE), -yPos - (1 * TILE_SIZE) * 11 };
	GLuint player = LoadTexture("spritesheetTransparent.png");
	glBindTexture(GL_TEXTURE_2D, player);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
	glDeleteTextures(1, &player);
	glDisable(GL_BLEND);
}

float Entity::lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}

void Entity::fixedUpdate()
{
	XVelo = lerp(XVelo, 0.0f, FIXED_TIMESTEP * 2.0f);
	YVelo = lerp(YVelo, 0.0f, FIXED_TIMESTEP * 2.0f);
	YVelo += yAccel * FIXED_TIMESTEP;
	xPos += XVelo * FIXED_TIMESTEP;
	yPos += YVelo * FIXED_TIMESTEP;
}

bool Entity::getIsStatic()
{
	return isStatic;
}

void Entity::setIsStatic(bool flag)
{
	isStatic = flag;
}