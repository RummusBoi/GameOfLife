#include<stdio.h>
#include<vector>
#include<iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <chrono>
#include <cmath>
#include <SDL2/SDL.h>
#include <thread>
#include "GameOfLife.h"
#include <png.h>

#define MAX_SOURCE_SIZE (0x100000)

using namespace std;

GameOfLife::GameOfLife (int width, int height, int timeSteps, std::vector<int> ruleSet, int cellSize, int targetFPS, bool drawGrid, bool drawToWindow) {
	if (drawToWindow) {
		
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
		window = SDL_CreateWindow("window", 200, 200, width * cellSize / 2, height * cellSize / 2, SDL_WINDOW_ALLOW_HIGHDPI);
		renderer = SDL_CreateRenderer(window, -1 , 0);
	}
//	SDL_Window *newwindow = SDL_CreateWindow("window2", 400, 400, 400, 400, SDL_WINDOW_SHOWN);
	
    this->ruleSet = ruleSet;
	this->timeSteps = timeSteps;
	srand((int)time(NULL));
    for (int j = 0; j < width; j++) {
        cells.push_back (std::vector<Cell> ());
        for (int i = 0; i < height; i++) {
			cells[j].push_back (Cell (0, j, i));
        }
    }
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			if (rand() % 100 > 70) {
				std::cout << "Setting cell " << j << " : " << i << ", value: " << cells[j][i].getValue() <<std::endl;
				cells[j][i].setValue(1);
				cellsAlive.push_back(&(cells[j][i]));
			}
		}
	}
//	cells[width / 2][height / 2].setValue(1);
//	cellsAlive.push_back(&cells[width / 2][height / 2]);
//
//	cells[width / 2 + 2][height / 2].setValue(1);
//	cellsAlive.push_back(&cells[width / 2 + 2][height / 2]);
//
//	cells[width / 2 + 1][height / 2 - 2].setValue(1);
//	cellsAlive.push_back(&cells[width / 2 + 1][height / 2 - 2]);
//
//	cells[width / 2 + 1][height / 2 + 2].setValue(1);
//	cellsAlive.push_back(&cells[width / 2 + 1][height / 2 + 2]);
	
	this->width = width;
	this->height = height;
	this->cellSize = cellSize;
	this->drawGrid = drawGrid;
	this->drawToWindow = drawToWindow;
	this->targetFPS = targetFPS;
	
	bool done = false;
	bool mouseDown = false;
	int mouseX = 0, mouseY = 0;
	//read user input before starting the game
	if (drawToWindow) {
		drawCells();
		while (!done) {
			
			
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_MOUSEMOTION:
						mouseX = event.motion.x;
						mouseY = event.motion.y;
						
						cout << "MouseX: " << mouseX << ", MouseY: " << mouseY << ", Corresponding cell: " << floor ((double)mouseX / (double)cellSize) << " : " << floor ((double)mouseY / (double)cellSize) << endl;
						
						break;
						
					case SDL_MOUSEBUTTONDOWN:
						if (event.button.button == SDL_BUTTON_LEFT) mouseDown = true;
						if (event.button.button == SDL_BUTTON_RIGHT) done = true;
						break;
						
					case SDL_MOUSEBUTTONUP:
						if (event.button.button == SDL_BUTTON_LEFT) mouseDown = false;
						break;
						
				
					default:
						break;
				
				}
			}
			
			if (mouseDown) {
				int x = floor ((double)mouseX / (double)cellSize);
				int y = floor ((double)mouseY / (double)cellSize);
				
				if (cells[x][y].getValue() == 0) {
					cells[x][y].setValue(1);
					cellsAlive.push_back(&cells[x][y]);
				}
				cout << "Placed cell at " << x << " : " << y << endl;
				drawCells();
			}
		}
	}
	
	
	runGame();
}

