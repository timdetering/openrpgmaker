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
#include <QRegExp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_skill.h"
#include "xmlParser.h"
#include "xmlTools.h"

Skill::Skill()
{
	name = "Untitled";
	description = "";
	normalData = new NormalData;
	escapeTeleportData = NULL;
	switchData = NULL;
	for (int i = 0; i < ProjectData::attributeNames.size(); ++i)
		normalData->attributes.append(false);
	for (int i = 0; i < ProjectData::conditionNames.size(); ++i)
		normalData->conditions.append(false);
	for (int i = 0; i < ProjectData::characterNames.size(); ++i)
	{
		AnimationSettings *settings = new AnimationSettings;
		settings->movementBeforeAttack = MOVEMENT_NONE;
		settings->animationPose = 0;
		normalData->characterAnimationSettings.append(settings);
	}
	for (int i = 0; i < ProjectData::monsterNames.size(); ++i)
	{
		AnimationSettings *settings = new AnimationSettings;
		settings->movementBeforeAttack = MOVEMENT_NONE;
		settings->animationPose = 0;
		normalData->monsterAnimationSettings.append(settings);
	}
	normalData->target = TARGET_1ENEMY;
	normalData->effectRating = 0;
	normalData->successRate = 0;
	normalData->battleAnimationID = 0;
	normalData->globalAnimationID = 0;
	normalData->attackInfluence = 0;
	normalData->wisdomInfluence = 0;
	normalData->variance = 0;
	normalData->percentConditionEffectivenessProbability = 0;
	normalData->changeHP = false;
	normalData->changeMP = false;
	normalData->changeAttack = false;
	normalData->changeDefense = false;
	normalData->changeWisdom = false;
	normalData->changeAgility = false;
	normalData->absorb = false;
	normalData->ignoreDefense = false;
	normalData->inflictConditions = true;
	normalData->reduceAttributeResistance = false;
	type = 0;
	mpConsumed = 0;
	mpIsNumber = true;
}

