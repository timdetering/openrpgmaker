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
#include <cstring>
#include "projectdata.h"
#include "projectdata_charactersprite.h"
#include "xmlParser.h"
#include "xmlTools.h"

CharacterSprite::CharacterSprite()
{
	name = "Untitled";
	mapPoseNames.reserve(32);
	mapPoseLocations.reserve(32);
	battlePoseNames.reserve(32);
	battlePoseLocations.reserve(32);
	for (int i = 0; i < 32; ++i)
	{
		mapPoseNames.append("");
		mapPoseLocations.append("");
		battlePoseNames.append("");
		battlePoseLocations.append("");
	}
	mapPoseNames[MAPPOSE_IDLE] = "Idle";
	mapPoseNames[MAPPOSE_WALKING] = "Walking";
	mapPoseNames[MAPPOSE_JUMPING] = "Jumping";
	mapPoseNames[MAPPOSE_GRAPPLING] = "Grappling";
	mapPoseLocations[MAPPOSE_IDLE] = "";
	mapPoseLocations[MAPPOSE_WALKING] = "";
	mapPoseLocations[MAPPOSE_JUMPING] = "";
	mapPoseLocations[MAPPOSE_GRAPPLING] = "";
	mapPoseNumLoops[MAPPOSE_IDLE] = 1;
	mapPoseNumLoops[MAPPOSE_WALKING] = 1;
	mapPoseNumLoops[MAPPOSE_JUMPING] = 1;
	mapPoseNumLoops[MAPPOSE_GRAPPLING] = 1;
	mapPosePingPong[MAPPOSE_IDLE] = false;
	mapPosePingPong[MAPPOSE_WALKING] = false;
	mapPosePingPong[MAPPOSE_JUMPING] = false;
	mapPosePingPong[MAPPOSE_GRAPPLING] = false;
	for (int i = 4; i < 32; ++i)
	{
		mapPoseNames[i] = "";
		mapPoseLocations[i] = "";
		mapPoseNumLoops[i] = 1;
		mapPosePingPong[i] = false;
	}
	battlePoseNames[BATTLEPOSE_IDLESTANCE] = "Idle Stance";
	battlePoseNames[BATTLEPOSE_ATTACK] = "Attack";
	battlePoseNames[BATTLEPOSE_SKILLUSED] = "Skill Used";
	battlePoseNames[BATTLEPOSE_DIEING] = "Dieing";
	battlePoseNames[BATTLEPOSE_DEAD] = "Dead";
	battlePoseNames[BATTLEPOSE_DAMAGED] = "Damaged";
	battlePoseNames[BATTLEPOSE_BADSTATUS] = "Bad Status";
	battlePoseNames[BATTLEPOSE_DEFENDING] = "Defending";
	battlePoseNames[BATTLEPOSE_RUNNINGLEFT] = "Running Left";
	battlePoseNames[BATTLEPOSE_RUNNINGRIGHT] = "Running Right";
	battlePoseNames[BATTLEPOSE_ITEMUSED] = "Item Used";
	battlePoseNames[BATTLEPOSE_WOUNDED] = "Wounded";
	battlePoseNames[BATTLEPOSE_VICTORY] = "Victory";
	battlePoseLocations[BATTLEPOSE_IDLESTANCE] = "";
	battlePoseLocations[BATTLEPOSE_ATTACK] = "";
	battlePoseLocations[BATTLEPOSE_SKILLUSED] = "";
	battlePoseLocations[BATTLEPOSE_DIEING] = "";
	battlePoseLocations[BATTLEPOSE_DEAD] = "";
	battlePoseLocations[BATTLEPOSE_DAMAGED] = "";
	battlePoseLocations[BATTLEPOSE_BADSTATUS] = "";
	battlePoseLocations[BATTLEPOSE_DEFENDING] = "";
	battlePoseLocations[BATTLEPOSE_RUNNINGLEFT] = "";
	battlePoseLocations[BATTLEPOSE_RUNNINGRIGHT] = "";
	battlePoseLocations[BATTLEPOSE_ITEMUSED] = "";
	battlePoseLocations[BATTLEPOSE_WOUNDED] = "";
	battlePoseLocations[BATTLEPOSE_VICTORY] = "";
	battlePoseNumFrames[BATTLEPOSE_IDLESTANCE] = 1;
	battlePoseNumFrames[BATTLEPOSE_ATTACK] = 1;
	battlePoseNumFrames[BATTLEPOSE_SKILLUSED] = 1;
	battlePoseNumFrames[BATTLEPOSE_DIEING] = 1;
	battlePoseNumFrames[BATTLEPOSE_DEAD] = 1;
	battlePoseNumFrames[BATTLEPOSE_DAMAGED] = 1;
	battlePoseNumFrames[BATTLEPOSE_BADSTATUS] = 1;
	battlePoseNumFrames[BATTLEPOSE_DEFENDING] = 1;
	battlePoseNumFrames[BATTLEPOSE_RUNNINGLEFT] = 1;
	battlePoseNumFrames[BATTLEPOSE_RUNNINGRIGHT] = 1;
	battlePoseNumFrames[BATTLEPOSE_ITEMUSED] = 1;
	battlePoseNumFrames[BATTLEPOSE_WOUNDED] = 1;
	battlePoseNumFrames[BATTLEPOSE_VICTORY] = 1;
	battlePoseFPS[BATTLEPOSE_IDLESTANCE] = 12;
	battlePoseFPS[BATTLEPOSE_ATTACK] = 12;
	battlePoseFPS[BATTLEPOSE_SKILLUSED] = 12;
	battlePoseFPS[BATTLEPOSE_DIEING] = 12;
	battlePoseFPS[BATTLEPOSE_DEAD] = 12;
	battlePoseFPS[BATTLEPOSE_DAMAGED] = 12;
	battlePoseFPS[BATTLEPOSE_BADSTATUS] = 12;
	battlePoseFPS[BATTLEPOSE_DEFENDING] = 12;
	battlePoseFPS[BATTLEPOSE_RUNNINGLEFT] = 12;
	battlePoseFPS[BATTLEPOSE_RUNNINGRIGHT] = 12;
	battlePoseFPS[BATTLEPOSE_ITEMUSED] = 12;
	battlePoseFPS[BATTLEPOSE_WOUNDED] = 12;
	battlePoseFPS[BATTLEPOSE_VICTORY] = 12;
	battlePosePingPong[BATTLEPOSE_IDLESTANCE] = false;
	battlePosePingPong[BATTLEPOSE_ATTACK] = false;
	battlePosePingPong[BATTLEPOSE_SKILLUSED] = false;
	battlePosePingPong[BATTLEPOSE_DIEING] = false;
	battlePosePingPong[BATTLEPOSE_DEAD] = false;
	battlePosePingPong[BATTLEPOSE_DAMAGED] = false;
	battlePosePingPong[BATTLEPOSE_BADSTATUS] = false;
	battlePosePingPong[BATTLEPOSE_DEFENDING] = false;
	battlePosePingPong[BATTLEPOSE_RUNNINGLEFT] = false;
	battlePosePingPong[BATTLEPOSE_RUNNINGRIGHT] = false;
	battlePosePingPong[BATTLEPOSE_ITEMUSED] = false;
	battlePosePingPong[BATTLEPOSE_WOUNDED] = false;
	battlePosePingPong[BATTLEPOSE_VICTORY] = false;
	for (int i = 13; i < 32; ++i)
	{
		battlePoseNames[i] = "";
		battlePoseLocations[i] = "";
		battlePoseNumFrames[i] = 1;
		battlePoseFPS[i] = 12;
		battlePosePingPong[i] = false;
	}
}

