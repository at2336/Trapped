#include "Game.h"

using namespace std;

SDL_Event event;

const Uint8 *keys = SDL_GetKeyboardState(NULL);

Game::Game()
{
	mapWidth = 0;
	mapHeight = 0;
	win = false;
	mapWidth = 0;
	mapHeight = 0;
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

void Game::hitEntity()
{
	for (int i = 1; i < entities.size(); i++)
	{
		if (entities[0]->getX() > entities[i]->getX() - entities[i]->getWidth() * 3.0f
			&& entities[0]->getX() <= entities[i]->getX() + entities[i]->getWidth() * 3.0f
			&& entities[0]->getY() >= entities[i]->getY()
			&& entities[0]->getY() < entities[1]->getY() + entities[i]->getHeight() - 0.1f)
		{
			if (entities[0]->getXVelo() > 0)
			{
				entities[0]->setX(entities[i]->getX() - entities[i]->getWidth() * 3.0f);
				entities[0]->setXVelo(0);
				entities[0]->setIsStatic(true);
				Mix_PlayChannel(-1, someSound, 0);
			}
			else if (entities[0]->getXVelo() < 0)
			{
				entities[0]->setX(entities[i]->getX() + entities[i]->getWidth() * 3.0f + 0.01f);
				entities[0]->setXVelo(0);
				entities[0]->setIsStatic(true);
				Mix_PlayChannel(-1, someSound, 0);
			}
			else if (entities[0]->getYVelo() < 0)
			{
				entities[0]->setY(entities[i]->getY() + entities[i]->getHeight() * 3.0f);
				entities[0]->setYVelo(0);
				entities[0]->setIsStatic(true);
				Mix_PlayChannel(-1, someSound, 0);
			}
			else if (entities[0]->getYVelo() > 0)
			{
				entities[0]->setY(entities[i]->getY() - entities[i]->getHeight() * 3.0f);
				entities[0]->setYVelo(0);
				entities[0]->setIsStatic(true);
				Mix_PlayChannel(-1, someSound, 0);
			}
		}
	}
}

float Game::distFromPlayer()
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->getType() == "player")
		{
			return 1;
		}
	}
}

void Game::hitWall()
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->getType() == "player" && entities[i]->getX() > 1.8)
		{
			entities[i]->setX(1.8);
			entities[i]->setCollideRight(true);
			entities[i]->setXVelo(0);
			entities[i]->setIsStatic(true);
			float screenShakeValue = 0;
		}
		else if (entities[i]->getType() == "player" && entities[i]->getX() < 0)
		{
			entities[i]->setX(0);
			entities[i]->setCollideLeft(true);
			entities[i]->setXVelo(0);
			entities[i]->setIsStatic(true);
		}
		if (entities[i]->getType() == "player" && entities[i]->getY() < -1.8)
		{
			entities[i]->setY(-1.8);
			entities[i]->setCollideTop(true);
			entities[i]->setYVelo(0);
			entities[i]->setIsStatic(true);
			cout << entities[i]->getY() << endl;
		}
		else if (entities[i]->getType() == "player" && entities[i]->getY() > 0.1)
		{
			entities[i]->setY(0);
			entities[i]->setCollideBottom(true);
			entities[i]->setYVelo(0);
			entities[i]->setIsStatic(true);
		}
	}
}

void Game::gameOver()
{
	Mix_FreeChunk(someSound);
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
	if (entities[0]->getX() > 1.5 && entities[0]->getY() < -1.5)
		cout << "win";
}

void Game::renderAndUpdate(ShaderProgram program)
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	while (SDL_PollEvent(&event)) {
		if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
		{
			if (entities[0]->getIsStatic() && entities[0]->getX() > 0)
			{
				entities[0]->setXVelo(-2.0f * elapsed);
				entities[0]->setIsStatic(false);
			}
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			if (entities[0]->getIsStatic() && entities[0]->getX() < 1.79)
			{
				entities[0]->setXVelo(2.0f * elapsed);
				entities[0]->setIsStatic(false);
			}
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_UP)
		{
			if (entities[0]->getIsStatic() && entities[0]->getY() >= -1.8)
			{
				entities[0]->setYVelo(-2.0f * elapsed);
				entities[0]->setIsStatic(false);
			}
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
		{
			if (entities[0]->getIsStatic() && entities[0]->getY() <= -1.8f)
			{
				entities[0]->setYVelo(2.0f * elapsed);
				entities[0]->setIsStatic(false);
			}
		}
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}

void Game::drawSprite(ShaderProgram program, GLint texture, float x, float y)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	float vertices[] = { -2.0, 1.0, -1.0, 3.0, -2.0, 3.0, -2.0, 1.0, -1.0, 1.0, -1.0, 3.0 };
	float texCoords[] = { 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);
}

void Game::DrawMap(ShaderProgram *program)
{
	vector<float> vertexData;
	vector<float> texData;

	glEnable(GL_TEXTURE_2D);
	GLuint mapSheet = LoadTexture("spritesheet.png");
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

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
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
		Entity* player = new Entity(placeX, placeY, w, h, type);
		entities.push_back(player);
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
