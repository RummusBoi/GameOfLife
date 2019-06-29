#include<stdio.h>
#include<iostream>
#include<vector>

#include "Cell.h"

Cell::Cell (int value, int x, int y) {
	(*this).value = value;
	this->x = x;
	this->y = y;
	this->added = false;
	neighbours = 0;
}

int Cell::getValue () {
    return value;
}

void Cell::setValue (int value) {
    this->value = value;
}

void Cell::updateValue() {
	this->value = newValue;
}

int Cell::getNeighbours(bool resetNeighbours) {
	if (resetNeighbours) {
		int n = neighbours;
		neighbours = 0;
		return n;
	}
	return neighbours;
}

void Cell::setNeighbours(int n) {
	this->neighbours = n;
}

int Cell::getX() {
	return x;
}

int Cell::getY() {
	return y;
}

bool Cell::isAdded() {
	return added;
}

void Cell::setAdded(bool added) {
	this->added = added;
}

