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
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configureevent_showbattleanimation_map.h"
#include "projectdata.h"
#include "projectdata_battleanimation.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"

ConfigureEvent_ShowBattleAnimation_Map::ConfigureEvent_ShowBattleAnimation_Map(int mapID, QWidget *parent) : QDialog(parent)
{
	ProjectData::DataReference<Map> *mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	setupUi(this);
	cboxTarget->addItem("Hero");
	typeList.append(HERO);
	idList.append(-1);
	for (int i = 0; i < ProjectData::vehicleNames.size(); ++i)
	{
		vehicleIndexes.append(cboxTarget->count());
		cboxTarget->addItem(ProjectData::vehicleNames[i]);
		typeList.append(VEHICLE);
		idList.append(i);
	}
	cboxTarget->addItem("This Event");
	typeList.append(THIS_EVENT);
	idList.append(-1);
	for (int i = 0; i < (*mapRef)->numEvents(); ++i)
	{
		eventIndexes.append(cboxTarget->count());
		cboxTarget->addItem((*mapRef)->getEvent(i)->getName());
		typeList.append(EVENT);
		idList.append(i);
	}
	animRef = NULL;
	wBattleAnimationPreview->setScale(0.25);
	cboxBattleAnimation->addItems(ProjectData::battleAnimationNames);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	delete mapRef;
}

ConfigureEvent_ShowBattleAnimation_Map::~ConfigureEvent_ShowBattleAnimation_Map()
{
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

int ConfigureEvent_ShowBattleAnimation_Map::getBattleAnimationID()
{
	return cboxBattleAnimation->currentIndex();
}

void ConfigureEvent_ShowBattleAnimation_Map::setBattleAnimationID(int value)
{
	cboxBattleAnimation->setCurrentIndex(value);
}

int ConfigureEvent_ShowBattleAnimation_Map::getTargetType()
{
	return typeList[cboxTarget->currentIndex()];
}

void ConfigureEvent_ShowBattleAnimation_Map::setTargetType(int type)
{
	bool foundOne = false;
	for (int i = 0; i < typeList.size() && !foundOne; ++i)
	{
		if (typeList[i] == type)
		{
			cboxTarget->setCurrentIndex(i);
			foundOne = true;
		}
	}
}

int ConfigureEvent_ShowBattleAnimation_Map::getTargetID()
{
	return idList[cboxTarget->currentIndex()];
}

void ConfigureEvent_ShowBattleAnimation_Map::setTargetID(int value)
{
	int currentType = typeList[cboxTarget->currentIndex()];
	if (currentType == VEHICLE)
		cboxTarget->setCurrentIndex(vehicleIndexes[value]);
	else if (currentType == EVENT)
		cboxTarget->setCurrentIndex(eventIndexes[value]);
}

bool ConfigureEvent_ShowBattleAnimation_Map::getHaltOtherProcesses()
{
	return cbHaltOtherProcesses->isChecked();
}

void ConfigureEvent_ShowBattleAnimation_Map::setHaltOtherProcesses(bool value)
{
	return cbHaltOtherProcesses->setChecked(value);
}

void ConfigureEvent_ShowBattleAnimation_Map::on_cboxBattleAnimation_currentIndexChanged(int index)
{
	if (index >= 0 && index < ProjectData::battleAnimationNames.size())
	{
		wBattleAnimationPreview->stopAnimation();
		if (animRef != NULL)
		{
			delete animRef;
			animRef = NULL;
		}
		animRef = new ProjectData::DataReference<BattleAnimation>(index, __FILE__, __LINE__);
		wBattleAnimationPreview->cycleAnimation(animRef->getDataID(), QPixmap(), false);
	}
}
