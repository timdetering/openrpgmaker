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
#include "projectdata_class.h"
#include "xmlParser.h"
#include "xmlTools.h"

Class::Class()
{
	name = "Untitled";
	for (int i = 0; i < ProjectData::conditionNames.size(); ++i)
		conditionResists.append(RESISTLEVEL_C);
	for (int i = 0; i < ProjectData::attributeNames.size(); ++i)
		attributeResists.append(RESISTLEVEL_C);
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
	spriteID = 0;
	spriteOpacity = 100;
	experiencePrimary = 1;
	experienceSecondary = 0;
	duelWielding = false;
	aiControlled = false;
	equipmentLocked = false;
	mightyGuard = false;
}

Class::Class(Class &other)
{
	name = other.name;
	for (int i = 0; i < other.conditionResists.size(); ++i)
		conditionResists.append(other.conditionResists[i]);
	for (int i = 0; i < other.attributeResists.size(); ++i)
		attributeResists.append(other.attributeResists[i]);
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
	spriteID = other.spriteID;
	spriteOpacity = other.spriteOpacity;
	experiencePrimary = other.experiencePrimary;
	experienceSecondary = other.experienceSecondary;
	duelWielding = other.duelWielding;
	aiControlled = other.aiControlled;
	equipmentLocked = other.equipmentLocked;
	mightyGuard = other.mightyGuard;
}

Class::Class(const Class &other)
{
	name = other.name;
	for (int i = 0; i < other.conditionResists.size(); ++i)
		conditionResists.append(other.conditionResists[i]);
	for (int i = 0; i < other.attributeResists.size(); ++i)
		attributeResists.append(other.attributeResists[i]);
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
	spriteID = other.spriteID;
	spriteOpacity = other.spriteOpacity;
	experiencePrimary = other.experiencePrimary;
	experienceSecondary = other.experienceSecondary;
	duelWielding = other.duelWielding;
	aiControlled = other.aiControlled;
	equipmentLocked = other.equipmentLocked;
	mightyGuard = other.mightyGuard;
}

Class::Class(XMLNode ClassNode)
{
	XMLNode TempNode;
	QString temp_string;
	QStringList tempStringList;
	name = ClassNode.getAttribute("name");
	spriteID = QString(ClassNode.getAttribute("sprite")).toInt();
	spriteOpacity = QString(ClassNode.getAttribute("spriteOpacity")).toInt();
	TempNode = ClassNode.getChildNode("options");
	temp_string = TempNode.getAttribute("duelWielding");
	duelWielding = (temp_string == "true") ? true:false;
	temp_string = TempNode.getAttribute("aiControlled");
	aiControlled = (temp_string == "true") ? true:false;
	temp_string = TempNode.getAttribute("equipmentLocked");
	equipmentLocked = (temp_string == "true") ? true:false;
	temp_string = TempNode.getAttribute("mightyGuard");
	mightyGuard = (temp_string == "true") ? true:false;
	TempNode = ClassNode.getChildNode("baseStats");
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
	TempNode = ClassNode.getChildNode("experienceCurve");
	experiencePrimary = QString(TempNode.getAttribute("primary")).toInt();
	experienceSecondary = QString(TempNode.getAttribute("secondary")).toInt();
	TempNode = ClassNode.getChildNode("battleCommands");
	for (int i = 0; i < 6; ++i)
		battleCommands[i] = QString(TempNode.getAttribute(QString("command%1").arg(i + 1).toUtf8().data())).toInt();
	TempNode = ClassNode.getChildNode("skillProgression");
	for (int i = 0; i < TempNode.nChildNode("skillLearned"); ++i)
	{
		XMLNode TempNode2 = TempNode.getChildNode("skillLearned", i);
		SkillLearned skillLearned;
		skillLearned.levelLearned = QString(TempNode2.getAttribute("level")).toInt();
		skillLearned.skillID = QString(TempNode2.getAttribute("skill")).toInt();
		skillProgression.append(skillLearned);
	}
	temp_string = ClassNode.getChildNode("conditionResists").getText();
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
	temp_string = ClassNode.getChildNode("attributeResists").getText();
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
}

Class::Class(StorageFile &storageFile)
{
	int temp_int;
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
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
	spriteID = storageFile.getSignedInt();
	spriteOpacity = storageFile.getSignedInt();
	experiencePrimary = storageFile.getSignedInt();
	experienceSecondary = storageFile.getSignedInt();
	duelWielding = storageFile.getBool();
	aiControlled = storageFile.getBool();
	equipmentLocked = storageFile.getBool();
	mightyGuard = storageFile.getBool();
}

