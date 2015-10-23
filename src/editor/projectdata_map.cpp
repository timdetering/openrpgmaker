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

#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "audiomanager.h"
#include "projectdata.h"
#include "projectdata_baseevent.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "xmlParser.h"
#include "xmlTools.h"

Map::Map()
{
	name = "Untitled";
	bgLocation = "";
	bbgLocation = "";
	layers.append(new LayerData);
	layers[0]->name = "Main Layer";
	layers[0]->visible = true;
	for (int y = 0; y < 15; ++y)
	{
		for (int x = 0; x < 20; ++x)
		{
			TileData tileData;
			tileData.tilesetID = 0x0;
			tileData.tileID = 0x0;
			tileData.tileFlags = 0x0;
			for (int i = 0; i < NUM_PHASES; ++i)
			{
				tileData.phaseDirections[i] = 0x0;
				tileData.phasePassability[i] = false;
			}
			layers[0]->tileData.append(tileData);
		}
	}
	bgmInfo.location = "";
	bgmInfo.volume = 1.0;
	bgmInfo.panning = 0.0;
	bgmInfo.speed = 1.0;
	bgmInfo.fadeInTime = 0;
	bgmInfo.retainPitch = 0;
	onLoadEvent = new BaseEvent(BaseEvent::MAPEVENT);
	encounterRate = 25;
	minPerBattle = 0;
	maxPerBattle = -1;
	width = 20;
	height = 15;
	horizontalMode = MODE_LOCKTOEDGE;
	verticalMode = MODE_LOCKTOEDGE;
	borderTile = 0x0000;
	parent = -1;
	bgHScrollSpeed = 1;
	bgVScrollSpeed = 1;
	teleport = ALLOW;
	escape = ALLOW;
	save = ALLOW;
	bgmType = BGM_EVENT;
	bbgType = BBG_TERRAIN;
	bgAutoHScroll = false;
	bgAutoVScroll = false;
}

Map::Map(Map *other)
{
	int i, j;
	name = other->name;
	bgLocation = other->bgLocation;
	for (i = 0; i < other->events.size(); ++i)
		events.append(new MapEvent(other->events[i]));
	for (i = 0; i < other->areas.size(); ++i)
	{
		areas.append(new MapArea);
		for (j = 0; j < other->areas[i]->monsterEncounters.size(); ++j)
			areas[i]->monsterEncounters.append(other->areas[i]->monsterEncounters[j]);
		areas[i]->x = other->areas[i]->x;
		areas[i]->y = other->areas[i]->y;
		areas[i]->w = other->areas[i]->w;
		areas[i]->h = other->areas[i]->h;
		areas[i]->encounterRate = other->areas[i]->encounterRate;
		areas[i]->minPerBattle = other->areas[i]->minPerBattle;
		areas[i]->maxPerBattle = other->areas[i]->maxPerBattle;
	}
	for (i = 0; i < other->monsterEncounters.size(); ++i)
		monsterEncounters.append(other->monsterEncounters[i]);
	width = other->width;
	height = other->height;
	horizontalMode = other->horizontalMode;
	verticalMode = other->verticalMode;
	borderTile = other->borderTile;
	for (int i = 0; i < other->layers.size(); ++i)
	{
		LayerData *otherLayer = other->layers[i];
		LayerData *layer = new LayerData;
		layer->name = otherLayer->name;
		layer->visible = otherLayer->visible;
		for (int j = 0; j < otherLayer->tileData.size(); ++j)
		{
			TileData *otherTileData = &(otherLayer->tileData[j]);
			TileData tileData;
			tileData.tilesetID = otherTileData->tilesetID;
			tileData.tileID = otherTileData->tileID;
			tileData.tileFlags = otherTileData->tileFlags;
			for (int k = 0; k < NUM_PHASES; ++k)
			{
				tileData.phaseDirections[k] = otherTileData->phaseDirections[k];
				tileData.phasePassability[k] = otherTileData->phasePassability[k];
			}
			layer->tileData.append(tileData);
		}
		layers.append(layer);
	}
	onLoadEvent = new BaseEvent(other->onLoadEvent);
	parent = other->parent;
	bgmInfo.location = other->bgmInfo.location;
	bgmInfo.volume = other->bgmInfo.volume;
	bgmInfo.panning = other->bgmInfo.panning;
	bgmInfo.speed = other->bgmInfo.speed;
	bgmInfo.fadeInTime = other->bgmInfo.fadeInTime;
	bgmInfo.retainPitch = other->bgmInfo.retainPitch;
	encounterRate = other->encounterRate;
	minPerBattle = other->minPerBattle;
	maxPerBattle = other->maxPerBattle;
	bgHScrollSpeed = other->bgHScrollSpeed;
	bgVScrollSpeed = other->bgVScrollSpeed;
	teleport = other->teleport;
	escape = other->escape;
	save = other->save;
	bgmType = other->bgmType;
	bbgType = other->bbgType;
	bgAutoHScroll = other->bgAutoHScroll;
	bgAutoVScroll = other->bgAutoVScroll;
}

