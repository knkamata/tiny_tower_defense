#pragma once
#include "Actor.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"

class Grid : public Actor
{
public:
	Grid(class Game* game);

	// Handle a mouse click at the x/y screen locations
	void ProcessClick(int x, int y);

	// Use A* to find a path
	bool FindPath(class Tile* start, class Tile* goal);
	void BuildTower();

	class Tile *GetStartTile();
	class Tile *GetEndTile();

	void UpdateActor(float deltaTime) override;
private:
	void SelectTile(size_t row, size_t col);
	void UpdatePathTiles(class Tile* start);

	class Tile* mSelectedTile;

	vector<vector<class Tile*>> mTiles;

	// Time until next enemy
	float mNextEnemy;
	// Time between enemies
	const float EnemyTime = 1.5f;

	// Rows/columns in grid
	const size_t NumRows = 7;
	const size_t NumCols = 16;

	// Start y position of top left corner
	const float StartY = 192.0f;
	// Width/height of each tile
	const float TileSize = 64.0f;
};
