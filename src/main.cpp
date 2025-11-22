#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include "Engine.hpp"

/*
* MVP = 3 steps
*	1. Load Object
*	2. Render Object
*	3. Move Around with Camera to see
* 
* 
* TODO:
*	swap the z and y coord since blender has z as up (cringe)
*		- add documentation (docs/ dir)
*		- cmake setup and move proj away from visual studio
*		- github README overview
*			- switch from static to dependency injection
*/

int main(int argc, char** argv) {

	try {
		int density = 0;
		if (argc < 2) {
			throw std::runtime_error("Need to pass a object file");
		}  
		
		if(argc == 3) {
			// density is included
			density = strtol(argv[2], NULL, 10);
		}
		
		pcr::Engine engine{argv[1], density};
		engine.Show();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << "\nEnter any character to end the program";
		
		std::cin.get();
		return EXIT_FAILURE;
	}
	catch (...) {
		std::cerr << "Unknown failure occured\nEnter any Character to end the program";

		std::cin.get();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}