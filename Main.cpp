#include "Game.h"

int main(void)
{
	Game	game;

	if (game.Initialize())
		game.GameLoop();
	game.Shutdown();

	return (0);
}
