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

#ifndef PROJECTDATA_TILE_H
#define PROJECTDATA_TILE_H

class QPainter;
class QPoint;
struct XMLNode;
class StorageFile;

#include <QImage>
#include <QString>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"

class Tile
{
	public:
		friend class Tileset;
		Tile();
		Tile(Tile& tile);
		Tile(const Tile& tile);
		Tile(XMLNode TileNode);
		Tile(QString tileLocation);
		Tile(StorageFile &storageFile);
		~Tile();
		void saveToStorageFile(StorageFile &storageFile);
		QImage getImage() const;
		QImage getTile(unsigned char flags=0x0, int frame=0);
		void drawTile(QPainter &painter, QPoint location, unsigned char flags=0x0, int frame=0);
		//int getType() const;
		QString getLocation() const;
		int getTerrainID() const;
		int getTileGroup() const;
		int getFPS() const;
		unsigned char getPhaseDirections(int phase) const;
		bool getPhasePassability(int phase) const;
		bool isPingPong() const;
		XMLNode getTileNode();
		int getTileCount() const;
		int getNumFrames() const;
		//void setType(int tileType);
		void setLocation(QString tileLocation);
		void setTerrainID(int tileTerrainID);
		void setTileGroup(int group);
		void setFPS(int value);
		void setPhaseDirections(int phase, unsigned char value);
		void setPhasePassability(int phase, bool on);
		void setPingPong(bool value);
		bool isAutoTile();
		bool isAnimatedTile();
		static bool tryLoad(XMLNode TileNode);
		//enum Type {AutoTile=0, AnimatedTile=1, AnimatedAutoTile=2, SingleTile=3};
		enum Flags {Tile_nw=128, Tile_n=64, Tile_ne=32, Tile_w=16, Tile_e=8, Tile_sw=4, Tile_s=2, Tile_se=1};
		enum PassDir {PASS_UP=0x1, PASS_RIGHT=0x2, PASS_DOWN=0x4, PASS_LEFT=0x8};
	private:
		QImage image;
		QString location;
		//int type;
		int terrainID;
		int tileGroup;
		int fps;
		unsigned char phaseDirections[NUM_PHASES];
		bool phasePassability[NUM_PHASES];
		bool pingPong;
		bool isEmpty;
};

#endif // PROJECTDATA_TILE_H
