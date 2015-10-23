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

#include "configureevent_changecharacterbasestats.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ChangeCharacterBaseStats::ConfigureEvent_ChangeCharacterBaseStats(QWidget *parent) : QDialog(parent)
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
	QObject::connect(rbMaxHP, SIGNAL(clicked()), this, SLOT(selectedStatChanged()));
	QObject::connect(rbMaxMP, SIGNAL(clicked()), this, SLOT(selectedStatChanged()));
	QObject::connect(rbAttack, SIGNAL(clicked()), this, SLOT(selectedStatChanged()));
	QObject::connect(rbDefense, SIGNAL(clicked()), this, SLOT(selectedStatChanged()));
	QObject::connect(rbWisdom, SIGNAL(clicked()), this, SLOT(selectedStatChanged()));
	QObject::connect(rbAgility, SIGNAL(clicked()), this, SLOT(selectedStatChanged()));
}

ConfigureEvent_ChangeCharacterBaseStats::~ConfigureEvent_ChangeCharacterBaseStats()
{
}

void ConfigureEvent_ChangeCharacterBaseStats::setTarget(int target)
{
	if (target == ENTIRE_PARTY)
		rbEntireParty->setChecked(true);
	else if (target == SPECIFIC_CHARACTER)
		rbSpecificCharacter->setChecked(true);
	else
		rbReferencedCharacter->setChecked(true);
}

void ConfigureEvent_ChangeCharacterBaseStats::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else if (rbReferencedCharacter->isChecked())
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

void ConfigureEvent_ChangeCharacterBaseStats::setOperation(bool increase)
{
	if (increase)
		rbIncreaseStat->setChecked(true);
	else
		rbDecreaseStat->setChecked(true);
}

void ConfigureEvent_ChangeCharacterBaseStats::setStat(int stat)
{
	if (stat == MAX_HP)
		rbMaxHP->setChecked(true);
	else if (stat == MAX_MP)
		rbMaxMP->setChecked(true);
	else if (stat == ATTACK)
		rbAttack->setChecked(true);
	else if (stat == DEFENSE)
		rbDefense->setChecked(true);
	else if (stat == WISDOM)
		rbWisdom->setChecked(true);
	else if (stat == AGILITY)
		rbAgility->setChecked(true);
}

void ConfigureEvent_ChangeCharacterBaseStats::setAmount(int amount)
{
	if (rbSpecificAmount->isChecked())
		sbAmount->setValue(amount);
	else
		lblAmountVariable->setText(ProjectData::numberVariableIdToString(amount));
}

void ConfigureEvent_ChangeCharacterBaseStats::setIsStoredAmount(bool storedAmount)
{
	if (storedAmount)
		rbStoredAmount->setChecked(true);
	else
		rbSpecificAmount->setChecked(true);
}

int ConfigureEvent_ChangeCharacterBaseStats::getTarget()
{
	if (rbEntireParty->isChecked())
		return ENTIRE_PARTY;
	else if (rbSpecificCharacter->isChecked())
		return SPECIFIC_CHARACTER;
	return REFERENCED_CHARACTER;
}

int ConfigureEvent_ChangeCharacterBaseStats::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	else if (rbReferencedCharacter->isChecked())
		return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	return 0;
}

bool ConfigureEvent_ChangeCharacterBaseStats::isIncreaseOperation()
{
	return rbIncreaseStat->isChecked();
}

int ConfigureEvent_ChangeCharacterBaseStats::getStat()
{
	if (rbMaxHP->isChecked())
		return MAX_HP;
	else if (rbMaxMP->isChecked())
		return MAX_MP;
	else if (rbAttack->isChecked())
		return ATTACK;
	else if (rbDefense->isChecked())
		return DEFENSE;
	else if (rbWisdom->isChecked())
		return WISDOM;
	return AGILITY;
}

int ConfigureEvent_ChangeCharacterBaseStats::getAmount()
{
	if (rbSpecificAmount->isChecked())
		return sbAmount->value();
	return ProjectData::numberVariableIdFromString(lblAmountVariable->text());
}

bool ConfigureEvent_ChangeCharacterBaseStats::isStoredAmount()
{
	return rbStoredAmount->isChecked();
}

void ConfigureEvent_ChangeCharacterBaseStats::on_bBrowseAmountVariables_clicked()
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

void ConfigureEvent_ChangeCharacterBaseStats::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_ChangeCharacterBaseStats::on_rbReferencedCharacter_toggled(bool checked)
{
	wCharacterVariable->setEnabled(checked);
}

void ConfigureEvent_ChangeCharacterBaseStats::on_rbSpecificAmount_toggled(bool checked)
{
	wAmount->setEnabled(checked);
	wAmountVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeCharacterBaseStats::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
}

void ConfigureEvent_ChangeCharacterBaseStats::selectedStatChanged()
{
	if (rbMaxHP->isChecked())
		sbAmount->setMaximum(9999);
	else
		sbAmount->setMaximum(999);
}
