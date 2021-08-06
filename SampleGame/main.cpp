#include "Game.h"

int main()
{
	using namespace PotatoEngine;
	Game sampleGame;

	bool res = sampleGame.Init();
	if (!res)
	{
		return 1;
	}

	sampleGame.Run();

	sampleGame.Reset();

	return 0;
}