Class::~Class()
{
	skillProgression.clear();
	conditionResists.clear();
	attributeResists.clear();
}

void Class::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
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
	storageFile.putSignedInt(spriteID);
	storageFile.putSignedInt(spriteOpacity);
	storageFile.putSignedInt(experiencePrimary);
	storageFile.putSignedInt(experienceSecondary);
	storageFile.putBool(duelWielding);
	storageFile.putBool(aiControlled);
	storageFile.putBool(equipmentLocked);
	storageFile.putBool(mightyGuard);
}

QStringList Class::getResources()
{
	return QStringList();
}

XMLNode Class::getClassNode()
{
	XMLNode TempNode, TempNode2, ClassNode = XMLNode::createXMLTopNode("class");
	QStringList tempStringList;
	ClassNode.addAttribute("name", name.toUtf8().data());
	ClassNode.addAttribute("sprite", QString::number(spriteID).toUtf8().data());
	ClassNode.addAttribute("spriteOpacity", QString::number(spriteOpacity).toUtf8().data());
	TempNode = ClassNode.addChild("options");
	TempNode.addAttribute("duelWielding", (duelWielding) ? "true":"false");
	TempNode.addAttribute("aiControlled", (aiControlled) ? "true":"false");
	TempNode.addAttribute("equipmentLocked", (equipmentLocked) ? "true":"false");
	TempNode.addAttribute("mightyGuard", (mightyGuard) ? "true":"false");
	TempNode = ClassNode.addChild("baseStats");
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
	TempNode = ClassNode.addChild("experienceCurve");
	TempNode.addAttribute("primary", QString::number(experiencePrimary).toUtf8().data());
	TempNode.addAttribute("secondary", QString::number(experienceSecondary).toUtf8().data());
	TempNode = ClassNode.addChild("battleCommands");
	for (int i = 0; i < 6; ++i)
		TempNode.addAttribute(QString("command%1").arg(i + 1).toUtf8().data(), QString::number(battleCommands[i]).toUtf8().data());
	TempNode = ClassNode.addChild("skillProgression");
	for (int i = 0; i < skillProgression.size(); ++i)
	{
		TempNode2 = TempNode.addChild("skillLearned");
		TempNode2.addAttribute("level", QString::number(skillProgression[i].levelLearned).toUtf8().data());
		TempNode2.addAttribute("skill", QString::number(skillProgression[i].skillID).toUtf8().data());
	}
	TempNode = ClassNode.addChild("conditionResists");
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
	TempNode = ClassNode.addChild("attributeResists");
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
	return ClassNode;
}

QString Class::getName()
{
	return name;
}

void Class::setName(QString value)
{
	name = value;
}

int Class::numSkillsLearned()
{
	return skillProgression.size();
}

Class::SkillLearned *Class::getSkillLearnedPointer(int skillLearnedID)
{
	return &(skillProgression[skillLearnedID]);
}

void Class::addSkillLearned(SkillLearned skillLearned)
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

void Class::deleteSkillLearned(int skillLearnedID)
{
	skillProgression.removeAt(skillLearnedID);
}

int Class::getConditionResist(int conditionID)
{
	return conditionResists[conditionID];
}

void Class::setConditionResist(int conditionID, int value)
{
	conditionResists[conditionID] = value;
}

void Class::resizeConditionResists(int numConditions)
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

int Class::getAttributeResist(int attributeID)
{
	return attributeResists[attributeID];
}

void Class::setAttributeResist(int attributeID, int value)
{
	attributeResists[attributeID] = value;
}

void Class::resizeAttributeResists(int numAttributes)
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

int Class::getMaxHP(int level)
{
	return maxHP[level - 1];
}

void Class::setMaxHP(int level, int value)
{
	maxHP[level - 1] = value;
}

int Class::getMaxMP(int level)
{
	return maxMP[level - 1];
}

void Class::setMaxMP(int level, int value)
{
	maxMP[level - 1] = value;
}

int Class::getAttack(int level)
{
	return attack[level - 1];
}

void Class::setAttack(int level, int value)
{
	attack[level - 1] = value;
}

int Class::getDefense(int level)
{
	return defense[level - 1];
}

