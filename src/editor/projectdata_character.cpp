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
#include "projectdata.h"
#include "projectdata_battleevent.h"
#include "projectdata_character.h"
#include "xmlParser.h"
#include "xmlTools.h"

Character::Character()
{
	name = "Untitled";
	faceGraphicLocation = "";
	for (int i = 0; i < ProjectData::conditionNames.size(); ++i)
		conditionResists.append(RESISTLEVEL_C);
	for (int i = 0; i < ProjectData::attributeNames.size(); ++i)
		attributeResists.append(RESISTLEVEL_C);
	battleEvent = new BattleEvent();
	customAI = new BattleEvent();
	criticalHitRate = 3.0;
	criticalHitMultiplier = 2.0;
	for (int i = 0; i < 99; ++i)
	{
		maxHP[i] = 1;
		maxMP[i] = 1;
		attack[i] = 1;
		defense[i] = 1;
		wisdom[i] = 1;
		agility[i] = 1;
	}
	for (int i = 0; i < 6; ++i)
		battleCommands[i] = -1;
	minLevel = 1;
	maxLevel = 99;
	classID = -1;
	spriteID = 0;
	experiencePrimary = 1;
	experienceSecondary = 0;
	weaponID = -1;
	shieldID = -1;
	armorID = -1;
	helmetID = -1;
	accessoryID = -1;
	unarmedBattleAnimationID = 0;
	aiLevel = AILEVEL_BASIC;
	spriteOpacity = 100;
	duelWielding = false;
	aiControlled = false;
	equipmentLocked = false;
	mightyGuard = false;
	criticalHits = true;
}

Character::Character(Character &other)
{
	name = other.name;
	faceGraphicLocation = other.faceGraphicLocation;
	for (int i = 0; i < other.conditionResists.size(); ++i)
		conditionResists.append(other.conditionResists[i]);
	for (int i = 0; i < other.attributeResists.size(); ++i)
		attributeResists.append(other.attributeResists[i]);
	battleEvent = new BattleEvent(*(other.battleEvent));
	customAI = new BattleEvent(*(other.customAI));
	criticalHitRate = other.criticalHitRate;
	criticalHitMultiplier = other.criticalHitMultiplier;
	for (int i = 0; i < 99; ++i)
	{
		maxHP[i] = other.maxHP[i];
		maxMP[i] = other.maxMP[i];
		attack[i] = other.attack[i];
		defense[i] = other.defense[i];
		wisdom[i] = other.wisdom[i];
		agility[i] = other.agility[i];
	}
	for (int i = 0; i < 6; ++i)
		battleCommands[i] = other.battleCommands[i];
	minLevel = other.minLevel;
	maxLevel = other.maxLevel;
	classID = other.classID;
	spriteID = other.spriteID;
	experiencePrimary = other.experiencePrimary;
	experienceSecondary = other.experienceSecondary;
	weaponID = other.weaponID;
	shieldID = other.shieldID;
	armorID = other.armorID;
	helmetID = other.helmetID;
	accessoryID = other.accessoryID;
	unarmedBattleAnimationID = other.unarmedBattleAnimationID;
	aiLevel = other.aiLevel;
	spriteOpacity = other.spriteOpacity;
	duelWielding = other.duelWielding;
	aiControlled = other.aiControlled;
	equipmentLocked = other.equipmentLocked;
	mightyGuard = other.mightyGuard;
	criticalHits = other.criticalHits;
}

Character::Character(const Character &other)
{
	name = other.name;
	faceGraphicLocation = other.faceGraphicLocation;
	for (int i = 0; i < other.conditionResists.size(); ++i)
		conditionResists.append(other.conditionResists[i]);
	for (int i = 0; i < other.attributeResists.size(); ++i)
		attributeResists.append(other.attributeResists[i]);
	battleEvent = new BattleEvent(*(other.battleEvent));
	customAI = new BattleEvent(*(other.customAI));
	criticalHitRate = other.criticalHitRate;
	criticalHitMultiplier = other.criticalHitMultiplier;
	for (int i = 0; i < 99; ++i)
	{
		maxHP[i] = other.maxHP[i];
		maxMP[i] = other.maxMP[i];
		attack[i] = other.attack[i];
		defense[i] = other.defense[i];
		wisdom[i] = other.wisdom[i];
		agility[i] = other.agility[i];
	}
	for (int i = 0; i < 6; ++i)
		battleCommands[i] = other.battleCommands[i];
	minLevel = other.minLevel;
	maxLevel = other.maxLevel;
	classID = other.classID;
	spriteID = other.spriteID;
	experiencePrimary = other.experiencePrimary;
	experienceSecondary = other.experienceSecondary;
	weaponID = other.weaponID;
	shieldID = other.shieldID;
	armorID = other.armorID;
	helmetID = other.helmetID;
	accessoryID = other.accessoryID;
	unarmedBattleAnimationID = other.unarmedBattleAnimationID;
	aiLevel = other.aiLevel;
	spriteOpacity = other.spriteOpacity;
	duelWielding = other.duelWielding;
	aiControlled = other.aiControlled;
	equipmentLocked = other.equipmentLocked;
	mightyGuard = other.mightyGuard;
	criticalHits = other.criticalHits;
}

