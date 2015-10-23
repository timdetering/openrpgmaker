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

#include "configureevent_changeclass.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ChangeClass::ConfigureEvent_ChangeClass(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	cboxClasses->addItems(ProjectData::classNames);
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	lblClassVariable->setText(ProjectData::numberVariableIdToString(0));
	wCharacterVariable->setEnabled(false);
	wClassVariable->setEnabled(false);
}

ConfigureEvent_ChangeClass::~ConfigureEvent_ChangeClass()
{
}

void ConfigureEvent_ChangeClass::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

void ConfigureEvent_ChangeClass::setIsReferencedCharacter(bool referenced)
{
	if (referenced)
		rbReferencedCharacter->setChecked(true);
	else
		rbSpecificCharacter->setChecked(true);
}

void ConfigureEvent_ChangeClass::setClassID(int classID)
{
	if (rbSpecificClass->isChecked())
		cboxClasses->setCurrentIndex(classID);
	else
		lblClassVariable->setText(ProjectData::numberVariableIdToString(classID));
}

void ConfigureEvent_ChangeClass::setIsReferencedClass(bool referenced)
{
	if (referenced)
		rbReferencedClass->setChecked(true);
	else
		rbSpecificClass->setChecked(true);
}

void ConfigureEvent_ChangeClass::setRetainCurrentLevel(bool retainCurrentLevel)
{
	if (retainCurrentLevel)
		rbRetainCurrentLevel->setChecked(true);
	else
		rbCharacterBecomesLevel1->setChecked(true);
}

void ConfigureEvent_ChangeClass::setBaseStatChange(int statChange)
{
	if (statChange == CHANGE_NONE)
		rbNoChange->setChecked(true);
	else if (statChange == CHANGE_HALVESTATS)
		rbHalveStats->setChecked(true);
	else if (statChange == CHANGE_TOLEVEL1OFCLASS)
		rbChangeToLevel1->setChecked(true);
	else if (statChange == CHANGE_TOCURRENTLEVELOFCLASS)
		rbChangeToCurrentLevel->setChecked(true);
}

void ConfigureEvent_ChangeClass::setLearnedSkills(int learnedSkills)
{
	if (learnedSkills == LEARNED_NONE)
		rbNoSkillChange->setChecked(true);
	else if (learnedSkills == LEARNED_ADDUPTONEWDELETEOLD)
		rbAddNewDeleteOldSkills->setChecked(true);
	else if (learnedSkills == LEARNED_ADDUPTONEWKEEPOLD)
		rbAddNewKeepOldSkills->setChecked(true);
}

void ConfigureEvent_ChangeClass::setShowLevelUpMessage(bool showMessage)
{
	cbShowLevelUpMessage->setChecked(showMessage);
}

int ConfigureEvent_ChangeClass::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
}

bool ConfigureEvent_ChangeClass::isReferencedCharacter()
{
	return rbReferencedCharacter->isChecked();
}

int ConfigureEvent_ChangeClass::getClassID()
{
	if (rbSpecificClass->isChecked())
		return cboxClasses->currentIndex();
	return ProjectData::numberVariableIdFromString(lblClassVariable->text());
}

bool ConfigureEvent_ChangeClass::isReferencedClass()
{
	return rbReferencedClass->isChecked();
}

bool ConfigureEvent_ChangeClass::retainCurrentLevel()
{
	return rbRetainCurrentLevel->isChecked();
}

int ConfigureEvent_ChangeClass::getBaseStatChange()
{
	if (rbNoChange->isChecked())
		return CHANGE_NONE;
	else if (rbHalveStats->isChecked())
		return CHANGE_HALVESTATS;
	else if (rbChangeToLevel1->isChecked())
		return CHANGE_TOLEVEL1OFCLASS;
	return CHANGE_TOCURRENTLEVELOFCLASS;
}

int ConfigureEvent_ChangeClass::getLearnedSkills()
{
	if (rbNoSkillChange->isChecked())
		return LEARNED_NONE;
	else if (rbAddNewDeleteOldSkills->isChecked())
		return LEARNED_ADDUPTONEWDELETEOLD;
	return LEARNED_ADDUPTONEWKEEPOLD;
}

bool ConfigureEvent_ChangeClass::showLevelUpMessage()
{
	return cbShowLevelUpMessage->isChecked();
}

void ConfigureEvent_ChangeClass::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_ChangeClass::on_bBrowseClassVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblClassVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblClassVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeClass::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
	wCharacterVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeClass::on_rbSpecificClass_toggled(bool checked)
{
	cboxClasses->setEnabled(checked);
	wClassVariable->setEnabled(!checked);
}
