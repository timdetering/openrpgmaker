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
#include "randommapgenerator_worldmap.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "tilechooser.h"
#include "xmlParser.h"

RandomMapGenerator_WorldMap::RandomMapGenerator_WorldMap(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	chances[MAP_50x50 + CHANCE_SPROUT_WATER] = 0.000075;
	chances[MAP_50x50 + CHANCE_SPROUT_CONTINENT_TERRAIN] = 0.0001;
	chances[MAP_50x50 + CHANCE_GROW_WATER] = 0.1;
	chances[MAP_50x50 + CHANCE_GROW_CONTINENT_TERRAIN] = 0.2;
	chances[MAP_50x50 + CHANCE_SPROUT_FEW_CONTINENTS] = 0.00005;
	chances[MAP_50x50 + CHANCE_SPROUT_NORMAL_CONTINENTS] = 0.0001;
	chances[MAP_50x50 + CHANCE_SPROUT_LOTS_CONTINENTS] = 0.0002;
	chances[MAP_50x50 + CHANCE_GROW_SMALL_CONTINENTS] = 0.05;
	chances[MAP_50x50 + CHANCE_GROW_MEDIUM_CONTINENTS] = 0.1;
	chances[MAP_50x50 + CHANCE_GROW_LARGE_CONTINENTS] = 0.15;
	chances[MAP_50x50 + CHANCE_SPROUT_FEW_TERRAIN] = 0.000025;
	chances[MAP_50x50 + CHANCE_SPROUT_NORMAL_TERRAIN] = 0.00005;
	chances[MAP_50x50 + CHANCE_SPROUT_LOTS_TERRAIN] = 0.000075;
	chances[MAP_50x50 + CHANCE_GROW_SMALL_TERRAIN] = 0.05;
	chances[MAP_50x50 + CHANCE_GROW_MEDIUM_TERRAIN] = 0.075;
	chances[MAP_50x50 + CHANCE_GROW_LARGE_TERRAIN] = 0.1;
	chances[MAP_100x100 + CHANCE_SPROUT_WATER] = 0.000075;
	chances[MAP_100x100 + CHANCE_SPROUT_CONTINENT_TERRAIN] = 0.0001;
	chances[MAP_100x100 + CHANCE_GROW_WATER] = 0.2;
	chances[MAP_100x100 + CHANCE_GROW_CONTINENT_TERRAIN] = 0.2;
	chances[MAP_100x100 + CHANCE_SPROUT_FEW_CONTINENTS] = 0.00005;
	chances[MAP_100x100 + CHANCE_SPROUT_NORMAL_CONTINENTS] = 0.0001;
	chances[MAP_100x100 + CHANCE_SPROUT_LOTS_CONTINENTS] = 0.0002;
	chances[MAP_100x100 + CHANCE_GROW_SMALL_CONTINENTS] = 0.125;
	chances[MAP_100x100 + CHANCE_GROW_MEDIUM_CONTINENTS] = 0.175;
	chances[MAP_100x100 + CHANCE_GROW_LARGE_CONTINENTS] = 0.225;
	chances[MAP_100x100 + CHANCE_SPROUT_FEW_TERRAIN] = 0.000025;
	chances[MAP_100x100 + CHANCE_SPROUT_NORMAL_TERRAIN] = 0.00005;
	chances[MAP_100x100 + CHANCE_SPROUT_LOTS_TERRAIN] = 0.000075;
	chances[MAP_100x100 + CHANCE_GROW_SMALL_TERRAIN] = 0.05;
	chances[MAP_100x100 + CHANCE_GROW_MEDIUM_TERRAIN] = 0.075;
	chances[MAP_100x100 + CHANCE_GROW_LARGE_TERRAIN] = 0.1;
	chances[MAP_200x200 + CHANCE_SPROUT_WATER] = 0.0001;
	chances[MAP_200x200 + CHANCE_SPROUT_CONTINENT_TERRAIN] = 0.0001;
	chances[MAP_200x200 + CHANCE_GROW_WATER] = 0.1;
	chances[MAP_200x200 + CHANCE_GROW_CONTINENT_TERRAIN] = 0.2;
	chances[MAP_200x200 + CHANCE_SPROUT_FEW_CONTINENTS] = 0.000005;
	chances[MAP_200x200 + CHANCE_SPROUT_NORMAL_CONTINENTS] = 0.00001;
	chances[MAP_200x200 + CHANCE_SPROUT_LOTS_CONTINENTS] = 0.00002;
	chances[MAP_200x200 + CHANCE_GROW_SMALL_CONTINENTS] = 0.2;
	chances[MAP_200x200 + CHANCE_GROW_MEDIUM_CONTINENTS] = 0.3;
	chances[MAP_200x200 + CHANCE_GROW_LARGE_CONTINENTS] = 0.4;
	chances[MAP_200x200 + CHANCE_SPROUT_FEW_TERRAIN] = 0.000025;
	chances[MAP_200x200 + CHANCE_SPROUT_NORMAL_TERRAIN] = 0.00005;
	chances[MAP_200x200 + CHANCE_SPROUT_LOTS_TERRAIN] = 0.000075;
	chances[MAP_200x200 + CHANCE_GROW_SMALL_TERRAIN] = 0.05;
	chances[MAP_200x200 + CHANCE_GROW_MEDIUM_TERRAIN] = 0.075;
	chances[MAP_200x200 + CHANCE_GROW_LARGE_TERRAIN] = 0.1;
	chances[MAP_300x300 + CHANCE_SPROUT_WATER] = 0.00005;
	chances[MAP_300x300 + CHANCE_SPROUT_CONTINENT_TERRAIN] = 0.0001;
	chances[MAP_300x300 + CHANCE_GROW_WATER] = 0.15;
	chances[MAP_300x300 + CHANCE_GROW_CONTINENT_TERRAIN] = 0.2;
	chances[MAP_300x300 + CHANCE_SPROUT_FEW_CONTINENTS] = 0.000005;
	chances[MAP_300x300 + CHANCE_SPROUT_NORMAL_CONTINENTS] = 0.00001;
	chances[MAP_300x300 + CHANCE_SPROUT_LOTS_CONTINENTS] = 0.00002;
	chances[MAP_300x300 + CHANCE_GROW_SMALL_CONTINENTS] = 0.2;
	chances[MAP_300x300 + CHANCE_GROW_MEDIUM_CONTINENTS] = 0.3;
	chances[MAP_300x300 + CHANCE_GROW_LARGE_CONTINENTS] = 0.4;
	chances[MAP_300x300 + CHANCE_SPROUT_FEW_TERRAIN] = 0.000025;
	chances[MAP_300x300 + CHANCE_SPROUT_NORMAL_TERRAIN] = 0.00005;
	chances[MAP_300x300 + CHANCE_SPROUT_LOTS_TERRAIN] = 0.000075;
	chances[MAP_300x300 + CHANCE_GROW_SMALL_TERRAIN] = 0.05;
	chances[MAP_300x300 + CHANCE_GROW_MEDIUM_TERRAIN] = 0.075;
	chances[MAP_300x300 + CHANCE_GROW_LARGE_TERRAIN] = 0.1;
	chances[MAP_400x400 + CHANCE_SPROUT_WATER] = 0.00005;
	chances[MAP_400x400 + CHANCE_SPROUT_CONTINENT_TERRAIN] = 0.0001;
	chances[MAP_400x400 + CHANCE_GROW_WATER] = 0.3;
	chances[MAP_400x400 + CHANCE_GROW_CONTINENT_TERRAIN] = 0.2;
	chances[MAP_400x400 + CHANCE_SPROUT_FEW_CONTINENTS] = 0.000025;
	chances[MAP_400x400 + CHANCE_SPROUT_NORMAL_CONTINENTS] = 0.000075;
	chances[MAP_400x400 + CHANCE_SPROUT_LOTS_CONTINENTS] = 0.0001;
	chances[MAP_400x400 + CHANCE_GROW_SMALL_CONTINENTS] = 0.1;
	chances[MAP_400x400 + CHANCE_GROW_MEDIUM_CONTINENTS] = 0.2;
	chances[MAP_400x400 + CHANCE_GROW_LARGE_CONTINENTS] = 0.3;
	chances[MAP_400x400 + CHANCE_SPROUT_FEW_TERRAIN] = 0.000025;
	chances[MAP_400x400 + CHANCE_SPROUT_NORMAL_TERRAIN] = 0.00005;
	chances[MAP_400x400 + CHANCE_SPROUT_LOTS_TERRAIN] = 0.000075;
	chances[MAP_400x400 + CHANCE_GROW_SMALL_TERRAIN] = 0.05;
	chances[MAP_400x400 + CHANCE_GROW_MEDIUM_TERRAIN] = 0.075;
	chances[MAP_400x400 + CHANCE_GROW_LARGE_TERRAIN] = 0.1;
	minSizes[MINIMUM_WATER_SIZE] = 0.2;
	minSizes[MINIMUM_SMALL_CONTINENT_SIZE] = 0.1;
	minSizes[MINIMUM_MEDIUM_CONTINENT_SIZE] = 0.2;
	minSizes[MINIMUM_LARGE_CONTINENT_SIZE] = 0.3;
	leName->setText("Untitled");
	configured = false;
	cancel = false;
	mapData1 = NULL;
	mapData2 = NULL;
	progressIndicator = NULL;
	bNumContinentsSpinButton->setSpinBox(sbNumContinents);
	bContinentNumberSpinButton->setSpinBox(sbContinentNumber);
	bNumTerrainSpinButton->setSpinBox(sbNumTerrain);
	bTerrainNumberSpinButton->setSpinBox(sbTerrainNumber);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

RandomMapGenerator_WorldMap::~RandomMapGenerator_WorldMap()
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
	if (progressIndicator != NULL)
	{
		progressIndicator->doneLoading();
		delete progressIndicator;
		progressIndicator = NULL;
	}
	if (scanContinents.size() != 0)
	{
		for (int i = 0; i < scanContinents.size(); ++i)
		{
			if (scanContinents[i].land.size() != 0)
				scanContinents[i].land.clear();
		}
		scanContinents.clear();
	}
}

