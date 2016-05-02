#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Entity.h"

Entity::Entity(float x, float y, float width, float height)
	: x(x), y(y), width(width), height(height)
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