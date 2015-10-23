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

#include <QPainter>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_tile.h"
#include "storagefile.h"
#include "xmlParser.h"
#include "xmlTools.h"

Tile::Tile()
{
	terrainID = 0;
	tileGroup = -1;
	fps = 8;
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		phaseDirections[i] = 0x0;
		phasePassability[i] = false;
	}
	pingPong = false;
}

Tile::Tile(Tile& tile)
{
	image = tile.image;
	location = tile.location;
	terrainID = tile.terrainID;
	tileGroup = tile.tileGroup;
	fps = tile.fps;
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		phaseDirections[i] = tile.phaseDirections[i];
		phasePassability[i] = tile.phasePassability[i];
	}
	pingPong = tile.pingPong;
}

Tile::Tile(const Tile& tile)
{
	image = tile.image;
	location = tile.location;
	terrainID = tile.terrainID;
	tileGroup = tile.tileGroup;
	fps = tile.fps;
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		phaseDirections[i] = tile.phaseDirections[i];
		phasePassability[i] = tile.phasePassability[i];
	}
	pingPong = tile.pingPong;
}

Tile::Tile(XMLNode TileNode)
{
	QString directions;
	unsigned long passability;
	location = ProjectData::getAbsoluteResourcePath(TileNode.getAttribute("file"));
	directions = QString(TileNode.getAttribute("phaseDirections"));
	passability = QString(TileNode.getAttribute("phasePassability")).toULong(0, 16);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		phaseDirections[i] = (unsigned char)(directions.mid(i, 1).toUShort(0, 16));
		phasePassability[i] = ((passability >> i) % 2 == 1) ? true:false;
	}
	terrainID = atoi(TileNode.getAttribute("terrain"));
	tileGroup = atoi(TileNode.getAttribute("group"));
	fps = atoi(TileNode.getAttribute("fps"));
	pingPong = (QString(TileNode.getAttribute("pingPong")) == "true") ? true:false;
	image = QImage(location).convertToFormat(QImage::Format_ARGB32);
	isEmpty = true;
	for (int y = 0; y < image.height() && isEmpty; ++y)
	{
		for (int x = 0; x < image.width() && isEmpty; ++x)
		{
			if (qAlpha(image.pixel(x, y)) != 0)
				isEmpty = false;
		}
	}
}

Tile::Tile(QString tileLocation)
{
	terrainID = 0;
	tileGroup = -1;
	fps = 8;
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		phaseDirections[i] = 0x0;
		phasePassability[i] = false;
	}
	pingPong = false;
	location = ProjectData::getAbsoluteResourcePath(tileLocation);
	image = QImage(location).convertToFormat(QImage::Format_ARGB32);
	isEmpty = true;
	for (int y = 0; y < image.height() && isEmpty; ++y)
	{
		for (int x = 0; x < image.width() && isEmpty; ++x)
		{
			
			if (qAlpha(image.pixel(x, y)) != 0)
				isEmpty = false;
		}
	}
}

Tile::Tile(StorageFile &storageFile)
{
	unsigned long passability;
	char *temp_string;
	temp_string = storageFile.getString();
	location = temp_string;
	delete[] temp_string;
	terrainID = storageFile.getSignedInt();
	tileGroup = storageFile.getSignedInt();
	fps = storageFile.getSignedInt();
	passability = storageFile.getUnsignedLong();
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		phaseDirections[i] = storageFile.getUnsignedChar();
		phasePassability[i] = ((passability >> i) % 2 == 1) ? true:false;
	}
	pingPong = storageFile.getBool();
	image = QImage(location).convertToFormat(QImage::Format_ARGB32);
	isEmpty = true;
	for (int y = 0; y < image.height() && isEmpty; ++y)
	{
		for (int x = 0; x < image.width() && isEmpty; ++x)
		{
			if (qAlpha(image.pixel(x, y)) != 0)
				isEmpty = false;
		}
	}
}

Tile::~Tile()
{
}

