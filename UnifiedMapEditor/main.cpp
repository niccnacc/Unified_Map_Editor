
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////



// TODO: move View incl. move, hidemenu, mousepos for spacemove



////////////////////////////////////////////////////////////

// #include <SFML/Audio.hpp>
// #include <SFML/Graphics.hpp>



#include "App.h"



int main(int, char const**)
{
	
	ume::App* app = new ume::App();
	if(NULL == app)
	{
		printf("main() Can't create App");
		EXIT_FAILURE;
	}

	int exitCode = app->run();
	
	delete app;
	app = NULL;
	
    return exitCode;
}





/*	// Load a music to play
    sf::Music music;
    if (!music.openFromFile(resourcePath() + "nice_music.ogg")) {return EXIT_FAILURE;}
    //music.play();
*/

	







// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your App bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// App bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//