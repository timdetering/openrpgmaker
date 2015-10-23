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

#include "configureevent_changecharactercondition.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ChangeCharacterCondition::ConfigureEvent_ChangeCharacterCondition(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	cboxConditions->addItems(ProjectData::conditionNames);
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	lblConditionVariable->setText(ProjectData::numberVariableIdToString(0));
	cboxCharacters->setEnabled(false);
	wCharacterVariable->setEnabled(false);
	wConditionVariable->setEnabled(false);
}

ConfigureEvent_ChangeCharacterCondition::~ConfigureEvent_ChangeCharacterCondition()
{
}

void ConfigureEvent_ChangeCharacterCondition::setTarget(int target)
{
	if (target == ENTIRE_PARTY)
		rbEntireParty->setChecked(true);
	else if (target == SPECIFIC_CHARACTER)
		rbSpecificCharacter->setChecked(true);
	else
		rbReferencedCharacter->setChecked(true);
}

void ConfigureEvent_ChangeCharacterCondition::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else if (rbReferencedCharacter->isChecked())
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

void ConfigureEvent_ChangeCharacterCondition::setOperation(bool add)
{
	if (add)
		rbAddCondition->setChecked(true);
	else
		rbRemoveCondition->setChecked(true);
}

void ConfigureEvent_ChangeCharacterCondition::setConditionID(int conditionID)
{
	if (rbSpecificCondition->isChecked())
		cboxConditions->setCurrentIndex(conditionID);
	else
		lblConditionVariable->setText(ProjectData::numberVariableIdToString(conditionID));
}

void ConfigureEvent_ChangeCharacterCondition::setIsReferencedCondition(bool referenced)
{
	if (referenced)
		rbReferencedCondition->setChecked(true);
	else
		rbSpecificCondition->setChecked(true);
}

int ConfigureEvent_ChangeCharacterCondition::getTarget()
{
	if (rbEntireParty->isChecked())
		return ENTIRE_PARTY;
	else if (rbSpecificCharacter->isChecked())
		return SPECIFIC_CHARACTER;
	return REFERENCED_CHARACTER;
}

int ConfigureEvent_ChangeCharacterCondition::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	else if (rbReferencedCharacter->isChecked())
		return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	return 0;
}

bool ConfigureEvent_ChangeCharacterCondition::isAddOperation()
{
	return rbAddCondition->isChecked();
}

int ConfigureEvent_ChangeCharacterCondition::getConditionID()
{
	if (rbSpecificCondition->isChecked())
		return cboxConditions->currentIndex();
	return ProjectData::numberVariableIdFromString(lblConditionVariable->text());
}

bool ConfigureEvent_ChangeCharacterCondition::isReferencedCondition()
{
	return rbReferencedCondition->isChecked();
}

void ConfigureEvent_ChangeCharacterCondition::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_ChangeCharacterCondition::on_bBrowseConditionVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblConditionVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblConditionVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeCharacterCondition::on_rbReferencedCharacter_toggled(bool checked)
{
	wCharacterVariable->setEnabled(checked);
}

void ConfigureEvent_ChangeCharacterCondition::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
}

void ConfigureEvent_ChangeCharacterCondition::on_rbSpecificCondition_toggled(bool checked)
{
	cboxConditions->setEnabled(checked);
	wConditionVariable->setEnabled(!checked);
}