Map::Map(XMLNode MapNode)
{
	XMLNode TempNode1, TempNode2, TempNode3;
	QString temp_string;
	QString layerData;
	size_t inputLength = 24;
	size_t outputLength;
	unsigned char tileDataBytes[18];
	char tileDataBase64[24];
	name = MapNode.getAttribute("name");
	width = atoi(MapNode.getAttribute("width"));
	height = atoi(MapNode.getAttribute("height"));
	parent = atoi(MapNode.getAttribute("parent"));
	temp_string = MapNode.getAttribute("horizontalMode");
	if (temp_string == "lockToEdge")
		horizontalMode = MODE_LOCKTOEDGE;
	else if (temp_string == "fillWithBorderTile")
		horizontalMode = MODE_FILLWITHBORDERTILE;
	else
		horizontalMode = MODE_WRAPAROUND;
	temp_string = MapNode.getAttribute("verticalMode");
	if (temp_string == "lockToEdge")
		verticalMode = MODE_LOCKTOEDGE;
	else if (temp_string == "fillWithBorderTile")
		verticalMode = MODE_FILLWITHBORDERTILE;
	else
		verticalMode = MODE_WRAPAROUND;
	borderTile = QString(MapNode.getAttribute("borderTile")).toUShort(0, 16);
	temp_string = MapNode.getAttribute("teleport");
	if (temp_string == "parent")
		teleport = ASINPARENT;
	else if (temp_string == "allow")
		teleport = ALLOW;
	else
		teleport = FORBID;
	temp_string = MapNode.getAttribute("escape");
	if (temp_string == "parent")
		escape = ASINPARENT;
	else if (temp_string == "allow")
		escape = ALLOW;
	else
		escape = FORBID;
	temp_string = MapNode.getAttribute("save");
	if (temp_string == "parent")
		save = ASINPARENT;
	else if (temp_string == "allow")
		save = ALLOW;
	else
		save = FORBID;
	TempNode1 = MapNode.getChildNode("bgm");
	temp_string = TempNode1.getAttribute("type");
	if (temp_string == "parent")
		bgmType = BGM_PARENT;
	else if (temp_string == "event")
		bgmType = BGM_EVENT;
	else
	{
		bgmType = BGM_SPECIFY;
		bgmInfo.location = TempNode1.getAttribute("location");
		bgmInfo.volume = QString(TempNode1.getAttribute("volume")).toFloat();
		bgmInfo.panning = QString(TempNode1.getAttribute("panning")).toFloat();
		bgmInfo.speed = QString(TempNode1.getAttribute("speed")).toFloat();
		bgmInfo.fadeInTime = QString(TempNode1.getAttribute("fadeInTime")).toInt();
		bgmInfo.retainPitch = (QString(TempNode1.getAttribute("retainPitch")) == "true") ? true:false;
	}
	TempNode1 = MapNode.getChildNode("bbg");
	temp_string = TempNode1.getAttribute("type");
	if (temp_string == "parent")
		bbgType = BBG_PARENT;
	else if (temp_string == "terrain")
		bbgType = BBG_TERRAIN;
	else
	{
		bbgType = BBG_SPECIFY;
		bbgLocation = TempNode1.getAttribute("location");
	}
	TempNode1 = MapNode.getChildNode("bg");
	bgHScrollSpeed = atoi(TempNode1.getAttribute("hscrollspeed"));
	bgVScrollSpeed = atoi(TempNode1.getAttribute("vscrollspeed"));
	bgAutoHScroll = (QString(TempNode1.getAttribute("autohscroll")) == "true") ? true:false;
	bgAutoVScroll = (QString(TempNode1.getAttribute("autovscroll")) == "true") ? true:false;
	bgLocation = ProjectData::getAbsoluteResourcePath(TempNode1.getAttribute("location"));
	for (int i = 0; i < MapNode.nChildNode("layer"); ++i)
	{
		LayerData *layer = new LayerData;
		TempNode1 = MapNode.getChildNode("layer", i);
		layer->name = TempNode1.getAttribute("name");
		layer->visible = true;
		layerData = QString(TempNode1.getText()).remove(QRegExp("\\s"));
		for (int j = 0; j < layerData.size(); j += 24)
		{
			TileData tileData;
			unsigned long phasePassability = 0x0;
			for (int k = 0; k < 24; ++k)
				tileDataBase64[k] = layerData[j + k].toLatin1();
			XMLTools::base64Decode(tileDataBase64, inputLength, tileDataBytes, outputLength);
			phasePassability += ((unsigned long)tileDataBytes[0]) << 16;
			phasePassability += ((unsigned long)tileDataBytes[1]) << 8;
			phasePassability += (unsigned long)tileDataBytes[2];
			for (int k = 0; k < NUM_PHASES; ++k)
				tileData.phasePassability[k] = ((phasePassability >> k) % 2 == 1) ? true:false;
			for (int k = 0; k < NUM_PHASES; k += 2)
			{
				tileData.phaseDirections[k / 2] = (tileDataBytes[k + 3] >> 4) % 16;
				tileData.phaseDirections[k / 2 + 1] = tileDataBytes[k + 4] % 16;
			}
			tileData.tilesetID = tileDataBytes[15];
			tileData.tileID = tileDataBytes[16];
			tileData.tileFlags = tileDataBytes[17];
			layer->tileData.append(tileData);
		}
		/*for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				TileData tileData;
				for (int j = 0; j < 24; ++j)
					tileDataBase64[j] = layerData[y * width + x
				unsigned long phasePassability = tileStringList[y * width + x].mid(0, 8).toULong(NULL, 16);
				unsigned long tileInfo = tileStringList[y * width + x].mid(8, 6).toULong(NULL, 16);
				for (int j = 0; j < 32; ++j)
					tileData.phasePassability[j] = ((phasePassability >> j) % 2 == 0) ? false:true;
				tileData.tilesetID = (unsigned char)((tileInfo >> 16) % 256);
				tileData.tileID = (unsigned char)((tileInfo >> 8) % 256);
				tileData.tileFlags = (unsigned char)(tileInfo % 256);
				layer->tileData.append(tileData);
			}
		}*/
		//tileStringList.clear();
		layers.append(layer);
	}
	onLoadEvent = new BaseEvent(BaseEvent::MAPEVENT, MapNode.getChildNode("onloadevent"));
	TempNode1 = MapNode.getChildNode("mapEvents");
	for (int i = 0; i < TempNode1.nChildNode("mapEvent"); ++i)
		events.append(new MapEvent(TempNode1.getChildNode("mapEvent", i)));
	TempNode1 = MapNode.getChildNode("monsterencounters");
	encounterRate = atoi(TempNode1.getAttribute("encounterrate"));
	minPerBattle = atoi(TempNode1.getAttribute("minperbattle"));
	maxPerBattle = atoi(TempNode1.getAttribute("maxperbattle"));
	for (int i = 0; i < TempNode1.nChildNode("monsterencounter"); ++i)
	{
		TempNode2 = TempNode1.getChildNode("monsterencounter", i);
		monsterEncounters.append(0ul);
		monsterEncounters[i] += (QString(TempNode2.getAttribute("monsterid")).toULong() % 65536) << 16;
		monsterEncounters[i] += QString(TempNode2.getAttribute("priority")).toULong() % 65536;
	}
	TempNode1 = MapNode.getChildNode("areas");
	for (int i = 0; i < TempNode1.nChildNode("area"); ++i)
	{
		TempNode2 = TempNode1.getChildNode("area");
		areas.append(new MapArea);
		areas[i]->encounterRate = atoi(TempNode2.getAttribute("encounterrate"));
		areas[i]->minPerBattle = atoi(TempNode2.getAttribute("minperbattle"));
		areas[i]->maxPerBattle = atoi(TempNode2.getAttribute("maxperbattle"));
		areas[i]->x = atoi(TempNode2.getAttribute("x"));
		areas[i]->y = atoi(TempNode2.getAttribute("y"));
		areas[i]->w = atoi(TempNode2.getAttribute("width"));
		areas[i]->h = atoi(TempNode2.getAttribute("height"));
		for (int j = 0; j < TempNode2.nChildNode("monsterencounter"); ++j)
		{
			TempNode3 = TempNode2.getChildNode("monsterencounter", j);
			areas[i]->monsterEncounters.append(0ul);
			areas[i]->monsterEncounters[j] += (QString(TempNode3.getAttribute("monsterid")).toULong() % 65536) << 16;
			areas[i]->monsterEncounters[j] += QString(TempNode3.getAttribute("priority")).toULong() % 65536;
		}
	}
}

Map::Map(StorageFile &storageFile)
{
	int temp_int1, temp_int2;
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	width = storageFile.getSignedInt();
	height = storageFile.getSignedInt();
	parent = storageFile.getSignedInt();
	horizontalMode = storageFile.getSignedInt();
	verticalMode = storageFile.getSignedInt();
	borderTile = storageFile.getUnsignedShort();
	bgHScrollSpeed = storageFile.getSignedInt();
	bgVScrollSpeed = storageFile.getSignedInt();
	teleport = storageFile.getSignedInt();
	escape = storageFile.getSignedInt();
	save = storageFile.getSignedInt();
	bgAutoHScroll = storageFile.getBool();
	bgAutoVScroll = storageFile.getBool();
	bgmType = storageFile.getSignedInt();
	if (bgmType == BGM_SPECIFY)
	{
		temp_string = storageFile.getString();
		bgmInfo.location = temp_string;
		delete[] temp_string;
		bgmInfo.volume = storageFile.getFloat();
		bgmInfo.panning = storageFile.getFloat();
		bgmInfo.speed = storageFile.getFloat();
		bgmInfo.fadeInTime = storageFile.getSignedInt();
		bgmInfo.retainPitch = storageFile.getBool();
	}
	bbgType = storageFile.getSignedInt();
	if (bbgType == BBG_SPECIFY)
	{
		temp_string = storageFile.getString();
		bbgLocation = temp_string;
		delete[] temp_string;
	}
	temp_string = storageFile.getString();
	bgLocation = temp_string;
	delete[] temp_string;
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		LayerData *layer = new LayerData;
		temp_string = storageFile.getString();
		layer->name = temp_string;
		delete[] temp_string;
		layer->visible = storageFile.getBool();
		for (int j = 0; j < width * height; ++j)
		{
			TileData tileData;
			unsigned long tileInfo = storageFile.getUnsignedLong();
			unsigned long phasePassability = storageFile.getUnsignedLong();
			for (int k = 0; k < NUM_PHASES; ++k)
			{
				tileData.phaseDirections[k] = storageFile.getUnsignedChar();
				tileData.phasePassability[k] = ((phasePassability >> k) % 2 == 1) ? true:false;
			}
			tileData.tilesetID = (unsigned char)((tileInfo >> 16) % 256);
			tileData.tileID = (unsigned char)((tileInfo >> 8) % 256);
			tileData.tileFlags = (unsigned char)(tileInfo % 256);
			layer->tileData.append(tileData);
		}
		layers.append(layer);
	}
	onLoadEvent = new BaseEvent(BaseEvent::MAPEVENT, storageFile);
	temp_int1 = storageFile.getSignedInt(); // Number of events
	for (int i = 0; i < temp_int1; ++i)
		events.append(new MapEvent(storageFile));
	encounterRate = storageFile.getSignedInt();
	minPerBattle = storageFile.getSignedInt();
	maxPerBattle = storageFile.getSignedInt();
	temp_int1 = storageFile.getSignedInt(); // Number of monster encounters
	for (int i = 0; i < temp_int1; ++i)
		monsterEncounters.append(storageFile.getUnsignedLong());
	temp_int1 = storageFile.getSignedInt(); // Number of areas
	for (int i = 0; i < temp_int1; ++i)
	{
		areas.append(new MapArea);
		areas[i]->encounterRate = storageFile.getSignedInt();
		areas[i]->minPerBattle = storageFile.getSignedInt();
		areas[i]->maxPerBattle = storageFile.getSignedInt();
		areas[i]->x = storageFile.getSignedInt();
		areas[i]->y = storageFile.getSignedInt();
		areas[i]->w = storageFile.getSignedInt();
		areas[i]->h = storageFile.getSignedInt();
		temp_int2 = storageFile.getSignedInt(); // Number of monster encounters
		for (int j = 0; j < temp_int2; ++j)
			areas[i]->monsterEncounters.append(storageFile.getUnsignedLong());
	}
}

