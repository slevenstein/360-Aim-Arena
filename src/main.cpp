// Support Code written by Michael D. Shah
// Last Updated: 6/11/21
// Please do not redistribute without asking permission.

// Functionality that we created
#include "SDLGraphicsProgram.hpp"

int main(int argc, char** argv){

	float sens = 0.002f;
	int screenWidth = 1920;
	int screenHeight = 1080;

	// possible parameters: none, sens, screen width + height, sens + screen width + height
	if(argc == 2 || argc == 4){
        sens = std::stof(argv[1]) / 1000;
    }
	if(argc == 3){
        screenWidth = std::stoi(argv[1]);
		screenHeight = std::stoi(argv[2]);
    }
	if(argc == 4){
        screenWidth = std::stoi(argv[2]);
		screenHeight = std::stoi(argv[3]);
    }

	// Create an instance of an object for a SDLGraphicsProgram
	SDLGraphicsProgram mySDLGraphicsProgram(sens,screenWidth,screenHeight);
	// Run our program forever
	mySDLGraphicsProgram.Loop();

	mySDLGraphicsProgram.~SDLGraphicsProgram();

	SDL_Delay(2000);


	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}
