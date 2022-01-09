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

void MemeField::Tile::Draw(const Vei2& screenPos, bool fucked,  Graphics& gfx) const
{
	if (!fucked)
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
				SpriteCodex::DrawTileNumber(screenPos, nNeigbourMemes, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			break;
		}
	}
	else
	{
		switch (state)
		{
		case State::Hidden:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			SpriteCodex::DrawTileButton(screenPos, gfx);
			break;
		case State::Flagged:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(screenPos, nNeigbourMemes, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
			break;
		}
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

void MemeField::Tile::ToggleFlag()
{
	assert(!IsRevealed());
	if (state == State::Hidden)
	{
		state = State::Flagged;
	}
	else
	{
		state = State::Hidden;
	}
}

bool MemeField::Tile::IsFlagged() const
{
	return state == State::Flagged;
}

void MemeField::Tile::SetNeigbourMemeCount(int memeCount)
{
	assert(nNeigbourMemes == -1);
	nNeigbourMemes = memeCount;
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

	for (Vei2 gridPos = { 0, 0 }; gridPos.y < height; ++gridPos.y)
	{
		for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
		{
			TileAt(gridPos).SetNeigbourMemeCount(CountNeighbourMemes(gridPos));
		}
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);

	for (Vei2 gridPos = { 0, 0 }; gridPos.y < height; ++gridPos.y)
	{
		for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
		{
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, isFucked, gfx);
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI(0, width * SpriteCodex::tileSize, 0, height * SpriteCodex::tileSize);
}

void MemeField::OnRevealClick(const Vei2 screenPos)
{
	if (!isFucked)
	{
		const Vei2 gridPos = ScreenToGrid(screenPos);
		assert(gridPos.x >= 0 && gridPos.x < width&& gridPos.y >= 0 && gridPos.y < height);
		Tile& tile = TileAt(gridPos);
		if (!tile.IsRevealed() && !tile.IsFlagged())
		{
			tile.Reveal();

			if (tile.HasMeme())
			{
				isFucked = true;
			}
		}
	}
}

void MemeField::OnFlagClick(const Vei2 screenPos)
{
	if(!isFucked)
	{
		const Vei2 gridPos = ScreenToGrid(screenPos);
		assert(gridPos.x >= 0 && gridPos.x < width&& gridPos.y >= 0 && gridPos.y < height);
		Tile& tile = TileAt(gridPos);
		if (!tile.IsRevealed())
		{
			tile.ToggleFlag();
		}
	}
}

MemeField::Tile& MemeField::TileAt(const Vei2& pos)
{
	return field[pos.y * width + pos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2& pos) const
{
	return field[pos.y * width + pos.x];
}

Vei2 MemeField::ScreenToGrid(const Vei2& screenPos)
{
	return screenPos / SpriteCodex::tileSize;
}

int MemeField::CountNeighbourMemes(const Vei2& gridPos)
{
	const int xStart = std::max(0, gridPos.x - 1);
	const int yStart = std::max(0, gridPos.y - 1);
	const int xEnd = std::min(width - 1, gridPos.x + 1);
	const int yEnd = std::min(height - 1, gridPos.y + 1);

	int count = 0;
	for (Vei2 gridPos = { xStart, yStart }; gridPos.y <= yEnd; gridPos.y++)
	{
		for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
		{
			if (TileAt(gridPos).HasMeme())
			{
				count++;
			}
		}
	}

	return count;
}