Map::~Map()
{
	if (onLoadEvent != NULL)
	{
		delete onLoadEvent;
		onLoadEvent = NULL;
	}
	for (int i = 0; i < layers.size(); ++i)
	{
		if (layers[i] != NULL)
		{
			delete layers[i];
			layers[i] = NULL;
		}
	}
	for (int i = 0; i < events.size(); ++i)
	{
		if (events[i] != NULL)
		{
			delete events[i];
			events[i] = NULL;
		}
	}
	for (int i = 0; i < areas.size(); ++i)
	{
		if (areas[i] != NULL)
		{
			delete areas[i];
			areas[i] = NULL;
		}
	}
	layers.clear();
	events.clear();
	areas.clear();
}

void Map::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putSignedInt(width);
	storageFile.putSignedInt(height);
	storageFile.putSignedInt(parent);
	storageFile.putSignedInt(horizontalMode);
	storageFile.putSignedInt(verticalMode);
	storageFile.putUnsignedShort(borderTile);
	storageFile.putSignedInt(bgHScrollSpeed);
	storageFile.putSignedInt(bgVScrollSpeed);
	storageFile.putSignedInt(teleport);
	storageFile.putSignedInt(escape);
	storageFile.putSignedInt(save);
	storageFile.putBool(bgAutoHScroll);
	storageFile.putBool(bgAutoVScroll);
	storageFile.putSignedInt(bgmType);
	if (bgmType == BGM_SPECIFY)
	{
		storageFile.putString(bgmInfo.location.toUtf8().data());
		storageFile.putFloat(bgmInfo.volume);
		storageFile.putFloat(bgmInfo.panning);
		storageFile.putFloat(bgmInfo.speed);
		storageFile.putSignedInt(bgmInfo.fadeInTime);
		storageFile.putBool(bgmInfo.retainPitch);
	}
	storageFile.putSignedInt(bbgType);
	if (bbgType == BBG_SPECIFY)
		storageFile.putString(bbgLocation.toUtf8().data());
	storageFile.putString(bgLocation.toUtf8().data());
	storageFile.putSignedInt(layers.size());
	for (int i = 0; i < layers.size(); ++i)
	{
		LayerData *layer = layers[i];
		storageFile.putString(layer->name.toUtf8().data());
		storageFile.putBool(layer->visible);
		for (int j = 0; j < width * height; ++j)
		{
			TileData *tileData = &(layer->tileData[j]);
			unsigned long data = 0x0;
			unsigned long phasePassability = 0x0;
			data += ((unsigned long)tileData->tilesetID) << 16;
			data += ((unsigned long)tileData->tileID) << 8;
			data += (unsigned long)tileData->tileFlags;
			for (int k = 0; k < NUM_PHASES; ++k)
				phasePassability += ((tileData->phasePassability[k]) ? 1:0) << k;
			storageFile.putUnsignedLong(data);
			storageFile.putUnsignedLong(phasePassability);
			for (int k = 0; k < NUM_PHASES; ++k)
				storageFile.putUnsignedChar(tileData->phaseDirections[k]);
		}
	}
	onLoadEvent->saveToStorageFile(storageFile);
	storageFile.putSignedInt(events.size());
	for (int i = 0; i < events.size(); ++i)
		events[i]->saveToStorageFile(storageFile);
	storageFile.putSignedInt(encounterRate);
	storageFile.putSignedInt(minPerBattle);
	storageFile.putSignedInt(maxPerBattle);
	storageFile.putSignedInt(monsterEncounters.size());
	for (int i = 0; i < monsterEncounters.size(); ++i)
		storageFile.putUnsignedLong(monsterEncounters[i]);
	storageFile.putSignedInt(areas.size());
	for (int i = 0; i < areas.size(); ++i)
	{
		storageFile.putSignedInt(areas[i]->encounterRate);
		storageFile.putSignedInt(areas[i]->minPerBattle);
		storageFile.putSignedInt(areas[i]->maxPerBattle);
		storageFile.putSignedInt(areas[i]->x);
		storageFile.putSignedInt(areas[i]->y);
		storageFile.putSignedInt(areas[i]->w);
		storageFile.putSignedInt(areas[i]->h);
		storageFile.putSignedInt(areas[i]->monsterEncounters.size());
		for (int j = 0; j < areas[i]->monsterEncounters.size(); ++j)
			storageFile.putUnsignedLong(areas[i]->monsterEncounters[j]);
	}
}