void RandomMapGenerator_WorldMap::setupGenerator(XMLNode ConfigurationNode)
{
	XMLNode ContinentNode, TerrainNode;
	ProjectData::DataReference<Tileset> *tilesetRef;
	Tile *pTile;
	QString temp_string;
	unsigned short tile;
	temp_string = ConfigurationNode.getAttribute("mapSize");
	if (temp_string == "small")
		rbMapSize_Small->setChecked(true);
	else if (temp_string == "medium")
		rbMapSize_Medium->setChecked(true);
	else if (temp_string == "large")
		rbMapSize_Large->setChecked(true);
	else if (temp_string == "extralarge")
		rbMapSize_ExtraLarge->setChecked(true);
	else
		rbMapSize_Huge->setChecked(true);
	tbWaterTile->setTile(QString(ConfigurationNode.getAttribute("waterTile")).toUShort(0, 16));
	tbBlankTile->setTile(QString(ConfigurationNode.getAttribute("blankTile")).toUShort(0, 16));
	numContinents = ConfigurationNode.nChildNode("continent");
	if (numContinents > 5)
		numContinents = 5;
	sbNumContinents->setValue(numContinents);
	for (int i = 0; i < 5; ++i)
	{
		continents[i].sproutChance = 1000000ul;
		continents[i].growChance = 1000000ul;
		if (i < numContinents)
		{
			ContinentNode = ConfigurationNode.getChildNode("continent", i);
			tile = QString(ContinentNode.getAttribute("tile")).toUShort(0, 16);
			continents[i].tileData.tilesetID = (unsigned char)((tile >> 8u) % 256);
			continents[i].tileData.tileID = (unsigned char)(tile % 256);
			continents[i].tileData.tileFlags = 0xff;
			tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
			pTile = (*tilesetRef)->getTile(tile % 256);
			for (int k = 0; k < NUM_PHASES; ++k)
			{
				continents[i].tileData.phaseDirections[k] = pTile->getPhaseDirections(k);
				continents[i].tileData.phasePassability[k] = pTile->getPhasePassability(k);
			}
			delete tilesetRef;
			continents[i].numTerrain = ContinentNode.nChildNode("terrain");
			temp_string = ContinentNode.getAttribute("amount");
			if (temp_string == "few")
				continents[i].amount = AMOUNT_FEW;
			else if (temp_string == "normal")
				continents[i].amount = AMOUNT_NORMAL;
			else
				continents[i].amount = AMOUNT_LOTS;
			temp_string = ContinentNode.getAttribute("size");
			if (temp_string == "small")
				continents[i].size = SIZE_SMALL;
			else if (temp_string == "medium")
				continents[i].size = SIZE_MEDIUM;
			else
				continents[i].size = SIZE_LARGE;
			for (int j = 0; j < 6; ++j)
			{
				continents[i].terrain[j].sproutChance = 1000000ul;
				continents[i].terrain[j].growChance = 1000000ul;
				if (j < continents[i].numTerrain)
				{
					TerrainNode = ContinentNode.getChildNode("terrain", j);
					tile = QString(ContinentNode.getAttribute("tile")).toUShort(0, 16);
					continents[i].terrain[j].tileData.tilesetID = (unsigned char)((tile >> 8u) % 256);
					continents[i].terrain[j].tileData.tileID = (unsigned char)(tile % 256);
					continents[i].terrain[j].tileData.tileFlags = 0xff;
					tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
					pTile = (*tilesetRef)->getTile(tile % 256);
					for (int k = 0; k < NUM_PHASES; ++k)
					{
						continents[i].terrain[j].tileData.phaseDirections[k] = pTile->getPhaseDirections(k);
						continents[i].terrain[j].tileData.phasePassability[k] = pTile->getPhasePassability(k);
					}
					delete tilesetRef;
					temp_string = TerrainNode.getAttribute("amount");
					if (temp_string == "few")
						continents[i].terrain[j].amount = AMOUNT_FEW;
					else if (temp_string == "normal")
						continents[i].terrain[j].amount = AMOUNT_NORMAL;
					else
						continents[i].terrain[j].amount = AMOUNT_LOTS;
					temp_string = TerrainNode.getAttribute("size");
					if (temp_string == "small")
						continents[i].terrain[j].size = SIZE_SMALL;
					else if (temp_string == "medium")
						continents[i].terrain[j].size = SIZE_MEDIUM;
					else
						continents[i].terrain[j].size = SIZE_LARGE;
				}
				else
				{
					continents[i].terrain[j].tileData.tilesetID = 0x0;
					continents[i].terrain[j].tileData.tileID = 0x0;
					continents[i].terrain[j].tileData.tileFlags = 0x0;
					for (int k = 0; k < NUM_PHASES; ++k)
					{
						continents[i].terrain[j].tileData.phaseDirections[k] = 0x0;
						continents[i].terrain[j].tileData.phasePassability[k] = false;
					}
					continents[i].terrain[j].amount = AMOUNT_NORMAL;
					continents[i].terrain[j].size = SIZE_MEDIUM;
				}
			}
		}
		else
		{
			continents[i].tileData.tilesetID = 0x0;
			continents[i].tileData.tileID = 0x0;
			continents[i].tileData.tileFlags = 0x0;
			for (int k = 0; k < NUM_PHASES; ++k)
			{
				continents[i].tileData.phaseDirections[k] = 0x0;
				continents[i].tileData.phasePassability[k] = false;
			}
			continents[i].numTerrain = 0;
			continents[i].amount = AMOUNT_NORMAL;
			continents[i].size = SIZE_MEDIUM;
			for (int j = 0; j < 6; ++j)
			{
				continents[i].terrain[j].tileData.tilesetID = 0x0;
				continents[i].terrain[j].tileData.tileID = 0x0;
				continents[i].terrain[j].tileData.tileFlags = 0x0;
				for (int k = 0; k < NUM_PHASES; ++k)
				{
					continents[i].terrain[j].tileData.phaseDirections[k] = 0x0;
					continents[i].terrain[j].tileData.phasePassability[k] = false;
				}
				continents[i].terrain[j].sproutChance = 1000000ul;
				continents[i].terrain[j].growChance = 1000000ul;
				continents[i].terrain[j].amount = AMOUNT_NORMAL;
				continents[i].terrain[j].size = SIZE_MEDIUM;
			}
		}
	}
	sbContinentNumber->setValue(1);
	tbContinentTile->setTile(((unsigned short)(continents[0].tileData.tilesetID) << 8) + (unsigned short)(continents[0].tileData.tileID));
	sbNumTerrain->setValue(continents[0].numTerrain);
	if (continents[0].amount == AMOUNT_FEW)
		rbCAmount_Few->setChecked(true);
	else if (continents[0].amount == AMOUNT_NORMAL)
		rbCAmount_Normal->setChecked(true);
	else
		rbCAmount_Lots->setChecked(true);
	if (continents[0].size == SIZE_SMALL)
		rbCSize_Small->setChecked(true);
	else if (continents[0].size == SIZE_MEDIUM)
		rbCSize_Medium->setChecked(true);
	else
		rbCSize_Large->setChecked(true);
	if (continents[0].numTerrain == 0)
		sbTerrainNumber->setValue(0);
	else
		sbTerrainNumber->setValue(1);
	tbTerrainTile->setTile(((unsigned short)(continents[0].terrain[0].tileData.tilesetID) << 8) + (unsigned long)(continents[0].terrain[0].tileData.tileID));
	if (continents[0].terrain[0].amount == AMOUNT_FEW)
		rbTAmount_Few->setChecked(true);
	else if (continents[0].terrain[0].amount == AMOUNT_NORMAL)
		rbTAmount_Normal->setChecked(true);
	else
		rbTAmount_Lots->setChecked(true);
	if (continents[0].terrain[0].size == SIZE_SMALL)
		rbTSize_Small->setChecked(true);
	else if (continents[0].terrain[0].size == SIZE_MEDIUM)
		rbTSize_Medium->setChecked(true);
	else
		rbTSize_Large->setChecked(true);
	waterSproutChance = 1000000ul;
	waterGrowChance = 1000000ul;
	cTerrainSproutChance = 1000000ul;
	cTerrainGrowChance = 1000000ul;
}