GameOfLife::GameOfLife (int width, int height, int timeSteps, int cellSize, int targetFPS) {
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	window = SDL_CreateWindow("window", 200, 200, width * cellSize / 2, height * cellSize / 2, SDL_WINDOW_ALLOW_HIGHDPI);
	renderer = SDL_CreateRenderer(window, -1 , 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
	pixels = new Uint32[width * height];
	
	SDL_RenderSetScale(renderer, cellSize, cellSize);
	windowRect.w = width;
	windowRect.h = height;
	windowRect.x = 0;
	windowRect.y = 0;
	
	this -> width = width;
	this -> height = height;
	this -> cellSize = cellSize;
	this -> timeSteps = timeSteps;
	this -> targetFPS = targetFPS;
	
	//this constructor runs the code on the gpu
	
	//generate random starting data:
	
	//ready the kernel for use:
}

Cell* GameOfLife::getCell (int x, int y) {
    return &cells[x][y];
}

void GameOfLife::runGame() {
	if (drawToWindow) {
		drawCells();
	}

	
	
	//Measure time of different parts of code:
	Stopwatch updatingUpdate = Stopwatch();
	Stopwatch updating = Stopwatch();
	Stopwatch drawing = Stopwatch();
	
	int counter = 0;
	Uint32 nextTime = SDL_GetTicks() + 1000 / targetFPS;
	
	for (int step = 0; step < timeSteps; step ++) {
		auto startTime = std::chrono::system_clock::now();
		
		
		updatingUpdate.start ();
		updateUpdateCells();
		updatingUpdate.stop ();
		
		updating.start ();
		updateCells();
		updating.stop ();
		
		
		if (drawToWindow && counter == 1) {
			sleep (timeLeft(nextTime));
			nextTime += targetFPS;
			drawing.start ();
			drawCells ();
			drawing.stop ();
			counter = 0;
		}
		counter ++;
		
		
		
		
		auto endTime = std::chrono::system_clock::now();
		
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		
		cout << "Frame " << step << " FPS: " << 1000. / elapsed.count () << " ; Cells alive: " << cellsAlive.size() << std::endl;
		
	}
	
	cout << "Time spent on updatingUpdate: " << (double)updatingUpdate.getTime() / 1000. << " s. " << endl;
	cout << "Time spent on updating: " << (double)updating.getTime()/1000. << " s. " << endl;
	cout << "Time spent on drawing: " << (double)drawing.getTime()/1000. << " s." << endl;
	
}

void GameOfLife::updateUpdateCells() {
	cellsToUpdate.clear();
	for (Cell *cell : cellsAlive) {
		if (!cell->isAdded()) {
			cellsToUpdate.push_back(cell);
			cell->setAdded(true);
		}
		
		for (int x = cell->getX () - 1; x <= cell->getX () + 1; x++) {
			for (int y = cell->getY () - 1; y <= cell->getY () + 1; y++) {
				if (0 <= x && x < width && 0 <= y && y < height && !(cell->getX() == x && cell->getY() == y)) {
					
					getCell(x, y)->setNeighbours(1 + getCell(x, y)->getNeighbours(false));
					if (getCell(x, y)->getValue() == 1) {
					}
					if (!getCell(x, y)->isAdded()) {
						cellsToUpdate.push_back(getCell(x, y));
						getCell(x, y)->setAdded(true);
					}
				}
			}
		}
	}
}

void GameOfLife::updateCells() {
	cellsAlive.clear();
	for (Cell *cell : cellsToUpdate) {
		//for now, the ruleSet is ignored.
		
		if ((cell->getValue() == 1 && (cell->getNeighbours(false) == 2 || cell->getNeighbours(false) == 3)) || (cell->getValue() == 0 &&
																											   (cell->getNeighbours(false) == 3 ||
																												cell->getNeighbours(false) == 3))) {
			cellsAlive.push_back(cell);
			cell->setValue(1);
		}
		else {
			cell->setValue(0);
		}
		
		cell->setNeighbours(0);
		cell->setAdded(false);
	}
}

void GameOfLife::drawCells () {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	
	if (drawGrid) {
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_Rect rect;
		
		rect.w = cellSize - 2;
		rect.h = cellSize - 2;
		
		for (int x = 0; x < width * cellSize; x+= cellSize) {
			for (int y = 0; y < height * cellSize; y+= cellSize) {
				rect.x = x + 1;
				rect.y = y + 1;
				
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
	
	SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
	
	for (Cell *cell : cellsAlive) {
		if (true){
			SDL_Rect rect;
			rect.x = cell->getX() * cellSize;
			rect.y = cell->getY() * cellSize;
			rect.w = cellSize;
			rect.h = cellSize;
			
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}

Uint32 toUint32 (float input) {
	return Uint32(input + 2147483646);
}

void GameOfLife::sleep (int msToSleep) {
	for(int i = 0; i < msToSleep; i++){
//		SDL_PumpEvents();
		SDL_Delay(1);
	}
}

void abort_(const char * s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

int x, y;

int imgwidth, imgheight;
png_byte color_type;
png_byte bit_depth;
int bytes_per_pixel;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void read_png_file(char* file_name)
{
	char header[8];    // 8 is the maximum size that can be checked
	
	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
		abort_("[read_png_file] File %s could not be opened for reading", file_name);
	fread(header, 1, 8, fp);
	
	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr)
		abort_("[read_png_file] png_create_read_struct failed");
	
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[read_png_file] png_create_info_struct failed");
	
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during init_io");
	
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	
	png_read_info(png_ptr, info_ptr);
	
	imgwidth = png_get_image_width(png_ptr, info_ptr);
	imgheight = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	bytes_per_pixel = png_get_rowbytes(png_ptr, info_ptr) / imgwidth;
	
	
	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	cout << number_of_passes << endl;
	
	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during read_image");
	
	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * imgheight);
	for (y=0; y<imgheight; y++)
		row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
	
	png_read_image(png_ptr, row_pointers);
	
	fclose(fp);
}

void GameOfLife::runGameGPU() {
	char filename[255] = "/Users/ralle/Desktop/Programs/C++/GameOfLife_XCode/Game Of Life/Game Of Life/colors6.png";
	read_png_file(filename);
	
	int gensPerFrame = 1;
	
	float* cells_array;
	cells_array = new float [width * height];
	srand((int)time(NULL));
	for (int i = 0; i < width * height; i++) {
		int ran = rand() % 100;
		cells_array[i] = ran >= 95 ? rand() % 10 : 0;
//		cells_array[i] = rand() % 100 > (((double)(i % width)) / (double)width) * 100 / 8 + 80? 1 : 0;
//		cells_array[i] = 0;
	}
	drawCellsGpu(cells_array);
	handleEvents(&gensPerFrame);
	cout << "Amount of cells: " << width * height << endl;
	
	cells_array[width * height / 2 + width / 2] = 10;
	cells_array[width * height / 2 + width / 2 + width*2 + 1] = 10;
	
//	cells_array[width * height / 2 + width / 2 + 2] = 1;
//	cells_array[width * height / 2 + width / 2 + 2*width + 2] = 1;
	
//	cells_array[width * height / 2 + width / 2 + 20 - width * 20] = 1;
	
	Uint32 lastFrame = SDL_GetTicks();
	
	cout << "Initialising kernel..." << endl;
	initiateKernel();
	cout << "Done." << endl;
	
	float* nextGen;
	nextGen = new float [width * height];
	
	
	cout << "Starting game loop..." << endl;
	
	float* cellsptr = &cells_array[0], *nextGenptr = &nextGen[0];
	drawCellsGpu(cells_array);
	
	Uint32 nextTime = SDL_GetTicks() + targetFPS;
	
	Stopwatch drawing = Stopwatch();
	Stopwatch updatingCells = Stopwatch();
	Stopwatch sleeping = Stopwatch();
	
	
	
	writingMemory = Stopwatch();
	
	cl_ulong kernelTime = 0;
	
	
	
	
	runKernel(cellsptr, nextGenptr, true, 3, &kernelTime, gensPerFrame);
	
	
	vector<thread> drawingThread;
	drawingThread.push_back (thread(&GameOfLife::drawCellsGpu, this, cellsptr));
	
	cout << "Starting gen 1. " << endl;
	
	for (int step = 0; step < timeSteps / gensPerFrame; step ++) {
		
		updatingCells.start();
		
		handleEvents (&gensPerFrame);
		
		runKernel(cellsptr, nextGenptr, false, (step) % 2, &kernelTime, gensPerFrame);
		
		cout << "Change: " << nextGenptr[1000] - cellsptr[1000] << endl;
		
		float* tmpptr = cellsptr;
		cellsptr = nextGenptr;
		nextGenptr = tmpptr;
		updatingCells.stop();
			
		drawing.start();
		drawingThread[0].join ();
		
		drawingThread.clear();
		
		drawingThread.push_back(thread(&GameOfLife::drawCellsGpu, this, cellsptr));
		
		
		
		drawing.stop ();
	
		sleeping.start();
		sleep(timeLeft(nextTime));
		sleeping.stop();
		
		nextTime += 1000 / targetFPS;
		
//		cout << "Generation " << step * gensPerFrame << "; FPS = " << 1000 / (SDL_GetTicks() - lastFrame) << endl;
		lastFrame = SDL_GetTicks();
		
	}
	
	drawingThread[0].join ();
	
	cout << "Time spent drawing: " << drawing.getTime() << endl;
	cout << "Time spent updating cells: " << updatingCells.getTime() << endl;
	cout << "Time spent on kernel computation: " << kernelTime << endl;
	cout << "Time spent sleeping: " << sleeping.getTime() << endl;
	cout << "Time spent writing to memory: " << writingMemory.getTime() << endl;
}

void GameOfLife::initiateKernel() {
	FILE *fp;
	size_t source_size;
	
	char filePath[] = "/Users/ralle/Desktop/Programs/C++/GameOfLifeGit/Game Of Life/Game Of Life/gpuKernelProgram.cl";
	
	
	
	fp = fopen(filePath, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		SDL_Delay(2000);
		exit(1);
	}
	char* source_str = (char*)malloc(MAX_SOURCE_SIZE);;
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose( fp );
	
	// Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_GPU, 1,
						 &device_id, &ret_num_devices);
	check_Error(ret, "setting device ids");
	
	// Create an OpenCL context
	context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
	check_Error(ret, "creating context");
	
	// Create a command queue
	command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);
	check_Error(ret, "creating command queue");

	//create memory objects in gpu memory:
	
	cells_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, height * width * sizeof(float), NULL, &ret);
	check_Error(ret, "creating buffer 1");
	cells_result_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, height * width * sizeof(float), NULL, &ret);
	check_Error(ret, "creating buffer 2");
	width_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);
	check_Error(ret, "creating buffer 3");
	height_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);
	check_Error(ret, "creating buffer 4");
	genInfo = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);
	check_Error(ret, "creating buffer 5");
	fpsInfo = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &ret);
	check_Error(ret, "creating buffer 6");
	
	
	
	//create program from souce:
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
	check_Error(ret, "creating program");
	
	// Build the program
	
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	check_Error(ret, "building program");
	
	kernel = clCreateKernel(program, "calNextGen", &ret);
	
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cells_mem_obj);
	check_Error(ret, "allocating memory 1");
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&width_mem_obj);
	check_Error(ret, "allocating memory 2");
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&height_mem_obj);
	check_Error(ret, "allocating memory 3");
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&cells_result_mem_obj);
	check_Error(ret, "allocating memory 4");
	ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&genInfo);
	check_Error(ret, "allocating memory 5");
	ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&fpsInfo);
	check_Error(ret, "allocating memory 6");
}

