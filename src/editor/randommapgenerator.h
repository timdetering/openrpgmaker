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

#ifndef RANDOMMAPGENERATOR_H
#define RANDOMMAPGENERATOR_H

#include <cstdio>
#include <cstdlib>
#include "ui_randommapgenerator.h"
#include "projectdata.h"
#include "randommapgenerator_building.h"
#include "randommapgenerator_cavernstructure.h"
#include "randommapgenerator_dungeonstructure.h"
#include "randommapgenerator_maze.h"
//#include "randommapgenerator_mountainstructure.h"
//#include "randommapgenerator_town.h"
#include "randommapgenerator_worldmap.h"
#include "randomnumbergenerator.h"

class RandomMapGenerator : public QDialog, public Ui::RandomMapGenerator
{
	Q_OBJECT
	public:
		RandomMapGenerator(QWidget *parent=0);
		~RandomMapGenerator();
		int numGeneratedMaps();
		Map *getGeneratedMap(int mapNum);
		enum MapType
		{
			TYPE_DUNGEONSTRUCTURE=0,
			TYPE_CAVERNSTRUCTURE=1,
			TYPE_MOUNTAINSTRUCTURE=2,
			TYPE_WORLDMAP=3,
			TYPE_TOWN=4,
			TYPE_BUILDING=5,
			TYPE_MAZE=6
		};
	protected slots:
		void on_cboxMapType_currentIndexChanged(int index);
		void on_tbConfigureGenerator_clicked();
		void on_tbGenerateMaps_clicked();
		void on_tbNewSeed_clicked();
		void on_cboxSelectMap_currentIndexChanged(int index);
		void on_tbZoom1to1_toggled(bool on);
		void on_tbZoom1to2_toggled(bool on);
		void on_tbZoom1to4_toggled(bool on);
		void on_tbZoom1to8_toggled(bool on);
		void on_buttonBox_helpRequested();
	private:
		RandomMapGenerator_Building *buildingGenerator;
		RandomMapGenerator_CavernStructure *cavernGenerator;
		RandomMapGenerator_DungeonStructure *dungeonGenerator;
		RandomMapGenerator_Maze *mazeGenerator;
		//RandomMapGenerator_MountainStructure *mountainGenerator;
		//RandomMapGenerator_Town *townGenerator;
		RandomMapGenerator_WorldMap *worldMapGenerator;
		RandomNumberGenerator numberGenerator;
		QGraphicsScene *previewScene;
		QGraphicsPixmapItem *previewImage;
		QRegExpValidator *randomSeedValidator;
		bool mapViewSetup;
		bool ignoreEvents;
};

#endif // RANDOMMAPGENERATOR_H
