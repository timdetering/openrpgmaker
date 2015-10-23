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

#include "configureevent_changeskills.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ChangeSkills::ConfigureEvent_ChangeSkills(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	cboxSkills->addItems(ProjectData::skillNames);
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	lblSkillVariable->setText(ProjectData::numberVariableIdToString(0));
	cboxCharacters->setEnabled(false);
	wCharacterVariable->setEnabled(false);
	wSkillVariable->setEnabled(false);
}

ConfigureEvent_ChangeSkills::~ConfigureEvent_ChangeSkills()
{
}

void ConfigureEvent_ChangeSkills::setTarget(int target)
{
	if (target == ENTIRE_PARTY)
		rbEntireParty->setChecked(true);
	else if (target == SPECIFIC_CHARACTER)
		rbSpecificCharacter->setChecked(true);
	else
		rbReferencedCharacter->setChecked(true);
}

void ConfigureEvent_ChangeSkills::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else if (rbReferencedCharacter->isChecked())
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

void ConfigureEvent_ChangeSkills::setOperation(bool add)
{
	if (add)
		rbAddSkill->setChecked(true);
	else
		rbRemoveSkill->setChecked(true);
}

void ConfigureEvent_ChangeSkills::setSkillID(int skillID)
{
	if (rbSpecificSkill->isChecked())
		cboxSkills->setCurrentIndex(skillID);
	else
		lblSkillVariable->setText(ProjectData::numberVariableIdToString(skillID));
}

void ConfigureEvent_ChangeSkills::setIsReferencedSkill(bool referenced)
{
	if (referenced)
		rbReferencedSkill->setChecked(true);
	else
		rbSpecificSkill->setChecked(true);
}

int ConfigureEvent_ChangeSkills::getTarget()
{
	if (rbEntireParty->isChecked())
		return ENTIRE_PARTY;
	else if (rbSpecificCharacter->isChecked())
		return SPECIFIC_CHARACTER;
	return REFERENCED_CHARACTER;
}

int ConfigureEvent_ChangeSkills::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	else if (rbReferencedCharacter->isChecked())
		return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	return 0;
}

bool ConfigureEvent_ChangeSkills::isAddOperation()
{
	return rbAddSkill->isChecked();
}

int ConfigureEvent_ChangeSkills::getSkillID()
{
	if (rbSpecificSkill->isChecked())
		return cboxSkills->currentIndex();
	return ProjectData::numberVariableIdFromString(lblSkillVariable->text());
}

bool ConfigureEvent_ChangeSkills::isReferencedSkill()
{
	return rbReferencedSkill->isChecked();
}

void ConfigureEvent_ChangeSkills::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_ChangeSkills::on_bBrowseSkillVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblSkillVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblSkillVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeSkills::on_rbReferencedCharacter_toggled(bool checked)
{
	wCharacterVariable->setEnabled(checked);
}

void ConfigureEvent_ChangeSkills::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
}

void ConfigureEvent_ChangeSkills::on_rbSpecificSkill_toggled(bool checked)
{
	cboxSkills->setEnabled(checked);
	wSkillVariable->setEnabled(!checked);
}
