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

#ifndef PROJECTDATA_SKILL_H
#define PROJECTDATA_SKILL_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "audiomanager.h"
#include "storagefile.h"
#include "xmlParser.h"

class Skill
{
	public:
		friend class ProjectData;
		enum Target {TARGET_1ENEMY=0, TARGET_ALLENEMIES, TARGET_SELF, TARGET_1ALLY, TARGET_ALLALLIES};
		enum MovementBeforeAttack {MOVEMENT_NONE=0, MOVEMENT_STEPFORWARD, MOVEMENT_JUMPFORWARD, MOVEMENT_MOVETOTARGET};
		struct AnimationSettings
		{
			int movementBeforeAttack;
			int animationPose;
		};
		struct NormalData
		{
			QList<bool> attributes;
			QList<bool> conditions;
			QList<AnimationSettings*> characterAnimationSettings;
			QList<AnimationSettings*> monsterAnimationSettings;
			int target;
			int effectRating;
			int successRate;
			int battleAnimationID;
			int globalAnimationID;
			int attackInfluence;
			int wisdomInfluence;
			int variance;
			int percentConditionEffectivenessProbability;
			bool changeHP;
			bool changeMP;
			bool changeAttack;
			bool changeDefense;
			bool changeWisdom;
			bool changeAgility;
			bool absorb;
			bool ignoreDefense;
			bool inflictConditions;
			bool reduceAttributeResistance;
		};
		struct EscapeTeleportData
		{
			AudioManager::SoundEffectInfo soundEffect;
		};
		struct SwitchData
		{
			AudioManager::SoundEffectInfo soundEffect;
			int switchID;
			bool allowField;
			bool allowBattle;
		};
		Skill();
		Skill(Skill &other);
		Skill(const Skill &other);
		Skill(XMLNode SkillNode);
		Skill(StorageFile &storageFile);
		~Skill();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getSkillNode();
		QString getName();
		void setName(QString value);
		QString getDescription();
		void setDescription(QString value);
		void getNormalData(NormalData *data);
		void setNormalData(NormalData *data);
		void getEscapeTeleportData(EscapeTeleportData *data);
		void setEscapeTeleportData(EscapeTeleportData *data);
		void getSwitchData(SwitchData *data);
		void setSwitchData(SwitchData *data);
		int getType();
		void setType(int value);
		void changeTypeRetainingSkillType(int newType);
		int getMPConsumed();
		void setMPConsumed(int value);
		bool isMPNumber();
		void setMPIsNumber(bool value);
		static bool tryLoad(XMLNode SkillNode);
	private:
		QString name;
		QString description;
		NormalData *normalData;
		EscapeTeleportData *escapeTeleportData;
		SwitchData *switchData;
		int type;
		int mpConsumed;
		bool mpIsNumber;
};

#endif // PROJECTDATA_SKILL_H
