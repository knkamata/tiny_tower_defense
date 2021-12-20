#pragma once

#include "./include/SDL2/SDL.h"
#include "./include/SDL2_image/SDL_image.h"
#include "Math.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Game
{
public:
	Game();

	bool Initialize();
	void GameLoop();
	void Shutdown();

	void AddActor(class Actor *actor);
	void RemoveActor(class Actor *actor);

	void AddSprite(class SpriteComponent *sprite);
	void RemoveSprite(class SpriteComponent *sprite);

	SDL_Texture *GetTexture(const string &filename);

	// Game specific
	class Grid *GetGrid() { return mGrid; }
	vector<class Enemy *> &GetEnemies() { return mEnemies; }
	class Enemy *GetNearestEnemy(const Vector2 &pos);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	SDL_Window   *mWindow;
	SDL_Renderer *mRenderer;
	Uint32        mTicksCount;
	bool          mIsRunning;

	unordered_map<string, SDL_Texture *> mTextures;

	vector<class Actor *> mActors;
	vector<class Actor *> mPendingActors;
	bool                  mIsUpdatingActors;

	vector<class SpriteComponent *> mSprites;

	// Game specific
	class Grid *mGrid;
	vector<class Enemy *> mEnemies;
	float mNextEnemy;
};
