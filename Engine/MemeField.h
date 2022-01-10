#pragma once

#include "Graphics.h"
#include "SpriteCodex.h"

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
		void Draw( const Vei2& screenPos,bool fucked,Graphics& gfx ) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighborMemeCount( int memeCount );
		bool HasNoNeighbourMemes() const;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborMemes = -1;
	};
public:
	MemeField( int nMemes );
	void Draw( Graphics& gfx ) const;
	RectI GetRect() const;
	void OnRevealClick( const Vei2& screenPos );
	void OnFlagClick( const Vei2& screenPos );
	bool CheckForWin();
private:
	void RevealTile(const Vei2& gridPos);
	Tile& TileAt( const Vei2& gridPos );
	const Tile& TileAt( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid( const Vei2& screenPos );
	int CountNeighborMemes( const Vei2& gridPos );
	void DrawBorder(int size, Color c, Graphics& gfx) const;
private:
	static constexpr int width = 10;
	static constexpr int height = 10;
	bool isFucked = false;
	Tile field[width * height];
	int memes;
	bool hasWon = false;

	static constexpr int borderSize = 10;
	static constexpr Color borderColor = Colors::Blue;

	const int pixelWidth = SpriteCodex::tileSize * width;
	const int pixelHeight = SpriteCodex::tileSize * height;
	const Vei2 center = Vei2(Graphics::ScreenWidth - pixelWidth, Graphics::ScreenHeight - pixelHeight) / 2;
};