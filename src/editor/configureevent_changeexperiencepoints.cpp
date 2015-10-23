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

#include "configureevent_changeexperiencepoints.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ChangeExperiencePoints::ConfigureEvent_ChangeExperiencePoints(QWidget *parent) : QDialog(parent)
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

ConfigureEvent_ChangeExperiencePoints::~ConfigureEvent_ChangeExperiencePoints()
{
}

void ConfigureEvent_ChangeExperiencePoints::setTarget(int target)
{
	if (target == ENTIRE_PARTY)
		rbEntireParty->setChecked(true);
	else if (target == SPECIFIC_CHARACTER)
		rbSpecificCharacter->setChecked(true);
	else
		rbReferencedCharacter->setChecked(true);
}

void ConfigureEvent_ChangeExperiencePoints::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else if (rbReferencedCharacter->isChecked())
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

void ConfigureEvent_ChangeExperiencePoints::setOperation(bool increase)
{
	if (increase)
		rbIncreaseExperience->setChecked(true);
	else
		rbDecreaseExperience->setChecked(true);
}

void ConfigureEvent_ChangeExperiencePoints::setAmount(int amount)
{
	if (rbSpecificAmount->isChecked())
		sbAmount->setValue(amount);
	else
		lblAmountVariable->setText(ProjectData::numberVariableIdToString(amount));
}

void ConfigureEvent_ChangeExperiencePoints::setIsStoredAmount(bool storedAmount)
{
	if (storedAmount)
		rbStoredAmount->setChecked(true);
	else
		rbSpecificAmount->setChecked(true);
}

void ConfigureEvent_ChangeExperiencePoints::setShowLevelUpMessage(bool showMessage)
{
	cbShowLevelUpMessage->setChecked(showMessage);
}

int ConfigureEvent_ChangeExperiencePoints::getTarget()
{
	if (rbEntireParty->isChecked())
		return ENTIRE_PARTY;
	else if (rbSpecificCharacter->isChecked())
		return SPECIFIC_CHARACTER;
	return REFERENCED_CHARACTER;
}

int ConfigureEvent_ChangeExperiencePoints::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	else if (rbReferencedCharacter->isChecked())
		return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	return 0;
}

bool ConfigureEvent_ChangeExperiencePoints::isIncreaseOperation()
{
	return rbIncreaseExperience->isChecked();
}

int ConfigureEvent_ChangeExperiencePoints::getAmount()
{
	if (rbSpecificAmount->isChecked())
		return sbAmount->value();
	return ProjectData::numberVariableIdFromString(lblAmountVariable->text());
}

bool ConfigureEvent_ChangeExperiencePoints::isStoredAmount()
{
	return rbStoredAmount->isChecked();
}

bool ConfigureEvent_ChangeExperiencePoints::showLevelUpMessage()
{
	return cbShowLevelUpMessage->isChecked();
}

void ConfigureEvent_ChangeExperiencePoints::on_bBrowseAmountVariables_clicked()
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

void ConfigureEvent_ChangeExperiencePoints::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_ChangeExperiencePoints::on_rbIncreaseExperience_toggled(bool checked)
{
	frmOption->setEnabled(checked);
}

void ConfigureEvent_ChangeExperiencePoints::on_rbReferencedCharacter_toggled(bool checked)
{
	wCharacterVariable->setEnabled(checked);
}

void ConfigureEvent_ChangeExperiencePoints::on_rbSpecificAmount_toggled(bool checked)
{
	wAmount->setEnabled(checked);
	wAmountVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeExperiencePoints::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
}
