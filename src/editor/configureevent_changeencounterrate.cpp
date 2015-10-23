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

#include "configureevent_changeencounterrate.h"

ConfigureEvent_ChangeEncounterRate::ConfigureEvent_ChangeEncounterRate(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	bEncRateSideButton->setSpinBox(sbEncRate);
	bMinStepsSideButton->setSpinBox(sbMinSteps);
	bMaxStepsSideButton->setSpinBox(sbMaxSteps);
}

ConfigureEvent_ChangeEncounterRate::~ConfigureEvent_ChangeEncounterRate()
{
}

void ConfigureEvent_ChangeEncounterRate::setEncounterRate(int rate)
{
	sbEncRate->setValue(rate);
}

int ConfigureEvent_ChangeEncounterRate::getEncounterRate()
{
	return sbEncRate->value();
}

void ConfigureEvent_ChangeEncounterRate::setMinSteps(int min)
{
	sbMinSteps->setValue(min);
}

int ConfigureEvent_ChangeEncounterRate::getMinSteps()
{
	return sbMinSteps->value();
}

void ConfigureEvent_ChangeEncounterRate::setMaxSteps(int max)
{
	sbMaxSteps->setValue(max);
}

int ConfigureEvent_ChangeEncounterRate::getMaxSteps()
{
	return sbMaxSteps->value();
}
