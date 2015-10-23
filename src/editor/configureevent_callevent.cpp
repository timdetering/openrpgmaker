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

#include "configureevent_callevent.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_CallEvent::ConfigureEvent_CallEvent(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCommonEvents->addItems(ProjectData::commonEventNames);
	lblVariable->setText(ProjectData::numberVariableIdToString(0));
	wVariable->setEnabled(false);
}

ConfigureEvent_CallEvent::~ConfigureEvent_CallEvent()
{
}

void ConfigureEvent_CallEvent::setEventID(int eventID)
{
	if (rbSpecificEvent->isChecked())
		cboxCommonEvents->setCurrentIndex(eventID);
	else
		lblVariable->setText(ProjectData::numberVariableIdToString(eventID));
}

void ConfigureEvent_CallEvent::setIsReferencedInVariable(bool referenced)
{
	if (referenced)
		rbReferencedEvent->setChecked(true);
	else
		rbSpecificEvent->setChecked(true);
}

int ConfigureEvent_CallEvent::getEventID()
{
	if (rbSpecificEvent->isChecked())
		return cboxCommonEvents->currentIndex();
	return lblVariable->text().left(4).toInt() - 1;
}

bool ConfigureEvent_CallEvent::isReferencedInVariable()
{
	return rbReferencedEvent->isChecked();
}

void ConfigureEvent_CallEvent::on_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_CallEvent::on_rbSpecificEvent_toggled(bool checked)
{
	cboxCommonEvents->setEnabled(checked);
	wVariable->setEnabled(!checked);
}
