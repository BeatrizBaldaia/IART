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
	//TODO: destructor stub
}

