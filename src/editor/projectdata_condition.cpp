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

#include <QString>
#include <QStringList>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_condition.h"
#include "xmlParser.h"
#include "xmlTools.h"

Condition::Condition()
{
	name = "Untitled";
	iconLocation = "";
	extent = EXTENT_ENDSAFTERBATTLE;
	group = 0;
	priority = 50;
	actionRestriction = RESTRICTION_NONE;
	hitPercentage = 100;
	susceptibilityA = 100;
	susceptibilityB = 100;
	susceptibilityC = 100;
	susceptibilityD = 100;
	susceptibilityE = 100;
	abatesAfterNumTurns = 0;
	perTurnRecoveryChance = 0;
	physicalDamageRecoveryChance = 0;
	attackAlteration = 100;
	defenseAlteration = 100;
	wisdomAlteration = 100;
	agilityAlteration = 100;
	attackInfluence = 0;
	wisdomInfluence = 0;
	animationID = 0;
	hpEffect = EFFECT_NONE;
	hpPercentPerTurn = 0;
	hpAmountPerTurn = 0;
	hpAmountPerNumSteps = 0;
	hpNumSteps = 1;
	mpEffect = EFFECT_NONE;
	mpPercentPerTurn = 0;
	mpAmountPerTurn = 0;
	mpAmountPerNumSteps = 0;
	mpNumSteps = 1;
	evadingChange = CHANGE_NOCHANGE;
	criticalHitsChange = CHANGE_NOCHANGE;
	reflectSkills = false;
	lockEquipment = false;
	preventSkillsWithAnAttackInfluence = false;
	preventSkillsWithAWisdomInfluence = false;
}

Condition::Condition(Condition &other)
{
	name = other.name;
	iconLocation = other.iconLocation;
	extent = other.extent;
	group = other.group;
	priority = other.priority;
	actionRestriction = other.actionRestriction;
	hitPercentage = other.hitPercentage;
	susceptibilityA = other.susceptibilityA;
	susceptibilityB = other.susceptibilityB;
	susceptibilityC = other.susceptibilityC;
	susceptibilityD = other.susceptibilityD;
	susceptibilityE = other.susceptibilityE;
	abatesAfterNumTurns = other.abatesAfterNumTurns;
	perTurnRecoveryChance = other.perTurnRecoveryChance;
	physicalDamageRecoveryChance = other.physicalDamageRecoveryChance;
	attackAlteration = other.attackAlteration;
	defenseAlteration = other.defenseAlteration;
	wisdomAlteration = other.wisdomAlteration;
	agilityAlteration = other.agilityAlteration;
	attackInfluence = other.attackInfluence;
	wisdomInfluence = other.wisdomInfluence;
	animationID = other.animationID;
	hpEffect = other.hpEffect;
	hpPercentPerTurn = other.hpPercentPerTurn;
	hpAmountPerTurn = other.hpAmountPerTurn;
	hpAmountPerNumSteps = other.hpAmountPerNumSteps;
	hpNumSteps = other.hpNumSteps;
	mpEffect = other.mpEffect;
	mpPercentPerTurn = other.mpPercentPerTurn;
	mpAmountPerTurn = other.mpAmountPerTurn;
	mpAmountPerNumSteps = other.mpAmountPerNumSteps;
	mpNumSteps = other.mpNumSteps;
	evadingChange = other.evadingChange;
	criticalHitsChange = other.criticalHitsChange;
	reflectSkills = other.reflectSkills;
	lockEquipment = other.lockEquipment;
	preventSkillsWithAnAttackInfluence = other.preventSkillsWithAnAttackInfluence;
	preventSkillsWithAWisdomInfluence = other.preventSkillsWithAWisdomInfluence;
}

