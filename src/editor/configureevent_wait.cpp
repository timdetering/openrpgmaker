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

#include "configureevent_wait.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_Wait::ConfigureEvent_Wait(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	bTimeSideButton->setSpinBox(sbTime);
	wVariable->setEnabled(false);
	lblVariable->setText(ProjectData::numberVariableIdToString(0));
}

ConfigureEvent_Wait::~ConfigureEvent_Wait()
{
}

void ConfigureEvent_Wait::setTime(int time)
{
	if (rbSpecificTime->isChecked())
		sbTime->setValue(time);
	else
		lblVariable->setText(ProjectData::numberVariableIdToString(time));
}

void ConfigureEvent_Wait::setIsStoredTime(bool stored)
{
	if (stored)
		rbStoredTime->setChecked(true);
	else
		rbSpecificTime->setChecked(true);
}

int ConfigureEvent_Wait::getTime()
{
	if (rbSpecificTime->isChecked())
		return sbTime->value();
	else
		return ProjectData::numberVariableIdFromString(lblVariable->text());
}

bool ConfigureEvent_Wait::isStoredTime()
{
	return rbStoredTime->isChecked();
}

void ConfigureEvent_Wait::on_bBrowseVariables_clicked()
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

void ConfigureEvent_Wait::on_rbSpecificTime_toggled(bool checked)
{
	wTime->setEnabled(checked);
	wVariable->setEnabled(!checked);
}
