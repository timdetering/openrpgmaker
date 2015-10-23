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

#include <QImage>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mapeventpageeditor.h"
#include "projectdata.h"
#include "projectdata_baseevent.h"
#include "projectdata_mapevent.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "xmlParser.h"
#include "xmlTools.h"

//BEGIN MapEvent Member Functions
MapEvent::MapEvent()
{
	name = "Untitled";
	xLocation = 0;
	yLocation = 0;
	pages.append(new Page());
}

MapEvent::MapEvent(MapEvent *other)
{
	name = other->name;
	xLocation = other->xLocation;
	yLocation = other->yLocation;
	for (int i = 0; i < other->pages.size(); ++i)
		pages.append(new Page(other->pages[i]));
}

MapEvent::MapEvent(XMLNode MapEventNode)
{
	QString tempString;
	name = MapEventNode.getAttribute("name");
	xLocation = QString(MapEventNode.getAttribute("x")).toInt();
	yLocation = QString(MapEventNode.getAttribute("y")).toInt();
	for (int i = 0, count = MapEventNode.nChildNode("page"); i < count; ++i)
		pages.append(new Page(MapEventNode.getChildNode("page", i)));
}

MapEvent::MapEvent(StorageFile &storageFile)
{
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	xLocation = storageFile.getSignedInt();
	yLocation = storageFile.getSignedInt();
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
		pages.append(new Page(storageFile));
}

MapEvent::~MapEvent()
{
	for (int i = 0; i < pages.size(); ++i)
		delete (pages[i]);
	pages.clear();
}

void MapEvent::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putSignedInt(xLocation);
	storageFile.putSignedInt(yLocation);
	storageFile.putSignedInt(pages.size());
	for (int i = 0; i < pages.size(); ++i)
		pages[i]->saveToStorageFile(storageFile);
}

XMLNode MapEvent::getMapEventNode()
{
	XMLNode EventNode = XMLNode::createXMLTopNode("mapEvent");
	QString tempString;
	EventNode.addAttribute("name", name.toUtf8().data());
	EventNode.addAttribute("x", QString::number(xLocation).toUtf8().data());
	EventNode.addAttribute("y", QString::number(yLocation).toUtf8().data());
	for (int i = 0; i < pages.size(); ++i)
		EventNode.addChild(pages[i]->getPageNode());
	return EventNode;
}

int MapEvent::numPages()
{
	return pages.size();
}

MapEvent::Page *MapEvent::getPage(int pageID)
{
	return pages[pageID];
}

void MapEvent::addPage(MapEvent::Page *page)
{
	pages.append(page);
}

void MapEvent::deletePage(int pageID)
{
	delete (pages[pageID]);
	pages.removeAt(pageID);
}

QString MapEvent::getName()
{
	return name;
}

void MapEvent::setName(QString value)
{
	name = value;
}

int MapEvent::getXLocation()
{
	return xLocation;
}

int MapEvent::getYLocation()
{
	return yLocation;
}

void MapEvent::setLocation(int x, int y)
{
	xLocation = x;
	yLocation = y;
}

void MapEvent::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	for (int i = 0; i < pages.size(); ++i)
		pages[i]->applySwappedTiles(swappedTiles);
}

bool MapEvent::tryLoad(XMLNode EventNode)
{
	XMLNode TempNode1, TempNode2, TempNode3;
	QStringList trueFalseList = QStringList() << "true" << "false";
	QStringList tempStringList;
	QString tempString;
	if (!XMLTools::attributeExists(EventNode, "name"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "x"))
		return false;
	if (!XMLTools::attributeExists(EventNode, "y"))
		return false;
	if (!XMLTools::nodesExist(EventNode, "page", 1, 99))
		return false;
	for (int i = 0, count = EventNode.nChildNode("page"); i < count; ++i)
	{
		if (!Page::tryLoad(EventNode.getChildNode("page", i)))
			return false;
	}
	return true;
}
//END MapEvent Member Functions

//BEGIN MapEvent::MovementPattern Member Functions
MapEvent::MovementPattern::MovementPattern(int type)
{
	type = type;
	soundEffect.location = "";
	soundEffect.volume = 1.0;
	soundEffect.panning = 0.0;
	soundEffect.speed = 1.0;
	soundEffect.numLoops = 0;
	soundEffect.sfxID = -1;
	soundEffect.retainPitch = true;
	stringValue = "";
	intValue = 0;
}

MapEvent::MovementPattern::MovementPattern(XMLNode PatternNode)
{
	QString tempString = PatternNode.getAttribute("type");
	soundEffect.location = "";
	soundEffect.volume = 1.0;
	soundEffect.panning = 0.0;
	soundEffect.speed = 1.0;
	soundEffect.numLoops = 0;
	soundEffect.sfxID = -1;
	soundEffect.retainPitch = true;
	stringValue = "";
	intValue = 0;
	if (tempString == "moveUpLeft")
		type = PATTERN_MOVEUPLEFT;
	else if (tempString == "moveUp")
		type = PATTERN_MOVEUP;
	else if (tempString == "moveUpRight")
		type = PATTERN_MOVEUPRIGHT;
	else if (tempString == "moveRight")
		type = PATTERN_MOVERIGHT;
	else if (tempString == "moveDownRight")
		type = PATTERN_MOVEDOWNRIGHT;
	else if (tempString == "moveDown")
		type = PATTERN_MOVEDOWN;
	else if (tempString == "moveDownLeft")
		type = PATTERN_MOVEDOWNLEFT;
	else if (tempString == "moveLeft")
		type = PATTERN_MOVELEFT;
	else if (tempString == "moveRandomly")
		type = PATTERN_MOVERANDOMLY;
	else if (tempString == "increaseMovementSpeed")
		type = PATTERN_INCREASEMOVEMENTSPEED;
	else if (tempString == "decreaseMovementSpeed")
		type = PATTERN_DECREASEMOVEMENTSPEED;
	else if (tempString == "moveForward")
		type = PATTERN_MOVEFORWARD;
	else if (tempString == "moveBackward")
		type = PATTERN_MOVEBACKWARD;
	else if (tempString == "moveTowardHero")
		type = PATTERN_MOVETOWARDHERO;
	else if (tempString == "moveAwayFromHero")
		type = PATTERN_MOVEAWAYFROMHERO;
	else if (tempString == "faceUp")
		type = PATTERN_FACEUP;
	else if (tempString == "faceDown")
		type = PATTERN_FACEDOWN;
	else if (tempString == "faceLeft")
		type = PATTERN_FACELEFT;
	else if (tempString == "faceRight")
		type = PATTERN_FACERIGHT;
	else if (tempString == "turnLeft")
		type = PATTERN_TURNLEFT;
	else if (tempString == "turnRight")
		type = PATTERN_TURNRIGHT;
	else if (tempString == "uTurn")
		type = PATTERN_UTURN;
	else if (tempString == "faceRandomDirection")
		type = PATTERN_FACERANDOMDIRECTION;
	else if (tempString == "faceLeftRightRandomly")
		type = PATTERN_FACELEFTRIGHTRANDOMLY;
	else if (tempString == "faceHero")
		type = PATTERN_FACEHERO;
	else if (tempString == "faceAwayFromHero")
		type = PATTERN_FACEAWAYFROMHERO;
	else if (tempString == "increaseMovementFrequency")
		type = PATTERN_INCREASEMOVEMENTFREQUENCY;
	else if (tempString == "decreaseMovementFrequency")
		type = PATTERN_DECREASEMOVEMENTFREQUENCY;
	else if (tempString == "beginJump")
		type = PATTERN_BEGINJUMP;
	else if (tempString == "endJump")
		type = PATTERN_ENDJUMP;
	else if (tempString == "lockFacing")
		type = PATTERN_LOCKFACING;
	else if (tempString == "unlockFacing")
		type = PATTERN_UNLOCKFACING;
	else if (tempString == "wait")
	{
		type = PATTERN_WAIT;
		intValue = QString(PatternNode.getAttribute("time")).toInt();
	}
	else if (tempString == "phasingModeOff")
		type = PATTERN_PHASINGMODEOFF;
	else if (tempString == "phasingModeOn")
		type = PATTERN_PHASINGMODEON;
	else if (tempString == "stopAnimation")
		type = PATTERN_STOPANIMATION;
	else if (tempString == "resumeAnimation")
		type = PATTERN_RESUMEANIMATION;
	else if (tempString == "changeGraphic")
	{
		type = PATTERN_CHANGEGRAPHIC;
		stringValue = ProjectData::getAbsoluteResourcePath(PatternNode.getAttribute("location"));
	}
	else if (tempString == "increaseTransparency")
	{
		type = PATTERN_INCREASETRANSPARENCY;
		intValue = QString(PatternNode.getAttribute("amount")).toInt();
	}
	else if (tempString == "decreaseTransparency")
	{
		type = PATTERN_DECREASETRANSPARENCY;
		intValue = QString(PatternNode.getAttribute("amount")).toInt();
	}
	else if (tempString == "switchOn")
	{
		type = PATTERN_SWITCHON;
		intValue = QString(PatternNode.getAttribute("switchID")).toInt();
	}
	else if (tempString == "switchOff")
	{
		type = PATTERN_SWITCHOFF;
		intValue = QString(PatternNode.getAttribute("switchID")).toInt();
	}
	else if (tempString == "playSoundEffect")
	{
		type = PATTERN_PLAYSOUNDEFFECT;
		soundEffect.location = ProjectData::getAbsoluteResourcePath(PatternNode.getAttribute("location"));
		soundEffect.volume = QString(PatternNode.getAttribute("volume")).toFloat();
		soundEffect.panning = QString(PatternNode.getAttribute("panning")).toFloat();
		soundEffect.speed = QString(PatternNode.getAttribute("speed")).toFloat();
		soundEffect.numLoops = QString(PatternNode.getAttribute("numLoops")).toInt();
		tempString = PatternNode.getAttribute("retainPitch");
		soundEffect.retainPitch = (tempString == "true") ? true:false;
	}
}

MapEvent::MovementPattern::MovementPattern(StorageFile &storageFile)
{
	char *temp_string;
	type = storageFile.getSignedInt();
	soundEffect.location = "";
	soundEffect.volume = 1.0;
	soundEffect.panning = 0.0;
	soundEffect.speed = 1.0;
	soundEffect.numLoops = 0;
	soundEffect.sfxID = -1;
	soundEffect.retainPitch = true;
	stringValue = "";
	intValue = 0;
	if (type == PATTERN_WAIT || type == PATTERN_INCREASETRANSPARENCY || type == PATTERN_DECREASETRANSPARENCY || type == PATTERN_SWITCHON || type == PATTERN_SWITCHOFF)
		intValue = storageFile.getSignedInt();
	else if (type == PATTERN_CHANGEGRAPHIC)
	{
		temp_string = storageFile.getString();
		stringValue = temp_string;
		delete[] temp_string;
	}
	else if (type == PATTERN_PLAYSOUNDEFFECT)
	{
		temp_string = storageFile.getString();
		soundEffect.location = temp_string;
		delete[] temp_string;
		soundEffect.volume = storageFile.getFloat();
		soundEffect.panning = storageFile.getFloat();
		soundEffect.speed = storageFile.getFloat();
		soundEffect.numLoops = storageFile.getSignedInt();
		soundEffect.retainPitch = storageFile.getBool();
	}
}

MapEvent::MovementPattern::MovementPattern(MovementPattern *other)
{
	soundEffect.location = other->soundEffect.location;
	soundEffect.volume = other->soundEffect.volume;
	soundEffect.panning = other->soundEffect.panning;
	soundEffect.speed = other->soundEffect.speed;
	soundEffect.numLoops = other->soundEffect.numLoops;
	soundEffect.sfxID = other->soundEffect.sfxID;
	soundEffect.retainPitch = other->soundEffect.retainPitch;
	stringValue = other->stringValue;
	type = other->type;
	intValue = other->intValue;
}

