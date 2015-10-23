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

#include <QColor>
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_battleevent.h"
#include "projectdata_monster.h"
#include "xmlParser.h"
#include "xmlTools.h"

Monster::Monster()
{
	name = "Untitled";
	for (int i = 0; i < ProjectData::conditionNames.size(); ++i)
		conditionResists.append(RESISTLEVEL_C);
	for (int i = 0; i < ProjectData::attributeNames.size(); ++i)
		attributeResists.append(RESISTLEVEL_C);
	battleEvent = new BattleEvent();
	customAI = new BattleEvent();
	criticalHitRate = 3.0;
	criticalHitMultiplier = 2.0;
	maxHP = 1;
	maxMP = 1;
	attack = 1;
	defense = 1;
	wisdom = 1;
	agility = 1;
	experienceGain = 0;
	moneyAquired = 0;
	itemDropProbability = 0;
	monsterAnimationID = 0;
	hueShift = 0;
	aiLevel = AILEVEL_BASIC;
	transparent = false;
	flying = false;
	attacksOftenMiss = false;
}

Monster::Monster(Monster &other)
{
	name = other.name;
	for (int i = 0; i < other.itemsDropped.size(); ++i)
		itemsDropped.append(other.itemsDropped[i]);
	for (int i = 0; i < other.behaviorActions.size(); ++i)
		behaviorActions.append(other.behaviorActions[i]);
	for (int i = 0; i < other.conditionResists.size(); ++i)
		conditionResists.append(other.conditionResists[i]);
	for (int i = 0; i < other.attributeResists.size(); ++i)
		attributeResists.append(other.attributeResists[i]);
	battleEvent = new BattleEvent(*(other.battleEvent));
	customAI = new BattleEvent(*(other.customAI));
	criticalHitRate = other.criticalHitRate;
	criticalHitMultiplier = other.criticalHitMultiplier;
	maxHP = other.maxHP;
	maxMP = other.maxMP;
	attack = other.attack;
	defense = other.defense;
	wisdom = other.wisdom;
	agility = other.agility;
	experienceGain = other.experienceGain;
	moneyAquired = other.moneyAquired;
	itemDropProbability = other.itemDropProbability;
	monsterAnimationID = other.monsterAnimationID;
	hueShift = other.hueShift;
	aiLevel = other.aiLevel;
	transparent = other.transparent;
	flying = other.flying;
	attacksOftenMiss = other.attacksOftenMiss;
}

Monster::Monster(const Monster &other)
{
	name = other.name;
	for (int i = 0; i < other.itemsDropped.size(); ++i)
		itemsDropped.append(other.itemsDropped[i]);
	for (int i = 0; i < other.behaviorActions.size(); ++i)
		behaviorActions.append(other.behaviorActions[i]);
	for (int i = 0; i < other.conditionResists.size(); ++i)
		conditionResists.append(other.conditionResists[i]);
	for (int i = 0; i < other.attributeResists.size(); ++i)
		attributeResists.append(other.attributeResists[i]);
	battleEvent = new BattleEvent(*(other.battleEvent));
	customAI = new BattleEvent(*(other.customAI));
	criticalHitRate = other.criticalHitRate;
	criticalHitMultiplier = other.criticalHitMultiplier;
	maxHP = other.maxHP;
	maxMP = other.maxMP;
	attack = other.attack;
	defense = other.defense;
	wisdom = other.wisdom;
	agility = other.agility;
	experienceGain = other.experienceGain;
	moneyAquired = other.moneyAquired;
	itemDropProbability = other.itemDropProbability;
	monsterAnimationID = other.monsterAnimationID;
	hueShift = other.hueShift;
	aiLevel = other.aiLevel;
	transparent = other.transparent;
	flying = other.flying;
	attacksOftenMiss = other.attacksOftenMiss;
}

