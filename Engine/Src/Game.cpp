#include <iostream>
#include <iomanip>
#include "Model.h"

#include "Game.h"

namespace PotatoEngine
{

bool Game::Init()
{
	return true;
}

int Game::Run()
{
	while (true)
	{
		Update();
	}

	return 0;
}

void Game::Reset()
{
}

Game::~Game()
{
	this->Reset();
}

void Game::Update()
{
}

} // namespace PotatoEngine
