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
	void DrawMap();
	void drawSprite(GLint texture, float x, float y);
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readEntityData(std::ifstream &stream);

private:
	int mapWidth;
	int mapHeight;
	int tile_size;
	unsigned char** levelData;
};