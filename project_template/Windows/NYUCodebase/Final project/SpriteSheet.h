#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(const char * image_path);
private:
	GLuint LoadTexture(const char *image_path);
	GLuint textureID;
};