Monster::Monster(XMLNode MonsterNode)
{
	XMLNode TempNode1, TempNode2;
	QString temp_string;
	name = MonsterNode.getAttribute("name");
	temp_string = MonsterNode.getAttribute("attacksOftenMiss");
	if (temp_string == "true")
		attacksOftenMiss = true;
	else
		attacksOftenMiss = false;
	aiLevel = QString(MonsterNode.getAttribute("aiLevel")).toInt();
	TempNode1 = MonsterNode.getChildNode("graphic");
	monsterAnimationID = QString(TempNode1.getAttribute("monsterAnimationID")).toInt();
	temp_string = MonsterNode.getAttribute("transparent");
	if (temp_string == "true")
		transparent = true;
	else
		transparent = false;
	temp_string = MonsterNode.getAttribute("flying");
	if (temp_string == "true")
		flying = true;
	else
		flying = false;
	hueShift = QString(MonsterNode.getAttribute("hueShift")).toInt();
	TempNode1 = MonsterNode.getChildNode("stats");
	maxHP = QString(TempNode1.getAttribute("maxHP")).toInt();
	maxMP = QString(TempNode1.getAttribute("maxMP")).toInt();
	attack = QString(TempNode1.getAttribute("attack")).toInt();
	defense = QString(TempNode1.getAttribute("defense")).toInt();
	wisdom = QString(TempNode1.getAttribute("wisdom")).toInt();
	agility = QString(TempNode1.getAttribute("agility")).toInt();
	temp_string = MonsterNode.getChildNode("conditionResists").getText();
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
	temp_string = MonsterNode.getChildNode("attributeResists").getText();
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
	TempNode1 = MonsterNode.getChildNode("criticalHits");
	temp_string = TempNode1.getAttribute("hasCriticalHits");
	if (temp_string == "true")
		criticalHits = true;
	else
		criticalHits = false;
	criticalHitRate = QString(TempNode1.getAttribute("rate")).toFloat();
	criticalHitMultiplier = QString(TempNode1.getAttribute("multiplier")).toFloat();
	TempNode1 = MonsterNode.getChildNode("spoils");
	experienceGain = QString(TempNode1.getAttribute("experienceGain")).toInt();
	moneyAquired = QString(TempNode1.getAttribute("moneyAquired")).toInt();
	itemDropProbability = QString(TempNode1.getAttribute("itemDropProbability")).toInt();
	for (int i = 0, count = TempNode1.nChildNode("itemDropped"); i < count; ++i)
	{
		ItemDrop itemDropped;
		TempNode2 = TempNode1.getChildNode("itemDropped", i);
		itemDropped.itemID = QString(TempNode2.getAttribute("itemID")).toInt();
		itemDropped.priority = QString(TempNode2.getAttribute("priority")).toInt();
		itemsDropped.append(itemDropped);
	}
	TempNode1 = MonsterNode.getChildNode("behavior");
	for (int i = 0, count = TempNode1.nChildNode("action"); i < count; ++i)
	{
		BehaviorAction action;
		TempNode2 = TempNode1.getChildNode("action", i);
		temp_string = TempNode2.getAttribute("precondition");
		if (temp_string == "switch")
			action.preconditionType = PRECONDITION_SWITCH;
		else if (temp_string == "turnselapsed")
			action.preconditionType = PRECONDITION_TURNSELAPSED;
		else if (temp_string == "monsterspresent")
			action.preconditionType = PRECONDITION_MONSTERSPRESENT;
		else if (temp_string == "monstershp")
			action.preconditionType = PRECONDITION_MONSTERSHP;
		else if (temp_string == "monstersmp")
			action.preconditionType = PRECONDITION_MONSTERSMP;
		else if (temp_string == "partysaveragelevel")
			action.preconditionType = PRECONDITION_PARTYSAVERAGELEVEL;
		else if (temp_string == "partysexhaustion")
			action.preconditionType = PRECONDITION_PARTYSEXHAUSTION;
		else
			action.preconditionType = PRECONDITION_NONE;
		action.priority = QString(TempNode2.getAttribute("priority")).toInt();
		action.preconditionValue1 = QString(TempNode2.getAttribute("preconditionValue1")).toInt();
		action.preconditionValue2 = QString(TempNode2.getAttribute("preconditionValue2")).toInt();
		temp_string = TempNode2.getAttribute("actionType");
		if (temp_string == "skill")
			action.actionType = TYPE_SKILL;
		else if (temp_string == "transform")
			action.actionType = TYPE_TRANSFORM;
		else
			action.actionType = TYPE_BASIC;
		action.actionID = QString(TempNode2.getAttribute("actionID")).toInt();
		temp_string = TempNode2.getAttribute("turnSwitchOn");
		if (temp_string == "true")
			action.turnSwitchOn = true;
		else
			action.turnSwitchOn = false;
		action.turnSwitchOnID = QString(TempNode2.getAttribute("turnSwitchOnID")).toInt();
		temp_string = TempNode2.getAttribute("turnSwitchOff");
		if (temp_string == "true")
			action.turnSwitchOff = true;
		else
			action.turnSwitchOff = false;
		action.turnSwitchOffID = QString(TempNode2.getAttribute("turnSwitchOffID")).toInt();
		action.monsterAnimationID = QString(TempNode2.getAttribute("monsterAnimationID")).toInt();
		behaviorActions.append(action);
	}
	battleEvent = new BattleEvent(MonsterNode.getChildNode("battleEvent"));
	customAI = new BattleEvent(MonsterNode.getChildNode("customAI"));
}

