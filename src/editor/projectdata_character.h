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

#ifndef PROJECTDATA_CHARACTER_H
#define PROJECTDATA_CHARACTER_H

class BattleEvent;

#include <QString>
#include <QList>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class Character
{
	public:
		friend class ProjectData;
		struct SkillLearned
		{
			int levelLearned;
			int skillID;
		};
		enum ResistLevel {RESISTLEVEL_E=0, RESISTLEVEL_D, RESISTLEVEL_C, RESISTLEVEL_B, RESISTLEVEL_A};
		enum AILevel {AILEVEL_BASIC=0, AILEVEL_INTERMEDIATE, AILEVEL_ADVANCED, AILEVEL_CUSTOM};
		Character();
		Character(Character &other);
		Character(const Character &other);
		Character(XMLNode CharacterNode);
		Character(StorageFile &storageFile);
		~Character();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getCharacterNode();
		BattleEvent *getBattleEventPointer();
		BattleEvent *getCustomAIPointer();
		QString getName();
		void setName(QString value);
		QString getFaceGraphicLocation();
		void setFaceGraphicLocation(QString location);
		int numSkillsLearned();
		SkillLearned *getSkillLearnedPointer(int skillLearnedID);
		void addSkillLearned(SkillLearned skillLearned);
		void deleteSkillLearned(int skillLearnedID);
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
		int getMaxHP(int level);
		void setMaxHP(int level, int value);
		int getMaxMP(int level);
		void setMaxMP(int level, int value);
		int getAttack(int level);
		void setAttack(int level, int value);
		int getDefense(int level);
		void setDefense(int level, int value);
		int getWisdom(int level);
		void setWisdom(int level, int value);
		int getAgility(int level);
		void setAgility(int level, int value);
		int getBattleCommand(int num);
		void setBattleCommand(int num, int value);
		int getMinLevel();
		void setMinLevel(int value);
		int getMaxLevel();
		void setMaxLevel(int value);
		int getClassID();
		void setClassID(int value);
		int getSpriteID();
		void setSpriteID(int value);
		int getExperiencePrimary();
		void setExperiencePrimary(int value);
		int getExperienceSecondary();
		void setExperienceSecondary(int value);
		int getWeaponID();
		void setWeaponID(int value);
		int getShieldID();
		void setShieldID(int value);
		int getArmorID();
		void setArmorID(int value);
		int getHelmetID();
		void setHelmetID(int value);
		int getAccessoryID();
		void setAccessoryID(int value);
		int getUnarmedBattleAnimationID();
		void setUnarmedBattleAnimationID(int value);
		int getAILevel();
		void setAILevel(int value);
		int getSpriteOpacity();
		void setSpriteOpacity(int value);
		bool isDuelWielding();
		void setDuelWielding(bool value);
		bool isAIControlled();
		void setAIControlled(bool value);
		bool isEquipmentLocked();
		void setEquipmentLocked(bool value);
		bool isMightyGuard();
		void setMightyGuard(bool value);
		bool isCriticalHits();
		void setCriticalHits(bool value);
		int getTotalExperience(int level);
		static bool tryLoad(XMLNode CharacterNode);
	private:
		QString name;
		QString faceGraphicLocation;
		QList<SkillLearned> skillProgression;
		QList<int> conditionResists;
		QList<int> attributeResists;
		BattleEvent *battleEvent;
		BattleEvent *customAI;
		float criticalHitRate;
		float criticalHitMultiplier;
		int maxHP[99];
		int maxMP[99];
		int attack[99];
		int defense[99];
		int wisdom[99];
		int agility[99];
		int battleCommands[6];
		int minLevel;
		int maxLevel;
		int classID;
		int spriteID;
		int experiencePrimary;
		int experienceSecondary;
		int weaponID;
		int shieldID;
		int armorID;
		int helmetID;
		int accessoryID;
		int unarmedBattleAnimationID;
		int aiLevel;
		int spriteOpacity;
		bool duelWielding;
		bool aiControlled;
		bool equipmentLocked;
		bool mightyGuard;
		bool criticalHits;
};

#endif // PROJECTDATA_CHARACTER_H
