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

#ifndef RANDOMMAPGENERATOR_CAVERNSTRUCTURE_H
#define RANDOMMAPGENERATOR_CAVERNSTRUCTURE_H

#include "ui_randommapgenerator_cavernstructure.h"
#include "loading.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "xmlParser.h"

class RandomMapGenerator_CavernStructure : public QDialog, public Ui::RandomMapGenerator_CavernStructure
{
	Q_OBJECT
	public:
		RandomMapGenerator_CavernStructure(QWidget *parent=0);
		~RandomMapGenerator_CavernStructure();
		void setupGenerator(XMLNode ConfigurationNode);
		XMLNode getConfigurationNode();
		bool isConfigured();
		QStringList getGeneratedMapNames();
		Map *getGeneratedMap(int mapNum);
		void generateCavernStructure(unsigned long seed);
	protected slots:
		void on_cbUseLowerWall_toggled(bool on);
		void on_cbUseUpperWall_toggled(bool on);
		void on_cbUseFloorB_toggled(bool on);
		void on_cbUseFloorC_toggled(bool on);
		void on_buttonBox_helpRequested();
		void on_buttonBox_accepted();
	private:
		enum Space
		{
			SPACE_FLOOR=0,
			SPACE_CEILING,
			SPACE_UPPERWALL,
			SPACE_LOWERWALL,
			SPACE_FLOORA,
			SPACE_FLOORB,
			SPACE_FLOORC,
			SPACE_UNUSED
		};
		int numNeighbors(int x, int y, int type);
		void init();
		void runSprout();
		void runGrowth();
		void runSprout2();
		void runGrowth2();
		void placeWalls();
		void copyMapData();
		bool phase1Done();
		bool phase2Done();
		RandomNumberGenerator numberGenerator;
		double placeFloor;
		double placeCeiling;
		double sproutChances[2];
		double growChances[2];
		unsigned long sproutChanceFloor;
		unsigned long sproutChanceCeiling;
		unsigned long growChanceFloor;
		unsigned long growChanceCeiling;
		unsigned long randomNumber;
		int mapWidth;
		int mapHeight;
		int *mapData1;
		int *mapData2;
		bool configured;
};

#endif // RANDOMMAPGENERATOR_CAVERNSTRUCTURE_H
