#include "Tile.h"

Tile::Tile(Game *game)
	: Actor(game),
	  mParent(nullptr),
	  f(0.0f),
	  g(0.0f),
	  h(0.0f),
	  mBlocked(false),
	  mSprite(nullptr),
	  mTileState(EDefault),
	  mSelected(false)
{
	mSprite = new SpriteComponent(this);
	UpdateTexture();
}

void Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}

void Tile::ToggleSelect()
{
	mSelected = !mSelected;
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	string	text;
	switch (mTileState)
	{
		case EStart:
			text = "Assets/TileTan.png";
			break;
		case EBase:
			text = "Assets/TileGreen.png";
			break;
		case EPath:
			if (mSelected)
				text = "Assets/TileGreySelected.png";
			else
				text = "Assets/TileGrey.png";
			break;
		case EDefault:
		default:
			if (mSelected)
				text = "Assets/TileBrownSelected.png";
			else
				text = "Assets/TileBrown.png";
			break;
	}
	mSprite->SetTexture(GetGame()->GetTexture(text));
}
