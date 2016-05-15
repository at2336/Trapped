#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "Entity.h"
#include <SDL_mixer.h>
using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


class Game
{
public:
	Game();
	GLuint LoadTexture(const char *image);
	void DrawMap(ShaderProgram *program);
	bool readEntityData(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readHeader(std::ifstream &stream);
	void placeEntity(string type, float placeX, float placeY);
	void drawEntities(ShaderProgram program);
	void DrawText(ShaderProgram *program, const char *image_path, string text, float size, float spacing, float x, float y);
	float returnX();
	float returnY();
	void hitLimit();
	void renderAndUpdate();
private:
	bool win;
	unsigned mapHeight;
	unsigned mapWidth;

	int** levelData;
	int spriteCountX = 30;
	int spriteCountY = 30;
	float elapsed;
	float cast;
	float TILE_SIZE = 0.1f;
	float lastFrameTicks = 0.0f;

	vector<Entity*> entities;

	Mix_Chunk *someSound;
};