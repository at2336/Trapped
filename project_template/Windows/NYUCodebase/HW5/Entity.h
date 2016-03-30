#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>


class Entity {
private:
	float x;
	float y;
	int textureID;
	float width;
	float height;
	float XVelo;
	float YVelo;
	float YAccel;

public:
	void setX(float x);
	void setY(float y);
	void setXVelo(float x);
	void setYVelo(float y);
	void setYAccel(float y);
	float getX();
	float getY();
	float getXVelo();
	float getYVelo();
	float getYAccel();
};