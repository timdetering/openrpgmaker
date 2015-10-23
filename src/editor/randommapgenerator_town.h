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

#ifndef RANDOMMAPGENERATOR_TOWN_H
#define RANDOMMAPGENERATOR_TOWN_H

#include "ui_randommapgenerator_town.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "xmlParser.h"

class RandomMapGenerator_Town : public QDialog, public Ui::RandomMapGenerator_Town
{
	Q_OBJECT
	public:
		RandomMapGenerator_Town(QWidget *parent=0);
		~RandomMapGenerator_Town();
		void setupGenerator(XMLNode ConfigurationNode);
		XMLNode getConfigurationNode();
		bool isConfigured();
		QStringList getGeneratedMapNames();
		Map *getGeneratedMap(int mapNum);
		void generateTown(unsigned long seed);
};

#endif // RANDOMMAPGENERATOR_TOWN_H