Condition::Condition(const Condition &other)
{
	name = other.name;
	iconLocation = other.iconLocation;
	extent = other.extent;
	group = other.group;
	priority = other.priority;
	actionRestriction = other.actionRestriction;
	hitPercentage = other.hitPercentage;
	susceptibilityA = other.susceptibilityA;
	susceptibilityB = other.susceptibilityB;
	susceptibilityC = other.susceptibilityC;
	susceptibilityD = other.susceptibilityD;
	susceptibilityE = other.susceptibilityE;
	abatesAfterNumTurns = other.abatesAfterNumTurns;
	perTurnRecoveryChance = other.perTurnRecoveryChance;
	physicalDamageRecoveryChance = other.physicalDamageRecoveryChance;
	attackAlteration = other.attackAlteration;
	defenseAlteration = other.defenseAlteration;
	wisdomAlteration = other.wisdomAlteration;
	agilityAlteration = other.agilityAlteration;
	attackInfluence = other.attackInfluence;
	wisdomInfluence = other.wisdomInfluence;
	animationID = other.animationID;
	hpEffect = other.hpEffect;
	hpPercentPerTurn = other.hpPercentPerTurn;
	hpAmountPerTurn = other.hpAmountPerTurn;
	hpAmountPerNumSteps = other.hpAmountPerNumSteps;
	hpNumSteps = other.hpNumSteps;
	mpEffect = other.mpEffect;
	mpPercentPerTurn = other.mpPercentPerTurn;
	mpAmountPerTurn = other.mpAmountPerTurn;
	mpAmountPerNumSteps = other.mpAmountPerNumSteps;
	mpNumSteps = other.mpNumSteps;
	evadingChange = other.evadingChange;
	criticalHitsChange = other.criticalHitsChange;
	reflectSkills = other.reflectSkills;
	lockEquipment = other.lockEquipment;
	preventSkillsWithAnAttackInfluence = other.preventSkillsWithAnAttackInfluence;
	preventSkillsWithAWisdomInfluence = other.preventSkillsWithAWisdomInfluence;
}

Condition::Condition(XMLNode ConditionNode)
{
	XMLNode TempNode;
	QString temp_string;
	name = ConditionNode.getAttribute("name");
	iconLocation = ProjectData::getAbsoluteResourcePath(ConditionNode.getAttribute("icon"));
	extent = QString(ConditionNode.getAttribute("extent")).toInt();
	group = QString(ConditionNode.getAttribute("group")).toInt();
	priority = QString(ConditionNode.getAttribute("priority")).toInt();
	actionRestriction = QString(ConditionNode.getAttribute("actionRestriction")).toInt();
	hitPercentage = QString(ConditionNode.getAttribute("hitPercentage")).toInt();
	animationID = QString(ConditionNode.getAttribute("animationID")).toInt();
	TempNode = ConditionNode.getChildNode("susceptibility");
	susceptibilityA = QString(TempNode.getAttribute("A")).toInt();
	susceptibilityB = QString(TempNode.getAttribute("B")).toInt();
	susceptibilityC = QString(TempNode.getAttribute("C")).toInt();
	susceptibilityD = QString(TempNode.getAttribute("D")).toInt();
	susceptibilityE = QString(TempNode.getAttribute("E")).toInt();
	TempNode = ConditionNode.getChildNode("recovery");
	abatesAfterNumTurns = QString(TempNode.getAttribute("abatesAfterNumTurns")).toInt();
	abatesAfterNumTurns = QString(TempNode.getAttribute("abatesAfterNumTurns")).toInt();
	perTurnRecoveryChance = QString(TempNode.getAttribute("perTurnRecoveryChance")).toInt();
	physicalDamageRecoveryChance = QString(TempNode.getAttribute("physicalDamageRecoveryChance")).toInt();
	TempNode = ConditionNode.getChildNode("statAlterations");
	attackAlteration = QString(TempNode.getAttribute("attack")).toInt();
	defenseAlteration = QString(TempNode.getAttribute("defense")).toInt();
	wisdomAlteration = QString(TempNode.getAttribute("wisdom")).toInt();
	agilityAlteration = QString(TempNode.getAttribute("agility")).toInt();
	TempNode = ConditionNode.getChildNode("preventSkills");
	temp_string = TempNode.getAttribute("withAttackInfluence");
	preventSkillsWithAnAttackInfluence = (temp_string == "true") ? true:false;
	attackInfluence = QString(TempNode.getAttribute("attackInfluence")).toInt();
	temp_string = TempNode.getAttribute("withWisdomInfluence");
	preventSkillsWithAWisdomInfluence = (temp_string == "true") ? true:false;
	wisdomInfluence = QString(TempNode.getAttribute("wisdomInfluence")).toInt();
	TempNode = ConditionNode.getChildNode("hpEffect");
	hpEffect = QString(TempNode.getAttribute("effect")).toInt();
	hpPercentPerTurn = QString(TempNode.getAttribute("percentPerTurn")).toInt();
	hpAmountPerTurn = QString(TempNode.getAttribute("amountPerTurn")).toInt();
	hpAmountPerNumSteps = QString(TempNode.getAttribute("amountPerNumSteps")).toInt();
	hpNumSteps = QString(TempNode.getAttribute("numSteps")).toInt();
	TempNode = ConditionNode.getChildNode("mpEffect");
	mpEffect = QString(TempNode.getAttribute("effect")).toInt();
	mpPercentPerTurn = QString(TempNode.getAttribute("percentPerTurn")).toInt();
	mpAmountPerTurn = QString(TempNode.getAttribute("amountPerTurn")).toInt();
	mpAmountPerNumSteps = QString(TempNode.getAttribute("amountPerNumSteps")).toInt();
	mpNumSteps = QString(TempNode.getAttribute("numSteps")).toInt();
	TempNode = ConditionNode.getChildNode("additionalEffects");
	evadingChange = QString(TempNode.getAttribute("evadingChange")).toInt();
	criticalHitsChange = QString(TempNode.getAttribute("criticalHitsChange")).toInt();
	temp_string = TempNode.getAttribute("reflectSkills");
	reflectSkills = (temp_string == "true") ? true:false;
	temp_string = TempNode.getAttribute("lockEquipment");
	lockEquipment = (temp_string == "true") ? true:false;
}