XMLNode RandomMapGenerator_WorldMap::getConfigurationNode()
{
	XMLNode ContinentNode, TerrainNode, ConfigurationNode = XMLNode::createXMLTopNode("worldmapgenerator");
	if (rbMapSize_Small->isChecked())
		ConfigurationNode.addAttribute("mapSize", "small");
	else if (rbMapSize_Medium->isChecked())
		ConfigurationNode.addAttribute("mapSize", "medium");
	else if (rbMapSize_Large->isChecked())
		ConfigurationNode.addAttribute("mapSize", "large");
	else if (rbMapSize_ExtraLarge->isChecked())
		ConfigurationNode.addAttribute("mapSize", "extralarge");
	else
		ConfigurationNode.addAttribute("mapSize", "huge");
	ConfigurationNode.addAttribute("waterTile", QString("%1").arg(tbWaterTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	ConfigurationNode.addAttribute("blankTile", QString("%1").arg(tbBlankTile->getTile(), 4, 16, QChar('0')).toUtf8().data());
	for (int i = 0; i < numContinents; ++i)
	{
		ContinentNode = ConfigurationNode.addChild("continent");
		ContinentNode.addAttribute("tile", QString("%1").arg(((unsigned short)(continents[i].tileData.tilesetID) << 8) + (unsigned short)(continents[i].tileData.tileID), 4, 16, QChar('0')).toUtf8().data());
		if (continents[i].amount == AMOUNT_FEW)
			ContinentNode.addAttribute("amount", "few");
		else if (continents[i].amount == AMOUNT_NORMAL)
			ContinentNode.addAttribute("amount", "normal");
		else
			ContinentNode.addAttribute("amount", "lots");
		if (continents[i].size == SIZE_SMALL)
			ContinentNode.addAttribute("size", "small");
		else if (continents[i].size == SIZE_MEDIUM)
			ContinentNode.addAttribute("size", "medium");
		else
			ContinentNode.addAttribute("size", "large");
		for (int j = 0; j < continents[i].numTerrain; ++j)
		{
			TerrainNode = ContinentNode.addChild("terrain");
			TerrainNode.addAttribute("tile", QString("%1").arg(((unsigned short)(continents[i].terrain[j].tileData.tilesetID) << 8) + (unsigned long)(continents[i].terrain[j].tileData.tileID), 4, 16, QChar('0')).toUtf8().data());
			if (continents[i].terrain[j].amount == AMOUNT_FEW)
				TerrainNode.addAttribute("amount", "few");
			else if (continents[i].terrain[j].amount == AMOUNT_NORMAL)
				TerrainNode.addAttribute("amount", "normal");
			else
				TerrainNode.addAttribute("amount", "lots");
			if (continents[i].terrain[j].size == SIZE_SMALL)
				TerrainNode.addAttribute("size", "small");
			else if (continents[i].terrain[j].size == SIZE_MEDIUM)
				TerrainNode.addAttribute("size", "medium");
			else
				TerrainNode.addAttribute("size", "large");
		}
	}
	return ConfigurationNode;
}

bool RandomMapGenerator_WorldMap::isConfigured()
{
	return configured;
}

bool RandomMapGenerator_WorldMap::canceled()
{
	return cancel;
}

QStringList RandomMapGenerator_WorldMap::getGeneratedMapNames()
{
	return QStringList() << leName->text();
}

Map *RandomMapGenerator_WorldMap::getGeneratedMap(int mapNum)
{
	Q_UNUSED(mapNum);
	ProjectData::DataReference<Tileset> *tilesetRef;
	Map::TileData blankTile, waterTile;
	Tile *pTile;
	unsigned short tile = tbBlankTile->getTile();
	blankTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	blankTile.tileID = (unsigned char)(tile % 256);
	blankTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		blankTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		blankTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	tile = tbWaterTile->getTile();
	waterTile.tilesetID = (unsigned char)((tile >> 8u) % 256);
	waterTile.tileID = (unsigned char)(tile % 256);
	waterTile.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((tile >> 8u) % 256, __FILE__, __LINE__);
	pTile = (*tilesetRef)->getTile(tile % 256);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		waterTile.phaseDirections[i] = pTile->getPhaseDirections(i);
		waterTile.phasePassability[i] = pTile->getPhasePassability(i);
	}
	delete tilesetRef;
	if (mapData1 != NULL)
	{
		Map *generatedMap = new Map;
		generatedMap->addLayer("Terrain Layer");
		generatedMap->setName(leName->text());
		generatedMap->resizeMap(mapSize, mapSize);
		generatedMap->setHorizontalMode(Map::MODE_WRAPAROUND);
		generatedMap->setVerticalMode(Map::MODE_WRAPAROUND);
		generatedMap->setBorderTile(tbWaterTile->getTile());
		for (int y = 0; y < mapSize; ++y)
		{
			for (int x = 0; x < mapSize; ++x)
			{
				int location = y * mapSize + x;
				int continent = mapData1[location].continent;
				int terrain = mapData1[location].terrain;
				generatedMap->setTileData(x, y, 0, blankTile);
				generatedMap->setTileData(x, y, 1, blankTile);
				if (continent == 0 && terrain == 0)
					generatedMap->setTileData(x, y, 0, waterTile);
				else if (continent != 0 && terrain == 0)
					generatedMap->setTileData(x, y, 0, continents[continent - 1].tileData);
				else
				{
					generatedMap->setTileData(x, y, 0, continents[continent - 1].tileData);
					generatedMap->setTileData(x, y, 1, continents[continent - 1].terrain[terrain - 1].tileData);
				}
			}
		}
		generatedMap->autoCalcAllTileFlags(0);
		generatedMap->autoCalcAllTileFlags(1);
		return generatedMap;
	}
	return NULL;
}

void RandomMapGenerator_WorldMap::generateWorldMap(unsigned long seed)
{
	int phase = 0;
	bool done = false;
	bool runCleanPhase = false;
	cancel = false;
	progressIndicator = new DualProgressIndicator("Generating World Map");
	progressIndicator->setOverallProgressBarRange(0, 6);
	progressIndicator->setOverallProgressBarValue(phase);
	progressIndicator->setCurrentProcessTitle("Setting up generator");
	QObject::connect(progressIndicator, SIGNAL(processCanceled()), this, SLOT(cancelGenerator()));
	progressIndicator->show();
	waterSproutChance = 1000000ul;
	waterGrowChance = 1000000ul;
	cTerrainSproutChance = 1000000ul;
	cTerrainGrowChance = 1000000ul;
	for (int i = 0; i < 5; ++i)
	{
		continents[i].sproutChance = 1000000ul;
		continents[i].growChance = 1000000ul;
		for (int j = 0; j < 6; ++j)
		{
			continents[i].terrain[j].sproutChance = 1000000ul;
			continents[i].terrain[j].growChance = 1000000ul;
		}
	}
	if (rbMapSize_Small->isChecked())
	{
		waterSproutChance *= chances[MAP_50x50 + CHANCE_SPROUT_WATER];
		waterGrowChance *= chances[MAP_50x50 + CHANCE_GROW_WATER];
		cTerrainSproutChance *= chances[MAP_50x50 + CHANCE_SPROUT_CONTINENT_TERRAIN];
		cTerrainGrowChance *= chances[MAP_50x50 + CHANCE_GROW_CONTINENT_TERRAIN];
		mapSize = 50;
		for (int i = 0; i < numContinents; ++i)
		{
			Continent *pContinent = &(continents[i]);
			if (pContinent->amount == AMOUNT_FEW)
				pContinent->sproutChance *= chances[MAP_50x50 + CHANCE_SPROUT_FEW_CONTINENTS];
			else if (pContinent->amount == AMOUNT_NORMAL)
				pContinent->sproutChance *= chances[MAP_50x50 + CHANCE_SPROUT_NORMAL_CONTINENTS];
			else
				pContinent->sproutChance *= chances[MAP_50x50 + CHANCE_SPROUT_LOTS_CONTINENTS];
			if (pContinent->size == SIZE_SMALL)
			{
				pContinent->growChance *= chances[MAP_50x50 + CHANCE_GROW_SMALL_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_SMALL_CONTINENT_SIZE];
			}
			else if (pContinent->size == SIZE_MEDIUM)
			{
				pContinent->growChance *= chances[MAP_50x50 + CHANCE_GROW_MEDIUM_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_MEDIUM_CONTINENT_SIZE];
			}
			else
			{
				pContinent->growChance *= chances[MAP_50x50 + CHANCE_GROW_LARGE_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_LARGE_CONTINENT_SIZE];
			}
			for (int j = 0; j < pContinent->numTerrain; ++j)
			{
				Terrain *pTerrain = &(pContinent->terrain[j]);
				if (pTerrain->amount == AMOUNT_FEW)
					pTerrain->sproutChance *= chances[MAP_50x50 + CHANCE_SPROUT_FEW_TERRAIN];
				else if (pTerrain->amount == AMOUNT_NORMAL)
					pTerrain->sproutChance *= chances[MAP_50x50 + CHANCE_SPROUT_NORMAL_TERRAIN];
				else
					pTerrain->sproutChance *= chances[MAP_50x50 + CHANCE_SPROUT_LOTS_TERRAIN];
				if (pTerrain->size == SIZE_SMALL)
					pTerrain->growChance *= chances[MAP_50x50 + CHANCE_GROW_SMALL_TERRAIN];
				else if (pTerrain->size == SIZE_MEDIUM)
					pTerrain->growChance *= chances[MAP_50x50 + CHANCE_GROW_MEDIUM_TERRAIN];
				else
					pTerrain->growChance *= chances[MAP_50x50 + CHANCE_GROW_LARGE_TERRAIN];
			}
		}
	}
	else if (rbMapSize_Medium->isChecked())
	{
		waterSproutChance *= chances[MAP_100x100 + CHANCE_SPROUT_WATER];
		waterGrowChance *= chances[MAP_100x100 + CHANCE_GROW_WATER];
		cTerrainSproutChance *= chances[MAP_100x100 + CHANCE_SPROUT_CONTINENT_TERRAIN];
		cTerrainGrowChance *= chances[MAP_100x100 + CHANCE_GROW_CONTINENT_TERRAIN];
		mapSize = 100;
		for (int i = 0; i < numContinents; ++i)
		{
			Continent *pContinent = &(continents[i]);
			if (pContinent->amount == AMOUNT_FEW)
				pContinent->sproutChance *= chances[MAP_100x100 + CHANCE_SPROUT_FEW_CONTINENTS];
			else if (pContinent->amount == AMOUNT_NORMAL)
				pContinent->sproutChance *= chances[MAP_100x100 + CHANCE_SPROUT_NORMAL_CONTINENTS];
			else
				pContinent->sproutChance *= chances[MAP_100x100 + CHANCE_SPROUT_LOTS_CONTINENTS];
			if (pContinent->size == SIZE_SMALL)
			{
				pContinent->growChance *= chances[MAP_100x100 + CHANCE_GROW_SMALL_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_SMALL_CONTINENT_SIZE];
			}
			else if (pContinent->size == SIZE_MEDIUM)
			{
				pContinent->growChance *= chances[MAP_100x100 + CHANCE_GROW_MEDIUM_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_MEDIUM_CONTINENT_SIZE];
			}
			else
			{
				pContinent->growChance *= chances[MAP_100x100 + CHANCE_GROW_LARGE_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_LARGE_CONTINENT_SIZE];
			}
			for (int j = 0; j < pContinent->numTerrain; ++j)
			{
				Terrain *pTerrain = &(pContinent->terrain[j]);
				if (pTerrain->amount == AMOUNT_FEW)
					pTerrain->sproutChance *= chances[MAP_100x100 + CHANCE_SPROUT_FEW_TERRAIN];
				else if (pTerrain->amount == AMOUNT_NORMAL)
					pTerrain->sproutChance *= chances[MAP_100x100 + CHANCE_SPROUT_NORMAL_TERRAIN];
				else
					pTerrain->sproutChance *= chances[MAP_100x100 + CHANCE_SPROUT_LOTS_TERRAIN];
				if (pTerrain->size == SIZE_SMALL)
					pTerrain->growChance *= chances[MAP_100x100 + CHANCE_GROW_SMALL_TERRAIN];
				else if (pTerrain->size == SIZE_MEDIUM)
					pTerrain->growChance *= chances[MAP_100x100 + CHANCE_GROW_MEDIUM_TERRAIN];
				else
					pTerrain->growChance *= chances[MAP_100x100 + CHANCE_GROW_LARGE_TERRAIN];
			}
		}
	}
	else if (rbMapSize_Large->isChecked())
	{
		waterSproutChance *= chances[MAP_200x200 + CHANCE_SPROUT_WATER];
		waterGrowChance *= chances[MAP_200x200 + CHANCE_GROW_WATER];
		cTerrainSproutChance *= chances[MAP_200x200 + CHANCE_SPROUT_CONTINENT_TERRAIN];
		cTerrainGrowChance *= chances[MAP_200x200 + CHANCE_GROW_CONTINENT_TERRAIN];
		mapSize = 200;
		for (int i = 0; i < numContinents; ++i)
		{
			Continent *pContinent = &(continents[i]);
			if (pContinent->amount == AMOUNT_FEW)
				pContinent->sproutChance *= chances[MAP_200x200 + CHANCE_SPROUT_FEW_CONTINENTS];
			else if (pContinent->amount == AMOUNT_NORMAL)
				pContinent->sproutChance *= chances[MAP_200x200 + CHANCE_SPROUT_NORMAL_CONTINENTS];
			else
				pContinent->sproutChance *= chances[MAP_200x200 + CHANCE_SPROUT_LOTS_CONTINENTS];
			if (pContinent->size == SIZE_SMALL)
			{
				pContinent->growChance *= chances[MAP_200x200 + CHANCE_GROW_SMALL_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_SMALL_CONTINENT_SIZE];
			}
			else if (pContinent->size == SIZE_MEDIUM)
			{
				pContinent->growChance *= chances[MAP_200x200 + CHANCE_GROW_MEDIUM_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_MEDIUM_CONTINENT_SIZE];
			}
			else
			{
				pContinent->growChance *= chances[MAP_200x200 + CHANCE_GROW_LARGE_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_LARGE_CONTINENT_SIZE];
			}
			for (int j = 0; j < pContinent->numTerrain; ++j)
			{
				Terrain *pTerrain = &(pContinent->terrain[j]);
				if (pTerrain->amount == AMOUNT_FEW)
					pTerrain->sproutChance *= chances[MAP_200x200 + CHANCE_SPROUT_FEW_TERRAIN];
				else if (pTerrain->amount == AMOUNT_NORMAL)
					pTerrain->sproutChance *= chances[MAP_200x200 + CHANCE_SPROUT_NORMAL_TERRAIN];
				else
					pTerrain->sproutChance *= chances[MAP_200x200 + CHANCE_SPROUT_LOTS_TERRAIN];
				if (pTerrain->size == SIZE_SMALL)
					pTerrain->growChance *= chances[MAP_200x200 + CHANCE_GROW_SMALL_TERRAIN];
				else if (pTerrain->size == SIZE_MEDIUM)
					pTerrain->growChance *= chances[MAP_200x200 + CHANCE_GROW_MEDIUM_TERRAIN];
				else
					pTerrain->growChance *= chances[MAP_200x200 + CHANCE_GROW_LARGE_TERRAIN];
			}
		}
	}
	else if (rbMapSize_ExtraLarge->isChecked())
	{
		waterSproutChance *= chances[MAP_300x300 + CHANCE_SPROUT_WATER];
		waterGrowChance *= chances[MAP_300x300 + CHANCE_GROW_WATER];
		cTerrainSproutChance *= chances[MAP_300x300 + CHANCE_SPROUT_CONTINENT_TERRAIN];
		cTerrainGrowChance *= chances[MAP_300x300 + CHANCE_GROW_CONTINENT_TERRAIN];
		mapSize = 300;
		for (int i = 0; i < numContinents; ++i)
		{
			Continent *pContinent = &(continents[i]);
			if (pContinent->amount == AMOUNT_FEW)
				pContinent->sproutChance *= chances[MAP_300x300 + CHANCE_SPROUT_FEW_CONTINENTS];
			else if (pContinent->amount == AMOUNT_NORMAL)
				pContinent->sproutChance *= chances[MAP_300x300 + CHANCE_SPROUT_NORMAL_CONTINENTS];
			else
				pContinent->sproutChance *= chances[MAP_300x300 + CHANCE_SPROUT_LOTS_CONTINENTS];
			if (pContinent->size == SIZE_SMALL)
			{
				pContinent->growChance *= chances[MAP_300x300 + CHANCE_GROW_SMALL_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_SMALL_CONTINENT_SIZE];
			}
			else if (pContinent->size == SIZE_MEDIUM)
			{
				pContinent->growChance *= chances[MAP_300x300 + CHANCE_GROW_MEDIUM_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_MEDIUM_CONTINENT_SIZE];
			}
			else
			{
				pContinent->growChance *= chances[MAP_300x300 + CHANCE_GROW_LARGE_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_LARGE_CONTINENT_SIZE];
			}
			for (int j = 0; j < pContinent->numTerrain; ++j)
			{
				Terrain *pTerrain = &(pContinent->terrain[j]);
				if (pTerrain->amount == AMOUNT_FEW)
					pTerrain->sproutChance *= chances[MAP_300x300 + CHANCE_SPROUT_FEW_TERRAIN];
				else if (pTerrain->amount == AMOUNT_NORMAL)
					pTerrain->sproutChance *= chances[MAP_300x300 + CHANCE_SPROUT_NORMAL_TERRAIN];
				else
					pTerrain->sproutChance *= chances[MAP_300x300 + CHANCE_SPROUT_LOTS_TERRAIN];
				if (pTerrain->size == SIZE_SMALL)
					pTerrain->growChance *= chances[MAP_300x300 + CHANCE_GROW_SMALL_TERRAIN];
				else if (pTerrain->size == SIZE_MEDIUM)
					pTerrain->growChance *= chances[MAP_300x300 + CHANCE_GROW_MEDIUM_TERRAIN];
				else
					pTerrain->growChance *= chances[MAP_300x300 + CHANCE_GROW_LARGE_TERRAIN];
			}
		}
	}
	else if (rbMapSize_Huge->isChecked())
	{
		waterSproutChance *= chances[MAP_400x400 + CHANCE_SPROUT_WATER];
		waterGrowChance *= chances[MAP_400x400 + CHANCE_GROW_WATER];
		cTerrainSproutChance *= chances[MAP_400x400 + CHANCE_SPROUT_CONTINENT_TERRAIN];
		cTerrainGrowChance *= chances[MAP_400x400 + CHANCE_GROW_CONTINENT_TERRAIN];
		mapSize = 400;
		for (int i = 0; i < numContinents; ++i)
		{
			Continent *pContinent = &(continents[i]);
			if (pContinent->amount == AMOUNT_FEW)
				pContinent->sproutChance *= chances[MAP_400x400 + CHANCE_SPROUT_FEW_CONTINENTS];
			else if (pContinent->amount == AMOUNT_NORMAL)
				pContinent->sproutChance *= chances[MAP_400x400 + CHANCE_SPROUT_NORMAL_CONTINENTS];
			else
				pContinent->sproutChance *= chances[MAP_400x400 + CHANCE_SPROUT_LOTS_CONTINENTS];
			if (pContinent->size == SIZE_SMALL)
			{
				pContinent->growChance *= chances[MAP_400x400 + CHANCE_GROW_SMALL_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_SMALL_CONTINENT_SIZE];
			}
			else if (pContinent->size == SIZE_MEDIUM)
			{
				pContinent->growChance *= chances[MAP_400x400 + CHANCE_GROW_MEDIUM_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_MEDIUM_CONTINENT_SIZE];
			}
			else
			{
				pContinent->growChance *= chances[MAP_400x400 + CHANCE_GROW_LARGE_CONTINENTS];
				pContinent->minimumSize = mapSize * minSizes[MINIMUM_LARGE_CONTINENT_SIZE];
			}
			for (int j = 0; j < pContinent->numTerrain; ++j)
			{
				Terrain *pTerrain = &(pContinent->terrain[j]);
				if (pTerrain->amount == AMOUNT_FEW)
					pTerrain->sproutChance *= chances[MAP_400x400 + CHANCE_SPROUT_FEW_TERRAIN];
				else if (pTerrain->amount == AMOUNT_NORMAL)
					pTerrain->sproutChance *= chances[MAP_400x400 + CHANCE_SPROUT_NORMAL_TERRAIN];
				else
					pTerrain->sproutChance *= chances[MAP_400x400 + CHANCE_SPROUT_LOTS_TERRAIN];
				if (pTerrain->size == SIZE_SMALL)
					pTerrain->growChance *= chances[MAP_400x400 + CHANCE_GROW_SMALL_TERRAIN];
				else if (pTerrain->size == SIZE_MEDIUM)
					pTerrain->growChance *= chances[MAP_400x400 + CHANCE_GROW_MEDIUM_TERRAIN];
				else
					pTerrain->growChance *= chances[MAP_400x400 + CHANCE_GROW_LARGE_TERRAIN];
			}
		}
	}
	minimumWaterSize = mapSize * minSizes[MINIMUM_WATER_SIZE];
	numberGenerator.seedGenerator(seed);
	if (mapData1 != NULL)
	{
		delete mapData1;
		mapData1 = NULL;
	}
	if (mapData2 != NULL)
	{
		delete mapData2;
		mapData2 = NULL;
	}
	mapData1 = new MapData[mapSize * mapSize];
	mapData2 = new MapData[mapSize * mapSize];
	for (int y = 0; y < mapSize; ++y)
	{
		for (int x = 0; x < mapSize; ++x)
		{
			int location = y * mapSize + x;
			mapData1[location].continent = 0;
			mapData1[location].terrain = 0;
			mapData1[location].unused = true;
			mapData2[location].continent = 0;
			mapData2[location].terrain = 0;
			mapData2[location].unused = true;
		}
	}
	init();
	++phase;
	progressIndicator->setOverallProgressBarValue(phase);
	progressIndicator->setCurrentProcessProgressBarRange(0, mapSize * mapSize);
	progressIndicator->setCurrentProcessProgressBarValue(0);
	progressIndicator->setCurrentProcessTitle("Generating water and continents.");
	runCleanPhase = true;
	while (!done && !cancel)
	{
		if (phase == 1 && !cancel)
		{
			runInitPhase1();
			runGrowPhase1();
			if (phase1Done())
			{
				++phase;
				progressIndicator->setOverallProgressBarValue(phase);
				progressIndicator->setCurrentProcessProgressBarRange(0, 0);
				progressIndicator->setCurrentProcessProgressBarValue(0);
				progressIndicator->setCurrentProcessTitle("Cleaning irregular shapes from continents.");
			}
		}
		else if (phase == 2 && !cancel)
		{
			if (runCleanPhase)
			{
				if (cleanPhase1())
				{
					++phase;
					progressIndicator->setOverallProgressBarValue(phase);
					progressIndicator->setCurrentProcessProgressBarRange(0, mapSize * mapSize);
					progressIndicator->setCurrentProcessProgressBarValue(0);
					progressIndicator->setCurrentProcessTitle("Scanning for, and removing, continents and water masses that are too small.");
				}
				else
					runCleanPhase = false;
			}
			else
			{
				runInitPhase1();
				runGrowPhase1();
				if (phase1Done(false))
					runCleanPhase = true;
			}
		}
		else if (phase == 3 && !cancel)
		{
			int numUsedSpace = 0;
			cleanPhase2();
			if (!cancel)
			{
				++phase;
				for (int y = 0; y < mapSize; ++y)
				{
					for (int x = 0; x < mapSize; ++x)
					{
						int location = y * mapSize + x;
						if (!(mapData1[location].unused) || mapData1[location].continent != 0 || mapData1[location].terrain != 0)
							++numUsedSpace;
					}
				}
				progressIndicator->setOverallProgressBarValue(phase);
				progressIndicator->setCurrentProcessProgressBarRange(numUsedSpace, mapSize * mapSize);
				progressIndicator->setCurrentProcessProgressBarValue(numUsedSpace);
				progressIndicator->setCurrentProcessTitle("Growing remaining water and continent masses to fill the resulting gap.");
			}
		}
		else if (phase == 4 && !cancel)
		{
			int numWaterSpaces = 0;
			runGrowPhase1();
			if (phase1Done())
			{
				++phase;
				for (int y = 0; y < mapSize; ++y)
				{
					for (int x = 0; x < mapSize; ++x)
					{
						int location = y * mapSize + x;
						//if (mapData1[y * mapSize + x] == 0)
						if (mapData1[location].continent == 0 && mapData1[location].terrain == 0 && !(mapData1[location].unused))
							++numWaterSpaces;
					}
				}
				progressIndicator->setOverallProgressBarValue(phase);
				progressIndicator->setCurrentProcessProgressBarRange(numWaterSpaces, mapSize * mapSize);
				progressIndicator->setCurrentProcessProgressBarValue(numWaterSpaces);
				progressIndicator->setCurrentProcessTitle("Generating terrain.");
			}
		}
		else if (phase == 5 && !cancel)
		{
			runInitPhase2();
			runGrowPhase2();
			if (phase2Done())
			{
				++phase;
				done = true;
				progressIndicator->setOverallProgressBarValue(phase);
				progressIndicator->setCurrentProcessProgressBarRange(0, 0);
				progressIndicator->setCurrentProcessProgressBarValue(0);
				progressIndicator->setCurrentProcessTitle("Done");
			}
		}
		for (int y = 0; y < mapSize && !cancel; ++y)
		{
			for (int x = 0; x < mapSize && !cancel; ++x)
			{
				int location = y * mapSize + x;
				mapData1[location].continent = mapData2[location].continent;
				mapData1[location].terrain = mapData2[location].terrain;
				mapData1[location].unused = mapData2[location].unused;
			}
		}
	}
	if (!cancel)
		progressIndicator->doneLoading();
	delete progressIndicator;
	progressIndicator = NULL;
}

void RandomMapGenerator_WorldMap::on_sbNumContinents_valueChanged(int value)
{
	numContinents = value;
	sbContinentNumber->setMaximum(value);
}

void RandomMapGenerator_WorldMap::on_sbContinentNumber_valueChanged(int value)
{
	sbNumTerrain->setValue(continents[value - 1].numTerrain);
	tbContinentTile->setTile(((unsigned short)(continents[value - 1].tileData.tilesetID) << 8) + (unsigned long)(continents[value - 1].tileData.tileID));
	if (continents[value - 1].amount == AMOUNT_FEW)
		rbCAmount_Few->setChecked(true);
	else if (continents[value - 1].amount == AMOUNT_NORMAL)
		rbCAmount_Normal->setChecked(true);
	else
		rbCAmount_Lots->setChecked(true);
	if (continents[value - 1].size == SIZE_SMALL)
		rbCSize_Small->setChecked(true);
	else if (continents[value - 1].size == SIZE_MEDIUM)
		rbCSize_Medium->setChecked(true);
	else
		rbCSize_Large->setChecked(true);
}

void RandomMapGenerator_WorldMap::on_sbNumTerrain_valueChanged(int value)
{
	continents[sbContinentNumber->value() - 1].numTerrain = value;
	if (value == 0)
	{
		sbTerrainNumber->setMinimum(0);
		sbTerrainNumber->setMaximum(0);
		sbTerrainNumber->setValue(0);
		sbTerrainNumber->setSpecialValueText("N/A");
	}
	else
	{
		sbTerrainNumber->setMinimum(1);
		sbTerrainNumber->setMaximum(value);
		sbTerrainNumber->setSpecialValueText("");
	}
}

void RandomMapGenerator_WorldMap::on_tbContinentTile_tileChanged(unsigned short value)
{
	ProjectData::DataReference<Tileset> *tilesetRef;
	int continent = sbContinentNumber->value() - 1;
	continents[continent].tileData.tilesetID = (unsigned char)((value >> 8u) % 256);
	continents[continent].tileData.tileID = (unsigned char)(value % 256);
	continents[continent].tileData.tileFlags = 0xff;
	tilesetRef = new ProjectData::DataReference<Tileset>((value >> 8u) % 256, __FILE__, __LINE__);
	for (int i = 0; i < 32; ++i)
		continents[continent].tileData.phasePassability[i] = (*tilesetRef)->getTile(value % 256)->getPhasePassability(i);
	delete tilesetRef;
}

void RandomMapGenerator_WorldMap::on_rbCAmount_Few_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].amount = AMOUNT_FEW;
}