void Tile::saveToStorageFile(StorageFile &storageFile)
{
	unsigned long passability = 0x0;
	storageFile.putString(location.toUtf8().data());
	storageFile.putSignedInt(terrainID);
	storageFile.putSignedInt(tileGroup);
	storageFile.putSignedInt(fps);
	for (int i = 0; i < NUM_PHASES; ++i)
		passability += ((phasePassability[i]) ? 1:0) << i;
	storageFile.putUnsignedLong(passability);
	for (int i = 0; i < NUM_PHASES; ++i)
		storageFile.putUnsignedChar(phasePassability[i]);
	storageFile.putBool(pingPong);
}

QImage Tile::getImage() const
{
	return image;
}

QImage Tile::getTile(unsigned char flags, int frame)
{
	QImage tile(32, 32, QImage::Format_ARGB32);
	QRect srcTopLeft, srcTopRight, srcBottomLeft, srcBottomRight;
	QRect dstTopLeft(0, 0, 16, 16);
	QRect dstTopRight(16, 0, 16, 16);
	QRect dstBottomLeft(0, 16, 16, 16);
	QRect dstBottomRight(16, 16, 16, 16);
	QPainter painter;
	tile.fill(qRgba(0, 0, 0, 0));
	painter.begin(&tile);
	drawTile(painter, QPoint(0, 0), flags, frame);
	painter.end();
	return tile;
}

void Tile::drawTile(QPainter &painter, QPoint location, unsigned char flags, int frame)
{
	QRect srcTopLeft, srcTopRight, srcBottomLeft, srcBottomRight;
	QRect dstTopLeft(location.x(), location.y(), 16, 16);
	QRect dstTopRight(location.x() + 16, location.y(), 16, 16);
	QRect dstBottomLeft(location.x(), location.y() + 16, 16, 16);
	QRect dstBottomRight(location.x() + 16, location.y() + 16, 16, 16);
	if (isEmpty)
		return;
	if (isAutoTile())
	{
		srcTopLeft = QRect(0, 128, 16, 16);
		srcTopRight = QRect(16, 128, 16, 16);
		srcBottomLeft = QRect(0, 144, 16, 16);
		srcBottomRight = QRect(16, 144, 16, 16);
		if ((flags & Tile_nw) == 0)
			srcTopLeft = QRect(0, 96, 16, 16);
		if ((flags & Tile_ne) == 0)
			srcTopRight = QRect(16, 96, 16, 16);
		if ((flags & Tile_sw) == 0)
			srcBottomLeft = QRect(0, 112, 16, 16);
		if ((flags & Tile_se) == 0)
			srcBottomRight = QRect(16, 112, 16, 16);
		if ((flags & Tile_w) == 0)
		{
			srcTopLeft = QRect(0, 32, 16, 16);
			srcBottomLeft = QRect(0, 48, 16, 16);
		}
		if ((flags & Tile_e) == 0)
		{
			srcTopRight = QRect(16, 32, 16, 16);
			srcBottomRight = QRect(16, 48, 16, 16);
		}
		if ((flags & Tile_n) == 0)
		{
			srcTopLeft = QRect(0, 64, 16, 16);
			srcTopRight = QRect(16, 64, 16, 16);
		}
		if ((flags & Tile_s) == 0)
		{
			srcBottomLeft = QRect(0, 80, 16, 16);
			srcBottomRight = QRect(16, 80, 16, 16);
		}
		if ((flags & Tile_n) == 0 && (flags & Tile_w) == 0)
			srcTopLeft = QRect(0, 0, 16, 16);
		if ((flags & Tile_n) == 0 && (flags & Tile_e) == 0)
			srcTopRight = QRect(16, 0, 16, 16);
		if ((flags & Tile_s) == 0 && (flags & Tile_w) == 0)
			srcBottomLeft = QRect(0, 16, 16, 16);
		if ((flags & Tile_s) == 0 && (flags & Tile_e) == 0)
			srcBottomRight = QRect(16, 16, 16, 16);
		srcTopLeft.setX(srcTopLeft.x() + (32 * frame));
		srcTopRight.setX(srcTopRight.x() + (32 * frame));
		srcBottomLeft.setX(srcBottomLeft.x() + (32 * frame));
		srcBottomRight.setX(srcBottomRight.x() + (32 * frame));
		painter.drawImage(dstTopLeft, image, srcTopLeft);
		painter.drawImage(dstTopRight, image, srcTopRight);
		painter.drawImage(dstBottomLeft, image, srcBottomLeft);
		painter.drawImage(dstBottomRight, image, srcBottomRight);
	}
	else
	{
		srcTopLeft = QRect(frame * 32, 0, 32, 32);
		painter.drawImage(location, image, srcTopLeft);
	}
}