XMLNode Map::getMapNode()
{
	XMLNode MapNode = XMLNode::createXMLTopNode("map");
	XMLNode TempNode1, TempNode2, TempNode3;
	QList<unsigned short> tilemap;
	size_t inputLength = 18;
	size_t outputLength;
	unsigned char tileDataBytes[18];
	char tileDataBase64[25];
	char temp_string[128];
	MapNode.addAttribute("name", name.toUtf8().data());
	MapNode.addAttribute("width", QString::number(width).toUtf8().data());
	MapNode.addAttribute("height", QString::number(height).toUtf8().data());
	MapNode.addAttribute("parent", QString::number(parent).toUtf8().data());
	if (horizontalMode == MODE_LOCKTOEDGE)
		MapNode.addAttribute("horizontalMode", "lockToEdge");
	else if (horizontalMode == MODE_FILLWITHBORDERTILE)
		MapNode.addAttribute("horizontalMode", "fillWithBorderTile");
	else
		MapNode.addAttribute("horizontalMode", "wrapAround");
	if (verticalMode == MODE_LOCKTOEDGE)
		MapNode.addAttribute("verticalMode", "lockToEdge");
	else if (verticalMode == MODE_FILLWITHBORDERTILE)
		MapNode.addAttribute("verticalMode", "fillWithBorderTile");
	else
		MapNode.addAttribute("verticalMode", "wrapAround");
	MapNode.addAttribute("borderTile", QString("%1").arg(borderTile, 4, 16, QChar('0')).toUtf8().data());
	if (teleport == ASINPARENT)
		MapNode.addAttribute("teleport", "parent");
	else if (teleport == ALLOW)
		MapNode.addAttribute("teleport", "allow");
	else
		MapNode.addAttribute("teleport", "forbid");
	if (escape == ASINPARENT)
		MapNode.addAttribute("escape", "parent");
	else if (escape == ALLOW)
		MapNode.addAttribute("escape", "allow");
	else
		MapNode.addAttribute("escape", "forbid");
	if (save == ASINPARENT)
		MapNode.addAttribute("save", "parent");
	else if (save == ALLOW)
		MapNode.addAttribute("save", "allow");
	else
		MapNode.addAttribute("save", "forbid");
	TempNode1 = MapNode.addChild("bgm");
	if (bgmType == BGM_PARENT)
		TempNode1.addAttribute("type", "parent");
	else if (bgmType == BGM_EVENT)
		TempNode1.addAttribute("type", "event");
	else
	{
		TempNode1.addAttribute("type", "specify");
		TempNode1.addAttribute("location", ProjectData::getRelativeResourcePath(bgmInfo.location).toUtf8().data());
		TempNode1.addAttribute("volume", QString::number(bgmInfo.volume).toUtf8().data());
		TempNode1.addAttribute("panning", QString::number(bgmInfo.panning).toUtf8().data());
		TempNode1.addAttribute("speed", QString::number(bgmInfo.speed).toUtf8().data());
		TempNode1.addAttribute("fadeInTime", QString::number(bgmInfo.fadeInTime).toUtf8().data());
		TempNode1.addAttribute("retainPitch", (bgmInfo.retainPitch) ? "true":"false");
	}
	TempNode1 = MapNode.addChild("bbg");
	if (bbgType == BBG_PARENT)
		TempNode1.addAttribute("type", "parent");
	else if (bbgType == BBG_TERRAIN)
		TempNode1.addAttribute("type", "terrain");
	else
	{
		TempNode1.addAttribute("type", "specify");
		TempNode1.addAttribute("location", ProjectData::getRelativeResourcePath(bbgLocation).toUtf8().data());
	}
	TempNode1 = MapNode.addChild("bg");
	sprintf(temp_string, "%d", bgHScrollSpeed);
	TempNode1.addAttribute("hscrollspeed", temp_string);
	sprintf(temp_string, "%d", bgVScrollSpeed);
	TempNode1.addAttribute("vscrollspeed", temp_string);
	sprintf(temp_string, "%s", (bgAutoHScroll) ? "true":"false");
	TempNode1.addAttribute("autohscroll", temp_string);
	sprintf(temp_string, "%s", (bgAutoVScroll) ? "true":"false");
	TempNode1.addAttribute("autovscroll", temp_string);
	TempNode1.addAttribute("location", ProjectData::getRelativeResourcePath(bgLocation).toUtf8().data());
	for (int i = 0; i < layers.size(); ++i)
	{
		LayerData *layer = layers[i];
		TempNode1 = MapNode.addChild("layer");
		TempNode1.addAttribute("name", layer->name.toUtf8().data());
		for (int y = 0; y < height; ++y)
		{
			QString layerData = "";
			for (int x = 0; x < width; ++x)
			{
				TileData *tileData = &(layer->tileData[y * width + x]);
				unsigned long phasePassability = 0x0;
				//unsigned long data = 0x0;
				for (int j = 0; j < NUM_PHASES; ++j)
					phasePassability += ((tileData->phasePassability[j]) ? 1:0) << j;
				tileDataBytes[0] = (unsigned char)((phasePassability >> 16) % 256);
				tileDataBytes[1] = (unsigned char)((phasePassability >> 8) % 256);
				tileDataBytes[2] = (unsigned char)(phasePassability % 256);
				for (int j = 0; j < NUM_PHASES; j += 2)
					tileDataBytes[j / 2 + 3] = (tileData->phaseDirections[j] << 4) + tileData->phaseDirections[j + 1];
				tileDataBytes[15] = tileData->tilesetID;
				tileDataBytes[16] = tileData->tileID;
				tileDataBytes[17] = tileData->tileFlags;
				XMLTools::base64Encode(tileDataBytes, inputLength, tileDataBase64, outputLength);
				tileDataBase64[outputLength] = '\0';
				layerData += tileDataBase64;
				/*layerData += QString("%1").arg(data, 8, 16, QChar('0'));
				data = 0x0;
				data += ((unsigned long)tileData->tilesetID) << 16;
				data += ((unsigned long)tileData->tileID) << 8;
				data += ((unsigned long)tileData->tileFlags);
				layerData += QString("%1 ").arg(data, 6, 16, QChar('0'));*/
			}
			//layerData.resize(layerData.size() - 1);
			TempNode1.addText(layerData.toUtf8().data());
		}
	}
	TempNode1 = onLoadEvent->getEventNode();
	TempNode1.updateName("onLoadEvent");
	MapNode.addChild(TempNode1);
	TempNode1 = MapNode.addChild("mapEvents");
	for (int i = 0; i < events.size(); ++i)
		TempNode1.addChild(events[i]->getMapEventNode());
	TempNode1 = MapNode.addChild("monsterencounters");
	sprintf(temp_string, "%d", encounterRate);
	TempNode1.addAttribute("encounterrate", temp_string);
	sprintf(temp_string, "%d", minPerBattle);
	TempNode1.addAttribute("minperbattle", temp_string);
	sprintf(temp_string, "%d", maxPerBattle);
	TempNode1.addAttribute("maxperbattle", temp_string);
	for (int i = 0; i < monsterEncounters.size(); ++i)
	{
		TempNode2 = TempNode1.addChild("monsterencounter");
		sprintf(temp_string, "%lu", (monsterEncounters[i] >> 16) % 65536ul);
		TempNode2.addAttribute("monsterID", temp_string);
		sprintf(temp_string, "%lu", monsterEncounters[i] % 65535ul);
		TempNode2.addAttribute("priority", temp_string);
	}
	TempNode1 = MapNode.addChild("areas");
	for (int i = 0; i < areas.size(); ++i)
	{
		TempNode2 = TempNode1.addChild("area");
		sprintf(temp_string, "%d", areas[i]->x);
		TempNode2.addAttribute("x", temp_string);
		sprintf(temp_string, "%d", areas[i]->y);
		TempNode2.addAttribute("y", temp_string);
		sprintf(temp_string, "%d", areas[i]->w);
		TempNode2.addAttribute("width", temp_string);
		sprintf(temp_string, "%d", areas[i]->h);
		TempNode2.addAttribute("height", temp_string);
		sprintf(temp_string, "%d", areas[i]->encounterRate);
		TempNode2.addAttribute("encounterrate", temp_string);
		sprintf(temp_string, "%d", areas[i]->minPerBattle);
		TempNode2.addAttribute("minperbattle", temp_string);
		sprintf(temp_string, "%d", areas[i]->maxPerBattle);
		TempNode2.addAttribute("maxperbattle", temp_string);
		for (int j = 0; j < areas[i]->monsterEncounters.size(); ++j)
		{
			TempNode3 = TempNode2.addChild("monsterencounter");
			sprintf(temp_string, "%lu", (monsterEncounters[i] >> 16) % 65536ul);
			TempNode3.addAttribute("monsterID", temp_string);
			sprintf(temp_string, "%lu", monsterEncounters[i] % 65535ul);
			TempNode3.addAttribute("priority", temp_string);
		}
	}
	return MapNode;
}

QString Map::getName() const
{
	return name;
}

void Map::setName(QString value)
{
	name = value;
}

QString Map::getBGLocation() const
{
	return bgLocation;
}

void Map::setBGLocation(QString location)
{
	bgLocation = location;
}

MapEvent* Map::getEvent(int eventID) const
{
	if (eventID >= 0 && eventID < events.size())
		return events[eventID];
	return NULL;
}

void Map::addEvent(MapEvent *event)
{
	events.append(event);
}

void Map::deleteEvent(int eventID)
{
	if (eventID >= 0 && eventID < events.size())
	{
		if (events[eventID] != NULL)
		{
			delete events[eventID];
			events[eventID] = NULL;
		}
		events.removeAt(eventID);
	}
}

int Map::numEvents()
{
	return events.size();
}

BaseEvent *Map::getOnLoadEvent()
{
	return onLoadEvent;
}

Map::MapArea *Map::getArea(int areaID) const
{
	if (areaID >= 0 && areaID < areas.size())
		return areas[areaID];
	return NULL;
}

void Map::addArea(Map::MapArea *area)
{
	areas.append(area);
}

void Map::deleteArea(int areaID)
{
	if (areaID >= 0 && areaID < areas.size())
	{
		if (areas[areaID] != NULL)
		{
			delete areas[areaID];
			areas[areaID] = NULL;
		}
		areas.removeAt(areaID);
	}
}

int Map::numAreas()
{
	return areas.size();
}

unsigned long Map::getMonsterEncounter(int encounterID) const
{
	if (encounterID >= 0 && encounterID < monsterEncounters.size())
		return monsterEncounters[encounterID];
	return 0ul;
}

void Map::addMonsterEncounter(unsigned long encounter)
{
	monsterEncounters.append(encounter);
}

void Map::deleteMonsterEncounter(int encounterID)
{
	if (encounterID >= 0 && encounterID < monsterEncounters.size())
		monsterEncounters.removeAt(encounterID);
}