XMLNode MapEvent::MovementPattern::getPatternNode()
{
	XMLNode PatternNode = XMLNode::createXMLTopNode("pattern");
	if (type == PATTERN_MOVEUPLEFT)
		PatternNode.addAttribute("type", "moveUpLeft");
	else if (type == PATTERN_MOVEUP)
		PatternNode.addAttribute("type", "moveUp");
	else if (type == PATTERN_MOVEUPRIGHT)
		PatternNode.addAttribute("type", "moveUpRight");
	else if (type == PATTERN_MOVERIGHT)
		PatternNode.addAttribute("type", "moveRight");
	else if (type == PATTERN_MOVEDOWNRIGHT)
		PatternNode.addAttribute("type", "moveDownRight");
	else if (type == PATTERN_MOVEDOWN)
		PatternNode.addAttribute("type", "moveDown");
	else if (type == PATTERN_MOVEDOWNLEFT)
		PatternNode.addAttribute("type", "moveDownLeft");
	else if (type == PATTERN_MOVELEFT)
		PatternNode.addAttribute("type", "moveLeft");
	else if (type == PATTERN_MOVERANDOMLY)
		PatternNode.addAttribute("type", "moveRandomly");
	else if (type == PATTERN_INCREASEMOVEMENTSPEED)
		PatternNode.addAttribute("type", "increaseMovementSpeed");
	else if (type == PATTERN_DECREASEMOVEMENTSPEED)
		PatternNode.addAttribute("type", "decreaseMovementSpeed");
	else if (type == PATTERN_MOVEFORWARD)
		PatternNode.addAttribute("type", "moveForward");
	else if (type == PATTERN_MOVEBACKWARD)
		PatternNode.addAttribute("type", "moveBackward");
	else if (type == PATTERN_MOVETOWARDHERO)
		PatternNode.addAttribute("type", "moveTowardHero");
	else if (type == PATTERN_MOVEAWAYFROMHERO)
		PatternNode.addAttribute("type", "moveAwayFromHero");
	else if (type == PATTERN_FACEUP)
		PatternNode.addAttribute("type", "faceUp");
	else if (type == PATTERN_FACEDOWN)
		PatternNode.addAttribute("type", "faceDown");
	else if (type == PATTERN_FACELEFT)
		PatternNode.addAttribute("type", "faceLeft");
	else if (type == PATTERN_FACERIGHT)
		PatternNode.addAttribute("type", "faceRight");
	else if (type == PATTERN_TURNLEFT)
		PatternNode.addAttribute("type", "turnLeft");
	else if (type == PATTERN_TURNRIGHT)
		PatternNode.addAttribute("type", "turnRight");
	else if (type == PATTERN_UTURN)
		PatternNode.addAttribute("type", "uTurn");
	else if (type == PATTERN_FACERANDOMDIRECTION)
		PatternNode.addAttribute("type", "faceRandomDirection");
	else if (type == PATTERN_FACELEFTRIGHTRANDOMLY)
		PatternNode.addAttribute("type", "faceLeftRightRandomly");
	else if (type == PATTERN_FACEHERO)
		PatternNode.addAttribute("type", "faceHero");
	else if (type == PATTERN_FACEAWAYFROMHERO)
		PatternNode.addAttribute("type", "faceAwayFromHero");
	else if (type == PATTERN_INCREASEMOVEMENTFREQUENCY)
		PatternNode.addAttribute("type", "increaseMovementFrequency");
	else if (type == PATTERN_DECREASEMOVEMENTFREQUENCY)
		PatternNode.addAttribute("type", "decreaseMovementFrequency");
	else if (type == PATTERN_BEGINJUMP)
		PatternNode.addAttribute("type", "beginJump");
	else if (type == PATTERN_ENDJUMP)
		PatternNode.addAttribute("type", "endJump");
	else if (type == PATTERN_LOCKFACING)
		PatternNode.addAttribute("type", "lockFacing");
	else if (type == PATTERN_UNLOCKFACING)
		PatternNode.addAttribute("type", "unlockFacing");
	else if (type == PATTERN_WAIT)
	{
		PatternNode.addAttribute("type", "wait");
		PatternNode.addAttribute("time", QString::number(intValue).toUtf8().data());
	}
	else if (type == PATTERN_PHASINGMODEOFF)
		PatternNode.addAttribute("type", "phasingModeOff");
	else if (type == PATTERN_PHASINGMODEON)
		PatternNode.addAttribute("type", "phasingModeOn");
	else if (type == PATTERN_STOPANIMATION)
		PatternNode.addAttribute("type", "stopAnimation");
	else if (type == PATTERN_RESUMEANIMATION)
		PatternNode.addAttribute("type", "resumeAnimation");
	else if (type == PATTERN_CHANGEGRAPHIC)
	{
		PatternNode.addAttribute("type", "changeGraphic");
		PatternNode.addAttribute("location", ProjectData::getRelativeResourcePath(stringValue).toUtf8().data());
	}
	else if (type == PATTERN_INCREASETRANSPARENCY)
	{
		PatternNode.addAttribute("type", "increaseTransparency");
		PatternNode.addAttribute("amount", QString::number(intValue).toUtf8().data());
	}
	else if (type == PATTERN_DECREASETRANSPARENCY)
	{
		PatternNode.addAttribute("type", "decreaseTransparency");
		PatternNode.addAttribute("amount", QString::number(intValue).toUtf8().data());
	}
	else if (type == PATTERN_SWITCHON)
	{
		PatternNode.addAttribute("type", "switchOn");
		PatternNode.addAttribute("switchID", QString::number(intValue).toUtf8().data());
	}
	else if (type == PATTERN_SWITCHOFF)
	{
		PatternNode.addAttribute("type", "switchOff");
		PatternNode.addAttribute("switchID", QString::number(intValue).toUtf8().data());
	}
	else if (type == PATTERN_PLAYSOUNDEFFECT)
	{
		PatternNode.addAttribute("type", "playSoundEffect");
		PatternNode.addAttribute("location", ProjectData::getRelativeResourcePath(soundEffect.location).toUtf8().data());
		PatternNode.addAttribute("volume", QString::number(soundEffect.volume).toUtf8().data());
		PatternNode.addAttribute("panning", QString::number(soundEffect.panning).toUtf8().data());
		PatternNode.addAttribute("speed", QString::number(soundEffect.speed).toUtf8().data());
		PatternNode.addAttribute("numLoops", QString::number(soundEffect.numLoops).toUtf8().data());
		PatternNode.addAttribute("retainPitch", (soundEffect.retainPitch) ? "true":"false");
	}
	return PatternNode;
}

void MapEvent::MovementPattern::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(type);
	if (type == PATTERN_WAIT || type == PATTERN_INCREASETRANSPARENCY || type == PATTERN_DECREASETRANSPARENCY || type == PATTERN_SWITCHON || type == PATTERN_SWITCHOFF)
		storageFile.putSignedInt(intValue);
	else if (type == PATTERN_CHANGEGRAPHIC)
		storageFile.putString(stringValue.toUtf8().data());
	else if (type == PATTERN_PLAYSOUNDEFFECT)
	{
		storageFile.putString(soundEffect.location.toUtf8().data());
		storageFile.putFloat(soundEffect.volume);
		storageFile.putFloat(soundEffect.panning);
		storageFile.putFloat(soundEffect.speed);
		storageFile.putSignedInt(soundEffect.numLoops);
		storageFile.putBool(soundEffect.retainPitch);
	}
}

