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

#include "configureevent_timeroperation.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_TimerOperation::ConfigureEvent_TimerOperation(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	wTimerVariable->setEnabled(false);
	wTimeVariable->setEnabled(false);
	wAlignment->setEnabled(false);
	lblTimerVariable->setText(ProjectData::numberVariableIdToString(0));
	lblTimeVariable->setText(ProjectData::numberVariableIdToString(0));
	bTimerSideButton->setSpinBox(sbTimer);
	bMinutesSideButton->setSpinBox(sbMinutes);
	bSecondsSideButton->setSpinBox(sbSeconds);
}

ConfigureEvent_TimerOperation::~ConfigureEvent_TimerOperation()
{
}

void ConfigureEvent_TimerOperation::setTimerID(int timerID)
{
	if (rbSpecificTimer->isChecked())
		sbTimer->setValue(timerID + 1);
	else
		lblTimerVariable->setText(ProjectData::numberVariableIdToString(timerID));
}

void ConfigureEvent_TimerOperation::setIsReferencedTimer(bool referenced)
{
	if (referenced)
		rbReferencedTimer->setChecked(true);
	else
		rbSpecificTimer->setChecked(true);
}

void ConfigureEvent_TimerOperation::setOperation(int operation)
{
	if (operation == SET)
		rbSet->setChecked(true);
	else if (operation == START)
		rbStart->setChecked(true);
	else
		rbStop->setChecked(true);
}

void ConfigureEvent_TimerOperation::setTime(int minutes, int seconds)
{
	if (rbSpecificTime->isChecked())
	{
		sbMinutes->setValue(minutes);
		sbSeconds->setValue(seconds);
	}
	else
		lblTimeVariable->setText(ProjectData::numberVariableIdToString(seconds));
}

void ConfigureEvent_TimerOperation::setIsStoredTime(bool stored)
{
	if (stored)
		rbStoredTime->setChecked(true);
	else
		rbSpecificTime->setChecked(true);
}

void ConfigureEvent_TimerOperation::setAlignment(int halign, int valign)
{
	cboxHorizontalAlignment->setCurrentIndex(halign);
	cboxVerticalAlignment->setCurrentIndex(valign);
}

void ConfigureEvent_TimerOperation::setDisplayTimerOnScreen(bool display)
{
	cbDisplayTimerOnScreen->setChecked(display);
}

void ConfigureEvent_TimerOperation::setTimerRunsDuringBattle(bool runsDuringBattle)
{
	cbTimerRunsDuringBattle->setChecked(runsDuringBattle);
}

int ConfigureEvent_TimerOperation::getTimerID()
{
	if (rbSpecificTimer->isChecked())
		return sbTimer->value() - 1;
	return ProjectData::numberVariableIdFromString(lblTimerVariable->text());
}

bool ConfigureEvent_TimerOperation::isReferencedTimer()
{
	return rbReferencedTimer->isChecked();
}

int ConfigureEvent_TimerOperation::getOperation()
{
	if (rbSet->isChecked())
		return SET;
	else if (rbStart->isChecked())
		return START;
	return STOP;
}

int ConfigureEvent_TimerOperation::getMinutes()
{
	if (rbSpecificTime->isChecked())
		return sbMinutes->value();
	return 0;
}

int ConfigureEvent_TimerOperation::getSeconds()
{
	if (rbSpecificTime->isChecked())
		return sbSeconds->value();
	return ProjectData::numberVariableIdFromString(lblTimeVariable->text());
}

bool ConfigureEvent_TimerOperation::isStoredTime()
{
	return rbStoredTime->isChecked();
}

int ConfigureEvent_TimerOperation::getHAlignment()
{
	return cboxHorizontalAlignment->currentIndex();
}

int ConfigureEvent_TimerOperation::getVAlignment()
{
	return cboxVerticalAlignment->currentIndex();
}

bool ConfigureEvent_TimerOperation::displayTimerOnScreen()
{
	return cbDisplayTimerOnScreen->isChecked();
}

bool ConfigureEvent_TimerOperation::timerRunsDuringBattle()
{
	return cbTimerRunsDuringBattle->isChecked();
}

void ConfigureEvent_TimerOperation::on_bBrowseTimeVariables_clicked()
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

void ConfigureEvent_TimerOperation::on_bBrowseTimerVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTimerVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTimerVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_TimerOperation::on_cbDisplayTimerOnScreen_toggled(bool checked)
{
	wAlignment->setEnabled(checked);
}

void ConfigureEvent_TimerOperation::on_rbSet_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wSet);
}

void ConfigureEvent_TimerOperation::on_rbSpecificTime_toggled(bool checked)
{
	wTime->setEnabled(checked);
	wTimeVariable->setEnabled(!checked);
}

void ConfigureEvent_TimerOperation::on_rbSpecificTimer_toggled(bool checked)
{
	wTimer->setEnabled(checked);
	wTimerVariable->setEnabled(!checked);
}

void ConfigureEvent_TimerOperation::on_rbStart_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wStart);
}

void ConfigureEvent_TimerOperation::on_rbStop_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wStop);
}
