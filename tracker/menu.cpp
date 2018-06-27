#include<sstream>
#include "menu.h"

Menu::Menu(SDL_Renderer* rend) :  
  renderer(rend), 
  gdata(Gamedata::getInstance()), 
  hudFrame( {gdata.getXmlInt("menu/loc/x"),
             gdata.getXmlInt("menu/loc/y"),
             gdata.getXmlInt("menu/width"),
             gdata.getXmlInt("menu/height")}
  ),
  backColor({static_cast<Uint8>(gdata.getXmlInt("menu/backColor/r")),
             static_cast<Uint8>(gdata.getXmlInt("menu/backColor/g")),
             static_cast<Uint8>(gdata.getXmlInt("menu/backColor/b")),
             static_cast<Uint8>(gdata.getXmlInt("menu/backColor/a"))}
  ),
  menuColor({static_cast<Uint8>(gdata.getXmlInt("menu/color/r")),
             static_cast<Uint8>(gdata.getXmlInt("menu/color/g")),
             static_cast<Uint8>(gdata.getXmlInt("menu/color/b")),
             static_cast<Uint8>(gdata.getXmlInt("menu/color/a"))}
  ),
  clock( Clock::getInstance() ),
  io( IoMod::getInstance() ),
  options(),
  optionLoc( { gdata.getXmlInt("menu/optionLoc/x"),
               gdata.getXmlInt("menu/optionLoc/y")}
           ),
  clicks( {Sprite("clickOff"), Sprite("clickOn")} ),
  currentClick(0),
  currentOption(0),
  spaces(gdata.getXmlInt("menu/spaces")),
  startClickX(optionLoc[0]-spaces),
  startClickY(optionLoc[1]+spaces),
  clickX(startClickX),
  clickY(startClickY),
	controlsOn(false)
{
  int noOfOptions = gdata.getXmlInt("menu/noOfOptions");
  std::stringstream strm;
  for (int i = 0; i < noOfOptions; ++i) {
    strm.clear();
    strm.str("");
    strm << i;
    std::string option("menu/option"+strm.str());
    options.push_back(gdata.getXmlStr(option));
  }
}

void Menu::incrIcon() { 
/*
  clickY += spaces; 
  if ( clickY > static_cast<int>(options.size())*spaces+optionLoc[1]) {
    clickY = startClickY;
    currentOption = 0;
  }
  else ++currentOption;
*/
	currentOption++;
	if (currentOption > (int)options.size()-1) currentOption = 0;
	clickY = (currentOption+1)*spaces + optionLoc[1];
}

void Menu::decrIcon() { 
/*
  clickY -= spaces; 
  if ( clickY < spaces+optionLoc[1]) {
    clickY = startClickY+spaces*(options.size()-1);
    currentOption = options.size()-1;
  }
  else --currentOption;
*/
	currentOption--;
	if(currentOption < 0) currentOption = (int)options.size()-1;
	clickY = (currentOption+1)*spaces + optionLoc[1];
}

void Menu::drawBackground() const {
  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the hud background color:
  SDL_SetRenderDrawColor( renderer, backColor.r, backColor.g, 
     backColor.b, backColor.a );

  // Draw the filled rectangle:
  SDL_RenderFillRect( renderer, &hudFrame );

  // Set the  color for the Menu outline:
  SDL_Rect menuFrame = {hudFrame.x+50, hudFrame.y+40,
                        hudFrame.w-100, hudFrame.h-100};
  SDL_SetRenderDrawColor( renderer, menuColor.r, 
                          menuColor.g, menuColor.b, menuColor.a );
  SDL_RenderFillRect( renderer, &menuFrame );

}

void Menu::draw() const {
  drawBackground();
  io.writeText("Start Menu", hudFrame.x+350, hudFrame.y+60);
  int space = spaces;
  for ( const std::string& option : options ) {
    io.writeText(option, optionLoc[0], optionLoc[1]+space);
    space += spaces;
  }
  // We have to draw the clickOn & clickOff relative to the screen,
  // and we don't want to offset by the location of the viewprot:
  clicks[currentClick].getImage()->draw(0, 0, clickX, clickY);

	if(controlsOn){
		IoMod::getInstance().writeText("LEFT: A,  RIGHT: D", (hudFrame.w/2)-100, hudFrame.h-200, {255,255,255,255});
		IoMod::getInstance().writeText("SHOOT: SPACE", (hudFrame.w/2)-100, hudFrame.h-170, {255,255,255,255});
		IoMod::getInstance().writeText("PAUSE: P", (hudFrame.w/2)-100, hudFrame.h-140, {255,255,255,255});
		IoMod::getInstance().writeText("IN-GAME CONTROLS: F1", (hudFrame.w/2)-100, hudFrame.h-110, {255,255,255,255});
	}
	IoMod::getInstance().writeText("Nicholas Luce", hudFrame.x+10, hudFrame.h-30, {255,255,255,255});
}

