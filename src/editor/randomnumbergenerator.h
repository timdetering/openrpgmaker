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

#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

//This is the Linear Congruential Generator algorithm
//Formula: X[n+1] = (a * x[n] + c) & m

class RandomNumberGenerator
{
	public:
		void seedGenerator(unsigned long seed);
		unsigned long getRandomNumber();
		unsigned long getRandomNumber(unsigned long min, unsigned long max);
		static const unsigned long RandMax;
	private:
		unsigned long currentState;
		static const unsigned long m;
		static const unsigned long a;
		static const unsigned long c;
};

#endif // RANDOMNUMBERGENERATOR_H