void RandomMapGenerator_WorldMap::on_rbCAmount_Normal_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].amount = AMOUNT_NORMAL;
}

void RandomMapGenerator_WorldMap::on_rbCAmount_Lots_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].amount = AMOUNT_LOTS;
}

void RandomMapGenerator_WorldMap::on_rbCSize_Small_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].size = SIZE_SMALL;
}

void RandomMapGenerator_WorldMap::on_rbCSize_Medium_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].size = SIZE_MEDIUM;
}

void RandomMapGenerator_WorldMap::on_rbCSize_Large_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].size = SIZE_LARGE;
}

void RandomMapGenerator_WorldMap::on_sbTerrainNumber_valueChanged(int value)
{
	if (value == 0)
	{
		frmTerrainTile->setEnabled(false);
		frmTerrainAmount->setEnabled(false);
		frmTerrainSize->setEnabled(false);
		frmTerrainPlaceHolder->setEnabled(false);
	}
	else
	{
		Terrain *terrain = &(continents[sbContinentNumber->value() - 1].terrain[value - 1]);
		frmTerrainTile->setEnabled(true);
		frmTerrainAmount->setEnabled(true);
		frmTerrainSize->setEnabled(true);
		frmTerrainPlaceHolder->setEnabled(true);
		tbTerrainTile->setTile(((unsigned short)(terrain->tileData.tilesetID) << 8) + (unsigned long)(terrain->tileData.tileID));
		if (terrain->amount == AMOUNT_FEW)
			rbTAmount_Few->setChecked(true);
		else if (terrain->amount == AMOUNT_NORMAL)
			rbTAmount_Normal->setChecked(true);
		else
			rbTAmount_Lots->setChecked(true);
		if (terrain->size == SIZE_SMALL)
			rbTSize_Small->setChecked(true);
		else if (terrain->size == SIZE_MEDIUM)
			rbTSize_Medium->setChecked(true);
		else
			rbTSize_Large->setChecked(true);
	}
}

