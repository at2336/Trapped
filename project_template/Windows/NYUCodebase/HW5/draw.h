#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include "ShaderProgram.h"

class Draw{
public:
	void DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY);
	void DrawMap(ShaderProgram *program, int index, int spriteCountX, int spriteCountY);
	void drawSprite(GLint texture, float x, float y, float xChange = 0, float yChange = 0);

private:

};