Condition::Condition(StorageFile &storageFile)
{
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	iconLocation = temp_string;
	delete[] temp_string;
	extent = storageFile.getSignedInt();
	group = storageFile.getSignedInt();
	priority = storageFile.getSignedInt();
	actionRestriction = storageFile.getSignedInt();
	hitPercentage = storageFile.getSignedInt();
	susceptibilityA = storageFile.getSignedInt();
	susceptibilityB = storageFile.getSignedInt();
	susceptibilityC = storageFile.getSignedInt();
	susceptibilityD = storageFile.getSignedInt();
	susceptibilityE = storageFile.getSignedInt();
	abatesAfterNumTurns = storageFile.getSignedInt();
	perTurnRecoveryChance = storageFile.getSignedInt();
	physicalDamageRecoveryChance = storageFile.getSignedInt();
	attackAlteration = storageFile.getSignedInt();
	defenseAlteration = storageFile.getSignedInt();
	wisdomAlteration = storageFile.getSignedInt();
	agilityAlteration = storageFile.getSignedInt();
	attackInfluence = storageFile.getSignedInt();
	wisdomInfluence = storageFile.getSignedInt();
	animationID = storageFile.getSignedInt();
	hpEffect = storageFile.getSignedInt();
	hpPercentPerTurn = storageFile.getSignedInt();
	hpAmountPerTurn = storageFile.getSignedInt();
	hpAmountPerNumSteps = storageFile.getSignedInt();
	hpNumSteps = storageFile.getSignedInt();
	mpEffect = storageFile.getSignedInt();
	mpPercentPerTurn = storageFile.getSignedInt();
	mpAmountPerTurn = storageFile.getSignedInt();
	mpAmountPerNumSteps = storageFile.getSignedInt();
	mpNumSteps = storageFile.getSignedInt();
	evadingChange = storageFile.getSignedInt();
	criticalHitsChange = storageFile.getSignedInt();
	reflectSkills = storageFile.getBool();
	lockEquipment = storageFile.getBool();
	preventSkillsWithAnAttackInfluence = storageFile.getBool();
	preventSkillsWithAWisdomInfluence = storageFile.getBool();
}