void RandomMapGenerator_WorldMap::on_tbTerrainTile_tileChanged(unsigned short value)
{
	ProjectData::DataReference<Tileset> *tilesetRef;
	int continent = sbContinentNumber->value() - 1;
	int terrain = sbTerrainNumber->value() - 1;
	if (continent != -1 && terrain != -1)
	{
		continents[continent].terrain[terrain].tileData.tilesetID = (unsigned char)((value >> 8u) % 256);
		continents[continent].terrain[terrain].tileData.tileID = (unsigned char)(value % 256);
		continents[continent].terrain[terrain].tileData.tileFlags = 0xff;
		tilesetRef = new ProjectData::DataReference<Tileset>((value >> 8u) % 256, __FILE__, __LINE__);
		for (int i = 0; i < 32; ++i)
			continents[continent].terrain[terrain].tileData.phasePassability[i] = (*tilesetRef)->getTile(value % 256)->getPhasePassability(i);
		delete tilesetRef;
	}
}

void RandomMapGenerator_WorldMap::on_rbTAmount_Few_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].terrain[sbTerrainNumber->value() - 1].amount = AMOUNT_FEW;
}

void RandomMapGenerator_WorldMap::on_rbTAmount_Normal_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].terrain[sbTerrainNumber->value() - 1].amount = AMOUNT_NORMAL;
}

