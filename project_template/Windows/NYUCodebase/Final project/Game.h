#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include <vector>
#include "Entity.h"
#include <string>
using namespace std;

enum GameState {LEVELONE, LEVELTWO, START, GAMEOVER};

class Game
{
public:
	Game();
	void hitEntity();
	void hitWall();
	void completeLevel();
	void renderAndUpdate();
	GLuint LoadTexture(const char *image);
	void DrawMap(ShaderProgram *program);
	void drawSprite(GLint texture, float x, float y);
	bool readEntityData(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readHeader(std::ifstream &stream);
	void placeEntity(string type, float placeX, float placeY);
	void drawEntities();
	float distFromPlayer();
private:
	int level;
	string direction;
	unsigned mapHeight;
	unsigned mapWidth;
	int state = START;
	bool win;

	int** levelData;

	float TILE_SIZE = 0.2f;

	int spriteCountX = 30;
	int spriteCountY = 30;

	vector<Entity*> entities;
	vector<Entity*> enemyEntities;
};