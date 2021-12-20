#include "Grid.h"

Grid::Grid(Game *game)
	: Actor(game),
	  mSelectedTile(nullptr)
{
	// 7 rows, 16 columns
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(NumCols);
	}

	// Create tiles
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPosition(Vector2(TileSize / 2.0f + j * TileSize, StartY + i * TileSize));
		}
	}

	// Set start/end tiles
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);

	// Set up adjacency lists
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (i > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i-1][j]);
			}
			if (i < NumRows - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i+1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j-1]);
			}
			if (j < NumCols - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j+1]);
			}
		}
	}

	// Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	mNextEnemy = EnemyTime;
}

void Grid::SelectTile(size_t row, size_t col)
{
	// Make sure it's a valid selection
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase)
	{
		// Deselect previous one
		if (mSelectedTile)
		{
			mSelectedTile->ToggleSelect();
		}
		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
}

void Grid::ProcessClick(int x, int y)
{
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
		{
			SelectTile(y, x);
		}
	}
}

// Implements A* pathfinding
bool Grid::FindPath(Tile *start, Tile *goal)
{
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j]->g = 0.0f;
			mTiles[i][j]->mInOpenSet = false;
			mTiles[i][j]->mInClosedSet = false;
		}
	}

	vector<Tile *> openSet;

	// Set current node to start and add to closed set (not traversing start tile)
	Tile *current = start;
	current->mInClosedSet = true;

	do
	{
		// Add adjacent nodes to open set
		for (Tile *neighbor : current->mAdjacent)
		{
			if (neighbor->mBlocked)
			{
				continue;
			}

			if (!neighbor->mInClosedSet)
			{
				if (!neighbor->mInOpenSet)
				{
					neighbor->mParent = current;
					// Compute and set heuristic (Euclidean distance)
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					// Compute and set g(x): actual cost that is the parent's plus const of traversing edge
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					openSet.emplace_back(neighbor);
					neighbor->mInOpenSet = true;
				}
				else
				{
					// Compute new actual cost (g(x)) if current becomes the parent
					float newG = current->g + TileSize;
					// If this new root is lower cost
					if (newG < neighbor->g)
					{
						// Update
						neighbor->mParent = current;
						neighbor->g = newG;
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		if (openSet.empty())
			break;

		auto iter = min_element(openSet.begin(), openSet.end(), [](Tile *a, Tile *b) { return a->f < b->f; });
		current = *iter;
		openSet.erase(iter);
		current->mInOpenSet = false;
		current->mInClosedSet = true;

	} while (current != goal);

	return (current == goal) ? true : false;
}

void Grid::UpdatePathTiles(Tile *start)
{
	// Reset all tiles to normal except for start/end
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15))
			{
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}
	Tile *t = start->mParent;
	while (t != GetEndTile())
	{
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}

void Grid::BuildTower()
{
	if (mSelectedTile && !mSelectedTile->mBlocked)
	{
		mSelectedTile->mBlocked = true;
		if (FindPath(GetEndTile(), GetStartTile()))
		{
			Tower *t = new Tower(GetGame());
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else
		{
			// This tower would block the path, so don't allow build
			mSelectedTile->mSelected = false;
			FindPath(GetEndTile(), GetStartTile());
		}
		UpdatePathTiles(GetStartTile());
	}
}

Tile *Grid::GetStartTile()
{
	return mTiles[3][0];
}

Tile *Grid::GetEndTile()
{
	return mTiles[3][15];
}

void Grid::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.0f)
	{
		new Enemy(GetGame());
		mNextEnemy += EnemyTime;
	}
}
