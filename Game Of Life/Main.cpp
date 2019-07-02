#include <stdio.h>
#include <iostream>
#include <vector>
#include <chrono>

#ifdef APPLE
#include <SDL/sdl.h>
#endif

#include "Cell.h"
#include "GameOfLife.h"
#include "/Users/ralle/Desktop/Programs/C++ includes/EasyBMP_1/EasyBMP.cpp"

#undef main



int main () {
	int width = 2500;
	int height = 1500;
	int timeSteps = 5000;
	int cellSize = 1;
	int targetFPS = 60;
	
	bool useGPU = true;

	std::cout << "starting program.." << std::endl;
    
//    SDL_Window *window = SDL_CreateWindow("myWindow", 100, 100, width , height, SDL_WINDOW_SHOWN);
//
//    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
//
//    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//    SDL_RenderClear(renderer);
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    SDL_RenderDrawPoint(renderer, 50, 50);
//    SDL_RenderPresent(renderer);
//    SDL_Delay(5000);
	
	std::cout << "starting" << std::endl;
	
    std::vector<int> ruleSet;
    for (int i = 0; i < 9; i++) ruleSet.push_back (0);
	
	auto start = std::chrono::system_clock::now();
	
	if(useGPU) {
		GameOfLife game = GameOfLife(width / cellSize, height / cellSize, timeSteps, cellSize, targetFPS);
		game.runGameGPU();
	} else GameOfLife game = GameOfLife (width / cellSize, height / cellSize, timeSteps, ruleSet, cellSize, targetFPS, false, true);
		
	
	
	auto end = std::chrono::system_clock::now();
	
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	
	std::cout << "Elapsed time: " << elapsed.count () << std::endl;
	
	std::cout << "Average frames per second: " << timeSteps / (double)elapsed.count() << std::endl;

	SDL_Delay(1000);

	return 0;
}