Monster::Monster(StorageFile &storageFile)
{
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		ItemDrop itemDropped;
		itemDropped.itemID = storageFile.getSignedInt();
		itemDropped.priority = storageFile.getSignedInt();
		itemsDropped.append(itemDropped);
	}
	behaviorActions.clear();
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
	{
		BehaviorAction action;
		action.preconditionType = storageFile.getSignedInt();
		action.priority = storageFile.getSignedInt();
		action.preconditionValue1 = storageFile.getSignedInt();
		action.preconditionValue2 = storageFile.getSignedInt();
		action.actionType = storageFile.getSignedInt();
		action.actionID = storageFile.getSignedInt();
		action.turnSwitchOnID = storageFile.getSignedInt();
		action.turnSwitchOffID = storageFile.getSignedInt();
		action.monsterAnimationID = storageFile.getSignedInt();
		action.turnSwitchOn = storageFile.getBool();
		action.turnSwitchOff = storageFile.getBool();
		behaviorActions.append(action);
	}
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
		conditionResists.append(storageFile.getSignedInt());
	for (int i = 0, count = storageFile.getSignedInt(); i < count; ++i)
		attributeResists.append(storageFile.getSignedInt());
	battleEvent = new BattleEvent();
	customAI = new BattleEvent();
	TODO("Uncomment the following lines when the battle events are ready");
	//battleEvent = new BattleEvent(storageFile);
	//customAI = new BattleEvent(storageFile);
	criticalHitRate = storageFile.getFloat();
	criticalHitMultiplier = storageFile.getFloat();
	maxHP = storageFile.getSignedInt();
	maxMP = storageFile.getSignedInt();
	attack = storageFile.getSignedInt();
	defense = storageFile.getSignedInt();
	wisdom = storageFile.getSignedInt();
	agility = storageFile.getSignedInt();
	experienceGain = storageFile.getSignedInt();
	moneyAquired = storageFile.getSignedInt();
	itemDropProbability = storageFile.getSignedInt();
	monsterAnimationID = storageFile.getSignedInt();
	hueShift = storageFile.getSignedInt();
	aiLevel = storageFile.getSignedInt();
	transparent = storageFile.getBool();
	flying = storageFile.getBool();
	attacksOftenMiss = storageFile.getBool();
}

