#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "SpriteSheet.h"

enum EntityType { ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_COIN };

class Entity {
private:
	float x;
	float y;
	int textureID;
	float width;
	float height;
	float XVelo;
	float YVelo;

	SpriteSheet sprite;

public:
	Entity(float x, float y, float width, float height);
	void setX(float x);
	void setY(float y);
	void setXVelo(float x);
	void setYVelo(float y);
	float getX();
	float getY();
	float getXVelo();
	float getYVelo();
	float getYAccel();
};