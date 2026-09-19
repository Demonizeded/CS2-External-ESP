#include "Application.hpp"
#define _CRTDBG_MAP_ALLOC
#include <thread>
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	core::Engine eng;
	if (!eng.attach()){
		std::cout << "Attaching failed!";
		return false;
	}
	if (!eng.initServices()){
		std::cout << "Initializing Services failed!";
		return false;
	}
	eng.handleMain();
}