Monster::~Monster()
{
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

void Monster::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putSignedInt(itemsDropped.size());
	for (int i = 0; i < itemsDropped.size(); ++i)
	{
		storageFile.putSignedInt(itemsDropped[i].itemID);
		storageFile.putSignedInt(itemsDropped[i].priority);
	}
	storageFile.putSignedInt(behaviorActions.size());
	for (int i = 0; i < behaviorActions.size(); ++i)
	{
		storageFile.putSignedInt(behaviorActions[i].preconditionType);
		storageFile.putSignedInt(behaviorActions[i].priority);
		storageFile.putSignedInt(behaviorActions[i].preconditionValue1);
		storageFile.putSignedInt(behaviorActions[i].preconditionValue2);
		storageFile.putSignedInt(behaviorActions[i].actionType);
		storageFile.putSignedInt(behaviorActions[i].actionID);
		storageFile.putSignedInt(behaviorActions[i].turnSwitchOnID);
		storageFile.putSignedInt(behaviorActions[i].turnSwitchOffID);
		storageFile.putSignedInt(behaviorActions[i].monsterAnimationID);
		storageFile.putBool(behaviorActions[i].turnSwitchOn);
		storageFile.putBool(behaviorActions[i].turnSwitchOff);
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
	storageFile.putSignedInt(maxHP);
	storageFile.putSignedInt(maxMP);
	storageFile.putSignedInt(attack);
	storageFile.putSignedInt(defense);
	storageFile.putSignedInt(wisdom);
	storageFile.putSignedInt(agility);
	storageFile.putSignedInt(experienceGain);
	storageFile.putSignedInt(moneyAquired);
	storageFile.putSignedInt(itemDropProbability);
	storageFile.putSignedInt(monsterAnimationID);
	storageFile.putSignedInt(hueShift);
	storageFile.putSignedInt(aiLevel);
	storageFile.putBool(transparent);
	storageFile.putBool(flying);
	storageFile.putBool(attacksOftenMiss);
}

QStringList Monster::getResources()
{
	QStringList resources;
	TODO("Add battle event's resources");
	TODO("Add custom AI's resources");
	return resources;
}

XMLNode Monster::getMonsterNode()
{
	XMLNode TempNode1, TempNode2, MonsterNode = XMLNode::createXMLTopNode("monster");
	QStringList tempStringList;
	MonsterNode.addAttribute("name", name.toUtf8().data());
	MonsterNode.addAttribute("attacksOftenMiss", (attacksOftenMiss) ? "true":"false");
	MonsterNode.addAttribute("aiLevel", QString::number(aiLevel).toUtf8().data());
	TempNode1 = MonsterNode.addChild("graphic");
	TempNode1.addAttribute("monsterAnimationID", QString::number(monsterAnimationID).toUtf8().data());
	TempNode1.addAttribute("transparent", (transparent) ? "true":"false");
	TempNode1.addAttribute("flying", (flying) ? "true":"false");
	TempNode1.addAttribute("hueShift", QString::number(hueShift).toUtf8().data());
	TempNode1 = MonsterNode.addChild("stats");
	TempNode1.addAttribute("maxHP", QString::number(maxHP).toUtf8().data());
	TempNode1.addAttribute("maxMP", QString::number(maxMP).toUtf8().data());
	TempNode1.addAttribute("attack", QString::number(attack).toUtf8().data());
	TempNode1.addAttribute("defense", QString::number(defense).toUtf8().data());
	TempNode1.addAttribute("wisdom", QString::number(wisdom).toUtf8().data());
	TempNode1.addAttribute("agility", QString::number(agility).toUtf8().data());
	TempNode1 = MonsterNode.addChild("conditionResists");
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
	TempNode1.addText(tempStringList.join("").toUtf8().data());
	tempStringList.clear();
	TempNode1 = MonsterNode.addChild("attributeResists");
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
	TempNode1.addText(tempStringList.join("").toUtf8().data());
	tempStringList.clear();
	TempNode1 = MonsterNode.addChild("criticalHits");
	TempNode1.addAttribute("hasCriticalHits", (criticalHits) ? "true":"false");
	TempNode1.addAttribute("rate", QString::number(criticalHitRate).toUtf8().data());
	TempNode1.addAttribute("multiplier", QString::number(criticalHitMultiplier).toUtf8().data());
	TempNode1 = MonsterNode.addChild("spoils");
	TempNode1.addAttribute("experienceGain", QString::number(experienceGain).toUtf8().data());
	TempNode1.addAttribute("moneyAquired", QString::number(moneyAquired).toUtf8().data());
	TempNode1.addAttribute("itemDropProbability", QString::number(itemDropProbability).toUtf8().data());
	for (int i = 0; i < itemsDropped.size(); ++i)
	{
		TempNode2 = TempNode1.addChild("itemDropped");
		TempNode2.addAttribute("itemID", QString::number(itemsDropped[i].itemID).toUtf8().data());
		TempNode2.addAttribute("priority", QString::number(itemsDropped[i].priority).toUtf8().data());
	}
	TempNode1 = MonsterNode.addChild("behavior");
	for (int i = 0; i < behaviorActions.size(); ++i)
	{
		BehaviorAction *action = &behaviorActions[i];
		TempNode2 = TempNode1.addChild("action");
		if (action->preconditionType == PRECONDITION_NONE)
			TempNode2.addAttribute("precondition", "none");
		else if (action->preconditionType == PRECONDITION_SWITCH)
			TempNode2.addAttribute("precondition", "switch");
		else if (action->preconditionType == PRECONDITION_TURNSELAPSED)
			TempNode2.addAttribute("precondition", "turnselapsed");
		else if (action->preconditionType == PRECONDITION_MONSTERSPRESENT)
			TempNode2.addAttribute("precondition", "monsterspresent");
		else if (action->preconditionType == PRECONDITION_MONSTERSHP)
			TempNode2.addAttribute("precondition", "monstershp");
		else if (action->preconditionType == PRECONDITION_MONSTERSMP)
			TempNode2.addAttribute("precondition", "monstersmp");
		else if (action->preconditionType == PRECONDITION_PARTYSAVERAGELEVEL)
			TempNode2.addAttribute("precondition", "partysaveragelevel");
		else if (action->preconditionType == PRECONDITION_PARTYSEXHAUSTION)
			TempNode2.addAttribute("precondition", "partysexhaustion");
		TempNode2.addAttribute("priority", QString::number(action->priority).toUtf8().data());
		TempNode2.addAttribute("preconditionValue1", QString::number(action->preconditionValue1).toUtf8().data());
		TempNode2.addAttribute("preconditionValue2", QString::number(action->preconditionValue2).toUtf8().data());
		if (action->actionType == TYPE_BASIC)
			TempNode2.addAttribute("actionType", "basic");
		else if (action->actionType == TYPE_SKILL)
			TempNode2.addAttribute("actionType", "skill");
		else if (action->actionType == TYPE_TRANSFORM)
			TempNode2.addAttribute("actionType", "transform");
		TempNode2.addAttribute("actionID", QString::number(action->actionID).toUtf8().data());
		TempNode2.addAttribute("turnSwitchOn", (action->turnSwitchOn) ? "true":"false");
		TempNode2.addAttribute("turnSwitchOnID", QString::number(action->turnSwitchOnID).toUtf8().data());
		TempNode2.addAttribute("turnSwitchOff", (action->turnSwitchOff) ? "true":"false");
		TempNode2.addAttribute("turnSwitchOffID", QString::number(action->turnSwitchOffID).toUtf8().data());
		TempNode2.addAttribute("monsterAnimationID", QString::number(action->monsterAnimationID).toUtf8().data());
	}
	TempNode1 = MonsterNode.addChild("battleEvent");
	TempNode1 = MonsterNode.addChild("customAI");
	TODO("Uncomment the following lines when the battle animations are ready");
	//TempNode1 = battleEvent->getEventNode();
	//TempNode1.updateName("battleEvent");
	//MonsterNode.addChild(TempNode1);
	//TempNode1 = customAI->getEventNode();
	//TempNode1.updateName("customAI");
	//MonsterNode.addChild(TempNode1);
	return MonsterNode;
}

BattleEvent *Monster::getBattleEventPointer()
{
	return battleEvent;
}

BattleEvent *Monster::getCustomAIPointer()
{
	return customAI;
}

QString Monster::getName()
{
	return name;
}

void Monster::setName(QString value)
{
	name = value;
}

void Monster::getItemDropped(int itemID, ItemDrop *item)
{
	item->itemID = itemsDropped[itemID].itemID;
	item->priority = itemsDropped[itemID].priority;
}

int Monster::numItemsDropped()
{
	return itemsDropped.size();
}

void Monster::setItemDropped(int itemID, ItemDrop &item)
{
	itemsDropped[itemID].itemID = item.itemID;
	itemsDropped[itemID].priority = item.priority;
}

void Monster::addItemDropped(ItemDrop item)
{
	itemsDropped.append(item);
}

void Monster::deleteItemDropped(int itemID)
{
	itemsDropped.removeAt(itemID);
}

void Monster::getBehaviorAction(int actionID, BehaviorAction *value)
{
	value->preconditionType = behaviorActions[actionID].preconditionType;
	value->priority = behaviorActions[actionID].priority;
	value->preconditionValue1 = behaviorActions[actionID].preconditionValue1;
	value->preconditionValue2 = behaviorActions[actionID].preconditionValue2;
	value->actionType = behaviorActions[actionID].actionType;
	value->actionID = behaviorActions[actionID].actionID;
	value->turnSwitchOnID = behaviorActions[actionID].turnSwitchOnID;
	value->turnSwitchOffID = behaviorActions[actionID].turnSwitchOffID;
	value->monsterAnimationID = behaviorActions[actionID].monsterAnimationID;
	value->turnSwitchOn = behaviorActions[actionID].turnSwitchOn;
	value->turnSwitchOff = behaviorActions[actionID].turnSwitchOff;
}

void Monster::setBehaviorAction(int actionID, BehaviorAction &value)
{
	behaviorActions[actionID].preconditionType = value.preconditionType;
	behaviorActions[actionID].priority = value.priority;
	behaviorActions[actionID].preconditionValue1 = value.preconditionValue1;
	behaviorActions[actionID].preconditionValue2 = value.preconditionValue2;
	behaviorActions[actionID].actionType = value.actionType;
	behaviorActions[actionID].actionID = value.actionID;
	behaviorActions[actionID].turnSwitchOnID = value.turnSwitchOnID;
	behaviorActions[actionID].turnSwitchOffID = value.turnSwitchOffID;
	behaviorActions[actionID].monsterAnimationID = value.monsterAnimationID;
	behaviorActions[actionID].turnSwitchOn = value.turnSwitchOn;
	behaviorActions[actionID].turnSwitchOff = value.turnSwitchOff;
}

void Monster::addBehaviorAction(BehaviorAction value)
{
	behaviorActions.append(value);
}

int Monster::numBehaviorActions()
{
	return behaviorActions.size();
}

void Monster::deleteBehaviorAction(int actionID)
{
	behaviorActions.removeAt(actionID);
}

int Monster::getConditionResist(int conditionID)
{
	return conditionResists[conditionID];
}

void Monster::setConditionResist(int conditionID, int value)
{
	conditionResists[conditionID] = value;
}

void Monster::resizeConditionResists(int numConditions)
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

int Monster::getAttributeResist(int attributeID)
{
	return attributeResists[attributeID];
}

void Monster::setAttributeResist(int attributeID, int value)
{
	attributeResists[attributeID] = value;
}

void Monster::resizeAttributeResists(int numAttributes)
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

float Monster::getCriticalHitRate()
{
	return criticalHitRate;
}

void Monster::setCriticalHitRate(float value)
{
	criticalHitRate = value;
}

float Monster::getCriticalHitMultiplier()
{
	return criticalHitMultiplier;
}

void Monster::setCriticalHitMultiplier(float value)
{
	criticalHitMultiplier = value;
}

int Monster::getMaxHP()
{
	return maxHP;
}

void Monster::setMaxHP(int value)
{
	maxHP = value;
}

int Monster::getMaxMP()
{
	return maxMP;
}

void Monster::setMaxMP(int value)
{
	maxMP = value;
}

int Monster::getAttack()
{
	return attack;
}

void Monster::setAttack(int value)
{
	attack = value;
}

int Monster::getDefense()
{
	return defense;
}

void Monster::setDefense(int value)
{
	defense = value;
}

int Monster::getWisdom()
{
	return wisdom;
}

void Monster::setWisdom(int value)
{
	wisdom = value;
}

int Monster::getAgility()
{
	return agility;
}

void Monster::setAgility(int value)
{
	agility = value;
}

int Monster::getExperienceGain()
{
	return experienceGain;
}

void Monster::setExperienceGain(int value)
{
	experienceGain = value;
}

int Monster::getMoneyAquired()
{
	return moneyAquired;
}

void Monster::setMoneyAquired(int value)
{
	moneyAquired = value;
}

int Monster::getItemDropProbability()
{
	return itemDropProbability;
}

void Monster::setItemDropProbability(int value)
{
	itemDropProbability = value;
}

int Monster::getMonsterAnimationID()
{
	return monsterAnimationID;
}

void Monster::setMonsterAnimationID(int value)
{
	monsterAnimationID = value;
}

int Monster::getHueShift()
{
	return hueShift;
}

void Monster::setHueShift(int value)
{
	hueShift = value;
}

int Monster::getAILevel()
{
	return aiLevel;
}

void Monster::setAILevel(int value)
{
	aiLevel = value;
}

bool Monster::isTransparent()
{
	return transparent;
}

void Monster::setIsTransparent(bool value)
{
	transparent = value;
}

bool Monster::isFlying()
{
	return flying;
}

void Monster::setIsFlying(bool value)
{
	flying = value;
}

bool Monster::isCriticalHits()
{
	return criticalHits;
}

void Monster::setCriticalHits(bool value)
{
	criticalHits = value;
}

bool Monster::getAttacksOftenMiss()
{
	return attacksOftenMiss;
}

void Monster::setAttacksOftenMiss(bool value)
{
	attacksOftenMiss = value;
}

QString Monster::getStrongAgainst()
{
	return "None";
}

QString Monster::getWeakAgainst()
{
	return "None";
}

bool Monster::tryLoad(XMLNode MonsterNode)
{
	XMLNode TempNode1, TempNode2;
	QStringList tempStringList;
	QStringList trueFalseList;
	QString temp_string;
	trueFalseList << "true";
	trueFalseList << "false";
	if (!XMLTools::attributeExists(MonsterNode, "name"))
		return false;
	if (!XMLTools::attributeExists(MonsterNode, "attacksOftenMiss"))
		return false;
	if (!XMLTools::attributeStringValid(MonsterNode, "attacksOftenMiss", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(MonsterNode, "aiLevel"))
		return false;
	if (!XMLTools::attributeIntRangeValid(MonsterNode, "aiLevel", 0, 3))
		return false;
	if (!XMLTools::nodeExists(MonsterNode, "graphic"))
		return false;
	TempNode1 = MonsterNode.getChildNode("graphic");
	if (!XMLTools::attributeExists(TempNode1, "monsterAnimationID"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "transparent"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "transparent", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "flying"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "flying", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "hueShift"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "hueShift", 0, 359))
		return false;
	if (!XMLTools::nodeExists(MonsterNode, "stats"))
		return false;
	TempNode1 = MonsterNode.getChildNode("stats");
	if (!XMLTools::attributeExists(TempNode1, "maxHP"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "maxHP", 1, 99999))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "maxMP"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "maxMP", 1, 9999))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "attack"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "attack", 1, 999))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "defense"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "defense", 1, 999))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "wisdom"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "wisdom", 1, 999))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "agility"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode1, "agility", 1, 999))
		return false;
	if (!XMLTools::nodeExists(MonsterNode, "conditionResists"))
		return false;
	temp_string = MonsterNode.getChildNode("conditionResists").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		QChar value = temp_string[i].toUpper();
		if (value != QChar('A') && value != QChar('B') && value != QChar('C') && value != QChar('D') && value != QChar('E'))
			QMessageBox::warning(0, "Warning: Invalid Data", QString("Resistance for condition %1 must be A, B, C, D, or E.\nDefaulting to E.").arg(i));
	}
	if (!XMLTools::nodeExists(MonsterNode, "attributeResists"))
		return false;
	temp_string = MonsterNode.getChildNode("attributeResists").getText();
	for (int i = 0; i < temp_string.size(); ++i)
	{
		QChar value = temp_string[i].toUpper();
		if (value != QChar('A') && value != QChar('B') && value != QChar('C') && value != QChar('D') && value != QChar('E'))
			QMessageBox::warning(0, "Warning: Invalid Data", QString("Resistance for attribute %1 must be A, B, C, D, or E.\nDefaulting to E.").arg(i));
	}
	if (!XMLTools::nodeExists(MonsterNode, "criticalHits"))
		return false;
	TempNode1 = MonsterNode.getChildNode("criticalHits");
	if (!XMLTools::attributeExists(TempNode1, "hasCriticalHits"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode1, "hasCriticalHits", trueFalseList, 1))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "rate"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "multiplier"))
		return false;
	if (!XMLTools::nodeExists(MonsterNode, "spoils"))
		return false;
	TempNode1 = MonsterNode.getChildNode("spoils");
	if (!XMLTools::attributeExists(TempNode1, "experienceGain"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "moneyAquired"))
		return false;
	if (!XMLTools::attributeExists(TempNode1, "itemDropProbability"))
		return false;
	for (int i = 0, count = TempNode1.nChildNode("itemDropped"); i < count; ++i)
	{
		TempNode2 = TempNode1.getChildNode("itemDropped", i);
		if (!XMLTools::attributeExists(TempNode2, "itemID"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "priority"))
			return false;
	}
	if (!XMLTools::nodeExists(MonsterNode, "behavior"))
		return false;
	TempNode1 = MonsterNode.getChildNode("behavior");
	for (int i = 0, count = TempNode1.nChildNode("action"); i < count; ++i)
	{
		TempNode2 = TempNode1.getChildNode("action", i);
		if (!XMLTools::attributeExists(TempNode2, "precondition"))
			return false;
		tempStringList << "none";
		tempStringList << "switch";
		tempStringList << "turnselapsed";
		tempStringList << "monsterspresent";
		tempStringList << "monstershp";
		tempStringList << "monstersmp";
		tempStringList << "partysaveragelevel";
		tempStringList << "partysexhaustion";
		if (!XMLTools::attributeStringValid(TempNode2, "precondition", tempStringList, 0))
			return false;
		tempStringList.clear();
		if (!XMLTools::attributeExists(TempNode2, "priority"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "preconditionValue1"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "preconditionValue2"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "actionType"))
			return false;
		tempStringList << "basic";
		tempStringList << "skill";
		tempStringList << "transform";
		if (!XMLTools::attributeStringValid(TempNode2, "actionType", tempStringList, 0))
			return false;
		tempStringList.clear();
		if (!XMLTools::attributeExists(TempNode2, "turnSwitchOn"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode2, "turnSwitchOn", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "turnSwitchOnID"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "turnSwitchOff"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode2, "turnSwitchOff", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "turnSwitchOffID"))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "monsterAnimationID"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode2, "monsterAnimationID", 0, 31))
			return false;
	}
	if (!XMLTools::nodeExists(MonsterNode, "battleEvent"))
		return false;
	TODO("Uncomment the following lines when the battle events are ready");
	//if (!BattleEvent::tryLoad(MonsterNode.getChildNode("battleEvent")))
	//	return false;
	if (!XMLTools::nodeExists(MonsterNode, "customAI"))
		return false;
	//if (!BattleEvent::tryLoad(MonsterNode.getChildNode("customAI")))
	//	return false;
	return true;
}
