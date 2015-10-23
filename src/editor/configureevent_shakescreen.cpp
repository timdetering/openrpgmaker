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

#include "configureevent_shakescreen.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ShakeScreen::ConfigureEvent_ShakeScreen(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	lblTimeVariable->setText(ProjectData::numberVariableIdToString(0));
	wTimeVariable->setEnabled(false);
	bTimeSideButton->setSpinBox(sbTime);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_ShakeScreen::~ConfigureEvent_ShakeScreen()
{
}

int ConfigureEvent_ShakeScreen::getAction()
{
	if (rbShakeOnce->isChecked())
		return SHAKE_ONCE;
	else if (rbBeginShaking->isChecked())
		return BEGIN_SHAKING;
	return STOP_SHAKING;
}

void ConfigureEvent_ShakeScreen::setAction(int value)
{
	if (value == SHAKE_ONCE)
		rbShakeOnce->setChecked(true);
	else if (value == BEGIN_SHAKING)
		rbBeginShaking->setChecked(true);
	else if (value == STOP_SHAKING)
		rbStopShaking->setChecked(true);
}

int ConfigureEvent_ShakeScreen::getTime()
{
	if (rbSpecificTime->isChecked())
		return sbTime->value();
	return ProjectData::numberVariableIdFromString(lblTimeVariable->text());
}

void ConfigureEvent_ShakeScreen::setTime(int value)
{
	if (rbSpecificTime->isChecked())
		sbTime->setValue(value);
	else
		lblTimeVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_ShakeScreen::getStrength()
{
	return hsStrength->value();
}

void ConfigureEvent_ShakeScreen::setStrength(int value)
{
	hsStrength->setValue(value);
}

int ConfigureEvent_ShakeScreen::getSpeed()
{
	return hsSpeed->value();
}

void ConfigureEvent_ShakeScreen::setSpeed(int value)
{
	hsSpeed->setValue(value);
}

bool ConfigureEvent_ShakeScreen::isTimeStoredInVariable()
{
	return rbStoredTime->isChecked();
}

void ConfigureEvent_ShakeScreen::setTimeStoredInVariable(bool stored)
{
	if (stored)
		rbStoredTime->setChecked(true);
	else
		rbSpecificTime->setChecked(true);
}

bool ConfigureEvent_ShakeScreen::getHaltOtherProcesses()
{
	return cbHaltOtherProcesses->isChecked();
}

void ConfigureEvent_ShakeScreen::setHaltOtherProcesses(bool halt)
{
	cbHaltOtherProcesses->setChecked(halt);
}

void ConfigureEvent_ShakeScreen::on_bBrowseTimeVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTimeVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTimeVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ShakeScreen::on_rbBeginShaking_toggled(bool checked)
{
	if (checked)
	{
		frmTime->setEnabled(false);
		frmOption->setEnabled(false);
		frmStrength->setEnabled(true);
		frmSpeed->setEnabled(true);
	}
}

void ConfigureEvent_ShakeScreen::on_rbShakeOnce_toggled(bool checked)
{
	if (checked)
	{
		frmTime->setEnabled(true);
		frmOption->setEnabled(true);
		frmStrength->setEnabled(true);
		frmSpeed->setEnabled(true);
	}
}

void ConfigureEvent_ShakeScreen::on_rbSpecificTime_toggled(bool checked)
{
	wTime->setEnabled(checked);
	wTimeVariable->setEnabled(!checked);
}

void ConfigureEvent_ShakeScreen::on_rbStopShaking_toggled(bool checked)
{
	if (checked)
	{
		frmTime->setEnabled(false);
		frmOption->setEnabled(false);
		frmStrength->setEnabled(false);
		frmSpeed->setEnabled(false);
	}
}
