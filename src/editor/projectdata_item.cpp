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
#include "projectdata_item.h"
#include "xmlParser.h"
#include "xmlTools.h"

Item::Item()
{
	name = "Untitled";
	description = "No description";
	weaponData = NULL;
	equipmentData = NULL;
	medicineData = NULL;
	skillBookData = NULL;
	seedData = NULL;
	skillScrollData = NULL;
	switchData = NULL;
	eventData = NULL;
	price = 0;
	type = TYPE_COMMONITEM;
}

Item::Item(Item &other)
{
	name = other.name;
	description = other.description;
	price = other.price;
	type = other.type;
	if (type == TYPE_WEAPON)
	{
		weaponData = new WeaponItem;
		for (int i = 0; i < other.weaponData->usableBy.size(); ++i)
			weaponData->usableBy.append(other.weaponData->usableBy[i]);
		for (int i = 0; i < other.weaponData->attribute.size(); ++i)
			weaponData->attribute.append(other.weaponData->attribute[i]);
		for (int i = 0; i < other.weaponData->conditions.size(); ++i)
			weaponData->conditions.append(other.weaponData->conditions[i]);
		weaponData->animationSettings.movementBeforeAttack = other.weaponData->animationSettings.movementBeforeAttack;
		weaponData->animationSettings.numberOfAttacks = other.weaponData->animationSettings.numberOfAttacks;
		weaponData->animationSettings.animationID = other.weaponData->animationSettings.animationID;
		weaponData->animationSettings.rangedWeaponAnimationID = other.weaponData->animationSettings.rangedWeaponAnimationID;
		weaponData->animationSettings.trajectory = other.weaponData->animationSettings.trajectory;
		weaponData->animationSettings.targetting = other.weaponData->animationSettings.targetting;
		weaponData->animationSettings.animationSpeed = other.weaponData->animationSettings.animationSpeed;
		weaponData->animationSettings.weaponAnimation = other.weaponData->animationSettings.weaponAnimation;
		weaponData->animationSettings.afterImage = other.weaponData->animationSettings.afterImage;
		weaponData->animationSettings.rangedWeapon = other.weaponData->animationSettings.rangedWeapon;
		weaponData->skillID = other.weaponData->skillID;
		weaponData->attackChange = other.weaponData->attackChange;
		weaponData->defenseChange = other.weaponData->defenseChange;
		weaponData->wisdomChange = other.weaponData->wisdomChange;
		weaponData->agilityChange = other.weaponData->agilityChange;
		weaponData->battleAnimationID = other.weaponData->battleAnimationID;
		weaponData->amountMPConsumed = other.weaponData->amountMPConsumed;
		weaponData->percentHitProbability = other.weaponData->percentHitProbability;
		weaponData->percentCriticalBonus = other.weaponData->percentCriticalBonus;
		weaponData->percentConditionEffectivenessProbability = other.weaponData->percentConditionEffectivenessProbability;
		weaponData->twoHanded = other.weaponData->twoHanded;
		weaponData->alwaysAttackFirst = other.weaponData->alwaysAttackFirst;
		weaponData->attackTwice = other.weaponData->attackTwice;
		weaponData->attackAllEnemies = other.weaponData->attackAllEnemies;
		weaponData->ignoreEnemyEvasion = other.weaponData->ignoreEnemyEvasion;
		weaponData->cursed = other.weaponData->cursed;
		weaponData->usableOnTheDead = other.weaponData->usableOnTheDead;
		weaponData->inflictConditions = other.weaponData->inflictConditions;
		weaponData->usableByCharacters = other.weaponData->usableByCharacters;
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		equipmentData = new EquipmentItem;
		for (int i = 0; i < other.equipmentData->usableBy.size(); ++i)
			equipmentData->usableBy.append(other.equipmentData->usableBy[i]);
		for (int i = 0; i < other.equipmentData->attributeResistance.size(); ++i)
			equipmentData->attributeResistance.append(other.equipmentData->attributeResistance[i]);
		for (int i = 0; i < other.equipmentData->conditions.size(); ++i)
			equipmentData->conditions.append(other.equipmentData->conditions[i]);
		equipmentData->skillID = other.equipmentData->skillID;
		equipmentData->attackChange = other.equipmentData->attackChange;
		equipmentData->defenseChange = other.equipmentData->defenseChange;
		equipmentData->wisdomChange = other.equipmentData->wisdomChange;
		equipmentData->agilityChange = other.equipmentData->agilityChange;
		equipmentData->percentConditionResistanceEffectiveness = other.equipmentData->percentConditionResistanceEffectiveness;
		equipmentData->preventCriticalHits = other.equipmentData->preventCriticalHits;
		equipmentData->increaseEvasion = other.equipmentData->increaseEvasion;
		equipmentData->halfMPConsumption = other.equipmentData->halfMPConsumption;
		equipmentData->noTerrainDamage = other.equipmentData->noTerrainDamage;
		equipmentData->cursed = other.equipmentData->cursed;
		equipmentData->resistConditions = other.equipmentData->resistConditions;
		equipmentData->usableByCharacters = other.equipmentData->usableByCharacters;
	}
	else if (type == TYPE_MEDICINE)
	{
		medicineData = new MedicineItem;
		for (int i = 0; i < other.medicineData->usableBy.size(); ++i)
			medicineData->usableBy.append(other.medicineData->usableBy[i]);
		for (int i = 0; i < other.medicineData->cureCondition.size(); ++i)
			medicineData->cureCondition.append(other.medicineData->cureCondition[i]);
		medicineData->numberOfUses = other.medicineData->numberOfUses;
		medicineData->percentRestoredHP = other.medicineData->percentRestoredHP;
		medicineData->amountRestoredHP = other.medicineData->amountRestoredHP;
		medicineData->percentRestoredMP = other.medicineData->percentRestoredMP;
		medicineData->amountRestoredMP = other.medicineData->amountRestoredMP;
		medicineData->targetSingleAlly = other.medicineData->targetSingleAlly;
		medicineData->usableByCharacters = other.medicineData->usableByCharacters;
		medicineData->onlyUsableOnFieldMap = other.medicineData->onlyUsableOnFieldMap;
		medicineData->onlyUsableOnDeadCharacters = other.medicineData->onlyUsableOnDeadCharacters;
	}
	else if (type == TYPE_SKILLBOOK)
	{
		skillBookData = new SkillBookItem;
		for (int i = 0; i < other.skillBookData->usableBy.size(); ++i)
			skillBookData->usableBy.append(other.skillBookData->usableBy[i]);
		skillBookData->numberOfUses = other.skillBookData->numberOfUses;
		skillBookData->skillID = other.skillBookData->skillID;
		skillBookData->usableByCharacters = other.skillBookData->usableByCharacters;
	}
	else if (type == TYPE_SEED)
	{
		seedData = new SeedItem;
		for (int i = 0; i < other.seedData->usableBy.size(); ++i)
			seedData->usableBy.append(other.seedData->usableBy[i]);
		seedData->numberOfUses = other.seedData->numberOfUses;
		seedData->maxHPChange = other.seedData->maxHPChange;
		seedData->maxMPChange = other.seedData->maxMPChange;
		seedData->attackChange = other.seedData->attackChange;
		seedData->defenseChange = other.seedData->defenseChange;
		seedData->wisdomChange = other.seedData->wisdomChange;
		seedData->agilityChange = other.seedData->agilityChange;
		seedData->usableByCharacters = other.seedData->usableByCharacters;
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		skillScrollData = new SkillScrollItem;
		for (int i = 0; i < other.skillScrollData->usableBy.size(); ++i)
			skillScrollData->usableBy.append(other.skillScrollData->usableBy[i]);
		skillScrollData->numberOfUses = other.skillScrollData->numberOfUses;
		skillScrollData->skillID = other.skillScrollData->skillID;
		skillScrollData->showItemNameMessageWhenUsed = other.skillScrollData->showItemNameMessageWhenUsed;
		skillScrollData->usableByCharacters = other.skillScrollData->usableByCharacters;
	}
	else if (type == TYPE_SWITCH)
	{
		switchData = new SwitchItem;
		switchData->numberOfUses = other.switchData->numberOfUses;
		switchData->switchID = other.switchData->switchID;
		switchData->allowFieldActivation = other.switchData->allowFieldActivation;
		switchData->allowBattleActivation = other.switchData->allowBattleActivation;
	}
	else if (type == TYPE_EVENT)
	{
		eventData = new EventItem;
		TODO("Uncomment the following line when the battle events are ready");
		//eventData->event = new Event(other.eventData->event);
		eventData->numberOfUses = other.eventData->numberOfUses;
		eventData->allowFieldActivation = other.eventData->allowFieldActivation;
		eventData->allowBattleActivation = other.eventData->allowBattleActivation;
	}
}

