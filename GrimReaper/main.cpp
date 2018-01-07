#include <cstdlib>
#include <ctime> 
#include "util.h"
#include "drawing_functions.h"
#include "SDL_mixer.h"
#include "globals.h"
#include "game.h"

int main(int argc, char **agv){

	srand(time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cout << "error initialising SDL" << endl;
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Grim Reaper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		Globals::ScreenWidth*Globals::ScreenScale, Globals::ScreenHeight*Globals::ScreenScale
		, SDL_WINDOW_SHOWN); //SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
	if (window == nullptr){
		SDL_Quit(); 
		cout << "window error" << endl;
		return 1;
	}

	Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Globals::renderer == nullptr){
		cleanup(window);
		SDL_Quit();
		cout << "renderer error" << endl;
		return 1;
	}

	SDL_RenderSetLogicalSize(Globals::renderer, Globals::ScreenWidth, Globals::ScreenHeight);

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		SDL_Quit();
		cout << "sdl image did not initialise" << endl;
		return 1;
	}

	if (TTF_Init() != 0){
		SDL_Quit();
		cout << "sdl ttf did not initialise" << endl;
		return 1;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_Quit();
		cout << "mixer did not initalise" << endl;
		return 1;
	}

	Game game;
	game.update();

	cleanup(Globals::renderer);
	cleanup(window);

	SDL_Quit();
	return 0;


}