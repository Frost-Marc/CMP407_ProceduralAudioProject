#include <SFML/System.hpp>
#include "FireSoundGen.h"
#include <iostream>

int main()
{
    FireSoundGen fire;

    fire.setIntensity(0.8f);
    fire.setCrackleRate(25.0f);
    fire.setBrightness(0.6f);
    fire.setFlickerSpeed(0.4f);

    fire.play();

    std::cout << "Procedural Fire Running...\n";
    std::cout << "Press Enter to quit.\n";
    std::cin.get();

	return 0;
}