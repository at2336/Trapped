#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include "ShaderProgram.h"

class Draw {
public:
	GLuint LoadTexture(const char *image);
	void DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY);
	void DrawMap(ShaderProgram *program);
	void drawSprite(GLint texture, float x, float y);
	bool readEntityData(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readHeader(std::ifstream &stream);

private:
	unsigned mapHeight;
	unsigned mapWidth;

	unsigned char** levelData;

	float tiles = 1.0f;

	int spriteCountX = 16;
	int spriteCountY = 16;
};