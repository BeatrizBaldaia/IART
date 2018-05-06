/*
 * Table.h
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#ifndef SRC_TABLE_H_
#define SRC_TABLE_H_

class Table {
private:
	int numberOfSeats;
	int occupiedSeats = 0;
public:
	Table();
	virtual ~Table();
	int getNumberOfSeats() const;
	void setNumberOfSeats(int numberOfSeats);
	int getOccupiedSeats();
	void incOccupiedSeats(int n);
	bool isFull();
	void resetOccupiedSeats();
};

#endif /* SRC_TABLE_H_ */