Skill::Skill(Skill &other)
{
	normalData = NULL;
	escapeTeleportData = NULL;
	switchData = NULL;
	name = other.name;
	description = other.description;
	type = other.type;
	mpConsumed = other.mpConsumed;
	mpIsNumber = other.mpIsNumber;
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_NORMAL)
	{
		normalData = new NormalData;
		for (int i = 0; i < other.normalData->attributes.size(); ++i)
			normalData->attributes.append(other.normalData->attributes[i]);
		for (int i = 0; i < other.normalData->conditions.size(); ++i)
			normalData->conditions.append(other.normalData->conditions[i]);
		for (int i = 0; i < other.normalData->characterAnimationSettings.size(); ++i)
		{
			AnimationSettings *settings = new AnimationSettings;
			settings->movementBeforeAttack = other.normalData->characterAnimationSettings[i]->movementBeforeAttack;
			settings->animationPose = other.normalData->characterAnimationSettings[i]->animationPose;
			normalData->characterAnimationSettings.append(settings);
		}
		for (int i = 0; i < other.normalData->monsterAnimationSettings.size(); ++i)
		{
			AnimationSettings *settings = new AnimationSettings;
			settings->movementBeforeAttack = other.normalData->monsterAnimationSettings[i]->movementBeforeAttack;
			settings->animationPose = other.normalData->monsterAnimationSettings[i]->animationPose;
			normalData->monsterAnimationSettings.append(settings);
		}
		normalData->target = other.normalData->target;
		normalData->effectRating = other.normalData->effectRating;
		normalData->successRate = other.normalData->successRate;
		normalData->battleAnimationID = other.normalData->battleAnimationID;
		normalData->globalAnimationID = other.normalData->globalAnimationID;
		normalData->attackInfluence = other.normalData->attackInfluence;
		normalData->wisdomInfluence = other.normalData->wisdomInfluence;
		normalData->variance = other.normalData->variance;
		normalData->percentConditionEffectivenessProbability = other.normalData->percentConditionEffectivenessProbability;
		normalData->changeHP = other.normalData->changeHP;
		normalData->changeMP = other.normalData->changeMP;
		normalData->changeAttack = other.normalData->changeAttack;
		normalData->changeDefense = other.normalData->changeDefense;
		normalData->changeWisdom = other.normalData->changeWisdom;
		normalData->changeAgility = other.normalData->changeAgility;
		normalData->absorb = other.normalData->absorb;
		normalData->ignoreDefense = other.normalData->ignoreDefense;
		normalData->inflictConditions = other.normalData->inflictConditions;
		normalData->reduceAttributeResistance = other.normalData->reduceAttributeResistance;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
	{
		escapeTeleportData = new EscapeTeleportData;
		escapeTeleportData->soundEffect.location = other.escapeTeleportData->soundEffect.location;
		escapeTeleportData->soundEffect.volume = other.escapeTeleportData->soundEffect.volume;
		escapeTeleportData->soundEffect.panning = other.escapeTeleportData->soundEffect.panning;
		escapeTeleportData->soundEffect.speed = other.escapeTeleportData->soundEffect.speed;
		escapeTeleportData->soundEffect.numLoops = other.escapeTeleportData->soundEffect.numLoops;
		escapeTeleportData->soundEffect.sfxID = other.escapeTeleportData->soundEffect.sfxID;
		escapeTeleportData->soundEffect.retainPitch = other.escapeTeleportData->soundEffect.retainPitch;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
	{
		switchData = new SwitchData;
		switchData->soundEffect.location = other.switchData->soundEffect.location;
		switchData->soundEffect.volume = other.switchData->soundEffect.volume;
		switchData->soundEffect.panning = other.switchData->soundEffect.panning;
		switchData->soundEffect.speed = other.switchData->soundEffect.speed;
		switchData->soundEffect.numLoops = other.switchData->soundEffect.numLoops;
		switchData->soundEffect.sfxID = other.switchData->soundEffect.sfxID;
		switchData->soundEffect.retainPitch = other.switchData->soundEffect.retainPitch;
		switchData->switchID = other.switchData->switchID;
		switchData->allowField = other.switchData->allowField;
		switchData->allowBattle = other.switchData->allowBattle;
	}
}

Skill::Skill(const Skill &other)
{
	normalData = NULL;
	escapeTeleportData = NULL;
	switchData = NULL;
	name = other.name;
	description = other.description;
	type = other.type;
	mpConsumed = other.mpConsumed;
	mpIsNumber = other.mpIsNumber;
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_NORMAL)
	{
		normalData = new NormalData;
		for (int i = 0; i < other.normalData->attributes.size(); ++i)
			normalData->attributes.append(other.normalData->attributes[i]);
		for (int i = 0; i < other.normalData->conditions.size(); ++i)
			normalData->conditions.append(other.normalData->conditions[i]);
		for (int i = 0; i < other.normalData->characterAnimationSettings.size(); ++i)
		{
			AnimationSettings *settings = new AnimationSettings;
			settings->movementBeforeAttack = other.normalData->characterAnimationSettings[i]->movementBeforeAttack;
			settings->animationPose = other.normalData->characterAnimationSettings[i]->animationPose;
			normalData->characterAnimationSettings.append(settings);
		}
		for (int i = 0; i < other.normalData->monsterAnimationSettings.size(); ++i)
		{
			AnimationSettings *settings = new AnimationSettings;
			settings->movementBeforeAttack = other.normalData->monsterAnimationSettings[i]->movementBeforeAttack;
			settings->animationPose = other.normalData->monsterAnimationSettings[i]->animationPose;
			normalData->monsterAnimationSettings.append(settings);
		}
		normalData->target = other.normalData->target;
		normalData->effectRating = other.normalData->effectRating;
		normalData->successRate = other.normalData->successRate;
		normalData->battleAnimationID = other.normalData->battleAnimationID;
		normalData->globalAnimationID = other.normalData->globalAnimationID;
		normalData->attackInfluence = other.normalData->attackInfluence;
		normalData->wisdomInfluence = other.normalData->wisdomInfluence;
		normalData->variance = other.normalData->variance;
		normalData->percentConditionEffectivenessProbability = other.normalData->percentConditionEffectivenessProbability;
		normalData->changeHP = other.normalData->changeHP;
		normalData->changeMP = other.normalData->changeMP;
		normalData->changeAttack = other.normalData->changeAttack;
		normalData->changeDefense = other.normalData->changeDefense;
		normalData->changeWisdom = other.normalData->changeWisdom;
		normalData->changeAgility = other.normalData->changeAgility;
		normalData->absorb = other.normalData->absorb;
		normalData->ignoreDefense = other.normalData->ignoreDefense;
		normalData->inflictConditions = other.normalData->inflictConditions;
		normalData->reduceAttributeResistance = other.normalData->reduceAttributeResistance;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
	{
		escapeTeleportData = new EscapeTeleportData;
		escapeTeleportData->soundEffect.location = other.escapeTeleportData->soundEffect.location;
		escapeTeleportData->soundEffect.volume = other.escapeTeleportData->soundEffect.volume;
		escapeTeleportData->soundEffect.panning = other.escapeTeleportData->soundEffect.panning;
		escapeTeleportData->soundEffect.speed = other.escapeTeleportData->soundEffect.speed;
		escapeTeleportData->soundEffect.numLoops = other.escapeTeleportData->soundEffect.numLoops;
		escapeTeleportData->soundEffect.sfxID = other.escapeTeleportData->soundEffect.sfxID;
		escapeTeleportData->soundEffect.retainPitch = other.escapeTeleportData->soundEffect.retainPitch;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
	{
		switchData = new SwitchData;
		switchData->soundEffect.location = other.switchData->soundEffect.location;
		switchData->soundEffect.volume = other.switchData->soundEffect.volume;
		switchData->soundEffect.panning = other.switchData->soundEffect.panning;
		switchData->soundEffect.speed = other.switchData->soundEffect.speed;
		switchData->soundEffect.numLoops = other.switchData->soundEffect.numLoops;
		switchData->soundEffect.sfxID = other.switchData->soundEffect.sfxID;
		switchData->soundEffect.retainPitch = other.switchData->soundEffect.retainPitch;
		switchData->switchID = other.switchData->switchID;
		switchData->allowField = other.switchData->allowField;
		switchData->allowBattle = other.switchData->allowBattle;
	}
}

Skill::Skill(XMLNode SkillNode)
{
	XMLNode TempNode1, TempNode2, TempNode3;
	QString temp_string;
	normalData = NULL;
	escapeTeleportData = NULL;
	switchData = NULL;
	name = SkillNode.getAttribute("name");
	description = SkillNode.getAttribute("description");
	type = QString(SkillNode.getAttribute("type")).toInt();
	mpConsumed = QString(SkillNode.getAttribute("mpConsumed")).toInt();
	mpIsNumber = (QString(SkillNode.getAttribute("isMPNumber")) == "true") ? true:false;
	TempNode1 = SkillNode.getChildNode("data");
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_NORMAL)
	{
		normalData = new NormalData;
		TempNode2 = TempNode1.getChildNode("attributes");
		normalData->reduceAttributeResistance = (QString(TempNode2.getAttribute("reduceResistance")) == "true") ? true:false;
		temp_string = QString(TempNode2.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			normalData->attributes.append((temp_string[i].digitValue() == 1) ? true:false);
		TempNode2 = TempNode1.getChildNode("conditions");
		normalData->inflictConditions = (QString(TempNode2.getAttribute("inflictConditions")) == "true") ? true:false;
		normalData->percentConditionEffectivenessProbability = QString(TempNode2.getAttribute("percentEffectiveness")).toInt();
		temp_string = QString(TempNode2.getText()).remove(QRegExp("\\s"));
		for (int i = 0; i < temp_string.size(); ++i)
			normalData->conditions.append((temp_string[i].digitValue() == 1) ? true:false);
		TempNode2 = TempNode1.getChildNode("animationSettings");
		for (int i = 0; i < TempNode2.nChildNode("character"); ++i)
		{
			TempNode3 = TempNode2.getChildNode("character", i);
			AnimationSettings *settings = new AnimationSettings;
			temp_string = TempNode3.getAttribute("movementBeforeAttack");
			if (temp_string == "none")
				settings->movementBeforeAttack = MOVEMENT_NONE;
			else if (temp_string == "step forward")
				settings->movementBeforeAttack = MOVEMENT_STEPFORWARD;
			else if (temp_string == "jump forward")
				settings->movementBeforeAttack = MOVEMENT_JUMPFORWARD;
			else if (temp_string == "move to target")
				settings->movementBeforeAttack = MOVEMENT_MOVETOTARGET;
			else
				settings->movementBeforeAttack = MOVEMENT_NONE;
			settings->animationPose = QString(TempNode3.getAttribute("animationPose")).toInt();
			normalData->characterAnimationSettings.append(settings);
		}
		for (int i = 0; i < TempNode2.nChildNode("monster"); ++i)
		{
			TempNode3 = TempNode2.getChildNode("monster", i);
			AnimationSettings *settings = new AnimationSettings;
			temp_string = TempNode3.getAttribute("movementBeforeAttack");
			if (temp_string == "none")
				settings->movementBeforeAttack = MOVEMENT_NONE;
			else if (temp_string == "step forward")
				settings->movementBeforeAttack = MOVEMENT_STEPFORWARD;
			else if (temp_string == "jump forward")
				settings->movementBeforeAttack = MOVEMENT_JUMPFORWARD;
			else if (temp_string == "move to target")
				settings->movementBeforeAttack = MOVEMENT_MOVETOTARGET;
			else
				settings->movementBeforeAttack = MOVEMENT_NONE;
			settings->animationPose = QString(TempNode3.getAttribute("animationPose")).toInt();
			normalData->monsterAnimationSettings.append(settings);
		}
		temp_string = TempNode1.getAttribute("target");
		if (temp_string == "one enemy")
			normalData->target = TARGET_1ENEMY;
		else if (temp_string == "all enemies")
			normalData->target = TARGET_ALLENEMIES;
		else if (temp_string == "self")
			normalData->target = TARGET_SELF;
		else if (temp_string == "one ally")
			normalData->target = TARGET_1ALLY;
		else if (temp_string == "all allies")
			normalData->target = TARGET_ALLALLIES;
		else
			normalData->target = TARGET_1ENEMY;
		normalData->effectRating = QString(TempNode1.getAttribute("effectRating")).toInt();
		normalData->successRate = QString(TempNode1.getAttribute("successRate")).toInt();
		normalData->battleAnimationID = QString(TempNode1.getAttribute("battleAnimationID")).toInt();
		normalData->globalAnimationID = QString(TempNode1.getAttribute("globalAnimationID")).toInt();
		normalData->attackInfluence = QString(TempNode1.getAttribute("attackInfluence")).toInt();
		normalData->wisdomInfluence = QString(TempNode1.getAttribute("wisdomInfluence")).toInt();
		normalData->variance = QString(TempNode1.getAttribute("variance")).toInt();
		normalData->changeHP = (QString(TempNode2.getAttribute("changeHP")) == "true") ? true:false;
		normalData->changeMP = (QString(TempNode2.getAttribute("changeMP")) == "true") ? true:false;
		normalData->changeAttack = (QString(TempNode2.getAttribute("changeAttack")) == "true") ? true:false;
		normalData->changeDefense = (QString(TempNode2.getAttribute("changeDefense")) == "true") ? true:false;
		normalData->changeWisdom = (QString(TempNode2.getAttribute("changeWisdom")) == "true") ? true:false;
		normalData->changeAgility = (QString(TempNode2.getAttribute("changeAgility")) == "true") ? true:false;
		normalData->absorb = (QString(TempNode2.getAttribute("absorb")) == "true") ? true:false;
		normalData->ignoreDefense = (QString(TempNode2.getAttribute("ignoreDefense")) == "true") ? true:false;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
	{
		escapeTeleportData = new EscapeTeleportData;
		TempNode2 = TempNode1.getChildNode("soundEffect");
		escapeTeleportData->soundEffect.location = ProjectData::getAbsoluteResourcePath(TempNode2.getAttribute("location"));
		escapeTeleportData->soundEffect.volume = QString(TempNode2.getAttribute("volume")).toFloat();
		escapeTeleportData->soundEffect.panning = QString(TempNode2.getAttribute("panning")).toFloat();
		escapeTeleportData->soundEffect.speed = QString(TempNode2.getAttribute("speed")).toFloat();
		escapeTeleportData->soundEffect.numLoops = QString(TempNode2.getAttribute("numLoops")).toInt();
		escapeTeleportData->soundEffect.retainPitch = (QString(TempNode2.getAttribute("retainPitch")) == "true") ? true:false;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
	{
		switchData = new SwitchData;
		TempNode2 = TempNode1.getChildNode("soundEffect");
		switchData->soundEffect.location = ProjectData::getAbsoluteResourcePath(TempNode2.getAttribute("location"));
		switchData->soundEffect.volume = QString(TempNode2.getAttribute("volume")).toFloat();
		switchData->soundEffect.panning = QString(TempNode2.getAttribute("panning")).toFloat();
		switchData->soundEffect.speed = QString(TempNode2.getAttribute("speed")).toFloat();
		switchData->soundEffect.numLoops = QString(TempNode2.getAttribute("numLoops")).toInt();
		switchData->soundEffect.retainPitch = (QString(TempNode2.getAttribute("retainPitch")) == "true") ? true:false;
		switchData->switchID = QString(TempNode1.getAttribute("switchID")).toInt();
		switchData->allowField = (QString(TempNode1.getAttribute("allowField")) == "true") ? true:false;
		switchData->allowBattle = (QString(TempNode1.getAttribute("allowBattle")) == "true") ? true:false;
	}
}

Skill::Skill(StorageFile &storageFile)
{
	int count;
	char *temp_string;
	normalData = NULL;
	escapeTeleportData = NULL;
	switchData = NULL;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	temp_string = storageFile.getString();
	description = temp_string;
	delete[] temp_string;
	type = storageFile.getSignedInt();
	mpConsumed = storageFile.getSignedInt();
	mpIsNumber = storageFile.getBool();
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_NORMAL)
	{
		normalData = new NormalData;
		count = storageFile.getSignedInt();
		for (int i = 0; i < count; ++i)
			normalData->attributes.append(storageFile.getBool());
		count = storageFile.getSignedInt();
		for (int i = 0; i < count; ++i)
			normalData->conditions.append(storageFile.getBool());
		count = storageFile.getSignedInt();
		for (int i = 0; i < count; ++i)
		{
			AnimationSettings *settings = new AnimationSettings;
			settings->movementBeforeAttack = storageFile.getSignedInt();
			settings->animationPose = storageFile.getSignedInt();
			normalData->characterAnimationSettings.append(settings);
		}
		count = storageFile.getSignedInt();
		for (int i = 0; i < count; ++i)
		{
			AnimationSettings *settings = new AnimationSettings;
			settings->movementBeforeAttack = storageFile.getSignedInt();
			settings->animationPose = storageFile.getSignedInt();
			normalData->monsterAnimationSettings.append(settings);
		}
		normalData->target = storageFile.getSignedInt();
		normalData->effectRating = storageFile.getSignedInt();
		normalData->successRate = storageFile.getSignedInt();
		normalData->battleAnimationID = storageFile.getSignedInt();
		normalData->globalAnimationID = storageFile.getSignedInt();
		normalData->attackInfluence = storageFile.getSignedInt();
		normalData->wisdomInfluence = storageFile.getSignedInt();
		normalData->variance = storageFile.getSignedInt();
		normalData->percentConditionEffectivenessProbability = storageFile.getSignedInt();
		normalData->changeHP = storageFile.getBool();
		normalData->changeMP = storageFile.getBool();
		normalData->changeAttack = storageFile.getBool();
		normalData->changeDefense = storageFile.getBool();
		normalData->changeWisdom = storageFile.getBool();
		normalData->changeAgility = storageFile.getBool();
		normalData->absorb = storageFile.getBool();
		normalData->ignoreDefense = storageFile.getBool();
		normalData->inflictConditions = storageFile.getBool();
		normalData->reduceAttributeResistance = storageFile.getBool();
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
	{
		escapeTeleportData = new EscapeTeleportData;
		temp_string = storageFile.getString();
		escapeTeleportData->soundEffect.location = temp_string;
		delete[] temp_string;
		escapeTeleportData->soundEffect.volume = storageFile.getFloat();
		escapeTeleportData->soundEffect.panning = storageFile.getFloat();
		escapeTeleportData->soundEffect.speed = storageFile.getFloat();
		escapeTeleportData->soundEffect.numLoops = storageFile.getSignedInt();
		escapeTeleportData->soundEffect.sfxID = -1;
		escapeTeleportData->soundEffect.retainPitch = storageFile.getBool();
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
	{
		switchData = new SwitchData;
		temp_string = storageFile.getString();
		switchData->soundEffect.location = temp_string;
		delete[] temp_string;
		switchData->soundEffect.volume = storageFile.getFloat();
		switchData->soundEffect.panning = storageFile.getFloat();
		switchData->soundEffect.speed = storageFile.getFloat();
		switchData->soundEffect.numLoops = storageFile.getSignedInt();
		switchData->soundEffect.sfxID = -1;
		switchData->soundEffect.retainPitch = storageFile.getBool();
		switchData->switchID = storageFile.getSignedInt();
		switchData->allowField = storageFile.getBool();
		switchData->allowBattle = storageFile.getBool();
	}
}

Skill::~Skill()
{
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_NORMAL)
	{
		for (int i = 0; i < normalData->characterAnimationSettings.size(); ++i)
			delete (normalData->characterAnimationSettings[i]);
		for (int i = 0; i < normalData->monsterAnimationSettings.size(); ++i)
			delete (normalData->monsterAnimationSettings[i]);
		normalData->attributes.clear();
		normalData->conditions.clear();
		normalData->characterAnimationSettings.clear();
		normalData->monsterAnimationSettings.clear();
		delete normalData;
		normalData = NULL;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
	{
		delete escapeTeleportData;
		escapeTeleportData = NULL;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
	{
		delete switchData;
		switchData = NULL;
	}
}

void Skill::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putString(description.toUtf8().data());
	storageFile.putSignedInt(type);
	storageFile.putSignedInt(mpConsumed);
	storageFile.putBool(mpIsNumber);
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_NORMAL)
	{
		storageFile.putSignedInt(normalData->attributes.size());
		for (int i = 0; i < normalData->attributes.size(); ++i)
			storageFile.putBool(normalData->attributes[i]);
		storageFile.putSignedInt(normalData->conditions.size());
		for (int i = 0; i < normalData->conditions.size(); ++i)
			storageFile.putBool(normalData->conditions[i]);
		storageFile.putSignedInt(normalData->characterAnimationSettings.size());
		for (int i = 0; i < normalData->characterAnimationSettings.size(); ++i)
		{
			storageFile.putSignedInt(normalData->characterAnimationSettings[i]->movementBeforeAttack);
			storageFile.putSignedInt(normalData->characterAnimationSettings[i]->animationPose);
		}
		storageFile.putSignedInt(normalData->monsterAnimationSettings.size());
		for (int i = 0; i < normalData->monsterAnimationSettings.size(); ++i)
		{
			storageFile.putSignedInt(normalData->monsterAnimationSettings[i]->movementBeforeAttack);
			storageFile.putSignedInt(normalData->monsterAnimationSettings[i]->animationPose);
		}
		storageFile.putSignedInt(normalData->target);
		storageFile.putSignedInt(normalData->effectRating);
		storageFile.putSignedInt(normalData->successRate);
		storageFile.putSignedInt(normalData->battleAnimationID);
		storageFile.putSignedInt(normalData->globalAnimationID);
		storageFile.putSignedInt(normalData->attackInfluence);
		storageFile.putSignedInt(normalData->wisdomInfluence);
		storageFile.putSignedInt(normalData->variance);
		storageFile.putSignedInt(normalData->percentConditionEffectivenessProbability);
		storageFile.putBool(normalData->changeHP);
		storageFile.putBool(normalData->changeMP);
		storageFile.putBool(normalData->changeAttack);
		storageFile.putBool(normalData->changeDefense);
		storageFile.putBool(normalData->changeWisdom);
		storageFile.putBool(normalData->changeAgility);
		storageFile.putBool(normalData->absorb);
		storageFile.putBool(normalData->ignoreDefense);
		storageFile.putBool(normalData->inflictConditions);
		storageFile.putBool(normalData->reduceAttributeResistance);
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
	{
		storageFile.putString(escapeTeleportData->soundEffect.location.toUtf8().data());
		storageFile.putFloat(escapeTeleportData->soundEffect.volume);
		storageFile.putFloat(escapeTeleportData->soundEffect.panning);
		storageFile.putFloat(escapeTeleportData->soundEffect.speed);
		storageFile.putSignedInt(escapeTeleportData->soundEffect.numLoops);
		storageFile.putBool(escapeTeleportData->soundEffect.retainPitch);
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
	{
		storageFile.putString(switchData->soundEffect.location.toUtf8().data());
		storageFile.putFloat(switchData->soundEffect.volume);
		storageFile.putFloat(switchData->soundEffect.panning);
		storageFile.putFloat(switchData->soundEffect.speed);
		storageFile.putSignedInt(switchData->soundEffect.numLoops);
		storageFile.putBool(switchData->soundEffect.retainPitch);
		storageFile.putSignedInt(switchData->switchID);
		storageFile.putBool(switchData->allowField);
		storageFile.putBool(switchData->allowBattle);
	}
}

QStringList Skill::getResources()
{
	QStringList resources;
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
		resources.append(escapeTeleportData->soundEffect.location);
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
		resources.append(switchData->soundEffect.location);
	return resources;
}

XMLNode Skill::getSkillNode()
{
	XMLNode TempNode1, TempNode2, TempNode3, SkillNode = XMLNode::createXMLTopNode("skill");
	QString temp_string;
	SkillNode.addAttribute("name", name.toUtf8().data());
	SkillNode.addAttribute("description", description.toUtf8().data());
	SkillNode.addAttribute("type", QString::number(type).toUtf8().data());
	SkillNode.addAttribute("mpConsumed", QString::number(mpConsumed).toUtf8().data());
	SkillNode.addAttribute("isMPNumber", (mpIsNumber) ? "true":"false");
	TempNode1 = SkillNode.addChild("data");
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_NORMAL)
	{
		TempNode2 = TempNode1.addChild("attributes");
		temp_string = "";
		for (int i = 0; i < normalData->attributes.size(); ++i)
			temp_string += (normalData->attributes[i]) ? "1":"0";
		TempNode2.addText(temp_string.toUtf8().data());
		TempNode2.addAttribute("reduceResistance", (normalData->reduceAttributeResistance) ? "true":"false");
		TempNode2 = TempNode1.addChild("conditions");
		temp_string = "";
		for (int i = 0; i < normalData->conditions.size(); ++i)
			temp_string += (normalData->conditions[i]) ? "1":"0";
		TempNode2.addText(temp_string.toUtf8().data());
		TempNode2.addAttribute("inflictConditions", (normalData->inflictConditions) ? "true":"false");
		TempNode2.addAttribute("percentEffectiveness", QString::number(normalData->percentConditionEffectivenessProbability).toUtf8().data());
		TempNode2 = TempNode1.addChild("animationSettings");
		for (int i = 0; i < normalData->characterAnimationSettings.size(); ++i)
		{
			TempNode3 = TempNode2.addChild("character");
			if (normalData->characterAnimationSettings[i]->movementBeforeAttack == MOVEMENT_NONE)
				TempNode3.addAttribute("movementBeforeAttack", "none");
			else if (normalData->characterAnimationSettings[i]->movementBeforeAttack == MOVEMENT_STEPFORWARD)
				TempNode3.addAttribute("movementBeforeAttack", "stepforward");
			else if (normalData->characterAnimationSettings[i]->movementBeforeAttack == MOVEMENT_JUMPFORWARD)
				TempNode3.addAttribute("movementBeforeAttack", "jumpforward");
			else if (normalData->characterAnimationSettings[i]->movementBeforeAttack == MOVEMENT_MOVETOTARGET)
				TempNode3.addAttribute("movementBeforeAttack", "movetotarget");
			TempNode3.addAttribute("animationPose", QString::number(normalData->characterAnimationSettings[i]->animationPose).toUtf8().data());
		}
		for (int i = 0; i < normalData->monsterAnimationSettings.size(); ++i)
		{
			TempNode3 = TempNode2.addChild("monster");
			if (normalData->monsterAnimationSettings[i]->movementBeforeAttack == MOVEMENT_NONE)
				TempNode3.addAttribute("movementBeforeAttack", "none");
			else if (normalData->monsterAnimationSettings[i]->movementBeforeAttack == MOVEMENT_STEPFORWARD)
				TempNode3.addAttribute("movementBeforeAttack", "stepforward");
			else if (normalData->monsterAnimationSettings[i]->movementBeforeAttack == MOVEMENT_JUMPFORWARD)
				TempNode3.addAttribute("movementBeforeAttack", "jumpforward");
			else if (normalData->monsterAnimationSettings[i]->movementBeforeAttack == MOVEMENT_MOVETOTARGET)
				TempNode3.addAttribute("movementBeforeAttack", "movetotarget");
			TempNode3.addAttribute("animationPose", QString::number(normalData->monsterAnimationSettings[i]->animationPose).toUtf8().data());
		}
		if (normalData->target == TARGET_1ENEMY)
			TempNode1.addAttribute("target", "one enemy");
		else if (normalData->target == TARGET_ALLENEMIES)
			TempNode1.addAttribute("target", "all enemies");
		else if (normalData->target == TARGET_SELF)
			TempNode1.addAttribute("target", "self");
		else if (normalData->target == TARGET_1ALLY)
			TempNode1.addAttribute("target", "one ally");
		else if (normalData->target == TARGET_ALLALLIES)
			TempNode1.addAttribute("target", "all allies");
		TempNode1.addAttribute("effectRating", QString::number(normalData->effectRating).toUtf8().data());
		TempNode1.addAttribute("successRate", QString::number(normalData->successRate).toUtf8().data());
		TempNode1.addAttribute("battleAnimationID", QString::number(normalData->battleAnimationID).toUtf8().data());
		TempNode1.addAttribute("globalAnimationID", QString::number(normalData->globalAnimationID).toUtf8().data());
		TempNode1.addAttribute("attackInfluence", QString::number(normalData->attackInfluence).toUtf8().data());
		TempNode1.addAttribute("wisdomInfluence", QString::number(normalData->wisdomInfluence).toUtf8().data());
		TempNode1.addAttribute("variance", QString::number(normalData->variance).toUtf8().data());
		TempNode1.addAttribute("changeHP", (normalData->changeHP) ? "true":"false");
		TempNode1.addAttribute("changeMP", (normalData->changeMP) ? "true":"false");
		TempNode1.addAttribute("changeAttack", (normalData->changeAttack) ? "true":"false");
		TempNode1.addAttribute("changeDefense", (normalData->changeDefense) ? "true":"false");
		TempNode1.addAttribute("changeWisdom", (normalData->changeWisdom) ? "true":"false");
		TempNode1.addAttribute("changeAgility", (normalData->changeAgility) ? "true":"false");
		TempNode1.addAttribute("absorb", (normalData->absorb) ? "true":"false");
		TempNode1.addAttribute("ignoreDefense", (normalData->ignoreDefense) ? "true":"false");
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
	{
		TempNode2 = TempNode1.addChild("soundEffect");
		
		escapeTeleportData = new EscapeTeleportData;
		TempNode2 = TempNode1.getChildNode("soundEffect");
		TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(escapeTeleportData->soundEffect.location).toUtf8().data());
		TempNode2.addAttribute("volume", QString::number(escapeTeleportData->soundEffect.volume).toUtf8().data());
		TempNode2.addAttribute("panning", QString::number(escapeTeleportData->soundEffect.panning).toUtf8().data());
		TempNode2.addAttribute("speed", QString::number(escapeTeleportData->soundEffect.speed).toUtf8().data());
		TempNode2.addAttribute("numLoops", QString::number(escapeTeleportData->soundEffect.numLoops).toUtf8().data());
		TempNode2.addAttribute("retainPitch", (escapeTeleportData->soundEffect.retainPitch) ? "true":"false");
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
	{
		TempNode2 = TempNode1.getChildNode("soundEffect");
		TempNode2.addAttribute("location", ProjectData::getRelativeResourcePath(switchData->soundEffect.location).toUtf8().data());
		TempNode2.addAttribute("volume", QString::number(switchData->soundEffect.volume).toUtf8().data());
		TempNode2.addAttribute("panning", QString::number(switchData->soundEffect.panning).toUtf8().data());
		TempNode2.addAttribute("speed", QString::number(switchData->soundEffect.speed).toUtf8().data());
		TempNode2.addAttribute("numLoops", QString::number(switchData->soundEffect.numLoops).toUtf8().data());
		TempNode2.addAttribute("retainPitch", (switchData->soundEffect.retainPitch) ? "true":"false");
		TempNode1.addAttribute("switchID", QString::number(switchData->switchID).toUtf8().data());
		TempNode1.addAttribute("allowField", (switchData->allowField) ? "true":"false");
		TempNode1.addAttribute("allowBattle", (switchData->allowBattle) ? "true":"false");
	}
	return SkillNode;
}

QString Skill::getName()
{
	return name;
}

void Skill::setName(QString value)
{
	name = value;
}

QString Skill::getDescription()
{
	return description;
}

void Skill::setDescription(QString value)
{
	description = value;
}

void Skill::getNormalData(NormalData *data)
{
	for (int i = 0; i < data->characterAnimationSettings.size(); ++i)
		delete data->characterAnimationSettings[i];
	for (int i = 0; i < data->monsterAnimationSettings.size(); ++i)
		delete data->monsterAnimationSettings[i];
	data->characterAnimationSettings.clear();
	data->monsterAnimationSettings.clear();
	data->attributes.clear();
	data->conditions.clear();
	for (int i = 0; i < normalData->attributes.size(); ++i)
		data->attributes.append(normalData->attributes[i]);
	for (int i = 0; i < normalData->conditions.size(); ++i)
		data->conditions.append(normalData->conditions[i]);
	for (int i = 0; i < normalData->characterAnimationSettings.size(); ++i)
	{
		AnimationSettings *settings = new AnimationSettings;
		settings->movementBeforeAttack = normalData->characterAnimationSettings[i]->movementBeforeAttack;
		settings->animationPose = normalData->characterAnimationSettings[i]->animationPose;
		data->characterAnimationSettings.append(settings);
	}
	for (int i = 0; i < normalData->monsterAnimationSettings.size(); ++i)
	{
		AnimationSettings *settings = new AnimationSettings;
		settings->movementBeforeAttack = normalData->monsterAnimationSettings[i]->movementBeforeAttack;
		settings->animationPose = normalData->monsterAnimationSettings[i]->animationPose;
		data->monsterAnimationSettings.append(settings);
	}
	data->target = normalData->target;
	data->effectRating = normalData->effectRating;
	data->successRate = normalData->successRate;
	data->battleAnimationID = normalData->battleAnimationID;
	data->globalAnimationID = normalData->globalAnimationID;
	data->attackInfluence = normalData->attackInfluence;
	data->wisdomInfluence = normalData->wisdomInfluence;
	data->variance = normalData->variance;
	data->changeHP = normalData->changeHP;
	data->changeMP = normalData->changeMP;
	data->changeAttack = normalData->changeAttack;
	data->changeDefense = normalData->changeDefense;
	data->changeWisdom = normalData->changeWisdom;
	data->changeAgility = normalData->changeAgility;
	data->absorb = normalData->absorb;
	data->ignoreDefense = normalData->ignoreDefense;
	data->inflictConditions = normalData->inflictConditions;
	data->reduceAttributeResistance = normalData->reduceAttributeResistance;
}

void Skill::setNormalData(NormalData *data)
{
	for (int i = 0; i < normalData->characterAnimationSettings.size(); ++i)
		delete normalData->characterAnimationSettings[i];
	for (int i = 0; i < normalData->monsterAnimationSettings.size(); ++i)
		delete normalData->monsterAnimationSettings[i];
	normalData->characterAnimationSettings.clear();
	normalData->monsterAnimationSettings.clear();
	normalData->attributes.clear();
	normalData->conditions.clear();
	for (int i = 0; i < data->attributes.size(); ++i)
		normalData->attributes.append(data->attributes[i]);
	for (int i = 0; i < data->conditions.size(); ++i)
		normalData->conditions.append(data->conditions[i]);
	for (int i = 0; i < data->characterAnimationSettings.size(); ++i)
	{
		AnimationSettings *settings = new AnimationSettings;
		settings->movementBeforeAttack = data->characterAnimationSettings[i]->movementBeforeAttack;
		settings->animationPose = data->characterAnimationSettings[i]->animationPose;
		normalData->characterAnimationSettings.append(settings);
	}
	for (int i = 0; i < data->monsterAnimationSettings.size(); ++i)
	{
		AnimationSettings *settings = new AnimationSettings;
		settings->movementBeforeAttack = data->monsterAnimationSettings[i]->movementBeforeAttack;
		settings->animationPose = data->monsterAnimationSettings[i]->animationPose;
		normalData->monsterAnimationSettings.append(settings);
	}
	normalData->target = data->target;
	normalData->effectRating = data->effectRating;
	normalData->successRate = data->successRate;
	normalData->battleAnimationID = data->battleAnimationID;
	normalData->globalAnimationID = data->globalAnimationID;
	normalData->attackInfluence = data->attackInfluence;
	normalData->wisdomInfluence = data->wisdomInfluence;
	normalData->variance = data->variance;
	normalData->changeHP = data->changeHP;
	normalData->changeMP = data->changeMP;
	normalData->changeAttack = data->changeAttack;
	normalData->changeDefense = data->changeDefense;
	normalData->changeWisdom = data->changeWisdom;
	normalData->changeAgility = data->changeAgility;
	normalData->absorb = data->absorb;
	normalData->ignoreDefense = data->ignoreDefense;
	normalData->inflictConditions = data->inflictConditions;
	normalData->reduceAttributeResistance = data->reduceAttributeResistance;
}

void Skill::getEscapeTeleportData(EscapeTeleportData *data)
{
	data->soundEffect.location = escapeTeleportData->soundEffect.location;
	data->soundEffect.volume = escapeTeleportData->soundEffect.volume;
	data->soundEffect.panning = escapeTeleportData->soundEffect.panning;
	data->soundEffect.speed = escapeTeleportData->soundEffect.speed;
	data->soundEffect.numLoops = escapeTeleportData->soundEffect.numLoops;
	data->soundEffect.sfxID = escapeTeleportData->soundEffect.sfxID;
	data->soundEffect.retainPitch = escapeTeleportData->soundEffect.retainPitch;
}

void Skill::setEscapeTeleportData(EscapeTeleportData *data)
{
	escapeTeleportData->soundEffect.location = data->soundEffect.location;
	escapeTeleportData->soundEffect.volume = data->soundEffect.volume;
	escapeTeleportData->soundEffect.panning = data->soundEffect.panning;
	escapeTeleportData->soundEffect.speed = data->soundEffect.speed;
	escapeTeleportData->soundEffect.numLoops = data->soundEffect.numLoops;
	escapeTeleportData->soundEffect.sfxID = data->soundEffect.sfxID;
	escapeTeleportData->soundEffect.retainPitch = data->soundEffect.retainPitch;
}

void Skill::getSwitchData(SwitchData *data)
{
	data->soundEffect.location = switchData->soundEffect.location;
	data->soundEffect.volume = switchData->soundEffect.volume;
	data->soundEffect.panning = switchData->soundEffect.panning;
	data->soundEffect.speed = switchData->soundEffect.speed;
	data->soundEffect.numLoops = switchData->soundEffect.numLoops;
	data->soundEffect.sfxID = switchData->soundEffect.sfxID;
	data->soundEffect.retainPitch = switchData->soundEffect.retainPitch;
	data->switchID = switchData->switchID;
	data->allowField = switchData->allowField;
	data->allowBattle = switchData->allowBattle;
}

void Skill::setSwitchData(SwitchData *data)
{
	switchData->soundEffect.location = data->soundEffect.location;
	switchData->soundEffect.volume = data->soundEffect.volume;
	switchData->soundEffect.panning = data->soundEffect.panning;
	switchData->soundEffect.speed = data->soundEffect.speed;
	switchData->soundEffect.numLoops = data->soundEffect.numLoops;
	switchData->soundEffect.sfxID = data->soundEffect.sfxID;
	switchData->soundEffect.retainPitch = data->soundEffect.retainPitch;
	switchData->switchID = data->switchID;
	switchData->allowField = data->allowField;
	switchData->allowBattle = data->allowBattle;
}

int Skill::getType()
{
	return type;
}

void Skill::setType(int value)
{
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_NORMAL)
		delete normalData;
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
		delete escapeTeleportData;
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
		delete switchData;
	type = value;
	if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_NORMAL)
	{
		normalData = new NormalData;
		for (int i = 0; i < ProjectData::attributeNames.size(); ++i)
			normalData->attributes.append(false);
		for (int i = 0; i < ProjectData::conditionNames.size(); ++i)
			normalData->conditions.append(false);
		for (int i = 0; i < ProjectData::characterNames.size(); ++i)
		{
			AnimationSettings *settings = new AnimationSettings;
			settings->movementBeforeAttack = MOVEMENT_NONE;
			settings->animationPose = 0;
			normalData->characterAnimationSettings.append(settings);
		}
		for (int i = 0; i < ProjectData::monsterNames.size(); ++i)
		{
			AnimationSettings *settings = new AnimationSettings;
			settings->movementBeforeAttack = MOVEMENT_NONE;
			settings->animationPose = 0;
			normalData->monsterAnimationSettings.append(settings);
		}
		normalData->target = TARGET_1ENEMY;
		normalData->effectRating = 0;
		normalData->successRate = 100;
		normalData->battleAnimationID = -1;
		normalData->globalAnimationID = -1;
		normalData->attackInfluence = 0;
		normalData->wisdomInfluence = 0;
		normalData->variance = 0;
		normalData->percentConditionEffectivenessProbability = 100;
		normalData->changeHP = false;
		normalData->changeMP = false;
		normalData->changeAttack = false;
		normalData->changeDefense = false;
		normalData->changeWisdom = false;
		normalData->changeAgility = false;
		normalData->absorb = false;
		normalData->ignoreDefense = false;
		normalData->inflictConditions = true;
		normalData->reduceAttributeResistance = false;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_ESCAPE)
	{
		escapeTeleportData = new EscapeTeleportData;
		escapeTeleportData->soundEffect.location = "";
		escapeTeleportData->soundEffect.volume = 1.0;
		escapeTeleportData->soundEffect.panning = 0.0;
		escapeTeleportData->soundEffect.speed = 1.0;
		escapeTeleportData->soundEffect.numLoops = 0;
		escapeTeleportData->soundEffect.sfxID = -1;
		escapeTeleportData->soundEffect.retainPitch = true;
	}
	else if (ProjectData::skillSubsets[type]->type == ProjectData::SKILLTYPE_SWITCH)
	{
		switchData = new SwitchData;
		switchData->soundEffect.location = "";
		switchData->soundEffect.volume = 1.0;
		switchData->soundEffect.panning = 0.0;
		switchData->soundEffect.speed = 1.0;
		switchData->soundEffect.numLoops = 0;
		switchData->soundEffect.sfxID = -1;
		switchData->soundEffect.retainPitch = true;
		switchData->switchID = 0;
		switchData->allowField = true;
		switchData->allowBattle = true;
	}
}

void Skill::changeTypeRetainingSkillType(int newType)
{
	type = newType;
}

int Skill::getMPConsumed()
{
	return mpConsumed;
}

void Skill::setMPConsumed(int value)
{
	mpConsumed = value;
}

bool Skill::isMPNumber()
{
	return mpIsNumber;
}

void Skill::setMPIsNumber(bool value)
{
	mpIsNumber = value;
}

bool Skill::tryLoad(XMLNode SkillNode)
{
	XMLNode TempNode1, TempNode2, TempNode3;
	QStringList tempStringList, trueFalseList;
	QString temp_string;
	int skillType;
	trueFalseList.append("true");
	trueFalseList.append("false");
	if (!XMLTools::attributeExists(SkillNode, "name"))
		return false;
	if (!XMLTools::attributeExists(SkillNode, "description"))
		return false;
	if (!XMLTools::attributeExists(SkillNode, "type"))
		return false;
	skillType = QString(SkillNode.getAttribute("type")).toInt();
	if (!XMLTools::attributeExists(SkillNode, "mpConsumed"))
		return false;
	if (!XMLTools::attributeIntRangeValid(SkillNode, "mpConsumed", 0, 9999))
		return false;
	if (!XMLTools::attributeExists(SkillNode, "isMPNumber"))
		return false;
	if (!XMLTools::attributeStringValid(SkillNode, "isMPNumber", trueFalseList, 1))
		return false;
	if (!XMLTools::nodeExists(SkillNode, "data"))
		return false;
	TempNode1 = SkillNode.getChildNode("data");
	if (ProjectData::skillSubsets[skillType]->type == ProjectData::SKILLTYPE_NORMAL)
	{
		if (!XMLTools::nodeExists(TempNode1, "attributes"))
			return false;
		TempNode2 = TempNode1.getChildNode("attributes");
		if (!XMLTools::attributeExists(TempNode2, "reduceResistance"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode2, "reduceResistance", trueFalseList, 1))
			return false;
		if (!XMLTools::nodeExists(TempNode1, "conditions"))
			return false;
		TempNode2 = TempNode1.getChildNode("conditions");
		if (!XMLTools::attributeExists(TempNode2, "inflictConditions"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode2, "inflictConditions", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode2, "percentEffectiveness"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode2, "percentEffectiveness", 0, 100))
			return false;
		if (!XMLTools::nodeExists(TempNode1, "animationSettings"))
			return false;
		TempNode2 = TempNode1.getChildNode("animationSettings");
		for (int i = 0; i < TempNode2.nChildNode("character"); ++i)
		{
			TempNode3 = TempNode2.getChildNode("character", i);
			if (!XMLTools::attributeExists(TempNode3, "movementBeforeAttack"))
				return false;
			tempStringList.clear();
			tempStringList << "none" << "step forward" << "jump forward" << "move to target";
			if (!XMLTools::attributeStringValid(TempNode3, "movementBeforeAttack", tempStringList, 0))
				return false;
			if (!XMLTools::attributeExists(TempNode3, "animationPose"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode3, "animationPose", 0, 31))
				return false;
		}
		for (int i = 0; i < TempNode2.nChildNode("monster"); ++i)
		{
			TempNode3 = TempNode2.getChildNode("monster", i);
			if (!XMLTools::attributeExists(TempNode3, "movementBeforeAttack"))
				return false;
			tempStringList.clear();
			tempStringList << "none" << "step forward" << "jump forward" << "move to target";
			if (!XMLTools::attributeStringValid(TempNode3, "movementBeforeAttack", tempStringList, 0))
				return false;
			if (!XMLTools::attributeExists(TempNode3, "animationPose"))
				return false;
			if (!XMLTools::attributeIntRangeValid(TempNode3, "animationPose", 0, 31))
				return false;
		}
		if (!XMLTools::attributeExists(TempNode1, "target"))
			return false;
		tempStringList.clear();
		tempStringList << "one enemy" << "all enemies" << "self" << "one ally" << "all allies";
		if (!XMLTools::attributeStringValid(TempNode1, "target", tempStringList, 0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "effectRating"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode1, "effectRating", 0, 9999))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "successRate"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode1, "succesRate", 0, 100))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "battleAnimationID"))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "globalAnimationID"))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "attackInfluence"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode1, "attackInfluence", 0, 10))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "wisdomInfluence"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode1, "wisdomInfluence", 0, 10))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "variance"))
			return false;
		if (!XMLTools::attributeIntRangeValid(TempNode1, "variance", 0, 10))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "changeHP"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "changeHP", trueFalseList, 1))
			return false;
		
		if (!XMLTools::attributeExists(TempNode1, "changeMP"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "changeMP", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "changeAttack"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "changeAttack", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "changeDefense"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "changeDefense", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "changeWisdom"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "changeWisdom", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "changeAgility"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "changeAgility", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "absorb"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "absorb", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "ignoreDefense"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "ignoreDefense", trueFalseList, 1))
			return false;
	}
	else if (ProjectData::skillSubsets[skillType]->type == ProjectData::SKILLTYPE_TELEPORT || ProjectData::skillSubsets[skillType]->type == ProjectData::SKILLTYPE_ESCAPE)
	{
		if (!XMLTools::nodeExists(TempNode1, "soundEffect"))
			return false;
		TempNode2 = TempNode1.getChildNode("soundEffect");
		if (!XMLTools::attributeExists(TempNode1, "location"))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "volume"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(TempNode1, "volume", 0.0, 1.0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "panning"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(TempNode1, "panning", -1.0, 1.0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "speed"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(TempNode1, "speed", 0.5, 2.0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "numLoops"))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "retainPitch"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "retainPitch", trueFalseList, 1))
			return false;
	}
	else if (ProjectData::skillSubsets[skillType]->type == ProjectData::SKILLTYPE_SWITCH)
	{
		if (!XMLTools::nodeExists(TempNode1, "soundEffect"))
			return false;
		TempNode2 = TempNode1.getChildNode("soundEffect");
		if (!XMLTools::attributeExists(TempNode1, "location"))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "volume"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(TempNode1, "volume", 0.0, 1.0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "panning"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(TempNode1, "panning", -1.0, 1.0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "speed"))
			return false;
		if (!XMLTools::attributeFloatRangeValid(TempNode1, "speed", 0.5, 2.0))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "numLoops"))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "retainPitch"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "retainPitch", trueFalseList, 1))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "switchID"))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "allowField"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "allowField", trueFalseList))
			return false;
		if (!XMLTools::attributeExists(TempNode1, "allowBattle"))
			return false;
		if (!XMLTools::attributeStringValid(TempNode1, "allowBattle", trueFalseList, 1))
			return false;
	}
	return true;
}