bool MapEvent::MovementPattern::tryLoad(XMLNode PatternNode)
{
	QStringList tempStringList;
	QStringList trueFalseList = QStringList() << "true" << "false";
	int patternType;
	tempStringList.clear();
	tempStringList << "moveUpLeft" << "moveUp" << "moveUpRight" << "moveRight" << "moveDownRight" << "moveDown""moveDownLeft" << "moveLeft" << "moveRandomly""increaseMovementSpeed" << "decreaseMovementSpeed" << "moveForward" << "moveBackward" << "moveTowardHero" << "moveAwayFromHero" << "faceUp" << "faceDown" << "faceLeft" << "faceRight" << "turnLeft" << "turnRight" << "uTurn" << "faceRandomDirection" << "faceLeftRightRandomly" << "faceHero" << "faceAwayFromHero" << "increaseMovementFrequency" << "decreaseMovementFrequency" << "beginJump" << "endJump" << "lockFacing" << "unlockFacing" << "wait" << "phasingModeOff" << "phasingModeOn" << "stopAnimation" << "resumeAnimation" << "changeGraphic" << "increaseTransparency" << "decreaseTransparency" << "switchOn" << "switchOff" << "playSoundEffect";
	if (!XMLTools::attributeExists(PatternNode, "type"))
		return false;
	if (!XMLTools::attributeStringValid(PatternNode, "type", tempStringList))
		return false;
	patternType = tempStringList.indexOf(PatternNode.getAttribute("type"));
	if (patternType == PATTERN_INCREASETRANSPARENCY || patternType == PATTERN_DECREASETRANSPARENCY)
	{
		if (!XMLTools::attributeExists(PatternNode, "amount"))
			return false;
	}
	else if (patternType == PATTERN_WAIT)
	{
		if (!XMLTools::attributeExists(PatternNode, "time"))
			return false;
	}
	else if (patternType == PATTERN_CHANGEGRAPHIC)
	{
		if (!XMLTools::attributeExists(PatternNode, "location"))
			return false;
	}
	else if (patternType == PATTERN_SWITCHON || patternType == PATTERN_SWITCHOFF)
	{
		if (!XMLTools::attributeExists(PatternNode, "switchID"))
			return false;
	}
	else if (patternType == PATTERN_PLAYSOUNDEFFECT)
	{
		if (!XMLTools::attributeExists(PatternNode, "location"))
			return false;
		if (!XMLTools::attributeExists(PatternNode, "volume"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(PatternNode, "volume", 0.0, 1.0))
			return false;
		if (!XMLTools::attributeExists(PatternNode, "panning"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(PatternNode, "panning", -1.0, 1.0))
			return false;
		if (!XMLTools::attributeExists(PatternNode, "speed"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(PatternNode, "speed", 0.5, 2.0))
			return false;
		if (!XMLTools::attributeExists(PatternNode, "numLoops"))
			return false;
		if (!XMLTools::attributeExists(PatternNode, "retainPitch"))
			return false;
		if (!XMLTools::attributeStringValid(PatternNode, "retainPitch", trueFalseList, 1))
			return false;
	}
	return true;
}
//END MapEvent::MovementPattern Member Functions

//BEGIN MapEvent::Precondition Member Functions
MapEvent::Precondition::Precondition()
{
	type = SWITCH;
	conditionUnion.switchCondition = new SwitchCondition;
	conditionUnion.switchCondition->switchID = 0;
	conditionUnion.switchCondition->switchReferencedInVariable = false;
	conditionUnion.switchCondition->isOn = true;
}

MapEvent::Precondition::Precondition(XMLNode ConditionNode)
{
	QString tempString;
	tempString = ConditionNode.getAttribute("type");
	if (tempString == "switch")
	{
		type = SWITCH;
		conditionUnion.switchCondition = new SwitchCondition;
		conditionUnion.switchCondition->switchID = QString(ConditionNode.getAttribute("switchID")).toInt();
		tempString = ConditionNode.getAttribute("switchReferencedInVariable");
		conditionUnion.switchCondition->switchReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("isOn");
		conditionUnion.switchCondition->isOn = (tempString == "true") ? true:false;
	}
	else if (tempString == "variable")
	{
		type = VARIABLE;
		conditionUnion.variableCondition = new VariableCondition;
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.variableCondition->comparison = VariableCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.variableCondition->comparison = VariableCondition::NOT_EQUAL_TO;
		else if (tempString == ">")
			conditionUnion.variableCondition->comparison = VariableCondition::GREATER_THAN;
		else if (tempString == ">=")
			conditionUnion.variableCondition->comparison = VariableCondition::GREATER_THAN_OR_EQUAL_TO;
		else if (tempString == "<")
			conditionUnion.variableCondition->comparison = VariableCondition::LESS_THAN;
		else if (tempString == "<=")
			conditionUnion.variableCondition->comparison = VariableCondition::LESS_THAN_OR_EQUAL_TO;
		conditionUnion.variableCondition->srcVariableID = QString(ConditionNode.getAttribute("srcVariableID")).toInt();
		conditionUnion.variableCondition->dstValue = QString(ConditionNode.getAttribute("dstValue")).toInt();
		tempString = ConditionNode.getAttribute("dstValueStoredInVariable");
		conditionUnion.variableCondition->dstValueStoredInVariable = (tempString == "true") ? true:false;
	}
	else if (tempString == "string")
	{
		type = STRING;
		conditionUnion.stringCondition = new StringCondition;
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.stringCondition->comparison = StringCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.stringCondition->comparison = StringCondition::NOT_EQUAL_TO;
		else if (tempString == "startsWith")
			conditionUnion.stringCondition->comparison = StringCondition::STARTS_WITH;
		else if (tempString == "endsWith")
			conditionUnion.stringCondition->comparison = StringCondition::ENDS_WITH;
		else if (tempString == "contains")
			conditionUnion.stringCondition->comparison = StringCondition::CONTAINS;
		else if (tempString == "doesNotContain")
			conditionUnion.stringCondition->comparison = StringCondition::DOES_NOT_CONTAIN;
		else if (tempString == "isEmpty")
			conditionUnion.stringCondition->comparison = StringCondition::IS_EMPTY;
		else if (tempString == "isNotEmpty")
			conditionUnion.stringCondition->comparison = StringCondition::IS_NOT_EMPTY;
		conditionUnion.stringCondition->srcStringID = QString(ConditionNode.getAttribute("srcStringID")).toInt();
		tempString = ConditionNode.getAttribute("srcStringReferencedInVariable");
		conditionUnion.stringCondition->srcStringReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("dstStringType");
		if (tempString == "stringVariable")
		{
			conditionUnion.stringCondition->dstStringType = StringCondition::STRING_VARIABLE;
			conditionUnion.stringCondition->dstValue = "";
			conditionUnion.stringCondition->dstStringID = QString(ConditionNode.getAttribute("dstStringID")).toInt();
		}
		else if (tempString == "referencedString")
		{
			conditionUnion.stringCondition->dstStringType = StringCondition::REFERENCED_STRING;
			conditionUnion.stringCondition->dstValue = "";
			conditionUnion.stringCondition->dstStringID = QString(ConditionNode.getAttribute("dstVariableID")).toInt();
		}
		else if (tempString == "value")
		{
			conditionUnion.stringCondition->dstStringType = StringCondition::VALUE;
			conditionUnion.stringCondition->dstValue = ConditionNode.getAttribute("dstValue");
			conditionUnion.stringCondition->dstStringID = 0;
		}
	}
	else if (tempString == "timer")
	{
		type = TIMER;
		conditionUnion.timerCondition = new TimerCondition;
		conditionUnion.timerCondition->timerID = QString(ConditionNode.getAttribute("timerID")).toInt();
		tempString = ConditionNode.getAttribute("timerReferencedInVariable");
		conditionUnion.timerCondition->timerReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("timeStoredInVariable");
		conditionUnion.timerCondition->timeStoredInVariable = (tempString == "true") ? true:false;
		if (conditionUnion.timerCondition->timeStoredInVariable)
		{
			conditionUnion.timerCondition->minutes = 0;
			conditionUnion.timerCondition->seconds = QString(ConditionNode.getAttribute("variableID")).toInt();
		}
		else
		{
			conditionUnion.timerCondition->minutes = QString(ConditionNode.getAttribute("minutes")).toInt();
			conditionUnion.timerCondition->seconds = QString(ConditionNode.getAttribute("seconds")).toInt();
		}
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.timerCondition->comparison = TimerCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.timerCondition->comparison = TimerCondition::NOT_EQUAL_TO;
		else if (tempString == ">")
			conditionUnion.timerCondition->comparison = TimerCondition::GREATER_THAN;
		else if (tempString == ">=")
			conditionUnion.timerCondition->comparison = TimerCondition::GREATER_THAN_OR_EQUAL_TO;
		else if (tempString == "<")
			conditionUnion.timerCondition->comparison = TimerCondition::LESS_THAN;
		else if (tempString == "<=")
			conditionUnion.timerCondition->comparison = TimerCondition::LESS_THAN_OR_EQUAL_TO;
	}
	else if (tempString == "money")
	{
		type = MONEY;
		conditionUnion.moneyCondition = new MoneyCondition;
		tempString = ConditionNode.getAttribute("comparison");
		if (tempString == "==")
			conditionUnion.moneyCondition->comparison = MoneyCondition::EQUAL_TO;
		else if (tempString == "!=")
			conditionUnion.moneyCondition->comparison = MoneyCondition::NOT_EQUAL_TO;
		else if (tempString == ">")
			conditionUnion.moneyCondition->comparison = MoneyCondition::GREATER_THAN;
		else if (tempString == ">=")
			conditionUnion.moneyCondition->comparison = MoneyCondition::GREATER_THAN_OR_EQUAL_TO;
		else if (tempString == "<")
			conditionUnion.moneyCondition->comparison = MoneyCondition::LESS_THAN;
		else if (tempString == "<=")
			conditionUnion.moneyCondition->comparison = MoneyCondition::LESS_THAN_OR_EQUAL_TO;
		conditionUnion.moneyCondition->value = QString(ConditionNode.getAttribute("value")).toInt();
		tempString = ConditionNode.getAttribute("valueStoredInVariable");
		conditionUnion.moneyCondition->valueStoredInVariable = (tempString == "true") ? true:false;
	}
	else if (tempString == "item")
	{
		type = ITEM;
		conditionUnion.itemCondition = new ItemCondition;
		conditionUnion.itemCondition->itemID = QString(ConditionNode.getAttribute("itemID")).toInt();
		tempString = ConditionNode.getAttribute("itemReferencedInVariable");
		conditionUnion.itemCondition->itemReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("partyPossesses");
		conditionUnion.itemCondition->partyPossesses = (tempString == "true") ? true:false;
	}
	else if (tempString == "hero")
	{
		type = HERO;
		conditionUnion.heroCondition = new HeroCondition;
		conditionUnion.heroCondition->characterID = QString(ConditionNode.getAttribute("characterID")).toInt();
		tempString = ConditionNode.getAttribute("characterReferencedInVariable");
		conditionUnion.heroCondition->characterReferencedInVariable = (tempString == "true") ? true:false;
		tempString = ConditionNode.getAttribute("heroConditionType");
		if (tempString == "inParty")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::IN_PARTY;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			conditionUnion.heroCondition->valueStoredInVariable = false;
		}
		else if (tempString == "nameIs")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::NAME_IS;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			tempString = ConditionNode.getAttribute("stringStoredInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
			if (tempString == "true")
				conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("string")).toInt();
			else
				conditionUnion.heroCondition->string = ConditionNode.getAttribute("string");
		}
		else if (tempString == "levelIsAtLeast")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::LEVEL_IS_AT_LEAST;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("level")).toInt();
			tempString = ConditionNode.getAttribute("levelStoredInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "hpIsAtLeast")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::HP_IS_AT_LEAST;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("hp")).toInt();
			tempString = ConditionNode.getAttribute("hpStoredInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "canUseSkill")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::CAN_USE_SKILL;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("skillID")).toInt();
			tempString = ConditionNode.getAttribute("skillReferencedInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "hasItemEquipped")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::HAS_ITEM_EQUIPPED;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("itemID")).toInt();
			tempString = ConditionNode.getAttribute("itemReferencedInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
		else if (tempString == "afflictedWithCondition")
		{
			conditionUnion.heroCondition->conditionType = HeroCondition::AFFLICTED_WITH_CONDITION;
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = QString(ConditionNode.getAttribute("conditionID")).toInt();
			tempString = ConditionNode.getAttribute("conditionReferencedInVariable");
			conditionUnion.heroCondition->valueStoredInVariable = (tempString == "true") ? true:false;
		}
	}
	else if (tempString == "partyIsRiding")
	{
		type = PARTY_IS_RIDING;
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
		conditionUnion.partyIsRidingCondition->vehicleID = QString(ConditionNode.getAttribute("vehicleID")).toInt();
		tempString = ConditionNode.getAttribute("vehicleReferencedInVariable");
		conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = (tempString == "true") ? true:false;
	}
}

MapEvent::Precondition::Precondition(StorageFile &storageFile)
{
	char *temp_string;
	type = (ConditionType)storageFile.getSignedInt();
	if (type == SWITCH)
	{
		conditionUnion.switchCondition = new SwitchCondition;
		conditionUnion.switchCondition->switchID = storageFile.getSignedInt();
		conditionUnion.switchCondition->switchReferencedInVariable = storageFile.getBool();
		conditionUnion.switchCondition->isOn = storageFile.getBool();
	}
	else if (type == VARIABLE)
	{
		conditionUnion.variableCondition = new VariableCondition;
		conditionUnion.variableCondition->comparison = storageFile.getSignedInt();
		conditionUnion.variableCondition->srcVariableID = storageFile.getSignedInt();
		conditionUnion.variableCondition->dstValue = storageFile.getSignedInt();
		conditionUnion.variableCondition->dstValueStoredInVariable = storageFile.getBool();
	}
	else if (type == STRING)
	{
		conditionUnion.stringCondition = new StringCondition;
		conditionUnion.stringCondition->comparison = storageFile.getSignedInt();
		conditionUnion.stringCondition->dstStringType = storageFile.getSignedInt();
		conditionUnion.stringCondition->srcStringID = storageFile.getSignedInt();
		conditionUnion.stringCondition->srcStringReferencedInVariable = storageFile.getBool();
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE || conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
		{
			conditionUnion.stringCondition->dstValue = "";
			conditionUnion.stringCondition->dstStringID = storageFile.getSignedInt();
		}
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::VALUE)
		{
			temp_string = storageFile.getString();
			conditionUnion.stringCondition->dstValue = temp_string;
			delete[] temp_string;
			conditionUnion.stringCondition->dstStringID = 0;
		}
	}
	else if (type == TIMER)
	{
		conditionUnion.timerCondition = new TimerCondition;
		conditionUnion.timerCondition->comparison = storageFile.getSignedInt();
		conditionUnion.timerCondition->timerID = storageFile.getSignedInt();
		conditionUnion.timerCondition->timerReferencedInVariable = storageFile.getBool();
		conditionUnion.timerCondition->timeStoredInVariable = storageFile.getBool();
		if (conditionUnion.timerCondition->timeStoredInVariable)
		{
			conditionUnion.timerCondition->minutes = 0;
			conditionUnion.timerCondition->seconds = storageFile.getSignedInt();
		}
		else
		{
			conditionUnion.timerCondition->minutes = storageFile.getSignedInt();
			conditionUnion.timerCondition->seconds = storageFile.getSignedInt();
		}
	}
	else if (type == MONEY)
	{
		conditionUnion.moneyCondition = new MoneyCondition;
		conditionUnion.moneyCondition->comparison = storageFile.getSignedInt();
		conditionUnion.moneyCondition->value = storageFile.getSignedInt();
		conditionUnion.moneyCondition->valueStoredInVariable = storageFile.getBool();
	}
	else if (type == ITEM)
	{
		conditionUnion.itemCondition = new ItemCondition;
		conditionUnion.itemCondition->itemID = storageFile.getSignedInt();
		conditionUnion.itemCondition->itemReferencedInVariable = storageFile.getBool();
		conditionUnion.itemCondition->partyPossesses = storageFile.getBool();
	}
	else if (type == HERO)
	{
		conditionUnion.heroCondition = new HeroCondition;
		conditionUnion.heroCondition->conditionType = storageFile.getSignedInt();
		conditionUnion.heroCondition->characterID = storageFile.getSignedInt();
		conditionUnion.heroCondition->characterReferencedInVariable = storageFile.getBool();
		if (conditionUnion.heroCondition->conditionType == HeroCondition::IN_PARTY)
		{
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			conditionUnion.heroCondition->valueStoredInVariable = false;
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = 0;
			conditionUnion.heroCondition->valueStoredInVariable = storageFile.getBool();
			if (conditionUnion.heroCondition->valueStoredInVariable)
				conditionUnion.heroCondition->value = storageFile.getSignedInt();
			else
			{
				temp_string = storageFile.getString();
				conditionUnion.heroCondition->string = temp_string;
				delete[] temp_string;
			}
		}
		else
		{
			conditionUnion.heroCondition->string = "";
			conditionUnion.heroCondition->value = storageFile.getSignedInt();
			conditionUnion.heroCondition->valueStoredInVariable = storageFile.getBool();
		}
	}
	else if (type == PARTY_IS_RIDING)
	{
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
		conditionUnion.partyIsRidingCondition->vehicleID = storageFile.getSignedInt();
		conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = storageFile.getBool();
	}
}

MapEvent::Precondition::Precondition(Precondition *other)
{
	type = other->type;
	if (type == SWITCH)
	{
		conditionUnion.switchCondition = new SwitchCondition;
		conditionUnion.switchCondition->switchID = other->conditionUnion.switchCondition->switchID;
		conditionUnion.switchCondition->switchReferencedInVariable = other->conditionUnion.switchCondition->switchReferencedInVariable;
		conditionUnion.switchCondition->isOn = other->conditionUnion.switchCondition->isOn;
	}
	else if (type == VARIABLE)
	{
		conditionUnion.variableCondition = new VariableCondition;
		conditionUnion.variableCondition->comparison = other->conditionUnion.variableCondition->comparison;
		conditionUnion.variableCondition->srcVariableID = other->conditionUnion.variableCondition->srcVariableID;
		conditionUnion.variableCondition->dstValue = other->conditionUnion.variableCondition->dstValue;
		conditionUnion.variableCondition->dstValueStoredInVariable = other->conditionUnion.variableCondition->dstValueStoredInVariable;
	}
	else if (type == STRING)
	{
		conditionUnion.stringCondition = new StringCondition;
		conditionUnion.stringCondition->comparison = other->conditionUnion.stringCondition->comparison;
		conditionUnion.stringCondition->dstStringType = other->conditionUnion.stringCondition->dstStringType;
		conditionUnion.stringCondition->dstValue = "";
		conditionUnion.stringCondition->dstValue.append(other->conditionUnion.stringCondition->dstValue);
		conditionUnion.stringCondition->srcStringID = other->conditionUnion.stringCondition->srcStringID;
		conditionUnion.stringCondition->dstStringID = other->conditionUnion.stringCondition->dstStringID;
		conditionUnion.stringCondition->srcStringReferencedInVariable = other->conditionUnion.stringCondition->srcStringReferencedInVariable;
	}
	else if (type == TIMER)
	{
		conditionUnion.timerCondition = new TimerCondition;
		conditionUnion.timerCondition->comparison = other->conditionUnion.timerCondition->comparison;
		conditionUnion.timerCondition->timerID = other->conditionUnion.timerCondition->timerID;
		conditionUnion.timerCondition->minutes = other->conditionUnion.timerCondition->minutes;
		conditionUnion.timerCondition->seconds = other->conditionUnion.timerCondition->seconds;
		conditionUnion.timerCondition->timerReferencedInVariable = other->conditionUnion.timerCondition->timerReferencedInVariable;
		conditionUnion.timerCondition->timeStoredInVariable = other->conditionUnion.timerCondition->timeStoredInVariable;
	}
	else if (type == MONEY)
	{
		conditionUnion.moneyCondition = new MoneyCondition;
		conditionUnion.moneyCondition->comparison = other->conditionUnion.moneyCondition->comparison;
		conditionUnion.moneyCondition->value = other->conditionUnion.moneyCondition->value;
		conditionUnion.moneyCondition->valueStoredInVariable = other->conditionUnion.moneyCondition->valueStoredInVariable;
	}
	else if (type == ITEM)
	{
		conditionUnion.itemCondition = new ItemCondition;
		conditionUnion.itemCondition->itemID = other->conditionUnion.itemCondition->itemID;
		conditionUnion.itemCondition->itemReferencedInVariable = other->conditionUnion.itemCondition->itemReferencedInVariable;
		conditionUnion.itemCondition->partyPossesses = other->conditionUnion.itemCondition->partyPossesses;
	}
	else if (type == HERO)
	{
		conditionUnion.heroCondition = new HeroCondition;
		conditionUnion.heroCondition->conditionType = other->conditionUnion.heroCondition->conditionType;
		conditionUnion.heroCondition->string = "";
		conditionUnion.heroCondition->string.append(other->conditionUnion.heroCondition->string);
		conditionUnion.heroCondition->value = other->conditionUnion.heroCondition->value;
		conditionUnion.heroCondition->characterID = other->conditionUnion.heroCondition->characterID;
		conditionUnion.heroCondition->characterReferencedInVariable = other->conditionUnion.heroCondition->characterReferencedInVariable;
		conditionUnion.heroCondition->valueStoredInVariable = other->conditionUnion.heroCondition->valueStoredInVariable;
	}
	else if (type == PARTY_IS_RIDING)
	{
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
		conditionUnion.partyIsRidingCondition->vehicleID = other->conditionUnion.partyIsRidingCondition->vehicleID;
		conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = other->conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable;
	}
}

MapEvent::Precondition::~Precondition()
{
	if (type == SWITCH)
		delete (conditionUnion.switchCondition);
	else if (type == VARIABLE)
		delete (conditionUnion.variableCondition);
	else if (type == STRING)
		delete (conditionUnion.stringCondition);
	else if (type == TIMER)
		delete (conditionUnion.timerCondition);
	else if (type == MONEY)
		delete (conditionUnion.moneyCondition);
	else if (type == ITEM)
		delete (conditionUnion.itemCondition);
	else if (type == HERO)
		delete (conditionUnion.heroCondition);
	else if (type == PARTY_IS_RIDING)
		delete (conditionUnion.partyIsRidingCondition);
}

XMLNode MapEvent::Precondition::getConditionNode()
{
	XMLNode ConditionNode = XMLNode::createXMLTopNode("condition");
	if (type == SWITCH)
	{
		ConditionNode.addAttribute("type", "switch");
		ConditionNode.addAttribute("switchID", QString::number(conditionUnion.switchCondition->switchID).toUtf8().data());
		ConditionNode.addAttribute("switchReferencedInVariable", (conditionUnion.switchCondition->switchReferencedInVariable) ? "true":"false");
		ConditionNode.addAttribute("isOn", (conditionUnion.switchCondition->isOn) ? "true":"false");
	}
	else if (type == VARIABLE)
	{
		ConditionNode.addAttribute("type", "variable");
		if (conditionUnion.variableCondition->comparison == VariableCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN)
			ConditionNode.addAttribute("comparison", ">");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", ">=");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN)
			ConditionNode.addAttribute("comparison", "<");
		else if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "<=");
		ConditionNode.addAttribute("srcVariableID", QString::number(conditionUnion.variableCondition->srcVariableID).toUtf8().data());
		ConditionNode.addAttribute("dstValue", QString::number(conditionUnion.variableCondition->dstValue).toUtf8().data());
		ConditionNode.addAttribute("dstValueStoredInVariable", (conditionUnion.variableCondition->dstValueStoredInVariable) ? "true":"false");
	}
	else if (type == STRING)
	{
		ConditionNode.addAttribute("type", "string");
		if (conditionUnion.stringCondition->comparison == StringCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.stringCondition->comparison == StringCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.stringCondition->comparison == StringCondition::STARTS_WITH)
			ConditionNode.addAttribute("comparison", "startsWith");
		else if (conditionUnion.stringCondition->comparison == StringCondition::ENDS_WITH)
			ConditionNode.addAttribute("comparison", "endsWith");
		else if (conditionUnion.stringCondition->comparison == StringCondition::CONTAINS)
			ConditionNode.addAttribute("comparison", "contains");
		else if (conditionUnion.stringCondition->comparison == StringCondition::DOES_NOT_CONTAIN)
			ConditionNode.addAttribute("comparison", "doesNotContain");
		else if (conditionUnion.stringCondition->comparison == StringCondition::IS_EMPTY)
			ConditionNode.addAttribute("comparison", "isEmpty");
		else if (conditionUnion.stringCondition->comparison == StringCondition::IS_NOT_EMPTY)
			ConditionNode.addAttribute("comparison", "isNotEmpty");
		ConditionNode.addAttribute("srcStringID", QString::number(conditionUnion.stringCondition->srcStringID).toUtf8().data());
		ConditionNode.addAttribute("srcStringReferencedInVariable", (conditionUnion.stringCondition->srcStringReferencedInVariable) ? "true":"false");
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE)
		{
			ConditionNode.addAttribute("dstStringType", "stringVariable");
			ConditionNode.addAttribute("dstStringID", QString::number(conditionUnion.stringCondition->dstStringID).toUtf8().data());
		}
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
		{
			ConditionNode.addAttribute("dstStringType", "referencedString");
			ConditionNode.addAttribute("dstVariableID", QString::number(conditionUnion.stringCondition->dstStringID).toUtf8().data());
		}
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::VALUE)
		{
			ConditionNode.addAttribute("dstStringType", "value");
			ConditionNode.addAttribute("dstValue", conditionUnion.stringCondition->dstValue.toUtf8().data());
		}
	}
	else if (type == TIMER)
	{
		ConditionNode.addAttribute("type", "timer");
		if (conditionUnion.timerCondition->comparison == TimerCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN)
			ConditionNode.addAttribute("comparison", ">");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", ">=");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN)
			ConditionNode.addAttribute("comparison", "<");
		else if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "<=");
		ConditionNode.addAttribute("timerID", QString::number(conditionUnion.timerCondition->timerID).toUtf8().data());
		ConditionNode.addAttribute("timerReferencedInVariable", (conditionUnion.timerCondition->timerReferencedInVariable) ? "true":"false");
		ConditionNode.addAttribute("timeStoredInVariable", (conditionUnion.timerCondition->timeStoredInVariable) ? "true":"false");
		if (conditionUnion.timerCondition->timeStoredInVariable)
			ConditionNode.addAttribute("variableID", QString::number(conditionUnion.timerCondition->seconds).toUtf8().data());
		else
		{
			ConditionNode.addAttribute("minutes", QString::number(conditionUnion.timerCondition->minutes).toUtf8().data());
			ConditionNode.addAttribute("seconds", QString::number(conditionUnion.timerCondition->seconds).toUtf8().data());
		}
	}
	else if (type == MONEY)
	{
		ConditionNode.addAttribute("type", "money");
		if (conditionUnion.moneyCondition->comparison == MoneyCondition::EQUAL_TO)
			ConditionNode.addAttribute("comparison", "==");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::NOT_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "!=");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::GREATER_THAN)
			ConditionNode.addAttribute("comparison", ">");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::GREATER_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", ">=");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::LESS_THAN)
			ConditionNode.addAttribute("comparison", "<");
		else if (conditionUnion.moneyCondition->comparison == MoneyCondition::LESS_THAN_OR_EQUAL_TO)
			ConditionNode.addAttribute("comparison", "<=");
		ConditionNode.addAttribute("value", QString::number(conditionUnion.moneyCondition->value).toUtf8().data());
		ConditionNode.addAttribute("valueStoredInVariable", (conditionUnion.moneyCondition->valueStoredInVariable) ? "true":"false");
	}
	else if (type == ITEM)
	{
		ConditionNode.addAttribute("type", "item");
		ConditionNode.addAttribute("itemID", QString::number(conditionUnion.itemCondition->itemID).toUtf8().data());
		ConditionNode.addAttribute("itemReferencedInVariable", (conditionUnion.itemCondition->itemReferencedInVariable) ? "true":"false");
		ConditionNode.addAttribute("partyPossesses", (conditionUnion.itemCondition->partyPossesses) ? "true":"false");
	}
	else if (type == HERO)
	{
		ConditionNode.addAttribute("type", "hero");
		ConditionNode.addAttribute("characterID", QString::number(conditionUnion.heroCondition->characterID).toUtf8().data());
		ConditionNode.addAttribute("characterReferencedInVariable", (conditionUnion.heroCondition->characterReferencedInVariable) ? "true":"false");
		if (conditionUnion.heroCondition->conditionType == HeroCondition::IN_PARTY)
			ConditionNode.addAttribute("heroConditionType", "inParty");
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			ConditionNode.addAttribute("heroConditionType", "nameIs");
			ConditionNode.addAttribute("stringStoredInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
			if (conditionUnion.heroCondition->valueStoredInVariable)
				ConditionNode.addAttribute("string", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			else
				ConditionNode.addAttribute("string", conditionUnion.heroCondition->string.toUtf8().data());
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::LEVEL_IS_AT_LEAST)
		{
			ConditionNode.addAttribute("heroConditionType", "levelIsAtLeast");
			ConditionNode.addAttribute("level", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("levelStoredInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HP_IS_AT_LEAST)
		{
			ConditionNode.addAttribute("heroConditionType", "hpIsAtLeast");
			ConditionNode.addAttribute("hp", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("hpStoredInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::CAN_USE_SKILL)
		{
			ConditionNode.addAttribute("heroConditionType", "canUseSkill");
			ConditionNode.addAttribute("skillID", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("skillReferencedInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HAS_ITEM_EQUIPPED)
		{
			ConditionNode.addAttribute("heroConditionType", "hasItemEquipped");
			ConditionNode.addAttribute("itemID", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("itemReferencedInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::AFFLICTED_WITH_CONDITION)
		{
			ConditionNode.addAttribute("heroConditionType", "afflictedWithCondition");
			ConditionNode.addAttribute("conditionID", QString::number(conditionUnion.heroCondition->value).toUtf8().data());
			ConditionNode.addAttribute("conditionReferencedInVariable", (conditionUnion.heroCondition->valueStoredInVariable) ? "true":"false");
		}
	}
	else if (type == PARTY_IS_RIDING)
	{
		ConditionNode.addAttribute("type", "partyIsRiding");
		ConditionNode.addAttribute("vehicleID", QString::number(conditionUnion.partyIsRidingCondition->vehicleID).toUtf8().data());
		ConditionNode.addAttribute("vehicleReferencedInVariable", (conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable) ? "true":"false");
	}
	return ConditionNode;
}

void MapEvent::Precondition::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt((int)type);
	if (type == SWITCH)
	{
		storageFile.putSignedInt(conditionUnion.switchCondition->switchID);
		storageFile.putBool(conditionUnion.switchCondition->switchReferencedInVariable);
		storageFile.putBool(conditionUnion.switchCondition->isOn);
	}
	else if (type == VARIABLE)
	{
		storageFile.putSignedInt(conditionUnion.variableCondition->comparison);
		storageFile.putSignedInt(conditionUnion.variableCondition->srcVariableID);
		storageFile.putSignedInt(conditionUnion.variableCondition->dstValue);
		storageFile.putBool(conditionUnion.variableCondition->dstValueStoredInVariable);
	}
	else if (type == STRING)
	{
		storageFile.putSignedInt(conditionUnion.stringCondition->comparison);
		storageFile.putSignedInt(conditionUnion.stringCondition->dstStringType);
		storageFile.putSignedInt(conditionUnion.stringCondition->srcStringID);
		storageFile.putBool(conditionUnion.stringCondition->srcStringReferencedInVariable);
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE || conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
			storageFile.putSignedInt(conditionUnion.stringCondition->dstStringID);
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::VALUE)
			storageFile.putString(conditionUnion.stringCondition->dstValue.toUtf8().data());
	}
	else if (type == TIMER)
	{
		storageFile.putSignedInt((int)conditionUnion.timerCondition->comparison);
		storageFile.putSignedInt(conditionUnion.timerCondition->timerID);
		storageFile.putBool(conditionUnion.timerCondition->timerReferencedInVariable);
		storageFile.putBool(conditionUnion.timerCondition->timeStoredInVariable);
		if (conditionUnion.timerCondition->timeStoredInVariable)
			storageFile.putSignedInt(conditionUnion.timerCondition->seconds);
		else
		{
			storageFile.putSignedInt(conditionUnion.timerCondition->minutes);
			storageFile.putSignedInt(conditionUnion.timerCondition->seconds);
		}
	}
	else if (type == MONEY)
	{
		storageFile.putSignedInt((int)conditionUnion.moneyCondition->comparison);
		storageFile.putSignedInt(conditionUnion.moneyCondition->value);
		storageFile.putBool(conditionUnion.moneyCondition->valueStoredInVariable);
	}
	else if (type == ITEM)
	{
		storageFile.putSignedInt(conditionUnion.itemCondition->itemID);
		storageFile.putBool(conditionUnion.itemCondition->itemReferencedInVariable);
		storageFile.putBool(conditionUnion.itemCondition->partyPossesses);
	}
	else if (type == HERO)
	{
		storageFile.putSignedInt((int)conditionUnion.heroCondition->conditionType);
		storageFile.putSignedInt(conditionUnion.heroCondition->characterID);
		storageFile.putBool(conditionUnion.heroCondition->characterReferencedInVariable);
		if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			storageFile.putBool(conditionUnion.heroCondition->valueStoredInVariable);
			if (conditionUnion.heroCondition->valueStoredInVariable)
				storageFile.putSignedInt(conditionUnion.heroCondition->value);
			else
				storageFile.putString(conditionUnion.heroCondition->string.toUtf8().data());
		}
		else
		{
			storageFile.putSignedInt(conditionUnion.heroCondition->value);
			storageFile.putBool(conditionUnion.heroCondition->valueStoredInVariable);
		}
	}
	else if (type == PARTY_IS_RIDING)
	{
		storageFile.putSignedInt(conditionUnion.partyIsRidingCondition->vehicleID);
		storageFile.putBool(conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable);
	}
}

QString MapEvent::Precondition::getViewableText() const
{
	QString viewableText = "";
	if (type == SWITCH)
	{
		viewableText += "Switch: ";
		if (conditionUnion.switchCondition->switchReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.switchCondition->switchID));
		else
			viewableText += QString("switch[%1]").arg(ProjectData::switchVariableIdToString(conditionUnion.switchCondition->switchID));
		if (conditionUnion.switchCondition->isOn)
			viewableText += " Is ON";
		else
			viewableText += " Is OFF";
	}
	else if (type == VARIABLE)
	{
		viewableText += "Variable: ";
		viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.variableCondition->srcVariableID));
		if (conditionUnion.variableCondition->comparison == VariableCondition::EQUAL_TO)
			viewableText += " == ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::NOT_EQUAL_TO)
			viewableText += " != ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN)
			viewableText += " > ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN_OR_EQUAL_TO)
			viewableText += " >= ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN)
			viewableText += " < ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN_OR_EQUAL_TO)
			viewableText += " <= ";
		if (conditionUnion.variableCondition->dstValueStoredInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.variableCondition->dstValue));
		else
			viewableText += QString::number(conditionUnion.variableCondition->dstValue);
	}
	else if (type == STRING)
	{
		viewableText += "String: ";
		if (conditionUnion.stringCondition->srcStringReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.stringCondition->srcStringID));
		else
			viewableText += QString("string[%1]").arg(ProjectData::stringVariableIdToString(conditionUnion.stringCondition->srcStringID));
		if (conditionUnion.stringCondition->comparison == StringCondition::EQUAL_TO)
			viewableText += " Is Equal To ";
		if (conditionUnion.stringCondition->comparison == StringCondition::NOT_EQUAL_TO)
			viewableText += " Isn\'t Equal To ";
		if (conditionUnion.stringCondition->comparison == StringCondition::STARTS_WITH)
			viewableText += " Starts With ";
		if (conditionUnion.stringCondition->comparison == StringCondition::ENDS_WITH)
			viewableText += " Ends With ";
		if (conditionUnion.stringCondition->comparison == StringCondition::CONTAINS)
			viewableText += " Contains ";
		if (conditionUnion.stringCondition->comparison == StringCondition::DOES_NOT_CONTAIN)
			viewableText += " Doesn\'t Contain ";
		if (conditionUnion.stringCondition->comparison == StringCondition::IS_EMPTY)
			viewableText += " Is Empty ";
		if (conditionUnion.stringCondition->comparison == StringCondition::IS_NOT_EMPTY)
			viewableText += " Isn\'t Empty ";
		if (conditionUnion.stringCondition->dstStringType == StringCondition::STRING_VARIABLE)
			viewableText += QString("string[%1]").arg(ProjectData::stringVariableIdToString(conditionUnion.stringCondition->dstStringID));
		else if (conditionUnion.stringCondition->dstStringType == StringCondition::REFERENCED_STRING)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.stringCondition->dstStringID));
		else
			viewableText += QString("\"%1\"").arg(conditionUnion.stringCondition->dstValue);
	}
	else if (type == TIMER)
	{
		viewableText += "Timer: ";
		if (conditionUnion.timerCondition->timerReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.timerCondition->timerID));
		else
			viewableText += QString::number(conditionUnion.timerCondition->timerID);
		if (conditionUnion.timerCondition->comparison == TimerCondition::EQUAL_TO)
			viewableText += " == ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::NOT_EQUAL_TO)
			viewableText += " != ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN)
			viewableText += " > ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::GREATER_THAN_OR_EQUAL_TO)
			viewableText += " >= ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN)
			viewableText += " < ";
		if (conditionUnion.timerCondition->comparison == TimerCondition::LESS_THAN_OR_EQUAL_TO)
			viewableText += " <= ";
		if (conditionUnion.timerCondition->timeStoredInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.timerCondition->seconds));
		else
			viewableText += QString("%1:%2").arg(conditionUnion.timerCondition->minutes).arg(conditionUnion.timerCondition->seconds);
	}
	else if (type == MONEY)
	{
		viewableText += "Money: ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::EQUAL_TO)
			viewableText += "== ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::NOT_EQUAL_TO)
			viewableText += "!= ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN)
			viewableText += "> ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::GREATER_THAN_OR_EQUAL_TO)
			viewableText += ">= ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN)
			viewableText += "< ";
		if (conditionUnion.variableCondition->comparison == VariableCondition::LESS_THAN_OR_EQUAL_TO)
			viewableText += "<= ";
		if (conditionUnion.moneyCondition->valueStoredInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.moneyCondition->value));
		else
			viewableText += QString::number(conditionUnion.moneyCondition->value);
	}
	else if (type == ITEM)
	{
		viewableText += "Item: ";
		if (conditionUnion.itemCondition->partyPossesses)
			viewableText += "Party Possesses ";
		else
			viewableText += "Party Doesn't Possess ";
		if (conditionUnion.itemCondition->itemReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.itemCondition->itemID));
		else
			viewableText += ProjectData::itemNames[conditionUnion.itemCondition->itemID];
	}
	else if (type == HERO)
	{
		viewableText += "Hero: ";
		if (conditionUnion.heroCondition->characterReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.heroCondition->characterID));
		else
			viewableText += ProjectData::characterNames[conditionUnion.heroCondition->characterID];
		if (conditionUnion.heroCondition->conditionType == HeroCondition::IN_PARTY)
			viewableText += " Is In Party";
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::NAME_IS)
		{
			viewableText += "\'s Name Is ";
			if (conditionUnion.heroCondition->valueStoredInVariable)
				viewableText += QString("string[%1]").arg(ProjectData::stringVariableIdToString(conditionUnion.heroCondition->value));
			else
				viewableText += conditionUnion.heroCondition->string;
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::LEVEL_IS_AT_LEAST)
		{
			viewableText += "\'s Level Is At Least ";
			viewableText += QString::number(conditionUnion.heroCondition->value);
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HP_IS_AT_LEAST)
		{
			viewableText += "\'s HP Is At Least ";
			viewableText += QString::number(conditionUnion.heroCondition->value);
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::CAN_USE_SKILL)
		{
			viewableText += " Can Use ";
			viewableText += ProjectData::skillNames[conditionUnion.heroCondition->value];
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::HAS_ITEM_EQUIPPED)
		{
			viewableText += " Has ";
			viewableText += ProjectData::itemNames[conditionUnion.heroCondition->value];
			viewableText += " Equipped";
		}
		else if (conditionUnion.heroCondition->conditionType == HeroCondition::AFFLICTED_WITH_CONDITION)
		{
			viewableText += " Afflicted With ";
			viewableText += ProjectData::attributeNames[conditionUnion.heroCondition->value];
		}
	}
	else if (type == PARTY_IS_RIDING)
	{
		viewableText += "Party Is Riding: ";
		if (conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable)
			viewableText += QString("variable[%1]").arg(ProjectData::numberVariableIdToString(conditionUnion.partyIsRidingCondition->vehicleID));
		else
			viewableText += ProjectData::vehicleNames[conditionUnion.partyIsRidingCondition->vehicleID];
	}
	return viewableText;
}

