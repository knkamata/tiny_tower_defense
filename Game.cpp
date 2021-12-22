#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "AIComponent.h"
#include "AIState.h"
#include "Grid.h"
#include "Enemy.h"

Game::Game()
	: mWindow(nullptr),
	  mRenderer(nullptr),
	  mIsRunning(true),
	  mIsUpdatingActors(false)
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return (false);
	}

	mWindow = SDL_CreateWindow("Tiny Tower Defense", 100, 100, 1024, 768, 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return (false);
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return (false);
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return (false);
	}

	LoadData();
	mTicksCount = SDL_GetTicks();

	return (true);
}

void Game::GameLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event	event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	if (keyState[SDL_SCANCODE_B])
	{
		mGrid->BuildTower();
	}

	// Process mouse
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT)
	{
		mGrid->ProcessClick(x, y);
	}

	mIsUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mIsUpdatingActors = false;
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	mIsUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mIsUpdatingActors = false;

	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	vector<Actor *> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 34, 139, 34, 255);
	SDL_RenderClear(mRenderer);

	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	mGrid = new Grid(this);

	// // Testing AIComponent
	// Actor		*a = new Actor(this);
	// AIComponent	*aic = new AIComponent(a);
	// // Register states with AIComponent
	// aic->RegisterState(new AIPatrol(aic));
	// aic->RegisterState(new AIDeath(aic));
	// aic->RegisterState(new AIAttack(aic));
	// // Start in patrol state
	// aic->ChangeState("Patrol");
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture *Game::GetTexture(const string &fileName)
{
	SDL_Texture	*tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface *surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}
		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor *actor)
{
	if (mIsUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor *actor)
{
	auto iter = find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	iter = find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent *sprite)
{
	int		DrawOrder = sprite->GetDrawOrder();
	auto	iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if (DrawOrder < (*iter)->GetDrawOrder())
			break;
	}
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent *sprite)
{
	auto	iter = find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

Enemy *Game::GetNearestEnemy(const Vector2 &pos)
{
	Enemy *best = nullptr;

	if (mEnemies.size() > 0)
	{
		best = mEnemies[0];
		float bestDistSq = (pos - best->GetPosition()).LengthSq();
		for (size_t i = 1; i < mEnemies.size(); i++)
		{
			float newDistSq = (pos - mEnemies[i]->GetPosition()).LengthSq();
			if (newDistSq < bestDistSq)
			{
				bestDistSq = newDistSq;
				best = mEnemies[i];
			}
		}
	}
	return best;
}
