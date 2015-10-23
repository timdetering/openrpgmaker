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

#include "configureevent_callmapevent.h"
#include "projectdata_mapevent.h"
#include "variableselector.h"

ConfigureEvent_CallMapEvent::ConfigureEvent_CallMapEvent(int mapID, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	for (int i = 0; i < (*mapRef)->numEvents(); ++i)
		cboxEvents->addItem((*mapRef)->getEvent(i)->getName());
	wEventVariable->setEnabled(false);
	wPageVariable->setEnabled(false);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_CallMapEvent::~ConfigureEvent_CallMapEvent()
{
	if (mapRef != NULL)
	{
		delete mapRef;
		mapRef = NULL;
	}
}

void ConfigureEvent_CallMapEvent::setEventID(int eventID)
{
	if (rbSpecificEvent->isChecked())
		cboxEvents->setCurrentIndex(eventID + 1);
	else
		lblEventVariable->setText(ProjectData::numberVariableIdToString(eventID));
}

int ConfigureEvent_CallMapEvent::getEventID()
{
	if (rbSpecificEvent->isChecked())
		return cboxEvents->currentIndex() - 1;
	return ProjectData::numberVariableIdFromString(lblEventVariable->text());
}

void ConfigureEvent_CallMapEvent::setEventReferencedInVariable(bool referenced)
{
	if (referenced)
		rbReferencedEvent->setChecked(true);
	else
		rbSpecificEvent->setChecked(true);
}

bool ConfigureEvent_CallMapEvent::isEventReferencedInVariable()
{
	return rbReferencedEvent->isChecked();
}

void ConfigureEvent_CallMapEvent::setPageID(int pageID)
{
	if (rbSpecificPage->isChecked())
		sbPage->setValue(pageID);
	else
		lblPageVariable->setText(ProjectData::numberVariableIdToString(pageID));
}

int ConfigureEvent_CallMapEvent::getPageID()
{
	if (rbSpecificPage->isChecked())
		return sbPage->value();
	return ProjectData::numberVariableIdFromString(lblPageVariable->text());
}

void ConfigureEvent_CallMapEvent::setPageReferencedInVariable(bool referenced)
{
	if (referenced)
		rbReferencedPage->setChecked(true);
	else
		rbSpecificPage->setChecked(true);
}

bool ConfigureEvent_CallMapEvent::isPageReferencedInVariable()
{
	return rbReferencedPage->isChecked();
}

void ConfigureEvent_CallMapEvent::on_bBrowseEventVariables_clicked()
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

void ConfigureEvent_CallMapEvent::on_bBrowsePageVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblPageVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblPageVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_CallMapEvent::on_cboxEvent_currentIndexChanged(int index)
{
	sbPage->setMaximum((*mapRef)->getEvent(index)->numPages());
}

void ConfigureEvent_CallMapEvent::on_rbSpecificEvent_toggled(bool checked)
{
	cboxEvents->setEnabled(checked);
	wEventVariable->setEnabled(!checked);
}

void ConfigureEvent_CallMapEvent::on_rbSpecificPage_toggled(bool checked)
{
	wPage->setEnabled(checked);
	wPageVariable->setEnabled(!checked);
}
