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
public:
	Table();
	int getNumberOfSeats() const;
	void setNumberOfSeats(int numberOfSeats);
};

#endif /* SRC_TABLE_H_ */