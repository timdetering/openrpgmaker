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

#ifndef RANDOMMAPGENERATOR_DUNGEONSTRUCTURE_H
#define RANDOMMAPGENERATOR_DUNGEONSTRUCTURE_H

#include "ui_randommapgenerator_dungeonstructure.h"
#include "loading.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "xmlParser.h"

class RandomMapGenerator_DungeonStructure : public QDialog, public Ui::RandomMapGenerator_DungeonStructure
{
	Q_OBJECT
	public:
		RandomMapGenerator_DungeonStructure(QWidget *parent=0);
		~RandomMapGenerator_DungeonStructure();
		void setupGenerator(XMLNode ConfigurationNode);
		XMLNode getConfigurationNode();
		bool isConfigured();
		QStringList getGeneratedMapNames();
		Map *getGeneratedMap(int mapNum);
		void generateDungeonStructure(unsigned long seed);
	protected slots:
		void on_cbUseLowerWall_toggled(bool on);
		void on_cbUseUpperWall_toggled(bool on);
		void on_cbUseRoomFloorB_toggled(bool on);
		void on_cbUseRoomFloorC_toggled(bool on);
		void on_cbUseHallFloorB_toggled(bool on);
		void on_cbUseHallFloorC_toggled(bool on);
		void on_buttonBox_helpRequested();
		void on_buttonBox_accepted();
	private:
		struct Point
		{
			int x;
			int y;
			Point& operator=(const Point &other);
			bool operator==(const Point &other) const;
			bool operator<(const Point &other) const;
			bool operator>(const Point &other) const;
		};
		int getRandomPoint();
		bool isSpaceAvailable(int x, int y, int w, int h);
		bool isAdjacentToFloor(Point point);
		void addArea(int x, int y, int w, int h, int surroundSides, unsigned short floorType);
		void createMapData();
		void placeWalls();
		void placeFloors();
		void fillArea(int x, int y, unsigned short type);
		int numNeighbors(Point point, int type);
		void cleanDungeon();
		void removeWall(Point point);
		void removeWall(int index);
		enum MapType
		{
			SPACE_UNUSED=0,
			SPACE_WALL=1,
			SPACE_HALLFLOOR=2,
			SPACE_ROOMFLOOR=3,
			SPACE_HALLFLOORA=4,
			SPACE_HALLFLOORB=5,
			SPACE_HALLFLOORC=6,
			SPACE_ROOMFLOORA=7,
			SPACE_ROOMFLOORB=8,
			SPACE_ROOMFLOORC=9,
			SPACE_LOWERWALL=10,
			SPACE_UPPERWALL=11,
			SPACE_CEILING=12
		};
		enum Direction {DIR_NORTH=1, DIR_SOUTH=2, DIR_EAST=4, DIR_WEST=8};
		RandomNumberGenerator numberGenerator;
		QMap<Point, unsigned short> generatedMap;
		QMap<Point, unsigned short>::iterator mapIter;
		QList<Point> wallList;
		int numRooms;
		int roomsPlaced;
		int minRoomWidth;
		int minRoomHeight;
		int maxRoomWidth;
		int maxRoomHeight;
		int minHallLength;
		int maxHallLength;
		int mapWidth;
		int mapHeight;
		unsigned short *mapData;
		bool configured;
};

#endif // RANDOMMAPGENERATOR_DUNGEONSTRUCTURE_H
