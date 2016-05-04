#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include "ShaderProgram.h"
#include <vector>
using namespace std;

class Draw {
public:
	Draw();
	void Init();
	GLuint LoadTexture(const char *image);
	void DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY);
	void DrawMap(ShaderProgram *program);
	void drawSprite(GLint texture, float x, float y);
	bool readEntityData(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readHeader(std::ifstream &stream);
	void placeEntity(string type, float placeX, float placeY);
private:
	unsigned mapHeight;
	unsigned mapWidth;

	bool win;

	int** levelData;

	float tiles = 0.09f;

	int spriteCountX = 30;
	int spriteCountY = 30;

	vector<Entity*> entities;
};