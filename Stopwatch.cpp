//
//  Stopwatch.cpp
//  Game Of Life
//
//  Created by Rasmus Hyldgård Samsing on 18/10/2018.
//  Copyright © 2018 Rasmus Hyldgård Samsing. All rights reserved.
//

#include "Stopwatch.h"

Stopwatch::Stopwatch () {
	time = 0;
	startingTime = std::chrono::high_resolution_clock::now();
	endingTime = std::chrono::high_resolution_clock::now();
	isRunning = false;
}

void Stopwatch::start() {
	if (!isRunning) {
		startingTime = std::chrono::high_resolution_clock::now();
		isRunning = true;
		
	}
	else std::cout << "Error starting stopwatch. " << std::endl;
}

void Stopwatch::stop () {
	if (isRunning) {
		endingTime = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds> (endingTime - startingTime);
		time += elapsed.count ();
		isRunning = false;
	}
}

void Stopwatch::reset() {
	time = 0;
}

double Stopwatch::getTime () {
	return time;
}

void Stopwatch::changeTime (double dt) {
	time += dt;
}