CharacterSprite::CharacterSprite(CharacterSprite &other)
{
	mapPoseNames.reserve(32);
	mapPoseLocations.reserve(32);
	battlePoseNames.reserve(32);
	battlePoseLocations.reserve(32);
	for (int i = 0; i < 32; ++i)
	{
		mapPoseNames.append("");
		mapPoseLocations.append("");
		battlePoseNames.append("");
		battlePoseLocations.append("");
	}
	name = other.name;
	for (int i = 0; i < 32; ++i)
	{
		mapPoseNames[i] = other.mapPoseNames[i];
		mapPoseLocations[i] = other.mapPoseLocations[i];
		mapPoseNumLoops[i] = other.mapPoseNumLoops[i];
		mapPosePingPong[i] = other.mapPosePingPong[i];
		battlePoseNames[i] = other.battlePoseNames[i];
		battlePoseLocations[i] = other.battlePoseLocations[i];
		battlePoseNumFrames[i] = other.battlePoseNumFrames[i];
		battlePoseFPS[i] = other.battlePoseFPS[i];
		battlePosePingPong[i] = other.battlePosePingPong[i];
	}
}

CharacterSprite::CharacterSprite(const CharacterSprite &other)
{
	mapPoseNames.reserve(32);
	mapPoseLocations.reserve(32);
	battlePoseNames.reserve(32);
	battlePoseLocations.reserve(32);
	for (int i = 0; i < 32; ++i)
	{
		mapPoseNames.append("");
		mapPoseLocations.append("");
		battlePoseNames.append("");
		battlePoseLocations.append("");
	}
	name = other.name;
	for (int i = 0; i < 32; ++i)
	{
		mapPoseNames[i] = other.mapPoseNames[i];
		mapPoseLocations[i] = other.mapPoseLocations[i];
		mapPoseNumLoops[i] = other.mapPoseNumLoops[i];
		mapPosePingPong[i] = other.mapPosePingPong[i];
		battlePoseNames[i] = other.battlePoseNames[i];
		battlePoseLocations[i] = other.battlePoseLocations[i];
		battlePoseNumFrames[i] = other.battlePoseNumFrames[i];
		battlePoseFPS[i] = other.battlePoseFPS[i];
		battlePosePingPong[i] = other.battlePosePingPong[i];
	}
}

