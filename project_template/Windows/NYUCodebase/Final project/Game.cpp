#include "Game.h"

using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

SDL_Event event;
const Uint8 *keys = SDL_GetKeyboardState(NULL);

Game::Game()
{
	win = false;

	string levelFile = "mapdata.txt";
	ifstream infile(levelFile);
	string line;
	bool flag = true;
	while (getline(infile, line) && flag == true) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				flag = false;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[ObjectsLayer]") {
			readEntityData(infile);
		}
	}
	someSound = Mix_LoadWAV("sfx.wav");
}

void Game::DrawText(ShaderProgram *program, const char *image_path, string text, float size, float spacing, float x, float y) 
{
	glEnable(GL_TEXTURE_2D);
	GLuint fontTexture = LoadTexture(image_path);
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size) -x, 0.5f * size-y,
			((size + spacing) * i) + (-0.5f * size) -x, -0.5f * size-y,
			((size + spacing) * i) + (0.5f * size) -x, 0.5f * size-y,
			((size + spacing) * i) + (0.5f * size) -x, -0.5f * size-y,
			((size + spacing) * i) + (0.5f * size) -x, 0.5f * size-y,
			((size + spacing) * i) + (-0.5f * size) -x, -0.5f * size-y,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y + texture_size,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
	glDisable(GL_BLEND);
}

void Game::createLevel2()
{
	if (level == 0)
	{
		string levelFile = "mapdata1.txt";
		ifstream infile(levelFile);
		string line;
		bool flag = true;
		while (getline(infile, line)) {
			if (line == "[header]") {
				if (!readHeader(infile)) {
					flag = false;
				}
			}
			else if (line == "[layer]") {
				readLayerData(infile);
			}
			else if (line == "[ObjectsLayer]") {
				readEntityData(infile);
			}
			level = 1;
		}
	}
}

void Game::createLevel3()
{
	if (level == 1)
	{
		string levelFile = "mapdata2.txt";
		ifstream infile(levelFile);
		string line;
		bool flag = true;
		while (getline(infile, line)) {
			if (line == "[header]") {
				if (!readHeader(infile)) {
					flag = false;
				}
			}
			else if (line == "[layer]") {
				readLayerData(infile);
			}
			else if (line == "[ObjectsLayer]") {
				readEntityData(infile);
			}
			level = 2;
		}
	}
}

void Game::completeLevel3()
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->getType() == "rock" && entities[0]->getX() > 1.81 && entities[0]->getY() < -1.81
			&& entities[i]->getX() > 0.7)
			state = WIN;
		cout << entities[0]->getX() << endl;
	}
}

void Game::renderAndUpdate2()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;

		if (keys[SDL_SCANCODE_LEFT])
		{
			if (entities[0]->getX() > 0)
			{
				entities[0]->setFace("left");
				entities[0]->setXVelo(-0.5);
			}
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			if (entities[0]->getX() < 1.79)
			{
				entities[0]->setFace("right");
				entities[0]->setXVelo(0.5);
			}
		}
		else if (keys[SDL_SCANCODE_UP])
		{
			if (entities[0]->getY() >= -1.8)
			{
				entities[0]->setFace("up");
				entities[0]->setYVelo(-0.5);
			}
		}
		else if (keys[SDL_SCANCODE_DOWN])
		{
			if (entities[0]->getY() <= -0.2)
			{
				entities[0]->setFace("down");
				entities[0]->setYVelo(0.5);
			}
		}
		else if (keys[SDL_SCANCODE_SPACE])
			state = GAMEOVER;
		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->getX() > 0.1 && entities[i]->getX() < 2.0 && entities[i]->getY() > -2.2 && entities[i]->getY() < -0.2
				&& abs(entities[0]->getX() - entities[i]->getX()) < 0.65
				&& abs(entities[0]->getY() - entities[i]->getY()) < 0.65
				&& entities[i]->getType() == "enemy")
			{
				if (entities[0]->getX() > entities[i]->getX())
					entities[i]->setXVelo(2.0 * fixedElapsed);
				if (entities[0]->getX() < entities[i]->getX())
					entities[i]->setXVelo(-2.0 * fixedElapsed);
				if (entities[0]->getY() < entities[i]->getY())
					entities[i]->setYVelo(-2.0 * fixedElapsed);
				if (entities[0]->getY() > entities[i]->getY())
					entities[i]->setYVelo(2.0 * fixedElapsed);
			}
			entities[i]->fixedUpdate();
		}
		entities[0]->fixedUpdate();
	}
}