void Class::setDefense(int level, int value)
{
	defense[level - 1] = value;
}

int Class::getWisdom(int level)
{
	return wisdom[level - 1];
}

void Class::setWisdom(int level, int value)
{
	wisdom[level - 1] = value;
}

int Class::getAgility(int level)
{
	return agility[level - 1];
}

void Class::setAgility(int level, int value)
{
	agility[level - 1] = value;
}

int Class::getBattleCommand(int num)
{
	return battleCommands[num];
}

void Class::setBattleCommand(int num, int value)
{
	battleCommands[num] = value;
}

int Class::getSpriteID()
{
	return spriteID;
}

void Class::setSpriteID(int value)
{
	spriteID = value;
}

int Class::getSpriteOpacity()
{
	return spriteOpacity;
}

void Class::setSpriteOpacity(int value)
{
	spriteOpacity = value;
}

int Class::getExperiencePrimary()
{
	return experiencePrimary;
}

void Class::setExperiencePrimary(int value)
{
	experiencePrimary = value;
}

int Class::getExperienceSecondary()
{
	return experienceSecondary;
}

void Class::setExperienceSecondary(int value)
{
	experienceSecondary = value;
}

bool Class::isDuelWielding()
{
	return duelWielding;
}

void Class::setDuelWielding(bool value)
{
	duelWielding = value;
}

bool Class::isAIControlled()
{
	return aiControlled;
}

void Class::setAIControlled(bool value)
{
	aiControlled = value;
}

bool Class::isEquipmentLocked()
{
	return equipmentLocked;
}

void Class::setEquipmentLocked(bool value)
{
	equipmentLocked = value;
}

bool Class::isMightyGuard()
{
	return mightyGuard;
}

void Class::setMightyGuard(bool value)
{
	mightyGuard = value;
}

bool Class::tryLoad(XMLNode ClassNode)
{
	XMLNode TempNode;
	QString temp_string;
	QStringList tempStringList;
	if (!XMLTools::attributeExists(ClassNode, "name"))
		return false;
	if (!XMLTools::attributeExists(ClassNode, "sprite"))
		return false;
	if (!XMLTools::attributeExists(ClassNode, "spriteOpacity"))
		return false;
	if (!XMLTools::nodeExists(ClassNode, "options"))
		return false;
	TempNode = ClassNode.getChildNode("options");
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
	if (!XMLTools::nodeExists(ClassNode, "baseStats"))
		return false;
	TempNode = ClassNode.getChildNode("baseStats");
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
	if (!XMLTools::nodeExists(ClassNode, "experienceCurve"))
		return false;
	TempNode = ClassNode.getChildNode("experienceCurve");
	if (!XMLTools::attributeExists(TempNode, "primary"))
		return false;
	if (!XMLTools::attributeExists(TempNode, "secondary"))
		return false;
	if (!XMLTools::nodeExists(ClassNode, "battleCommands"))
		return false;
	TempNode = ClassNode.getChildNode("battleCommands");
	for (int i = 0; i < 6; ++i)
	{
		if (!XMLTools::attributeExists(TempNode, QString("command%1").arg(i + 1).toUtf8().data()))
			return false;
	}
	if (!XMLTools::nodeExists(ClassNode, "skillProgression"))
		return false;
	TempNode = ClassNode.getChildNode("skillProgression");
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
	if (!XMLTools::nodeExists(ClassNode, "conditionResists"))
		return false;
	temp_string = ClassNode.getChildNode("conditionResists").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		QChar value = temp_string[i].toUpper();
		if (value != QChar('A') && value != QChar('B') && value != QChar('C') && value != QChar('D') && value != QChar('E'))
			QMessageBox::warning(0, "Warning: Invalid Data", QString("Resistance for condition %1 must be A, B, C, D, or E.\nDefaulting to E.").arg(i));
	}
	if (!XMLTools::nodeExists(ClassNode, "attributeResists"))
		return false;
	temp_string = ClassNode.getChildNode("attributeResists").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		QChar value = temp_string[i].toUpper();
		if (value != QChar('A') && value != QChar('B') && value != QChar('C') && value != QChar('D') && value != QChar('E'))
			QMessageBox::warning(0, "Warning: Invalid Data", QString("Resistance for attribute %1 must be A, B, C, D, or E.\nDefaulting to E.").arg(i));
	}
	return true;
}