void RandomMapGenerator_WorldMap::on_rbTAmount_Lots_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].terrain[sbTerrainNumber->value() - 1].amount = AMOUNT_LOTS;
}

void RandomMapGenerator_WorldMap::on_rbTSize_Small_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].terrain[sbTerrainNumber->value() - 1].size = SIZE_SMALL;
}

void RandomMapGenerator_WorldMap::on_rbTSize_Medium_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].terrain[sbTerrainNumber->value() - 1].size = SIZE_MEDIUM;
}

void RandomMapGenerator_WorldMap::on_rbTSize_Large_toggled(bool on)
{
	if (on)
		continents[sbContinentNumber->value() - 1].terrain[sbTerrainNumber->value() - 1].size = SIZE_LARGE;
}

void RandomMapGenerator_WorldMap::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("World Map Generator");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void RandomMapGenerator_WorldMap::on_buttonBox_accepted()
{
	configured = true;
}

void RandomMapGenerator_WorldMap::cancelGenerator()
{
	cancel = true;
}

int RandomMapGenerator_WorldMap::numDiagonalNeighbors(int x, int y, int continent, int terrain, bool unused)
{
	MapData *pMapData;
	int x2, y2, numNeighbors = 0;
	x2 = x - 1;
	y2 = y - 1;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (continent == pMapData->continent && terrain == pMapData->terrain && unused == pMapData->unused)
		++numNeighbors;
	x2 = x + 1;
	y2 = y - 1;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (continent == pMapData->continent && terrain == pMapData->terrain && unused == pMapData->unused)
		++numNeighbors;
	x2 = x - 1;
	y2 = y + 1;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (continent == pMapData->continent && terrain == pMapData->terrain && unused == pMapData->unused)
		++numNeighbors;
	x2 = x + 1;
	y2 = y + 1;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (continent == pMapData->continent && terrain == pMapData->terrain && unused == pMapData->unused)
		++numNeighbors;
	return numNeighbors;
}

