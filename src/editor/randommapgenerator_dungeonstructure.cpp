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
#include "randommapgenerator_dungeonstructure.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "tilechooser.h"
#include "xmlParser.h"

RandomMapGenerator_DungeonStructure::RandomMapGenerator_DungeonStructure(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	leName->setText("Untitled");
	tbLowerWallTile->setEnabled(false);
	tbUpperWallTile->setEnabled(false);
	tbRoomFloorBTile->setEnabled(false);
	tbRoomFloorCTile->setEnabled(false);
	tbHallFloorBTile->setEnabled(false);
	tbHallFloorCTile->setEnabled(false);
	bNumRoomsSpinButton->setSpinBox(sbNumRooms);
	bMinRoomWidthSpinButton->setSpinBox(sbMinRoomWidth);
	bMinRoomHeightSpinButton->setSpinBox(sbMinRoomHeight);
	bMaxRoomWidthSpinButton->setSpinBox(sbMaxRoomWidth);
	bMaxRoomHeightSpinButton->setSpinBox(sbMaxRoomHeight);
	bMinHallLengthSpinButton->setSpinBox(sbMinHallLength);
	bMaxHallLengthSpinButton->setSpinBox(sbMaxHallLength);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	mapData = NULL;
	configured = false;
}

RandomMapGenerator_DungeonStructure::~RandomMapGenerator_DungeonStructure()
{
	if (mapData != NULL)
	{
		delete[] mapData;
		mapData = NULL;
	}
	generatedMap.clear();
	wallList.clear();
}

void RandomMapGenerator_DungeonStructure::setupGenerator(XMLNode ConfigurationNode)
{
	XMLNode TempNode;
	if (QString(ConfigurationNode.getAttribute("wallPosition")) == "above")
		rbWallAboveFloor->setChecked(true);
	else
		rbWallBelowFloor->setChecked(true);
	TempNode = ConfigurationNode.getChildNode("rooms");
	sbNumRooms->setValue(atoi(TempNode.getAttribute("num")));
	sbMinRoomWidth->setValue(atoi(TempNode.getAttribute("minWidth")));
	sbMinRoomHeight->setValue(atoi(TempNode.getAttribute("minHeight")));
	sbMaxRoomWidth->setValue(atoi(TempNode.getAttribute("maxWidth")));
	sbMaxRoomHeight->setValue(atoi(TempNode.getAttribute("maxHeight")));
	TempNode = ConfigurationNode.getChildNode("hallLength");
	sbMinHallLength->setValue(atoi(TempNode.getAttribute("min")));
	sbMaxHallLength->setValue(atoi(TempNode.getAttribute("max")));
	if (QString(TempNode.getAttribute("removeDeadEnds")) == "true")
		cbRemoveDeadEnds->setChecked(true);
	else
		cbRemoveDeadEnds->setChecked(false);
	TempNode = ConfigurationNode.getChildNode("wallTiles");
	tbCeilingTile->setTile(QString(TempNode.getAttribute("ceiling")).toUShort(0, 16));
	tbLowerWallTile->setTile(QString(TempNode.getAttribute("lowerWall")).toUShort(0, 16));
	tbUpperWallTile->setTile(QString(TempNode.getAttribute("upperWall")).toUShort(0, 16));
	if (QString(TempNode.getAttribute("useLowerWall")) == "true")
		cbUseLowerWall->setChecked(true);
	else
		cbUseLowerWall->setChecked(false);
	if (QString(TempNode.getAttribute("useUpperWall")) == "true")
		cbUseUpperWall->setChecked(true);
	else
		cbUseUpperWall->setChecked(false);
	TempNode = ConfigurationNode.getChildNode("roomTiles");
	tbRoomFloorATile->setTile(QString(TempNode.getAttribute("floorA")).toUShort(0, 16));
	tbRoomFloorBTile->setTile(QString(TempNode.getAttribute("floorB")).toUShort(0, 16));
	tbRoomFloorCTile->setTile(QString(TempNode.getAttribute("floorC")).toUShort(0, 16));
	if (QString(TempNode.getAttribute("useFloorB")) == "true")
		cbUseRoomFloorB->setChecked(true);
	else
		cbUseRoomFloorB->setChecked(false);
	if (QString(TempNode.getAttribute("useFloorC")) == "true")
		cbUseRoomFloorC->setChecked(true);
	else
		cbUseRoomFloorC->setChecked(false);
	TempNode = ConfigurationNode.getChildNode("hallTiles");
	tbHallFloorATile->setTile(QString(TempNode.getAttribute("floorA")).toUShort(0, 16));
	tbHallFloorBTile->setTile(QString(TempNode.getAttribute("floorB")).toUShort(0, 16));
	tbHallFloorCTile->setTile(QString(TempNode.getAttribute("floorC")).toUShort(0, 16));
	if (QString(TempNode.getAttribute("useFloorB")) == "true")
		cbUseHallFloorB->setChecked(true);
	else
		cbUseHallFloorB->setChecked(false);
	if (QString(TempNode.getAttribute("useFloorC")) == "true")
		cbUseHallFloorC->setChecked(true);
	else
		cbUseHallFloorC->setChecked(false);
}

