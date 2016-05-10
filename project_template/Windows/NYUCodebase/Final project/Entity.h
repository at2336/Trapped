#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
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
	SpriteSheet sprite;
	string type;
	bool isStatic = true;
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
	void setCollideTop(bool collide);
	void setCollideBottom(bool collide);
	void setCollideRight(bool collide);
	void setCollideLeft(bool collide);
	void drawPlayer();
	GLuint LoadTexture(const char *image);
	void DrawSpriteSheetSprite(ShaderProgram *program);
	string getType();
	float SIZE = 1.5f;
	float TILE_SIZE = 0.2f;
	bool isStatic();
};