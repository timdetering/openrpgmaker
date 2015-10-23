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

#ifndef RANDOMMAPGENERATOR_MAZE_H
#define RANDOMMAPGENERATOR_MAZE_H

#include "ui_randommapgenerator_maze.h"
#include "loading.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "xmlParser.h"

class RandomMapGenerator_Maze : public QDialog, public Ui::RandomMapGenerator_Maze
{
	Q_OBJECT
	public:
		RandomMapGenerator_Maze(QWidget *parent=0);
		~RandomMapGenerator_Maze();
		void setupGenerator(XMLNode ConfigurationNode);
		XMLNode getConfigurationNode();
		bool isConfigured();
		QStringList getGeneratedMapNames();
		Map *getGeneratedMap(int mapNum);
		void generateMaze(unsigned long seed);
	protected slots:
		void on_buttonBox_helpRequested();
		void on_buttonBox_accepted();
	private:
		enum CellType {CELL_UNVISITED=0, CELL_WALL=1, CELL_FLOOR=2};
		enum PathType {PATH_TOPTOBOTTOM=0, PATH_BOTTOMTOTOP=1, PATH_LEFTTORIGHT=2, PATH_RIGHTTOLEFT=3};
		enum Direction {DIR_NORTH=0, DIR_SOUTH=1, DIR_EAST=2, DIR_WEST=3, DIR_NONE=4};
		int numNeighbors(int x, int y);
		void generateMaze(int x, int y, int direction=DIR_NONE);
		bool fixMaze(int x, int y);
		void printMaze();
		int randomInteger(int min, int max);
		RandomNumberGenerator numberGenerator;
		int *mapData;
		int mapWidth;
		int mapHeight;
		int pathStructure;
		bool configured;
};

#endif // RANDOMMAPGENERATOR_MAZE_H