int Map::numMonsterEncounters()
{
	return monsterEncounters.size();
}

unsigned char Map::getTileFlags(int x, int y, int layer) const
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
		return layers[layer]->tileData[y * width + x].tileFlags;
	return 0u;
}

unsigned char Map::getTilesetID(int x, int y, int layer) const
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
		return layers[layer]->tileData[y * width + x].tilesetID;
	return 0u;
}

unsigned char Map::getTileID(int x, int y, int layer) const
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
		return layers[layer]->tileData[y * width + x].tileID;
	return 0u;
}

unsigned char Map::getPhaseDirections(int x, int y, int layer, int phaseID) const
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
		return layers[layer]->tileData[y * width + x].phaseDirections[phaseID];
	return 0x0;
}

bool Map::isPhaseOn(int x, int y, int layer, int phaseID) const
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size() && phaseID >= 0 && phaseID < 32)
		return layers[layer]->tileData[y * width + x].phasePassability[phaseID];
	return false;
}

Map::TileData Map::getTileData(int x, int y, int layer) const
{
	TileData *pTileData;
	TileData returnTileData;
	returnTileData.tilesetID = 0x0;
	returnTileData.tileID = 0x0;
	returnTileData.tileFlags = 0x0;
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		returnTileData.phaseDirections[i] = 0x0;
		returnTileData.phasePassability[i] = false;
	}
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
	{
		pTileData = &(layers[layer]->tileData[y * width + x]);
		returnTileData.tilesetID = pTileData->tilesetID;
		returnTileData.tileID = pTileData->tileID;
		returnTileData.tileFlags = pTileData->tileFlags;
		for (int i = 0; i < NUM_PHASES; ++i)
		{
			returnTileData.phaseDirections[i] = pTileData->phaseDirections[i];
			returnTileData.phasePassability[i] = pTileData->phasePassability[i];
		}
	}
	return returnTileData;
}

Map::LayerData *Map::getLayerData(int layer)
{
	if (layer >= 0 && layer < layers.size())
		return layers[layer];
	return NULL;
}

QString Map::getLayerName(int layer) const
{
	return layers[layer]->name;
}

void Map::addLayer(QString name)
{
	LayerData *layer = new LayerData;
	layer->name = name;
	layer->visible = true;
	for (int i = 0; i < width * height; ++i)
	{
		TileData tileData;
		tileData.tilesetID = 0x0;
		tileData.tileID = 0x0;
		tileData.tileFlags = 0x0;
		for (int j = 0; j < NUM_PHASES; ++j)
		{
			tileData.phaseDirections[j] = 0x0;
			tileData.phasePassability[j] = false;
		}
		layer->tileData.append(tileData);
	}
	layers.append(layer);
}

void Map::deleteLayer(int layer)
{
	delete (layers[layer]);
	layers.removeAt(layer);
}

void Map::moveLayerUp(int layer)
{
	layers.swap(layer, layer - 1);
}

void Map::moveLayerDown(int layer)
{
	layers.swap(layer, layer + 1);
}

void Map::setLayerName(int layer, QString name)
{
	layers[layer]->name = name;
}

bool Map::isLayerVisible(int layer) const
{
	return layers[layer]->visible;
}

void Map::setLayerVisible(int layer, bool visible)
{
	layers[layer]->visible = visible;
}

void Map::setTileID(int x, int y, int layer, unsigned char tilesetID, unsigned char tileID, bool autocalc)
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
	{
		TileData *pTileData = &(layers[layer]->tileData[y * width + x]);
		ProjectData::DataReference<Tileset> *tilesetRef = new ProjectData::DataReference<Tileset>((int)tilesetID, __FILE__, __LINE__);
		pTileData->tilesetID = tilesetID;
		pTileData->tileID = tileID;
		if (((*tilesetRef)->getTile((int)tileID)->isAutoTile()) && autocalc)
			autoCalcTileFlags(x, y, layer);
		delete tilesetRef;
	}
}

void Map::setTileFlags(int x, int y, int layer, unsigned char flags)
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
		layers[layer]->tileData[y * width + x].tileFlags = flags;
}

void Map::setPhaseDirections(int x, int y, int layer, int phaseID, unsigned char directions)
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
		layers[layer]->tileData[y * width + x].phaseDirections[phaseID] = directions;
}

void Map::setPhaseOn(int x, int y, int layer, int phaseID, bool on)
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
		layers[layer]->tileData[y * width + x].phasePassability[phaseID] = on;
}

void Map::setTileData(int x, int y, int layer, const TileData &tileData)
{
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
	{
		TileData *pData = &(layers[layer]->tileData[y * width + x]);
		pData->tilesetID = tileData.tilesetID;
		pData->tileID = tileData.tileID;
		pData->tileFlags = tileData.tileFlags;
		for (int i = 0; i < NUM_PHASES; ++i)
		{
			pData->phaseDirections[i] = tileData.phaseDirections[i];
			pData->phasePassability[i] = tileData.phasePassability[i];
		}
	}
}