void GameOfLife::runKernel(float* input, float* output, bool allocateMemory, int gen_info, cl_ulong* kernelTime, int gensPerFrame) {
	cl_int ret;
	if (gen_info == 3) {
		gen_info = 0;
		writingMemory.start();
		ret = clEnqueueWriteBuffer (command_queue, cells_mem_obj, CL_TRUE, 0, width * height * sizeof(int), input, 0, NULL, NULL);
		check_Error(ret, "writing buffer 1");
		ret = clEnqueueWriteBuffer(command_queue, cells_result_mem_obj, CL_TRUE, 0, width * height * sizeof(int), output, 0, NULL, NULL);
		check_Error(ret, "writing buffer 2");
		ret = clEnqueueWriteBuffer(command_queue, width_mem_obj, CL_TRUE, 0, sizeof(int), &width , 0, NULL, NULL);
		check_Error(ret, "writing buffer 3");
		ret = clEnqueueWriteBuffer(command_queue, height_mem_obj, CL_TRUE, 0, sizeof(int), &height , 0, NULL, NULL);
		check_Error(ret, "writing buffer 4");
		ret = clEnqueueWriteBuffer(command_queue, genInfo, CL_TRUE, 0, sizeof(int), &gen_info, 0, NULL, NULL);
		check_Error(ret, "writing buffer 5");
		ret = clEnqueueWriteBuffer(command_queue, fpsInfo, CL_TRUE, 0, sizeof(int), &gensPerFrame, 0, NULL, NULL);
		writingMemory.stop ();
	} else {
//		writingMemory.start();
//		ret = clEnqueueWriteBuffer(command_queue, genInfo, CL_TRUE, 0, sizeof(int), &gen_info, 0, NULL, NULL);
//		writingMemory.stop();
	}
	
	
//	cout << "Time to write to buffer: " << writingMemory.getTime() << endl;

	size_t global_item_size = width * height; // Process the entire lists
	size_t local_item_size = 256; // Process in groups of 64
	
	while (global_item_size % local_item_size != 0) {
		local_item_size /= 2;
		if (local_item_size == 1) {
			cout << "ERROR: global size is not divisible by work size. " << endl;
			SDL_Delay(2000);
			exit (1);
		}
	}
	cl_event event;
	
	
	cl_ulong time_start;
	cl_ulong time_end;
	
	for (int gen = 0;  gen < gensPerFrame; gen++) {
		
		int genData = (gen + (gensPerFrame % 2) * gen_info) % 2;
		
		writingMemory.start();
		ret = clEnqueueWriteBuffer(command_queue, genInfo, CL_TRUE, 0, sizeof(int), &genData, 0, NULL, NULL);
		check_Error(ret, "writing to buffer");
		
		
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
		check_Error(ret, "enqueuing kernel");
		clWaitForEvents(1, &event);
		clFinish(command_queue);
		
		writingMemory.stop();
		
		ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
		ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
		
		writingMemory.changeTime( - ((double)time_end - (double)time_start));
		
		*kernelTime += time_end - time_start;
	}
	
	
	
	
//	cout << "Time to complete computation: " << time_end << " - " << time_start << " = " << nanoSeconds << endl;
	
	check_Error(ret, "enqueuing kernel operation");
	
	ret = clEnqueueReadBuffer(command_queue, cells_result_mem_obj, CL_TRUE, 0, width * height * sizeof(int), output, 0, NULL, NULL);
	check_Error(ret, "reading buffer");
}

