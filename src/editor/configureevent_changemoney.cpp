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

#include "configureevent_changemoney.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ChangeMoney::ConfigureEvent_ChangeMoney(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	lblAmountVariable->setText(ProjectData::numberVariableIdToString(0));
	bAmountSideButton->setSpinBox(sbAmount);
	wAmountVariable->setEnabled(false);
}

ConfigureEvent_ChangeMoney::~ConfigureEvent_ChangeMoney()
{
}

void ConfigureEvent_ChangeMoney::setOperation(bool increase)
{
	if (increase)
		rbIncreaseMoney->setChecked(true);
	else
		rbDecreaseMoney->setChecked(true);
}

void ConfigureEvent_ChangeMoney::setAmount(int amount)
{
	if (rbSpecificAmount->isChecked())
		sbAmount->setValue(amount);
	else
		lblAmountVariable->setText(ProjectData::numberVariableIdToString(amount));
}

void ConfigureEvent_ChangeMoney::setIsStoredAmount(bool storedAmount)
{
	if (storedAmount)
		rbStoredAmount->setChecked(true);
	else
		rbSpecificAmount->setChecked(true);
}

bool ConfigureEvent_ChangeMoney::isIncreaseOperation()
{
	return rbIncreaseMoney->isChecked();
}

int ConfigureEvent_ChangeMoney::getAmount()
{
	if (rbSpecificAmount->isChecked())
		return sbAmount->value();
	return ProjectData::numberVariableIdFromString(lblAmountVariable->text());
}

bool ConfigureEvent_ChangeMoney::isStoredAmount()
{
	return rbStoredAmount->isChecked();
}

void ConfigureEvent_ChangeMoney::on_bBrowseAmountVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblAmountVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblAmountVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMoney::on_rbSpecificAmount_toggled(bool checked)
{
	wAmount->setEnabled(checked);
	wAmountVariable->setEnabled(!checked);
}
