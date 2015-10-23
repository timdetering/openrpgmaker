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

#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_battleevent.h"
#include "projectdata_monstergroup.h"
#include "xmlParser.h"
#include "xmlTools.h"

MonsterGroup::MonsterGroup()
{
	name = "Untitled";
	for (int i = 0; i < ProjectData::terrainNames.size(); ++i)
		habitat.append(true);
	battleEvent = new BattleEvent;
	alignment = ALIGN_MANUAL;
}

MonsterGroup::MonsterGroup(MonsterGroup &other)
{
	name = other.name;
	for (int i = 0; i < other.monsterLayout.size(); ++i)
	{
		MonsterPosition position;
		position.monsterID = other.monsterLayout[i].monsterID;
		position.x = other.monsterLayout[i].x;
		position.y = other.monsterLayout[i].y;
		monsterLayout.append(position);
	}
	for (int i = 0; i < other.habitat.size(); ++i)
		habitat.append(other.habitat[i]);
	battleEvent = new BattleEvent(*battleEvent);
	alignment = other.alignment;
}

MonsterGroup::MonsterGroup(const MonsterGroup &other)
{
	name = other.name;
	for (int i = 0; i < other.monsterLayout.size(); ++i)
	{
		MonsterPosition position;
		position.monsterID = other.monsterLayout[i].monsterID;
		position.x = other.monsterLayout[i].x;
		position.y = other.monsterLayout[i].y;
		monsterLayout.append(position);
	}
	for (int i = 0; i < other.habitat.size(); ++i)
		habitat.append(other.habitat[i]);
	battleEvent = new BattleEvent(*battleEvent);
	alignment = other.alignment;
}

MonsterGroup::MonsterGroup(XMLNode MonsterGroupNode)
{
	XMLNode TempNode1, TempNode2;
	QString temp_string;
	name = MonsterGroupNode.getAttribute("name");
	temp_string = MonsterGroupNode.getAttribute("alignment");
	if (temp_string == "manual")
		alignment = ALIGN_MANUAL;
	else
		alignment = ALIGN_AUTOMATIC;
	TempNode1 = MonsterGroupNode.getChildNode("monsterLayout");
	for (int i = 0, count = TempNode1.nChildNode("monster"); i < count; ++i)
	{
		MonsterPosition position;
		TempNode2 = TempNode1.getChildNode("monster", i);
		position.monsterID = QString(TempNode2.getAttribute("monsterID")).toInt();
		position.x = QString(TempNode2.getAttribute("x")).toInt();
		position.y = QString(TempNode2.getAttribute("y")).toInt();
		monsterLayout.append(position);
	}
	temp_string = MonsterGroupNode.getChildNode("habitat").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		if (temp_string[i] == QChar('0'))
			habitat.append(false);
		else
			habitat.append(true);
	}
	battleEvent = new BattleEvent(MonsterGroupNode.getChildNode("battleEvent"));
}

MonsterGroup::MonsterGroup(StorageFile &storageFile)
{
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete temp_string;
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		MonsterPosition position;
		position.monsterID = storageFile.getSignedInt();
		position.x = storageFile.getSignedInt();
		position.y = storageFile.getSignedInt();
		monsterLayout.append(position);
	}
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
		habitat.append(storageFile.getBool());
	battleEvent = new BattleEvent;
	TODO("Uncomment the following line when the battle animations are ready");
	//battleEvent = new BattleEvent(storageFile);
	alignment = storageFile.getSignedInt();
}

MonsterGroup::~MonsterGroup()
{
	if (battleEvent != NULL)
	{
		delete battleEvent;
		battleEvent = NULL;
	}
}

void MonsterGroup::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putSignedInt(monsterLayout.size());
	for (int i = 0; i < monsterLayout.size(); ++i)
	{
		storageFile.putSignedInt(monsterLayout[i].monsterID);
		storageFile.putSignedInt(monsterLayout[i].x);
		storageFile.putSignedInt(monsterLayout[i].y);
	}
	storageFile.putSignedInt(habitat.size());
	for (int i = 0; i < habitat.size(); ++i)
		storageFile.putBool(habitat[i]);
	TODO("Uncomment the following line when the battle animations are ready");
	//battleEvent.saveToStorageFile(storageFile);
	storageFile.putSignedInt(alignment);
}

QStringList MonsterGroup::getResources()
{
	QStringList resources;
	TODO("Add Battle Event's resources");
	return resources;
}

