//
//  Stopwatch.hpp
//  Game Of Life
//
//  Created by Rasmus Hyldgård Samsing on 18/10/2018.
//  Copyright © 2018 Rasmus Hyldgård Samsing. All rights reserved.
//

#ifndef Stopwatch_hpp
#define Stopwatch_hpp

#include <stdio.h>
#include <iostream>
#include <chrono>

class Stopwatch{
public:
	Stopwatch ();
	void start ();
	void stop ();
	void reset ();
	void changeTime (double dt);
	double getTime ();
	
private:
	double time;
	bool isRunning;
	std::chrono::time_point<std::chrono::high_resolution_clock> startingTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endingTime;
};

#endif /* Stopwatch_hpp */