void GameOfLife::drawCellsGpu (float *cells_array) {
	SDL_RenderClear(renderer);
	float max = cells_array[0];
	for (int i = 0; i < width * height; i++) {		
		//set pixel value
		//pixels[i] = (Uint32)((double)red * redDelay) << 16 | (Uint32)((double)green * greenDelay) << 8 | (Uint32)((double)blue * blueDelay);
		int multiplier = 1;
		pixels[i] = (Uint32)(multiplier*cells_array[i]) << 16 | (Uint32)(multiplier*cells_array[i]) << 8 | (Uint32)(multiplier*cells_array[i]);
		if(cells_array[i] > max) max = cells_array[i];
	}
	cout << "Max: " << max << endl;
	SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * width);
	SDL_RenderCopy(renderer, texture, NULL, &windowRect);
	
	SDL_RenderPresent(renderer);
	
}

void GameOfLife::check_Error (cl_int result, string operation) {
	if(result != CL_SUCCESS) {
		cout << "Failed on " << " with error code " << endl;
		std::cerr << getErrorString(result) << std::endl;
		SDL_Delay(5000);
		exit (1);
	}
}

void GameOfLife::handleEvents(int* gensPerFrame) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				cout << "Detected keydown!" << endl;
				switch (event.key.keysym.sym) {
					case SDLK_RIGHT:
						cout << "Detected right" << endl;
						(*gensPerFrame) ++;
						
						break;
						
					case SDLK_LEFT:
						cout << "Detected left" << endl;
						if ((*gensPerFrame) > 1) {
							(*gensPerFrame) --;
						}
						
						break;
					case SDLK_SPACE:
						cout << "Ending simulation" << endl;
						SDL_Quit();
						exit (1);
					
					default:
						break;
				}
			default:
				break;
		}
	}
}

