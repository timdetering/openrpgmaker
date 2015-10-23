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

#ifndef PROJECTDATA_CLASS_H
#define PROJECTDATA_CLASS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class Class
{
	public:
		friend class ProjectData;
		struct SkillLearned
		{
			int levelLearned;
			int skillID;
		};
		enum ResistLevel {RESISTLEVEL_E=0, RESISTLEVEL_D, RESISTLEVEL_C, RESISTLEVEL_B, RESISTLEVEL_A};
		Class();
		Class(Class &other);
		Class(const Class &other);
		Class(XMLNode ClassNode);
		Class(StorageFile &storageFile);
		~Class();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getClassNode();
		QString getName();
		void setName(QString value);
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
		int getSpriteID();
		void setSpriteID(int value);
		int getSpriteOpacity();
		void setSpriteOpacity(int value);
		int getExperiencePrimary();
		void setExperiencePrimary(int value);
		int getExperienceSecondary();
		void setExperienceSecondary(int value);
		bool isDuelWielding();
		void setDuelWielding(bool value);
		bool isAIControlled();
		void setAIControlled(bool value);
		bool isEquipmentLocked();
		void setEquipmentLocked(bool value);
		bool isMightyGuard();
		void setMightyGuard(bool value);
		static bool tryLoad(XMLNode ClassNode);
	private:
		QString name;
		QList<SkillLearned> skillProgression;
		QList<int> conditionResists;
		QList<int> attributeResists;
		int maxHP[99];
		int maxMP[99];
		int attack[99];
		int defense[99];
		int wisdom[99];
		int agility[99];
		int battleCommands[6];
		int spriteID;
		int spriteOpacity;
		int experiencePrimary;
		int experienceSecondary;
		bool duelWielding;
		bool aiControlled;
		bool equipmentLocked;
		bool mightyGuard;
};

#endif // PROJECTDATA_CLASS_H