int RandomMapGenerator_WorldMap::numCardinalNeighbors(int x, int y, int continent, int terrain, bool unused)
{
	MapData *pMapData;
	int x2, y2, numNeighbors = 0;
	x2 = x;
	y2 = y - 1;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (continent == pMapData->continent && terrain == pMapData->terrain && unused == pMapData->unused)
		++numNeighbors;
	x2 = x - 1;
	y2 = y;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (continent == pMapData->continent && terrain == pMapData->terrain && unused == pMapData->unused)
		++numNeighbors;
	x2 = x + 1;
	y2 = y;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (continent == pMapData->continent && terrain == pMapData->terrain && unused == pMapData->unused)
		++numNeighbors;
	x2 = x;
	y2 = y + 1;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (continent == pMapData->continent && terrain == pMapData->terrain && unused == pMapData->unused)
		++numNeighbors;
	return numNeighbors;
}

void RandomMapGenerator_WorldMap::init()
{
	MapData *pMapData1;
	int x, y, numToPlace, numPlaced;
	for (int i = 0; i < numContinents; ++i)
	{
		if (continents[i].amount == AMOUNT_FEW)
			numToPlace = mapSize * mapSize / 1000;
		else if (continents[i].amount == AMOUNT_NORMAL)
			numToPlace = mapSize * mapSize / 500;
		else if (continents[i].amount == AMOUNT_LOTS)
			numToPlace = mapSize * mapSize / 250;
		numPlaced = 0;
		while (numPlaced < numToPlace)
		{
			x = (int)numberGenerator.getRandomNumber(0ul, (unsigned long)mapSize);
			y = (int)numberGenerator.getRandomNumber(0ul, (unsigned long)mapSize);
			pMapData1 = &mapData1[y * mapSize + x];
			if (pMapData1->unused && pMapData1->continent == 0 && numCardinalNeighbors(x, y, 0, 0, true) == 4)
			{
				pMapData1->continent = i + 1;
				pMapData1->terrain = 0;
				pMapData1->unused = true;
				++numPlaced;
			}
		}
	}
	numToPlace = mapSize / 5;
	numPlaced = 0;
	while (numPlaced < numToPlace)
	{
		x = (int)numberGenerator.getRandomNumber(0ul, (unsigned long)mapSize);
		y = (int)numberGenerator.getRandomNumber(0ul, (unsigned long)mapSize);
		pMapData1 = &mapData1[y * mapSize + x];
		if (pMapData1->unused && pMapData1->continent == 0 && numCardinalNeighbors(x, y, 0, 0, true) == 4)
		{
			pMapData1->continent = 0;
			pMapData1->terrain = 0;
			pMapData1->unused = false;
			++numPlaced;
		}
	}
}

void RandomMapGenerator_WorldMap::runInitPhase1()
{
	MapData *pMapData1;
	MapData *pMapData2;
	unsigned long randomNumber;
	for (int y = 0; y < mapSize && !cancel; ++y)
	{
		for (int x = 0; x < mapSize && !cancel; ++x)
		{
			pMapData1 = &mapData1[y * mapSize + x];
			pMapData2 = &mapData2[y * mapSize + x];
			if (pMapData1->unused && pMapData1->continent == 0)
			{
				randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
				if (randomNumber <= waterSproutChance)
				{
					pMapData2->continent = 0;
					pMapData2->terrain = 0;
					pMapData2->unused = false;
				}
			}
			/*for (int i = 0; i < numContinents && pMapData2->continent == 0 && pMapData2->unused && !cancel; ++i)
			{
				randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
				if (randomNumber <= continents[i].sproutChance)
				{
					pMapData2->continent = i + 1;
					pMapData2->terrain = 0;
					pMapData2->unused = true;
				}
			}*/
		}
	}
}

void RandomMapGenerator_WorldMap::runInitPhase2()
{
	MapData *pMapData1;
	MapData *pMapData2;
	Continent *pContinent;
	unsigned long randomNumber;
	for (int y = 0; y < mapSize && !cancel; ++y)
	{
		for (int x = 0; x < mapSize && !cancel; ++x)
		{
			pMapData1 = &mapData1[y * mapSize + x];
			pMapData2 = &mapData2[y * mapSize + x];
			if (pMapData1->continent > 0)
				pContinent = &continents[pMapData1->continent - 1];
			else
				pContinent = NULL;
			if (pMapData1->unused)
			{
				randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
				if (randomNumber <= cTerrainSproutChance)
					pMapData2->unused = false;
			}
			if (pContinent != NULL)
			{
				for (int i = 0; i < pContinent->numTerrain && pMapData2->unused && !cancel; ++i)
				{
					randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
					if (randomNumber <= pContinent->terrain[i].sproutChance)
					{
						pMapData2->terrain = i + 1;
						pMapData2->unused = false;
					}
				}
			}
		}
	}
}