const char *GameOfLife::getErrorString(cl_int error)
{
	switch(error){
			// run-time and JIT compiler errors
		case 0: return "CL_SUCCESS";
		case -1: return "CL_DEVICE_NOT_FOUND";
		case -2: return "CL_DEVICE_NOT_AVAILABLE";
		case -3: return "CL_COMPILER_NOT_AVAILABLE";
		case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
		case -5: return "CL_OUT_OF_RESOURCES";
		case -6: return "CL_OUT_OF_HOST_MEMORY";
		case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
		case -8: return "CL_MEM_COPY_OVERLAP";
		case -9: return "CL_IMAGE_FORMAT_MISMATCH";
		case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
		case -11: return "CL_BUILD_PROGRAM_FAILURE";
		case -12: return "CL_MAP_FAILURE";
		case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
		case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
		case -15: return "CL_COMPILE_PROGRAM_FAILURE";
		case -16: return "CL_LINKER_NOT_AVAILABLE";
		case -17: return "CL_LINK_PROGRAM_FAILURE";
		case -18: return "CL_DEVICE_PARTITION_FAILED";
		case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
			
			// compile-time errors
		case -30: return "CL_INVALID_VALUE";
		case -31: return "CL_INVALID_DEVICE_TYPE";
		case -32: return "CL_INVALID_PLATFORM";
		case -33: return "CL_INVALID_DEVICE";
		case -34: return "CL_INVALID_CONTEXT";
		case -35: return "CL_INVALID_QUEUE_PROPERTIES";
		case -36: return "CL_INVALID_COMMAND_QUEUE";
		case -37: return "CL_INVALID_HOST_PTR";
		case -38: return "CL_INVALID_MEM_OBJECT";
		case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
		case -40: return "CL_INVALID_IMAGE_SIZE";
		case -41: return "CL_INVALID_SAMPLER";
		case -42: return "CL_INVALID_BINARY";
		case -43: return "CL_INVALID_BUILD_OPTIONS";
		case -44: return "CL_INVALID_PROGRAM";
		case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
		case -46: return "CL_INVALID_KERNEL_NAME";
		case -47: return "CL_INVALID_KERNEL_DEFINITION";
		case -48: return "CL_INVALID_KERNEL";
		case -49: return "CL_INVALID_ARG_INDEX";
		case -50: return "CL_INVALID_ARG_VALUE";
		case -51: return "CL_INVALID_ARG_SIZE";
		case -52: return "CL_INVALID_KERNEL_ARGS";
		case -53: return "CL_INVALID_WORK_DIMENSION";
		case -54: return "CL_INVALID_WORK_GROUP_SIZE";
		case -55: return "CL_INVALID_WORK_ITEM_SIZE";
		case -56: return "CL_INVALID_GLOBAL_OFFSET";
		case -57: return "CL_INVALID_EVENT_WAIT_LIST";
		case -58: return "CL_INVALID_EVENT";
		case -59: return "CL_INVALID_OPERATION";
		case -60: return "CL_INVALID_GL_OBJECT";
		case -61: return "CL_INVALID_BUFFER_SIZE";
		case -62: return "CL_INVALID_MIP_LEVEL";
		case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
		case -64: return "CL_INVALID_PROPERTY";
		case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
		case -66: return "CL_INVALID_COMPILER_OPTIONS";
		case -67: return "CL_INVALID_LINKER_OPTIONS";
		case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";
			
			// extension errors
		case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
		case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
		case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
		case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
		case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
		case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
		default: return "Unknown OpenCL error";
	}
}

Uint32 GameOfLife::timeLeft (Uint32 nextTime) {
	Uint32 now = SDL_GetTicks();
	if (nextTime <= now) {
		return 0;
	} else {
		return nextTime - now;
	}
}