void MapEvent::Precondition::setType(int value)
{
	if (type == SWITCH)
		delete (conditionUnion.switchCondition);
	else if (type == VARIABLE)
		delete (conditionUnion.variableCondition);
	else if (type == STRING)
		delete (conditionUnion.stringCondition);
	else if (type == TIMER)
		delete (conditionUnion.timerCondition);
	else if (type == MONEY)
		delete (conditionUnion.moneyCondition);
	else if (type == ITEM)
		delete (conditionUnion.itemCondition);
	else if (type == HERO)
		delete (conditionUnion.heroCondition);
	else if (type == PARTY_IS_RIDING)
		delete (conditionUnion.partyIsRidingCondition);
	type = value;
	if (type == SWITCH)
		conditionUnion.switchCondition = new SwitchCondition;
	else if (type == VARIABLE)
		conditionUnion.variableCondition = new VariableCondition;
	else if (type == STRING)
		conditionUnion.stringCondition = new StringCondition;
	else if (type == TIMER)
		conditionUnion.timerCondition = new TimerCondition;
	else if (type == MONEY)
		conditionUnion.moneyCondition = new MoneyCondition;
	else if (type == ITEM)
		conditionUnion.itemCondition = new ItemCondition;
	else if (type == HERO)
		conditionUnion.heroCondition = new HeroCondition;
	else if (type == PARTY_IS_RIDING)
		conditionUnion.partyIsRidingCondition = new PartyIsRidingCondition;
}