void Map::autoCalcTileFlags(int x, int y, int layer, bool adjust)
{
	ProjectData::DataReference<Tileset> *tilesetRef;
	TileData *pTile1;
	TileData *pTile2;
	int x2, y2, tileGroup1, tileGroup2, borderTileGroup;
	unsigned short borderTilesetID = (borderTile >> 8) % 256;
	unsigned short borderTileID = borderTile % 256;
	tilesetRef = new ProjectData::DataReference<Tileset>(borderTilesetID, __FILE__, __LINE__);
	borderTileGroup = (*tilesetRef)->getTile(borderTileID)->getTileGroup();
	delete tilesetRef;
	if (x >= 0 && x < width && y >= 0 && y < height && layer >= 0 && layer < layers.size())
	{
		pTile1 = &(layers[layer]->tileData[y * width + x]);
		tilesetRef = new ProjectData::DataReference<Tileset>(pTile1->tilesetID, __FILE__, __LINE__);
		tileGroup1 = (*tilesetRef)->getTile(pTile1->tileID)->getTileGroup();
		pTile1->tileFlags = 0x0;
		if ((*tilesetRef)->getTile(pTile1->tileID)->isAutoTile())
		{
			delete tilesetRef;
			x2 = x - 1;
			y2 = y - 1;
			if (horizontalMode == MODE_WRAPAROUND && x2 < 0)
				x2 += width;
			if (verticalMode == MODE_WRAPAROUND && y2 < 0)
				y2 += height;
			if (x2 >= 0 && y2 >= 0)
			{
				pTile2 = &(layers[layer]->tileData[y2 * width + x2]);
				tilesetRef = new ProjectData::DataReference<Tileset>(pTile2->tilesetID, __FILE__, __LINE__);
				tileGroup2 = (*tilesetRef)->getTile(pTile2->tileID)->getTileGroup();
				delete tilesetRef;
				if (pTile1->tilesetID == pTile2->tilesetID && ((pTile1->tileID == pTile2->tileID) || (tileGroup1 != -1 && tileGroup1 == tileGroup2)))
				{
					if ((pTile2->tileFlags & Tile::Tile_se) == 0 && adjust)
						pTile2->tileFlags += Tile::Tile_se;
					pTile1->tileFlags += Tile::Tile_nw;
				}
				else if ((pTile2->tileFlags & Tile::Tile_se) != 0 && adjust)
					pTile2->tileFlags -= Tile::Tile_se;
			}
			else if (pTile1->tilesetID == borderTilesetID && ((pTile1->tileID == borderTileID) || (tileGroup1 != -1 && tileGroup1 == borderTileGroup)))
				pTile1->tileFlags += Tile::Tile_nw;
			x2 = x;
			y2 = y - 1;
			if (verticalMode == MODE_WRAPAROUND && y2 < 0)
				y2 += height;
			if (y2 >= 0)
			{
				pTile2 = &(layers[layer]->tileData[y2 * width + x2]);
				tilesetRef = new ProjectData::DataReference<Tileset>(pTile2->tilesetID, __FILE__, __LINE__);
				tileGroup2 = (*tilesetRef)->getTile(pTile2->tileID)->getTileGroup();
				delete tilesetRef;
				if (pTile1->tilesetID == pTile2->tilesetID && ((pTile1->tileID == pTile2->tileID) || (tileGroup1 != -1 && tileGroup1 == tileGroup2)))
				{
					if ((pTile2->tileFlags & Tile::Tile_s) == 0 && adjust)
						pTile2->tileFlags += Tile::Tile_s;
					pTile1->tileFlags += Tile::Tile_n;
				}
				else if ((pTile2->tileFlags & Tile::Tile_s) != 0 && adjust)
					pTile2->tileFlags -= Tile::Tile_s;
			}
			else if (pTile1->tilesetID == borderTilesetID && ((pTile1->tileID == borderTileID) || (tileGroup1 != -1 && tileGroup1 == borderTileGroup)))
				pTile1->tileFlags += Tile::Tile_n;
			x2 = x + 1;
			y2 = y - 1;
			if (horizontalMode == MODE_WRAPAROUND && x2 >= width)
				x2 -= width;
			if (verticalMode == MODE_WRAPAROUND && y2 < 0)
				y2 += height;
			if (y2 >= 0 && x2 < width)
			{
				pTile2 = &(layers[layer]->tileData[y2 * width + x2]);
				tilesetRef = new ProjectData::DataReference<Tileset>(pTile2->tilesetID, __FILE__, __LINE__);
				tileGroup2 = (*tilesetRef)->getTile(pTile2->tileID)->getTileGroup();
				delete tilesetRef;
				if (pTile1->tilesetID == pTile2->tilesetID && ((pTile1->tileID == pTile2->tileID) || (tileGroup1 != -1 && tileGroup1 == tileGroup2)))
				{
					if ((pTile2->tileFlags & Tile::Tile_sw) == 0 && adjust)
						pTile2->tileFlags += Tile::Tile_sw;
					pTile1->tileFlags += Tile::Tile_ne;
				}
				else if ((pTile2->tileFlags & Tile::Tile_sw) != 0 && adjust)
					pTile2->tileFlags -= Tile::Tile_sw;
			}
			else if (pTile1->tilesetID == borderTilesetID && ((pTile1->tileID == borderTileID) || (tileGroup1 != -1 && tileGroup1 == borderTileGroup)))
				pTile1->tileFlags += Tile::Tile_ne;
			x2 = x - 1;
			y2 = y;
			if (horizontalMode == MODE_WRAPAROUND && x2 < 0)
				x2 += width;
			if (x2 >= 0)
			{
				pTile2 = &(layers[layer]->tileData[y2 * width + x2]);
				tilesetRef = new ProjectData::DataReference<Tileset>(pTile2->tilesetID, __FILE__, __LINE__);
				tileGroup2 = (*tilesetRef)->getTile(pTile2->tileID)->getTileGroup();
				delete tilesetRef;
				if (pTile1->tilesetID == pTile2->tilesetID && ((pTile1->tileID == pTile2->tileID) || (tileGroup1 != -1 && tileGroup1 == tileGroup2)))
				{
					if ((pTile2->tileFlags & Tile::Tile_e) == 0 && adjust)
						pTile2->tileFlags += Tile::Tile_e;
					pTile1->tileFlags += Tile::Tile_w;
				}
				else if ((pTile2->tileFlags & Tile::Tile_e) != 0 && adjust)
					pTile2->tileFlags -= Tile::Tile_e;
			}
			else if (pTile1->tilesetID == borderTilesetID && ((pTile1->tileID == borderTileID) || (tileGroup1 != -1 && tileGroup1 == borderTileGroup)))
				pTile1->tileFlags += Tile::Tile_w;
			x2 = x + 1;
			y2 = y;
			if (horizontalMode == MODE_WRAPAROUND && x2 >= width)
				x2 -= width;
			if (x2 < width)
			{
				pTile2 = &(layers[layer]->tileData[y2 * width + x2]);
				tilesetRef = new ProjectData::DataReference<Tileset>(pTile2->tilesetID, __FILE__, __LINE__);
				tileGroup2 = (*tilesetRef)->getTile(pTile2->tileID)->getTileGroup();
				delete tilesetRef;
				if (pTile1->tilesetID == pTile2->tilesetID && ((pTile1->tileID == pTile2->tileID) || (tileGroup1 != -1 && tileGroup1 == tileGroup2)))
				{
					if ((pTile2->tileFlags & Tile::Tile_w) == 0 && adjust)
						pTile2->tileFlags += Tile::Tile_w;
					pTile1->tileFlags += Tile::Tile_e;
				}
				else if ((pTile2->tileFlags & Tile::Tile_w) != 0 && adjust)
					pTile2->tileFlags -= Tile::Tile_w;
			}
			else if (pTile1->tilesetID == borderTilesetID && ((pTile1->tileID == borderTileID) || (tileGroup1 != -1 && tileGroup1 == borderTileGroup)))
				pTile1->tileFlags += Tile::Tile_e;
			x2 = x - 1;
			y2 = y + 1;
			if (horizontalMode == MODE_WRAPAROUND && x2 < 0)
				x2 += width;
			if (verticalMode == MODE_WRAPAROUND && y2 >= height)
				y2 -= height;
			if (x2 >= 0 && y2 < height)
			{
				pTile2 = &(layers[layer]->tileData[y2 * width + x2]);
				tilesetRef = new ProjectData::DataReference<Tileset>(pTile2->tilesetID, __FILE__, __LINE__);
				tileGroup2 = (*tilesetRef)->getTile(pTile2->tileID)->getTileGroup();
				delete tilesetRef;
				if (pTile1->tilesetID == pTile2->tilesetID && ((pTile1->tileID == pTile2->tileID) || (tileGroup1 != -1 && tileGroup1 == tileGroup2)))
				{
					if ((pTile2->tileFlags & Tile::Tile_ne) == 0 && adjust)
						pTile2->tileFlags += Tile::Tile_ne;
					pTile1->tileFlags += Tile::Tile_sw;
				}
				else if ((pTile2->tileFlags & Tile::Tile_ne) != 0 && adjust)
					pTile2->tileFlags -= Tile::Tile_ne;
			}
			else if (pTile1->tilesetID == borderTilesetID && ((pTile1->tileID == borderTileID) || (tileGroup1 != -1 && tileGroup1 == borderTileGroup)))
				pTile1->tileFlags += Tile::Tile_sw;
			x2 = x;
			y2 = y + 1;
			if (verticalMode == MODE_WRAPAROUND && y2 >= height)
				y2 -= height;
			if (y2 < height)
			{
				pTile2 = &(layers[layer]->tileData[y2 * width + x2]);
				tilesetRef = new ProjectData::DataReference<Tileset>(pTile2->tilesetID, __FILE__, __LINE__);
				tileGroup2 = (*tilesetRef)->getTile(pTile2->tileID)->getTileGroup();
				delete tilesetRef;
				if (pTile1->tilesetID == pTile2->tilesetID && ((pTile1->tileID == pTile2->tileID) || (tileGroup1 != -1 && tileGroup1 == tileGroup2)))
				{
					if ((pTile2->tileFlags & Tile::Tile_n) == 0 && adjust)
						pTile2->tileFlags += Tile::Tile_n;
					pTile1->tileFlags += Tile::Tile_s;
				}
				else if ((pTile2->tileFlags & Tile::Tile_n) != 0 && adjust)
					pTile2->tileFlags -= Tile::Tile_n;
			}
			else if (pTile1->tilesetID == borderTilesetID && ((pTile1->tileID == borderTileID) || (tileGroup1 != -1 && tileGroup1 == borderTileGroup)))
				pTile1->tileFlags += Tile::Tile_s;
			x2 = x + 1;
			y2 = y + 1;
			if (horizontalMode == MODE_WRAPAROUND && x2 >= width)
				x2 -= width;
			if (verticalMode == MODE_WRAPAROUND && y2 >= height)
				y2 -= height;
			if (x2 < width && y2 < height)
			{
				pTile2 = &(layers[layer]->tileData[y2 * width + x2]);
				tilesetRef = new ProjectData::DataReference<Tileset>(pTile2->tilesetID, __FILE__, __LINE__);
				tileGroup2 = (*tilesetRef)->getTile(pTile2->tileID)->getTileGroup();
				delete tilesetRef;
				if (pTile1->tilesetID == pTile2->tilesetID && ((pTile1->tileID == pTile2->tileID) || (tileGroup1 != -1 && tileGroup1 == tileGroup2)))
				{
					if ((pTile2->tileFlags & Tile::Tile_nw) == 0 && adjust)
						pTile2->tileFlags += Tile::Tile_nw;
					pTile1->tileFlags += Tile::Tile_se;
				}
				else if ((pTile2->tileFlags & Tile::Tile_nw) != 0 && adjust)
					pTile2->tileFlags -= Tile::Tile_nw;
			}
			else if (pTile1->tilesetID == borderTilesetID && ((pTile1->tileID == borderTileID) || (tileGroup1 != -1 && tileGroup1 == borderTileGroup)))
				pTile1->tileFlags += Tile::Tile_se;
		}
		else
			delete tilesetRef;
	}
}