Item::Item(const Item &other)
{
	name = other.name;
	description = other.description;
	price = other.price;
	type = other.type;
	if (type == TYPE_WEAPON)
	{
		weaponData = new WeaponItem;
		for (int i = 0; i < other.weaponData->usableBy.size(); ++i)
			weaponData->usableBy.append(other.weaponData->usableBy[i]);
		for (int i = 0; i < other.weaponData->attribute.size(); ++i)
			weaponData->attribute.append(other.weaponData->attribute[i]);
		for (int i = 0; i < other.weaponData->conditions.size(); ++i)
			weaponData->conditions.append(other.weaponData->conditions[i]);
		weaponData->animationSettings.movementBeforeAttack = other.weaponData->animationSettings.movementBeforeAttack;
		weaponData->animationSettings.numberOfAttacks = other.weaponData->animationSettings.numberOfAttacks;
		weaponData->animationSettings.animationID = other.weaponData->animationSettings.animationID;
		weaponData->animationSettings.rangedWeaponAnimationID = other.weaponData->animationSettings.rangedWeaponAnimationID;
		weaponData->animationSettings.trajectory = other.weaponData->animationSettings.trajectory;
		weaponData->animationSettings.targetting = other.weaponData->animationSettings.targetting;
		weaponData->animationSettings.animationSpeed = other.weaponData->animationSettings.animationSpeed;
		weaponData->animationSettings.weaponAnimation = other.weaponData->animationSettings.weaponAnimation;
		weaponData->animationSettings.afterImage = other.weaponData->animationSettings.afterImage;
		weaponData->animationSettings.rangedWeapon = other.weaponData->animationSettings.rangedWeapon;
		weaponData->skillID = other.weaponData->skillID;
		weaponData->attackChange = other.weaponData->attackChange;
		weaponData->defenseChange = other.weaponData->defenseChange;
		weaponData->wisdomChange = other.weaponData->wisdomChange;
		weaponData->agilityChange = other.weaponData->agilityChange;
		weaponData->battleAnimationID = other.weaponData->battleAnimationID;
		weaponData->amountMPConsumed = other.weaponData->amountMPConsumed;
		weaponData->percentHitProbability = other.weaponData->percentHitProbability;
		weaponData->percentCriticalBonus = other.weaponData->percentCriticalBonus;
		weaponData->percentConditionEffectivenessProbability = other.weaponData->percentConditionEffectivenessProbability;
		weaponData->twoHanded = other.weaponData->twoHanded;
		weaponData->alwaysAttackFirst = other.weaponData->alwaysAttackFirst;
		weaponData->attackTwice = other.weaponData->attackTwice;
		weaponData->attackAllEnemies = other.weaponData->attackAllEnemies;
		weaponData->ignoreEnemyEvasion = other.weaponData->ignoreEnemyEvasion;
		weaponData->cursed = other.weaponData->cursed;
		weaponData->usableOnTheDead = other.weaponData->usableOnTheDead;
		weaponData->inflictConditions = other.weaponData->inflictConditions;
		weaponData->usableByCharacters = other.weaponData->usableByCharacters;
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		equipmentData = new EquipmentItem;
		for (int i = 0; i < other.equipmentData->usableBy.size(); ++i)
			equipmentData->usableBy.append(other.equipmentData->usableBy[i]);
		for (int i = 0; i < other.equipmentData->attributeResistance.size(); ++i)
			equipmentData->attributeResistance.append(other.equipmentData->attributeResistance[i]);
		for (int i = 0; i < other.equipmentData->conditions.size(); ++i)
			equipmentData->conditions.append(other.equipmentData->conditions[i]);
		equipmentData->skillID = other.equipmentData->skillID;
		equipmentData->attackChange = other.equipmentData->attackChange;
		equipmentData->defenseChange = other.equipmentData->defenseChange;
		equipmentData->wisdomChange = other.equipmentData->wisdomChange;
		equipmentData->agilityChange = other.equipmentData->agilityChange;
		equipmentData->percentConditionResistanceEffectiveness = other.equipmentData->percentConditionResistanceEffectiveness;
		equipmentData->preventCriticalHits = other.equipmentData->preventCriticalHits;
		equipmentData->increaseEvasion = other.equipmentData->increaseEvasion;
		equipmentData->halfMPConsumption = other.equipmentData->halfMPConsumption;
		equipmentData->noTerrainDamage = other.equipmentData->noTerrainDamage;
		equipmentData->cursed = other.equipmentData->cursed;
		equipmentData->resistConditions = other.equipmentData->resistConditions;
		equipmentData->usableByCharacters = other.equipmentData->usableByCharacters;
	}
	else if (type == TYPE_MEDICINE)
	{
		medicineData = new MedicineItem;
		for (int i = 0; i < other.medicineData->usableBy.size(); ++i)
			medicineData->usableBy.append(other.medicineData->usableBy[i]);
		for (int i = 0; i < other.medicineData->cureCondition.size(); ++i)
			medicineData->cureCondition.append(other.medicineData->cureCondition[i]);
		medicineData->numberOfUses = other.medicineData->numberOfUses;
		medicineData->percentRestoredHP = other.medicineData->percentRestoredHP;
		medicineData->amountRestoredHP = other.medicineData->amountRestoredHP;
		medicineData->percentRestoredMP = other.medicineData->percentRestoredMP;
		medicineData->amountRestoredMP = other.medicineData->amountRestoredMP;
		medicineData->targetSingleAlly = other.medicineData->targetSingleAlly;
		medicineData->usableByCharacters = other.medicineData->usableByCharacters;
		medicineData->onlyUsableOnFieldMap = other.medicineData->onlyUsableOnFieldMap;
		medicineData->onlyUsableOnDeadCharacters = other.medicineData->onlyUsableOnDeadCharacters;
	}
	else if (type == TYPE_SKILLBOOK)
	{
		skillBookData = new SkillBookItem;
		for (int i = 0; i < other.skillBookData->usableBy.size(); ++i)
			skillBookData->usableBy.append(other.skillBookData->usableBy[i]);
		skillBookData->numberOfUses = other.skillBookData->numberOfUses;
		skillBookData->skillID = other.skillBookData->skillID;
		skillBookData->usableByCharacters = other.skillBookData->usableByCharacters;
	}
	else if (type == TYPE_SEED)
	{
		seedData = new SeedItem;
		for (int i = 0; i < other.seedData->usableBy.size(); ++i)
			seedData->usableBy.append(other.seedData->usableBy[i]);
		seedData->numberOfUses = other.seedData->numberOfUses;
		seedData->maxHPChange = other.seedData->maxHPChange;
		seedData->maxMPChange = other.seedData->maxMPChange;
		seedData->attackChange = other.seedData->attackChange;
		seedData->defenseChange = other.seedData->defenseChange;
		seedData->wisdomChange = other.seedData->wisdomChange;
		seedData->agilityChange = other.seedData->agilityChange;
		seedData->usableByCharacters = other.seedData->usableByCharacters;
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		skillScrollData = new SkillScrollItem;
		for (int i = 0; i < other.skillScrollData->usableBy.size(); ++i)
			skillScrollData->usableBy.append(other.skillScrollData->usableBy[i]);
		skillScrollData->numberOfUses = other.skillScrollData->numberOfUses;
		skillScrollData->skillID = other.skillScrollData->skillID;
		skillScrollData->showItemNameMessageWhenUsed = other.skillScrollData->showItemNameMessageWhenUsed;
		skillScrollData->usableByCharacters = other.skillScrollData->usableByCharacters;
	}
	else if (type == TYPE_SWITCH)
	{
		switchData = new SwitchItem;
		switchData->numberOfUses = other.switchData->numberOfUses;
		switchData->switchID = other.switchData->switchID;
		switchData->allowFieldActivation = other.switchData->allowFieldActivation;
		switchData->allowBattleActivation = other.switchData->allowBattleActivation;
	}
	else if (type == TYPE_EVENT)
	{
		eventData = new EventItem;
		TODO("Uncomment the following line when the battle events are ready");
		//eventData->event = new Event(other.eventData->event);
		eventData->numberOfUses = other.eventData->numberOfUses;
		eventData->allowFieldActivation = other.eventData->allowFieldActivation;
		eventData->allowBattleActivation = other.eventData->allowBattleActivation;
	}
}

