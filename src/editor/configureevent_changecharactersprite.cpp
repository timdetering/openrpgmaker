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

#include "configureevent_changecharactersprite.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ChangeCharacterSprite::ConfigureEvent_ChangeCharacterSprite(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	cboxSprites->addItems(ProjectData::characterSpriteNames);
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	lblSpriteVariable->setText(ProjectData::numberVariableIdToString(0));
	wCharacterVariable->setEnabled(false);
	wSpriteVariable->setEnabled(false);
}

ConfigureEvent_ChangeCharacterSprite::~ConfigureEvent_ChangeCharacterSprite()
{
}

void ConfigureEvent_ChangeCharacterSprite::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

void ConfigureEvent_ChangeCharacterSprite::setIsReferencedCharacter(bool referenced)
{
	if (referenced)
		rbReferencedCharacter->setChecked(true);
	else
		rbSpecificCharacter->setChecked(true);
}

void ConfigureEvent_ChangeCharacterSprite::setSpriteID(int spriteID)
{
	if (rbSpecificSprite->isChecked())
		cboxSprites->setCurrentIndex(spriteID);
	else
		lblSpriteVariable->setText(ProjectData::numberVariableIdToString(spriteID));
}

void ConfigureEvent_ChangeCharacterSprite::setIsReferencedSprite(bool referenced)
{
	if (referenced)
		rbReferencedSprite->setChecked(true);
	else
		rbSpecificSprite->setChecked(true);
}

int ConfigureEvent_ChangeCharacterSprite::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
}

bool ConfigureEvent_ChangeCharacterSprite::isReferencedCharacter()
{
	return rbReferencedCharacter->isChecked();
}

int ConfigureEvent_ChangeCharacterSprite::getSpriteID()
{
	if (rbSpecificSprite->isChecked())
		return cboxSprites->currentIndex();
	return ProjectData::numberVariableIdFromString(lblSpriteVariable->text());
}

bool ConfigureEvent_ChangeCharacterSprite::isReferencedSprite()
{
	return rbReferencedSprite->isChecked();
}

void ConfigureEvent_ChangeCharacterSprite::on_bBrowseSpriteVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblSpriteVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblSpriteVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeCharacterSprite::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_ChangeCharacterSprite::on_rbSpecificSprite_toggled(bool checked)
{
	cboxSprites->setEnabled(checked);
	wSpriteVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeCharacterSprite::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
	wCharacterVariable->setEnabled(!checked);
}
