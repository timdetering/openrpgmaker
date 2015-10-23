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

#include "configureevent_changecharacterhp.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ChangeCharacterHP::ConfigureEvent_ChangeCharacterHP(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	lblAmountVariable->setText(ProjectData::numberVariableIdToString(0));
	bAmountSideButton->setSpinBox(sbAmount);
	cboxCharacters->setEnabled(false);
	wCharacterVariable->setEnabled(false);
	wAmountVariable->setEnabled(false);
}

ConfigureEvent_ChangeCharacterHP::~ConfigureEvent_ChangeCharacterHP()
{
}

void ConfigureEvent_ChangeCharacterHP::setTarget(int target)
{
	if (target == ENTIRE_PARTY)
		rbEntireParty->setChecked(true);
	else if (target == SPECIFIC_CHARACTER)
		rbSpecificCharacter->setChecked(true);
	else
		rbReferencedCharacter->setChecked(true);
}

void ConfigureEvent_ChangeCharacterHP::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else if (rbReferencedCharacter->isChecked())
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

void ConfigureEvent_ChangeCharacterHP::setOperation(bool increase)
{
	if (increase)
		rbIncreaseHP->setChecked(true);
	else
		rbDecreaseHP->setChecked(true);
}

void ConfigureEvent_ChangeCharacterHP::setAmount(int amount)
{
	if (rbSpecificAmount->isChecked())
		sbAmount->setValue(amount);
	else
		lblAmountVariable->setText(ProjectData::numberVariableIdToString(amount));
}

void ConfigureEvent_ChangeCharacterHP::setIsStoredAmount(bool storedAmount)
{
	if (storedAmount)
		rbStoredAmount->setChecked(true);
	else
		rbSpecificAmount->setChecked(true);
}

int ConfigureEvent_ChangeCharacterHP::getTarget()
{
	if (rbEntireParty->isChecked())
		return ENTIRE_PARTY;
	else if (rbSpecificCharacter->isChecked())
		return SPECIFIC_CHARACTER;
	return REFERENCED_CHARACTER;
}

int ConfigureEvent_ChangeCharacterHP::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	else if (rbReferencedCharacter->isChecked())
		return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	return 0;
}

bool ConfigureEvent_ChangeCharacterHP::isIncreaseOperation()
{
	return rbIncreaseHP->isChecked();
}

int ConfigureEvent_ChangeCharacterHP::getAmount()
{
	if (rbSpecificAmount->isChecked())
		return sbAmount->value();
	return ProjectData::numberVariableIdFromString(lblAmountVariable->text());
}

bool ConfigureEvent_ChangeCharacterHP::isStoredAmount()
{
	return rbStoredAmount->isChecked();
}

void ConfigureEvent_ChangeCharacterHP::on_bBrowseAmountVariables_clicked()
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

void ConfigureEvent_ChangeCharacterHP::on_bBrowseCharacterVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeCharacterHP::on_rbReferencedCharacter_toggled(bool checked)
{
	wCharacterVariable->setEnabled(checked);
}

void ConfigureEvent_ChangeCharacterHP::on_rbSpecificAmount_toggled(bool checked)
{
	wAmount->setEnabled(checked);
	wAmountVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeCharacterHP::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
}
