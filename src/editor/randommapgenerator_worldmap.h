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

#ifndef RANDOMMAPGENERATOR_WORLDMAP_H
#define RANDOMMAPGENERATOR_WORLDMAP_H

#include "ui_randommapgenerator_worldmap.h"
#include "dualprogressindicator.h"
#include "projectdata_map.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "xmlParser.h"

class RandomMapGenerator_WorldMap : public QDialog, public Ui::RandomMapGenerator_WorldMap
{
	Q_OBJECT
	public:
		RandomMapGenerator_WorldMap(QWidget *parent=0);
		~RandomMapGenerator_WorldMap();
		void setupGenerator(XMLNode ConfigurationNode);
		XMLNode getConfigurationNode();
		bool isConfigured();
		bool canceled();
		QStringList getGeneratedMapNames();
		Map *getGeneratedMap(int mapNum);
		void generateWorldMap(unsigned long seed);
	protected slots:
		void on_sbNumContinents_valueChanged(int value);
		void on_sbContinentNumber_valueChanged(int value);
		void on_sbNumTerrain_valueChanged(int value);
		void on_tbContinentTile_tileChanged(unsigned short value);
		void on_rbCAmount_Few_toggled(bool on);
		void on_rbCAmount_Normal_toggled(bool on);
		void on_rbCAmount_Lots_toggled(bool on);
		void on_rbCSize_Small_toggled(bool on);
		void on_rbCSize_Medium_toggled(bool on);
		void on_rbCSize_Large_toggled(bool on);
		void on_sbTerrainNumber_valueChanged(int value);
		void on_tbTerrainTile_tileChanged(unsigned short value);
		void on_rbTAmount_Few_toggled(bool on);
		void on_rbTAmount_Normal_toggled(bool on);
		void on_rbTAmount_Lots_toggled(bool on);
		void on_rbTSize_Small_toggled(bool on);
		void on_rbTSize_Medium_toggled(bool on);
		void on_rbTSize_Large_toggled(bool on);
		void on_buttonBox_helpRequested();
		void on_buttonBox_accepted();
		void cancelGenerator();
	private:
		enum Amount {AMOUNT_FEW=0, AMOUNT_NORMAL, AMOUNT_LOTS};
		enum Size {SIZE_SMALL=0, SIZE_MEDIUM, SIZE_LARGE};
		enum MapSize {MAP_50x50=0, MAP_100x100=16, MAP_200x200=32, MAP_300x300=48, MAP_400x400=64};
		enum MinSizes
		{
			MINIMUM_WATER_SIZE=0,
			MINIMUM_SMALL_CONTINENT_SIZE=1,
			MINIMUM_MEDIUM_CONTINENT_SIZE=2,
			MINIMUM_LARGE_CONTINENT_SIZE=3
		};
		enum Chance
		{
			CHANCE_SPROUT_WATER=0,
			CHANCE_SPROUT_CONTINENT_TERRAIN=1,
			CHANCE_GROW_WATER=2,
			CHANCE_GROW_CONTINENT_TERRAIN=3,
			CHANCE_SPROUT_FEW_CONTINENTS=4,
			CHANCE_SPROUT_NORMAL_CONTINENTS=5,
			CHANCE_SPROUT_LOTS_CONTINENTS=6,
			CHANCE_GROW_SMALL_CONTINENTS=7,
			CHANCE_GROW_MEDIUM_CONTINENTS=8,
			CHANCE_GROW_LARGE_CONTINENTS=9,
			CHANCE_SPROUT_FEW_TERRAIN=10,
			CHANCE_SPROUT_NORMAL_TERRAIN=11,
			CHANCE_SPROUT_LOTS_TERRAIN=12,
			CHANCE_GROW_SMALL_TERRAIN=13,
			CHANCE_GROW_MEDIUM_TERRAIN=14,
			CHANCE_GROW_LARGE_TERRAIN=15
		};
		struct MapData
		{
			int continent;
			int terrain;
			bool unused;
		};
		struct LandPiece
		{
			int x;
			int y;
			int continent;
			int terrain;
			bool unused;
		};
		struct ScanContinent
		{
			QList<LandPiece> land;
			bool erase;
		};
		struct Terrain
		{
			Map::TileData tileData;
			unsigned long sproutChance;
			unsigned long growChance;
			int amount;
			int size;
		};
		struct Continent
		{
			Terrain terrain[6];
			Map::TileData tileData;
			unsigned long sproutChance;
			unsigned long growChance;
			int numTerrain;
			int amount;
			int size;
			int minimumSize;
		};
		int numDiagonalNeighbors(int x, int y, int continent, int terrain, bool unused);
		int numCardinalNeighbors(int x, int y, int continent, int terrain, bool unused);
		void init();
		void runInitPhase1();
		void runInitPhase2();
		void runGrowPhase1();
		void runGrowPhase2();
		bool cleanPhase1();
		void createContinent(int x, int y, int continent, int terrain, bool unused, QList<LandPiece> *land);
		void cleanPhase2();
		bool phase1Done(bool updateProgressBar=true);
		bool phase2Done();
		DualProgressIndicator *progressIndicator;
		QList<ScanContinent> scanContinents;
		QList<int> scannedTiles;
		RandomNumberGenerator numberGenerator;
		Continent continents[5];
		MapData *mapData1;
		MapData *mapData2;
		double chances[80];
		double minSizes[4];
		unsigned long waterSproutChance;
		unsigned long waterGrowChance;
		unsigned long cTerrainSproutChance;
		unsigned long cTerrainGrowChance;
		int mapSize;
		int total;
		int numContinents;
		int minimumWaterSize;
		bool configured;
		bool cancel;
};

#endif // RANDOMMAPGENERATOR_WORLDMAP_H
