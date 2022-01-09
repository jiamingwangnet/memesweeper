#include "MemeField.h"
#include <assert.h>
#include <random>
#include "SpriteCodex.h"

void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
{
	switch (state)
	{
	case State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case State::Revealed:
		if (!HasMeme())
		{
			SpriteCodex::DrawTile0(screenPos, gfx);
		}
		else
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		break;
	}
}

void MemeField::Tile::Reveal()
{
	assert(state == State::Hidden);
	state = State::Revealed;
}

bool MemeField::Tile::IsRevealed() const
{
	return state == State::Revealed;
}

MemeField::MemeField(int memes)
{
	assert(memes > 0 && memes < width * height);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	for (int nSpawned = 0; nSpawned < memes; ++nSpawned)
	{
		Vei2 pos;

		do
		{
			pos = { xDist(rng), yDist(rng) };
		} 
		while (TileAt(pos).HasMeme());

		TileAt(pos).SpawnMeme();
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);

	for (Vei2 gridPos = { 0, 0 }; gridPos.y < height; ++gridPos.y)
	{
		for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
		{
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, gfx);
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI(0, width * SpriteCodex::tileSize, 0, height * SpriteCodex::tileSize);
}

MemeField::Tile& MemeField::TileAt(const Vei2& pos)
{
	return field[pos.y * width + pos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2& pos) const
{
	return field[pos.y * width + pos.x];
}
