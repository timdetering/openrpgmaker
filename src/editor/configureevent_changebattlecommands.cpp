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

#include "configureevent_changebattlecommands.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ChangeBattleCommands::ConfigureEvent_ChangeBattleCommands(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
		cboxBattleCommands->addItem(ProjectData::battleCommands[i]->name);
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	lblCommandVariable->setText(ProjectData::numberVariableIdToString(0));
	wCharacterVariable->setEnabled(false);
	wCommandVariable->setEnabled(false);
}

ConfigureEvent_ChangeBattleCommands::~ConfigureEvent_ChangeBattleCommands()
{
}

void ConfigureEvent_ChangeBattleCommands::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

void ConfigureEvent_ChangeBattleCommands::setIsReferencedCharacter(bool referenced)
{
	if (referenced)
		rbReferencedCharacter->setChecked(true);
	else
		rbSpecificCharacter->setChecked(true);
}

void ConfigureEvent_ChangeBattleCommands::setCommandID(int commandID)
{
	if (rbSpecificCommand->isChecked())
		cboxBattleCommands->setCurrentIndex(commandID);
	else
		lblCommandVariable->setText(ProjectData::numberVariableIdToString(commandID));
}

void ConfigureEvent_ChangeBattleCommands::setIsReferencedCommand(bool referenced)
{
	if (referenced)
		rbReferencedCommand->setChecked(true);
	else
		rbSpecificCommand->setChecked(true);
}

void ConfigureEvent_ChangeBattleCommands::setOperation(bool add)
{
	if (add)
		rbAdd->setChecked(true);
	else
		rbRemove->setChecked(true);
}

int ConfigureEvent_ChangeBattleCommands::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
}

bool ConfigureEvent_ChangeBattleCommands::isReferencedCharacter()
{
	return rbReferencedCharacter->isChecked();
}

int ConfigureEvent_ChangeBattleCommands::getCommandID()
{
	if (rbSpecificCommand->isChecked())
		return cboxBattleCommands->currentIndex();
	return ProjectData::numberVariableIdFromString(lblCommandVariable->text());
}

bool ConfigureEvent_ChangeBattleCommands::isReferencedCommand()
{
	return rbReferencedCommand->isChecked();
}

bool ConfigureEvent_ChangeBattleCommands::isAddOperation()
{
	return rbAdd->isChecked();
}

void ConfigureEvent_ChangeBattleCommands::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_ChangeBattleCommands::on_bBrowseCommandVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblCommandVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblCommandVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeBattleCommands::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
	wCharacterVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeBattleCommands::on_rbSpecificCommand_toggled(bool checked)
{
	cboxBattleCommands->setEnabled(checked);
	wCommandVariable->setEnabled(!checked);
}
