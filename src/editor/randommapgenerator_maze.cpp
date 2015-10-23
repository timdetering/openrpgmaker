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
#include <cstdlib>
#include "projectdata.h"
#include "projectdata_map.h"
#include "projectdata_tileset.h"
#include "projectdata_tile.h"
#include "randommapgenerator_maze.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "tilechooser.h"
#include "xmlParser.h"

RandomMapGenerator_Maze::RandomMapGenerator_Maze(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	leName->setText("Untitled");
	configured = false;
	mapData = NULL;
	bWidthSpinButton->setSpinBox(sbWidth);
	bHeightSpinButton->setSpinBox(sbHeight);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

RandomMapGenerator_Maze::~RandomMapGenerator_Maze()
{
	if (mapData != NULL)
	{
		delete[] mapData;
		mapData = NULL;
	}
}

void RandomMapGenerator_Maze::setupGenerator(XMLNode ConfigurationNode)
{
	sbWidth->setValue(atoi(ConfigurationNode.getAttribute("width")));
	sbHeight->setValue(atoi(ConfigurationNode.getAttribute("height")));
	sldrPathStructure->setValue(atoi(ConfigurationNode.getAttribute("pathStructure")));
	tbWallTile->setTile(QString(ConfigurationNode.getAttribute("wallTile")).toUShort(0, 16));
	tbFloorTile->setTile(QString(ConfigurationNode.getAttribute("floorTile")).toUShort(0, 16));
}

XMLNode RandomMapGenerator_Maze::getConfigurationNode()
{
	XMLNode ConfigurationNode = XMLNode::createXMLTopNode("mazegenerator");
	ConfigurationNode.addAttribute("width", sbWidth->cleanText().toUtf8().data());
	ConfigurationNode.addAttribute("height", sbHeight->cleanText().toUtf8().data());
	ConfigurationNode.addAttribute("pathStructure", QString::number(sldrPathStructure->value()).toUtf8().data());
	ConfigurationNode.addAttribute("wallTile", QString("%1").arg(tbWallTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	ConfigurationNode.addAttribute("floorTile", QString("%1").arg(tbFloorTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	return ConfigurationNode;
}

bool RandomMapGenerator_Maze::isConfigured()
{
	return configured;
}

QStringList RandomMapGenerator_Maze::getGeneratedMapNames()
{
	return QStringList() << leName->text();
}

Map *RandomMapGenerator_Maze::getGeneratedMap(int mapNum)
{
	Q_UNUSED(mapNum);
	ProjectData::DataReference<Tileset> *tilesetRef;
	Map::TileData wallTile, floorTile;
	Tile *pTile;
	unsigned short tile = tbWallTile->getTile();
	wallTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	wallTile.tileID = (unsigned char)(tile % 256);
	wallTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		wallTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		wallTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbFloorTile->getTile();
	floorTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	floorTile.tileID = (unsigned char)(tile % 256);
	floorTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		floorTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		floorTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	if (mapData != NULL)
	{
		Map *generatedMap = new Map;
		generatedMap->setName(leName->text());
		generatedMap->resizeMap(mapWidth, mapHeight);
		generatedMap->setBorderTile(tbWallTile->getTile());
		for (int y = 0; y < mapHeight; ++y)
		{
			for (int x = 0; x < mapWidth; ++x)
			{
				if (mapData[y * mapWidth + x] == CELL_WALL)
					generatedMap->setTileData(x, y, 0, wallTile);
				else if (mapData[y * mapWidth + x] == CELL_FLOOR)
					generatedMap->setTileData(x, y, 0, floorTile);
				else
					generatedMap->setTileData(x, y, 0, wallTile);
			}
		}
		generatedMap->autoCalcAllTileFlags(0);
		return generatedMap;
	}
	return NULL;
}

void RandomMapGenerator_Maze::generateMaze(unsigned long seed)
{
	Loading *generatingIndicator;
	int pathType, startX, startY;
	generatingIndicator = new Loading("Generating Maze");
	generatingIndicator->show();
	numberGenerator.seedGenerator(seed);
	srand(seed);
	mapWidth = sbWidth->value();
	mapHeight = sbHeight->value();
	if (mapData != NULL)
	{
		delete[] mapData;
		mapData = NULL;
	}
	mapData = new int[mapWidth * mapHeight];
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			if (x == 0 || x == mapWidth - 1 || y == 0 || y == mapHeight - 1)
				mapData[y * mapWidth + x] = CELL_WALL;
			else
				mapData[y * mapWidth + x] = CELL_UNVISITED;
		}
	}
	pathStructure = sldrPathStructure->value();
	pathType = (int)numberGenerator.getRandomNumber(1ul, 20ul) % 4;
	if (pathType == PATH_TOPTOBOTTOM)
	{
		int x, y;
		bool done = false;
		startX = (int)numberGenerator.getRandomNumber(1ul, (unsigned long)(mapWidth - 2));
		startY = 1;
		generateMaze(startX, startY);
		while (!done)
		{
			bool found1 = false;
			for (y = 0; y < mapHeight; ++y)
			{
				for (x = 0; x < mapWidth; ++x)
				{
					if (mapData[y * mapWidth + x] == CELL_UNVISITED)
					{
						fixMaze(x, y);
						found1 = true;
					}
				}
			}
			if (!found1)
				done = true;
		}
	}
	else if (pathType == PATH_BOTTOMTOTOP)
	{
		int x, y;
		bool done = false;
		startX = (int)numberGenerator.getRandomNumber(1ul, (unsigned long)(mapWidth - 2));
		startY = mapHeight - 2;
		generateMaze(startX, startY);
		while (!done)
		{
			bool found1 = false;
			for (y = 0; y < mapHeight; ++y)
			{
				for (x = 0; x < mapWidth; ++x)
				{
					if (mapData[y * mapWidth + x] == CELL_UNVISITED)
					{
						fixMaze(x, y);
						found1 = true;
					}
				}
			}
			if (!found1)
				done = true;
		}
	}
	else if (pathType == PATH_LEFTTORIGHT)
	{
		int x, y;
		bool done = false;
		startX = 1;
		startY = (int)numberGenerator.getRandomNumber(1ul, (unsigned long)(mapHeight - 2));
		generateMaze(startX, startY);
		while (!done)
		{
			bool found1 = false;
			for (y = 0; y < mapHeight; ++y)
			{
				for (x = 0; x < mapWidth; ++x)
				{
					if (mapData[y * mapWidth + x] == CELL_UNVISITED)
					{
						fixMaze(x, y);
						found1 = true;
					}
				}
			}
			if (!found1)
				done = true;
		}
	}
	else if (pathType == PATH_RIGHTTOLEFT)
	{
		int x, y;
		bool done = false;
		startX = mapWidth - 2;
		startY = (int)numberGenerator.getRandomNumber(1ul, (unsigned long)(mapHeight - 2));
		generateMaze(startX, startY);
		while (!done)
		{
			bool found1 = false;
			for (y = 0; y < mapHeight; ++y)
			{
				for (x = 0; x < mapWidth; ++x)
				{
					if (mapData[y * mapWidth + x] == CELL_UNVISITED)
					{
						fixMaze(x, y);
						found1 = true;
					}
				}
			}
			if (!found1)
				done = true;
		}
	}
	generatingIndicator->doneLoading();
	delete generatingIndicator;
}

void RandomMapGenerator_Maze::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Maze Generator");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void RandomMapGenerator_Maze::on_buttonBox_accepted()
{
	configured = true;
}

int RandomMapGenerator_Maze::numNeighbors(int x, int y)
{
	int x2, y2, numneighbors = 0;
	x2 = x;
	y2 = y - 1;
	if (x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (mapData[y2 * mapWidth + x2] == CELL_FLOOR)
			++numneighbors;
	}
	x2 = x - 1;
	y2 = y;
	if (x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (mapData[y2 * mapWidth + x2] == CELL_FLOOR)
			++numneighbors;
	}
	x2 = x + 1;
	y2 = y;
	if (x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (mapData[y2 * mapWidth + x2] == CELL_FLOOR)
			++numneighbors;
	}
	x2 = x;
	y2 = y + 1;
	if (x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (mapData[y2 * mapWidth + x2] == CELL_FLOOR)
			++numneighbors;
	}
	return numneighbors;
}

void RandomMapGenerator_Maze::generateMaze(int x, int y, int direction)
{
	int x2, y2;
	bool done = false;
	QCoreApplication::processEvents();
	if (mapData[y * mapWidth + x] == CELL_UNVISITED)
		mapData[y * mapWidth + x] = CELL_FLOOR;
	else
		return;
	x2 = x;
	y2 = y - 1;
	if (x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (numNeighbors(x2, y2) > 1 && mapData[y2 * mapWidth + x2] == CELL_UNVISITED)
			mapData[y2 * mapWidth + x2] = CELL_WALL;
	}
	x2 = x;
	y2 = y + 1;
	if (x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (numNeighbors(x2, y2) > 1 && mapData[y2 * mapWidth + x2] == CELL_UNVISITED)
			mapData[y2 * mapWidth + x2] = CELL_WALL;
	}
	x2 = x + 1;
	y2 = y;
	if (x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (numNeighbors(x2, y2) > 1 && mapData[y2 * mapWidth + x2] == CELL_UNVISITED)
			mapData[y2 * mapWidth + x2] = CELL_WALL;
	}
	x2 = x - 1;
	y2 = y;
	if (x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (numNeighbors(x2, y2) > 1 && mapData[y2 * mapWidth + x2] == CELL_UNVISITED)
			mapData[y2 * mapWidth + x2] = CELL_WALL;
	}
	while (!done)
	{
		int max = 0;
		int northPriority = 50;
		int southPriority = 50;
		int eastPriority = 50;
		int westPriority = 50;
		bool north;
		bool south;
		bool east;
		bool west;
		if (direction == DIR_NORTH)
		{
			northPriority += pathStructure;
			eastPriority -= pathStructure;
			westPriority -= pathStructure;
		}
		else if (direction == DIR_SOUTH)
		{
			southPriority += pathStructure;
			eastPriority -= pathStructure;
			westPriority -= pathStructure;
		}
		else if (direction == DIR_EAST)
		{
			eastPriority += pathStructure;
			northPriority -= pathStructure;
			southPriority -= pathStructure;
		}
		else if (direction == DIR_WEST)
		{
			westPriority += pathStructure;
			northPriority -= pathStructure;
			southPriority -= pathStructure;
		}
		x2 = x;
		y2 = y - 1;
		if (x2 <= 0 || x2 >= mapWidth - 1 || y2 <= 0 || y2 >= mapHeight - 1)
			north = false;
		else if (mapData[y2 * mapWidth + x2] != CELL_UNVISITED)
			north = false;
		else
		{
			north = true;
		}
		x2 = x;
		y2 = y + 1;
		if (x2 <= 0 || x2 >= mapWidth - 1 || y2 <= 0 || y2 >= mapHeight - 1)
			south = false;
		else if (mapData[y2 * mapWidth + x2] != CELL_UNVISITED)
			south = false;
		else
		{
			south = true;
		}
		x2 = x + 1;
		y2 = y;
		if (x2 <= 0 || x2 >= mapWidth - 1 || y2 <= 0 || y2 >= mapHeight - 1)
			east = false;
		else if (mapData[y2 * mapWidth + x2] != CELL_UNVISITED)
			east = false;
		else
		{
			east = true;
		}
		x2 = x - 1;
		y2 = y;
		if (x2 <= 0 || x2 >= mapWidth - 1 || y2 <= 0 || y2 >= mapHeight - 1)
			west = false;
		else if (mapData[y2 * mapWidth + x2] != CELL_UNVISITED)
			west = false;
		else
		{
			west = true;
		}
		if (north)
			max += northPriority;
		if (south)
			max += southPriority;
		if (east)
			max += eastPriority;
		if (west)
			max += westPriority;
		if (max > 0)
		{
			int next = numberGenerator.getRandomNumber(1, max);
			if (north)
			{
				next -= northPriority;
				if (next <= 0)
					generateMaze(x, y - 1, DIR_NORTH);
			}
			if (south)
			{
				next -= southPriority;
				if (next <= 0)
					generateMaze(x, y + 1, DIR_SOUTH);
			}
			if (east)
			{
				next -= eastPriority;
				if (next <= 0)
					generateMaze(x + 1, y, DIR_EAST);
			}
			if (west)
			{
				next -= westPriority;
				if (next <= 0)
					generateMaze(x - 1, y, DIR_WEST);
			}
		}
		else
			done = true;
	}
}

bool RandomMapGenerator_Maze::fixMaze(int x, int y)
{
	int x2, y2;
	bool opened = false;
	QCoreApplication::processEvents();
	x2 = x;
	y2 = y - 1;
	if (!opened && x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (mapData[y2 * mapWidth + x2] == CELL_WALL)
		{
			int numneighbors = numNeighbors(x2, y2);
			if (numneighbors >= 1)
			{
				mapData[y2 * mapWidth + x2] = CELL_FLOOR;
				opened = true;
				generateMaze(x, y);
			}
		}
	}
	x2 = x - 1;
	y2 = y;
	if (!opened && x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (mapData[y2 * mapWidth + x2] == CELL_WALL)
		{
			int numneighbors = numNeighbors(x2, y2);
			if (numneighbors >= 1)
			{
				mapData[y2 * mapWidth + x2] = CELL_FLOOR;
				opened = true;
				generateMaze(x, y);
			}
		}
	}
	x2 = x + 1;
	y2 = y;
	if (!opened && x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (mapData[y2 * mapWidth + x2] == CELL_WALL)
		{
			int numneighbors = numNeighbors(x2, y2);
			if (numneighbors >= 1)
			{
				mapData[y2 * mapWidth + x2] = CELL_FLOOR;
				opened = true;
				generateMaze(x, y);
			}
		}
	}
	x2 = x;
	y2 = y + 1;
	if (!opened && x2 > 0 && x2 < mapWidth - 1 && y2 > 0 && y2 < mapHeight - 1)
	{
		if (mapData[y2 * mapWidth + x2] == CELL_WALL)
		{
			int numneighbors = numNeighbors(x2, y2);
			if (numneighbors >= 1)
			{
				mapData[y2 * mapWidth + x2] = CELL_FLOOR;
				opened = true;
				generateMaze(x, y);
			}
		}
	}
	return opened;
}
