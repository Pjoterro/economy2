/*
 * dice.cpp
 *
 *  Created on: 14 cze 2022
 *      Author: pjoter
 */

#include <stdlib.h>
#include <cstdio>
#include "banking/dice.h"

dice::dice(int size) :
	size{size}
{}

int dice::roll() {
	return ((rand() % this->size) + 1);
}

dice::~dice() {}

