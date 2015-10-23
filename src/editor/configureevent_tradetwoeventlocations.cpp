/****************************************************************************
 *  Open RPG Maker is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  Open RPG Maker is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with Open RPG Maker. If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 *  ---                                                                     *
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configureevent_tradetwoeventlocations.h"
#include "projectdata.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"

ConfigureEvent_TradeTwoEventLocations::ConfigureEvent_TradeTwoEventLocations(int mapID, QWidget *parent) : QDialog(parent)
{
	ProjectData::DataReference<Map> *mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	setupUi(this);
	cboxEvent1->addItem("Hero");
	cboxEvent2->addItem("Hero");
	typeList.append(HERO);
	idList.append(-1);
	for (int i = 0; i < ProjectData::vehicleNames.size(); ++i)
	{
		vehicleIndexes.append(cboxEvent1->count());
		cboxEvent1->addItem(ProjectData::vehicleNames[i]);
		cboxEvent2->addItem(ProjectData::vehicleNames[i]);
		typeList.append(VEHICLE);
		idList.append(i);
	}
	cboxEvent1->addItem("This Event");
	cboxEvent2->addItem("This Event");
	typeList.append(THIS_EVENT);
	idList.append(-1);
	for (int i = 0; i < (*mapRef)->numEvents(); ++i)
	{
		eventIndexes.append(cboxEvent1->count());
		cboxEvent1->addItem((*mapRef)->getEvent(i)->getName());
		cboxEvent2->addItem((*mapRef)->getEvent(i)->getName());
		typeList.append(EVENT);
		idList.append(i);
	}
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	delete mapRef;
}

ConfigureEvent_TradeTwoEventLocations::~ConfigureEvent_TradeTwoEventLocations()
{
}

int ConfigureEvent_TradeTwoEventLocations::getEvent1Type()
{
	return typeList[cboxEvent1->currentIndex()];
}

void ConfigureEvent_TradeTwoEventLocations::setEvent1Type(int value)
{
	bool foundOne = false;
	for (int i = 0; i < typeList.size() && !foundOne; ++i)
	{
		if (typeList[i] == value)
		{
			cboxEvent1->setCurrentIndex(i);
			foundOne = true;
		}
	}
}

int ConfigureEvent_TradeTwoEventLocations::getEvent1ID()
{
	return idList[cboxEvent1->currentIndex()];
}

void ConfigureEvent_TradeTwoEventLocations::setEvent1ID(int value)
{
	int currentType = typeList[cboxEvent1->currentIndex()];
	if (currentType == VEHICLE)
		cboxEvent1->setCurrentIndex(vehicleIndexes[value]);
	else if (currentType == EVENT)
		cboxEvent1->setCurrentIndex(eventIndexes[value]);
}

int ConfigureEvent_TradeTwoEventLocations::getEvent2Type()
{
	return typeList[cboxEvent2->currentIndex()];
}

void ConfigureEvent_TradeTwoEventLocations::setEvent2Type(int value)
{
	bool foundOne = false;
	for (int i = 0; i < typeList.size() && !foundOne; ++i)
	{
		if (typeList[i] == value)
		{
			cboxEvent2->setCurrentIndex(i);
			foundOne = true;
		}
	}
}

int ConfigureEvent_TradeTwoEventLocations::getEvent2ID()
{
	return idList[cboxEvent2->currentIndex()];
}

void ConfigureEvent_TradeTwoEventLocations::setEvent2ID(int value)
{
	int currentType = typeList[cboxEvent2->currentIndex()];
	if (currentType == VEHICLE)
		cboxEvent2->setCurrentIndex(vehicleIndexes[value]);
	else if (currentType == EVENT)
		cboxEvent2->setCurrentIndex(eventIndexes[value]);
}