Condition::~Condition()
{
}

void Condition::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putString(iconLocation.toUtf8().data());
	storageFile.putSignedInt(extent);
	storageFile.putSignedInt(group);
	storageFile.putSignedInt(priority);
	storageFile.putSignedInt(actionRestriction);
	storageFile.putSignedInt(hitPercentage);
	storageFile.putSignedInt(susceptibilityA);
	storageFile.putSignedInt(susceptibilityB);
	storageFile.putSignedInt(susceptibilityC);
	storageFile.putSignedInt(susceptibilityD);
	storageFile.putSignedInt(susceptibilityE);
	storageFile.putSignedInt(abatesAfterNumTurns);
	storageFile.putSignedInt(perTurnRecoveryChance);
	storageFile.putSignedInt(physicalDamageRecoveryChance);
	storageFile.putSignedInt(attackAlteration);
	storageFile.putSignedInt(defenseAlteration);
	storageFile.putSignedInt(wisdomAlteration);
	storageFile.putSignedInt(agilityAlteration);
	storageFile.putSignedInt(attackInfluence);
	storageFile.putSignedInt(wisdomInfluence);
	storageFile.putSignedInt(animationID);
	storageFile.putSignedInt(hpEffect);
	storageFile.putSignedInt(hpPercentPerTurn);
	storageFile.putSignedInt(hpAmountPerTurn);
	storageFile.putSignedInt(hpAmountPerNumSteps);
	storageFile.putSignedInt(hpNumSteps);
	storageFile.putSignedInt(mpEffect);
	storageFile.putSignedInt(mpPercentPerTurn);
	storageFile.putSignedInt(mpAmountPerTurn);
	storageFile.putSignedInt(mpAmountPerNumSteps);
	storageFile.putSignedInt(mpNumSteps);
	storageFile.putSignedInt(evadingChange);
	storageFile.putSignedInt(criticalHitsChange);
	storageFile.putBool(reflectSkills);
	storageFile.putBool(lockEquipment);
	storageFile.putBool(preventSkillsWithAnAttackInfluence);
	storageFile.putBool(preventSkillsWithAWisdomInfluence);
}

QStringList Condition::getResources()
{
	return QStringList() << iconLocation;
}

