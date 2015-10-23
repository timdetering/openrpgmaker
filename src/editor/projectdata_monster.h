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

#ifndef PROJECTDATA_MONSTER_H
#define PROJECTDATA_MONSTER_H

#include <QString>
#include <QImage>
#include <QPainter>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class QColor;
class BattleEvent;

class Monster
{
	public:
		friend class ProjectData;
		struct ItemDrop
		{
			int itemID;
			int priority;
		};
		struct BehaviorAction
		{
			int preconditionType;
			int priority;
			int preconditionValue1;
			int preconditionValue2;
			int actionType;
			int actionID;
			int turnSwitchOnID;
			int turnSwitchOffID;
			int monsterAnimationID;
			bool turnSwitchOn;
			bool turnSwitchOff;
		};
		enum Preconditon
		{
			PRECONDITION_NONE=0,
			PRECONDITION_SWITCH,
			PRECONDITION_TURNSELAPSED,
			PRECONDITION_MONSTERSPRESENT,
			PRECONDITION_MONSTERSHP,
			PRECONDITION_MONSTERSMP,
			PRECONDITION_PARTYSAVERAGELEVEL,
			PRECONDITION_PARTYSEXHAUSTION
		};
		enum BehaviorActionType {TYPE_BASIC=0, TYPE_SKILL, TYPE_TRANSFORM};
		enum BehaviorBasicAction
		{
			ACTION_ATTACK=0,
			ACTION_DOUBLEATTACK,
			ACTION_DEFEND,
			ACTION_OBSERVEBATTLE,
			ACTION_CHARGEUP,
			ACTION_SELFDESTRUCT,
			ACTION_ESCAPE,
			ACTION_NOACTION
		};
		enum ResistLevel {RESISTLEVEL_E=0, RESISTLEVEL_D, RESISTLEVEL_C, RESISTLEVEL_B, RESISTLEVEL_A};
		enum AILevel {AILEVEL_BASIC=0, AILEVEL_INTERMEDIATE, AILEVEL_ADVANCED, AILEVEL_CUSTOM};
		Monster();
		Monster(Monster &other);
		Monster(const Monster &other);
		Monster(XMLNode MonsterNode);
		Monster(StorageFile &storageFile);
		~Monster();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getMonsterNode();
		BattleEvent *getBattleEventPointer();
		BattleEvent *getCustomAIPointer();
		QString getName();
		void setName(QString value);
		void getItemDropped(int itemID, ItemDrop *item);
		int numItemsDropped();
		void setItemDropped(int itemID, ItemDrop &item);
		void addItemDropped(ItemDrop item);
		void deleteItemDropped(int itemID);
		void getBehaviorAction(int actionID, BehaviorAction *value);
		int numBehaviorActions();
		void setBehaviorAction(int actionID, BehaviorAction &value);
		void addBehaviorAction(BehaviorAction value);
		void deleteBehaviorAction(int actionID);
		int getConditionResist(int conditionID);
		void setConditionResist(int conditionID, int value);
		void resizeConditionResists(int numConditions);
		int getAttributeResist(int attributeID);
		void setAttributeResist(int attributeID, int value);
		void resizeAttributeResists(int numAttributes);
		float getCriticalHitRate();
		void setCriticalHitRate(float value);
		float getCriticalHitMultiplier();
		void setCriticalHitMultiplier(float value);
		int getMaxHP();
		void setMaxHP(int value);
		int getMaxMP();
		void setMaxMP(int value);
		int getAttack();
		void setAttack(int value);
		int getDefense();
		void setDefense(int value);
		int getWisdom();
		void setWisdom(int value);
		int getAgility();
		void setAgility(int value);
		int getExperienceGain();
		void setExperienceGain(int value);
		int getMoneyAquired();
		void setMoneyAquired(int value);
		int getItemDropProbability();
		void setItemDropProbability(int value);
		int getMonsterAnimationID();
		void setMonsterAnimationID(int value);
		int getHueShift();
		void setHueShift(int value);
		int getAILevel();
		void setAILevel(int value);
		bool isTransparent();
		void setIsTransparent(bool value);
		bool isFlying();
		void setIsFlying(bool value);
		bool isCriticalHits();
		void setCriticalHits(bool value);
		bool getAttacksOftenMiss();
		void setAttacksOftenMiss(bool value);
		QString getStrongAgainst();
		QString getWeakAgainst();
		static bool tryLoad(XMLNode MonsterNode);
	private:
		QString name;
		QList<ItemDrop> itemsDropped;
		QList<BehaviorAction> behaviorActions;
		QList<int> conditionResists;
		QList<int> attributeResists;
		BattleEvent *battleEvent;
		BattleEvent *customAI;
		float criticalHitRate;
		float criticalHitMultiplier;
		int maxHP;
		int maxMP;
		int attack;
		int defense;
		int wisdom;
		int agility;
		int experienceGain;
		int moneyAquired;
		int itemDropProbability;
		int monsterAnimationID;
		int hueShift;
		int aiLevel;
		bool transparent;
		bool flying;
		bool criticalHits;
		bool attacksOftenMiss;
};

#endif // PROJECTDATA_MONSTER_H
