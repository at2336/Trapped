#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"

class Spritesheet
{
public:
	Spritesheet();
	Spritesheet(const char * image_path);
	void DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY);
	GLuint LoadTexture(const char *image);
private:
	GLuint textureID;
};
