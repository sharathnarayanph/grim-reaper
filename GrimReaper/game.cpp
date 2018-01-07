#include "game.h"

Game::Game() {
	string resPath = getResourcePath();
	backgroundImage = loadTexture(resPath + "hell.png", Globals::renderer);
	splashImage = loadTexture(resPath + "grimReaperTitle.png", Globals::renderer);
	overlayImage = loadTexture(resPath + "overlay.png", Globals::renderer);

	splashShowing = true;
	overlayTimer = 2;

	SoundManager::soundManager.loadSound("hit", resPath + "hit.wav");
	SoundManager::soundManager.loadSound("enemyHit", resPath + "enemyHit.wav");
	SoundManager::soundManager.loadSound("swing", resPath + "swing.wav");
	SoundManager::soundManager.loadSound("dash", resPath + "dash.wav");
	SoundManager::soundManager.loadSound("growl", resPath + "growl.wav");
	SoundManager::soundManager.loadSound("enemyDie", resPath + "enemyDie.wav");

	song = Mix_LoadMUS(string(resPath + "GrimTheEvil.wav").c_str());
	if (song != NULL)
		Mix_PlayMusic(song, -1);

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

	skeletonAnimSet = new AnimationSet();
	skeletonAnimSet->loadAnimationSet("skeleton.fdset", fsTypes, true, 0, true);

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
	cleanup(splashImage);
	cleanup(overlayImage);

	if (scoreTexture != NULL)
		cleanup(scoreTexture);

	Mix_PausedMusic();
	Mix_FreeMusic(song);

	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimSet;
	delete wallAnimSet;
	delete skeletonAnimSet;

	delete hero;

	Entity::removeAllFromList(&walls, true);
	Entity::removeAllFromList(&enemies, true); 
}

void Game::update() {
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 1;
	bool quit = false;

	SDL_Event e;
	TimeState::timeState.reset();
	//Game loop
	while (!quit) {
		TimeState::timeState.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);
		Entity::removeInactiveEntitiesFromList(&enemies, true);

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE: 
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					if (splashShowing)
						splashShowing = false;
					if (overlayTimer <= 0 && hero->hp < 1){
						//Clean and restart
						enemiesToBuild = 2;
						enemiesBuilt = 0;
						enemyBuildTimer = 3;
						overlayTimer = 2;

						Skeleton::killCnt = 0;

						if (scoreTexture != NULL){
							cleanup(scoreTexture);
							scoreTexture = NULL;
						}

						//Remove existing enemies
						for (list<Entity*>::iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++){
							(*enemy)->active = false;
						}
						hero->revive();
					}

					break;

				}
			}
			heroInput.update(&e);
		}

		if (hero->hp < 1 && overlayTimer > 0){
			overlayTimer -= TimeState::timeState.dT;
		}

		//Update Entities
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			(*entity)->update();
		}

		//Spawn skeletons
		if (hero->hp > 0 && !splashShowing){
			if (enemiesToBuild == enemiesBuilt){
				enemiesToBuild = enemiesToBuild * 2;
				enemiesBuilt = 0;
				enemyBuildTimer = 4;
			}
			enemyBuildTimer -= TimeState::timeState.dT;
			if (enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < 10){
				Skeleton *enemy = new Skeleton(skeletonAnimSet);
				//Set enemies position in arena
				enemy->x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
				enemy->y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
				enemy->invincibleTimer = 0.1;

				enemies.push_back(enemy);
				Entity::entities.push_back(enemy);
			}
		}

		draw();
	}

}
void Game::draw() {
	//Clear the screen
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globals::renderer);

	if (splashShowing){
		renderTexture(splashImage, Globals::renderer, 0, 0);
	}
	else {
		renderTexture(backgroundImage, Globals::renderer, 0, 0);

		//Sort entries based on y axis depth
		Entity::entities.sort(Entity::EntityCompare);

		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			(*entity)->draw();
		}

		if (overlayTimer <= 0 && hero->hp < 1){
			renderTexture(overlayImage, Globals::renderer, 0, 0);
			if (scoreTexture == NULL){
				//Score 
				SDL_Color color = { 255, 255, 255, 255 };

				stringstream ss;
				ss << "Souls reaped: " << Skeleton::killCnt;

				string resPath = getResourcePath();
				scoreTexture = renderText(ss.str(), resPath + "vermin_vibes_1989.ttf", color, 30, Globals::renderer);
			}
			renderTexture(scoreTexture, Globals::renderer, 20, 50);
		}
	}
	SDL_RenderPresent(Globals::renderer);
}