QString Tile::getLocation() const
{
	return location;
}

int Tile::getTerrainID() const
{
	return terrainID;
}

int Tile::getTileGroup() const
{
	return tileGroup;
}

int Tile::getFPS() const
{
	return fps;
}

unsigned char Tile::getPhaseDirections(int phase) const
{
	return phaseDirections[phase];
}

bool Tile::getPhasePassability(int phase) const
{
	return phasePassability[phase];
}

bool Tile::isPingPong() const
{
	return pingPong;
}

XMLNode Tile::getTileNode()
{
	XMLNode TileNode = XMLNode::createXMLTopNode("tile");
	QString directions;
	unsigned long passability = 0x0;
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		directions += QString("%1").arg((unsigned short)phaseDirections[i], 1, 16, QChar('0'));
		passability += ((phasePassability[i]) ? 1:0) << i;
	}
	TileNode.addAttribute("phaseDirections", directions.toUtf8().data());
	TileNode.addAttribute("phasePassability", QString("%1").arg(passability, 6, 16, QChar('0')).toUtf8().data());
	TileNode.addAttribute("terrain", QString::number((unsigned int)terrainID).toUtf8().data());
	TileNode.addAttribute("group", QString::number(tileGroup).toUtf8().data());
	TileNode.addAttribute("fps", QString::number(fps).toUtf8().data());
	TileNode.addAttribute("pingPong", (pingPong) ? "true":"false");
	TileNode.addAttribute("file", ProjectData::getRelativeResourcePath(location).toUtf8().data());
	return TileNode;
}

int Tile::getTileCount() const
{
	return (image.width() / 32) * (image.height() / 32);
}

int Tile::getNumFrames() const
{
	return image.width() / 32;
}

void Tile::setLocation(QString tileLocation)
{
	location = ProjectData::getAbsoluteResourcePath(tileLocation);
	image = QImage(location).convertToFormat(QImage::Format_ARGB32);
	isEmpty = true;
	for (int y = 0; y < image.height() && isEmpty; ++y)
	{
		for (int x = 0; x < image.width() && isEmpty; ++x)
		{
			if (qAlpha(image.pixel(x, y)) != 0)
				isEmpty = false;
		}
	}
}

void Tile::setTerrainID(int tileTerrainID)
{
	terrainID = tileTerrainID;
}

void Tile::setTileGroup(int group)
{
	tileGroup = group;
}

void Tile::setFPS(int value)
{
	fps = value;
}

void Tile::setPhaseDirections(int phase, unsigned char value)
{
	phaseDirections[phase] = value;
}

void Tile::setPhasePassability(int phase, bool on)
{
	phasePassability[phase] = on;
}

void Tile::setPingPong(bool value)
{
	pingPong = value;
}

bool Tile::isAutoTile()
{
	return (image.height() == 160);
}

bool Tile::isAnimatedTile()
{
	return (image.width() > 32);
}

bool Tile::tryLoad(XMLNode TileNode)
{
	if (!XMLTools::attributeExists(TileNode, "file"))
		return false;
	if (!XMLTools::attributeExists(TileNode, "phaseDirections"))
		return false;
	if (!XMLTools::attributeExists(TileNode, "phasePassability"))
		return false;
	if (!XMLTools::attributeExists(TileNode, "terrain"))
		return false;
	if (!XMLTools::attributeExists(TileNode, "group"))
		return false;
	if (!XMLTools::attributeExists(TileNode, "fps"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TileNode, "fps", 1, 24))
		return false;
	if (!XMLTools::attributeExists(TileNode, "pingPong"))
		return false;
	if (!XMLTools::attributeStringValid(TileNode, "pingPong", QStringList() << "true" << "false", 1))
		return false;
	return true;
}
