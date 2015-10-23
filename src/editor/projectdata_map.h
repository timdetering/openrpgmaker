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

#ifndef PROJECTDATA_MAP_H
#define PROJECTDATA_MAP_H

template <typename T> class QList;
class QString;
class MapEvent;
class BaseEvent;

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "audiomanager.h"
#include "projectdata.h"
#include "projectdata_tileset.h"
#include "storagefile.h"
#include "xmlParser.h"

class Map
{
	public:
		friend class ProjectData;
		enum {MODE_LOCKTOEDGE=0, MODE_FILLWITHBORDERTILE=1, MODE_WRAPAROUND};
		enum {ASINPARENT=0, ALLOW=1, FORBID=2};
		enum {BGM_PARENT=0, BGM_EVENT=1, BGM_SPECIFY=2};
		enum {BBG_PARENT=0, BBG_TERRAIN=1, BBG_SPECIFY=2};
		enum {PASS_UP=0x1, PASS_RIGHT=0x2, PASS_DOWN=0x4, PASS_LEFT=0x8};
		struct MapArea
		{
			QList<unsigned long> monsterEncounters;
			int x, y;
			int w, h;
			int encounterRate;
			int minPerBattle;
			int maxPerBattle;
		};
		struct TileData
		{
			TileData &operator=(const TileData &other)
			{
				tilesetID = other.tilesetID;
				tileID = other.tileID;
				tileFlags = other.tileFlags;
				for (int i = 0; i < NUM_PHASES; ++i)
				{
					phaseDirections[i] = other.phaseDirections[i];
					phasePassability[i] = other.phasePassability[i];
				}
				return (*this);
			}
			TileData &operator=(TileData &other)
			{
				tilesetID = other.tilesetID;
				tileID = other.tileID;
				tileFlags = other.tileFlags;
				for (int i = 0; i < NUM_PHASES; ++i)
				{
					phaseDirections[i] = other.phaseDirections[i];
					phasePassability[i] = other.phasePassability[i];
				}
				return (*this);
			}
			bool operator==(TileData &other)
			{
				if (tilesetID == other.tilesetID && tileID == other.tileID)
					return true;
				return false;
			}
			bool operator!=(TileData &other)
			{
				if (tilesetID != other.tilesetID)
					return true;
				if (tileID != other.tileID)
					return true;
				return false;
			}
			unsigned char tilesetID;
			unsigned char tileID;
			unsigned char tileFlags;
			unsigned char phaseDirections[NUM_PHASES];
			bool phasePassability[NUM_PHASES];
		};
		struct LayerData
		{
			QList<TileData> tileData;
			QString name;
			bool visible;
		};
		Map();
		Map(Map *other);
		Map(XMLNode MapNode);
		Map(StorageFile &storageFile);
		~Map();
		void saveToStorageFile(StorageFile &storageFile);
		XMLNode getMapNode();
		QString getName() const;
		void setName(QString value);
		QString getBGLocation() const;
		void setBGLocation(QString location);
		MapEvent* getEvent(int eventID) const;
		void addEvent(MapEvent *event);
		void deleteEvent(int eventID);
		int numEvents();
		BaseEvent *getOnLoadEvent();
		MapArea *getArea(int areaID) const;
		void addArea(MapArea *area);
		void deleteArea(int areaID);
		int numAreas();
		unsigned long getMonsterEncounter(int encounterID) const;
		void addMonsterEncounter(unsigned long encounter);
		void deleteMonsterEncounter(int encounterID);
		int numMonsterEncounters();
		unsigned char getTileFlags(int x, int y, int layer) const;
		unsigned char getTilesetID(int x, int y, int layer) const;
		unsigned char getTileID(int x, int y, int layer) const;
		unsigned char getPhaseDirections(int x, int y, int layer, int phaseID) const;
		bool isPhaseOn(int x, int y, int layer, int phaseID) const;
		TileData getTileData(int x, int y, int layer) const;
		LayerData *getLayerData(int layer);
		QString getLayerName(int layer) const;
		void addLayer(QString name);
		void deleteLayer(int layer);
		void moveLayerUp(int layer);
		void moveLayerDown(int layer);
		void setLayerName(int layer, QString name);
		bool isLayerVisible(int layer) const;
		void setLayerVisible(int layer, bool visible);
		void setTileID(int x, int y, int layer, unsigned char tilesetID, unsigned char tileID, bool autocalc=true);
		void setTileFlags(int x, int y, int layer, unsigned char flags);
		void setPhaseDirections(int x, int y, int layer, int phaseID, unsigned char directions);
		void setPhaseOn(int x, int y, int layer, int phaseID, bool on);
		void setTileData(int x, int y, int layer, const TileData &tileData);
		void autoCalcTileFlags(int x, int y, int layer, bool adjust=true);
		int getEncounterRate() const;
		void setEncounterRate(int value);
		int getMinPerBattle() const;
		void setMinPerBattle(int value);
		int getMaxPerBattle() const;
		void setMaxPerBattle(int value);
		int getWidth() const;
		int getHeight() const;
		int numLayers() const;
		void resizeMap(int newWidth, int newHeight);
		void cropMap(int xOffset, int yOffset, int newWidth, int newHeight);
		int getBGHScrollSpeed() const;
		void setBGHScrollSpeed(int value);
		bool getBGAutoHScroll() const;
		void setBGAutoHScroll(bool value);
		int getBGVScrollSpeed() const;
		void setBGVScrollSpeed(int value);
		bool getBGAutoVScroll() const;
		void setBGAutoVScroll(bool value);
		int getHorizontalMode() const;
		void setHorizontalMode(int value);
		int getVerticalMode() const;
		void setVerticalMode(int value);
		int getParent() const;
		void setParent(int value);
		int getTeleport() const;
		void setTeleport(int value);
		int getEscape() const;
		void setEscape(int value);
		int getSave() const;
		void setSave(int value);
		void getBGMInfo(AudioManager::MusicInfo *info);
		void setBGMInfo(AudioManager::MusicInfo info);
		int getBGMType() const;
		void setBGMType(int value);
		QString getBBGLocation() const;
		void setBBGLocation(QString value);
		int getBBGType();
		void setBBGType(int value);
		unsigned short getBorderTile() const;
		void setBorderTile(unsigned short value);
		bool eventAtLocation(QPoint &pos);
		int eventIDAtLocation(QPoint &pos);
		void autoCalcAllTileFlags(int layer);
		void applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles);
		static bool tryLoad(XMLNode MapNode);
	private:
		void replaceTilesetReferences(int oldID, int newID);
		void moveEventInsideMap(int eventID);
		QString name;
		QString bgLocation;
		QString bbgLocation;
		QList<MapEvent*> events;
		QList<MapArea*> areas;
		QList<LayerData*> layers;
		QList<unsigned long> monsterEncounters;
		BaseEvent *onLoadEvent;
		AudioManager::MusicInfo bgmInfo;
		int encounterRate;
		int minPerBattle;
		int maxPerBattle;
		int width;
		int height;
		int parent;
		int bgHScrollSpeed;
		int bgVScrollSpeed;
		int horizontalMode;
		int verticalMode;
		int teleport;
		int escape;
		int save;
		int bgmType;
		int bbgType;
		unsigned short borderTile;
		bool bgAutoHScroll;
		bool bgAutoVScroll;
};

#endif // PROJECTDATA_MAP_H
