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

#include "configureevent_changeheroname.h"
#include "projectdata.h"
#include "projectdata_character.h"
#include "storagefile.h"
#include "variableselector.h"

ConfigureEvent_ChangeHeroName::ConfigureEvent_ChangeHeroName(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	lblStringVariable->setText(ProjectData::stringVariableIdToString(0));
	wCharacterVariable->setEnabled(false);
	wStringVariable->setEnabled(false);
}

ConfigureEvent_ChangeHeroName::~ConfigureEvent_ChangeHeroName()
{
}

void ConfigureEvent_ChangeHeroName::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
	{
		cboxCharacters->setCurrentIndex(characterID);
		setString(ProjectData::characterNames[characterID]);
	}
	else
	{
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
		setString(ProjectData::characterNames[0]);
	}
}

void ConfigureEvent_ChangeHeroName::setIsReferencedCharacter(bool referenced)
{
	if (referenced)
		rbReferencedCharacter->setChecked(true);
	else
		rbSpecificCharacter->setChecked(true);
}

void ConfigureEvent_ChangeHeroName::setString(QString string)
{
	leString->setText(string);
}

void ConfigureEvent_ChangeHeroName::setStringID(int stringID)
{
	if (stringID >= 0)
	{
		rbValueOfString->setChecked(true);
		lblStringVariable->setText(ProjectData::stringVariableIdToString(stringID));
	}
	else
		rbSpecificValue->setChecked(true);
}

int ConfigureEvent_ChangeHeroName::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
}

bool ConfigureEvent_ChangeHeroName::isReferencedCharacter()
{
	return rbReferencedCharacter->isChecked();
}

QString ConfigureEvent_ChangeHeroName::getString()
{
	return leString->text();
}

int ConfigureEvent_ChangeHeroName::getStringID()
{
	if (rbSpecificValue->isChecked())
		return -1;
	return ProjectData::stringVariableIdFromString(lblStringVariable->text());
}

void ConfigureEvent_ChangeHeroName::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_ChangeHeroName::on_bBrowseStringVariables_clicked()
{
	VariableSelector *variableSelector;
	int stringID = ProjectData::stringVariableIdFromString(lblStringVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_STRINGS, stringID);
	if (variableSelector->exec())
	{
		stringID = variableSelector->getVariableID();
		lblCharacterVariable->setText(ProjectData::stringVariableIdToString(stringID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeHeroName::on_cboxCharacters_currentIndexChanged(int index)
{
	setString(ProjectData::characterNames[index]);
}

void ConfigureEvent_ChangeHeroName::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
	wCharacterVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeHeroName::on_rbSpecificValue_toggled(bool checked)
{
	leString->setEnabled(checked);
	wStringVariable->setEnabled(!checked);
}
