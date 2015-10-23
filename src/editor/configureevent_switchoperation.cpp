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

#include "configureevent_switchoperation.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_SwitchOperation::ConfigureEvent_SwitchOperation(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	wSwitchRange->setEnabled(false);
	wVariable->setEnabled(false);
	lblSwitch->setText(ProjectData::switchVariableIdToString(0));
	lblStartSwitch->setText(ProjectData::switchVariableIdToString(0));
	lblEndSwitch->setText(ProjectData::switchVariableIdToString(0));
	lblVariable->setText(ProjectData::numberVariableIdToString(0));
}

ConfigureEvent_SwitchOperation::~ConfigureEvent_SwitchOperation()
{
}

void ConfigureEvent_SwitchOperation::setSwitchToChange(int switchToChange)
{
	if (switchToChange == SINGLE)
		rbSpecificSwitch->setChecked(true);
	else if (switchToChange == RANGE)
		rbRangeOfSwitches->setChecked(true);
	else
		rbReferencedSwitch->setChecked(true);
}

void ConfigureEvent_SwitchOperation::setSwitchIDs(int switch1ID, int switch2ID)
{
	if (rbSpecificSwitch->isChecked())
		lblSwitch->setText(ProjectData::switchVariableIdToString(switch1ID));
	else if (rbRangeOfSwitches->isChecked())
	{
		lblStartSwitch->setText(ProjectData::switchVariableIdToString(switch1ID));
		lblEndSwitch->setText(ProjectData::switchVariableIdToString(switch2ID));
	}
	else
		lblVariable->setText(ProjectData::numberVariableIdToString(switch1ID));
}

void ConfigureEvent_SwitchOperation::setOperation(int operation)
{
	if (operation == TURN_ON)
		rbTurnON->setChecked(true);
	else if (operation == TURN_OFF)
		rbTurnOFF->setChecked(true);
	else
		rbToggle->setChecked(true);
}

int ConfigureEvent_SwitchOperation::getSwitchToChange()
{
	if (rbSpecificSwitch->isChecked())
		return SINGLE;
	else if (rbRangeOfSwitches->isChecked())
		return RANGE;
	return REFERENCE;
}

int ConfigureEvent_SwitchOperation::getSwitch1ID()
{
	if (rbSpecificSwitch->isChecked())
		return ProjectData::switchVariableIdFromString(lblSwitch->text());
	else if (rbRangeOfSwitches->isChecked())
		return ProjectData::switchVariableIdFromString(lblStartSwitch->text());
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

int ConfigureEvent_SwitchOperation::getSwitch2ID()
{
	if (rbRangeOfSwitches->isChecked())
		return ProjectData::switchVariableIdFromString(lblEndSwitch->text());
	return 0;
}

int ConfigureEvent_SwitchOperation::getOperation()
{
	if (rbTurnON->isChecked())
		return TURN_ON;
	else if (rbTurnOFF->isChecked())
		return TURN_OFF;
	return TOGGLE;
}

void ConfigureEvent_SwitchOperation::on_bBrowseEndSwitches_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lblEndSwitch->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		switchID = variableSelector->getVariableID();
		lblEndSwitch->setText(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureEvent_SwitchOperation::on_bBrowseStartSwitches_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lblStartSwitch->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		switchID = variableSelector->getVariableID();
		lblStartSwitch->setText(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureEvent_SwitchOperation::on_bBrowseSwitches_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lblSwitch->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		switchID = variableSelector->getVariableID();
		lblSwitch->setText(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureEvent_SwitchOperation::on_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::switchVariableIdFromString(lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_SwitchOperation::on_rbRangeOfSwitches_toggled(bool checked)
{
	wSwitchRange->setEnabled(checked);
}

void ConfigureEvent_SwitchOperation::on_rbReferencedSwitch_toggled(bool checked)
{
	wVariable->setEnabled(checked);
}

void ConfigureEvent_SwitchOperation::on_rbSpecificSwitch_toggled(bool checked)
{
	wSwitch->setEnabled(checked);
}