XMLNode MonsterGroup::getMonsterGroupNode()
{
	XMLNode TempNode1, TempNode2, MonsterGroupNode = XMLNode::createXMLTopNode("monstergroup");
	QString temp_string;
	MonsterGroupNode.addAttribute("name", name.toUtf8().data());
	MonsterGroupNode.addAttribute("alignment", (alignment == ALIGN_MANUAL) ? "manual":"automatic");
	TempNode1 = MonsterGroupNode.addChild("monsterLayout");
	for (int i = 0; i < monsterLayout.size(); ++i)
	{
		TempNode2 = TempNode1.addChild("monster");
		TempNode2.addAttribute("monsterID", QString::number(monsterLayout[i].monsterID).toUtf8().data());
		TempNode2.addAttribute("x", QString::number(monsterLayout[i].x).toUtf8().data());
		TempNode2.addAttribute("y", QString::number(monsterLayout[i].y).toUtf8().data());
	}
	TempNode1 = MonsterGroupNode.addChild("habitat");
	temp_string = "";
	for (int i = 0; i < habitat.size(); ++i)
		temp_string += (habitat[i]) ? QChar('0'):QChar('1');
	TempNode1.addText(temp_string.toUtf8().data());
	MonsterGroupNode.addChild("battleEvent");
	TODO("Uncomment the following lines when the battle animations are ready");
	//TempNode1 = battleEvent->getEventNode();
	//TempNode1.updateName("battleEvent");
	//MonsterGroupNode.addChild(TempNode1);
	return MonsterGroupNode;
}

QString MonsterGroup::getName()
{
	return name;
}

void MonsterGroup::setName(QString value)
{
	name = value;
}

void MonsterGroup::addMonster(MonsterGroup::MonsterPosition position)
{
	monsterLayout.append(position);
}

void MonsterGroup::deleteMonsterPosition(int positionID)
{
	monsterLayout.removeAt(positionID);
}

MonsterGroup::MonsterPosition *MonsterGroup::getMonsterPositionPointer(int positionID)
{
	return &(monsterLayout[positionID]);
}

bool MonsterGroup::getHabitat(int terrainID)
{
	return habitat[terrainID];
}

void MonsterGroup::setHabitat(int terrainID, bool value)
{
	habitat[terrainID] = value;
}

void MonsterGroup::resizeHabitat(int numTerrain)
{
	if (numTerrain >= habitat.size())
	{
		for (int i = habitat.size(); i < numTerrain; ++i)
			habitat.append(true);
	}
	else
	{
		for (int i = numTerrain; i < habitat.size(); ++i)
			habitat.removeLast();
	}
}

BattleEvent *MonsterGroup::getBattleEventPointer()
{
	return battleEvent;
}

int MonsterGroup::getAlignment()
{
	return alignment;
}

void MonsterGroup::setAlignment(int value)
{
	alignment = value;
}

bool MonsterGroup::tryLoad(XMLNode MonsterGroupNode)
{
	XMLNode TempNode1, TempNode2;
	QString temp_string;
	if (!XMLTools::attributeExists(MonsterGroupNode, "name"))
		return false;
	if (!XMLTools::attributeExists(MonsterGroupNode, "alignment"))
		return false;
	if (!XMLTools::attributeStringValid(MonsterGroupNode, "alignment", QStringList() << "manual" << "automatic", 1))
		return false;
	if (!XMLTools::nodeExists(MonsterGroupNode, "monsterLayout"))
		return false;
	TempNode1 = MonsterGroupNode.getChildNode("monsterLayout");
	for (int i = 0, count = TempNode1.nChildNode("monster"); i < count; ++i)
	{
		TempNode2 = TempNode1.getChildNode("monster", i);
		if (!XMLTools::attributeExists(TempNode2, "monsterID"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "x"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "y"))
			return false;
	}
	if (!XMLTools::nodeExists(MonsterGroupNode, "habitat"))
		return false;
	temp_string = MonsterGroupNode.getChildNode("habitat").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		if (temp_string[i] != QChar('0') && temp_string[i] != QChar('1'))
			QMessageBox::warning(0, "Warning: Invalid Data", QString("Habitat %1 either a 0 or a 1.\nDefaulting to 1.").arg(i));
	}
	if (!XMLTools::nodeExists(MonsterGroupNode, "battleEvent"))
		return false;
	//if (!BattleEvent::tryLoad(MonsterGroupNode.getChildNode("battleEvent")))
	//	return false;
	return true;
}