void MapEvent::Precondition::getSwitchCondition(SwitchCondition *other)
{
	other->switchID = conditionUnion.switchCondition->switchID;
	other->switchReferencedInVariable = conditionUnion.switchCondition->switchReferencedInVariable;
	other->isOn = conditionUnion.switchCondition->isOn;
}

void MapEvent::Precondition::setSwitchCondition(SwitchCondition *other)
{
	conditionUnion.switchCondition->switchID = other->switchID;
	conditionUnion.switchCondition->switchReferencedInVariable = other->switchReferencedInVariable;
	conditionUnion.switchCondition->isOn = other->isOn;
}

void MapEvent::Precondition::getVariableCondition(VariableCondition *other)
{
	other->srcVariableID = conditionUnion.variableCondition->srcVariableID;
	other->dstValue = conditionUnion.variableCondition->dstValue;
	other->comparison = conditionUnion.variableCondition->comparison;
	other->dstValueStoredInVariable = conditionUnion.variableCondition->dstValueStoredInVariable;
}

void MapEvent::Precondition::setVariableCondition(VariableCondition *other)
{
	conditionUnion.variableCondition->srcVariableID = other->srcVariableID;
	conditionUnion.variableCondition->dstValue = other->dstValue;
	conditionUnion.variableCondition->comparison = other->comparison;
	conditionUnion.variableCondition->dstValueStoredInVariable = other->dstValueStoredInVariable;
}

