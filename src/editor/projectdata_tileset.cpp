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

#include <QStringList>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "xmlParser.h"
#include "xmlTools.h"

Tileset::Tileset()
{
	name = "Untitled";
}

Tileset::Tileset(Tileset &other)
{
	name = other.name;
	tiles.clear();
	for (int i = 0; i < other.tiles.size(); ++i)
		tiles.append(new Tile(*(other.tiles[i])));
}

Tileset::Tileset(const Tileset &other)
{
	name = other.name;
	tiles.clear();
	for (int i = 0; i < other.tiles.size(); ++i)
		tiles.append(new Tile(*(other.tiles[i])));
}

Tileset::Tileset(XMLNode TilesetNode)
{
	name = TilesetNode.getAttribute("name");
	for (int i = 0; i < TilesetNode.nChildNode("tile"); ++i)
		tiles.append(new Tile(TilesetNode.getChildNode("tile", i)));
}

Tileset::Tileset(StorageFile &storageFile)
{
	int count;
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	count = storageFile.getSignedInt();
	for (int i = 0; i < count; ++i)
		tiles.append(new Tile(storageFile));
}

Tileset::~Tileset()
{
	for (int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i] != NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}
	tiles.clear();
}

QStringList Tileset::getResources()
{
	QStringList resources;
	for (int i = 0; i < tiles.size(); ++i)
		resources.append(tiles[i]->getLocation());
	return resources;
}

void Tileset::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putSignedInt(tiles.size());
	for (int i = 0; i < tiles.size(); ++i)
		tiles[i]->saveToStorageFile(storageFile);
}

QString Tileset::getName() const
{
	return name;
}

Tile *Tileset::getTile(int tileID) const
{
	return tiles[tileID];
}

XMLNode Tileset::getTilesetNode()
{
	XMLNode TilesetNode = XMLNode::createXMLTopNode("tileset");
	TilesetNode.addAttribute("name", name.toUtf8().data());
	for (int i = 0; i < tiles.size(); ++i)
		TilesetNode.addChild(tiles[i]->getTileNode());
	return TilesetNode;
}

int Tileset::numTiles() const
{
	return tiles.size();
}

void Tileset::setName(QString newName)
{
	name = newName;
}

void Tileset::swapTiles(int tile1, int tile2)
{
	if (tile1 != tile2)
		tiles.swap(tile1, tile2);
}

void Tileset::addTile(Tile *tile)
{
	tiles.append(tile);
}

void Tileset::deleteTile(int tileID)
{
	if (tiles[tileID] != NULL)
	{
		delete tiles[tileID];
		tiles[tileID] = NULL;
	}
	tiles.removeAt(tileID);
}

bool Tileset::tryLoad(XMLNode TilesetNode)
{
	if (!XMLTools::attributeExists(TilesetNode, "name"))
		return false;
	for (int i = 0; i < TilesetNode.nChildNode("tile"); ++i)
		if (!Tile::tryLoad(TilesetNode.getChildNode("tile", i)))
			return false;
	return true;
}

void Tileset::replaceTerrainReferences(int oldID, int newID)
{
	int terrainID;
	for (int i = 0; i < tiles.size(); ++i)
	{
		terrainID = (int)(tiles[i]->getTerrainID());
		if (terrainID == oldID)
			tiles[i]->setTerrainID((unsigned char)newID);
		else if (terrainID >= ProjectData::terrainNames.size())
			tiles[i]->setTerrainID((unsigned char)0u);
	}
}