XMLNode Condition::getConditionNode()
{
	XMLNode TempNode, ConditionNode = XMLNode::createXMLTopNode("condition");
	ConditionNode.addAttribute("name", name.toUtf8().data());
	ConditionNode.addAttribute("icon", ProjectData::getRelativeResourcePath(iconLocation).toUtf8().data());
	ConditionNode.addAttribute("extent", QString::number(extent).toUtf8().data());
	ConditionNode.addAttribute("group", QString::number(group).toUtf8().data());
	ConditionNode.addAttribute("priority", QString::number(priority).toUtf8().data());
	ConditionNode.addAttribute("actionRestriction", QString::number(actionRestriction).toUtf8().data());
	ConditionNode.addAttribute("hitPercentage", QString::number(hitPercentage).toUtf8().data());
	ConditionNode.addAttribute("animationID", QString::number(animationID).toUtf8().data());
	TempNode = ConditionNode.addChild("susceptibility");
	TempNode.addAttribute("A", QString::number(susceptibilityA).toUtf8().data());
	TempNode.addAttribute("B", QString::number(susceptibilityB).toUtf8().data());
	TempNode.addAttribute("C", QString::number(susceptibilityC).toUtf8().data());
	TempNode.addAttribute("D", QString::number(susceptibilityD).toUtf8().data());
	TempNode.addAttribute("E", QString::number(susceptibilityE).toUtf8().data());
	TempNode = ConditionNode.addChild("recovery");
	TempNode.addAttribute("abatesAfterNumTurns", QString::number(abatesAfterNumTurns).toUtf8().data());
	TempNode.addAttribute("abatesAfterNumTurns", QString::number(abatesAfterNumTurns).toUtf8().data());
	TempNode.addAttribute("perTurnRecoveryChance", QString::number(perTurnRecoveryChance).toUtf8().data());
	TempNode.addAttribute("physicalDamageRecoveryChance", QString::number(physicalDamageRecoveryChance).toUtf8().data());
	TempNode = ConditionNode.addChild("statAlterations");
	TempNode.addAttribute("attack", QString::number(attackAlteration).toUtf8().data());
	TempNode.addAttribute("defense", QString::number(defenseAlteration).toUtf8().data());
	TempNode.addAttribute("wisdom", QString::number(wisdomAlteration).toUtf8().data());
	TempNode.addAttribute("agility", QString::number(agilityAlteration).toUtf8().data());
	TempNode = ConditionNode.addChild("preventSkills");
	TempNode.addAttribute("withAttackInfluence", (preventSkillsWithAnAttackInfluence) ? "true":"false");
	TempNode.addAttribute("attackInfluence", QString::number(attackInfluence).toUtf8().data());
	TempNode.addAttribute("withWisdomInfluence", (preventSkillsWithAWisdomInfluence) ? "true":"false");
	TempNode.addAttribute("wisdomInfluence", QString::number(wisdomInfluence).toUtf8().data());
	TempNode = ConditionNode.addChild("hpEffect");
	TempNode.addAttribute("effect", QString::number(hpEffect).toUtf8().data());
	TempNode.addAttribute("percentPerTurn", QString::number(hpPercentPerTurn).toUtf8().data());
	TempNode.addAttribute("amountPerTurn", QString::number(hpAmountPerTurn).toUtf8().data());
	TempNode.addAttribute("amountPerNumSteps", QString::number(hpAmountPerNumSteps).toUtf8().data());
	TempNode.addAttribute("numSteps", QString::number(hpNumSteps).toUtf8().data());
	TempNode = ConditionNode.addChild("mpEffect");
	TempNode.addAttribute("effect", QString::number(mpEffect).toUtf8().data());
	TempNode.addAttribute("percentPerTurn", QString::number(mpPercentPerTurn).toUtf8().data());
	TempNode.addAttribute("amountPerTurn", QString::number(mpAmountPerTurn).toUtf8().data());
	TempNode.addAttribute("amountPerNumSteps", QString::number(mpAmountPerNumSteps).toUtf8().data());
	TempNode.addAttribute("numSteps", QString::number(mpNumSteps).toUtf8().data());
	TempNode = ConditionNode.addChild("additionalEffects");
	TempNode.addAttribute("evadingChange", QString::number(evadingChange).toUtf8().data());
	TempNode.addAttribute("criticalHitsChange", QString::number(criticalHitsChange).toUtf8().data());
	TempNode.addAttribute("reflectSkills", (reflectSkills) ? "true":"false");
	TempNode.addAttribute("lockEquipment", (lockEquipment) ? "true":"false");
	return ConditionNode;
}

QString Condition::getName()
{
	return name;
}

void Condition::setName(QString value)
{
	name = value;
}

QString Condition::getIconLocation()
{
	return iconLocation;
}

void Condition::setIconLocation(QString value)
{
	iconLocation = value;
}

int Condition::getExtent()
{
	return extent;
}

void Condition::setExtent(int value)
{
	extent = value;
}

int Condition::getGroup()
{
	return group;
}

void Condition::setGroup(int value)
{
	group = value;
}

int Condition::getPriority()
{
	return priority;
}

void Condition::setPriority(int value)
{
	priority = value;
}

int Condition::getActionRestriction()
{
	return actionRestriction;
}

void Condition::setActionRestriction(int value)
{
	actionRestriction = value;
}

int Condition::getHitPercentage()
{
	return hitPercentage;
}

void Condition::setHitPercentage(int value)
{
	hitPercentage = value;
}

int Condition::getSusceptibilityA()
{
	return susceptibilityA;
}

void Condition::setSusceptibilityA(int value)
{
	susceptibilityA = value;
}

