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

#ifndef PROJECTDATA_ITEM_H
#define PROJECTDATA_ITEM_H

#include <QList>
#include <QString>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class Item
{
	public:
		friend class ProjectData;
		enum ItemType
		{
			TYPE_COMMONITEM=0,
			TYPE_WEAPON,
			TYPE_SHIELD,
			TYPE_ARMOR,
			TYPE_HELMET,
			TYPE_ACCESSORY,
			TYPE_MEDICINE,
			TYPE_SKILLBOOK,
			TYPE_SEED,
			TYPE_SKILLSCROLL,
			TYPE_SWITCH,
			TYPE_EVENT
		};
		enum NumberOfUses
		{
			USES_UNLIMITED=0,
			USES_ONE,
			USES_TWO,
			USES_THREE,
			USES_FOUR,
			USES_FIVE
		};
		enum MovementBeforeAttack
		{
			MOVEMENT_NONE=0,
			MOVEMENT_STEPFORWARD,
			MOVEMENT_JUMPFORWARD,
			MOVEMENT_MOVETOTARGET
		};
		enum Trajectory
		{
			TRAJECTORY_FLYSTRAIGHTTOTARGET=0,
			TRAJECTORY_RETURNAFTERSTRIKING
		};
		enum Targetting
		{
			TARGET_SINGLE=0,
			TARGET_FLYDOWNCENTEROFSCREEN,
			TARGET_STRIKEEACHSIMULTANEOUSLY,
			TARGET_STRIKEEACHINTURN
		};
		enum AnimationSpeed
		{
			SPEED_FAST=0,
			SPEED_MEDIUM,
			SPEED_SLOW
		};
		struct AnimationSettings
		{
			int movementBeforeAttack;
			int numberOfAttacks;
			int animationID;
			int rangedWeaponAnimationID;
			int trajectory;
			int targetting;
			int animationSpeed;
			bool weaponAnimation;
			bool afterImage;
			bool rangedWeapon;
		};
		struct WeaponItem
		{
			QList<bool> usableBy;
			QList<bool> attribute;
			QList<bool> conditions;
			AnimationSettings animationSettings;
			int skillID;
			int attackChange;
			int defenseChange;
			int wisdomChange;
			int agilityChange;
			int battleAnimationID;
			int amountMPConsumed;
			int percentHitProbability;
			int percentCriticalBonus;
			int percentConditionEffectivenessProbability;
			bool twoHanded;
			bool alwaysAttackFirst;
			bool attackTwice;
			bool attackAllEnemies;
			bool ignoreEnemyEvasion;
			bool cursed;
			bool usableOnTheDead;
			bool inflictConditions;
			bool usableByCharacters;
		};
		struct EquipmentItem
		{
			QList<bool> usableBy;
			QList<bool> attributeResistance;
			QList<bool> conditions;
			int skillID;
			int attackChange;
			int defenseChange;
			int wisdomChange;
			int agilityChange;
			int percentConditionResistanceEffectiveness;
			bool preventCriticalHits;
			bool increaseEvasion;
			bool halfMPConsumption;
			bool noTerrainDamage;
			bool cursed;
			bool resistConditions;
			bool usableByCharacters;
		};
		struct MedicineItem
		{
			QList<bool> usableBy;
			QList<bool> cureCondition;
			int numberOfUses;
			int percentRestoredHP;
			int amountRestoredHP;
			int percentRestoredMP;
			int amountRestoredMP;
			bool targetSingleAlly;
			bool usableByCharacters;
			bool onlyUsableOnFieldMap;
			bool onlyUsableOnDeadCharacters;
		};
		struct SkillBookItem
		{
			QList<bool> usableBy;
			int numberOfUses;
			int skillID;
			bool usableByCharacters;
		};
		struct SeedItem
		{
			QList<bool> usableBy;
			int numberOfUses;
			int maxHPChange;
			int maxMPChange;
			int attackChange;
			int defenseChange;
			int wisdomChange;
			int agilityChange;
			bool usableByCharacters;
		};
		struct SkillScrollItem
		{
			QList<bool> usableBy;
			int numberOfUses;
			int skillID;
			bool showItemNameMessageWhenUsed;
			bool usableByCharacters;
		};
		struct SwitchItem
		{
			int numberOfUses;
			int switchID;
			bool allowFieldActivation;
			bool allowBattleActivation;
		};
		struct EventItem
		{
			//Event *event;
			int numberOfUses;
			bool allowFieldActivation;
			bool allowBattleActivation;
		};
		Item();
		Item(Item &other);
		Item(const Item &other);
		Item(XMLNode ItemNode);
		Item(StorageFile &storageFile);
		~Item();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getItemNode();
		QString getName();
		void setName(QString value);
		QString getDescription();
		void setDescription(QString value);
		WeaponItem *getWeaponData();
		EquipmentItem *getEquipmentData();
		MedicineItem *getMedicineData();
		SkillBookItem *getSkillBookData();
		SeedItem *getSeedData();
		SkillScrollItem *getSkillScrollData();
		SwitchItem *getSwitchData();
		EventItem *getEventData();
		int getPrice();
		void setPrice(int value);
		int getType();
		void setType(int value);
		void characterDeleted(int characterID);
		void characterAdded();
		void classDeleted(int classID);
		void classAdded();
		static bool tryLoad(XMLNode ItemNode);
	private:
		QString name;
		QString description;
		WeaponItem *weaponData;
		EquipmentItem *equipmentData;
		MedicineItem *medicineData;
		SkillBookItem *skillBookData;
		SeedItem *seedData;
		SkillScrollItem *skillScrollData;
		SwitchItem *switchData;
		EventItem *eventData;
		int price;
		int type;
};

#endif // PROJECTDATA_ITEM_H
