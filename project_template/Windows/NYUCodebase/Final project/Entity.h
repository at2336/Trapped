#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SpriteSheet.h"
#include <string>
using namespace std;

enum EntityType { ENTITY_PLAYER, ENTITY_ENEMY};

class Entity {
private:
	float xPos;
	float yPos;
	int textureID;
	float width;
	float height;
	float XVelo;
	float YVelo;
	bool collideTop;
	bool collideRight;
	bool collideLeft;
	bool collideBottom;
	bool deadOrWon;
	bool isStatic = true;
	SpriteSheet sprite;
	string type;
	float accel;
	string face = "down";
	int runAnimation[2] = { };
	const int numFrames = 5;
	float animationElapsed = 0.0f;
	float framesPerSecond = 30.0f;
	int currentIndex = 0;
public:
	Entity(float x, float y, float width, float height, string type);
	void setX(float x);
	void setY(float y);
	void setXVelo(float x);
	void setYVelo(float y);
	float getX();
	float getY();
	float getXVelo();
	float getYVelo();
	float getWidth();
	float getHeight();
	void setCollideTop(bool collide);
	void setCollideBottom(bool collide);
	void setCollideRight(bool collide);
	void setCollideLeft(bool collide);
	void drawPlayer();
	void setFace(string direction);
	GLuint LoadTexture(const char *image);
	void DrawSpriteSheetSprite(ShaderProgram *program);
	string getType();
	float SIZE = 1.5f;
	float TILE_SIZE = 0.2f;
	bool getIsStatic();
	float lerp(float v0, float v1, float t);
	void setIsStatic(bool flag);
	float TIMESTEP = 0.016f;
	void fixedUpdate();
};