int Condition::getSusceptibilityB()
{
	return susceptibilityB;
}

void Condition::setSusceptibilityB(int value)
{
	susceptibilityB = value;
}

int Condition::getSusceptibilityC()
{
	return susceptibilityC;
}

void Condition::setSusceptibilityC(int value)
{
	susceptibilityC = value;
}

int Condition::getSusceptibilityD()
{
	return susceptibilityD;
}

void Condition::setSusceptibilityD(int value)
{
	susceptibilityD = value;
}

int Condition::getSusceptibilityE()
{
	return susceptibilityE;
}

void Condition::setSusceptibilityE(int value)
{
	susceptibilityE = value;
}

int Condition::getAbatesAfterNumTurns()
{
	return abatesAfterNumTurns;
}

void Condition::setAbatesAfterNumTurns(int value)
{
	abatesAfterNumTurns = value;
}

int Condition::getPerTurnRecoveryChance()
{
	return perTurnRecoveryChance;
}

void Condition::setPerTurnRecoveryChance(int value)
{
	perTurnRecoveryChance = value;
}

int Condition::getPhysicalDamageRecoveryChance()
{
	return physicalDamageRecoveryChance;
}

void Condition::setPhysicalDamageRecoveryChance(int value)
{
	physicalDamageRecoveryChance = value;
}

int Condition::getAttackAlteration()
{
	return attackAlteration;
}

void Condition::setAttackAlteration(int value)
{
	attackAlteration = value;
}

int Condition::getDefenseAlteration()
{
	return defenseAlteration;
}

void Condition::setDefenseAlteration(int value)
{
	defenseAlteration = value;
}

int Condition::getWisdomAlteration()
{
	return wisdomAlteration;
}

void Condition::setWisdomAlteration(int value)
{
	wisdomAlteration = value;
}

int Condition::getAgilityAlteration()
{
	return agilityAlteration;
}

void Condition::setAgilityAlteration(int value)
{
	agilityAlteration = value;
}

int Condition::getAttackInfluence()
{
	return attackInfluence;
}

void Condition::setAttackInfluence(int value)
{
	attackInfluence = value;
}

int Condition::getWisdomInfluence()
{
	return wisdomInfluence;
}

void Condition::setWisdomInfluence(int value)
{
	wisdomInfluence = value;
}

int Condition::getAnimationID()
{
	return animationID;
}

void Condition::setAnimationID(int value)
{
	animationID = value;
}

int Condition::getHPEffect()
{
	return hpEffect;
}

void Condition::setHPEffect(int value)
{
	hpEffect = value;
}

int Condition::getHPPercentPerTurn()
{
	return hpPercentPerTurn;
}

void Condition::setHPPercentPerTurn(int value)
{
	hpPercentPerTurn = value;
}

int Condition::getHPAmountPerTurn()
{
	return hpAmountPerTurn;
}

void Condition::setHPAmountPerTurn(int value)
{
	hpAmountPerTurn = value;
}

int Condition::getHPAmountPerNumSteps()
{
	return hpAmountPerNumSteps;
}

void Condition::setHPAmountPerNumSteps(int value)
{
	hpAmountPerNumSteps = value;
}

int Condition::getHPNumSteps()
{
	return hpNumSteps;
}

void Condition::setHPNumSteps(int value)
{
	hpNumSteps = value;
}

int Condition::getMPEffect()
{
	return mpEffect;
}

void Condition::setMPEffect(int value)
{
	mpEffect = value;
}

int Condition::getMPPercentPerTurn()
{
	return mpPercentPerTurn;
}

void Condition::setMPPercentPerTurn(int value)
{
	mpPercentPerTurn = value;
}

int Condition::getMPAmountPerTurn()
{
	return mpAmountPerTurn;
}

void Condition::setMPAmountPerTurn(int value)
{
	mpAmountPerTurn = value;
}

int Condition::getMPAmountPerNumSteps()
{
	return mpAmountPerNumSteps;
}