Item::Item(XMLNode ItemNode)
{
	XMLNode TempNode;
	QStringList tempStringList;
	QString temp_string;
	name = ItemNode.getAttribute("name");
	description = ItemNode.getAttribute("description");
	price = QString(ItemNode.getAttribute("price")).toInt();
	temp_string = ItemNode.getAttribute("type");
	if (temp_string == "commonitem")
		type = TYPE_COMMONITEM;
	if (temp_string == "weapon")
		type = TYPE_WEAPON;
	if (temp_string == "shield")
		type = TYPE_SHIELD;
	if (temp_string == "armor")
		type = TYPE_ARMOR;
	if (temp_string == "helmet")
		type = TYPE_HELMET;
	if (temp_string == "accessory")
		type = TYPE_ACCESSORY;
	if (temp_string == "medicine")
		type = TYPE_MEDICINE;
	if (temp_string == "skillbook")
		type = TYPE_SKILLBOOK;
	if (temp_string == "seed")
		type = TYPE_SEED;
	if (temp_string == "skillscroll")
		type = TYPE_SKILLSCROLL;
	if (temp_string == "switch")
		type = TYPE_SWITCH;
	if (temp_string == "event")
		type = TYPE_EVENT;
	if (type == TYPE_WEAPON)
	{
		weaponData = new WeaponItem;
		TempNode = ItemNode.getChildNode("usableBy");
		temp_string = TempNode.getAttribute("usableBy");
		if (temp_string == "heroes")
			weaponData->usableByCharacters = true;
		else
			weaponData->usableByCharacters = false;
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			weaponData->usableBy.append((temp_string[i].digitValue() == 1) ? true:false);
		TempNode = ItemNode.getChildNode("attributes");
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			weaponData->attribute.append((temp_string[i].digitValue() == 1) ? true:false);
		TempNode = ItemNode.getChildNode("conditions");
		temp_string = TempNode.getAttribute("inflictConditions");
		if (temp_string == "true")
			weaponData->inflictConditions = true;
		else
			weaponData->inflictConditions = false;
		weaponData->percentConditionEffectivenessProbability = QString(TempNode.getAttribute("effectivenessProbability")).toInt();
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			weaponData->conditions.append((temp_string[i].digitValue() == 1) ? true:false);
		weaponData->skillID = QString(ItemNode.getAttribute("skillID")).toInt();
		TempNode = ItemNode.getChildNode("statChange");
		weaponData->attackChange = QString(TempNode.getAttribute("attack")).toInt();
		weaponData->defenseChange = QString(TempNode.getAttribute("defense")).toInt();
		weaponData->wisdomChange = QString(TempNode.getAttribute("wisdom")).toInt();
		weaponData->agilityChange = QString(TempNode.getAttribute("agility")).toInt();
		TempNode = ItemNode.getChildNode("animation");
		temp_string = TempNode.getAttribute("movementBeforeAttack");
		if (temp_string == "none")
			weaponData->animationSettings.movementBeforeAttack = MOVEMENT_NONE;
		else if (temp_string == "stepforward")
			weaponData->animationSettings.movementBeforeAttack = MOVEMENT_STEPFORWARD;
		else if (temp_string == "jumpforward")
			weaponData->animationSettings.movementBeforeAttack = MOVEMENT_JUMPFORWARD;
		else
			weaponData->animationSettings.movementBeforeAttack = MOVEMENT_MOVETOTARGET;
		weaponData->animationSettings.numberOfAttacks = QString(TempNode.getAttribute("numberOfAttacks")).toInt();
		weaponData->animationSettings.animationID = QString(TempNode.getAttribute("animationID")).toInt();
		weaponData->animationSettings.rangedWeaponAnimationID = QString(TempNode.getAttribute("rangedWeaponAnimationID")).toInt();
		temp_string = TempNode.getAttribute("trajectory");
		if (temp_string == "flystraighttotarget")
			weaponData->animationSettings.trajectory = TRAJECTORY_FLYSTRAIGHTTOTARGET;
		else
			weaponData->animationSettings.trajectory = TRAJECTORY_RETURNAFTERSTRIKING;
		temp_string = TempNode.getAttribute("targetting");
		if (temp_string == "single")
			weaponData->animationSettings.targetting = TARGET_SINGLE;
		else if (temp_string == "flydowncenterofscreen")
			weaponData->animationSettings.targetting = TARGET_FLYDOWNCENTEROFSCREEN;
		else if (temp_string == "strikeeachsimultaneously")
			weaponData->animationSettings.targetting = TARGET_STRIKEEACHSIMULTANEOUSLY;
		else
			weaponData->animationSettings.targetting = TARGET_STRIKEEACHINTURN;
		temp_string = TempNode.getAttribute("animationSpeed");
		if (temp_string == "fast")
			weaponData->animationSettings.animationSpeed = SPEED_FAST;
		else if (temp_string == "medium")
			weaponData->animationSettings.animationSpeed = SPEED_MEDIUM;
		else
			weaponData->animationSettings.animationSpeed = SPEED_SLOW;
		temp_string = TempNode.getAttribute("weaponAnimation");
		if (temp_string == "true")
			weaponData->animationSettings.weaponAnimation = true;
		else
			weaponData->animationSettings.weaponAnimation = false;
		temp_string = TempNode.getAttribute("afterImage");
		if (temp_string == "true")
			weaponData->animationSettings.afterImage = true;
		else
			weaponData->animationSettings.afterImage = false;
		temp_string = TempNode.getAttribute("rangedWeapon");
		if (temp_string == "true")
			weaponData->animationSettings.rangedWeapon = true;
		else
			weaponData->animationSettings.rangedWeapon = false;
		weaponData->battleAnimationID = QString(TempNode.getAttribute("battleAnimationID")).toInt();
		TempNode = ItemNode.getChildNode("options");
		weaponData->amountMPConsumed = QString(TempNode.getAttribute("mpConsumed")).toInt();
		weaponData->percentHitProbability = QString(TempNode.getAttribute("hitProbability")).toInt();
		temp_string = TempNode.getAttribute("twoHanded");
		if (temp_string == "true")
			weaponData->twoHanded = true;
		else
			weaponData->twoHanded = false;
		temp_string = TempNode.getAttribute("alwaysAttackFirst");
		if (temp_string == "true")
			weaponData->alwaysAttackFirst = true;
		else
			weaponData->alwaysAttackFirst = false;
		temp_string = TempNode.getAttribute("attackTwice");
		if (temp_string == "true")
			weaponData->attackTwice = true;
		else
			weaponData->attackTwice = false;
		temp_string = TempNode.getAttribute("attackAllEnemies");
		if (temp_string == "true")
			weaponData->attackAllEnemies = true;
		else
			weaponData->attackAllEnemies = false;
		temp_string = TempNode.getAttribute("ignoreEnemyEvasion");
		if (temp_string == "true")
			weaponData->ignoreEnemyEvasion = true;
		else
			weaponData->ignoreEnemyEvasion = false;
		temp_string = TempNode.getAttribute("cursed");
		if (temp_string == "true")
			weaponData->cursed = true;
		else
			weaponData->cursed = false;
		temp_string = TempNode.getAttribute("usableOnTheDead");
		if (temp_string == "true")
			weaponData->usableOnTheDead = true;
		else
			weaponData->usableOnTheDead = false;
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		equipmentData = new EquipmentItem;
		TempNode = ItemNode.getChildNode("usableBy");
		temp_string = TempNode.getAttribute("usableBy");
		if (temp_string == "heroes")
			equipmentData->usableByCharacters = true;
		else
			equipmentData->usableByCharacters = false;
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			equipmentData->usableBy.append((temp_string[i].digitValue() == 1) ? true:false);
		TempNode = ItemNode.getChildNode("attributeResistance");
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			equipmentData->attributeResistance.append((temp_string[i].digitValue() == 1) ? true:false);
		TempNode = ItemNode.getChildNode("conditions");
		temp_string = TempNode.getAttribute("resistConditions");
		if (temp_string == "true")
			equipmentData->resistConditions = true;
		else
			equipmentData->resistConditions = false;
		equipmentData->percentConditionResistanceEffectiveness = QString(TempNode.getAttribute("resistanceEffectiveness")).toInt();
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			equipmentData->conditions.append((temp_string[i].digitValue() == 1) ? true:false);
		equipmentData->skillID = QString(ItemNode.getAttribute("skillID")).toInt();
		TempNode = ItemNode.getChildNode("statChange");
		equipmentData->attackChange = QString(TempNode.getAttribute("attack")).toInt();
		equipmentData->defenseChange = QString(TempNode.getAttribute("defense")).toInt();
		equipmentData->wisdomChange = QString(TempNode.getAttribute("wisdom")).toInt();
		equipmentData->agilityChange = QString(TempNode.getAttribute("agility")).toInt();
		TempNode = ItemNode.getChildNode("options");
		temp_string = TempNode.getAttribute("preventCriticalHits");
		if (temp_string == "true")
			equipmentData->preventCriticalHits = true;
		else
			equipmentData->preventCriticalHits = false;
		temp_string = TempNode.getAttribute("increaseEvasion");
		if (temp_string == "true")
			equipmentData->increaseEvasion = true;
		else
			equipmentData->increaseEvasion = false;
		temp_string = TempNode.getAttribute("halfMPConsumption");
		if (temp_string == "true")
			equipmentData->halfMPConsumption = true;
		else
			equipmentData->halfMPConsumption = false;
		temp_string = TempNode.getAttribute("noTerrainDamage");
		if (temp_string == "true")
			equipmentData->noTerrainDamage = true;
		else
			equipmentData->noTerrainDamage = false;
		temp_string = TempNode.getAttribute("cursed");
		if (temp_string == "true")
			equipmentData->cursed = true;
		else
			equipmentData->cursed = false;
	}
	else if (type == TYPE_MEDICINE)
	{
		medicineData = new MedicineItem;
		TempNode = ItemNode.getChildNode("usableBy");
		temp_string = TempNode.getAttribute("usableBy");
		if (temp_string == "heroes")
			medicineData->usableByCharacters = true;
		else
			medicineData->usableByCharacters = false;
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			medicineData->usableBy.append((temp_string[i].digitValue() == 1) ? true:false);
		TempNode = ItemNode.getChildNode("cureCondition");
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			medicineData->cureCondition.append((temp_string[i].digitValue() == 1) ? true:false);
		medicineData->numberOfUses = QString(ItemNode.getAttribute("numberOfUses")).toInt();
		TempNode = ItemNode.getChildNode("options");
		medicineData->percentRestoredHP = QString(TempNode.getAttribute("percentRestoredHP")).toInt();
		medicineData->amountRestoredHP = QString(TempNode.getAttribute("amountRestoredHP")).toInt();
		medicineData->percentRestoredMP = QString(TempNode.getAttribute("percentRestoredMP")).toInt();
		medicineData->amountRestoredMP = QString(TempNode.getAttribute("amountRestoredMP")).toInt();
		temp_string = TempNode.getAttribute("targetsSingleAlly");
		if (temp_string == "true")
			medicineData->targetSingleAlly = true;
		else
			medicineData->targetSingleAlly = false;
		temp_string = TempNode.getAttribute("onlyUsableOnFieldMap");
		if (temp_string == "true")
			medicineData->onlyUsableOnFieldMap = true;
		else
			medicineData->onlyUsableOnFieldMap = false;
		temp_string = TempNode.getAttribute("onlyUsableOnDeadCharacters");
		if (temp_string == "true")
			medicineData->onlyUsableOnDeadCharacters = true;
		else
			medicineData->onlyUsableOnDeadCharacters = false;
	}
	else if (type == TYPE_SKILLBOOK)
	{
		skillBookData = new SkillBookItem;
		TempNode = ItemNode.getChildNode("usableBy");
		temp_string = TempNode.getAttribute("usableBy");
		if (temp_string == "heroes")
			skillBookData->usableByCharacters = true;
		else
			skillBookData->usableByCharacters = false;
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			skillBookData->usableBy.append((temp_string[i].digitValue() == 1) ? true:false);
		skillBookData->numberOfUses = QString(ItemNode.getAttribute("numberOfUses")).toInt();
		skillBookData->skillID = QString(ItemNode.getAttribute("skillID")).toInt();
	}
	else if (type == TYPE_SEED)
	{
		seedData = new SeedItem;
		TempNode = ItemNode.getChildNode("usableBy");
		temp_string = TempNode.getAttribute("usableBy");
		if (temp_string == "heroes")
			seedData->usableByCharacters = true;
		else
			seedData->usableByCharacters = false;
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			seedData->usableBy.append((temp_string[i].digitValue() == 1) ? true:false);
		seedData->numberOfUses = QString(ItemNode.getAttribute("numberOfUses")).toInt();
		TempNode = ItemNode.getChildNode("statChange");
		seedData->maxHPChange = QString(TempNode.getAttribute("maxHP")).toInt();
		seedData->maxMPChange = QString(TempNode.getAttribute("maxMP")).toInt();
		seedData->attackChange = QString(TempNode.getAttribute("attack")).toInt();
		seedData->defenseChange = QString(TempNode.getAttribute("defense")).toInt();
		seedData->wisdomChange = QString(TempNode.getAttribute("wisdom")).toInt();
		seedData->agilityChange = QString(TempNode.getAttribute("agility")).toInt();
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		skillScrollData = new SkillScrollItem;
		TempNode = ItemNode.getChildNode("usableBy");
		temp_string = TempNode.getAttribute("usableBy");
		if (temp_string == "heroes")
			skillScrollData->usableByCharacters = true;
		else
			skillScrollData->usableByCharacters = false;
		temp_string = QString(TempNode.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			skillScrollData->usableBy.append((temp_string[i].digitValue() == 1) ? true:false);
		skillScrollData->numberOfUses = QString(ItemNode.getAttribute("numberOfUses")).toInt();
		skillScrollData->skillID = QString(ItemNode.getAttribute("skillID")).toInt();
		temp_string = ItemNode.getAttribute("showItemNameMessageWhenUsed");
		if (temp_string == "true")
			skillScrollData->showItemNameMessageWhenUsed = true;
		else
			skillScrollData->showItemNameMessageWhenUsed = false;
	}
	else if (type == TYPE_SWITCH)
	{
		switchData = new SwitchItem;
		switchData->numberOfUses = QString(ItemNode.getAttribute("numberOfUses")).toInt();
		switchData->switchID = QString(ItemNode.getAttribute("switchID")).toInt();
		temp_string = ItemNode.getAttribute("allowFieldActivation");
		if (temp_string == "true")
			switchData->allowFieldActivation = true;
		else
			switchData->allowFieldActivation = false;
		temp_string = ItemNode.getAttribute("allowBattleActivation");
		if (temp_string == "true")
			switchData->allowBattleActivation = true;
		else
			switchData->allowBattleActivation = false;
	}
	else if (type == TYPE_EVENT)
	{
		eventData = new EventItem;
		eventData->numberOfUses = QString(ItemNode.getAttribute("numberOfUses")).toInt();
		temp_string = ItemNode.getAttribute("allowFieldActivation");
		if (temp_string == "true")
			eventData->allowFieldActivation = true;
		else
			eventData->allowFieldActivation = false;
		temp_string = ItemNode.getAttribute("allowBattleActivation");
		if (temp_string == "true")
			eventData->allowBattleActivation = true;
		else
			eventData->allowBattleActivation = false;
		TODO("Uncomment the following line when the battle events are ready");
		//eventData->event = new Event(ItemNode.getChildNode("event"));
	}
}

Item::Item(StorageFile &storageFile)
{
	int temp_int;
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	description = temp_string;
	delete[] temp_string;
	price = storageFile.getSignedInt();
	type = storageFile.getSignedInt();
	if (type == TYPE_WEAPON)
	{
		weaponData = new WeaponItem;
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			weaponData->usableBy.append(storageFile.getBool());
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			weaponData->attribute.append(storageFile.getBool());
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			weaponData->conditions.append(storageFile.getBool());
		weaponData->animationSettings.movementBeforeAttack = storageFile.getSignedInt();
		weaponData->animationSettings.numberOfAttacks = storageFile.getSignedInt();
		weaponData->animationSettings.animationID = storageFile.getSignedInt();
		weaponData->animationSettings.rangedWeaponAnimationID = storageFile.getSignedInt();
		weaponData->animationSettings.trajectory = storageFile.getSignedInt();
		weaponData->animationSettings.targetting = storageFile.getSignedInt();
		weaponData->animationSettings.animationSpeed = storageFile.getSignedInt();
		weaponData->animationSettings.weaponAnimation = storageFile.getBool();
		weaponData->animationSettings.afterImage = storageFile.getBool();
		weaponData->animationSettings.rangedWeapon = storageFile.getBool();
		weaponData->skillID = storageFile.getSignedInt();
		weaponData->attackChange = storageFile.getSignedInt();
		weaponData->defenseChange = storageFile.getSignedInt();
		weaponData->wisdomChange = storageFile.getSignedInt();
		weaponData->agilityChange = storageFile.getSignedInt();
		weaponData->battleAnimationID = storageFile.getSignedInt();
		weaponData->amountMPConsumed = storageFile.getSignedInt();
		weaponData->percentHitProbability = storageFile.getSignedInt();
		weaponData->percentCriticalBonus = storageFile.getSignedInt();
		weaponData->percentConditionEffectivenessProbability = storageFile.getSignedInt();
		weaponData->twoHanded = storageFile.getBool();
		weaponData->alwaysAttackFirst = storageFile.getBool();
		weaponData->attackTwice = storageFile.getBool();
		weaponData->attackAllEnemies = storageFile.getBool();
		weaponData->ignoreEnemyEvasion = storageFile.getBool();
		weaponData->cursed = storageFile.getBool();
		weaponData->usableOnTheDead = storageFile.getBool();
		weaponData->inflictConditions = storageFile.getBool();
		weaponData->usableByCharacters = storageFile.getBool();
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		equipmentData = new EquipmentItem;
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			equipmentData->usableBy.append(storageFile.getBool());
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			equipmentData->attributeResistance.append(storageFile.getBool());
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			equipmentData->conditions.append(storageFile.getBool());
		equipmentData->skillID = storageFile.getSignedInt();
		equipmentData->attackChange = storageFile.getSignedInt();
		equipmentData->defenseChange = storageFile.getSignedInt();
		equipmentData->wisdomChange = storageFile.getSignedInt();
		equipmentData->agilityChange = storageFile.getSignedInt();
		equipmentData->percentConditionResistanceEffectiveness = storageFile.getSignedInt();
		equipmentData->preventCriticalHits = storageFile.getBool();
		equipmentData->increaseEvasion = storageFile.getBool();
		equipmentData->halfMPConsumption = storageFile.getBool();
		equipmentData->noTerrainDamage = storageFile.getBool();
		equipmentData->cursed = storageFile.getBool();
		equipmentData->resistConditions = storageFile.getBool();
		equipmentData->usableByCharacters = storageFile.getBool();
	}
	else if (type == TYPE_MEDICINE)
	{
		medicineData = new MedicineItem;
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			medicineData->usableBy.append(storageFile.getBool());
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			medicineData->cureCondition.append(storageFile.getBool());
		medicineData->numberOfUses = storageFile.getSignedInt();
		medicineData->percentRestoredHP = storageFile.getSignedInt();
		medicineData->amountRestoredHP = storageFile.getSignedInt();
		medicineData->percentRestoredMP = storageFile.getSignedInt();
		medicineData->amountRestoredMP = storageFile.getSignedInt();
		medicineData->targetSingleAlly = storageFile.getBool();
		medicineData->usableByCharacters = storageFile.getBool();
		medicineData->onlyUsableOnFieldMap = storageFile.getBool();
		medicineData->onlyUsableOnDeadCharacters = storageFile.getBool();
	}
	else if (type == TYPE_SKILLBOOK)
	{
		skillBookData = new SkillBookItem;
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			skillBookData->usableBy.append(storageFile.getBool());
		skillBookData->numberOfUses = storageFile.getSignedInt();
		skillBookData->skillID = storageFile.getSignedInt();
		skillBookData->usableByCharacters = storageFile.getBool();
	}
	else if (type == TYPE_SEED)
	{
		seedData = new SeedItem;
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			seedData->usableBy.append(storageFile.getBool());
		seedData->numberOfUses = storageFile.getSignedInt();
		seedData->maxHPChange = storageFile.getSignedInt();
		seedData->maxMPChange = storageFile.getSignedInt();
		seedData->attackChange = storageFile.getSignedInt();
		seedData->defenseChange = storageFile.getSignedInt();
		seedData->wisdomChange = storageFile.getSignedInt();
		seedData->agilityChange = storageFile.getSignedInt();
		seedData->usableByCharacters = storageFile.getBool();
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		skillScrollData = new SkillScrollItem;
		temp_int = storageFile.getSignedInt();
		for (int i = 0; i < temp_int; ++i)
			skillScrollData->usableBy.append(storageFile.getBool());
		skillScrollData->numberOfUses = storageFile.getSignedInt();
		skillScrollData->skillID = storageFile.getSignedInt();
		skillScrollData->showItemNameMessageWhenUsed = storageFile.getBool();
		skillScrollData->usableByCharacters = storageFile.getBool();
	}
	else if (type == TYPE_SWITCH)
	{
		switchData = new SwitchItem;
		switchData->numberOfUses = storageFile.getSignedInt();
		switchData->switchID = storageFile.getSignedInt();
		switchData->allowFieldActivation = storageFile.getBool();
		switchData->allowBattleActivation = storageFile.getBool();
	}
	else if (type == TYPE_EVENT)
	{
		eventData = new EventItem;
		eventData->numberOfUses = storageFile.getSignedInt();
		eventData->allowFieldActivation = storageFile.getBool();
		eventData->allowBattleActivation = storageFile.getBool();
		TODO("Uncomment the following line when the battle events are ready");
		//eventData->event = new Event(storageFile);
	}
}

Item::~Item()
{
	if (type == TYPE_WEAPON)
		delete weaponData;
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
		delete equipmentData;
	else if (type == TYPE_MEDICINE)
		delete medicineData;
	else if (type == TYPE_SKILLBOOK)
		delete skillBookData;
	else if (type == TYPE_SEED)
		delete seedData;
	else if (type == TYPE_SKILLSCROLL)
		delete skillScrollData;
	else if (type == TYPE_SWITCH)
		delete switchData;
	else if (type == TYPE_EVENT)
	{
		TODO("Uncomment the following line when the battle events are ready");
		//delete eventData->event;
		delete eventData;
	}
}

void Item::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putString(description.toUtf8().data());
	storageFile.putSignedInt(price);
	storageFile.putSignedInt(type);
	if (type == TYPE_WEAPON)
	{
		storageFile.putSignedInt(weaponData->usableBy.size());
		for (int i = 0; i < weaponData->usableBy.size(); ++i)
			storageFile.putBool(weaponData->usableBy[i]);
		storageFile.putSignedInt(weaponData->attribute.size());
		for (int i = 0; i < weaponData->attribute.size(); ++i)
			storageFile.putBool(weaponData->attribute[i]);
		storageFile.putSignedInt(weaponData->conditions.size());
		for (int i = 0; i < weaponData->conditions.size(); ++i)
			storageFile.putBool(weaponData->conditions[i]);
		storageFile.putSignedInt(weaponData->animationSettings.movementBeforeAttack);
		storageFile.putSignedInt(weaponData->animationSettings.numberOfAttacks);
		storageFile.putSignedInt(weaponData->animationSettings.animationID);
		storageFile.putSignedInt(weaponData->animationSettings.rangedWeaponAnimationID);
		storageFile.putSignedInt(weaponData->animationSettings.trajectory);
		storageFile.putSignedInt(weaponData->animationSettings.targetting);
		storageFile.putSignedInt(weaponData->animationSettings.animationSpeed);
		storageFile.putBool(weaponData->animationSettings.weaponAnimation);
		storageFile.putBool(weaponData->animationSettings.afterImage);
		storageFile.putBool(weaponData->animationSettings.rangedWeapon);
		storageFile.putSignedInt(weaponData->skillID);
		storageFile.putSignedInt(weaponData->attackChange);
		storageFile.putSignedInt(weaponData->defenseChange);
		storageFile.putSignedInt(weaponData->wisdomChange);
		storageFile.putSignedInt(weaponData->agilityChange);
		storageFile.putSignedInt(weaponData->battleAnimationID);
		storageFile.putSignedInt(weaponData->amountMPConsumed);
		storageFile.putSignedInt(weaponData->percentHitProbability);
		storageFile.putSignedInt(weaponData->percentCriticalBonus);
		storageFile.putSignedInt(weaponData->percentConditionEffectivenessProbability);
		storageFile.putBool(weaponData->twoHanded);
		storageFile.putBool(weaponData->alwaysAttackFirst);
		storageFile.putBool(weaponData->attackTwice);
		storageFile.putBool(weaponData->attackAllEnemies);
		storageFile.putBool(weaponData->ignoreEnemyEvasion);
		storageFile.putBool(weaponData->cursed);
		storageFile.putBool(weaponData->usableOnTheDead);
		storageFile.putBool(weaponData->inflictConditions);
		storageFile.putBool(weaponData->usableByCharacters);
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		storageFile.putSignedInt(equipmentData->usableBy.size());
		for (int i = 0; i < equipmentData->usableBy.size(); ++i)
			storageFile.putBool(equipmentData->usableBy[i]);
		storageFile.putSignedInt(equipmentData->attributeResistance.size());
		for (int i = 0; i < equipmentData->attributeResistance.size(); ++i)
			storageFile.putBool(equipmentData->attributeResistance[i]);
		storageFile.putSignedInt(equipmentData->conditions.size());
		for (int i = 0; i < equipmentData->conditions.size(); ++i)
			storageFile.putBool(equipmentData->conditions[i]);
		storageFile.putSignedInt(equipmentData->skillID);
		storageFile.putSignedInt(equipmentData->attackChange);
		storageFile.putSignedInt(equipmentData->defenseChange);
		storageFile.putSignedInt(equipmentData->wisdomChange);
		storageFile.putSignedInt(equipmentData->agilityChange);
		storageFile.putSignedInt(equipmentData->percentConditionResistanceEffectiveness);
		storageFile.putBool(equipmentData->preventCriticalHits);
		storageFile.putBool(equipmentData->increaseEvasion);
		storageFile.putBool(equipmentData->halfMPConsumption);
		storageFile.putBool(equipmentData->noTerrainDamage);
		storageFile.putBool(equipmentData->cursed);
		storageFile.putBool(equipmentData->resistConditions);
		storageFile.putBool(equipmentData->usableByCharacters);
	}
	else if (type == TYPE_MEDICINE)
	{
		storageFile.putSignedInt(medicineData->usableBy.size());
		for (int i = 0; i < medicineData->usableBy.size(); ++i)
			storageFile.putBool(medicineData->usableBy[i]);
		storageFile.putSignedInt(medicineData->cureCondition.size());
		for (int i = 0; i < medicineData->cureCondition.size(); ++i)
			storageFile.putBool(medicineData->cureCondition[i]);
		storageFile.putSignedInt(medicineData->numberOfUses);
		storageFile.putSignedInt(medicineData->percentRestoredHP);
		storageFile.putSignedInt(medicineData->amountRestoredHP);
		storageFile.putSignedInt(medicineData->percentRestoredMP);
		storageFile.putSignedInt(medicineData->amountRestoredMP);
		storageFile.putBool(medicineData->targetSingleAlly);
		storageFile.putBool(medicineData->usableByCharacters);
		storageFile.putBool(medicineData->onlyUsableOnFieldMap);
		storageFile.putBool(medicineData->onlyUsableOnDeadCharacters);
	}
	else if (type == TYPE_SKILLBOOK)
	{
		storageFile.putSignedInt(skillBookData->usableBy.size());
		for (int i = 0; i < skillBookData->usableBy.size(); ++i)
			storageFile.putBool(skillBookData->usableBy[i]);
		storageFile.putSignedInt(skillBookData->numberOfUses);
		storageFile.putSignedInt(skillBookData->skillID);
		storageFile.putBool(skillBookData->usableByCharacters);
	}
	else if (type == TYPE_SEED)
	{
		storageFile.putSignedInt(seedData->usableBy.size());
		for (int i = 0; i < seedData->usableBy.size(); ++i)
			storageFile.putBool(seedData->usableBy[i]);
		storageFile.putSignedInt(seedData->numberOfUses);
		storageFile.putSignedInt(seedData->maxHPChange);
		storageFile.putSignedInt(seedData->maxMPChange);
		storageFile.putSignedInt(seedData->attackChange);
		storageFile.putSignedInt(seedData->defenseChange);
		storageFile.putSignedInt(seedData->wisdomChange);
		storageFile.putSignedInt(seedData->agilityChange);
		storageFile.putBool(seedData->usableByCharacters);
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		storageFile.putSignedInt(skillScrollData->usableBy.size());
		for (int i = 0; i < skillScrollData->usableBy.size(); ++i)
			storageFile.putBool(skillScrollData->usableBy[i]);
		storageFile.putSignedInt(skillScrollData->numberOfUses);
		storageFile.putSignedInt(skillScrollData->skillID);
		storageFile.putBool(skillScrollData->showItemNameMessageWhenUsed);
		storageFile.putBool(skillScrollData->usableByCharacters);
	}
	else if (type == TYPE_SWITCH)
	{
		storageFile.putSignedInt(switchData->numberOfUses);
		storageFile.putSignedInt(switchData->switchID);
		storageFile.putBool(switchData->allowFieldActivation);
		storageFile.putBool(switchData->allowBattleActivation);
	}
	else if (type == TYPE_EVENT)
	{
		storageFile.putSignedInt(eventData->numberOfUses);
		storageFile.putBool(eventData->allowFieldActivation);
		storageFile.putBool(eventData->allowBattleActivation);
		TODO("Uncomment the following line when the battle events are ready");
		//eventData->event->saveToStorageFile(storageFile);
	}
}