CharacterSprite::CharacterSprite(XMLNode SpriteNode)
{
	XMLNode MapSpriteNode, BattleSpriteNode, TempNode;
	mapPoseNames.reserve(32);
	mapPoseLocations.reserve(32);
	battlePoseNames.reserve(32);
	battlePoseLocations.reserve(32);
	for (int i = 0; i < 32; ++i)
	{
		mapPoseNames.append("");
		mapPoseLocations.append("");
		battlePoseNames.append("");
		battlePoseLocations.append("");
	}
	name = SpriteNode.getAttribute("name");
	MapSpriteNode = SpriteNode.getChildNode("mapSprite");
	BattleSpriteNode = SpriteNode.getChildNode("battleSprite");
	for (int i = 0; i < 32; ++i)
	{
		TempNode = MapSpriteNode.getChildNode("pose", i);
		mapPoseNames[i] = TempNode.getAttribute("name");
		mapPoseLocations[i] = ProjectData::getAbsoluteResourcePath(TempNode.getAttribute("location"));
		mapPoseNumLoops[i] = QString(TempNode.getAttribute("numLoops")).toInt();
		mapPosePingPong[i] = (QString(TempNode.getAttribute("pingPong")) == "true") ? true:false;
		TempNode = BattleSpriteNode.getChildNode("pose", i);
		battlePoseNames[i] = TempNode.getAttribute("name");
		battlePoseLocations[i] = ProjectData::getAbsoluteResourcePath(TempNode.getAttribute("location"));
		battlePoseNumFrames[i] = QString(TempNode.getAttribute("numFrames")).toInt();
		battlePoseFPS[i] = QString(TempNode.getAttribute("fps")).toInt();
		battlePosePingPong[i] = (QString(TempNode.getAttribute("pingPong")) == "true") ? true:false;
	}
	mapPoseNames[MAPPOSE_IDLE] = "Idle";
	mapPoseNames[MAPPOSE_WALKING] = "Walking";
	mapPoseNames[MAPPOSE_JUMPING] = "Jumping";
	mapPoseNames[MAPPOSE_GRAPPLING] = "Grappling";
	battlePoseNames[BATTLEPOSE_IDLESTANCE] = "Idle Stance";
	battlePoseNames[BATTLEPOSE_ATTACK] = "Attack";
	battlePoseNames[BATTLEPOSE_SKILLUSED] = "Skill Used";
	battlePoseNames[BATTLEPOSE_DIEING] = "Dieing";
	battlePoseNames[BATTLEPOSE_DEAD] = "Dead";
	battlePoseNames[BATTLEPOSE_DAMAGED] = "Damaged";
	battlePoseNames[BATTLEPOSE_BADSTATUS] = "Bad Status";
	battlePoseNames[BATTLEPOSE_DEFENDING] = "Defending";
	battlePoseNames[BATTLEPOSE_RUNNINGLEFT] = "Running Left";
	battlePoseNames[BATTLEPOSE_RUNNINGRIGHT] = "Running Right";
	battlePoseNames[BATTLEPOSE_ITEMUSED] = "Item Used";
	battlePoseNames[BATTLEPOSE_WOUNDED] = "Wounded";
	battlePoseNames[BATTLEPOSE_VICTORY] = "Victory";
}