void Condition::setMPAmountPerNumSteps(int value)
{
	mpAmountPerNumSteps = value;
}

int Condition::getMPNumSteps()
{
	return mpNumSteps;
}

void Condition::setMPNumSteps(int value)
{
	mpNumSteps = value;
}

int Condition::getEvadingChange()
{
	return evadingChange;
}

void Condition::setEvadingChange(int value)
{
	evadingChange = value;
}

int Condition::getCriticalHitsChange()
{
	return criticalHitsChange;
}

void Condition::setCriticalHitsChange(int value)
{
	criticalHitsChange = value;
}

bool Condition::getReflectSkills()
{
	return reflectSkills;
}

void Condition::setReflectSkills(bool value)
{
	reflectSkills = value;
}

bool Condition::getLockEquipment()
{
	return lockEquipment;
}

void Condition::setLockEquipment(bool value)
{
	lockEquipment = value;
}

bool Condition::getPreventSkillsWithAnAttackInfluence()
{
	return preventSkillsWithAnAttackInfluence;
}

void Condition::setPreventSkillsWithAnAttackInfluence(bool value)
{
	preventSkillsWithAnAttackInfluence = value;
}

bool Condition::getPreventSkillsWithAWisdomInfluence()
{
	return preventSkillsWithAWisdomInfluence;
}

void Condition::setPreventSkillsWithAWisdomInfluence(bool value)
{
	preventSkillsWithAWisdomInfluence = value;
}

