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
#include "projectdata_monsteranimation.h"
#include "xmlParser.h"
#include "xmlTools.h"

MonsterAnimation::MonsterAnimation()
{
	name = "Untitled";
	poseNames[POSE_IDLESTANCE] = "Idle Stance";
	poseNames[POSE_ATTACK] = "Attack";
	poseNames[POSE_SKILLUSED] = "Skill Used";
	poseNames[POSE_DIEING] = "Dieing";
	poseNames[POSE_DEAD] = "Dead";
	poseNames[POSE_DAMAGED] = "Damaged";
	poseNames[POSE_BADSTATUS] = "Bad Status";
	poseNames[POSE_DEFENDING] = "Defending";
	poseNames[POSE_RUNNINGLEFT] = "Running Left";
	poseNames[POSE_RUNNINGRIGHT] = "Running Right";
	poseNames[POSE_ITEMUSED] = "Item Used";
	poseNames[POSE_WOUNDED] = "Wounded";
	poseNames[POSE_VICTORY] = "Victory";
	poseLocations[POSE_IDLESTANCE] = "";
	poseLocations[POSE_ATTACK] = "";
	poseLocations[POSE_SKILLUSED] = "";
	poseLocations[POSE_DIEING] = "";
	poseLocations[POSE_DEAD] = "";
	poseLocations[POSE_DAMAGED] = "";
	poseLocations[POSE_BADSTATUS] = "";
	poseLocations[POSE_DEFENDING] = "";
	poseLocations[POSE_RUNNINGLEFT] = "";
	poseLocations[POSE_RUNNINGRIGHT] = "";
	poseLocations[POSE_ITEMUSED] = "";
	poseLocations[POSE_WOUNDED] = "";
	poseLocations[POSE_VICTORY] = "";
	poseNumFrames[POSE_IDLESTANCE] = 1;
	poseNumFrames[POSE_ATTACK] = 1;
	poseNumFrames[POSE_SKILLUSED] = 1;
	poseNumFrames[POSE_DIEING] = 1;
	poseNumFrames[POSE_DEAD] = 1;
	poseNumFrames[POSE_DAMAGED] = 1;
	poseNumFrames[POSE_BADSTATUS] = 1;
	poseNumFrames[POSE_DEFENDING] = 1;
	poseNumFrames[POSE_RUNNINGLEFT] = 1;
	poseNumFrames[POSE_RUNNINGRIGHT] = 1;
	poseNumFrames[POSE_ITEMUSED] = 1;
	poseNumFrames[POSE_WOUNDED] = 1;
	poseNumFrames[POSE_VICTORY] = 1;
	poseFPS[POSE_IDLESTANCE] = 12;
	poseFPS[POSE_ATTACK] = 12;
	poseFPS[POSE_SKILLUSED] = 12;
	poseFPS[POSE_DIEING] = 12;
	poseFPS[POSE_DEAD] = 12;
	poseFPS[POSE_DAMAGED] = 12;
	poseFPS[POSE_BADSTATUS] = 12;
	poseFPS[POSE_DEFENDING] = 12;
	poseFPS[POSE_RUNNINGLEFT] = 12;
	poseFPS[POSE_RUNNINGRIGHT] = 12;
	poseFPS[POSE_ITEMUSED] = 12;
	poseFPS[POSE_WOUNDED] = 12;
	poseFPS[POSE_VICTORY] = 12;
	posePingPong[POSE_IDLESTANCE] = false;
	posePingPong[POSE_ATTACK] = false;
	posePingPong[POSE_SKILLUSED] = false;
	posePingPong[POSE_DIEING] = false;
	posePingPong[POSE_DEAD] = false;
	posePingPong[POSE_DAMAGED] = false;
	posePingPong[POSE_BADSTATUS] = false;
	posePingPong[POSE_DEFENDING] = false;
	posePingPong[POSE_RUNNINGLEFT] = false;
	posePingPong[POSE_RUNNINGRIGHT] = false;
	posePingPong[POSE_ITEMUSED] = false;
	posePingPong[POSE_WOUNDED] = false;
	posePingPong[POSE_VICTORY] = false;
	for (int i = 11; i < 32; ++i)
	{
		poseNames[i] = "";
		poseLocations[i] = "";
		poseNumFrames[i] = 1;
		poseFPS[i] = 12;
		posePingPong[i] = false;
	}
}

MonsterAnimation::MonsterAnimation(MonsterAnimation &other)
{
	name = other.name;
	for (int i = 0; i < 32; ++i)
	{
		poseNames[i] = other.poseNames[i];
		poseLocations[i] = other.poseLocations[i];
		poseNumFrames[i] = other.poseNumFrames[i];
		poseFPS[i] = other.poseFPS[i];
		posePingPong[i] = other.posePingPong[i];
	}
}

MonsterAnimation::MonsterAnimation(const MonsterAnimation &other)
{
	name = other.name;
	for (int i = 0; i < 32; ++i)
	{
		poseNames[i] = other.poseNames[i];
		poseLocations[i] = other.poseLocations[i];
		poseNumFrames[i] = other.poseNumFrames[i];
		poseFPS[i] = other.poseFPS[i];
		posePingPong[i] = other.posePingPong[i];
	}
}

