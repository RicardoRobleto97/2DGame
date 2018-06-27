// Originally written by Brian Malloy        Data-Driven Object oriented Game Construction
//Adapted by Nicholas Luce
#include "engine.h"

RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
	bool play = true;
	try {
		while( play ){
			Engine* engine = new Engine;
			play = engine->play();
			delete engine;
		}
		delete RenderContext::getInstance();
	}
	catch (const string& msg) { std::cout << msg << std::endl; }
	catch (...) {
		std::cout << "Oops, someone threw an exception!" << std::endl;
	}
	return 0;
}