void Game::renderAndUpdate3()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;

		if (keys[SDL_SCANCODE_LEFT])
		{
			if (entities[0]->getX() > 0)
			{
				entities[0]->setFace("left");
				entities[0]->setXVelo(-0.75);
			}
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			if (entities[0]->getX() < 1.79)
			{
				entities[0]->setFace("right");
				entities[0]->setXVelo(0.4);
			}
		}
		else if (keys[SDL_SCANCODE_UP])
		{
			if (entities[0]->getY() >= -1.8)
			{
				entities[0]->setFace("up");
				entities[0]->setYVelo(-0.4);
			}
		}
		else if (keys[SDL_SCANCODE_DOWN])
		{
			if (entities[0]->getY() <= -0.2)
			{
				entities[0]->setFace("down");
				entities[0]->setYVelo(0.4);
			}
		}
		else if (keys[SDL_SCANCODE_SPACE])
			state = GAMEOVER;
		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->getX() > 0.1 && entities[i]->getX() < 2.0 && entities[i]->getY() > -2.2 && entities[i]->getY() < -0.2
				&& abs(entities[0]->getX() - entities[i]->getX()) < 0.65
				&& abs(entities[0]->getY() - entities[i]->getY()) < 0.65
				&& entities[i]->getType() == "enemy")
			{
				if (entities[0]->getX() > entities[i]->getX())
					entities[i]->setXVelo(2.0 * fixedElapsed);
				if (entities[0]->getX() < entities[i]->getX())
					entities[i]->setXVelo(-2.0 * fixedElapsed);
				if (entities[0]->getY() < entities[i]->getY())
					entities[i]->setYVelo(-2.0 * fixedElapsed);
				if (entities[0]->getY() > entities[i]->getY())
					entities[i]->setYVelo(2.0 * fixedElapsed);
			}
			entities[i]->fixedUpdate();
		}
	}
	entities[0]->fixedUpdate();
	push();
	completeLevel3();
}

void Game::push()
{
	for (int i = 1; i < entities.size(); i++)
	{
		if (entities[0]->getX() + entities[0]->getWidth() > entities[i]->getX() - entities[i]->getWidth() /2 && abs(entities[0]->getY() - entities[i]->getY()) < 0.05
			&& entities[0]->getXVelo() > 0 && entities[i]->getType() != "win" && entities[i]->getType() != "enemy"
			&& entities[0]->getX() + entities[0]->getWidth()/2 < entities[i]->getX() + entities[i]->getWidth()/2)
		{
			entities[0]->setX(entities[i]->getX() - entities[i]->getWidth() - 0.01);
			entities[i]->setXVelo(0.1);
			Mix_PlayChannel(-1, someSound, 0);
		}
		else if (entities[0]->getX() + entities[0]->getWidth() < entities[i]->getX() + entities[i]->getWidth() /2 && abs(entities[0]->getY() - entities[i]->getY()) < 0.05
			&& entities[0]->getXVelo() < 0 && entities[0]->getX() > entities[i]->getX()&& entities[i]->getType() != "win" && entities[i]->getType() != "enemy")
		{
			entities[0]->setX(entities[i]->getX() + entities[i]->getWidth() + 0.01);
			entities[0]->setXVelo(-0.1);
			Mix_PlayChannel(-1, someSound, 0);
		}
		entities[i]->fixedUpdate();
	}
}

