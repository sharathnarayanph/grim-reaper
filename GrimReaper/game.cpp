#include "game.h"

Game::Game() {
	string resPath = getResourcePath();
	backgroundImage = loadTexture(resPath + "map.png", Globals::renderer);

	list<FrameSetType> fsTypes;

	//Collisions
	FrameSetType colBoxType;
	colBoxType.setName = "collisionBox";
	colBoxType.fsType = FrameSetType::FSTYPE_RECT;

	FrameSetType hitBoxType;
	hitBoxType.setName = "hitBox";
	hitBoxType.fsType = FrameSetType::FSTYPE_RECT;

	FrameSetType dmgType;
	dmgType.setName = "damage";
	dmgType.fsType = FrameSetType::FSTYPE_VAL;

	fsTypes.push_back(colBoxType);
	fsTypes.push_back(hitBoxType);
	fsTypes.push_back(dmgType);

	heroAnimSet = new AnimationSet();
	heroAnimSet->loadAnimationSet("grimReaper.fdset", fsTypes, true, 0, true);

	wallAnimSet = new AnimationSet();
	wallAnimSet->loadAnimationSet("arena.fdset", fsTypes);

	//Hero Entity
	hero = new Hero(heroAnimSet);
	hero->invincibleTimer = 0;
	hero->x = Globals::ScreenWidth / 2;
	hero->y = Globals::ScreenHeight / 2;
	heroInput.hero = hero;
	Entity::entities.push_back(hero);

	//Building walls on all four sides
	int tileSize = 32;
	for (int i = 0; i < Globals::ScreenWidth / tileSize; i++) {
		//Top row
		Arena* newWall = new Arena(wallAnimSet);
		newWall->x = i * tileSize + tileSize/2;
		newWall->y = tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//Bottom row
		newWall = new Arena(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = Globals::ScreenHeight - tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}

	for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++) {
		//Left column
		Arena* newWall = new Arena(wallAnimSet);
		newWall->x = tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//Right column
		newWall = new Arena(wallAnimSet);
		newWall->x = Globals::ScreenWidth - tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}

}

Game::~Game() {
	cleanup(backgroundImage);

	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimSet;
	delete wallAnimSet;

	delete hero;

	Entity::removeAllFromList(&walls, true);
}

void Game::update() {
	bool quit = false;

	SDL_Event e;
	TimeState::timeState.reset();
	//Game loop
	while (!quit) {
		TimeState::timeState.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE: 
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					hero->revive();
					break;

				}
			}
			heroInput.update(&e);
		}

		//Update Entities
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			(*entity)->update();
		}

		draw();
	}

}
void Game::draw() {
	//Clear the screen
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globals::renderer);

	renderTexture(backgroundImage, Globals::renderer, 0, 0);

	//Sort entries based on y axis depth
	Entity::entities.sort(Entity::EntityCompare);

	for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
		(*entity)->draw();
	}

	SDL_RenderPresent(Globals::renderer);
}