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

#include <QGraphicsRectItem>
#include <QPen>
#include <QRect>
#include <QMouseEvent>
#include "configureevent_changeeventlocation.h"
#include "projectdata.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"
#include "variableselector.h"

ConfigureEvent_ChangeEventLocation::ConfigureEvent_ChangeEventLocation(int mapID, QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	ProjectData::DataReference<Map> *mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	setupUi(this);
	for (int i = 0; i < (*mapRef)->numEvents(); ++i)
		cboxEvents->addItem((*mapRef)->getEvent(i)->getName());
	lblEventVariable->setText(varText);
	lblXVariable->setText(varText);
	lblYVariable->setText(varText);
	lblLayerVariable->setText(varText);
	wLocation->setupMapLocation(mapID);
	setLocation(0, 0, 0);
	delete mapRef;
	resize(sizeHint());
	setMinimumSize(size());
	setMaximumSize(size());
}

ConfigureEvent_ChangeEventLocation::~ConfigureEvent_ChangeEventLocation()
{
}

void ConfigureEvent_ChangeEventLocation::setEventID(int eventID)
{
	if (rbSpecificEvent->isChecked())
		cboxEvents->setCurrentIndex(eventID + 1);
	else
		lblEventVariable->setText(ProjectData::numberVariableIdToString(eventID));
}

int ConfigureEvent_ChangeEventLocation::getEventID()
{
	if (rbSpecificEvent->isChecked())
		return cboxEvents->currentIndex() - 1;
	return ProjectData::numberVariableIdFromString(lblEventVariable->text());
}

void ConfigureEvent_ChangeEventLocation::setEventReferencedInVariable(bool referenced)
{
	if (referenced)
		rbReferencedEvent->setChecked(true);
	else
		rbSpecificEvent->setChecked(true);
}

bool ConfigureEvent_ChangeEventLocation::isEventReferencedInVariable()
{
	return rbReferencedEvent->isChecked();
}

void ConfigureEvent_ChangeEventLocation::setLocation(int x, int y, int layer)
{
	if (rbSpecificLocation->isChecked())
		wLocation->setLocation(x, y, layer);
	else
	{
		lblXVariable->setText(ProjectData::numberVariableIdToString(x));
		lblYVariable->setText(ProjectData::numberVariableIdToString(y));
		lblLayerVariable->setText(ProjectData::numberVariableIdToString(layer));
	}
}

int ConfigureEvent_ChangeEventLocation::getXLocation()
{
	if (rbSpecificLocation->isChecked())
		return wLocation->getXLocation();
	return ProjectData::numberVariableIdFromString(lblXVariable->text());
}

int ConfigureEvent_ChangeEventLocation::getYLocation()
{
	if (rbSpecificLocation->isChecked())
		return wLocation->getYLocation();
	return ProjectData::numberVariableIdFromString(lblYVariable->text());
}

int ConfigureEvent_ChangeEventLocation::getLayer()
{
	if (rbSpecificLocation->isChecked())
		return wLocation->getLayer();
	return ProjectData::numberVariableIdFromString(lblLayerVariable->text());
}

void ConfigureEvent_ChangeEventLocation::setLocationStoredInVariables(bool stored)
{
	if (stored)
		rbStoredLocation->setChecked(true);
	else
		rbSpecificLocation->setChecked(true);
}

bool ConfigureEvent_ChangeEventLocation::isLocationStoredInVariables()
{
	return rbStoredLocation->isChecked();
}

void ConfigureEvent_ChangeEventLocation::on_bBrowseEventVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblEventVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblEventVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeEventLocation::on_bBrowseXVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblXVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblXVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeEventLocation::on_bBrowseYVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblYVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblYVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeEventLocation::on_bBrowseLayerVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblLayerVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblLayerVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeEventLocation::on_rbSpecificEvent_toggled(bool checked)
{
	cboxEvents->setEnabled(checked);
	wEventVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeEventLocation::on_rbSpecificLocation_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wLocation);
	else
		stackedWidget->setCurrentWidget(wVariables);
}
