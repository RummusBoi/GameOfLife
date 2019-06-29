#pragma once
#include<stdio.h>
#include<iostream>
#include<vector>

class Cell {
public: 
    Cell (int value, int x, int y);
    int getValue ();
	void updateValue ();

    void setValue (int newValue);
	int getNeighbours (bool resetNeighbours);
	void setNeighbours (int n);
	bool isAdded ();
	void setAdded (bool added);
	
	
	int getX ();
	int getY ();
	
private:
    int value;
	int newValue;
	int neighbours;
	bool added;
	int x;
	int y;
};