int Map::getEncounterRate() const
{
	return encounterRate;
}

void Map::setEncounterRate(int value)
{
	encounterRate = value;
}

int Map::getMinPerBattle() const
{
	return minPerBattle;
}

void Map::setMinPerBattle(int value)
{
	minPerBattle = value;
}

int Map::getMaxPerBattle() const
{
	return maxPerBattle;
}

void Map::setMaxPerBattle(int value)
{
	maxPerBattle = value;
}

int Map::getWidth() const
{
	return width;
}

int Map::getHeight() const
{
	return height;
}

int Map::numLayers() const
{
	return layers.size();
}

void Map::resizeMap(int newWidth, int newHeight)
{
	if (newWidth != width || newHeight != height)
	{
		for (int i = 0; i < layers.size(); ++i)
		{
			QList<TileData> oldData = layers[i]->tileData;
			TileData newData;
			layers[i]->tileData.clear();
			for (int y = 0; y < newHeight; ++y)
			{
				for (int x = 0; x < newWidth; ++x)
				{
					if (x < width && y < height)
					{
						newData.tilesetID = oldData[y * width + x].tilesetID;
						newData.tileID = oldData[y * width + x].tileID;
						newData.tileFlags = oldData[y * width + x].tileFlags;
						for (int j = 0; j < NUM_PHASES; ++j)
						{
							newData.phaseDirections[j] = oldData[y * width + x].phaseDirections[j];
							newData.phasePassability[j] = oldData[y * width + x].phasePassability[j];
						}
					}
					else
					{
						newData.tilesetID = 0x0;
						newData.tileID = 0x0;
						newData.tileID = 0x0;
						newData.tileFlags = 0x0;
						for (int j = 0; j < NUM_PHASES; ++j)
						{
							newData.phaseDirections[j] = 0x0;
							newData.phasePassability[j] = false;
						}
					}
					layers[i]->tileData.append(newData);
				}
			}
			oldData.clear();
		}
		width = newWidth;
		height = newHeight;
		for (int i = 0; i < events.size(); ++i)
		{
			if (events[i]->getXLocation() >= width || events[i]->getYLocation() >= height)
				moveEventInsideMap(i);
		}
	}
}

void Map::cropMap(int xOffset, int yOffset, int newWidth, int newHeight)
{
	int x2, y2;
	if (newWidth != width || newHeight != height)
	{
		for (int i = 0; i < layers.size(); ++i)
		{
			QList<TileData> oldData = layers[i]->tileData;
			TileData newData;
			layers[i]->tileData.clear();
			for (int y = 0; y < newHeight; ++y)
			{
				for (int x = 0; x < newWidth; ++x)
				{
					x2 = x + xOffset;
					y2 = y + yOffset;
					newData = oldData[y2 * width + x2];
					layers[i]->tileData.append(newData);
				}
			}
			oldData.clear();
		}
		width = newWidth;
		height = newHeight;
		for (int i = 0; i < events.size(); ++i)
		{
			int eventX = events[i]->getXLocation();
			int eventY = events[i]->getYLocation();
			eventX -= xOffset;
			eventY -= yOffset;
			events[i]->setLocation(eventX, eventY);
			if (eventX < 0 || eventX >= width || eventY < 0 || eventY >= height)
				moveEventInsideMap(i);
		}
	}
}

int Map::getBGHScrollSpeed() const
{
	return bgHScrollSpeed;
}

void Map::setBGHScrollSpeed(int value)
{
	bgHScrollSpeed = value;
}

bool Map::getBGAutoHScroll() const
{
	return bgAutoHScroll;
}

void Map::setBGAutoHScroll(bool value)
{
	bgAutoHScroll = value;
}

int Map::getBGVScrollSpeed() const
{
	return bgVScrollSpeed;
}

void Map::setBGVScrollSpeed(int value)
{
	bgVScrollSpeed = value;
}

bool Map::getBGAutoVScroll() const
{
	return bgAutoVScroll;
}

void Map::setBGAutoVScroll(bool value)
{
	bgAutoVScroll = value;
}

int Map::getHorizontalMode() const
{
	return horizontalMode;
}

void Map::setHorizontalMode(int value)
{
	horizontalMode = value;
}

int Map::getVerticalMode() const
{
	return verticalMode;
}

void Map::setVerticalMode(int value)
{
	verticalMode = value;
}

int Map::getParent() const
{
	return parent;
}

void Map::setParent(int value)
{
	parent = value;
	if (parent == -1)
	{
		if (teleport == ASINPARENT)
			teleport = ALLOW;
		if (escape == ASINPARENT)
			escape = ALLOW;
		if (save == ASINPARENT)
			save = ALLOW;
		if (bgmType == BGM_PARENT)
			bgmType = BGM_EVENT;
		if (bbgType == BBG_PARENT)
			bbgType = BBG_TERRAIN;
	}
}

int Map::getTeleport() const
{
	return teleport;
}

void Map::setTeleport(int value)
{
	teleport = value;
}

int Map::getEscape() const
{
	return escape;
}

void Map::setEscape(int value)
{
	escape = value;
}

int Map::getSave() const
{
	return save;
}

void Map::setSave(int value)
{
	save = value;
}

void Map::getBGMInfo(AudioManager::MusicInfo *info)
{
	info->location = bgmInfo.location;
	info->volume = bgmInfo.volume;
	info->panning = bgmInfo.panning;
	info->speed = bgmInfo.speed;
	info->fadeInTime = bgmInfo.fadeInTime;
	info->retainPitch = bgmInfo.retainPitch;
}

void Map::setBGMInfo(AudioManager::MusicInfo info)
{
	bgmInfo.location = info.location;
	bgmInfo.volume = info.volume;
	bgmInfo.panning = info.panning;
	bgmInfo.speed = info.speed;
	bgmInfo.fadeInTime = info.fadeInTime;
	bgmInfo.retainPitch = info.retainPitch;
}

int Map::getBGMType() const
{
	return bgmType;
}

void Map::setBGMType(int value)
{
	bgmType = value;
}

QString Map::getBBGLocation() const
{
	return bbgLocation;
}

void Map::setBBGLocation(QString value)
{
	bbgLocation = value;
}

int Map::getBBGType()
{
	return bbgType;
}

void Map::setBBGType(int value)
{
	bbgType = value;
}

unsigned short Map::getBorderTile() const
{
	return borderTile;
}

void Map::setBorderTile(unsigned short value)
{
	borderTile = value;
}

bool Map::eventAtLocation(QPoint &pos)
{
	for (int i = 0; i < events.size(); ++i)
	{
		if (pos == QPoint(events[i]->getXLocation(), events[i]->getYLocation()))
			return true;
	}
	return false;
}

int Map::eventIDAtLocation(QPoint &pos)
{
	for (int i = 0; i < events.size(); ++i)
	{
		if (pos == QPoint(events[i]->getXLocation(), events[i]->getYLocation()))
			return i;
	}
	return -1;
}

void Map::autoCalcAllTileFlags(int layer)
{
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
			autoCalcTileFlags(x, y, layer);
}

