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

#include "configureevent_completehealing.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_CompleteHealing::ConfigureEvent_CompleteHealing(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	cboxCharacters->setEnabled(false);
	wCharacterVariable->setEnabled(false);
}

ConfigureEvent_CompleteHealing::~ConfigureEvent_CompleteHealing()
{
}

void ConfigureEvent_CompleteHealing::setTarget(int target)
{
	if (target == ENTIRE_PARTY)
		rbEntireParty->setChecked(true);
	else if (target == SPECIFIC_CHARACTER)
		rbSpecificCharacter->setChecked(true);
	else
		rbReferencedCharacter->setChecked(true);
}

void ConfigureEvent_CompleteHealing::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else if (rbReferencedCharacter->isChecked())
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

int ConfigureEvent_CompleteHealing::getTarget()
{
	if (rbEntireParty->isChecked())
		return ENTIRE_PARTY;
	else if (rbSpecificCharacter->isChecked())
		return SPECIFIC_CHARACTER;
	return REFERENCED_CHARACTER;
}

int ConfigureEvent_CompleteHealing::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	else if (rbReferencedCharacter->isChecked())
		return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	return 0;
}

void ConfigureEvent_CompleteHealing::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_CompleteHealing::on_rbReferencedCharacter_toggled(bool checked)
{
	wCharacterVariable->setEnabled(checked);
}

void ConfigureEvent_CompleteHealing::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
}