void Game::hitEntity()
{
	for (int i = 1; i < entities.size(); i++)
	{
		if (entities[0]->getX() + entities[0]->getWidth() > entities[i]->getX() - entities[i]->getWidth() / 2 && abs(entities[0]->getY() - entities[i]->getY()) < 0.05
			&& entities[0]->getXVelo() > 0 && entities[i]->getType() != "win" && entities[i]->getType() != "enemy")
		{
			entities[0]->setXVelo(0);
			entities[0]->setIsStatic(true);
			entities[0]->setX(entities[i]->getX() - entities[i]->getWidth() * 3.0f);
			Mix_PlayChannel(-1, someSound, 0);
		}
		else if (entities[0]->getX() + entities[0]->getWidth() < entities[i]->getX() + entities[i]->getWidth() * 3 && abs(entities[0]->getY() - entities[i]->getY()) < 0.05
			&& entities[0]->getXVelo() < 0 && entities[0]->getX() > entities[i]->getX() + entities[i]->getWidth() && entities[i]->getType() != "win" && entities[i]->getType() != "enemy")
		{
			entities[0]->setXVelo(0);
			entities[0]->setIsStatic(true);
			entities[0]->setX(entities[i]->getX() + entities[i]->getWidth() * 3.0f);
			Mix_PlayChannel(-1, someSound, 0);
		}
		else if (entities[0]->getY() + entities[0]->getHeight() < entities[i]->getY() + entities[i]->getHeight() * 2 && abs(entities[0]->getX() - entities[i]->getX()) < 0.05
			&& entities[0]->getYVelo() < 0 && entities[i]->getType() != "win" && entities[i]->getType() != "enemy")
		{
			entities[0]->setYVelo(0);
			entities[0]->setIsStatic(true);
			entities[0]->setY(entities[i]->getY() + entities[i]->getHeight() * 3.0f + 0.01f);
			Mix_PlayChannel(-1, someSound, 0);
		}
		else if (entities[0]->getY() + entities[0]->getHeight() > entities[i]->getY() - entities[i]->getHeight() * 3 && abs(entities[0]->getX() - entities[i]->getX()) < 0.05
			&& entities[0]->getYVelo() > 0 && entities[i]->getType() != "win" && entities[0]->getY() >= -1.8 && entities[i]->getType() != "enemy")
		{
			entities[0]->setYVelo(0);
			entities[0]->setIsStatic(true);
			entities[0]->setY(entities[i]->getY() - entities[i]->getHeight() * 3.0f - 0.01f);
			Mix_PlayChannel(-1, someSound, 0);
		}
	}
}

void Game::hide()
{
	if (keys[SDL_SCANCODE_H])
	{
		float ticks = (float)SDL_GetTicks();
		cast += ticks - lastFrameTicks;
		entities[0]->setFace("down");
		cout << cast << endl;
		if (cast > 40000 && cast < 100000)
		{
			entities[0]->setX(-50);
			entities[0]->setY(-50);
		}
	}
}

void Game::hitEnemy()
{
	for (int i = 1; i < entities.size(); i++)
	{
		if (entities[i]->getType() == "enemy"
			&& abs(entities[0]->getX() - entities[i]->getX()) <= 0.06
			&& abs(entities[0]->getY() - entities[i]->getY()) <= 0.06)
			state = GAMEOVER;
	}
}


void trailingMovement()
{

}

void Game::hitWall()
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->getX() > 2.02)
		{
			entities[i]->setX(2.02);
			entities[i]->setCollideRight(true);
			entities[i]->setXVelo(0);
			entities[i]->setIsStatic(true);
			float screenShakeValue = 0;
		}
		else if (entities[i]->getX() < 0.2)
		{
			entities[i]->setX(0.2);
			entities[i]->setCollideLeft(true);
			entities[i]->setXVelo(0);
			entities[i]->setIsStatic(true);
		}
		if (entities[i]->getY() < -2.0)
		{
			entities[i]->setY(-2.0);
			entities[i]->setCollideTop(true);
			entities[i]->setYVelo(0);
			entities[i]->setIsStatic(true);
		}
		else if (entities[i]->getY() > -0.2)
		{
			entities[i]->setY(-0.2);
			entities[i]->setCollideBottom(true);
			entities[i]->setYVelo(0);
			entities[i]->setIsStatic(true);
		}
	}
}

void Game::gameOver(ShaderProgram *program)
{
	Mix_FreeChunk(someSound);
	string lost = "You have lost";
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	DrawText(program, "font1.png", lost, 0.15, 0.025, 0, 0.9);
}

void Game::winner(ShaderProgram *program)
{
	Mix_FreeChunk(someSound);
	string lost = "You won!";
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	DrawText(program, "font1.png", lost, 0.15, 0.025, -0.5, 0.9);
}

void Game::drawEntities(ShaderProgram program)
{
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->DrawSpriteSheetSprite(&program);
	}
}

