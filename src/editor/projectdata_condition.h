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

#ifndef PROJECTDATA_CONDITION_H
#define PROJECTDATA_CONDITION_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class Condition
{
	public:
		friend class ProjectData;
		enum Extent {EXTENT_ENDSAFTERBATTLE=0, EXTENT_PERSISTSAFTERBATTLE};
		enum ActionRestriction
		{
			RESTRICTION_NONE=0,
			RESTRICTION_NOACTIONALLOWED,
			RESTRICTION_ATTACKALLIESRANDOMLY,
			RESTRICTION_ATTACKENEMIESRANDOMLY,
			RESTRICTION_ATTACKBOTHRANDOMLY
		};
		enum HPMPEffect {EFFECT_NONE=0, EFFECT_RECOVERY, EFFECT_DAMAGE};
		enum EffectChange {CHANGE_NOCHANGE=0, CHANGE_ALWAYS, CHANGE_NEVER};
		Condition();
		Condition(Condition &other);
		Condition(const Condition &other);
		Condition(XMLNode ConditionNode);
		Condition(StorageFile &storageFile);
		~Condition();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getConditionNode();
		QString getName();
		void setName(QString value);
		QString getIconLocation();
		void setIconLocation(QString value);
		int getExtent();
		void setExtent(int value);
		int getGroup();
		void setGroup(int value);
		int getPriority();
		void setPriority(int value);
		int getActionRestriction();
		void setActionRestriction(int value);
		int getHitPercentage();
		void setHitPercentage(int value);
		int getSusceptibilityA();
		void setSusceptibilityA(int value);
		int getSusceptibilityB();
		void setSusceptibilityB(int value);
		int getSusceptibilityC();
		void setSusceptibilityC(int value);
		int getSusceptibilityD();
		void setSusceptibilityD(int value);
		int getSusceptibilityE();
		void setSusceptibilityE(int value);
		int getAbatesAfterNumTurns();
		void setAbatesAfterNumTurns(int value);
		int getPerTurnRecoveryChance();
		void setPerTurnRecoveryChance(int value);
		int getPhysicalDamageRecoveryChance();
		void setPhysicalDamageRecoveryChance(int value);
		int getAttackAlteration();
		void setAttackAlteration(int value);
		int getDefenseAlteration();
		void setDefenseAlteration(int value);
		int getWisdomAlteration();
		void setWisdomAlteration(int value);
		int getAgilityAlteration();
		void setAgilityAlteration(int value);
		int getAttackInfluence();
		void setAttackInfluence(int value);
		int getWisdomInfluence();
		void setWisdomInfluence(int value);
		int getAnimationID();
		void setAnimationID(int value);
		int getHPEffect();
		void setHPEffect(int value);
		int getHPPercentPerTurn();
		void setHPPercentPerTurn(int value);
		int getHPAmountPerTurn();
		void setHPAmountPerTurn(int value);
		int getHPAmountPerNumSteps();
		void setHPAmountPerNumSteps(int value);
		int getHPNumSteps();
		void setHPNumSteps(int value);
		int getMPEffect();
		void setMPEffect(int value);
		int getMPPercentPerTurn();
		void setMPPercentPerTurn(int value);
		int getMPAmountPerTurn();
		void setMPAmountPerTurn(int value);
		int getMPAmountPerNumSteps();
		void setMPAmountPerNumSteps(int value);
		int getMPNumSteps();
		void setMPNumSteps(int value);
		int getEvadingChange();
		void setEvadingChange(int value);
		int getCriticalHitsChange();
		void setCriticalHitsChange(int value);
		bool getReflectSkills();
		void setReflectSkills(bool value);
		bool getLockEquipment();
		void setLockEquipment(bool value);
		bool getPreventSkillsWithAnAttackInfluence();
		void setPreventSkillsWithAnAttackInfluence(bool value);
		bool getPreventSkillsWithAWisdomInfluence();
		void setPreventSkillsWithAWisdomInfluence(bool value);
		static bool tryLoad(XMLNode ConditionNode);
	private:
		QString name;
		QString iconLocation;
		int extent;
		int group;
		int priority;
		int actionRestriction;
		int hitPercentage;
		int susceptibilityA;
		int susceptibilityB;
		int susceptibilityC;
		int susceptibilityD;
		int susceptibilityE;
		int abatesAfterNumTurns;
		int perTurnRecoveryChance;
		int physicalDamageRecoveryChance;
		int attackAlteration;
		int defenseAlteration;
		int wisdomAlteration;
		int agilityAlteration;
		int attackInfluence;
		int wisdomInfluence;
		int animationID;
		int hpEffect;
		int hpPercentPerTurn;
		int hpAmountPerTurn;
		int hpAmountPerNumSteps;
		int hpNumSteps;
		int mpEffect;
		int mpPercentPerTurn;
		int mpAmountPerTurn;
		int mpAmountPerNumSteps;
		int mpNumSteps;
		int evadingChange;
		int criticalHitsChange;
		bool reflectSkills;
		bool lockEquipment;
		bool preventSkillsWithAnAttackInfluence;
		bool preventSkillsWithAWisdomInfluence;
};

#endif // PROJECTDATA_CONDITION_H