XMLNode RandomMapGenerator_DungeonStructure::getConfigurationNode()
{
	XMLNode TempNode, ConfigurationNode = XMLNode::createXMLTopNode("dungeongenerator");
	ConfigurationNode.addAttribute("wallPosition", (rbWallAboveFloor->isChecked()) ? "above":"below");
	TempNode = ConfigurationNode.addChild("rooms");
	TempNode.addAttribute("num", sbNumRooms->cleanText().toUtf8().data());
	TempNode.addAttribute("minWidth", sbMinRoomWidth->cleanText().toUtf8().data());
	TempNode.addAttribute("minHeight", sbMinRoomHeight->cleanText().toUtf8().data());
	TempNode.addAttribute("maxWidth", sbMaxRoomWidth->cleanText().toUtf8().data());
	TempNode.addAttribute("maxHeight", sbMaxRoomHeight->cleanText().toUtf8().data());
	TempNode = ConfigurationNode.addChild("hallLength");
	TempNode.addAttribute("min", sbMinHallLength->cleanText().toUtf8().data());
	TempNode.addAttribute("max", sbMaxHallLength->cleanText().toUtf8().data());
	TempNode.addAttribute("removeDeadEnds", (cbRemoveDeadEnds->isChecked()) ? "true":"false");
	TempNode = ConfigurationNode.addChild("wallTiles");
	TempNode.addAttribute("ceiling", QString("%1").arg(tbCeilingTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("lowerWall", QString("%1").arg(tbLowerWallTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("upperWall", QString("%1").arg(tbUpperWallTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("useLowerWall", (cbUseLowerWall->isChecked()) ? "true":"false");
	TempNode.addAttribute("useUpperWall", (cbUseUpperWall->isChecked()) ? "true":"false");
	TempNode = ConfigurationNode.addChild("roomTiles");
	TempNode.addAttribute("floorA", QString("%1").arg(tbRoomFloorATile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("floorB", QString("%1").arg(tbRoomFloorBTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("floorC", QString("%1").arg(tbRoomFloorCTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("useFloorB", (cbUseRoomFloorB->isChecked()) ? "true":"false");
	TempNode.addAttribute("useFloorC", (cbUseRoomFloorC->isChecked()) ? "true":"false");
	TempNode = ConfigurationNode.addChild("hallTiles");
	TempNode.addAttribute("floorA", QString("%1").arg(tbHallFloorATile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("floorB", QString("%1").arg(tbHallFloorBTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("floorC", QString("%1").arg(tbHallFloorCTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("useFloorB", (cbUseHallFloorB->isChecked()) ? "true":"false");
	TempNode.addAttribute("useFloorC", (cbUseHallFloorC->isChecked()) ? "true":"false");
	return ConfigurationNode;
}

bool RandomMapGenerator_DungeonStructure::isConfigured()
{
	return configured;
}

QStringList RandomMapGenerator_DungeonStructure::getGeneratedMapNames()
{
	return QStringList() << leName->text();
}

Map *RandomMapGenerator_DungeonStructure::getGeneratedMap(int mapNum)
{
	Q_UNUSED(mapNum);
	Map *map = new Map;
	ProjectData::DataReference<Tileset> *tilesetRef;
	Map::TileData ceilingTile, lowerWallTile, upperWallTile, roomFloorATile, roomFloorBTile, roomFloorCTile, hallFloorATile, hallFloorBTile, hallFloorCTile;
	Tile *pTile;
	unsigned short tile = tbCeilingTile->getTile();
	ceilingTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	ceilingTile.tileID = (unsigned char)(tile % 256);
	ceilingTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		ceilingTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		ceilingTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbLowerWallTile->getTile();
	lowerWallTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	lowerWallTile.tileID = (unsigned char)(tile % 256);
	lowerWallTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		lowerWallTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		lowerWallTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbUpperWallTile->getTile();
	upperWallTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	upperWallTile.tileID = (unsigned char)(tile % 256);
	upperWallTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		upperWallTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		upperWallTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbRoomFloorATile->getTile();
	roomFloorATile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	roomFloorATile.tileID = (unsigned char)(tile % 256);
	roomFloorATile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		roomFloorATile.phaseDirections[i] = pTile->getPhaseDirections(i);
		roomFloorATile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbRoomFloorBTile->getTile();
	roomFloorBTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	roomFloorBTile.tileID = (unsigned char)(tile % 256);
	roomFloorBTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		roomFloorBTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		roomFloorBTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbRoomFloorCTile->getTile();
	roomFloorCTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	roomFloorCTile.tileID = (unsigned char)(tile % 256);
	roomFloorCTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		roomFloorCTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		roomFloorCTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbHallFloorATile->getTile();
	hallFloorATile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	hallFloorATile.tileID = (unsigned char)(tile % 256);
	hallFloorATile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		hallFloorATile.phaseDirections[i] = pTile->getPhaseDirections(i);
		hallFloorATile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbHallFloorBTile->getTile();
	hallFloorBTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	hallFloorBTile.tileID = (unsigned char)(tile % 256);
	hallFloorBTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		hallFloorBTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		hallFloorBTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbHallFloorCTile->getTile();
	hallFloorCTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	hallFloorCTile.tileID = (unsigned char)(tile % 256);
	hallFloorCTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		hallFloorCTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		hallFloorCTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	map->setName(leName->text());
	map->resizeMap(mapWidth, mapHeight);
	map->setBorderTile(tbCeilingTile->getTile());
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int location = y * mapWidth + x;
			if (mapData[location] == SPACE_CEILING)
				map->setTileData(x, y, 0, ceilingTile);
			else if (mapData[location] == SPACE_UPPERWALL)
				map->setTileData(x, y, 0, upperWallTile);
			else if (mapData[location] == SPACE_LOWERWALL)
				map->setTileData(x, y, 0, lowerWallTile);
			else if (mapData[location] == SPACE_ROOMFLOORA)
				map->setTileData(x, y, 0, roomFloorATile);
			else if (mapData[location] == SPACE_ROOMFLOORB)
				map->setTileData(x, y, 0, roomFloorBTile);
			else if (mapData[location] == SPACE_ROOMFLOORC)
				map->setTileData(x, y, 0, roomFloorCTile);
			else if (mapData[location] == SPACE_HALLFLOORA)
				map->setTileData(x, y, 0, hallFloorATile);
			else if (mapData[location] == SPACE_HALLFLOORB)
				map->setTileData(x, y, 0, hallFloorBTile);
			else if (mapData[location] == SPACE_HALLFLOORC)
				map->setTileData(x, y, 0, hallFloorCTile);
		}
	}
	map->autoCalcAllTileFlags(0);
	return map;
}

void RandomMapGenerator_DungeonStructure::generateDungeonStructure(unsigned long seed)
{
	Loading *generatingIndicator;
	Point point, point2;
	int roomWidth, roomHeight, hallLength, availableDirections, randomNumber, roomPlacementAttempts, wallIndex;
	int maxPlacementAttempts = 5;
	bool north, south, east, west, done;
	generatingIndicator = new Loading("Generating Dungeon-Like Structure");
	generatingIndicator->show();
	numberGenerator.seedGenerator(seed);
	generatedMap.clear();
	wallList.clear();
	numRooms = sbNumRooms->value();
	minRoomWidth = sbMinRoomWidth->value();
	minRoomHeight = sbMinRoomHeight->value();
	maxRoomWidth = sbMaxRoomWidth->value();
	maxRoomHeight = sbMaxRoomHeight->value();
	minHallLength = sbMinHallLength->value();
	maxHallLength = sbMaxHallLength->value();
	roomsPlaced = 0;
	roomWidth = (int)(numberGenerator.getRandomNumber((unsigned long)(minRoomWidth * 1000), (unsigned long)(maxRoomWidth * 1000)) / 1000ul) + 2;
	roomHeight = (int)(numberGenerator.getRandomNumber((unsigned long)(minRoomHeight * 1000), (unsigned long)(maxRoomHeight * 1000)) / 1000ul) + 2;
	addArea(0, 0, roomWidth, roomHeight, DIR_NORTH | DIR_SOUTH | DIR_EAST | DIR_WEST, SPACE_ROOMFLOOR);
	++roomsPlaced;
	while (roomsPlaced < numRooms)
	{
		QCoreApplication::processEvents();
		done = false;
		if (roomPlacementAttempts != maxPlacementAttempts)
			done = true;
		while (!done)
		{
			hallLength = (int)(numberGenerator.getRandomNumber((unsigned long)(minHallLength * 1000), (unsigned long)(maxHallLength * 1000)) / 1000);
			wallIndex = getRandomPoint();
			point = wallList[wallIndex];
			availableDirections = 0;
			north = false;
			south = false;
			east = false;
			west = false;
			if (isSpaceAvailable(point.x - 1, point.y - hallLength, 3, hallLength))
			{
				north = true;
				++availableDirections;
			}
			if (isSpaceAvailable(point.x - 1, point.y + 1, 3, hallLength))
			{
				south = true;
				++availableDirections;
			}
			if (isSpaceAvailable(point.x + 1, point.y - 1, hallLength, 3))
			{
				east = true;
				++availableDirections;
			}
			if (isSpaceAvailable(point.x - hallLength, point.y - 1, hallLength, 3))
			{
				west = true;
				++availableDirections;
			}
			if (availableDirections > 0)
			{
				randomNumber = (int)(numberGenerator.getRandomNumber(1000ul, (unsigned long)(availableDirections * 1000)) / 1000);
				if (north)
				{
					--randomNumber;
					if (randomNumber == 0)
					{
						removeWall(wallIndex);
						addArea(point.x - 1, point.y - hallLength, 3, hallLength, DIR_NORTH | DIR_EAST | DIR_WEST, SPACE_HALLFLOOR);
						north = false;
						south = false;
						east = false;
						west = false;
						done = true;
					}
				}
				if (south && randomNumber > 0)
				{
					--randomNumber;
					if (randomNumber == 0)
					{
						removeWall(wallIndex);
						addArea(point.x - 1, point.y + 1, 3, hallLength, DIR_SOUTH | DIR_EAST | DIR_WEST, SPACE_HALLFLOOR);
						north = false;
						south = false;
						east = false;
						west = false;
						done = true;
					}
				}
				if (east && randomNumber > 0)
				{
					--randomNumber;
					if (randomNumber == 0)
					{
						removeWall(wallIndex);
						addArea(point.x + 1, point.y - 1, hallLength, 3, DIR_NORTH | DIR_SOUTH | DIR_EAST, SPACE_HALLFLOOR);
						north = false;
						south = false;
						east = false;
						west = false;
						done = true;
					}
				}
				if (west && randomNumber > 0)
				{
					--randomNumber;
					if (randomNumber == 0)
					{
						removeWall(wallIndex);
						addArea(point.x - hallLength, point.y - 1, hallLength, 3, DIR_NORTH | DIR_SOUTH | DIR_WEST, SPACE_HALLFLOOR);
						north = false;
						south = false;
						east = false;
						west = false;
						done = true;
					}
				}
			}
		}
		done = false;
		roomPlacementAttempts = 0;
		QCoreApplication::processEvents();
		while (!done && roomPlacementAttempts < maxPlacementAttempts)
		{
			int max = 0;
			wallIndex = getRandomPoint();
			point = wallList[wallIndex];
			north = false;
			south = false;
			east = false;
			west = false;
			roomWidth = (int)(numberGenerator.getRandomNumber((unsigned long)(minRoomWidth * 1000), (unsigned long)(maxRoomWidth * 1000)) / 1000ul) + 2;
			roomHeight = (int)(numberGenerator.getRandomNumber((unsigned long)(minRoomHeight * 1000), (unsigned long)(maxRoomHeight * 1000)) / 1000ul) + 2;
			point2.x = point.x;
			point2.y = point.y - 1;
			if (generatedMap.find(point2) == generatedMap.end())
				north = true;
			point2.x = point.x;
			point2.y = point.y + 1;
			if (generatedMap.find(point2) == generatedMap.end())
				south = true;
			point2.x = point.x + 1;
			point2.y = point.y;
			if (generatedMap.find(point2) == generatedMap.end())
				east = true;
			point2.x = point.x - 1;
			point2.y = point.y;
			if (generatedMap.find(point2) == generatedMap.end())
				west = true;
			if (north)
				++max;
			if (south)
				++max;
			if (east)
				++max;
			if (west)
				++max;
			randomNumber = (int)(numberGenerator.getRandomNumber(1000ul, (unsigned long)(max * 1000)) / 1000ul);
			if (north)
			{
				--randomNumber;
				if (randomNumber == 0)
				{
					max = 0;
					bool spaceAvailable[roomWidth - 2];
					for (int i = 0; i < roomWidth - 2; ++i)
					{
						spaceAvailable[i] = isSpaceAvailable(point.x - i - 1, point.y - roomHeight, roomWidth, roomHeight);
						if (spaceAvailable[i])
							++max;
					}
					if (max > 0)
					{
						randomNumber = (int)(numberGenerator.getRandomNumber(1000ul, (unsigned long)(max * 1000)) / 1000ul);
						for (int i = 0; i < roomWidth - 2 && randomNumber > 0; ++i)
						{
							if (spaceAvailable[i])
							{
								--randomNumber;
								if (randomNumber == 0)
								{
									addArea(point.x - i - 1, point.y - roomHeight, roomWidth, roomHeight, DIR_NORTH | DIR_SOUTH | DIR_EAST | DIR_WEST, SPACE_ROOMFLOOR);
									removeWall(wallIndex);
									point2.x = point.x;
									point2.y = point.y - 1;
									removeWall(point2);
									++roomsPlaced;
									done = true;
								}
							}
						}
					}
					north = false;
					south = false;
					east = false;
					west = false;
				}
			}
			if (south)
			{
				--randomNumber;
				if (randomNumber == 0)
				{
					max = 0;
					bool spaceAvailable[roomWidth - 2];
					for (int i = 0; i < roomWidth - 2; ++i)
					{
						spaceAvailable[i] = isSpaceAvailable(point.x - i - 1, point.y + 1, roomWidth, roomHeight);
						if (spaceAvailable[i])
							++max;
					}
					if (max > 0)
					{
						randomNumber = (int)(numberGenerator.getRandomNumber(1000ul, (unsigned long)(max * 1000)) / 1000ul);
						for (int i = 0; i < roomWidth - 2 && randomNumber > 0; ++i)
						{
							if (spaceAvailable[i])
							{
								--randomNumber;
								if (randomNumber == 0)
								{
									addArea(point.x - i - 1, point.y + 1, roomWidth, roomHeight, DIR_NORTH | DIR_SOUTH | DIR_EAST | DIR_WEST, SPACE_ROOMFLOOR);
									removeWall(wallIndex);
									point2.x = point.x;
									point2.y = point.y + 1;
									removeWall(point2);
									++roomsPlaced;
									done = true;
								}
							}
						}
					}
					north = false;
					south = false;
					east = false;
					west = false;
				}
			}
			if (east)
			{
				--randomNumber;
				if (randomNumber == 0)
				{
					max = 0;
					bool spaceAvailable[roomHeight - 2];
					for (int i = 0; i < roomHeight - 2; ++i)
					{
						spaceAvailable[i] = isSpaceAvailable(point.x + 1, point.y - i - 1, roomWidth, roomHeight);
						if (spaceAvailable[i])
							++max;
					}
					if (max > 0)
					{
						randomNumber = (int)(numberGenerator.getRandomNumber(1000ul, (unsigned long)(max * 1000)) / 1000ul);
						for (int i = 0; i < roomHeight - 2 && randomNumber > 0; ++i)
						{
							if (spaceAvailable[i])
							{
								--randomNumber;
								if (randomNumber == 0)
								{
									addArea(point.x + 1, point.y - i - 1, roomWidth, roomHeight, DIR_NORTH | DIR_SOUTH | DIR_EAST | DIR_WEST, SPACE_ROOMFLOOR);
									removeWall(wallIndex);
									point2.x = point.x + 1;
									point2.y = point.y;
									removeWall(point2);
									++roomsPlaced;
									done = true;
								}
							}
						}
					}
					north = false;
					south = false;
					east = false;
					west = false;
				}
			}
			if (west)
			{
				--randomNumber;
				if (randomNumber == 0)
				{
					max = 0;
					bool spaceAvailable[roomHeight - 2];
					for (int i = 0; i < roomHeight - 2; ++i)
					{
						spaceAvailable[i] = isSpaceAvailable(point.x - roomWidth, point.y - i - 1, roomWidth, roomHeight);
						if (spaceAvailable[i])
							++max;
					}
					if (max > 0)
					{
						randomNumber = (int)(numberGenerator.getRandomNumber(1000ul, (unsigned long)(max * 1000)) / 1000ul);
						for (int i = 0; i < roomHeight - 2 && randomNumber > 0; ++i)
						{
							if (spaceAvailable[i])
							{
								--randomNumber;
								if (randomNumber == 0)
								{
									addArea(point.x - roomWidth, point.y - i - 1, roomWidth, roomHeight, DIR_NORTH | DIR_SOUTH | DIR_EAST | DIR_WEST, SPACE_ROOMFLOOR);
									removeWall(wallIndex);
									point2.x = point.x - 1;
									point2.y = point.y;
									removeWall(point2);
									++roomsPlaced;
									done = true;
								}
							}
						}
					}
					north = false;
					south = false;
					east = false;
					west = false;
				}
			}
			++roomPlacementAttempts;
		}
	}
	if (cbRemoveDeadEnds->isChecked())
		cleanDungeon();
	createMapData();
	placeWalls();
	placeFloors();
	generatingIndicator->doneLoading();
	delete generatingIndicator;
}

void RandomMapGenerator_DungeonStructure::on_cbUseLowerWall_toggled(bool on)
{
	tbLowerWallTile->setEnabled(on);
}

void RandomMapGenerator_DungeonStructure::on_cbUseUpperWall_toggled(bool on)
{
	tbUpperWallTile->setEnabled(on);
}

void RandomMapGenerator_DungeonStructure::on_cbUseRoomFloorB_toggled(bool on)
{
	tbRoomFloorBTile->setEnabled(on);
}

void RandomMapGenerator_DungeonStructure::on_cbUseRoomFloorC_toggled(bool on)
{
	tbRoomFloorCTile->setEnabled(on);
}

void RandomMapGenerator_DungeonStructure::on_cbUseHallFloorB_toggled(bool on)
{
	tbHallFloorBTile->setEnabled(on);
}

void RandomMapGenerator_DungeonStructure::on_cbUseHallFloorC_toggled(bool on)
{
	tbHallFloorCTile->setEnabled(on);
}

void RandomMapGenerator_DungeonStructure::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Dungeon Generator");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void RandomMapGenerator_DungeonStructure::on_buttonBox_accepted()
{
	configured = true;
}

RandomMapGenerator_DungeonStructure::Point& RandomMapGenerator_DungeonStructure::Point::operator=(const RandomMapGenerator_DungeonStructure::Point &other)
{
	x = other.x;
	y = other.y;
	return *this;
}

bool RandomMapGenerator_DungeonStructure::Point::operator==(const RandomMapGenerator_DungeonStructure::Point &other) const
{
	if (x == other.x && y == other.y)
		return true;
	return false;
}

bool RandomMapGenerator_DungeonStructure::Point::operator<(const RandomMapGenerator_DungeonStructure::Point &other) const
{
	if (y < other.y)
		return true;
	else if (y == other.y && x < other.x)
		return true;
	return false;
}

bool RandomMapGenerator_DungeonStructure::Point::operator>(const RandomMapGenerator_DungeonStructure::Point &other) const
{
	if (y > other.y)
		return true;
	else if (y == other.y && x > other.x)
		return true;
	return false;
}

int RandomMapGenerator_DungeonStructure::getRandomPoint()
{
	int randomPoint;
	bool done = false;
	while (!done)
	{
		randomPoint = (int)numberGenerator.getRandomNumber(1ul, (unsigned long)wallList.size()) - 1;
		if (isAdjacentToFloor(wallList[randomPoint]))
			done = true;
	}
	return randomPoint;
}

bool RandomMapGenerator_DungeonStructure::isSpaceAvailable(int x, int y, int w, int h)
{
	for (int y2 = 0; y2 < h; ++y2)
	{
		for (int x2 = 0; x2 < w; ++x2)
		{
			Point point;
			point.x = x2 + x;
			point.y = y2 + y;
			if (generatedMap.contains(point))
				return false;
		}
	}
	return true;
}

bool RandomMapGenerator_DungeonStructure::isAdjacentToFloor(Point point)
{
	Point point2;
	int type;
	point2.x = point.x;
	point2.y = point.y - 1;
	if (generatedMap.contains(point2))
	{
		type = generatedMap[point2];
		if (type == SPACE_HALLFLOOR || type == SPACE_ROOMFLOOR)
			return true;
	}
	point2.x = point.x - 1;
	point2.y = point.y;
	if (generatedMap.contains(point2))
	{
		type = generatedMap[point2];
		if (type == SPACE_HALLFLOOR || type == SPACE_ROOMFLOOR)
			return true;
	}
	point2.x = point.x + 1;
	point2.y = point.y;
	if (generatedMap.contains(point2))
	{
		type = generatedMap[point2];
		if (type == SPACE_HALLFLOOR || type == SPACE_ROOMFLOOR)
			return true;
	}
	point2.x = point.x;
	point2.y = point.y + 1;
	if (generatedMap.contains(point2))
	{
		type = generatedMap[point2];
		if (type == SPACE_HALLFLOOR || type == SPACE_ROOMFLOOR)
			return true;
	}
	return false;
}

void RandomMapGenerator_DungeonStructure::addArea(int x, int y, int w, int h, int surroundSides, unsigned short floorType)
{
	for (int y2 = 0; y2 < h; ++y2)
	{
		for (int x2 = 0; x2 < w; ++x2)
		{
			Point point;
			point.x = x2 + x;
			point.y = y2 + y;
			if ((surroundSides & DIR_NORTH) != 0 && y2 == 0)
			{
				generatedMap.insert(point, SPACE_WALL);
				wallList.append(point);
			}
			else if ((surroundSides & DIR_SOUTH) != 0 && y2 == h - 1)
			{
				generatedMap.insert(point, SPACE_WALL);
				wallList.append(point);
			}
			else if ((surroundSides & DIR_EAST) != 0 && x2 == w - 1)
			{
				generatedMap.insert(point, SPACE_WALL);
				wallList.append(point);
			}
			else if ((surroundSides & DIR_WEST) != 0 && x2 == 0)
			{
				generatedMap.insert(point, SPACE_WALL);
				wallList.append(point);
			}
			else
				generatedMap.insert(point, floorType);
		}
	}
}

void RandomMapGenerator_DungeonStructure::createMapData()
{
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;
	int width, height;
	int offsetX, offsetY;
	for (mapIter = generatedMap.begin(); mapIter != generatedMap.end(); ++mapIter)
	{
		if (mapIter.value() != SPACE_WALL)
		{
			if (mapIter.key().x < minX)
				minX = mapIter.key().x;
			if (mapIter.key().x > maxX)
				maxX = mapIter.key().x;
			if (mapIter.key().y < minY)
				minY = mapIter.key().y;
			if (mapIter.key().y > maxY)
				maxY = mapIter.key().y;
		}
	}
	width = maxX - minX + 5;
	height = maxY - minY + 5;
	offsetX = (-1 * minX) + 2;
	offsetY = (-1 * minY) + 2;
	mapWidth = width;
	mapHeight = height;
	if (width < 20)
	{
		mapWidth = 20;
		offsetX += (mapWidth - width) / 2;
	}
	if (height < 15)
	{
		mapHeight = 15;
		offsetY += (mapHeight - height) / 2;
	}
	if (mapData != NULL)
	{
		delete[] mapData;
		mapData = NULL;
	}
	mapData = new unsigned short[mapWidth * mapHeight];
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			Point point;
			point.x = x - offsetX;
			point.y = y - offsetY;
			if (!generatedMap.contains(point))
				mapData[y * mapWidth + x] = SPACE_CEILING;
			else if (generatedMap[point] == SPACE_WALL)
				mapData[y * mapWidth + x] = SPACE_CEILING;
			else
				mapData[y * mapWidth + x] = generatedMap[point];
		}
	}
}

void RandomMapGenerator_DungeonStructure::placeWalls()
{
	int y2, y3;
	bool useLowerWall = cbUseLowerWall->isChecked();
	bool useUpperWall = cbUseUpperWall->isChecked();
	bool wallAboveFloor = rbWallAboveFloor->isChecked();
	if (useLowerWall || useUpperWall)
	{
		for (int y = 0; y < mapHeight; ++y)
		{
			for (int x = 0; x < mapWidth; ++x)
			{
				int location = y * mapWidth + x;
				if (mapData[location] == SPACE_CEILING)
				{
					if (useLowerWall && !useUpperWall)
					{
						if (wallAboveFloor)
							y2 = y + 1;
						else
							y2 = y - 1;
						if (y2 >= 0 && y2 < mapHeight)
						{
							unsigned short type = mapData[y2 * mapWidth + x];
							if (type == SPACE_HALLFLOOR || type == SPACE_ROOMFLOOR)
								mapData[location] = SPACE_LOWERWALL;
						}
					}
					else if (!useLowerWall && useUpperWall)
					{
						if (wallAboveFloor)
							y2 = y + 1;
						else
							y2 = y - 1;
						if (y2 >= 0 && y2 < mapHeight)
						{
							unsigned short type = mapData[y2 * mapWidth + x];
							if (type == SPACE_HALLFLOOR || type == SPACE_ROOMFLOOR)
								mapData[location] = SPACE_UPPERWALL;
						}
					}
					else
					{
						if (wallAboveFloor)
						{
							y2 = y + 1;
							y3 = y + 2;
						}
						else
						{
							y2 = y - 1;
							y3 = y - 2;
						}
						if (y2 >= 0 && y2 < mapHeight)
						{
							unsigned short type = mapData[y2 * mapWidth + x];
							if (type == SPACE_HALLFLOOR || type == SPACE_ROOMFLOOR)
							{
								if (wallAboveFloor)
									mapData[location] = SPACE_LOWERWALL;
								else
									mapData[location] = SPACE_UPPERWALL;
							}
						}
						if (y3 >= 0 && y3 < mapHeight)
						{
							int type1 = mapData[y2 * mapWidth + x];
							int type2 = mapData[y3 * mapWidth + x];
							if (wallAboveFloor && (type1 == SPACE_CEILING || type1 == SPACE_LOWERWALL) && (type2 == SPACE_HALLFLOOR || type2 == SPACE_ROOMFLOOR))
								mapData[location] = SPACE_UPPERWALL;
							else if (!wallAboveFloor && (type1 == SPACE_CEILING || type1 == SPACE_UPPERWALL) && (type2 == SPACE_HALLFLOOR || type2 == SPACE_ROOMFLOOR))
								mapData[location] = SPACE_LOWERWALL;
						}
					}
				}
			}
		}
	}
}

void RandomMapGenerator_DungeonStructure::placeFloors()
{
	int randomNumber, max;
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			if (mapData[y * mapWidth + x] == SPACE_HALLFLOOR)
			{
				max = 0;
				if (cbUseHallFloorB->isChecked())
					++max;
				if (cbUseHallFloorC->isChecked())
					++max;
				randomNumber = (int)(numberGenerator.getRandomNumber(0ul, (unsigned long)(max * 1000)) / 1000ul);
				if (randomNumber == 0)
					fillArea(x, y, SPACE_HALLFLOORA);
				else if (randomNumber == 1 && cbUseHallFloorB->isChecked())
					fillArea(x, y, SPACE_HALLFLOORB);
				else
					fillArea(x, y, SPACE_HALLFLOORC);
			}
			else if (mapData[y * mapWidth + x] == SPACE_ROOMFLOOR)
			{
				max = 0;
				if (cbUseHallFloorB->isChecked())
					++max;
				if (cbUseHallFloorC->isChecked())
					++max;
				randomNumber = (int)(numberGenerator.getRandomNumber(0ul, (unsigned long)(max * 1000)) / 1000ul);
				if (randomNumber == 0)
					fillArea(x, y, SPACE_ROOMFLOORA);
				else if (randomNumber == 1 && cbUseHallFloorB->isChecked())
					fillArea(x, y, SPACE_ROOMFLOORB);
				else
					fillArea(x, y, SPACE_ROOMFLOORC);
			}
		}
	}
}

void RandomMapGenerator_DungeonStructure::fillArea(int x, int y, unsigned short type)
{
	int x2, y2;
	const unsigned short oldType = mapData[y * mapWidth + x];
	mapData[y * mapWidth + x] = type;
	x2 = x;
	y2 = y - 1;
	if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight)
	{
		if (mapData[y2 * mapWidth + x2] == oldType)
			fillArea(x2, y2, type);
	}
	x2 = x - 1;
	y2 = y;
	if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight)
	{
		if (mapData[y2 * mapWidth + x2] == oldType)
			fillArea(x2, y2, type);
	}
	x2 = x + 1;
	y2 = y;
	if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight)
	{
		if (mapData[y2 * mapWidth + x2] == oldType)
			fillArea(x2, y2, type);
	}
	x2 = x;
	y2 = y + 1;
	if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight)
	{
		if (mapData[y2 * mapWidth + x2] == oldType)
			fillArea(x2, y2, type);
	}
}

int RandomMapGenerator_DungeonStructure::numNeighbors(Point point, int type)
{
	Point point2;
	int numNeighbors = 0;
	point2.x = point.x;
	point2.y = point.y - 1;
	if (generatedMap.contains(point2))
	{
		if (generatedMap[point2] == type)
			++numNeighbors;
	}
	point2.x = point.x - 1;
	point2.y = point.y;
	if (generatedMap.contains(point2))
	{
		if (generatedMap[point2] == type)
			++numNeighbors;
	}
	point2.x = point.x + 1;
	point2.y = point.y;
	if (generatedMap.contains(point2))
	{
		if (generatedMap[point2] == type)
			++numNeighbors;
	}
	point2.x = point.x;
	point2.y = point.y + 1;
	if (generatedMap.contains(point2))
	{
		if (generatedMap[point2] == type)
			++numNeighbors;
	}
	return numNeighbors;
}

void RandomMapGenerator_DungeonStructure::cleanDungeon()
{
	QList<Point> pointList;
	bool done = false;
	bool found1 = false;
	while (!done)
	{
		QCoreApplication::processEvents();
		found1 = false;
		for (mapIter = generatedMap.begin(); mapIter != generatedMap.end(); ++mapIter)
		{
			int neighbors = 0;
			unsigned short type = mapIter.value();
			neighbors += numNeighbors(mapIter.key(), SPACE_HALLFLOOR);
			neighbors += numNeighbors(mapIter.key(), SPACE_ROOMFLOOR);
			if (type == SPACE_HALLFLOOR && neighbors <= 1)
			{
				Point point;
				point.x = mapIter.key().x;
				point.y = mapIter.key().y - 1;
				if (generatedMap.contains(point))
				{
					if (generatedMap[point] == SPACE_WALL)
						pointList.append(point);
				}
				point.x = mapIter.key().x - 1;
				point.y = mapIter.key().y;
				if (generatedMap.contains(point))
				{
					if (generatedMap[point] == SPACE_WALL)
						pointList.append(point);
				}
				point.x = mapIter.key().x + 1;
				point.y = mapIter.key().y;
				if (generatedMap.contains(point))
				{
					if (generatedMap[point] == SPACE_WALL)
						pointList.append(point);
				}
				point.x = mapIter.key().x;
				point.y = mapIter.key().y + 1;
				if (generatedMap.contains(point))
				{
					if (generatedMap[point] == SPACE_WALL)
						pointList.append(point);
				}
				pointList.append(mapIter.key());
				found1 = true;
			}
		}
		for (int i = 0; i < pointList.size(); ++i)
			generatedMap.remove(pointList[i]);
		pointList.clear();
		if (!found1)
			done = true;
	}
}

void RandomMapGenerator_DungeonStructure::removeWall(Point point)
{
	bool found = false;
	if (generatedMap.contains(point))
	{
		generatedMap[point] = SPACE_HALLFLOOR;
		for (int i = 0; i < wallList.size() && !found; ++i)
		{
			if (wallList[i] == point)
				wallList.removeAt(i);
		}
	}
}

void RandomMapGenerator_DungeonStructure::removeWall(int index)
{
	if (generatedMap.contains(wallList[index]))
	{
		generatedMap[wallList[index]] = SPACE_HALLFLOOR;
		wallList.removeAt(index);
	}
}