Character::Character(XMLNode CharacterNode)
{
	XMLNode TempNode;
	QString temp_string;
	QStringList tempStringList;
	name = CharacterNode.getAttribute("name");
	minLevel = QString(CharacterNode.getAttribute("minLevel")).toInt();
	maxLevel = QString(CharacterNode.getAttribute("maxLevel")).toInt();
	classID = QString(CharacterNode.getAttribute("classID")).toInt();
	unarmedBattleAnimationID = QString(CharacterNode.getAttribute("unarmedBattleAnimation")).toInt();
	aiLevel = QString(CharacterNode.getAttribute("aiLevel")).toInt();
	TempNode = CharacterNode.getChildNode("options");
	temp_string = TempNode.getAttribute("duelWielding");
	duelWielding = (temp_string == "true") ? true:false;
	temp_string = TempNode.getAttribute("aiControlled");
	aiControlled = (temp_string == "true") ? true:false;
	temp_string = TempNode.getAttribute("equipmentLocked");
	equipmentLocked = (temp_string == "true") ? true:false;
	temp_string = TempNode.getAttribute("mightyGuard");
	mightyGuard = (temp_string == "true") ? true:false;
	TempNode = CharacterNode.getChildNode("criticalHits");
	temp_string = TempNode.getAttribute("hasCriticalHits");
	criticalHits = (temp_string == "true") ? true:false;
	criticalHitRate = QString(TempNode.getAttribute("rate")).toFloat();
	criticalHitMultiplier = QString(TempNode.getAttribute("multiplier")).toFloat();
	TempNode = CharacterNode.getChildNode("graphics");
	faceGraphicLocation = ProjectData::getAbsoluteResourcePath(TempNode.getAttribute("faceGraphic"));
	spriteOpacity = QString(TempNode.getAttribute("spriteOpacity")).toInt();
	spriteID = QString(TempNode.getAttribute("sprite")).toInt();
	TempNode = CharacterNode.getChildNode("baseStats");
	tempStringList = QString(TempNode.getChildNode("maxHP").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	for (int i = 0; i < 99; ++i)
	{
		if (i >= tempStringList.size())
			maxHP[i] = 1;
		else
			maxHP[i] = tempStringList[i].toInt();
	}
	tempStringList = QString(TempNode.getChildNode("maxMP").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	for (int i = 0; i < 99; ++i)
	{
		if (i >= tempStringList.size())
			maxMP[i] = 1;
		else
			maxMP[i] = tempStringList[i].toInt();
	}
	tempStringList = QString(TempNode.getChildNode("attack").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	for (int i = 0; i < 99; ++i)
	{
		if (i >= tempStringList.size())
			attack[i] = 1;
		else
			attack[i] = tempStringList[i].toInt();
	}
	tempStringList = QString(TempNode.getChildNode("defense").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	for (int i = 0; i < 99; ++i)
	{
		if (i >= tempStringList.size())
			defense[i] = 1;
		else
			defense[i] = tempStringList[i].toInt();
	}
	tempStringList = QString(TempNode.getChildNode("wisdom").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	for (int i = 0; i < 99; ++i)
	{
		if (i >= tempStringList.size())
			wisdom[i] = 1;
		else
			wisdom[i] = tempStringList[i].toInt();
	}
	tempStringList = QString(TempNode.getChildNode("agility").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	for (int i = 0; i < 99; ++i)
	{
		if (i >= tempStringList.size())
			agility[i] = 1;
		else
			agility[i] = tempStringList[i].toInt();
	}
	TempNode = CharacterNode.getChildNode("experienceCurve");
	experiencePrimary = QString(TempNode.getAttribute("primary")).toInt();
	experienceSecondary = QString(TempNode.getAttribute("secondary")).toInt();
	TempNode = CharacterNode.getChildNode("battleCommands");
	for (int i = 0; i < 6; ++i)
		battleCommands[i] = QString(TempNode.getAttribute(QString("command%1").arg(i + 1).toUtf8().data())).toInt();
	TempNode = CharacterNode.getChildNode("startingEquipment");
	weaponID = QString(TempNode.getAttribute("weapon")).toInt();
	shieldID = QString(TempNode.getAttribute("shield")).toInt();
	armorID = QString(TempNode.getAttribute("armor")).toInt();
	helmetID = QString(TempNode.getAttribute("helmet")).toInt();
	accessoryID = QString(TempNode.getAttribute("accessory")).toInt();
	TempNode = CharacterNode.getChildNode("skillProgression");
	for (int i = 0; i < TempNode.nChildNode("skillLearned"); ++i)
	{
		XMLNode TempNode2 = TempNode.getChildNode("skillLearned", i);
		SkillLearned skillLearned;
		skillLearned.levelLearned = QString(TempNode2.getAttribute("level")).toInt();
		skillLearned.skillID = QString(TempNode2.getAttribute("skill")).toInt();
		skillProgression.append(skillLearned);
	}
	temp_string = CharacterNode.getChildNode("conditionResists").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		QChar value = temp_string[i].toUpper();
		if (value == QChar('A'))
			conditionResists.append(RESISTLEVEL_A);
		else if (value == QChar('B'))
			conditionResists.append(RESISTLEVEL_B);
		else if (value == QChar('C'))
			conditionResists.append(RESISTLEVEL_C);
		else if (value == QChar('D'))
			conditionResists.append(RESISTLEVEL_D);
		else
			conditionResists.append(RESISTLEVEL_E);
	}
	temp_string = CharacterNode.getChildNode("attributeResists").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		QChar value = temp_string[i].toUpper();
		if (value == QChar('A'))
			attributeResists.append(RESISTLEVEL_A);
		else if (value == QChar('B'))
			attributeResists.append(RESISTLEVEL_B);
		else if (value == QChar('C'))
			attributeResists.append(RESISTLEVEL_C);
		else if (value == QChar('D'))
			attributeResists.append(RESISTLEVEL_D);
		else
			attributeResists.append(RESISTLEVEL_E);
	}
	battleEvent = new BattleEvent(CharacterNode.getChildNode("battleEvent"));
	customAI = new BattleEvent(CharacterNode.getChildNode("customAI"));
}

Character::Character(StorageFile &storageFile)
{
	int temp_int;
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	faceGraphicLocation = temp_string;
	delete[] temp_string;
	temp_int = storageFile.getSignedInt();
	for (int i = 0; i < temp_int; ++i)
	{
		SkillLearned skillLearned;
		skillLearned.levelLearned = storageFile.getSignedInt();
		skillLearned.skillID = storageFile.getSignedInt();
		skillProgression.append(skillLearned);
	}
	temp_int = storageFile.getSignedInt();
	for (int i = 0; i < temp_int; ++i)
		conditionResists.append(storageFile.getSignedInt());
	temp_int = storageFile.getSignedInt();
	for (int i = 0; i < temp_int; ++i)
		attributeResists.append(storageFile.getSignedInt());
	battleEvent = new BattleEvent();
	customAI = new BattleEvent();
	TODO("Uncomment the following lines when the battle events are ready");
	//battleEvent = new BattleEvent(storageFile);
	//customAI = new BattleEvent(storageFile);
	criticalHitRate = storageFile.getFloat();
	criticalHitMultiplier = storageFile.getFloat();
	for (int i = 0; i < 99; ++i)
	{
		maxHP[i] = storageFile.getSignedInt();
		maxMP[i] = storageFile.getSignedInt();
		attack[i] = storageFile.getSignedInt();
		defense[i] = storageFile.getSignedInt();
		wisdom[i] = storageFile.getSignedInt();
		agility[i] = storageFile.getSignedInt();
	}
	for (int i = 0; i < 6; ++i)
		battleCommands[i] = storageFile.getSignedInt();
	minLevel = storageFile.getSignedInt();
	maxLevel = storageFile.getSignedInt();
	classID = storageFile.getSignedInt();
	spriteID = storageFile.getSignedInt();
	experiencePrimary = storageFile.getSignedInt();
	experienceSecondary = storageFile.getSignedInt();
	weaponID = storageFile.getSignedInt();
	shieldID = storageFile.getSignedInt();
	armorID = storageFile.getSignedInt();
	helmetID = storageFile.getSignedInt();
	accessoryID = storageFile.getSignedInt();
	unarmedBattleAnimationID = storageFile.getSignedInt();
	aiLevel = storageFile.getSignedInt();
	spriteOpacity = storageFile.getSignedInt();
	duelWielding = storageFile.getBool();
	aiControlled = storageFile.getBool();
	equipmentLocked = storageFile.getBool();
	mightyGuard = storageFile.getBool();
	criticalHits = storageFile.getBool();
}

Character::~Character()
{
	skillProgression.clear();
	conditionResists.clear();
	attributeResists.clear();
	if (battleEvent != NULL)
	{
		delete battleEvent;
		battleEvent = NULL;
	}
	if (customAI != NULL)
	{
		delete customAI;
		customAI = NULL;
	}
}

void Character::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putString(faceGraphicLocation.toUtf8().data());
	storageFile.putSignedInt(skillProgression.size());
	for (int i = 0; i < skillProgression.size(); ++i)
	{
		storageFile.putSignedInt(skillProgression[i].levelLearned);
		storageFile.putSignedInt(skillProgression[i].skillID);
	}
	storageFile.putSignedInt(conditionResists.size());
	for (int i = 0; i < conditionResists.size(); ++i)
		storageFile.putSignedInt(conditionResists[i]);
	storageFile.putSignedInt(attributeResists.size());
	for (int i = 0; i < attributeResists.size(); ++i)
		storageFile.putSignedInt(attributeResists[i]);
	TODO("Uncomment the following lines when the battle events are ready");
	//battleEvent->saveToStorageFile(storageFile);
	//customAI->saveToStorageFile(storageFile);
	storageFile.putFloat(criticalHitRate);
	storageFile.putFloat(criticalHitMultiplier);
	for (int i = 0; i < 99; ++i)
	{
		storageFile.putSignedInt(maxHP[i]);
		storageFile.putSignedInt(maxMP[i]);
		storageFile.putSignedInt(attack[i]);
		storageFile.putSignedInt(defense[i]);
		storageFile.putSignedInt(wisdom[i]);
		storageFile.putSignedInt(agility[i]);
	}
	for (int i = 0; i < 6; ++i)
		storageFile.putSignedInt(battleCommands[i]);
	storageFile.putSignedInt(minLevel);
	storageFile.putSignedInt(maxLevel);
	storageFile.putSignedInt(classID);
	storageFile.putSignedInt(spriteID);
	storageFile.putSignedInt(experiencePrimary);
	storageFile.putSignedInt(experienceSecondary);
	storageFile.putSignedInt(weaponID);
	storageFile.putSignedInt(shieldID);
	storageFile.putSignedInt(armorID);
	storageFile.putSignedInt(helmetID);
	storageFile.putSignedInt(accessoryID);
	storageFile.putSignedInt(unarmedBattleAnimationID);
	storageFile.putSignedInt(aiLevel);
	storageFile.putSignedInt(spriteOpacity);
	storageFile.putBool(duelWielding);
	storageFile.putBool(aiControlled);
	storageFile.putBool(equipmentLocked);
	storageFile.putBool(mightyGuard);
	storageFile.putBool(criticalHits);
}

QStringList Character::getResources()
{
	QStringList resources;
	resources.append(faceGraphicLocation);
	TODO("Add battle event's resources");
	TODO("Add custom AI's resources");
	return resources;
}

XMLNode Character::getCharacterNode()
{
	XMLNode TempNode, TempNode2, CharacterNode = XMLNode::createXMLTopNode("character");
	QStringList tempStringList;
	CharacterNode.addAttribute("name", name.toUtf8().data());
	CharacterNode.addAttribute("minLevel", QString::number(minLevel).toUtf8().data());
	CharacterNode.addAttribute("maxLevel", QString::number(maxLevel).toUtf8().data());
	CharacterNode.addAttribute("classID", QString::number(classID).toUtf8().data());
	CharacterNode.addAttribute("unarmedBattleAnimation", QString::number(unarmedBattleAnimationID).toUtf8().data());
	CharacterNode.addAttribute("aiLevel", QString::number(aiLevel).toUtf8().data());
	TempNode = CharacterNode.addChild("options");
	TempNode.addAttribute("duelWielding", (duelWielding) ? "true":"false");
	TempNode.addAttribute("aiControlled", (aiControlled) ? "true":"false");
	TempNode.addAttribute("equipmentLocked", (equipmentLocked) ? "true":"false");
	TempNode.addAttribute("mightyGuard", (mightyGuard) ? "true":"false");
	TempNode = CharacterNode.addChild("criticalHits");
	TempNode.addAttribute("hasCriticalHits", (criticalHits) ? "true":"false");
	TempNode.addAttribute("rate", QString::number(criticalHitRate).toUtf8().data());
	TempNode.addAttribute("multiplier", QString::number(criticalHitMultiplier).toUtf8().data());
	TempNode = CharacterNode.addChild("graphics");
	TempNode.addAttribute("faceGraphic", ProjectData::getRelativeResourcePath(faceGraphicLocation).toUtf8().data());
	TempNode.addAttribute("spriteOpacity", QString::number(spriteOpacity).toUtf8().data());
	TempNode.addAttribute("sprite", QString::number(spriteID).toUtf8().data());
	TempNode = CharacterNode.addChild("baseStats");
	TempNode2 = TempNode.addChild("maxHP");
	for (int i = 0; i < 99; ++i)
		tempStringList.append(QString::number(maxHP[i]));
	TempNode2.addText(tempStringList.join(" ").toUtf8().data());
	tempStringList.clear();
	TempNode2 = TempNode.addChild("maxMP");
	for (int i = 0; i < 99; ++i)
		tempStringList.append(QString::number(maxMP[i]));
	TempNode2.addText(tempStringList.join(" ").toUtf8().data());
	tempStringList.clear();
	TempNode2 = TempNode.addChild("attack");
	for (int i = 0; i < 99; ++i)
		tempStringList.append(QString::number(attack[i]));
	TempNode2.addText(tempStringList.join(" ").toUtf8().data());
	tempStringList.clear();
	TempNode2 = TempNode.addChild("defense");
	for (int i = 0; i < 99; ++i)
		tempStringList.append(QString::number(defense[i]));
	TempNode2.addText(tempStringList.join(" ").toUtf8().data());
	tempStringList.clear();
	TempNode2 = TempNode.addChild("wisdom");
	for (int i = 0; i < 99; ++i)
		tempStringList.append(QString::number(wisdom[i]));
	TempNode2.addText(tempStringList.join(" ").toUtf8().data());
	tempStringList.clear();
	TempNode2 = TempNode.addChild("agility");
	for (int i = 0; i < 99; ++i)
		tempStringList.append(QString::number(agility[i]));
	TempNode2.addText(tempStringList.join(" ").toUtf8().data());
	tempStringList.clear();
	TempNode = CharacterNode.addChild("experienceCurve");
	TempNode.addAttribute("primary", QString::number(experiencePrimary).toUtf8().data());
	TempNode.addAttribute("secondary", QString::number(experienceSecondary).toUtf8().data());
	TempNode = CharacterNode.addChild("battleCommands");
	for (int i = 0; i < 6; ++i)
		TempNode.addAttribute(QString("command%1").arg(i + 1).toUtf8().data(), QString::number(battleCommands[i]).toUtf8().data());
	TempNode = CharacterNode.addChild("startingEquipment");
	TempNode.addAttribute("weapon", QString::number(weaponID).toUtf8().data());
	TempNode.addAttribute("shield", QString::number(shieldID).toUtf8().data());
	TempNode.addAttribute("armor", QString::number(armorID).toUtf8().data());
	TempNode.addAttribute("helmet", QString::number(helmetID).toUtf8().data());
	TempNode.addAttribute("accessory", QString::number(accessoryID).toUtf8().data());
	TempNode = CharacterNode.addChild("skillProgression");
	for (int i = 0; i < skillProgression.size(); ++i)
	{
		TempNode2 = TempNode.addChild("skillLearned");
		TempNode2.addAttribute("level", QString::number(skillProgression[i].levelLearned).toUtf8().data());
		TempNode2.addAttribute("skill", QString::number(skillProgression[i].skillID).toUtf8().data());
	}
	TempNode = CharacterNode.addChild("conditionResists");
	for (int i = 0; i < conditionResists.size(); ++i)
	{
		if (conditionResists[i] == RESISTLEVEL_A)
			tempStringList.append("A");
		else if (conditionResists[i] == RESISTLEVEL_B)
			tempStringList.append("B");
		else if (conditionResists[i] == RESISTLEVEL_C)
			tempStringList.append("C");
		else if (conditionResists[i] == RESISTLEVEL_D)
			tempStringList.append("D");
		else
			tempStringList.append("E");
	}
	TempNode.addText(tempStringList.join("").toUtf8().data());
	tempStringList.clear();
	TempNode = CharacterNode.addChild("attributeResists");
	for (int i = 0; i < attributeResists.size(); ++i)
	{
		if (attributeResists[i] == RESISTLEVEL_A)
			tempStringList.append("A");
		else if (attributeResists[i] == RESISTLEVEL_B)
			tempStringList.append("B");
		else if (attributeResists[i] == RESISTLEVEL_C)
			tempStringList.append("C");
		else if (attributeResists[i] == RESISTLEVEL_D)
			tempStringList.append("D");
		else
			tempStringList.append("E");
	}
	TempNode.addText(tempStringList.join("").toUtf8().data());
	tempStringList.clear();
	TempNode = CharacterNode.addChild("battleEvent");
	TempNode = CharacterNode.addChild("customAI");
	TODO("Uncomment the following lines when the battle animations are ready");
	//TempNode = battleEvent->getEventNode();
	//TempNode.updateName("battleEvent");
	//CharacterNode.addChild(TempNode);
	//TempNode = customAI->getEventNode();
	//TempNode.updateName("customAI");
	//CharacterNode.addChild(TempNode);
	return CharacterNode;
}

BattleEvent *Character::getBattleEventPointer()
{
	return battleEvent;
}

BattleEvent *Character::getCustomAIPointer()
{
	return customAI;
}

QString Character::getName()
{
	return name;
}

void Character::setName(QString value)
{
	name = value;
}

QString Character::getFaceGraphicLocation()
{
	return faceGraphicLocation;
}

void Character::setFaceGraphicLocation(QString location)
{
	faceGraphicLocation = location;
}

int Character::numSkillsLearned()
{
	return skillProgression.size();
}

Character::SkillLearned *Character::getSkillLearnedPointer(int skillLearnedID)
{
	return &(skillProgression[skillLearnedID]);
}

void Character::addSkillLearned(SkillLearned skillLearned)
{
	QList<SkillLearned>::iterator iter;
	bool foundSpot;
	for (iter = skillProgression.begin(); iter != skillProgression.end() && !foundSpot; ++iter)
	{
		if (iter->levelLearned > skillLearned.levelLearned)
		{
			iter = skillProgression.insert(iter, skillLearned);
			foundSpot = true;
		}
	}
	if (!foundSpot)
		skillProgression.append(skillLearned);
}

void Character::deleteSkillLearned(int skillLearnedID)
{
	skillProgression.removeAt(skillLearnedID);
}

int Character::getConditionResist(int conditionID)
{
	return conditionResists[conditionID];
}

void Character::setConditionResist(int conditionID, int value)
{
	conditionResists[conditionID] = value;
}

void Character::resizeConditionResists(int numConditions)
{
	if (numConditions > conditionResists.size())
	{
		for (int i = conditionResists.size(); i < numConditions; ++i)
			conditionResists.append(RESISTLEVEL_C);
	}
	else
	{
		for (int i = numConditions; i < conditionResists.size(); ++i)
			conditionResists.removeAt(numConditions);
	}
}

int Character::getAttributeResist(int attributeID)
{
	return attributeResists[attributeID];
}

void Character::setAttributeResist(int attributeID, int value)
{
	attributeResists[attributeID] = value;
}

void Character::resizeAttributeResists(int numAttributes)
{
	if (numAttributes > attributeResists.size())
	{
		for (int i = attributeResists.size(); i < numAttributes; ++i)
			attributeResists.append(RESISTLEVEL_C);
	}
	else
	{
		for (int i = numAttributes; i < attributeResists.size(); ++i)
			attributeResists.removeAt(numAttributes);
	}
}

float Character::getCriticalHitRate()
{
	return criticalHitRate;
}

void Character::setCriticalHitRate(float value)
{
	criticalHitRate = value;
}

float Character::getCriticalHitMultiplier()
{
	return criticalHitMultiplier;
}

void Character::setCriticalHitMultiplier(float value)
{
	criticalHitMultiplier = value;
}

int Character::getMaxHP(int level)
{
	return maxHP[level - 1];
}

void Character::setMaxHP(int level, int value)
{
	maxHP[level - 1] = value;
}

int Character::getMaxMP(int level)
{
	return maxMP[level - 1];
}

void Character::setMaxMP(int level, int value)
{
	maxMP[level - 1] = value;
}

int Character::getAttack(int level)
{
	return attack[level - 1];
}

void Character::setAttack(int level, int value)
{
	attack[level - 1] = value;
}

int Character::getDefense(int level)
{
	return defense[level - 1];
}

void Character::setDefense(int level, int value)
{
	defense[level - 1] = value;
}

int Character::getWisdom(int level)
{
	return wisdom[level - 1];
}

void Character::setWisdom(int level, int value)
{
	wisdom[level - 1] = value;
}

int Character::getAgility(int level)
{
	return agility[level - 1];
}

void Character::setAgility(int level, int value)
{
	agility[level - 1] = value;
}

int Character::getBattleCommand(int num)
{
	return battleCommands[num];
}

void Character::setBattleCommand(int num, int value)
{
	battleCommands[num] = value;
}

int Character::getMinLevel()
{
	return minLevel;
}

void Character::setMinLevel(int value)
{
	minLevel = value;
}

int Character::getMaxLevel()
{
	return maxLevel;
}

void Character::setMaxLevel(int value)
{
	maxLevel = value;
}

int Character::getClassID()
{
	return classID;
}

void Character::setClassID(int value)
{
	classID = value;
}

int Character::getSpriteID()
{
	return spriteID;
}

void Character::setSpriteID(int value)
{
	spriteID = value;
}

int Character::getExperiencePrimary()
{
	return experiencePrimary;
}

void Character::setExperiencePrimary(int value)
{
	experiencePrimary = value;
}

int Character::getExperienceSecondary()
{
	return experienceSecondary;
}

void Character::setExperienceSecondary(int value)
{
	experienceSecondary = value;
}

int Character::getWeaponID()
{
	return weaponID;
}

void Character::setWeaponID(int value)
{
	weaponID = value;
}

int Character::getShieldID()
{
	return shieldID;
}

void Character::setShieldID(int value)
{
	shieldID = value;
}

int Character::getArmorID()
{
	return armorID;
}

void Character::setArmorID(int value)
{
	armorID = value;
}

int Character::getHelmetID()
{
	return helmetID;
}

void Character::setHelmetID(int value)
{
	helmetID = value;
}

int Character::getAccessoryID()
{
	return accessoryID;
}

void Character::setAccessoryID(int value)
{
	accessoryID = value;
}

int Character::getUnarmedBattleAnimationID()
{
	return unarmedBattleAnimationID;
}

void Character::setUnarmedBattleAnimationID(int value)
{
	unarmedBattleAnimationID = value;
}

int Character::getAILevel()
{
	return aiLevel;
}

void Character::setAILevel(int value)
{
	aiLevel = value;
}

int Character::getSpriteOpacity()
{
	return spriteOpacity;
}

void Character::setSpriteOpacity(int value)
{
	spriteOpacity = value;
}

bool Character::isDuelWielding()
{
	return duelWielding;
}

void Character::setDuelWielding(bool value)
{
	duelWielding = value;
}

bool Character::isAIControlled()
{
	return aiControlled;
}

void Character::setAIControlled(bool value)
{
	aiControlled = value;
}

bool Character::isEquipmentLocked()
{
	return equipmentLocked;
}

void Character::setEquipmentLocked(bool value)
{
	equipmentLocked = value;
}

bool Character::isMightyGuard()
{
	return mightyGuard;
}

void Character::setMightyGuard(bool value)
{
	mightyGuard = value;
}

bool Character::isCriticalHits()
{
	return criticalHits;
}

void Character::setCriticalHits(bool value)
{
	criticalHits = value;
}

int Character::getTotalExperience(int level)
{
	int totalExperience = 0;
	int nextLevelExperience = 0;
	for (int i = 0; i < level; ++i)
	{
		totalExperience += nextLevelExperience;
		nextLevelExperience = (experiencePrimary * (i + 1)) + experienceSecondary;
	}
	return totalExperience;
}

bool Character::tryLoad(XMLNode CharacterNode)
{
	XMLNode TempNode;
	QString temp_string;
	QStringList tempStringList;
	int temp_int;
	if (!XMLTools::attributeExists(CharacterNode, "name"))
		return false;
	if (!XMLTools::attributeExists(CharacterNode, "minLevel"))
		return false;
	if (!XMLTools::attributeIntRangeValid(CharacterNode, "minLevel", 1, 99))
		return false;
	temp_int = QString(CharacterNode.getAttribute("minLevel")).toInt();
	if (!XMLTools::attributeExists(CharacterNode, "maxLevel"))
		return false;
	if (!XMLTools::attributeIntRangeValid(CharacterNode, "maxLevel", temp_int, 99))
		return false;
	if (!XMLTools::attributeExists(CharacterNode, "classID"))
		return false;
	if (!XMLTools::attributeExists(CharacterNode, "unarmedBattleAnimation"))
		return false;
	if (!XMLTools::attributeExists(CharacterNode, "aiLevel"))
		return false;
	if (!XMLTools::attributeIntRangeValid(CharacterNode, "aiLevel", 0, 3))
		return false;
	if (!XMLTools::nodeExists(CharacterNode, "options"))
		return false;
	TempNode = CharacterNode.getChildNode("options");
	if (!XMLTools::attributeExists(TempNode, "duelWielding"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "duelWielding", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(TempNode, "aiControlled"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "aiControlled", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(TempNode, "equipmentLocked"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "equipmentLocked", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(TempNode, "mightyGuard"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "mightyGuard", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::nodeExists(CharacterNode, "criticalHits"))
		return false;
	TempNode = CharacterNode.getChildNode("criticalHits");
	if (!XMLTools::attributeExists(TempNode, "hasCriticalHits"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "hasCriticalHits", QStringList() << "true" << "false", 1))
		return false;
	if (!XMLTools::attributeExists(TempNode, "rate"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "multiplier"))
		return false;
	if (!XMLTools::nodeExists(CharacterNode, "graphics"))
		return false;
	TempNode = CharacterNode.getChildNode("graphics");
	if (!XMLTools::attributeExists(TempNode, "faceGraphic"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "sprite"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "spriteOpacity"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "spriteOpacity", 0, 100))
		return false;
	if (!XMLTools::nodeExists(CharacterNode, "baseStats"))
		return false;
	TempNode = CharacterNode.getChildNode("baseStats");
	if (!XMLTools::nodeExists(TempNode, "maxHP"))
		return false;
	tempStringList = QString(TempNode.getChildNode("maxHP").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	if (tempStringList.size() != 99)
	{
		QMessageBox::warning(0, "Warning: Missing Data", "'maxHP' node does not have the values for all 99 possible levels.\nA default value of 1 will be assigned to the missing levels.");
		return false;
	}
	tempStringList.clear();
	if (!XMLTools::nodeExists(TempNode, "maxMP"))
		return false;
	tempStringList = QString(TempNode.getChildNode("maxMP").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	if (tempStringList.size() != 99)
	{
		QMessageBox::warning(0, "Warning: Missing Data", "'maxMP' node does not have the values for all 99 possible levels.\nA default value of 1 will be assigned to the missing levels.");
		return false;
	}
	tempStringList.clear();
	if (!XMLTools::nodeExists(TempNode, "attack"))
		return false;
	tempStringList = QString(TempNode.getChildNode("attack").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	if (tempStringList.size() != 99)
	{
		QMessageBox::warning(0, "Warning: Missing Data", "'attack' node does not have the values for all 99 possible levels.\nA default value of 1 will be assigned to the missing levels.");
		return false;
	}
	tempStringList.clear();
	if (!XMLTools::nodeExists(TempNode, "defense"))
		return false;
	tempStringList = QString(TempNode.getChildNode("defense").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	if (tempStringList.size() != 99)
	{
		QMessageBox::warning(0, "Warning: Missing Data", "'defense' node does not have the values for all 99 possible levels.\nA default value of 1 will be assigned to the missing levels.");
		return false;
	}
	tempStringList.clear();
	if (!XMLTools::nodeExists(TempNode, "wisdom"))
		return false;
	tempStringList = QString(TempNode.getChildNode("wisdom").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	if (tempStringList.size() != 99)
	{
		QMessageBox::warning(0, "Warning: Missing Data", "'wisdom' node does not have the values for all 99 possible levels.\nA default value of 1 will be assigned to the missing levels.");
		return false;
	}
	tempStringList.clear();
	if (!XMLTools::nodeExists(TempNode, "agility"))
		return false;
	tempStringList = QString(TempNode.getChildNode("agility").getText()).split(QRegExp("\\s"), QString::SkipEmptyParts);
	if (tempStringList.size() != 99)
	{
		QMessageBox::warning(0, "Warning: Missing Data", "'agility' node does not have the values for all 99 possible levels.\nA default value of 1 will be assigned to the missing levels.");
		return false;
	}
	tempStringList.clear();
	if (!XMLTools::nodeExists(CharacterNode, "experienceCurve"))
		return false;
	TempNode = CharacterNode.getChildNode("experienceCurve");
	if (!XMLTools::attributeExists(TempNode, "primary"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "secondary"))
		return false;
	if (!XMLTools::nodeExists(CharacterNode, "battleCommands"))
		return false;
	TempNode = CharacterNode.getChildNode("battleCommands");
	for (int i = 0; i < 6; ++i)
	{
		if (!XMLTools::attributeExists(TempNode, QString("command%1").arg(i + 1).toUtf8().data()))
			return false;
	}
	if (!XMLTools::nodeExists(CharacterNode, "startingEquipment"))
		return false;
	TempNode = CharacterNode.getChildNode("startingEquipment");
	if (!XMLTools::attributeExists(TempNode, "weapon"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "shield"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "armor"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "helmet"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "accessory"))
		return false;
	if (!XMLTools::nodeExists(CharacterNode, "skillProgression"))
		return false;
	TempNode = CharacterNode.getChildNode("skillProgression");
	for (int i = 0; i < TempNode.nChildNode("skillLearned"); ++i)
	{
		XMLNode TempNode2 = TempNode.getChildNode("skillLearned", i);
		if (!XMLTools::attributeExists(TempNode2, "level"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode2, "level", 1, 99))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "skill"))
			return false;
	}
	if (!XMLTools::nodeExists(CharacterNode, "conditionResists"))
		return false;
	temp_string = CharacterNode.getChildNode("conditionResists").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		QChar value = temp_string[i].toUpper();
		if (value != QChar('A') && value != QChar('B') && value != QChar('C') && value != QChar('D') && value != QChar('E'))
			QMessageBox::warning(0, "Warning: Invalid Data", QString("Resistance for condition %1 must be A, B, C, D, or E.\nDefaulting to E.").arg(i));
	}
	if (!XMLTools::nodeExists(CharacterNode, "attributeResists"))
		return false;
	temp_string = CharacterNode.getChildNode("attributeResists").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		QChar value = temp_string[i].toUpper();
		if (value != QChar('A') && value != QChar('B') && value != QChar('C') && value != QChar('D') && value != QChar('E'))
			QMessageBox::warning(0, "Warning: Invalid Data", QString("Resistance for attribute %1 must be A, B, C, D, or E.\nDefaulting to E.").arg(i));
	}
	if (!XMLTools::nodeExists(CharacterNode, "battleEvent"))
		return false;
	TODO("Uncomment the following lines when the battle events are ready");
	//if (!BattleEvent::tryLoad(CharacterNode.getChildNode("battleEvent")))
	//	return false;
	if (!XMLTools::nodeExists(CharacterNode, "customAI"))
		return false;
	//if (!BattleEvent::tryLoad(CharacterNode.getChildNode("customAI")))
	//	return false;
	return true;
}