bool Condition::tryLoad(XMLNode ConditionNode)
{
	XMLNode TempNode;
	QStringList trueFalseList;
	trueFalseList << "true";
	trueFalseList << "false";
	if (!XMLTools::attributeExists(ConditionNode, "name"))
		return false;
	if (!XMLTools::attributeExists(ConditionNode, "icon"))
		return false;
	if (!XMLTools::attributeExists(ConditionNode, "extent"))
		return false;
	if (!XMLTools::attributeIntRangeValid(ConditionNode, "extent", 0, 1))
		return false;
	if (!XMLTools::attributeExists(ConditionNode, "group"))
		return false;
	if (!XMLTools::attributeIntRangeValid(ConditionNode, "group", 0, 8))
		return false;
	if (!XMLTools::attributeExists(ConditionNode, "priority"))
		return false;
	if (!XMLTools::attributeIntRangeValid(ConditionNode, "priority", 0, 100))
		return false;
	if (!XMLTools::attributeExists(ConditionNode, "actionRestriction"))
		return false;
	if (!XMLTools::attributeIntRangeValid(ConditionNode, "actionRestriction", 0, 4))
		return false;
	if (!XMLTools::attributeExists(ConditionNode, "hitPercentage"))
		return false;
	if (!XMLTools::attributeIntRangeValid(ConditionNode, "hitPercentage", 0, 100))
		return false;
	if (!XMLTools::attributeExists(ConditionNode, "animationID"))
		return false;
	if (!XMLTools::attributeIntRangeValid(ConditionNode, "animationID", 0, 31))
		return false;
	if (!XMLTools::nodeExists(ConditionNode, "susceptibility"))
		return false;
	TempNode = ConditionNode.getChildNode("susceptibility");
	if (!XMLTools::attributeExists(TempNode, "A"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "A", 0, 100))
		return false;
	if (!XMLTools::attributeExists(TempNode, "B"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "B", 0, 100))
		return false;
	if (!XMLTools::attributeExists(TempNode, "C"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "C", 0, 100))
		return false;
	if (!XMLTools::attributeExists(TempNode, "D"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "D", 0, 100))
		return false;
	if (!XMLTools::attributeExists(TempNode, "E"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "E", 0, 100))
		return false;
	if (!XMLTools::nodeExists(ConditionNode, "recovery"))
		return false;
	TempNode = ConditionNode.getChildNode("recovery");
	if (!XMLTools::attributeExists(TempNode, "abatesAfterNumTurns"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "abatesAfterNumTurns", 0, 999))
		return false;
	if (!XMLTools::attributeExists(TempNode, "perTurnRecoveryChance"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "perTurnRecoveryChance", 0, 100))
		return false;
	if (!XMLTools::attributeExists(TempNode, "physicalDamageRecoveryChance"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "physicalDamageRecoveryChance", 0, 100))
		return false;
	if (!XMLTools::nodeExists(ConditionNode, "statAlterations"))
		return false;
	TempNode = ConditionNode.getChildNode("statAlterations");
	if (!XMLTools::attributeExists(TempNode, "attack"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "attack", 0, 200))
		return false;
	if (!XMLTools::attributeExists(TempNode, "defense"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "defense", 0, 200))
		return false;
	if (!XMLTools::attributeExists(TempNode, "wisdom"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "wisdom", 0, 200))
		return false;
	if (!XMLTools::attributeExists(TempNode, "agility"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "agility", 0, 200))
		return false;
	if (!XMLTools::nodeExists(ConditionNode, "preventSkills"))
		return false;
	TempNode = ConditionNode.getChildNode("preventSkills");
	if (!XMLTools::attributeExists(TempNode, "withAttackInfluence"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "withAttackInfluence", trueFalseList, 0))
		return false;
	if (!XMLTools::attributeExists(TempNode, "attackInfluence"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "attackInfluence", 0, 999))
		return false;
	if (!XMLTools::attributeExists(TempNode, "withWisdomInfluence"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "withWisdomInfluence", trueFalseList, 0))
		return false;
	if (!XMLTools::attributeExists(TempNode, "wisdomInfluence"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "wisdomInfluence", 0, 999))
		return false;
	if (!XMLTools::nodeExists(ConditionNode, "hpEffect"))
		return false;
	TempNode = ConditionNode.getChildNode("hpEffect");
	if (!XMLTools::attributeExists(TempNode, "effect"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "effect", 0, 2))
		return false;
	if (!XMLTools::attributeExists(TempNode, "percentPerTurn"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "percentPerTurn", 0, 100))
		return false;
	if (!XMLTools::attributeExists(TempNode, "amountPerTurn"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "amountPerTurn", 0, 9999))
		return false;
	if (!XMLTools::attributeExists(TempNode, "amountPerNumSteps"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "amountPerNumSteps", 0, 9999))
		return false;
	if (!XMLTools::attributeExists(TempNode, "numSteps"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "numSteps", 1, 999))
		return false;
	if (!XMLTools::nodeExists(ConditionNode, "mpEffect"))
		return false;
	TempNode = ConditionNode.getChildNode("mpEffect");
	if (!XMLTools::attributeExists(TempNode, "effect"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "effect", 0, 2))
		return false;
	if (!XMLTools::attributeExists(TempNode, "percentPerTurn"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "percentPerTurn", 0, 100))
		return false;
	if (!XMLTools::attributeExists(TempNode, "amountPerTurn"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "amountPerTurn", 0, 999))
		return false;
	if (!XMLTools::attributeExists(TempNode, "amountPerNumSteps"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "amountPerNumSteps", 0, 999))
		return false;
	if (!XMLTools::attributeExists(TempNode, "numSteps"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "numSteps", 1, 999))
		return false;
	if (!XMLTools::nodeExists(ConditionNode, "additionalEffects"))
		return false;
	TempNode = ConditionNode.getChildNode("additionalEffects");
	if (!XMLTools::attributeExists(TempNode, "evadingChange"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "evadingChange", 0, 2))
		return false;
	if (!XMLTools::attributeExists(TempNode, "criticalHitsChange"))
		return false;
	if (!XMLTools::attributeIntRangeValid(TempNode, "criticalHitsChange", 0, 2))
		return false;
	if (!XMLTools::attributeExists(TempNode, "reflectSkills"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "reflectSkills", trueFalseList, 0))
		return false;
	if (!XMLTools::attributeExists(TempNode, "lockEquipment"))
		return false;
	if (!XMLTools::attributeStringValid(TempNode, "lockEquipment", trueFalseList, 0))
		return false;
	return true;
}