void RandomMapGenerator_WorldMap::runGrowPhase1()
{
	MapData *pMapData1;
	MapData *pMapData2;
	unsigned long randomNumber;
	for (int y = 0; y < mapSize && !cancel; ++y)
	{
		for (int x = 0; x < mapSize && !cancel; ++x)
		{
			pMapData1 = &mapData1[y * mapSize + x];
			pMapData2 = &mapData2[y * mapSize + x];
			if (pMapData1->unused && pMapData1->continent == 0)
			{
				int numNeighbors = numCardinalNeighbors(x, y, 0, 0, false);
				if (numNeighbors == 4)
				{
					pMapData2->continent = 0;
					pMapData2->terrain = 0;
					pMapData2->unused = false;
				}
				else if (numNeighbors > 0)
				{
					randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
					if (randomNumber <= waterGrowChance * numNeighbors)
					{
						pMapData2->continent = 0;
						pMapData2->terrain = 0;
						pMapData2->unused = false;
					}
				}
			}
			for (int i = 0; i < numContinents && pMapData1->continent == 0 && pMapData2->unused && !cancel; ++i)
			{
				int numNeighbors = numCardinalNeighbors(x, y, i + 1, 0, true);
				if (numNeighbors == 4)
				{
					pMapData2->continent = i + 1;
					pMapData2->terrain = 0;
					pMapData2->unused = true;
				}
				else if (numNeighbors > 0)
				{
					randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
					if (randomNumber < continents[i].growChance * numNeighbors)
					{
						pMapData2->continent = i + 1;
						pMapData2->terrain = 0;
						pMapData2->unused = true;
					}
				}
			}
		}
	}
}

void RandomMapGenerator_WorldMap::runGrowPhase2()
{
	MapData *pMapData1;
	MapData *pMapData2;
	Continent *pContinent;
	unsigned long randomNumber;
	for (int y = 0; y < mapSize && !cancel; ++y)
	{
		for (int x = 0; x < mapSize && !cancel; ++x)
		{
			pMapData1 = &mapData1[y * mapSize + x];
			pMapData2 = &mapData2[y * mapSize + x];
			if (pMapData1->continent > 0)
				pContinent = &continents[pMapData1->continent - 1];
			else
				pContinent = NULL;
			if (pMapData1->unused)
			{
				int numNeighbors = numCardinalNeighbors(x, y, pMapData1->continent, 0, false);
				if (numNeighbors == 4)
				{
					pMapData2->terrain = 0;
					pMapData2->unused = false;
				}
				else if (numNeighbors > 0)
				{
					randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
					if (randomNumber < pContinent->growChance * numNeighbors)
					{
						pMapData2->terrain = 0;
						pMapData2->unused = false;
					}
				}
			}
			if (pContinent != NULL)
			{
				for (int i = 0; i < pContinent->numTerrain && pMapData2->unused && !cancel; ++i)
				{
					int numNeighbors = numCardinalNeighbors(x, y, pMapData1->continent, i + 1, false);
					if (numNeighbors == 4)
					{
						pMapData2->terrain = i + 1;
						pMapData2->unused = false;
					}
					else if (numNeighbors > 0)
					{
						randomNumber = numberGenerator.getRandomNumber(0ul, 1000000ul);
						if (randomNumber < pContinent->terrain[i].growChance * numNeighbors)
						{
							pMapData2->terrain = i + 1;
							pMapData2->unused = false;
						}
					}
				}
			}
		}
	}
}

bool RandomMapGenerator_WorldMap::cleanPhase1()
{
	MapData *pMapData1;
	MapData *pMapData2;
	int numNeighbors;
	bool isClean = true;
	for (int y = 0; y < mapSize && !cancel; ++y)
	{
		for (int x = 0; x < mapSize && !cancel; ++x)
		{
			pMapData1 = &mapData1[y * mapSize + x];
			pMapData2 = &mapData2[y * mapSize + x];
			numNeighbors = numCardinalNeighbors(x, y, pMapData1->continent, pMapData1->terrain, pMapData1->unused);
			numNeighbors += numDiagonalNeighbors(x, y, pMapData1->continent, pMapData1->terrain, pMapData1->unused);
			if (numNeighbors < 3)
			{
				pMapData1->continent = 0;
				pMapData1->terrain = 0;
				pMapData1->unused = true;
				pMapData2->continent = 0;
				pMapData2->terrain = 0;
				pMapData2->unused = true;
				isClean = false;
			}
		}
	}
	return isClean;
}

void RandomMapGenerator_WorldMap::createContinent(int x, int y, int continent, int terrain, bool unused, QList<RandomMapGenerator_WorldMap::LandPiece> *land)
{
	MapData *pMapData;
	LandPiece piece;
	int x2, y2;
	piece.x = x;
	piece.y = y;
	piece.continent = continent;
	piece.terrain = terrain;
	piece.unused = unused;
	land->append(piece);
	scannedTiles[y * mapSize + x] = 0;
	++total;
	progressIndicator->setCurrentProcessProgressBarValue(total);
	x2 = x + 1;
	y2 = y;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (pMapData->continent == continent && pMapData->terrain == terrain && pMapData->unused == unused && scannedTiles[y2 * mapSize + x2] == -1 && !cancel)
		createContinent(x2, y2, continent, terrain, unused, land);
	x2 = x - 1;
	y2 = y;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (pMapData->continent == continent && pMapData->terrain == terrain && pMapData->unused == unused && scannedTiles[y2 * mapSize + x2] == -1 && !cancel)
		createContinent(x2, y2, continent, terrain, unused, land);
	x2 = x;
	y2 = y + 1;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (pMapData->continent == continent && pMapData->terrain == terrain && pMapData->unused == unused && scannedTiles[y2 * mapSize + x2] == -1 && !cancel)
		createContinent(x2, y2, continent, terrain, unused, land);
	x2 = x;
	y2 = y - 1;
	if (x2 < 0)
		x2 += mapSize;
	else if (x2 >= mapSize)
		x2 -= mapSize;
	if (y2 < 0)
		y2 += mapSize;
	else if (y2 >= mapSize)
		y2 -= mapSize;
	pMapData = &mapData1[y2 * mapSize + x2];
	if (pMapData->continent == continent && pMapData->terrain == terrain && pMapData->unused == unused && scannedTiles[y2 * mapSize + x2] == -1 && !cancel)
		createContinent(x2, y2, continent, terrain, unused, land);
}

void RandomMapGenerator_WorldMap::cleanPhase2()
{
	MapData *pMapData;
	total = 0;
	for (int i = 0; i < mapSize * mapSize; ++i)
		scannedTiles.append(-1);
	for (int y = 0; y < mapSize && !cancel; ++y)
	{
		for (int x = 0; x < mapSize && !cancel; ++x)
		{
			pMapData = &mapData1[y * mapSize + x];
			if (scannedTiles[y * mapSize + x] == -1 && !cancel)
			{
				ScanContinent continent;
				createContinent(x, y, pMapData->continent, pMapData->terrain, pMapData->unused, &continent.land);
				if (pMapData->continent == 0 && continent.land.size() <= minimumWaterSize)
					continent.erase = true;
				else if (pMapData->continent > 0)
				{
					if (continent.land.size() <= continents[pMapData->continent - 1].minimumSize)
						continent.erase = true;
					else
						continent.erase = false;
				}
				else
					continent.erase = false;
				scanContinents.append(continent);
			}
		}
	}
	for (int i = 0; i < scanContinents.size() && !cancel; ++i)
	{
		if (scanContinents[i].erase && !cancel)
		{
			for (int j = 0; j < scanContinents[i].land.size() && !cancel; ++j)
			{
				pMapData = &mapData1[scanContinents[i].land[j].y * mapSize + scanContinents[i].land[j].x];
				pMapData->continent = 0;
				pMapData->terrain = 0;
				pMapData->unused = true;
			}
		}
		scanContinents[i].land.clear();
	}
	scanContinents.clear();
	scannedTiles.clear();
}

bool RandomMapGenerator_WorldMap::phase1Done(bool updateProgressBar)
{
	int usedSpace = 0;
	for (int y = 0; y < mapSize && !cancel; ++y)
	{
		for (int x = 0; x < mapSize && !cancel; ++x)
		{
			int location = y * mapSize + x;
			if (!(mapData1[location].unused))
				++usedSpace;
			else if (mapData1[location].continent != 0)
				++usedSpace;
		}
	}
	if (updateProgressBar)
		progressIndicator->setCurrentProcessProgressBarValue(usedSpace);
	if (usedSpace == mapSize * mapSize)
		return true;
	return false;
}

bool RandomMapGenerator_WorldMap::phase2Done()
{
	int usedSpace = 0;
	for (int y = 0; y < mapSize && !cancel; ++y)
	{
		for (int x = 0; x < mapSize && !cancel; ++x)
		{
			if (!(mapData1[y * mapSize + x].unused))
				++usedSpace;
		}
	}
	progressIndicator->setCurrentProcessProgressBarValue(usedSpace);
	if (usedSpace == mapSize * mapSize)
		return true;
	return false;
}
