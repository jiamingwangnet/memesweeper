#pragma once
#include "Vei2.h"
#include "Graphics.h"
#include "RectI.h"

class MemeField
{
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		void SpawnMeme();
		bool HasMeme() const;
		void Draw(const Vei2& screenPos, bool fucked, Graphics& gfx) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeigbourMemeCount(int memeCount);
	private:
		bool hasMeme = false;
		State state = State::Hidden;
		int nNeigbourMemes = -1;
	};

public:
	MemeField(int memes);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void OnRevealClick(const Vei2 screenPos);
	void OnFlagClick(const Vei2 screenPos);
private:
	Tile& TileAt(const Vei2& pos);
	const Tile& TileAt(const Vei2& pos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
	int CountNeighbourMemes(const Vei2& gridPos);
private:
	static constexpr int width = 20;
	static constexpr int height = 16;
	bool isFucked = false;

	Tile field[width * height];
};