void MapEvent::Precondition::getStringCondition(StringCondition *other)
{
	other->dstValue = conditionUnion.stringCondition->dstValue;
	other->srcStringID = conditionUnion.stringCondition->srcStringID;
	other->dstStringID = conditionUnion.stringCondition->dstStringID;
	other->comparison = conditionUnion.stringCondition->comparison;
	other->dstStringType = conditionUnion.stringCondition->dstStringType;
	other->srcStringReferencedInVariable = conditionUnion.stringCondition->srcStringReferencedInVariable;
}

void MapEvent::Precondition::setStringCondition(StringCondition *other)
{
	conditionUnion.stringCondition->dstValue = other->dstValue;
	conditionUnion.stringCondition->srcStringID = other->srcStringID;
	conditionUnion.stringCondition->dstStringID = other->dstStringID;
	conditionUnion.stringCondition->comparison = other->comparison;
	conditionUnion.stringCondition->dstStringType = other->dstStringType;
	conditionUnion.stringCondition->srcStringReferencedInVariable = other->srcStringReferencedInVariable;
}

void MapEvent::Precondition::getTimerCondition(TimerCondition *other)
{
	other->timerID = conditionUnion.timerCondition->timerID;
	other->minutes = conditionUnion.timerCondition->minutes;
	other->seconds = conditionUnion.timerCondition->seconds;
	other->comparison = conditionUnion.timerCondition->comparison;
	other->timerReferencedInVariable = conditionUnion.timerCondition->timerReferencedInVariable;
	other->timeStoredInVariable = conditionUnion.timerCondition->timeStoredInVariable;
}

void MapEvent::Precondition::setTimerCondition(TimerCondition *other)
{
	conditionUnion.timerCondition->timerID = other->timerID;
	conditionUnion.timerCondition->minutes = other->minutes;
	conditionUnion.timerCondition->seconds = other->seconds;
	conditionUnion.timerCondition->comparison = other->comparison;
	conditionUnion.timerCondition->timerReferencedInVariable = other->timerReferencedInVariable;
	conditionUnion.timerCondition->timeStoredInVariable = other->timeStoredInVariable;
}

void MapEvent::Precondition::getMoneyCondition(MoneyCondition *other)
{
	other->value = conditionUnion.moneyCondition->value;
	other->comparison = conditionUnion.moneyCondition->comparison;
	other->valueStoredInVariable = conditionUnion.moneyCondition->valueStoredInVariable;
}

void MapEvent::Precondition::setMoneyCondition(MoneyCondition *other)
{
	conditionUnion.moneyCondition->value = other->value;
	conditionUnion.moneyCondition->comparison = other->comparison;
	conditionUnion.moneyCondition->valueStoredInVariable = other->valueStoredInVariable;
}

void MapEvent::Precondition::getItemCondition(ItemCondition *other)
{
	other->itemID = conditionUnion.itemCondition->itemID;
	other->itemReferencedInVariable = conditionUnion.itemCondition->itemReferencedInVariable;
	other->partyPossesses = conditionUnion.itemCondition->partyPossesses;
}

void MapEvent::Precondition::setItemCondition(ItemCondition *other)
{
	conditionUnion.itemCondition->itemID = other->itemID;
	conditionUnion.itemCondition->itemReferencedInVariable = other->itemReferencedInVariable;
	conditionUnion.itemCondition->partyPossesses = other->partyPossesses;
}

void MapEvent::Precondition::getHeroCondition(HeroCondition *other)
{
	other->string = conditionUnion.heroCondition->string;
	other->value = conditionUnion.heroCondition->value;
	other->characterID = conditionUnion.heroCondition->characterID;
	other->conditionType = conditionUnion.heroCondition->conditionType;
	other->characterReferencedInVariable = conditionUnion.heroCondition->characterReferencedInVariable;
	other->valueStoredInVariable = conditionUnion.heroCondition->valueStoredInVariable;
}

void MapEvent::Precondition::setHeroCondition(HeroCondition *other)
{
	conditionUnion.heroCondition->string = other->string;
	conditionUnion.heroCondition->value = other->value;
	conditionUnion.heroCondition->characterID = other->characterID;
	conditionUnion.heroCondition->conditionType = other->conditionType;
	conditionUnion.heroCondition->characterReferencedInVariable = other->characterReferencedInVariable;
	conditionUnion.heroCondition->valueStoredInVariable = other->valueStoredInVariable;
}

void MapEvent::Precondition::getPartyIsRidingCondition(PartyIsRidingCondition *other)
{
	other->vehicleID = conditionUnion.partyIsRidingCondition->vehicleID;
	other->vehicleReferencedInVariable = conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable;
}

void MapEvent::Precondition::setPartyIsRidingCondition(PartyIsRidingCondition *other)
{
	conditionUnion.partyIsRidingCondition->vehicleID = other->vehicleID;
	conditionUnion.partyIsRidingCondition->vehicleReferencedInVariable = other->vehicleReferencedInVariable;
}