MonsterAnimation::MonsterAnimation(XMLNode MonsterAnimationNode)
{
	XMLNode TempNode;
	name = MonsterAnimationNode.getAttribute("name");
	for (int i = 0; i < 32; ++i)
	{
		TempNode = MonsterAnimationNode.getChildNode("pose", i);
		poseNames[i] = TempNode.getAttribute("name");
		poseLocations[i] = ProjectData::getAbsoluteResourcePath(TempNode.getAttribute("location"));
		poseNumFrames[i] = QString(TempNode.getAttribute("numFrames")).toInt();
		poseFPS[i] = QString(TempNode.getAttribute("fps")).toInt();
		posePingPong[i] = (QString(TempNode.getAttribute("pingPong")) == "true") ? true:false;
	}
	poseNames[POSE_IDLESTANCE] = "Idle Stance";
	poseNames[POSE_ATTACK] = "Attack";
	poseNames[POSE_SKILLUSED] = "Skill Used";
	poseNames[POSE_DIEING] = "Dieing";
	poseNames[POSE_DEAD] = "Dead";
	poseNames[POSE_DAMAGED] = "Damaged";
	poseNames[POSE_BADSTATUS] = "Bad Status";
	poseNames[POSE_DEFENDING] = "Defending";
	poseNames[POSE_RUNNINGLEFT] = "Running Left";
	poseNames[POSE_RUNNINGRIGHT] = "Running Right";
	poseNames[POSE_ITEMUSED] = "Item Used";
	poseNames[POSE_WOUNDED] = "Wounded";
	poseNames[POSE_VICTORY] = "Victory";
}

MonsterAnimation::MonsterAnimation(StorageFile &storageFile)
{
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	for (int i = 0; i < 32; ++i)
	{
		temp_string = storageFile.getString();
		poseNames[i] = temp_string;
		delete[] temp_string;
		temp_string = storageFile.getString();
		poseLocations[i] = temp_string;
		delete[] temp_string;
		poseNumFrames[i] = storageFile.getSignedInt();
		poseFPS[i] = storageFile.getSignedInt();
		posePingPong[i] = storageFile.getBool();
	}
}

MonsterAnimation::~MonsterAnimation()
{
}

void MonsterAnimation::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	for (int i = 0; i < 32; ++i)
	{
		storageFile.putString(poseNames[i].toUtf8().data());
		storageFile.putString(poseLocations[i].toUtf8().data());
		storageFile.putSignedInt(poseNumFrames[i]);
		storageFile.putSignedInt(poseFPS[i]);
		storageFile.putBool(posePingPong[i]);
	}
}

QStringList MonsterAnimation::getResources()
{
	QStringList resources;
	for (int i = 0; i < 32; ++i)
		resources.append(poseLocations[i]);
	return resources;
}

XMLNode MonsterAnimation::getMonsterAnimationNode()
{
	XMLNode TempNode, MonsterNode = XMLNode::createXMLTopNode("monsteranimation");
	MonsterNode.addAttribute("name", name.toUtf8().data());
	for (int i = 0; i < 32; ++i)
	{
		TempNode = MonsterNode.addChild("pose");
		TempNode.addAttribute("name", poseNames[i].toUtf8().data());
		TempNode.addAttribute("location", ProjectData::getRelativeResourcePath(poseLocations[i]).toUtf8().data());
		TempNode.addAttribute("numFrames", QString::number(poseNumFrames[i]).toUtf8().data());
		TempNode.addAttribute("fps", QString::number(poseFPS[i]).toUtf8().data());
		TempNode.addAttribute("pingPong", (posePingPong[i]) ? "true":"false");
	}
	return MonsterNode;
}

QString MonsterAnimation::getName()
{
	return name;
}

void MonsterAnimation::setName(QString value)
{
	name = value;
}

QString MonsterAnimation::getPoseName(int poseID)
{
	return poseNames[poseID];
}

void MonsterAnimation::setPoseName(int poseID, QString value)
{
	poseNames[poseID] = value;
}

QString MonsterAnimation::getPoseLocation(int poseID)
{
	return poseLocations[poseID];
}

void MonsterAnimation::setPoseLocation(int poseID, QString value)
{
	poseLocations[poseID] = value;
}

int MonsterAnimation::getPoseNumFrames(int poseID)
{
	return poseNumFrames[poseID];
}

void MonsterAnimation::setPoseNumFrames(int poseID, int value)
{
	poseNumFrames[poseID] = value;
}

int MonsterAnimation::getPoseFPS(int poseID)
{
	return poseFPS[poseID];
}

void MonsterAnimation::setPoseFPS(int poseID, int value)
{
	poseFPS[poseID] = value;
}

bool MonsterAnimation::isPosePingPong(int poseID)
{
	return posePingPong[poseID];
}

void MonsterAnimation::setPosePingPong(int poseID, bool value)
{
	posePingPong[poseID] = value;
}

bool MonsterAnimation::tryLoad(XMLNode MonsterAnimationNode)
{
	XMLNode TempNode;
	if (!XMLTools::attributeExists(MonsterAnimationNode, "name"))
		return false;
	if (!XMLTools::nodesExist(MonsterAnimationNode, "pose", 32, 32))
		return false;
	for (int i = 0; i < 32; ++i)
	{
		TempNode = MonsterAnimationNode.getChildNode("pose", i);
		if (!XMLTools::attributeExists(TempNode, "name"))
			return false;
		if (!XMLTools::attributeExists(TempNode, "location"))
			return false;
		if (!XMLTools::attributeExists(TempNode, "numFrames"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "numFrames", 1, 24))
			return false;
		if (!XMLTools::attributeExists(TempNode, "fps"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "fps", 1, 24))
			return false;
		if (!XMLTools::attributeExists(TempNode, "pingPong"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "pingPong", QStringList() << "true" << "false", 1))
			return false;
	}
	return true;
}
