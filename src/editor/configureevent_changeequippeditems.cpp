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

#include "configureevent_changeequippeditems.h"
#include "projectdata.h"
#include "projectdata_item.h"
#include "storagefile.h"
#include "variableselector.h"

ConfigureEvent_ChangeEquippedItems::ConfigureEvent_ChangeEquippedItems(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxCharacters->addItems(ProjectData::characterNames);
	updateItemList();
	lblCharacterVariable->setText(ProjectData::numberVariableIdToString(0));
	lblItemVariable->setText(ProjectData::numberVariableIdToString(0));
	cboxCharacters->setEnabled(false);
	wCharacterVariable->setEnabled(false);
	wItemVariable->setEnabled(false);
	frmUnequipSlot->setEnabled(false);
}

ConfigureEvent_ChangeEquippedItems::~ConfigureEvent_ChangeEquippedItems()
{
}

void ConfigureEvent_ChangeEquippedItems::setTarget(int target)
{
	if (target == ENTIRE_PARTY)
		rbEntireParty->setChecked(true);
	else if (target == SPECIFIC_CHARACTER)
		rbSpecificCharacter->setChecked(true);
	else
		rbReferencedCharacter->setChecked(true);
}

void ConfigureEvent_ChangeEquippedItems::setCharacterID(int characterID)
{
	if (rbSpecificCharacter->isChecked())
		cboxCharacters->setCurrentIndex(characterID);
	else if (rbReferencedCharacter->isChecked())
		lblCharacterVariable->setText(ProjectData::numberVariableIdToString(characterID));
}

void ConfigureEvent_ChangeEquippedItems::setOperation(bool equip)
{
	if (equip)
		rbEquipItem->setChecked(true);
	else
		rbUnequipItem->setChecked(true);
}

void ConfigureEvent_ChangeEquippedItems::setItemID(int itemID)
{
	if (rbSpecificItem->isChecked())
	{
		if (itemIDs.contains(itemID))
			cboxItems->setCurrentIndex(itemIDs.indexOf(itemID));
		else
			cboxItems->setCurrentIndex(0);
	}
	else
		lblItemVariable->setText(ProjectData::numberVariableIdToString(itemID));
}

void ConfigureEvent_ChangeEquippedItems::setIsReferencedItem(bool referenced)
{
	if (referenced)
		rbReferencedItem->setChecked(true);
	else
		rbSpecificItem->setChecked(true);
}

void ConfigureEvent_ChangeEquippedItems::setUnequippedSlot(int slot)
{
	if (slot == ALL)
		rbAll->setChecked(true);
	else if (slot == WEAPON)
		rbWeapon->setChecked(true);
	else if (slot == SHIELD)
		rbShield->setChecked(true);
	else if (slot == ARMOR)
		rbArmor->setChecked(true);
	else if (slot == HELMET)
		rbHelmet->setChecked(true);
	else if (slot == ACCESSORY)
		rbAccessory->setChecked(true);
}

int ConfigureEvent_ChangeEquippedItems::getTarget()
{
	if (rbEntireParty->isChecked())
		return ENTIRE_PARTY;
	else if (rbSpecificCharacter->isChecked())
		return SPECIFIC_CHARACTER;
	return REFERENCED_CHARACTER;
}

int ConfigureEvent_ChangeEquippedItems::getCharacterID()
{
	if (rbSpecificCharacter->isChecked())
		return cboxCharacters->currentIndex();
	else if (rbReferencedCharacter->isChecked())
		return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	return 0;
}

bool ConfigureEvent_ChangeEquippedItems::isEquipOperation()
{
	return rbEquipItem->isChecked();
}

int ConfigureEvent_ChangeEquippedItems::getItemID()
{
	if (rbSpecificItem->isChecked())
		return itemIDs[cboxItems->currentIndex()];
	return ProjectData::numberVariableIdFromString(lblItemVariable->text());
}

bool ConfigureEvent_ChangeEquippedItems::isReferencedItem()
{
	return rbReferencedItem->isChecked();
}

int ConfigureEvent_ChangeEquippedItems::getUnequippedSlot()
{
	if (rbAll->isChecked())
		return ALL;
	else if (rbWeapon->isChecked())
		return WEAPON;
	else if (rbShield->isChecked())
		return SHIELD;
	else if (rbArmor->isChecked())
		return ARMOR;
	else if (rbHelmet->isChecked())
		return HELMET;
	return ACCESSORY;
}

void ConfigureEvent_ChangeEquippedItems::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_ChangeEquippedItems::on_bBrowseItemVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblItemVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblItemVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeEquippedItems::on_rbEquipItem_toggled(bool checked)
{
	frmItem->setEnabled(checked);
	frmUnequipSlot->setEnabled(!checked);
}

void ConfigureEvent_ChangeEquippedItems::on_rbReferencedCharacter_toggled(bool checked)
{
	wCharacterVariable->setEnabled(checked);
}

void ConfigureEvent_ChangeEquippedItems::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
}

void ConfigureEvent_ChangeEquippedItems::on_rbSpecificItem_toggled(bool checked)
{
	cboxItems->setEnabled(checked);
	wItemVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeEquippedItems::updateItemList()
{
	StorageFile storageFile;
	QString fileLocation;
	for (int i = 0; i < ProjectData::itemNames.size(); ++i)
	{
		ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>(i, __FILE__, __LINE__);
		int itemType = (*itemRef)->getType();
		if (itemType >= Item::TYPE_WEAPON && itemType <= Item::TYPE_ACCESSORY)
		{
			cboxItems->addItem(ProjectData::itemNames[i]);
			itemIDs.append(i);
		}
		delete itemRef;
	}
}