bool MapEvent::Precondition::tryLoad(XMLNode ConditionNode)
{
	QStringList tempStringList;
	QStringList trueFalseList = QStringList() << "true" << "false";
	QString tempString;
	int conditionType;
	tempStringList.clear();
	tempStringList << "switch" << "variable" << "string" << "timer" << "money" << "item" << "hero" << "partyIsRiding" << "previousSelectedArea" << "bgmPlayedThroughOnce";
	if (!XMLTools::attributeExists(ConditionNode, "type"))
		return false;
	if (!XMLTools::attributeStringValid(ConditionNode, "type", tempStringList))
		return false;
	conditionType = tempStringList.indexOf(QString(ConditionNode.getAttribute("type")));
	if (conditionType == SWITCH)
	{
		if (!XMLTools::attributeExists(ConditionNode, "switchID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "switchReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "switchReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "isOn"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "isOn", trueFalseList, 1))
			return false;
	}
	else if (conditionType == VARIABLE)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << ">" << ">=" << "<" << "<=";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "srcVariableID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "dstValue"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "dstValueStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "dstValueStoredInVariable", trueFalseList, 1))
			return false;
	}
	else if (conditionType == STRING)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << "startsWith" << "endsWith" << "contains" << "doesNotContain" << "isEmpty" << "isNotEmpty";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "srcStringID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "srcStringReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "srcStringReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "dstStringType"))
			return false;
		tempStringList.clear();
		tempStringList << "stringVariable" << "referencedString" << "value";
		if (!XMLTools::attributeStringValid(ConditionNode, "dstStringType", tempStringList))
			return false;
		tempString = ConditionNode.getAttribute("dstStringType");
		if (tempString == "stringVariable")
		{
			if (!XMLTools::attributeExists(ConditionNode, "dstStringID"))
				return false;
		}
		else if (tempString == "referencedString")
		{
			if (!XMLTools::attributeExists(ConditionNode, "dstVariableID"))
				return false;
		}
		else if (tempString == "value")
		{
			if (!XMLTools::attributeExists(ConditionNode, "dstValue"))
				return false;
		}
	}
	else if (conditionType == TIMER)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << ">" << ">=" << "<" << "<=";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "timerID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "timerReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "timerReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "timeStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "timeStoredInVariable", trueFalseList, 1))
			return false;
		if (QString(ConditionNode.getAttribute("timeStoredInVariable")) == "true")
		{
			if (!XMLTools::attributeExists(ConditionNode, "variableID"))
				return false;
		}
		else
		{
			if (!XMLTools::attributeExists(ConditionNode, "minutes"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "seconds"))
				return false;
			if (!XMLTools::attributeIntRangeValid(ConditionNode, "seconds", 0, 59))
				return false;
		}
	}
	else if (conditionType == MONEY)
	{
		if (!XMLTools::attributeExists(ConditionNode, "comparison"))
			return false;
		tempStringList.clear();
		tempStringList << "==" << "!=" << ">" << ">=" << "<" << "<=";
		if (!XMLTools::attributeStringValid(ConditionNode, "comparison", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "value"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "valueStoredInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "valueStoredInVariable", trueFalseList, 1))
			return false;
	}
	else if (conditionType == ITEM)
	{
		if (!XMLTools::attributeExists(ConditionNode, "itemID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "itemReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "itemReferencedInVariable", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "partyPossesses"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "partyPossesses", trueFalseList, 1))
			return false;
	}
	else if (conditionType == HERO)
	{
		int heroConditionType;
		if (!XMLTools::attributeExists(ConditionNode, "heroConditionType"))
			return false;
		tempStringList.clear();
		tempStringList << "inParty" << "nameIs" << "levelIsAtLeast" << "hpIsAtLeast" << "canUseSkill" << "hasItemEquipped" << "afflictedWithCondition";
		if (!XMLTools::attributeStringValid(ConditionNode, "heroConditionType", tempStringList))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "characterID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "characterReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "characterReferencedInVariable", trueFalseList, 1))
			return false;
		heroConditionType = tempStringList.indexOf(QString(ConditionNode.getAttribute("heroConditionType")));
		if (heroConditionType == HeroCondition::NAME_IS)
		{
			if (!XMLTools::attributeExists(ConditionNode, "string"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "stringStoredInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "stringStoredInVariable", trueFalseList, 1))
				return false;
		}
		else if (heroConditionType == HeroCondition::LEVEL_IS_AT_LEAST)
		{
			if (!XMLTools::attributeExists(ConditionNode, "level"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "levelStoredInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "levelStoredInVariable", trueFalseList, 1))
				return false;
			if (QString(ConditionNode.getAttribute("levelStoredInVariable")) != "true")
			{
				if (!XMLTools::attributeIntRangeValid(ConditionNode, "level", 1, 99))
					return false;
			}
		}
		else if (heroConditionType == HeroCondition::HP_IS_AT_LEAST)
		{
			if (!XMLTools::attributeExists(ConditionNode, "hp"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "hpStoredInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "hpStoredInVariable", trueFalseList, 1))
				return false;
			if (QString(ConditionNode.getAttribute("hpStoredInVariable")) != "true")
			{
				if (!XMLTools::attributeIntRangeValid(ConditionNode, "hp", 1, 9999))
					return false;
			}
		}
		else if (heroConditionType == HeroCondition::CAN_USE_SKILL)
		{
			if (!XMLTools::attributeExists(ConditionNode, "skillID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "skillReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "skillReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (heroConditionType == HeroCondition::HAS_ITEM_EQUIPPED)
		{
			if (!XMLTools::attributeExists(ConditionNode, "itemID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "itemReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "itemReferencedInVariable", trueFalseList, 1))
				return false;
		}
		else if (heroConditionType == HeroCondition::AFFLICTED_WITH_CONDITION)
		{
			if (!XMLTools::attributeExists(ConditionNode, "conditionID"))
				return false;
			if (!XMLTools::attributeExists(ConditionNode, "conditionReferencedInVariable"))
				return false;
			if (!XMLTools::attributeStringValid(ConditionNode, "conditionReferencedInVariable", trueFalseList, 1))
				return false;
		}
	}
	else if (conditionType == PARTY_IS_RIDING)
	{
		if (!XMLTools::attributeExists(ConditionNode, "vehicleID"))
			return false;
		if (!XMLTools::attributeExists(ConditionNode, "vehicleReferencedInVariable"))
			return false;
		if (!XMLTools::attributeStringValid(ConditionNode, "vehicleReferencedInVariable", trueFalseList, 1))
			return false;
	}
	return true;
}
//END MapEvent::Precondition Member Functions

//BEGIN MapEvent::Page Member Functions
MapEvent::Page::Page()
{
	event = new BaseEvent(BaseEvent::MAPEVENT);
	spriteLocation = "sprite:";
	spriteOpacity = 100;
	triggerCondition = TRIGGER_ACTIONKEY;
	layer = 0;
	relationToHero = RELATION_BESIDEHERO;
	animationType = ANIMTYPE_NONCONTINUOUS;
	movementFrequency = FREQUENCY_NORMAL;
	movementType = MOVETYPE_STATIONARY;
	movementSpeed = 3;
	preconditionsMet = MEET_ALL;
	numPreconditionsMet = 1;
	collidesWithEventID = 0;
	repeatMovementPattern = true;
	ignoreImpossibleMoves = true;
	forbidEventOverlap = true;
}

MapEvent::Page::Page(XMLNode PageNode)
{
	XMLNode TempNode1, TempNode2;
	QString tempString;
	collidesWithEventID = 0;
	event = new BaseEvent(BaseEvent::MAPEVENT, PageNode.getChildNode("events"));
	TempNode1 = PageNode.getChildNode("preconditions");
	tempString = TempNode1.getAttribute("meet");
	numPreconditionsMet = 1;
	if (tempString == "all")
		preconditionsMet = MEET_ALL;
	else if (tempString == "atLeast")
	{
		preconditionsMet = MEET_ATLEAST;
		numPreconditionsMet = QString(TempNode1.getAttribute("num")).toInt();
	}
	else if (tempString == "none")
		preconditionsMet = MEET_NONE;
	for (int i = 0, count = TempNode1.nChildNode("condition"); i < count; ++i)
		preconditions.append(new Precondition(TempNode1.getChildNode("condition", i)));
	spriteLocation = ProjectData::getAbsoluteResourcePath(PageNode.getAttribute("spriteLocation"));
	spriteOpacity = QString(PageNode.getAttribute("opacity")).toInt();
	tempString = PageNode.getAttribute("triggerCondition");
	if (tempString == "actionKey")
		triggerCondition = TRIGGER_ACTIONKEY;
	else if (tempString == "touchedByHero")
		triggerCondition = TRIGGER_TOUCHEDBYHERO;
	else if (tempString == "collidesWithHero")
		triggerCondition = TRIGGER_COLLIDESWITHHERO;
	else if (tempString == "collidesWithEvent")
	{
		triggerCondition = TRIGGER_COLLIDESWITHEVENT;
		collidesWithEventID = QString(PageNode.getAttribute("eventID")).toInt();
	}
	else if (tempString == "autoStart")
		triggerCondition = TRIGGER_AUTOSTART;
	else if (tempString == "parallelProcess")
		triggerCondition = TRIGGER_PARALLELPROCESS;
	layer = QString(PageNode.getAttribute("layer")).toInt();
	tempString = PageNode.getAttribute("relationToHero");
	if (tempString == "belowHero")
		relationToHero = RELATION_BELOWHERO;
	else if (tempString == "besideHero")
		relationToHero = RELATION_BESIDEHERO;
	else if (tempString == "aboveHero")
		relationToHero = RELATION_ABOVEHERO;
	tempString = PageNode.getAttribute("animationType");
	if (tempString == "nonContinuous")
		animationType = ANIMTYPE_NONCONTINUOUS;
	else if (tempString == "continuous")
		animationType = ANIMTYPE_CONTINUOUS;
	else if (tempString == "fixedNonContinuous")
		animationType = ANIMTYPE_FIXEDNONCONTINUOUS;
	else if (tempString == "fixedContinuous")
		animationType = ANIMTYPE_FIXEDCONTINUOUS;
	else if (tempString == "fixedGraphic")
		animationType = ANIMTYPE_FIXEDGRAPHIC;
	else if (tempString == "spinAround")
		animationType = ANIMTYPE_SPINAROUND;
	tempString = PageNode.getAttribute("movementFrequency");
	if (tempString == "1/8 normal")
		movementFrequency = FREQUENCY_ONEEIGHTHNORMAL;
	else if (tempString == "1/4 normal")
		movementFrequency = FREQUENCY_ONEFOURTHNORMAL;
	else if (tempString == "1/2 normal")
		movementFrequency = FREQUENCY_ONEHALFNORMAL;
	else if (tempString == "normal")
		movementFrequency = FREQUENCY_NORMAL;
	else if (tempString == "2x normal")
		movementFrequency = FREQUENCY_TWICENORMAL;
	else if (tempString == "4x normal")
		movementFrequency = FREQUENCY_FOURTIMESNORMAL;
	tempString = PageNode.getAttribute("movementType");
	if (tempString == "stationary")
		movementType = MOVETYPE_STATIONARY;
	else if (tempString == "random")
		movementType = MOVETYPE_RANDOM;
	else if (tempString == "vertical")
		movementType = MOVETYPE_VERTICAL;
	else if (tempString == "horizontal")
		movementType = MOVETYPE_HORIZONTAL;
	else if (tempString == "towardHero")
		movementType = MOVETYPE_TOWARDHERO;
	else if (tempString == "awayFromHero")
		movementType = MOVETYPE_AWAYFROMHERO;
	else if (tempString == "customPattern")
		movementType = MOVETYPE_CUSTOMPATTERN;
	movementSpeed = QString(PageNode.getAttribute("movementSpeed")).toInt();
	tempString = TempNode1.getAttribute("forbidEventOverlap");
	forbidEventOverlap = (tempString == "true") ? true:false;
	if (movementType == MOVETYPE_CUSTOMPATTERN)
	{
		TempNode1 = PageNode.getChildNode("customMovementPattern");
		tempString = TempNode1.getAttribute("repeatPattern");
		repeatMovementPattern = (tempString == "true") ? true:false;
		tempString = TempNode1.getAttribute("ignoreImpossibleMoves");
		ignoreImpossibleMoves = (tempString == "true") ? true:false;
		for (int i = 0, count = TempNode1.nChildNode("pattern"); i < count; ++i)
			customMovementPattern.append(new MovementPattern(TempNode1.getChildNode("pattern", i)));
	}
}

MapEvent::Page::Page(StorageFile &storageFile)
{
	char *temp_string;
	event = new BaseEvent(BaseEvent::MAPEVENT, storageFile);
	preconditionsMet = storageFile.getSignedInt();
	numPreconditionsMet = storageFile.getSignedInt();
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
		preconditions.append(new Precondition(storageFile));
	temp_string = storageFile.getString();
	spriteLocation = temp_string;
	delete[] temp_string;
	spriteOpacity = storageFile.getSignedInt();
	triggerCondition = storageFile.getSignedInt();
	layer = storageFile.getSignedInt();
	relationToHero = storageFile.getSignedInt();
	animationType = storageFile.getSignedInt();
	movementFrequency = storageFile.getSignedInt();
	movementType = storageFile.getSignedInt();
	movementSpeed = storageFile.getSignedInt();
	collidesWithEventID = storageFile.getSignedInt();
	forbidEventOverlap = storageFile.getBool();
	if (movementType == MOVETYPE_CUSTOMPATTERN)
	{
		repeatMovementPattern = storageFile.getBool();
		ignoreImpossibleMoves = storageFile.getBool();
		for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
			customMovementPattern.append(new MovementPattern(storageFile));
	}
}

MapEvent::Page::Page(Page *other)
{
	event = new BaseEvent(other->event);
	for (int i = 0; i < other->preconditions.size(); ++i)
		preconditions.append(new Precondition(other->preconditions[i]));
	for (int i = 0; i < other->customMovementPattern.size(); ++i)
		customMovementPattern.append(new MovementPattern(other->customMovementPattern[i]));
	spriteLocation = "";
	spriteLocation.append(other->spriteLocation);
	spriteOpacity = other->spriteOpacity;
	triggerCondition = other->triggerCondition;
	layer = other->layer;
	relationToHero = other->relationToHero;
	animationType = other->animationType;
	movementFrequency = other->movementFrequency;
	movementType = other->movementType;
	movementSpeed = other->movementSpeed;
	preconditionsMet = other->preconditionsMet;
	numPreconditionsMet = other->numPreconditionsMet;
	collidesWithEventID = other->collidesWithEventID;
	repeatMovementPattern = other->repeatMovementPattern;
	ignoreImpossibleMoves = other->ignoreImpossibleMoves;
	forbidEventOverlap = other->forbidEventOverlap;
}

MapEvent::Page::~Page()
{
	for (int i = 0; i < preconditions.size(); ++i)
		delete (preconditions[i]);
	for (int i = 0; i < customMovementPattern.size(); ++i)
		delete (customMovementPattern[i]);
	preconditions.clear();
	customMovementPattern.clear();
	delete (event);
	event = NULL;
}

XMLNode MapEvent::Page::getPageNode()
{
	XMLNode TempNode1, TempNode2, PageNode = XMLNode::createXMLTopNode("page");
	QString tempString;
	TempNode1 = PageNode.addChild("preconditions");
	if (preconditionsMet == MEET_ALL)
		TempNode1.addAttribute("meet", "all");
	else if (preconditionsMet == MEET_ATLEAST)
	{
		TempNode1.addAttribute("meet", "atLeast");
		TempNode1.addAttribute("num", QString::number(numPreconditionsMet).toUtf8().data());
	}
	else if (preconditionsMet == MEET_NONE)
		TempNode1.addAttribute("meet", "none");
	for (int i = 0; i < preconditions.size(); ++i)
		TempNode1.addChild(preconditions[i]->getConditionNode());
	PageNode.addAttribute("spriteLocation", ProjectData::getRelativeResourcePath(spriteLocation).toUtf8().data());
	PageNode.addAttribute("opacity", QString::number(spriteOpacity).toUtf8().data());
	if (triggerCondition == TRIGGER_ACTIONKEY)
		PageNode.addAttribute("triggerCondition", "actionKey");
	else if (triggerCondition == TRIGGER_TOUCHEDBYHERO)
		PageNode.addAttribute("triggerCondition", "touchedByHero");
	else if (triggerCondition == TRIGGER_COLLIDESWITHHERO)
		PageNode.addAttribute("triggerCondition", "collidesWithHero");
	else if (triggerCondition == TRIGGER_COLLIDESWITHEVENT)
	{
		PageNode.addAttribute("triggerCondition", "collidesWithEvent");
		PageNode.addAttribute("eventID", QString::number(collidesWithEventID).toUtf8().data());
	}
	else if (triggerCondition == TRIGGER_AUTOSTART)
		PageNode.addAttribute("triggerCondition", "autoStart");
	else if (triggerCondition == TRIGGER_PARALLELPROCESS)
		PageNode.addAttribute("triggerCondition", "parallelProcess");
	PageNode.addAttribute("layer", QString::number(layer).toUtf8().data());
	if (relationToHero == RELATION_BELOWHERO)
		PageNode.addAttribute("relationToHero", "belowHero");
	else if (relationToHero == RELATION_BESIDEHERO)
		PageNode.addAttribute("relationToHero", "besideHero");
	else if (relationToHero == RELATION_ABOVEHERO)
		PageNode.addAttribute("relationToHero", "aboveHero");
	if (animationType == ANIMTYPE_NONCONTINUOUS)
		PageNode.addAttribute("animationType", "nonContinuous");
	else if (animationType == ANIMTYPE_CONTINUOUS)
		PageNode.addAttribute("animationType", "continuous");
	else if (animationType == ANIMTYPE_FIXEDNONCONTINUOUS)
		PageNode.addAttribute("animationType", "fixedNonContinuous");
	else if (animationType == ANIMTYPE_FIXEDCONTINUOUS)
		PageNode.addAttribute("animationType", "fixedContinuous");
	else if (animationType == ANIMTYPE_FIXEDGRAPHIC)
		PageNode.addAttribute("animationType", "fixedGraphic");
	else if (animationType == ANIMTYPE_SPINAROUND)
		PageNode.addAttribute("animationType", "spinAround");
	if (movementFrequency == FREQUENCY_ONEEIGHTHNORMAL)
		PageNode.addAttribute("movementFrequency", "1/8 normal");
	else if (movementFrequency == FREQUENCY_ONEFOURTHNORMAL)
		PageNode.addAttribute("movementFrequency", "1/4 normal");
	else if (movementFrequency == FREQUENCY_ONEHALFNORMAL)
		PageNode.addAttribute("movementFrequency", "1/2 normal");
	else if (movementFrequency == FREQUENCY_NORMAL)
		PageNode.addAttribute("movementFrequency", "normal");
	else if (movementFrequency == FREQUENCY_TWICENORMAL)
		PageNode.addAttribute("movementFrequency", "2x normal");
	else if (movementFrequency == FREQUENCY_FOURTIMESNORMAL)
		PageNode.addAttribute("movementFrequency", "4x normal");
	if (movementType == MOVETYPE_STATIONARY)
		PageNode.addAttribute("movementType", "stationary");
	else if (movementType == MOVETYPE_RANDOM)
		PageNode.addAttribute("movementType", "random");
	else if (movementType == MOVETYPE_VERTICAL)
		PageNode.addAttribute("movementType", "vertical");
	else if (movementType == MOVETYPE_HORIZONTAL)
		PageNode.addAttribute("movementType", "horizontal");
	else if (movementType == MOVETYPE_TOWARDHERO)
		PageNode.addAttribute("movementType", "towardHero");
	else if (movementType == MOVETYPE_AWAYFROMHERO)
		PageNode.addAttribute("movementType", "awayFromHero");
	else if (movementType == MOVETYPE_CUSTOMPATTERN)
		PageNode.addAttribute("movementType", "customPattern");
	PageNode.addAttribute("movementSpeed", QString::number(movementSpeed).toUtf8().data());
	PageNode.addAttribute("forbidEventOverlap", (forbidEventOverlap) ? "true":"false");
	if (movementType == MOVETYPE_CUSTOMPATTERN)
	{
		TempNode1 = PageNode.addChild("customMovementPattern");
		TempNode1.addAttribute("repeatPattern", (repeatMovementPattern) ? "true":"false");
		TempNode1.addAttribute("ignoreImpossibleMoves", (ignoreImpossibleMoves) ? "true":"false");
		for (int i = 0; i < customMovementPattern.size(); ++i)
			TempNode1.addChild(customMovementPattern[i]->getPatternNode());
	}
	TempNode1 = event->getEventNode();
	TempNode1.updateName("events");
	PageNode.addChild(TempNode1);
	return PageNode;
}

void MapEvent::Page::saveToStorageFile(StorageFile &storageFile)
{
	event->saveToStorageFile(storageFile);
	storageFile.putSignedInt(preconditionsMet);
	storageFile.putSignedInt(numPreconditionsMet);
	storageFile.putSignedInt(preconditions.size());
	for (int i = 0; i < preconditions.size(); ++i)
		preconditions[i]->saveToStorageFile(storageFile);
	storageFile.putString(spriteLocation.toUtf8().data());
	storageFile.putSignedInt(spriteOpacity);
	storageFile.putSignedInt(triggerCondition);
	storageFile.putSignedInt(layer);
	storageFile.putSignedInt(relationToHero);
	storageFile.putSignedInt(animationType);
	storageFile.putSignedInt(movementFrequency);
	storageFile.putSignedInt(movementType);
	storageFile.putSignedInt(movementSpeed);
	storageFile.putSignedInt(collidesWithEventID);
	storageFile.putBool(forbidEventOverlap);
	if (movementType == MOVETYPE_CUSTOMPATTERN)
	{
		storageFile.putBool(repeatMovementPattern);
		storageFile.putBool(ignoreImpossibleMoves);
		storageFile.putSignedInt(customMovementPattern.size());
		for (int i = 0; i < customMovementPattern.size(); ++i)
			customMovementPattern[i]->saveToStorageFile(storageFile);
	}
}

QImage MapEvent::Page::getImage()
{
	QImage image;
	if (spriteLocation.startsWith("sprite:"))
			image = QImage(spriteLocation.mid(7));
	else if (spriteLocation.startsWith("tile:"))
	{
		ProjectData::DataReference<Tileset> *tilesetRef;
		int tilesetID = spriteLocation.mid(5, 2).toInt(NULL, 16);
		int tileID = spriteLocation.mid(7, 2).toInt(NULL, 16);
		if (tilesetID >= ProjectData::tilesetNames.size())
			image = QImage(":/icons/invalidtileset.png");
		else
		{
			tilesetRef = new ProjectData::DataReference<Tileset>(tilesetID, __FILE__, __LINE__);
			if (tileID >= (*tilesetRef)->numTiles())
				image = QImage(":/icons/invalidtile.png");
			else
				image = (*tilesetRef)->getTile(tileID)->getTile();
			delete tilesetRef;
		}
	}
	if (image.isNull())
	{
		image = QImage(32, 32, QImage::Format_ARGB32);
		image.fill(qRgba(0, 0, 0, 0));
	}
	return image;
}

QImage MapEvent::Page::getImage(int direction, int pose)
{
	QImage image;
	if (spriteLocation.startsWith("sprite:"))
		image = QImage(spriteLocation.mid(7)).copy(pose, direction, 48, 64);
	else if (spriteLocation.startsWith("tile:"))
	{
		ProjectData::DataReference<Tileset> *tilesetRef;
		int tilesetID = spriteLocation.mid(5, 2).toInt(NULL, 16);
		int tileID = spriteLocation.mid(7, 2).toInt(NULL, 16);
		if (tilesetID >= ProjectData::tilesetNames.size())
			image = QImage(":/icons/invalidtileset.png");
		else
		{
			tilesetRef = new ProjectData::DataReference<Tileset>(tilesetID, __FILE__, __LINE__);
			if (tileID >= (*tilesetRef)->numTiles())
				image = QImage(":/icons/invalidtile.png");
			else
				image = (*tilesetRef)->getTile(tileID)->getTile();
			delete tilesetRef;
		}
	}
	if (image.isNull())
	{
		image = QImage(32, 32, QImage::Format_ARGB32);
		image.fill(qRgba(0, 0, 0, 0));
	}
	return image;
}

void MapEvent::Page::applySwappedTiles(QList<Tileset::SwapTileIDs> swappedTiles)
{
	if (spriteLocation.startsWith("tile:"))
	{
		int tilesetID = spriteLocation.mid(5, 2).toInt(NULL, 16);
		int tileID = spriteLocation.mid(7, 2).toInt(NULL, 16);
		for (int i = 0; i < swappedTiles.size(); ++i)
		{
			if (tilesetID == swappedTiles[i].tileset && tileID)
			{
				if (tileID == swappedTiles[i].tile1)
					tileID = swappedTiles[i].tile2;
				else if (tileID == swappedTiles[i].tile2)
					tileID = swappedTiles[i].tile1;
			}
		}
		spriteLocation = QString("tile:%1%2").arg(tilesetID, 2, 16, QChar('0')).arg(tileID, 2, 16, QChar('0'));
	}
	event->applySwappedTiles(swappedTiles);
}

bool MapEvent::Page::tryLoad(XMLNode PageNode)
{
	XMLNode TempNode1, TempNode2;
	QStringList tempStringList;
	QStringList trueFalseList = QStringList() << "true" << "false";
	if (!XMLTools::nodeExists(PageNode, "events"))
		return false;
	if (!BaseEvent::tryLoad(PageNode.getChildNode("events"), BaseEvent::MAPEVENT))
		return false;
	if (!XMLTools::nodeExists(PageNode, "preconditions"))
		return false;
	TempNode1 = PageNode.getChildNode("preconditions");
	if (!XMLTools::attributeExists(TempNode1, "meet"))
		return false;
	tempStringList.clear();
	tempStringList << "all" << "atLeast" << "none";
	if (!XMLTools::attributeStringValid(TempNode1, "meet", tempStringList))
		return false;
	if (QString(TempNode1.getAttribute("meet")) == "atLeast")
	{
		if (!XMLTools::attributeExists(TempNode1, "num"))
			return false;
	}
	for (int i = 0, count = TempNode1.nChildNode("precondition"); i < count; ++i)
	{
		if (!Precondition::tryLoad(TempNode1.getChildNode("precondition", i)))
			return false;
	}
	if (!XMLTools::attributeExists(PageNode, "spriteLocation"))
		return false;
	if (!XMLTools::attributeExists(PageNode, "opacity"))
		return false;
	if (!XMLTools::attributeIntRangeValid(PageNode, "opacity", 0, 100))
		return false;
	tempStringList.clear();
	tempStringList << "actionKey" << "touchedByHero" << "collidesWithHero" << "collidesWithEvent" << "autoStart" << "parallelProcess";
	if (!XMLTools::attributeExists(PageNode, "triggerCondition"))
		return false;
	if (!XMLTools::attributeStringValid(PageNode, "triggerCondition", tempStringList))
		return false;
	if (QString(PageNode.getAttribute("triggerCondition")) == "collidesWithEvent")
	{
		if (!XMLTools::attributeExists(PageNode, "eventID"))
			return false;
	}
	if (!XMLTools::attributeExists(PageNode, "layer"))
		return false;
	tempStringList.clear();
	tempStringList << "belowHero" << "besideHero" << "aboveHero";
	if (!XMLTools::attributeExists(PageNode, "relationToHero"))
		return false;
	if (!XMLTools::attributeStringValid(PageNode, "relationToHero", tempStringList))
		return false;
	tempStringList.clear();
	tempStringList << "nonContinuous" << "continuous" << "fixedNonContinuous" << "fixedContinuous" << "fixedGraphic" << "spinAround";
	if (!XMLTools::attributeExists(PageNode, "animationType"))
		return false;
	if (!XMLTools::attributeStringValid(PageNode, "animationType", tempStringList))
		return false;
	tempStringList.clear();
	tempStringList << "1/8 normal" << "1/4 normal" << "1/2 normal" << "normal" << "2x normal" << "4x normal";
	if (!XMLTools::attributeExists(PageNode, "movementFrequency"))
		return false;
	if (!XMLTools::attributeStringValid(PageNode, "movementFrequency", tempStringList))
		return false;
	tempStringList.clear();
	tempStringList << "stationary" << "random" << "vertical" << "horizontal" << "towardHero" << "awayFromHero" << "customPattern";
	if (!XMLTools::attributeExists(PageNode, "movementType"))
		return false;
	if (!XMLTools::attributeStringValid(PageNode, "movementType", tempStringList))
		return false;
	if (!XMLTools::attributeExists(PageNode, "movementSpeed"))
		return false;
	if (!XMLTools::attributeIntRangeValid(PageNode, "movementSpeed", 1, 7))
		return false;
	if (!XMLTools::attributeExists(PageNode, "forbidEventOverlap"))
		return false;
	if (!XMLTools::attributeStringValid(PageNode, "forbidEventOverlap", trueFalseList, 1))
		return false;
	if (QString(PageNode.getAttribute("movementType")) == "customPattern")
	{
		if (!XMLTools::nodeExists(PageNode, "customMovementPattern"))
			return false;
		TempNode1 = PageNode.getChildNode("customMovementPattern");
		if (!XMLTools::attributeExists(TempNode1, "repeatPattern"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "repeatPattern", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "ignoreImpossibleMoves"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "ignoreImpossibleMoves", trueFalseList, 1))
			return false;
		for (int i = 0, count = TempNode1.nChildNode("pattern"); i < count; ++i)
		{
			if (!MovementPattern::tryLoad(TempNode1.getChildNode("pattern", i)))
				return false;
		}
	}
	return true;
}
//END MapEvent::Page Member Functions
