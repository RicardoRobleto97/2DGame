#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "smartSprite.h"
#include "player.h"
#include "multisprite.h"
#include "twoWayMultiSprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"

Engine::~Engine() { 
  //delete sprites
	for( auto& sprite : sprites ) {
    delete sprite;
  }
	delete player;
	delete strategy;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor")),
	stars("stars",Gamedata::getInstance().getXmlInt("stars/factor")),
  viewport( Viewport::getInstance() ),
	menuEngine(),
	sound(),
	sprites(),
	player(new Player("Ship")),
	strategy(new PerPixelCollisionStrategy),
	collision(false),
	godMode(false),
  makeVideo( false )
{
	int spriteCount = 0;
	int catcherCount = Gamedata::getInstance().getXmlInt("Catcher/count");
	spriteCount = catcherCount;
	sprites.reserve(spriteCount);

	Vector2f pos = player->getPosition();
	int w = player->getScaledWidth();
	int h = player->getScaledHeight();

	//add catchers
	for(int i = 0; i < catcherCount; i++) {
  	sprites.emplace_back(new SmartSprite("Catcher", pos, w, h));
		static_cast<Player*>(player)->attach(sprites[i]);
	}

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  world.draw();
	stars.draw();

  //draw sprites
	for( auto& sprite : sprites ) {
    sprite->draw();
  }

	player->draw();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
	auto it = sprites.begin();
	while(it != sprites.end()) {
		//if an enemy hit the player
		if(strategy->execute(*player, **it) && !collision) {
			collision = true;
			static_cast<Player*>(player)->detach(*it);
			if(!godMode) player->explode();
			(*it)->explode();
			sound[1];
		}
		//check bullet collision
		if(static_cast<Player*>(player)->collided(*it)){
			static_cast<Player*>(player)->detach(*it);
			(*it)->explode();
			sound[2];
		}
		++it;
	}
}

void Engine::update(Uint32 ticks) {
	checkForCollisions();	
	auto it = sprites.begin();
	while(it != sprites.end()) {
			(*it)->update(ticks);
			++it;
	}
	player->update(ticks);
	
	it = sprites.begin();
	while(it != sprites.end()) {
		if((*it)->getExplodeState()) {
			it = sprites.erase(it);
		}
		else ++it;
	}

	if(player->getExplodeState()) { 
		collision = false;
		menuEngine.redrawBack();
		viewport.drawGameOver(); 
		SDL_RenderPresent(renderer);
		clock.pause();
	}
	else if( sprites.size() == 0) {
		menuEngine.redrawBack();
		viewport.drawWin();
		SDL_RenderPresent(renderer);
		sound[3];		
		clock.pause();
	}

  world.update();
	stars.update();
  viewport.update(clock.getFps(), 
									static_cast<Player*>(player)->getBulletListSize(), 
									static_cast<Player*>(player)->getFreeListSize()); 
	// always update viewport last
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
	//sound.toggleMusic();
	//start with menu
	clock.pause();
	godMode = menuEngine.play();
	clock.unpause();

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
				if(keystate[SDL_SCANCODE_SPACE]) {
					static_cast<Player*>(player)->shoot();
					sound[0];
				}
        if ( keystate[SDL_SCANCODE_P] || keystate[SDL_SCANCODE_F1]) {
          if ( clock.isPaused() ) {
						clock.unpause();
					}
          else {
						clock.pause();
						viewport.drawHUD();
						SDL_RenderPresent(renderer);
					}
        }
				if(keystate[SDL_SCANCODE_R]) {
					clock.unpause();
					return true;
				}
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
			
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
			if(keystate[SDL_SCANCODE_A]){
				static_cast<Player*>(player)->left();
			}
			if(keystate[SDL_SCANCODE_D]){
				static_cast<Player*>(player)->right();
			}
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
	return false;
}
