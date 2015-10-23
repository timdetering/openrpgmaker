/****************************************************************************
 *  Open RPG Maker is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  Open RPG Maker is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with Open RPG Maker. If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 *  ---                                                                     *
 *  Copyright (C) 2010, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <cstdio>
#include "randomnumbergenerator.h"

//The values for a, c, and m are the same ones used in glibc
const unsigned long RandomNumberGenerator::RandMax = 4294967294ul;
const unsigned long RandomNumberGenerator::m = 2147483647ul; // m
const unsigned long RandomNumberGenerator::a = 1103515245ul; // a
const unsigned long RandomNumberGenerator::c = 12345ul; // c

void RandomNumberGenerator::seedGenerator(unsigned long seed)
{
	currentState = seed;
}

unsigned long RandomNumberGenerator::getRandomNumber()
{
	currentState = ((a * currentState) + c) & m;
	return currentState;
}

unsigned long RandomNumberGenerator::getRandomNumber(unsigned long min, unsigned long max)
{
	return (getRandomNumber() % (max - min + 1ul)) + min;
}
