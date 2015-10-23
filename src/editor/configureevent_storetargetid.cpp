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

#include "configureevent_storetargetid.h"
#include "projectdata.h"
#include "projectdata_map.h"
#include "selectmaplocation.h"
#include "variableselector.h"

ConfigureEvent_StoreTargetID::ConfigureEvent_StoreTargetID(QString targetType, int map, QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	setupUi(this);
	setWindowTitle(QString("Store %1 ID").arg(targetType));
	lblLocationTitle->setText(QString("Location of Target %1").arg(targetType));
	lblVariableTitle->setText(QString("Store %1 ID In Variable").arg(targetType));
	lblLocation->setText("(0, 0) layer=0");
	lblTargetVariable->setText(varText);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	mapID = map;
	currentX = 0;
	currentY = 0;
	layer = 0;
	locationStoredInVariables = false;
}

ConfigureEvent_StoreTargetID::~ConfigureEvent_StoreTargetID()
{
}

int ConfigureEvent_StoreTargetID::getXLocation()
{
	return currentX;
}

void ConfigureEvent_StoreTargetID::setXLocation(int value)
{
	currentX = value;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}

int ConfigureEvent_StoreTargetID::getYLocation()
{
	return currentY;
}

void ConfigureEvent_StoreTargetID::setYLocation(int value)
{
	currentY = value;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}

int ConfigureEvent_StoreTargetID::getLayer()
{
	return layer;
}

void ConfigureEvent_StoreTargetID::setLayer(int value)
{
	layer = value;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}

int ConfigureEvent_StoreTargetID::getTargetVariableID()
{
	return ProjectData::numberVariableIdFromString(lblTargetVariable->text());
}

void ConfigureEvent_StoreTargetID::setTargetVariableID(int value)
{
	lblTargetVariable->setText(ProjectData::numberVariableIdToString(value));
}

bool ConfigureEvent_StoreTargetID::isLocationStoredInVariables()
{
	return locationStoredInVariables;
}

void ConfigureEvent_StoreTargetID::setLocationStoredInVariables(bool value)
{
	locationStoredInVariables = value;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}

void ConfigureEvent_StoreTargetID::on_bBrowseTargetVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTargetVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTargetVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StoreTargetID::on_bSelectLocation_clicked()
{
	SelectMapLocation *selectLocation = new SelectMapLocation(mapID);
	selectLocation->setLocationStoredInVariables(locationStoredInVariables);
	selectLocation->setLocation(currentX, currentY, layer);
	if (selectLocation->exec())
	{
		currentX = selectLocation->getXLocation();
		currentY = selectLocation->getYLocation();
		layer = selectLocation->getLayer();
		locationStoredInVariables = selectLocation->isLocationStoredInVariables();
	}
	delete selectLocation;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}