void Game::completeLevel()
{
		if (entities[0]->getX() > 1.81 && entities[0]->getY() < -1.81)
		{
			if (state == LEVELTWO)
			{
				state = LEVELTHREE;
				clearEntities();
			}
			if (state == LEVELONE)
			{
				state = LEVELTWO;
				clearEntities();
			}
		}
}

void Game::clearEntities()
{
	entities.clear();
}

void Game::render()
{
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	if (state == LEVELONE)
	{
		DrawMap(&program);
		drawEntities(program);
		renderAndUpdate(program);
		hide();
	}
	else if (state == LEVELTWO)
	{
		createLevel2();
		DrawMap(&program);
		drawEntities(program);
		renderAndUpdate2();
		hitEnemy();
		hitWall();
		completeLevel();
		hide();
		hitEntity();
	}
	else if (state == LEVELTHREE)
	{
		createLevel3();
		DrawMap(&program);
		drawEntities(program);
		hitEnemy();
		hitWall();
		renderAndUpdate3();
	}
	else if (state == WIN)
		winner(&program);
	else if (state == GAMEOVER)
		gameOver(&program);
}

void Game::renderAndUpdate(ShaderProgram program)
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;

		if (keys[SDL_SCANCODE_LEFT])
		{
			if (entities[0]->getX() > 0 && entities[0]->getIsStatic())
			{
				entities[0]->setFace("left");
				entities[0]->setXVelo(-4.0);
				entities[0]->setIsStatic(false);
			}
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			if (entities[0]->getX() < 1.79 && entities[0]->getIsStatic())
			{
				entities[0]->setFace("right");
				entities[0]->setXVelo(4.0);
				entities[0]->setIsStatic(false);
			}
		}
		else if (keys[SDL_SCANCODE_UP])
		{
			if (entities[0]->getY() >= -1.8 && entities[0]->getIsStatic())
			{
				entities[0]->setFace("up");
				entities[0]->setYVelo(-4.0);
				entities[0]->setIsStatic(false);
			}
		}
		else if (keys[SDL_SCANCODE_DOWN])
		{
			if (entities[0]->getY() <= -0.2 && entities[0]->getIsStatic())
			{
				entities[0]->setFace("down");
				entities[0]->setYVelo(4.0);
				entities[0]->setIsStatic(false);
			}
		}
		else if (keys[SDL_SCANCODE_SPACE])
		{
			if (state == START)
				state = LEVELONE;
			else 
				state = GAMEOVER;
		}
		entities[0]->fixedUpdate();
	}
	hitWall();
	hitEntity();
	completeLevel();
}

GLuint Game::LoadTexture(const char *image)
{
	SDL_Surface *surface = IMG_Load(image);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}

void Game::DrawMap(ShaderProgram *program)
{
	vector<float> vertexData;
	vector<float> texData;

	glEnable(GL_TEXTURE_2D);
	GLuint mapSheet = LoadTexture("spritesheetTransparent.png");
	glBindTexture(GL_TEXTURE_2D, mapSheet);
	//glTranslatef((-tiles * mapWidth * 0.1f), (tiles * mapHeight * 0.5f), 0.0f);

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0) {
				float u = (float)(((int)levelData[y][x]) % spriteCountX) / (float)spriteCountX;
				float v = (float)(((int)levelData[y][x]) / spriteCountX) / (float)spriteCountY;
				float spriteWidth = 1.0f / (float)spriteCountX;
				float spriteHeight = 1.0f / (float)spriteCountY;

				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y),
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, -TILE_SIZE * y - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});

				texData.insert(texData.end(), {
					u, v + spriteHeight,
					u + spriteWidth, v,
					u, v,
					u, v + spriteHeight,
					u + spriteWidth, v + spriteHeight,
					u + spriteWidth, v
				});
			}
		}
	}

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 2);

	glDisable(GL_TEXTURE_2D);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	glDeleteTextures(1, &mapSheet);
}

bool Game::readHeader(std::ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}
	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new int*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new int[mapWidth];
		}
		return true;
	}
}

bool Game::readLayerData(std::ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);

				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					int val = atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

void Game::placeEntity(string type, float placeX, float placeY)
{
		float w = 1.0f / 16.0f;
		float h = 1.0f / 16.0f;
		Entity* entity = new Entity(placeX, placeY, w, h, type);
		entities.push_back(entity);
}

bool Game::readEntityData(std::ifstream &stream) {
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}
