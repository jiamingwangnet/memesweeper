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
		void Draw(const Vei2& screenPos, Graphics& gfx) const;
	private:
		bool hasMeme;
		State state = State::Hidden;
	};

public:
	MemeField(int memes);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
private:
	Tile& TileAt(const Vei2& pos);
	const Tile& TileAt(const Vei2& pos) const;
private:
	static constexpr int width = 20;
	static constexpr int height = 16;

	Tile field[width * height];
};