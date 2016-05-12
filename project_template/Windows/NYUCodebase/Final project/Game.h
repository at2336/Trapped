#include "ShaderProgram.h"
#include <vector>
#include "Entity.h"
#include <string>
#include <SDL_mixer.h>
#include "math.h"

using namespace std;

enum GameState {LEVELONE, LEVELTWO, LEVELTHREE, START, GAMEOVER, WIN};

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
	bool readEntityData(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readHeader(std::ifstream &stream);
	void placeEntity(string type, float placeX, float placeY);
	void drawEntities(ShaderProgram program);
	void gameOver(ShaderProgram *program);
	void DrawText(ShaderProgram *program, const char *image_path, string text, float size, float spacing, float x, float y);
	void render();
	void createLevel2();
	void renderAndUpdate2();
	void clearEntities();
	void hitEnemy();
	void hide();
	void push();
	void createLevel3();
	void renderAndUpdate3();
	void completeLevel3();
	void winner(ShaderProgram *program);
private:
	int level = 0;
	string direction;
	unsigned mapHeight;
	unsigned mapWidth;
	int state = LEVELONE;
	bool win;
	int** levelData;

	float TILE_SIZE = 0.2f;

	int spriteCountX = 30;
	int spriteCountY = 30;
	float elapsed;
	float cast;

	vector<Entity*> entities;
	vector<Entity*> enemyEntities;
	float lastFrameTicks = 0.0f;

	float screenShakeSpeed = 0.1f;
	float screenShakeIntensity = 0.2f;
	Mix_Chunk *someSound;
};