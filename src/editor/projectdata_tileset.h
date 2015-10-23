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

#ifndef PROJECTDATA_TILESET_H
#define PROJECTDATA_TILESET_H

class Tile;

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class Tileset
{
	public:
		friend class ProjectData;
		struct SwapTileIDs
		{
			int tileset;
			int tile1;
			int tile2;
		};
		Tileset();
		Tileset(Tileset &other);
		Tileset(const Tileset &other);
		Tileset(XMLNode TilesetNode);
		Tileset(StorageFile &storageFile);
		~Tileset();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		QString getName() const;
		Tile *getTile(int tileID) const;
		XMLNode getTilesetNode();
		int numTiles() const;
		void setName(QString newName);
		void swapTiles(int tile1, int tile2);
		void addTile(Tile *tile);
		void deleteTile(int tileID);
		static bool tryLoad(XMLNode TilesetNode);
	private:
		void replaceTerrainReferences(int oldID, int newID);
		QString name;
		QList<Tile*> tiles;
};

#endif // PROJECTDATA_TILESET_H
