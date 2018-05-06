/*
 * Table.cpp
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#include "Table.h"

Table::Table() {
	this->numberOfSeats=0;
}

int Table::getNumberOfSeats() const {
	return numberOfSeats;
}

void Table::setNumberOfSeats(int numberOfSeats) {
	this->numberOfSeats = numberOfSeats;
}

Table::~Table() {
}

int Table::getOccupiedSeats() {
	return this->occupiedSeats;
}

void Table::incOccupiedSeats(int n) {
	this->occupiedSeats += n;
}
bool Table::isFull() {
	return this->occupiedSeats > this->numberOfSeats;
}

void Table::resetOccupiedSeats() {
	this->occupiedSeats = 0;
}

