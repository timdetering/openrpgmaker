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
#include "randommapgenerator_cavernstructure.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "tilechooser.h"
#include "xmlParser.h"

RandomMapGenerator_CavernStructure::RandomMapGenerator_CavernStructure(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	leName->setText("Untitled");
	tbLowerWallTile->setEnabled(false);
	tbUpperWallTile->setEnabled(false);
	tbFloorBTile->setEnabled(false);
	tbFloorCTile->setEnabled(false);
	bWidthSpinButton->setSpinBox(sbWidth);
	bHeightSpinButton->setSpinBox(sbHeight);
	placeFloor = 0.0001;
	placeCeiling = 0.0001;
	sproutChances[SPACE_FLOOR] = 0.000075;
	sproutChances[SPACE_CEILING] = 0.0001;
	growChances[SPACE_FLOOR] = 0.2;
	growChances[SPACE_CEILING] = 0.175;
	mapData1 = NULL;
	mapData2 = NULL;
	configured = false;
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

RandomMapGenerator_CavernStructure::~RandomMapGenerator_CavernStructure()
{
	if (mapData1 != NULL)
	{
		delete[] mapData1;
		mapData1 = NULL;
	}
	if (mapData2 != NULL)
	{
		delete[] mapData2;
		mapData2 = NULL;
	}
}

void RandomMapGenerator_CavernStructure::setupGenerator(XMLNode ConfigurationNode)
{
	XMLNode TempNode;
	sbWidth->setValue(atoi(ConfigurationNode.getAttribute("width")));
	sbHeight->setValue(atoi(ConfigurationNode.getAttribute("height")));
	if (QString(ConfigurationNode.getAttribute("surroundWithWall")) == "true")
		cbSurroundWithWall->setChecked(true);
	else
		cbSurroundWithWall->setChecked(false);
	if (QString(ConfigurationNode.getAttribute("wallPosition")) == "above")
		rbWallAboveFloor->setChecked(true);
	else
		rbWallBelowFloor->setChecked(true);
	TempNode = ConfigurationNode.getChildNode("wallTiles");
	tbCeilingTile->setTile(QString(ConfigurationNode.getAttribute("ceiling")).toUShort(0, 16));
	tbLowerWallTile->setTile(QString(ConfigurationNode.getAttribute("lowerWall")).toUShort(0, 16));
	tbUpperWallTile->setTile(QString(ConfigurationNode.getAttribute("upperWall")).toUShort(0, 16));
	if (QString(ConfigurationNode.getAttribute("useLowerWall")) == "true")
		cbUseLowerWall->setChecked(true);
	else
		cbUseLowerWall->setChecked(false);
	if (QString(ConfigurationNode.getAttribute("useUpperWall")) == "true")
		cbUseUpperWall->setChecked(true);
	else
		cbUseUpperWall->setChecked(false);
	TempNode = ConfigurationNode.getChildNode("floorTiles");
	tbFloorATile->setTile(QString(ConfigurationNode.getAttribute("floorA")).toUShort(0, 16));
	tbFloorBTile->setTile(QString(ConfigurationNode.getAttribute("floorB")).toUShort(0, 16));
	tbFloorCTile->setTile(QString(ConfigurationNode.getAttribute("floorC")).toUShort(0, 16));
	if (QString(ConfigurationNode.getAttribute("useFloorB")) == "true")
		cbUseFloorB->setChecked(true);
	else
		cbUseFloorB->setChecked(false);
	if (QString(ConfigurationNode.getAttribute("useFloorC")) == "true")
		cbUseFloorC->setChecked(true);
	else
		cbUseFloorC->setChecked(false);
}

XMLNode RandomMapGenerator_CavernStructure::getConfigurationNode()
{
	XMLNode TempNode, ConfigurationNode = XMLNode::createXMLTopNode("caverngenerator");
	ConfigurationNode.addAttribute("width", sbWidth->cleanText().toUtf8().data());
	ConfigurationNode.addAttribute("height", sbHeight->cleanText().toUtf8().data());
	ConfigurationNode.addAttribute("surroundWithWall", (cbSurroundWithWall->isChecked()) ? "true":"false");
	ConfigurationNode.addAttribute("wallPosition", (rbWallAboveFloor->isChecked()) ? "above":"below");
	TempNode = ConfigurationNode.addChild("wallTiles");
	TempNode.addAttribute("ceiling", QString("%1").arg(tbCeilingTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("lowerWall", QString("%1").arg(tbLowerWallTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("upperWall", QString("%1").arg(tbUpperWallTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("useLowerWall", (cbUseLowerWall->isChecked()) ? "true":"false");
	TempNode.addAttribute("useUpperWall", (cbUseUpperWall->isChecked()) ? "true":"false");
	TempNode = ConfigurationNode.addChild("floorTiles");
	TempNode.addAttribute("floorA", QString("%1").arg(tbFloorATile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("floorB", QString("%1").arg(tbFloorBTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("floorC", QString("%1").arg(tbFloorCTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	TempNode.addAttribute("useFloorB", (cbUseFloorB->isChecked()) ? "true":"false");
	TempNode.addAttribute("useFloorC", (cbUseFloorC->isChecked()) ? "true":"false");
	return ConfigurationNode;
}

bool RandomMapGenerator_CavernStructure::isConfigured()
{
	return configured;
}

QStringList RandomMapGenerator_CavernStructure::getGeneratedMapNames()
{
	return QStringList() << leName->text();
}

Map *RandomMapGenerator_CavernStructure::getGeneratedMap(int mapNum)
{
	Q_UNUSED(mapNum);
	ProjectData::DataReference<Tileset> *tilesetRef;
	Map::TileData ceilingTile, lowerWallTile, upperWallTile, floorATile, floorBTile, floorCTile;
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
	tile = tbFloorATile->getTile();
	floorATile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	floorATile.tileID = (unsigned char)(tile % 256);
	floorATile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		floorATile.phaseDirections[i] = pTile->getPhaseDirections(i);
		floorATile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbFloorBTile->getTile();
	floorBTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	floorBTile.tileID = (unsigned char)(tile % 256);
	floorBTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		floorBTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		floorBTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbFloorCTile->getTile();
	floorCTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	floorCTile.tileID = (unsigned char)(tile % 256);
	floorCTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		floorCTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		floorCTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	if (mapData1 != NULL)
	{
		Map *generatedMap = new Map;
		generatedMap->setName(leName->text());
		generatedMap->resizeMap(mapWidth, mapHeight);
		generatedMap->setBorderTile(tbCeilingTile->getTile());
		for (int y = 0; y < mapHeight; ++y)
		{
			for (int x = 0; x < mapWidth; ++x)
			{
				int location = y * mapWidth + x;
				if (mapData1[location] == SPACE_CEILING)
					generatedMap->setTileData(x, y, 0, ceilingTile);
				else if (mapData1[location] == SPACE_UPPERWALL)
					generatedMap->setTileData(x, y, 0, upperWallTile);
				else if (mapData1[location] == SPACE_LOWERWALL)
					generatedMap->setTileData(x, y, 0, lowerWallTile);
				else if (mapData1[location] == SPACE_FLOORA)
					generatedMap->setTileData(x, y, 0, floorATile);
				else if (mapData1[location] == SPACE_FLOORB)
					generatedMap->setTileData(x, y, 0, floorBTile);
				else if (mapData1[location] == SPACE_FLOORC)
					generatedMap->setTileData(x, y, 0, floorCTile);
			}
		}
		generatedMap->autoCalcAllTileFlags(0);
		return generatedMap;
	}
	return NULL;
}

void RandomMapGenerator_CavernStructure::generateCavernStructure(unsigned long seed)
{
	Loading *generatingIndicator;
	int phase = 0;
	bool done = false;
	if (cbSurroundWithWall->isChecked())
	{
		sproutChances[SPACE_FLOOR] = 0.000075;
		sproutChances[SPACE_CEILING] = 0.0025;
		growChances[SPACE_FLOOR] = 0.3;
		growChances[SPACE_CEILING] = 0.05;
	}
	else
	{
		sproutChances[SPACE_FLOOR] = 0.000075;
		sproutChances[SPACE_CEILING] = 0.0001;
		growChances[SPACE_FLOOR] = 0.2;
		growChances[SPACE_CEILING] = 0.175;
	}
	generatingIndicator = new Loading("Generating Cavern-Like Structure");
	generatingIndicator->show();
	mapWidth = sbWidth->value();
	mapHeight = sbHeight->value();
	if (mapData1 != NULL)
	{
		delete[] mapData1;
		mapData1 = NULL;
	}
	if (mapData2 != NULL)
	{
		delete[] mapData2;
		mapData2 = NULL;
	}
	mapData1 = new int[mapWidth * mapHeight];
	mapData2 = new int[mapWidth * mapHeight];
	numberGenerator.seedGenerator(seed);
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			if (cbSurroundWithWall->isChecked() && (x == 0 || x == mapWidth - 1 || y == 0 || y == mapHeight - 1))
			{
				mapData1[y * mapWidth + x] = SPACE_CEILING;
				mapData2[y * mapWidth + x] = SPACE_CEILING;
			}
			else
			{
				mapData1[y * mapWidth + x] = SPACE_UNUSED;
				mapData2[y * mapWidth + x] = SPACE_UNUSED;
			}
		}
	}
	sproutChanceFloor = (unsigned long)(sproutChances[SPACE_FLOOR] * 1000000ul * (mapWidth * mapHeight * 0.0001));
	sproutChanceCeiling = (unsigned long)(sproutChances[SPACE_CEILING] * 1000000ul * (mapWidth * mapHeight * .0001));
	growChanceFloor = (unsigned long)(growChances[SPACE_FLOOR] * 1000000ul);
	growChanceCeiling = (unsigned long)(growChances[SPACE_CEILING] * 1000000ul);
	init();
	while (!done)
	{
		QCoreApplication::processEvents();
		if (phase == 0)
		{
			runSprout();
			runGrowth();
			copyMapData();
			if (phase1Done())
				++phase;
		}
		else if (phase == 1)
		{
			placeWalls();
			copyMapData();
			++phase;
		}
		else if (phase == 2)
		{
			runSprout2();
			runGrowth2();
			copyMapData();
			if (phase2Done())
			{
				++phase;
				done = true;
			}
		}
	}
	generatingIndicator->doneLoading();
	delete generatingIndicator;
}

void RandomMapGenerator_CavernStructure::on_cbUseLowerWall_toggled(bool on)
{
	tbLowerWallTile->setEnabled(on);
}

void RandomMapGenerator_CavernStructure::on_cbUseUpperWall_toggled(bool on)
{
	tbUpperWallTile->setEnabled(on);
}

void RandomMapGenerator_CavernStructure::on_cbUseFloorB_toggled(bool on)
{
	tbFloorBTile->setEnabled(on);
}

void RandomMapGenerator_CavernStructure::on_cbUseFloorC_toggled(bool on)
{
	tbFloorCTile->setEnabled(on);
}

void RandomMapGenerator_CavernStructure::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Cavern Generator");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void RandomMapGenerator_CavernStructure::on_buttonBox_accepted()
{
	configured = true;
}

int RandomMapGenerator_CavernStructure::numNeighbors(int x, int y, int type)
{
	int x2, y2, type2, neighbors = 0;
	x2 = x;
	y2 = y - 1;
	if (x2 < 0)
		x2 += mapWidth;
	else if (x2 >= mapWidth)
		x2 -= mapWidth;
	if (y2 < 0)
		y2 += mapHeight;
	else if (y2 >= mapHeight)
		y2 -= mapHeight;
	type2 = mapData1[y2 * mapWidth + x2];
	if (type2 == type)
		++neighbors;
	x2 = x - 1;
	y2 = y;
	if (x2 < 0)
		x2 += mapWidth;
	else if (x2 >= mapWidth)
		x2 -= mapWidth;
	if (y2 < 0)
		y2 += mapHeight;
	else if (y2 >= mapHeight)
		y2 -= mapHeight;
	type2 = mapData1[y2 * mapWidth + x2];
	if (type2 == type)
		++neighbors;
	x2 = x + 1;
	y2 = y;
	if (x2 < 0)
		x2 += mapWidth;
	else if (x2 >= mapWidth)
		x2 -= mapWidth;
	if (y2 < 0)
		y2 += mapHeight;
	else if (y2 >= mapHeight)
		y2 -= mapHeight;
	type2 = mapData1[y2 * mapWidth + x2];
	if (type2 == type)
		++neighbors;
	x2 = x;
	y2 = y + 1;
	if (x2 < 0)
		x2 += mapWidth;
	else if (x2 >= mapWidth)
		x2 -= mapWidth;
	if (y2 < 0)
		y2 += mapHeight;
	else if (y2 >= mapHeight)
		y2 -= mapHeight;
	type2 = mapData1[y2 * mapWidth + x2];
	if (type2 == type)
		++neighbors;
	return neighbors;
}

void RandomMapGenerator_CavernStructure::init()
{
	int numPlaced, numToPlace;
	numPlaced = 0;
	numToPlace = placeFloor * (mapWidth * mapHeight);
	if (numToPlace < 10)
		numToPlace = 10;
	while (numPlaced < numToPlace)
	{
		int x = (int)numberGenerator.getRandomNumber(0ul, (unsigned long)mapWidth);
		int y = (int)numberGenerator.getRandomNumber(0ul, (unsigned long)mapHeight);
		if (mapData1[y * mapWidth + x] == SPACE_UNUSED)
		{
			mapData1[y * mapWidth + x] = SPACE_FLOOR;
			++numPlaced;
		}
	}
	numPlaced = 0;
	numToPlace = placeCeiling * (mapWidth * mapHeight);
	if (numToPlace < 10)
		numToPlace = 10;
	while (numPlaced < numToPlace)
	{
		int x = (int)numberGenerator.getRandomNumber(0ul, (unsigned long)mapWidth);
		int y = (int)numberGenerator.getRandomNumber(0ul, (unsigned long)mapHeight);
		if (mapData1[y * mapWidth + x] == SPACE_UNUSED && numNeighbors(x, y, SPACE_UNUSED) == 4)
		{
			mapData1[y * mapWidth + x] = SPACE_CEILING;
			++numPlaced;
		}
	}
}

void RandomMapGenerator_CavernStructure::runSprout()
{
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int location = y * mapWidth + x;
			if (mapData1[location] == SPACE_UNUSED)
			{
				randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
				if (randomNumber <= sproutChanceFloor)
					mapData2[location] = SPACE_FLOOR;
			}
			if (mapData1[location] == SPACE_UNUSED)
			{
				randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
				if (randomNumber <= sproutChanceCeiling)
					mapData2[location] = SPACE_CEILING;
			}
		}
	}
}

void RandomMapGenerator_CavernStructure::runGrowth()
{
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int location = y * mapWidth + x;
			if (mapData1[location] == SPACE_UNUSED)
			{
				int neighbors = numNeighbors(x, y, SPACE_FLOOR);
				if (neighbors == 4)
					mapData2[location] = SPACE_FLOOR;
				else if (neighbors > 0)
				{
					randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
					if (randomNumber <= growChanceFloor * neighbors)
						mapData2[location] = SPACE_FLOOR;
				}
			}
			if (mapData1[location] == SPACE_UNUSED)
			{
				int neighbors = numNeighbors(x, y, SPACE_CEILING);
				if (neighbors == 4)
					mapData2[location] = SPACE_CEILING;
				else if (neighbors > 0)
				{
					randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
					if (randomNumber <= growChanceCeiling * neighbors)
						mapData2[location] = SPACE_CEILING;
				}
			}
		}
	}
}

void RandomMapGenerator_CavernStructure::runSprout2()
{
	bool useFloorB = cbUseFloorB->isChecked();
	bool useFloorC = cbUseFloorC->isChecked();
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int location = y * mapWidth + x;
			if (mapData1[location] == SPACE_FLOOR && !useFloorB && !useFloorC)
				mapData2[location] = SPACE_FLOORA;
			if (mapData1[location] == SPACE_FLOOR)
			{
				randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
				if (randomNumber <= sproutChanceFloor)
					mapData2[location] = SPACE_FLOORA;
			}
			if (mapData1[location] == SPACE_FLOOR && useFloorB)
			{
				randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
				if (randomNumber <= sproutChanceFloor)
					mapData2[location] = SPACE_FLOORB;
			}
			if (mapData1[location] == SPACE_FLOOR && useFloorC)
			{
				randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
				if (randomNumber <= sproutChanceFloor)
					mapData2[location] = SPACE_FLOORC;
			}
		}
	}
}

void RandomMapGenerator_CavernStructure::runGrowth2()
{
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int location = y * mapWidth + x;
			if (mapData1[location] == SPACE_FLOOR)
			{
				int neighbors = numNeighbors(x, y, SPACE_FLOORA);
				if (neighbors == 4)
					mapData2[location] = SPACE_FLOORA;
				else if (neighbors > 0)
				{
					randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
					if (randomNumber <= growChanceFloor * neighbors)
						mapData2[location] = SPACE_FLOORA;
				}
			}
			if (mapData1[location] == SPACE_FLOOR)
			{
				int neighbors = numNeighbors(x, y, SPACE_FLOORB);
				if (neighbors == 4)
					mapData2[location] = SPACE_FLOORB;
				else if (neighbors > 0)
				{
					randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
					if (randomNumber <= growChanceFloor * neighbors)
						mapData2[location] = SPACE_FLOORB;
				}
			}
			if (mapData1[location] == SPACE_FLOOR)
			{
				int neighbors = numNeighbors(x, y, SPACE_FLOORC);
				if (neighbors == 4)
					mapData2[location] = SPACE_FLOORC;
				else if (neighbors > 0)
				{
					randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
					if (randomNumber <= growChanceFloor * neighbors)
						mapData2[location] = SPACE_FLOORC;
				}
			}
		}
	}
}

void RandomMapGenerator_CavernStructure::placeWalls()
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
				if (mapData1[location] == SPACE_CEILING)
				{
					if (useLowerWall && !useUpperWall)
					{
						if (wallAboveFloor)
							y2 = y + 1;
						else
							y2 = y - 1;
						if (y2 >= 0 && y2 < mapHeight)
						{
							if (mapData1[y2 * mapWidth + x] == SPACE_FLOOR)
								mapData2[location] = SPACE_LOWERWALL;
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
							if (mapData1[y2 * mapWidth + x] == SPACE_FLOOR)
								mapData2[location] = SPACE_UPPERWALL;
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
							if (mapData1[y2 * mapWidth + x] == SPACE_FLOOR)
							{
								if (wallAboveFloor)
									mapData2[location] = SPACE_LOWERWALL;
								else
									mapData2[location] = SPACE_UPPERWALL;
							}
						}
						if (y3 >= 0 && y3 < mapHeight)
						{
							int type1 = mapData1[y2 * mapWidth + x];
							int type2 = mapData1[y3 * mapWidth + x];
							if (wallAboveFloor && (type1 == SPACE_CEILING || type1 == SPACE_LOWERWALL) && type2 == SPACE_FLOOR)
								mapData2[location] = SPACE_UPPERWALL;
							else if (!wallAboveFloor && (type1 == SPACE_CEILING || type1 == SPACE_UPPERWALL) && type2 == SPACE_FLOOR)
								mapData2[location] = SPACE_LOWERWALL;
						}
					}
				}
			}
		}
	}
}

void RandomMapGenerator_CavernStructure::copyMapData()
{
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
			mapData1[y * mapWidth + x] = mapData2[y * mapWidth + x];
	}
}

bool RandomMapGenerator_CavernStructure::phase1Done()
{
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			if (mapData1[y * mapWidth + x] == SPACE_UNUSED)
				return false;
		}
	}
	return true;
}

bool RandomMapGenerator_CavernStructure::phase2Done()
{
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			if (mapData1[y * mapWidth + x] == SPACE_FLOOR)
				return false;
		}
	}
	return true;
}