void Map::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	for (int i = 0; i < layers.size(); ++i)
	{
		for (int j = 0; j < width * height; ++j)
		{
			TileData *pTileData = &(layers[i]->tileData[j]);
			for (int k = 0; k < swappedTiles.size(); ++k)
			{
				if ((int)pTileData->tilesetID == swappedTiles[k].tileset)
				{
					if ((int)pTileData->tileID == swappedTiles[k].tile1)
						pTileData->tileID = (unsigned char)swappedTiles[k].tile2;
					else if ((int)pTileData->tileID == swappedTiles[k].tile2)
						pTileData->tileID = (unsigned char)swappedTiles[k].tile1;
				}
			}
		}
	}
	for (int i = 0; i < events.size(); ++i)
		events[i]->applySwappedTiles(swappedTiles);
	if (onLoadEvent != NULL)
		onLoadEvent->applySwappedTiles(swappedTiles);
}

bool Map::tryLoad(XMLNode MapNode)
{
	XMLNode TempNode1, TempNode2, TempNode3;
	QStringList list;
	int tempParent;
	if (!XMLTools::attributeExists(MapNode, "name"))
		return false;
	if (!XMLTools::attributeExists(MapNode, "width"))
		return false;
	if (!XMLTools::attributeIntRangeValid(MapNode, "width", 1, 512))
		return false;
	if (!XMLTools::attributeExists(MapNode, "height"))
		return false;
	if (!XMLTools::attributeIntRangeValid(MapNode, "height", 1, 512))
		return false;
	if (!XMLTools::attributeExists(MapNode, "horizontalMode"))
		return false;
	if (!XMLTools::attributeStringValid(MapNode, "horizontalMode", QStringList() << "lockToEdge" << "fillWithBorderTile" << "wrapAround", 2))
		return false;
	if (!XMLTools::attributeExists(MapNode, "verticalMode"))
		return false;
	if (!XMLTools::attributeStringValid(MapNode, "verticalMode", QStringList() << "lockToEdge" << "fillWithBorderTile" << "wrapAround", 2))
		return false;
	if (!XMLTools::attributeExists(MapNode, "borderTile"))
		return false;
	if (!XMLTools::attributeExists(MapNode, "parent"))
		return false;
	tempParent = atoi(MapNode.getAttribute("parent"));
	if (!XMLTools::attributeExists(MapNode, "teleport"))
		return false;
	if (!XMLTools::attributeExists(MapNode, "escape"))
		return false;
	if (!XMLTools::attributeExists(MapNode, "save"))
		return false;
	if (tempParent == -1)
	{
		XMLTools::attributeStringValid(MapNode, "teleport", QStringList() << "allow" << "forbid", 0);
		XMLTools::attributeStringValid(MapNode, "escape", QStringList() << "allow" << "forbid", 0);
		XMLTools::attributeStringValid(MapNode, "save", QStringList() << "allow" << "forbid", 0);
	}
	else
	{
		XMLTools::attributeStringValid(MapNode, "teleport", QStringList() << "parent" << "allow" << "forbid", 2);
		XMLTools::attributeStringValid(MapNode, "escape", QStringList() << "parent" << "allow" << "forbid", 2);
		XMLTools::attributeStringValid(MapNode, "save", QStringList() << "parent" << "allow" << "forbid", 2);
	}
	if (!XMLTools::nodeExists(MapNode, "bgm"))
		return false;
	TempNode1 = MapNode.getChildNode("bgm");
	if (!XMLTools::attributeExists(TempNode1, "type"))
		return false;
	if (tempParent == -1)
		XMLTools::attributeStringValid(TempNode1, "type", QStringList() << "event" << "specify", 1);
	else
		XMLTools::attributeStringValid(TempNode1, "type", QStringList() << "parent" << "event" << "specify", 2);
	if (QString(TempNode1.getAttribute("type")) == "specify")
	{
		if (!XMLTools::attributeExists(TempNode1, "location"))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "volume"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(TempNode1, "volume", 0.0, 1.0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "panning"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(TempNode1, "panning", -1.0, 1.0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "speed"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(TempNode1, "speed", 0.5, 2.0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "fadeInTime"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode1, "fadeInTiem", 0, 10))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "retainPitch"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "retainPitch", QStringList() << "true" << "false", 1))
			return false;
	}
	if (!XMLTools::nodeExists(MapNode, "bbg"))
		return false;
	TempNode1 = MapNode.getChildNode("bbg");
	if (!XMLTools::attributeExists(TempNode1, "type"))
		return false;
	if (tempParent == -1)
		XMLTools::attributeStringValid(TempNode1, "type", QStringList() << "terrain" << "specify", 0);
	else
		XMLTools::attributeStringValid(TempNode1, "type", QStringList() << "parent" << "terrain" << "specify", 2);
	if (QString(TempNode1.getAttribute("type")) == "specify" && !XMLTools::attributeExists(TempNode1, "location"))
		return false;
	if (!XMLTools::nodeExists(MapNode, "bg"))
		return false;
	TempNode1 = MapNode.getChildNode("bg");
	if (!XMLTools::attributeExists(TempNode1, "hscrollspeed"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "vscrollspeed"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "hscrollspeed", -16, 16))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "vscrollspeed", -16, 16))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "autohscroll"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "autovscroll"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "location"))
		return false;
	if (!XMLTools::nodesExist(MapNode, "layer", 1, 99))
		return false;
	for (int i = 0; i < MapNode.nChildNode("layer"); ++i)
	{
		TempNode1 = MapNode.getChildNode("layer", i);
		if (!XMLTools::attributeExists(TempNode1, "name"))
			return false;
	}
	if (!XMLTools::nodeExists(MapNode, "onloadevent"))
		return false;
	TempNode1 = MapNode.getChildNode("onloadevent");
	if (!BaseEvent::tryLoad(TempNode1, BaseEvent::MAPEVENT))
		return false;
	if (!XMLTools::nodeExists(MapNode, "mapEvents"))
		return false;
	TempNode1 = MapNode.getChildNode("mapEvents");
	for (int i = 0; i < TempNode1.nChildNode("event"); ++i)
	{
		if (!MapEvent::tryLoad(TempNode1.getChildNode("event", i)))
			return false;
	}
	if (!XMLTools::nodeExists(MapNode, "monsterencounters"))
		return false;
	TempNode1 = MapNode.getChildNode("monsterencounters");
	if (!XMLTools::attributeExists(TempNode1, "encounterrate"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "minperbattle"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "maxperbattle"))
		return false;
	for (int i = 0, count = TempNode1.nChildNode("monsterencounter"); i < count; ++i)
	{
		TempNode2 = TempNode1.getChildNode("monsterencounter", i);
		if (!XMLTools::attributeExists(TempNode2, "monsterid"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "priority"))
			return false;
	}
	if (!XMLTools::nodeExists(MapNode, "areas"))
		return false;
	TempNode1 = MapNode.getChildNode("areas");
	for (int i = 0, count = TempNode1.nChildNode("area"); i < count; ++i)
	{
		TempNode2 = TempNode1.getChildNode("area", i);
		if (!XMLTools::attributeExists(TempNode2, "encounterrate"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "minperbattle"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "maxperbattle"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "x"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "y"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "width"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "height"))
			return false;
		for (int j = 0, count2 = TempNode2.nChildNode("monsterencounter"); j < count2; ++j)
		{
			TempNode3 = TempNode2.getChildNode("monsterencounter", j);
			if (!XMLTools::attributeExists(TempNode3, "monsterid"))
				return false;
			if (!XMLTools::attributeExists(TempNode3, "priority"))
				return false;
		}
	}
	return true;
}

void Map::replaceTilesetReferences(int oldID, int newID)
{
	if ((borderTile >> 8) % 256 == oldID)
		borderTile = ((unsigned short)oldID << 8) + (borderTile % 256);
	for (int i = 0; i < layers.size(); ++i)
	{
		for (int j = 0; j < width * height; ++j)
		{
			if (layers[i]->tileData[j].tilesetID == oldID)
				layers[i]->tileData[j].tilesetID = newID;
		}
	}
}

void Map::moveEventInsideMap(int eventID)
{
	bool foundSpot = false;
	for (int y = 0; y < height && !foundSpot; ++y)
	{
		for (int x = 0; x < width && !foundSpot; ++x)
		{
			QPoint location = QPoint(x, y);
			if (!eventAtLocation(location))
			{
				events[eventID]->setLocation(x, y);
				foundSpot = true;
			}
		}
	}
}
