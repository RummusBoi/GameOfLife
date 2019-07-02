#pragma once
#include<stdio.h>
#include <vector>
#include <string.h>
#include "Cell.h"
#include <SDL2/SDL.h>
#include "Stopwatch.h"
#include<OpenCL/cl.h>

class GameOfLife {
public:
	GameOfLife (int width, int height, int timeSteps, std::vector<int> ruleSet, int cellSize, int targetFPS, bool drawGrid, bool drawToWindow);
	GameOfLife (int width, int height, int timeSteps, int cellSize, int targetFPS);
    void setValue (int x, int y, int newValue);
    Cell* getCell (int x, int y);
	void runGame ();
	void runGameGPU ();
private:
	SDL_Window *window;
	SDL_Rect windowRect;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	Uint32* pixels;
	
	cl_mem cells_mem_obj;
	cl_mem cells_result_mem_obj;
	cl_mem width_mem_obj;
	cl_mem height_mem_obj;
	cl_mem genInfo;
	cl_mem fpsInfo;
	cl_kernel kernel;
	cl_command_queue command_queue;
	cl_program program;
	cl_context context;
	
    int width, height, cellSize, targetFPS;
	int timeSteps;
	bool drawGrid;
	bool drawToWindow;
	
	void updateCells ();
	void drawCells ();
	void updateUpdateCells ();
	void sleep (int msToSleep);
	void initiateKernel ();
	void runKernel (float* input, float* output, bool allocateMemory, int gen_info, cl_ulong* kernelTime, int gensPerFrame);
	void drawCellsGpu (float* cells_array);
	void check_Error (cl_int result, std::string operation);
	void handleEvents (int* gensPerFrame);
	
	const char *getErrorString(cl_int error);
	
    std::vector<int> ruleSet;
    std::vector <std::vector <Cell> > cells;
	std::vector<Cell*> cellsToUpdate;
	std::vector<Cell*> cellsAlive;
	
	Stopwatch writingMemory;
	
	Uint32 timeLeft (Uint32 nextTime);
};