CharacterSprite::CharacterSprite(StorageFile &storageFile)
{
	char *temp_string;
	mapPoseNames.reserve(32);
	mapPoseLocations.reserve(32);
	battlePoseNames.reserve(32);
	battlePoseLocations.reserve(32);
	for (int i = 0; i < 32; ++i)
	{
		mapPoseNames.append("");
		mapPoseLocations.append("");
		battlePoseNames.append("");
		battlePoseLocations.append("");
	}
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	for (int i = 0; i < 32; ++i)
	{
		temp_string = storageFile.getString();
		mapPoseNames[i] = temp_string;
		delete[] temp_string;
		temp_string = storageFile.getString();
		mapPoseLocations[i] = temp_string;
		delete[] temp_string;
		mapPoseNumLoops[i] = storageFile.getSignedInt();
		mapPosePingPong[i] = storageFile.getBool();
		temp_string = storageFile.getString();
		battlePoseNames[i] = temp_string;
		delete[] temp_string;
		temp_string = storageFile.getString();
		battlePoseLocations[i] = temp_string;
		delete[] temp_string;
		battlePoseNumFrames[i] = storageFile.getSignedInt();
		battlePoseFPS[i] = storageFile.getSignedInt();
		battlePosePingPong[i] = storageFile.getBool();
	}
}

CharacterSprite::~CharacterSprite()
{
}

void CharacterSprite::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	for (int i = 0; i < 32; ++i)
	{
		storageFile.putString(mapPoseNames[i].toUtf8().data());
		storageFile.putString(mapPoseLocations[i].toUtf8().data());
		storageFile.putSignedInt(mapPoseNumLoops[i]);
		storageFile.putBool(mapPosePingPong[i]);
		storageFile.putString(battlePoseNames[i].toUtf8().data());
		storageFile.putString(battlePoseLocations[i].toUtf8().data());
		storageFile.putSignedInt(battlePoseNumFrames[i]);
		storageFile.putSignedInt(battlePoseFPS[i]);
		storageFile.putBool(battlePosePingPong[i]);
	}
}

QStringList CharacterSprite::getResources()
{
	QStringList resources;
	for (int i = 0; i < 32; ++i)
	{
		resources.append(mapPoseLocations[i]);
		resources.append(battlePoseLocations[i]);
	}
	return resources;
}

XMLNode CharacterSprite::getCharacterSpriteNode()
{
	XMLNode TempNode, MapSpriteNode, BattleSpriteNode, SpriteNode = XMLNode::createXMLTopNode("charactersprite");
	SpriteNode.addAttribute("name", name.toUtf8().data());
	MapSpriteNode = SpriteNode.addChild("mapSprite");
	BattleSpriteNode = SpriteNode.addChild("battleSprite");
	for (int i = 0; i < 32; ++i)
	{
		TempNode = MapSpriteNode.addChild("pose");
		TempNode.addAttribute("name", mapPoseNames[i].toUtf8().data());
		TempNode.addAttribute("location", ProjectData::getRelativeResourcePath(mapPoseLocations[i]).toUtf8().data());
		TempNode.addAttribute("numLoops", QString::number(mapPoseNumLoops[i]).toUtf8().data());
		TempNode.addAttribute("pingPong", (mapPosePingPong[i]) ? "true":"false");
		TempNode = BattleSpriteNode.addChild("pose");
		TempNode.addAttribute("name", battlePoseNames[i].toUtf8().data());
		TempNode.addAttribute("location", ProjectData::getRelativeResourcePath(battlePoseLocations[i]).toUtf8().data());
		TempNode.addAttribute("numFrames", QString::number(battlePoseNumFrames[i]).toUtf8().data());
		TempNode.addAttribute("fps", QString::number(battlePoseFPS[i]).toUtf8().data());
		TempNode.addAttribute("pingPong", (battlePosePingPong[i]) ? "true":"false");
	}
	return SpriteNode;
}