QStringList Item::getResources()
{
	return QStringList();
}

XMLNode Item::getItemNode()
{
	XMLNode TempNode, ItemNode = XMLNode::createXMLTopNode("item");
	QStringList tempStringList;
	QString temp_string;
	ItemNode.addAttribute("name", name.toUtf8().data());
	ItemNode.addAttribute("description", description.toUtf8().data());
	ItemNode.addAttribute("price", QString::number(price).toUtf8().data());
	if (type == TYPE_COMMONITEM)
		ItemNode.addAttribute("type", "commonitem");
	if (type == TYPE_WEAPON)
		ItemNode.addAttribute("type", "weapon");
	if (type == TYPE_SHIELD)
		ItemNode.addAttribute("type", "shield");
	if (type == TYPE_ARMOR)
		ItemNode.addAttribute("type", "armor");
	if (type == TYPE_HELMET)
		ItemNode.addAttribute("type", "helmet");
	if (type == TYPE_ACCESSORY)
		ItemNode.addAttribute("type", "accessory");
	if (type == TYPE_MEDICINE)
		ItemNode.addAttribute("type", "medicine");
	if (type == TYPE_SKILLBOOK)
		ItemNode.addAttribute("type", "skillbook");
	if (type == TYPE_SEED)
		ItemNode.addAttribute("type", "seed");
	if (type == TYPE_SKILLSCROLL)
		ItemNode.addAttribute("type", "skillscroll");
	if (type == TYPE_SWITCH)
		ItemNode.addAttribute("type", "switch");
	if (type == TYPE_EVENT)
		ItemNode.addAttribute("type", "event");
	if (type == TYPE_WEAPON)
	{
		TempNode = ItemNode.addChild("usableBy");
		TempNode.addAttribute("usableBy", (weaponData->usableByCharacters) ? "heroes":"classes");
		temp_string = "";
		for (int i = 0; i < weaponData->usableBy.size(); ++i)
			temp_string += (weaponData->usableBy[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		TempNode = ItemNode.addChild("attributes");
		temp_string = "";
		for (int i = 0; i < weaponData->attribute.size(); ++i)
			temp_string += (weaponData->attribute[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		TempNode = ItemNode.addChild("conditions");
		TempNode.addAttribute("inflictConditions", (weaponData->inflictConditions) ? "true":"false");
		TempNode.addAttribute("effectivenessProbability", QString::number(weaponData->percentConditionEffectivenessProbability).toUtf8().data());
		temp_string = "";
		for (int i = 0; i < weaponData->conditions.size(); ++i)
			temp_string += (weaponData->conditions[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		ItemNode.addAttribute("skillID", QString::number(weaponData->skillID).toUtf8().data());
		TempNode = ItemNode.addChild("statChange");
		TempNode.addAttribute("attack", QString::number(weaponData->attackChange).toUtf8().data());
		TempNode.addAttribute("defense", QString::number(weaponData->defenseChange).toUtf8().data());
		TempNode.addAttribute("wisdom", QString::number(weaponData->wisdomChange).toUtf8().data());
		TempNode.addAttribute("agility", QString::number(weaponData->agilityChange).toUtf8().data());
		TempNode = ItemNode.addChild("animation");
		if (weaponData->animationSettings.movementBeforeAttack == MOVEMENT_NONE)
			TempNode.addAttribute("movementBeforeAttack", "none");
		else if (weaponData->animationSettings.movementBeforeAttack == MOVEMENT_STEPFORWARD)
			TempNode.addAttribute("movementBeforeAttack", "stepforward");
		else if (weaponData->animationSettings.movementBeforeAttack == MOVEMENT_JUMPFORWARD)
			TempNode.addAttribute("movementBeforeAttack", "jumpforward");
		else
			TempNode.addAttribute("movementBeforeAttack", "movetotarget");
		TempNode.addAttribute("numberOfAttacks", QString::number(weaponData->animationSettings.numberOfAttacks).toUtf8().data());
		TempNode.addAttribute("animationID", QString::number(weaponData->animationSettings.animationID).toUtf8().data());
		TempNode.addAttribute("rangedWeaponAnimationID", QString::number(weaponData->animationSettings.rangedWeaponAnimationID).toUtf8().data());
		if (weaponData->animationSettings.trajectory == TRAJECTORY_FLYSTRAIGHTTOTARGET)
			TempNode.addAttribute("trajectory", "flystraighttotarget");
		else
			TempNode.addAttribute("trajectory", "returnafterstriking");
		if (weaponData->animationSettings.targetting == TARGET_SINGLE)
			TempNode.addAttribute("targetting", "single");
		else if (weaponData->animationSettings.targetting == TARGET_FLYDOWNCENTEROFSCREEN)
			TempNode.addAttribute("targetting", "flydowncenterofscreen");
		else if (weaponData->animationSettings.targetting == TARGET_STRIKEEACHSIMULTANEOUSLY)
			TempNode.addAttribute("targetting", "strikeeachsimultaneously");
		else
			TempNode.addAttribute("targetting", "strikeeachinturn");
		if (weaponData->animationSettings.animationSpeed == SPEED_FAST)
			TempNode.addAttribute("animationSpeed", "fast");
		else if (weaponData->animationSettings.animationSpeed == SPEED_MEDIUM)
			TempNode.addAttribute("animationSpeed", "medium");
		else
			TempNode.addAttribute("animationSpeed", "slow");
		TempNode.addAttribute("weaponAnimation", (weaponData->animationSettings.weaponAnimation) ? "true":"false");
		TempNode.addAttribute("afterImage", (weaponData->animationSettings.afterImage) ? "true":"false");
		TempNode.addAttribute("rangedWeapon", (weaponData->animationSettings.rangedWeapon) ? "true":"false");
		TempNode.addAttribute("battleAnimationID", QString::number(weaponData->battleAnimationID).toUtf8().data());
		TempNode = ItemNode.addChild("options");
		TempNode.addAttribute("mpConsumed", QString::number(weaponData->amountMPConsumed).toUtf8().data());
		TempNode.addAttribute("hitProbability", QString::number(weaponData->percentHitProbability).toUtf8().data());
		TempNode.addAttribute("twoHanded", (weaponData->twoHanded) ? "true":"false");
		TempNode.addAttribute("alwaysAttackFirst", (weaponData->alwaysAttackFirst) ? "true":"false");
		TempNode.addAttribute("attackTwice", (weaponData->attackTwice) ? "true":"false");
		TempNode.addAttribute("attackAllEnemies", (weaponData->attackAllEnemies) ? "true":"false");
		TempNode.addAttribute("ignoreEnemyEvasion", (weaponData->ignoreEnemyEvasion) ? "true":"false");
		TempNode.addAttribute("cursed", (weaponData->cursed) ? "true":"false");
		TempNode.addAttribute("usableOnTheDead", (weaponData->usableOnTheDead) ? "true":"false");
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		TempNode = ItemNode.addChild("usableBy");
		TempNode.addAttribute("usableBy", (equipmentData->usableByCharacters) ? "heroes":"classes");
		temp_string = "";
		for (int i = 0; i < equipmentData->usableBy.size(); ++i)
			temp_string += (equipmentData->usableBy[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		TempNode = ItemNode.addChild("attributeResistance");
		temp_string = "";
		for (int i = 0; i < equipmentData->attributeResistance.size(); ++i)
			temp_string += (equipmentData->attributeResistance[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		TempNode = ItemNode.addChild("conditions");
		TempNode.addAttribute("resistConditions", (equipmentData->resistConditions) ? "true":"false");
		TempNode.addAttribute("resistanceEffectiveness", QString::number(equipmentData->percentConditionResistanceEffectiveness).toUtf8().data());
		temp_string = "";
		for (int i = 0; i < equipmentData->conditions.size(); ++i)
			temp_string += (equipmentData->conditions[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		ItemNode.addAttribute("skillID", QString::number(equipmentData->skillID).toUtf8().data());
		TempNode = ItemNode.addChild("statChange");
		TempNode.addAttribute("attack", QString::number(equipmentData->attackChange).toUtf8().data());
		TempNode.addAttribute("defense", QString::number(equipmentData->defenseChange).toUtf8().data());
		TempNode.addAttribute("wisdom", QString::number(equipmentData->wisdomChange).toUtf8().data());
		TempNode.addAttribute("agility", QString::number(equipmentData->agilityChange).toUtf8().data());
		TempNode = ItemNode.addChild("options");
		TempNode.addAttribute("preventCriticalHits", (equipmentData->preventCriticalHits) ? "true":"false");
		TempNode.addAttribute("increaseEvasion", (equipmentData->increaseEvasion) ? "true":"false");
		TempNode.addAttribute("halfMPConsumption", (equipmentData->halfMPConsumption) ? "true":"false");
		TempNode.addAttribute("noTerrainDamage", (equipmentData->noTerrainDamage) ? "true":"false");
		TempNode.addAttribute("cursed", (equipmentData->cursed) ? "true":"false");
	}
	else if (type == TYPE_MEDICINE)
	{
		TempNode = ItemNode.addChild("usableBy");
		TempNode.addAttribute("usableBy", (medicineData->usableByCharacters) ? "heroes":"classes");
		temp_string = "";
		for (int i = 0; i < medicineData->usableBy.size(); ++i)
			temp_string += (medicineData->usableBy[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		TempNode = ItemNode.addChild("cureCondition");
		temp_string = "";
		for (int i = 0; i < medicineData->cureCondition.size(); ++i)
			temp_string += (medicineData->cureCondition[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		ItemNode.addAttribute("numberOfUses", QString::number(medicineData->numberOfUses).toUtf8().data());
		TempNode = ItemNode.addChild("options");
		TempNode.addAttribute("percentRestoredHP", QString::number(medicineData->percentRestoredHP).toUtf8().data());
		TempNode.addAttribute("amountRestoredHP", QString::number(medicineData->amountRestoredHP).toUtf8().data());
		TempNode.addAttribute("percentRestoredMP", QString::number(medicineData->percentRestoredMP).toUtf8().data());
		TempNode.addAttribute("amountRestoredMP", QString::number(medicineData->amountRestoredMP).toUtf8().data());
		TempNode.addAttribute("targetsSingleAlly", (medicineData->targetSingleAlly) ? "true":"false");
		TempNode.addAttribute("onlyUsableOnFieldMap", (medicineData->onlyUsableOnFieldMap) ? "true":"false");
		TempNode.addAttribute("onlyUsableOnDeadCharacters", (medicineData->onlyUsableOnDeadCharacters) ? "true":"false");
	}
	else if (type == TYPE_SKILLBOOK)
	{
		TempNode = ItemNode.addChild("usableBy");
		TempNode.addAttribute("usableBy", (skillBookData->usableByCharacters) ? "heroes":"classes");
		temp_string = "";
		for (int i = 0; i < skillBookData->usableBy.size(); ++i)
			temp_string += (skillBookData->usableBy[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		ItemNode.addAttribute("numberOfUses", QString::number(skillBookData->numberOfUses).toUtf8().data());
		ItemNode.addAttribute("skillID", QString::number(skillBookData->skillID).toUtf8().data());
	}
	else if (type == TYPE_SEED)
	{
		TempNode = ItemNode.addChild("usableBy");
		TempNode.addAttribute("usableBy", (seedData->usableByCharacters) ? "heroes":"classes");
		temp_string = "";
		for (int i = 0; i < seedData->usableBy.size(); ++i)
			temp_string += (seedData->usableBy[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		ItemNode.addAttribute("numberOfUses", QString::number(seedData->numberOfUses).toUtf8().data());
		TempNode = ItemNode.addChild("statChange");
		TempNode.addAttribute("maxHP", QString::number(seedData->maxHPChange).toUtf8().data());
		TempNode.addAttribute("maxMP", QString::number(seedData->maxMPChange).toUtf8().data());
		TempNode.addAttribute("attack", QString::number(seedData->attackChange).toUtf8().data());
		TempNode.addAttribute("defense", QString::number(seedData->defenseChange).toUtf8().data());
		TempNode.addAttribute("wisdom", QString::number(seedData->wisdomChange).toUtf8().data());
		TempNode.addAttribute("agility", QString::number(seedData->agilityChange).toUtf8().data());
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		TempNode = ItemNode.addChild("usableBy");
		TempNode.addAttribute("usableBy", (skillScrollData->usableByCharacters) ? "heroes":"classes");
		temp_string = "";
		for (int i = 0; i < skillScrollData->usableBy.size(); ++i)
			temp_string += (skillScrollData->usableBy[i]) ? "1":"0";
		TempNode.addText(temp_string.toUtf8().data());
		ItemNode.addAttribute("numberOfUses", QString::number(skillScrollData->numberOfUses).toUtf8().data());
		ItemNode.addAttribute("skillID", QString::number(skillScrollData->skillID).toUtf8().data());
		ItemNode.addAttribute("showItemNameMessageWhenUsed", (skillScrollData->showItemNameMessageWhenUsed) ? "true":"false");
	}
	else if (type == TYPE_SWITCH)
	{
		ItemNode.addAttribute("numberOfUses", QString::number(switchData->numberOfUses).toUtf8().data());
		ItemNode.addAttribute("switchID", QString::number(switchData->switchID).toUtf8().data());
		ItemNode.addAttribute("allowFieldActivation", (switchData->allowFieldActivation) ? "true":"false");
		ItemNode.addAttribute("allowBattleActivation", (switchData->allowBattleActivation) ? "true":"false");
	}
	else if (type == TYPE_EVENT)
	{
		ItemNode.addAttribute("numberOfUses", QString::number(eventData->numberOfUses).toUtf8().data());
		ItemNode.addAttribute("allowFieldActivation", (eventData->allowFieldActivation) ? "true":"false");
		ItemNode.addAttribute("allowBattleActivation", (eventData->allowBattleActivation) ? "true":"false");
		TODO("Uncomment the following line when the battle events are ready");
		//ItemNode.addChild(eventData->event->getEventNode());
	}
	return ItemNode;
}

QString Item::getName()
{
	return name;
}

void Item::setName(QString value)
{
	name = value;
}

QString Item::getDescription()
{
	return description;
}

void Item::setDescription(QString value)
{
	description = value;
}

Item::WeaponItem *Item::getWeaponData()
{
	return weaponData;
}

Item::EquipmentItem *Item::getEquipmentData()
{
	return equipmentData;
}

Item::MedicineItem *Item::getMedicineData()
{
	return medicineData;
}

Item::SkillBookItem *Item::getSkillBookData()
{
	return skillBookData;
}

Item::SeedItem *Item::getSeedData()
{
	return seedData;
}

Item::SkillScrollItem *Item::getSkillScrollData()
{
	return skillScrollData;
}

Item::SwitchItem *Item::getSwitchData()
{
	return switchData;
}

Item::EventItem *Item::getEventData()
{
	return eventData;
}

int Item::getPrice()
{
	return price;
}

void Item::setPrice(int value)
{
	price = value;
}

int Item::getType()
{
	return type;
}

void Item::setType(int value)
{
	if (type == TYPE_WEAPON)
		delete weaponData;
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
		delete equipmentData;
	else if (type == TYPE_MEDICINE)
		delete medicineData;
	else if (type == TYPE_SKILLBOOK)
		delete skillBookData;
	else if (type == TYPE_SEED)
		delete seedData;
	else if (type == TYPE_SKILLSCROLL)
		delete skillScrollData;
	else if (type == TYPE_SWITCH)
		delete switchData;
	else if (type == TYPE_EVENT)
	{
		TODO("Uncomment the following line when the battle events are ready");
		//delete eventData->event;
		delete eventData;
	}
	type = value;
	if (type == TYPE_WEAPON)
	{
		weaponData = new WeaponItem;
		weaponData->animationSettings.movementBeforeAttack = MOVEMENT_NONE;
		weaponData->animationSettings.numberOfAttacks = 1;
		weaponData->animationSettings.animationID = 0;
		weaponData->animationSettings.rangedWeaponAnimationID = 0;
		weaponData->animationSettings.trajectory = TRAJECTORY_FLYSTRAIGHTTOTARGET;
		weaponData->animationSettings.targetting = TARGET_SINGLE;
		weaponData->animationSettings.animationSpeed = SPEED_FAST;
		weaponData->animationSettings.weaponAnimation = true;
		weaponData->animationSettings.afterImage = false;
		weaponData->animationSettings.rangedWeapon = false;
		weaponData->skillID = 0;
		weaponData->attackChange = 0;
		weaponData->defenseChange = 0;
		weaponData->wisdomChange = 0;
		weaponData->agilityChange = 0;
		weaponData->battleAnimationID = 0;
		weaponData->amountMPConsumed = 0;
		weaponData->percentHitProbability = 0;
		weaponData->percentCriticalBonus = 0;
		weaponData->percentConditionEffectivenessProbability = 0;
		weaponData->twoHanded = false;
		weaponData->alwaysAttackFirst = false;
		weaponData->attackTwice = false;
		weaponData->attackAllEnemies = false;
		weaponData->ignoreEnemyEvasion = false;
		weaponData->cursed = false;
		weaponData->usableOnTheDead = false;
		weaponData->inflictConditions = false;
		weaponData->usableByCharacters = true;
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		equipmentData = new EquipmentItem;
		equipmentData->skillID = 0;
		equipmentData->attackChange = 0;
		equipmentData->defenseChange = 0;
		equipmentData->wisdomChange = 0;
		equipmentData->agilityChange = 0;
		equipmentData->percentConditionResistanceEffectiveness = 0;
		equipmentData->preventCriticalHits = false;
		equipmentData->increaseEvasion = false;
		equipmentData->halfMPConsumption = false;
		equipmentData->noTerrainDamage = false;
		equipmentData->cursed = false;
		equipmentData->resistConditions = false;
		equipmentData->usableByCharacters = true;
	}
	else if (type == TYPE_MEDICINE)
	{
		medicineData = new MedicineItem;
		medicineData->numberOfUses = 1;
		medicineData->percentRestoredHP = 0;
		medicineData->amountRestoredHP = 0;
		medicineData->percentRestoredMP = 0;
		medicineData->amountRestoredMP = 0;
		medicineData->targetSingleAlly = 0;
		medicineData->usableByCharacters = true;
		medicineData->onlyUsableOnFieldMap = false;
		medicineData->onlyUsableOnDeadCharacters = false;
	}
	else if (type == TYPE_SKILLBOOK)
	{
		skillBookData = new SkillBookItem;
		skillBookData->numberOfUses = 1;
		skillBookData->skillID = 0;
		skillBookData->usableByCharacters = true;
	}
	else if (type == TYPE_SEED)
	{
		seedData = new SeedItem;
		seedData->numberOfUses = 1;
		seedData->maxHPChange = 0;
		seedData->maxMPChange = 0;
		seedData->attackChange = 0;
		seedData->defenseChange = 0;
		seedData->wisdomChange = 0;
		seedData->agilityChange = 0;
		seedData->usableByCharacters = true;
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		skillScrollData = new SkillScrollItem;
		skillScrollData->numberOfUses = 1;
		skillScrollData->skillID = 0;
		skillScrollData->showItemNameMessageWhenUsed = true;
		skillScrollData->usableByCharacters = true;
	}
	else if (type == TYPE_SWITCH)
	{
		switchData = new SwitchItem;
		switchData->numberOfUses = 1;
		switchData->switchID = 0;
		switchData->allowFieldActivation = true;
		switchData->allowBattleActivation = true;
	}
	else if (type == TYPE_EVENT)
	{
		eventData = new EventItem;
		TODO("Uncomment the following line when the battle events are ready");
		//eventData->event = new Event(other.eventData->event);
		eventData->numberOfUses = 1;
		eventData->allowFieldActivation = true;
		eventData->allowBattleActivation = true;
	}
}

void Item::characterDeleted(int characterID)
{
	if (type == TYPE_WEAPON)
	{
		if (weaponData->usableByCharacters)
			weaponData->usableBy.removeAt(characterID);
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		if (equipmentData->usableByCharacters)
			equipmentData->usableBy.removeAt(characterID);
	}
	else if (type == TYPE_MEDICINE)
	{
		if (medicineData->usableByCharacters)
			medicineData->usableBy.removeAt(characterID);
	}
	else if (type == TYPE_SKILLBOOK)
	{
		if (skillBookData->usableByCharacters)
			skillBookData->usableBy.removeAt(characterID);
	}
	else if (type == TYPE_SEED)
	{
		if (seedData->usableByCharacters)
			seedData->usableBy.removeAt(characterID);
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		if (skillScrollData->usableByCharacters)
			skillScrollData->usableBy.removeAt(characterID);
	}
}

void Item::characterAdded()
{
	if (type == TYPE_WEAPON)
	{
		if (weaponData->usableByCharacters)
			weaponData->usableBy.append(true);
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		if (equipmentData->usableByCharacters)
			equipmentData->usableBy.append(true);
	}
	else if (type == TYPE_MEDICINE)
	{
		if (medicineData->usableByCharacters)
			medicineData->usableBy.append(true);
	}
	else if (type == TYPE_SKILLBOOK)
	{
		if (skillBookData->usableByCharacters)
			skillBookData->usableBy.append(true);
	}
	else if (type == TYPE_SEED)
	{
		if (seedData->usableByCharacters)
			seedData->usableBy.append(true);
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		if (skillScrollData->usableByCharacters)
			skillScrollData->usableBy.append(true);
	}
}

void Item::classDeleted(int classID)
{
	if (type == TYPE_WEAPON)
	{
		if (!weaponData->usableByCharacters)
			weaponData->usableBy.removeAt(classID);
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		if (!equipmentData->usableByCharacters)
			equipmentData->usableBy.removeAt(classID);
	}
	else if (type == TYPE_MEDICINE)
	{
		if (!medicineData->usableByCharacters)
			medicineData->usableBy.removeAt(classID);
	}
	else if (type == TYPE_SKILLBOOK)
	{
		if (!skillBookData->usableByCharacters)
			skillBookData->usableBy.removeAt(classID);
	}
	else if (type == TYPE_SEED)
	{
		if (!seedData->usableByCharacters)
			seedData->usableBy.removeAt(classID);
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		if (!skillScrollData->usableByCharacters)
			skillScrollData->usableBy.removeAt(classID);
	}
}

void Item::classAdded()
{
	if (type == TYPE_WEAPON)
	{
		if (!weaponData->usableByCharacters)
			weaponData->usableBy.append(true);
	}
	else if (type == TYPE_SHIELD || type == TYPE_ARMOR || type == TYPE_HELMET || type == TYPE_ACCESSORY)
	{
		if (!equipmentData->usableByCharacters)
			equipmentData->usableBy.append(true);
	}
	else if (type == TYPE_MEDICINE)
	{
		if (!medicineData->usableByCharacters)
			medicineData->usableBy.append(true);
	}
	else if (type == TYPE_SKILLBOOK)
	{
		if (!skillBookData->usableByCharacters)
			skillBookData->usableBy.append(true);
	}
	else if (type == TYPE_SEED)
	{
		if (!seedData->usableByCharacters)
			seedData->usableBy.append(true);
	}
	else if (type == TYPE_SKILLSCROLL)
	{
		if (!skillScrollData->usableByCharacters)
			skillScrollData->usableBy.append(true);
	}
}

bool Item::tryLoad(XMLNode ItemNode)
{
	XMLNode TempNode;
	QStringList tempStringList;
	QString temp_string;
	int temp_type;
	if (!XMLTools::attributeExists(ItemNode, "name"))
		return false;
	if (!XMLTools::attributeExists(ItemNode, "description"))
		return false;
	if (!XMLTools::attributeExists(ItemNode, "price"))
		return false;
	if (!XMLTools::attributeExists(ItemNode, "type"))
		return false;
	tempStringList += "commonitem";
	tempStringList += "weapon";
	tempStringList += "shield";
	tempStringList += "armor";
	tempStringList += "helmet";
	tempStringList += "accessory";
	tempStringList += "medicine";
	tempStringList += "skillbook";
	tempStringList += "seed";
	tempStringList += "skillscroll";
	tempStringList += "switch";
	tempStringList += "event";
	if (!XMLTools::attributeStringValid(ItemNode, "type", tempStringList))
		return false;
	temp_string = ItemNode.getAttribute("type");
	if (temp_string == "commonitem")
		temp_type = TYPE_COMMONITEM;
	else if (temp_string == "weapon")
		temp_type = TYPE_WEAPON;
	else if (temp_string == "shield")
		temp_type = TYPE_SHIELD;
	else if (temp_string == "armor")
		temp_type = TYPE_ARMOR;
	else if (temp_string == "helmet")
		temp_type = TYPE_HELMET;
	else if (temp_string == "accessory")
		temp_type = TYPE_ACCESSORY;
	else if (temp_string == "medicine")
		temp_type = TYPE_MEDICINE;
	else if (temp_string == "skillbook")
		temp_type = TYPE_SKILLBOOK;
	else if (temp_string == "seed")
		temp_type = TYPE_SEED;
	else if (temp_string == "skillscroll")
		temp_type = TYPE_SKILLSCROLL;
	else if (temp_string == "switch")
		temp_type = TYPE_SWITCH;
	else//(temp_string == "event")
		temp_type = TYPE_EVENT;
	if (temp_type == TYPE_WEAPON)
	{
		if (!XMLTools::nodeExists(ItemNode, "usableBy"))
			return false;
		TempNode = ItemNode.getChildNode("usableBy");
		if (!XMLTools::attributeExists(TempNode, "usableBy"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "usableBy", QStringList() << "heroes" << "classes", 1))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "attributes"))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "conditions"))
			return false;
		TempNode = ItemNode.getChildNode("conditions");
		if (!XMLTools::attributeExists(TempNode, "inflictConditions"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "inflictConditions", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "effectivenessProbability"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "effectivenessProbability", 0, 100))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "skillID"))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "statChange"))
			return false;
		TempNode = ItemNode.getChildNode("statChange");
		if (!XMLTools::attributeExists(TempNode, "attack"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "attack", -500, 500))
			return false;
		if (!XMLTools::attributeExists(TempNode, "defense"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "defense", -500, 500))
			return false;
		if (!XMLTools::attributeExists(TempNode, "wisdom"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "wisdom", -500, 500))
			return false;
		if (!XMLTools::attributeExists(TempNode, "agility"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "agility", -500, 500))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "animation"))
			return false;
		TempNode = ItemNode.getChildNode("animation");
		if (!XMLTools::attributeExists(TempNode, "movementBeforeAttack"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "movementBeforeAttack", QStringList() << "none" << "stepforward" << "jumpforward" << "movetotarget", 3))
			return false;
		if (!XMLTools::attributeExists(TempNode, "numberOfAttacks"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "numberOfAttacks", 1, 3))
			return false;
		if (!XMLTools::attributeExists(TempNode, "animationID"))
			return false;
		if (!XMLTools::attributeExists(TempNode, "rangedWeaponAnimationID"))
			return false;
		if (!XMLTools::attributeExists(TempNode, "trajectory"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "trajectory", QStringList() << "flystraighttotarget" << "returnafterstriking", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "targetting"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "targetting", QStringList() << "single" << "flydowncenterofscreen" << "strikeeachsimultaneously" << "strikeeachinturn", 3))
			return false;
		if (!XMLTools::attributeExists(TempNode, "animationSpeed"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "animationSpeed", QStringList() << "fast" << "medium" << "slow", 2))
			return false;
		if (!XMLTools::attributeExists(TempNode, "weaponAnimation"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "weaponAnimation", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "afterImage"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "afterImage", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "rangedWeapon"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "rangedWeapon", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "battleAnimationID"))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "options"))
			return false;
		TempNode = ItemNode.getChildNode("options");
		if (!XMLTools::attributeExists(TempNode, "mpConsumed"))
			return false;
		if (!XMLTools::attributeExists(TempNode, "hitProbability"))
			return false;
		if (!XMLTools::attributeExists(TempNode, "twoHanded"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "twoHanded", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "alwaysAttackFirst"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "alwaysAttackFirst", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "attackTwice"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "attackTwice", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "attackAllEnemies"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "attackAllEnemies", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "ignoreEnemyEvasion"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "ignoreEnemyEvasion", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "cursed"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "cursed", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "usableOnTheDead"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "usableOnTheDead", QStringList() << "true" << "false", 1))
			return false;
	}
	else if (temp_type == TYPE_SHIELD || temp_type == TYPE_ARMOR || temp_type == TYPE_HELMET || temp_type == TYPE_ACCESSORY)
	{
		if (!XMLTools::nodeExists(ItemNode, "usableBy"))
			return false;
		TempNode = ItemNode.getChildNode("usableBy");
		if (!XMLTools::attributeExists(TempNode, "usableBy"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "usableBy", QStringList() << "heroes" << "classes", 1))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "attributeResistance"))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "conditions"))
			return false;
		TempNode = ItemNode.getChildNode("conditions");
		if (!XMLTools::attributeExists(TempNode, "resistConditions"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "resistConditions", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "resistanceEffectiveness"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "resistanceEffectiveness", 0, 100))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "skillID"))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "statChange"))
			return false;
		TempNode = ItemNode.getChildNode("statChange");
		if (!XMLTools::attributeExists(TempNode, "attack"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "attack", -500, 500))
			return false;
		if (!XMLTools::attributeExists(TempNode, "defense"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "defense", -500, 500))
			return false;
		if (!XMLTools::attributeExists(TempNode, "wisdom"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "wisdom", -500, 500))
			return false;
		if (!XMLTools::attributeExists(TempNode, "agility"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "agility", -500, 500))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "options"))
			return false;
		TempNode = ItemNode.getChildNode("options");
		if (!XMLTools::attributeExists(TempNode, "preventCriticalHits"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "preventCriticalHits", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "increaseEvasion"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "increaseEvasion", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "halfMPConsumption"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "halfMPConsumption", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "noTerrainDamage"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "noTerrainDamage", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "cursed"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "cursed", QStringList() << "true" << "false", 1))
			return false;
	}
	else if (temp_type == TYPE_MEDICINE)
	{
		if (!XMLTools::nodeExists(ItemNode, "usableBy"))
			return false;
		TempNode = ItemNode.getChildNode("usableBy");
		if (!XMLTools::attributeExists(TempNode, "usableBy"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "usableBy", QStringList() << "heroes" << "classes", 1))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "cureCondition"))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "numberOfUses"))
			return false;
		if (!XMLTools::attributeIntRangeValid(ItemNode, "numberOfUses", 0, 5))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "options"))
			return false;
		TempNode = ItemNode.getChildNode("options");
		if (!XMLTools::attributeExists(TempNode, "percentRestoredHP"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "percentRestoredHP", 0, 100))
			return false;
		if (!XMLTools::attributeExists(TempNode, "amountRestoredHP"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "amountRestoredHP", 0, 9999))
			return false;
		if (!XMLTools::attributeExists(TempNode, "percentRestoredMP"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "percentRestoredMP", 0, 100))
			return false;
		if (!XMLTools::attributeExists(TempNode, "amountRestoredMP"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "amountRestoredMP", 0, 999))
			return false;
		if (!XMLTools::attributeExists(TempNode, "targetsSingleAlly"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "targetsSingleAlly", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "onlyUsableOnFieldMap"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "onlyUsableOnFieldMap", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(TempNode, "onlyUsableOnDeadCharacters"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "onlyUsableOnDeadCharacters", QStringList() << "true" << "false", 1))
			return false;
	}
	else if (temp_type == TYPE_SKILLBOOK)
	{
		if (!XMLTools::nodeExists(ItemNode, "usableBy"))
			return false;
		TempNode = ItemNode.getChildNode("usableBy");
		if (!XMLTools::attributeExists(TempNode, "usableBy"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "usableBy", QStringList() << "heroes" << "classes", 1))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "numberOfUses"))
			return false;
		if (!XMLTools::attributeIntRangeValid(ItemNode, "numberOfUses", 0, 5))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "skillID"))
			return false;
	}
	else if (temp_type == TYPE_SEED)
	{
		if (!XMLTools::nodeExists(ItemNode, "usableBy"))
			return false;
		TempNode = ItemNode.getChildNode("usableBy");
		if (!XMLTools::attributeExists(TempNode, "usableBy"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "usableBy", QStringList() << "heroes" << "classes", 1))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "numberOfUses"))
			return false;
		if (!XMLTools::attributeIntRangeValid(ItemNode, "numberOfUses", 0, 5))
			return false;
		if (!XMLTools::nodeExists(ItemNode, "statChange"))
			return false;
		TempNode = ItemNode.getChildNode("statChange");
		if (!XMLTools::attributeExists(TempNode, "maxHP"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "maxHP", 0, 9999))
			return false;
		if (!XMLTools::attributeExists(TempNode, "maxMP"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "maxMP", 0, 999))
			return false;
		if (!XMLTools::attributeExists(TempNode, "attack"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "attack", 0, 999))
			return false;
		if (!XMLTools::attributeExists(TempNode, "defense"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "defense", 0, 999))
			return false;
		if (!XMLTools::attributeExists(TempNode, "wisdom"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "wisdom", 0, 999))
			return false;
		if (!XMLTools::attributeExists(TempNode, "agility"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode, "agility", 0, 999))
			return false;
	}
	else if (temp_type == TYPE_SKILLSCROLL)
	{
		if (!XMLTools::nodeExists(ItemNode, "usableBy"))
			return false;
		TempNode = ItemNode.getChildNode("usableBy");
		if (!XMLTools::attributeExists(TempNode, "usableBy"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode, "usableBy", QStringList() << "heroes" << "classes", 1))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "numberOfUses"))
			return false;
		if (!XMLTools::attributeIntRangeValid(ItemNode, "numberOfUses", 0, 5))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "skillID"))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "showItemNameMessageWhenUsed"))
			return false;
		if (!XMLTools::attributeStringValid(ItemNode, "showItemNameMessageWhenUsed", QStringList() << "true" << "false", 1))
			return false;
	}
	else if (temp_type == TYPE_SWITCH)
	{
		if (!XMLTools::attributeExists(ItemNode, "numberOfUses"))
			return false;
		if (!XMLTools::attributeIntRangeValid(ItemNode, "numberOfUses", 0, 5))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "switchID"))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "allowFieldActivation"))
			return false;
		if (!XMLTools::attributeStringValid(ItemNode, "allowFieldActivation", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "allowBattleActivation"))
			return false;
		if (!XMLTools::attributeStringValid(ItemNode, "allowBattleActivation", QStringList() << "true" << "false", 1))
			return false;
	}
	else if (temp_type == TYPE_EVENT)
	{
		if (!XMLTools::attributeExists(ItemNode, "numberOfUses"))
			return false;
		if (!XMLTools::attributeIntRangeValid(ItemNode, "numberOfUses", 0, 5))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "allowFieldActivation"))
			return false;
		if (!XMLTools::attributeStringValid(ItemNode, "allowFieldActivation", QStringList() << "true" << "false", 1))
			return false;
		if (!XMLTools::attributeExists(ItemNode, "allowBattleActivation"))
			return false;
		if (!XMLTools::attributeStringValid(ItemNode, "allowBattleActivation", QStringList() << "true" << "false", 1))
			return false;
		TODO("Uncomment the following lines when the battle events are ready");
		//if (!XMLTools::nodeExists(ItemNode, "event"))
		//	return false;
		//if (!Event::tryLoad(ItemNode.getChildNode("event")))
		//	return false;
	}
	return true;
}
