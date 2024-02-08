#include "App.h"
//https://lazyfoo.net/tutorials/SDL/02_getting_an_image_on_the_screen/index.php
//http://www.sdltutorials.com/
//https://epub.uni-regensburg.de/17610/
//https://wiki.libsdl.org/SDL2/FrontPage

int main(int argc, char** arcv)
{
#ifdef _DEBUG
	App app;
	if (app.Init())
		app.Run();
	return 0;
#else
	try
	{
		App app;
		if (app.Init())
			app.Run();
		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << "catched " << e.what() << std::endl;
		for (std::string& l : loger)
		{
			std::cout << l << '\n';
		}
		return -1;
	}
#endif // !_DEBUG


	
}