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

#include "configureevent_tintscreen.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_TintScreen::ConfigureEvent_TintScreen(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	lblOpacityVariable->setText(ProjectData::numberVariableIdToString(0));
	lblTimeVariable->setText(ProjectData::numberVariableIdToString(0));
	wOpacityVariable->setEnabled(false);
	wTimeVariable->setEnabled(false);
	bOpacitySideButton->setSpinBox(sbOpacity);
	bTimeSideButton->setSpinBox(sbTime);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_TintScreen::~ConfigureEvent_TintScreen()
{
}

QColor ConfigureEvent_TintScreen::getColor()
{
	return bColor->getColor();
}

void ConfigureEvent_TintScreen::setColor(QColor color)
{
	bColor->setColor(color);
}

int ConfigureEvent_TintScreen::getOpacity()
{
	if (rbSpecificOpacity->isChecked())
		return sbOpacity->value();
	return ProjectData::numberVariableIdFromString(lblOpacityVariable->text());
}

void ConfigureEvent_TintScreen::setOpacity(int value)
{
	if (rbSpecificOpacity->isChecked())
		sbOpacity->setValue(value);
	else
		lblOpacityVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_TintScreen::getTransitionTime()
{
	if (rbSpecificTime->isChecked())
		return sbTime->value();
	return ProjectData::numberVariableIdFromString(lblTimeVariable->text());
}

void ConfigureEvent_TintScreen::setTransitionTime(int value)
{
	if (rbSpecificTime->isChecked())
		sbTime->setValue(value);
	else
		lblTimeVariable->setText(ProjectData::numberVariableIdToString(value));
}

bool ConfigureEvent_TintScreen::isOpacityStoredInVariable()
{
	return rbStoredOpacity->isChecked();
}

void ConfigureEvent_TintScreen::setOpacityStoredInVariable(bool stored)
{
	if (stored)
		rbStoredOpacity->setChecked(true);
	else
		rbSpecificOpacity->setChecked(true);
}

bool ConfigureEvent_TintScreen::isTimeStoredInVariable()
{
	return rbStoredTime->isChecked();
}

void ConfigureEvent_TintScreen::setTimeStoredInVariable(bool stored)
{
	if (stored)
		rbStoredTime->setChecked(true);
	else
		rbSpecificTime->setChecked(true);
}

bool ConfigureEvent_TintScreen::getHaltOtherProcesses()
{
	return cbHaltOtherProcesses->isChecked();
}

void ConfigureEvent_TintScreen::setHaltOtherProcesses(bool value)
{
	cbHaltOtherProcesses->setChecked(value);
}

void ConfigureEvent_TintScreen::on_bBrowseOpacityVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblOpacityVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblOpacityVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_TintScreen::on_bBrowseTimeVariables_clicked()
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

void ConfigureEvent_TintScreen::on_rbSpecificOpacity_toggled(bool checked)
{
	wOpacity->setEnabled(checked);
	wOpacityVariable->setEnabled(!checked);
}

void ConfigureEvent_TintScreen::on_rbSpecificTime_toggled(bool checked)
{
	wTime->setEnabled(checked);
	wTimeVariable->setEnabled(!checked);
}
