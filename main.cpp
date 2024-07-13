#include <res/Cfg.hpp>
#include <app/Game.hpp>




int main()
{
	Cfg::Initialize();

	Game demo;

	demo.Run();

	return 0;
}