#include <vector>
#include <SDL.h>
#include "renderContext.h"
#include "world.h"
#include "viewport.h"
#include "menuEngine.h"
#include "sound.h"

class CollisionStrategy;
class SmartSprite;
class Player;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

	Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World world;
	World stars;
  Viewport& viewport;

	MenuEngine menuEngine;
	SDLSound sound;

	std::vector<Drawable *> sprites;
	Drawable *player;
  
	CollisionStrategy * strategy;
	bool collision;
	bool godMode;

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
