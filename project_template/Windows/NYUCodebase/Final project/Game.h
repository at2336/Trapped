#include "ShaderProgram.h"
#include <vector>
#include "Entity.h"
#include <string>
#include <SDL_mixer.h>

using namespace std;

enum GameState {LEVELONE, LEVELTWO, START, GAMEOVER};

class Game
{
public:
	Game();
	void hitEntity();
	void hitWall();
	void completeLevel();
	void renderAndUpdate(ShaderProgram program);
	GLuint LoadTexture(const char *image);
	void DrawMap(ShaderProgram *program);
	void drawSprite(ShaderProgram program, GLint texture, float x, float y);
	bool readEntityData(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readHeader(std::ifstream &stream);
	void placeEntity(string type, float placeX, float placeY);
	void drawEntities(ShaderProgram program);
	float distFromPlayer();
	void gameOver();
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
	float elapsed;
	vector<Entity*> entities;
	vector<Entity*> enemyEntities;
	float lastFrameTicks = 0.0f;

	Mix_Chunk *someSound;
};