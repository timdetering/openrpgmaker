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

#include "configureevent_flashscreen.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_FlashScreen::ConfigureEvent_FlashScreen(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	bOpacitySideButton->setSpinBox(sbOpacity);
	bDelaySideButton->setSpinBox(sbDelay);
	lblOpacityVariable->setText(ProjectData::numberVariableIdToString(0));
	lblDelayVariable->setText(ProjectData::numberVariableIdToString(0));
	wOpacityVariable->setEnabled(false);
	wDelayVariable->setEnabled(false);
	frmDelay->setEnabled(false);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_FlashScreen::~ConfigureEvent_FlashScreen()
{
}

int ConfigureEvent_FlashScreen::getFlash()
{
	return cboxFlash->currentIndex();
}

void ConfigureEvent_FlashScreen::setFlash(int value)
{
	cboxFlash->setCurrentIndex(value);
}

QColor ConfigureEvent_FlashScreen::getColor()
{
	return bColor->getColor();
}

void ConfigureEvent_FlashScreen::setColor(QColor color)
{
	bColor->setColor(color);
}

int ConfigureEvent_FlashScreen::getOpacity()
{
	if (rbSpecificOpacity->isChecked())
		return sbOpacity->value();
	else
		return ProjectData::numberVariableIdFromString(lblOpacityVariable->text());
}

void ConfigureEvent_FlashScreen::setOpacity(int value)
{
	if (rbSpecificOpacity->isChecked())
		sbOpacity->setValue(value);
	else
		lblOpacityVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_FlashScreen::getDelayBetweenFlashes()
{
	if (rbSpecificDelay->isChecked())
		return sbDelay->value();
	else
		return ProjectData::numberVariableIdFromString(lblDelayVariable->text());
}

void ConfigureEvent_FlashScreen::setDelayBetweenFlashes(int value)
{
	if (rbSpecificDelay->isChecked())
		sbDelay->setValue(value);
	else
		lblDelayVariable->setText(ProjectData::numberVariableIdToString(value));
}

bool ConfigureEvent_FlashScreen::isOpacityStoredInVariable()
{
	return rbStoredOpacity->isChecked();
}

void ConfigureEvent_FlashScreen::setOpacityStoredInVariable(bool stored)
{
	if (stored)
		rbStoredOpacity->setChecked(true);
	else
		rbSpecificOpacity->setChecked(true);
}

bool ConfigureEvent_FlashScreen::isDelayStoredInVariable()
{
	return rbStoredDelay->isChecked();
}

void ConfigureEvent_FlashScreen::setDelayStoredInVariable(bool stored)
{
	if (stored)
		rbStoredDelay->setChecked(true);
	else
		rbSpecificDelay->setChecked(true);
}

bool ConfigureEvent_FlashScreen::getHaltOtherProcesses()
{
	return cbHaltOtherProcesses->isChecked();
}

void ConfigureEvent_FlashScreen::setHaltOtherProcesses(bool value)
{
	cbHaltOtherProcesses->setChecked(value);
}

void ConfigureEvent_FlashScreen::on_bBrowseDelayVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblDelayVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblDelayVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_FlashScreen::on_bBrowseOpacityVariables_clicked()
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

void ConfigureEvent_FlashScreen::on_cboxFlash_currentIndexChanged(int index)
{
	if (index == ONCE)
	{
		cbHaltOtherProcesses->setEnabled(true);
		frmColor->setEnabled(true);
		frmOpacity->setEnabled(true);
		frmDelay->setEnabled(false);
	}
	else if (index == BEGIN)
	{
		cbHaltOtherProcesses->setEnabled(false);
		frmColor->setEnabled(true);
		frmOpacity->setEnabled(true);
		frmDelay->setEnabled(true);
	}
	else if (index == STOP)
	{
		cbHaltOtherProcesses->setEnabled(false);
		frmColor->setEnabled(false);
		frmOpacity->setEnabled(false);
		frmDelay->setEnabled(false);
	}
}

void ConfigureEvent_FlashScreen::on_rbSpecificDelay_toggled(bool checked)
{
	wDelay->setEnabled(checked);
	wDelayVariable->setEnabled(!checked);
}

void ConfigureEvent_FlashScreen::on_rbSpecificOpacity_toggled(bool checked)
{
	wOpacity->setEnabled(checked);
	wOpacityVariable->setEnabled(!checked);
}
