#include <vector>
#include <SDL.h>
#include "menu.h"

class MenuEngine {
public:
  MenuEngine ();
  ~MenuEngine ();
  bool play();
  int getOptionChoice() const { return optionChoice; }
	void redrawBack();

private:
  Clock& clock;
  SDL_Renderer * const renderer;
  Menu menu;
  int optionChoice;

  void draw() const;
  void update(Uint32);

  MenuEngine(const MenuEngine&);
  MenuEngine& operator=(const MenuEngine&);
};