QString CharacterSprite::getName()
{
	return name;
}

void CharacterSprite::setName(QString value)
{
	name = value;
}

QString CharacterSprite::getMapPoseName(int poseID)
{
	return mapPoseNames[poseID];
}

void CharacterSprite::setMapPoseName(int poseID, QString name)
{
	mapPoseNames[poseID] = name;
}

QString CharacterSprite::getMapPoseLocation(int poseID)
{
	return mapPoseLocations[poseID];
}

void CharacterSprite::setMapPoseLocation(int poseID, QString location)
{
	mapPoseLocations[poseID] = location;
}

int CharacterSprite::getMapPoseNumLoops(int poseID)
{
	return mapPoseNumLoops[poseID];
}

void CharacterSprite::setMapPoseNumLoops(int poseID, int numLoops)
{
	mapPoseNumLoops[poseID] = numLoops;
}

bool CharacterSprite::isMapPosePingPong(int poseID)
{
	return mapPosePingPong[poseID];
}

void CharacterSprite::setMapPosePingPong(int poseID, bool pingPong)
{
	mapPosePingPong[poseID] = pingPong;
}

QString CharacterSprite::getBattlePoseName(int poseID)
{
	return battlePoseNames[poseID];
}

void CharacterSprite::setBattlePoseName(int poseID, QString name)
{
	battlePoseNames[poseID] = name;
}

QString CharacterSprite::getBattlePoseLocation(int poseID)
{
	return battlePoseLocations[poseID];
}

void CharacterSprite::setBattlePoseLocation(int poseID, QString location)
{
	battlePoseLocations[poseID] = location;
}

int CharacterSprite::getBattlePoseNumFrames(int poseID)
{
	return battlePoseNumFrames[poseID];
}

void CharacterSprite::setBattlePoseNumFrames(int poseID, int numFrames)
{
	battlePoseNumFrames[poseID] = numFrames;
}

int CharacterSprite::getBattlePoseFPS(int poseID)
{
	return battlePoseFPS[poseID];
}

void CharacterSprite::setBattlePoseFPS(int poseID, int fps)
{
	battlePoseFPS[poseID] = fps;
}

bool CharacterSprite::isBattlePosePingPong(int poseID)
{
	return battlePosePingPong[poseID];
}

void CharacterSprite::setBattlePosePingPong(int poseID, bool pingPong)
{
	battlePosePingPong[poseID] = pingPong;
}

bool CharacterSprite::tryLoad(XMLNode SpriteNode)
{
	XMLNode TempNode1, TempNode2;
	if (!XMLTools::attributeExists(SpriteNode, "name"))
		return false;
	if (!XMLTools::nodeExists(SpriteNode, "mapSprite"))
		return false;
	TempNode1 = SpriteNode.getChildNode("mapSprite");
	if (!XMLTools::nodesExist(TempNode1, "pose", 32, 32))
		return false;
	for (int i = 0; i < 32; ++i)
	{
		TempNode2 = TempNode1.getChildNode("pose", i);
		if (!XMLTools::attributeExists(TempNode2, "name"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "location"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "numLoops"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode2, "numLoops", 1, 4))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "pingPong"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode2, "pingPong", QStringList() << "true" << "false", 1))
			return false;
	}
	if (!XMLTools::nodeExists(SpriteNode, "battleSprite"))
		return false;
	TempNode1 = SpriteNode.getChildNode("battleSprite");
	if (!XMLTools::nodesExist(TempNode1, "pose", 32, 32))
		return false;
	for (int i = 0; i < 32; ++i)
	{
		TempNode2 = TempNode1.getChildNode("pose", i);
		if (!XMLTools::attributeExists(TempNode2, "name"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "location"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "numFrames"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode2, "numFrames", 1, 24))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "fps"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode2, "fps", 1, 24))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "pingPong"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode2, "pingPong", QStringList() << "true" << "false", 1))
			return false;
	}
	return true;
}
