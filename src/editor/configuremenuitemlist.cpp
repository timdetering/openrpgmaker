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

#include <QString>
#include "configuremenuitemlist.h"
#include "menuitemtable.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureMenuItemList::ConfigureMenuItemList(MenuItemTable *listToConfigure, QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	int tableType;
	list = listToConfigure;
	setupUi(this);
	setupSkillTypeList();
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	skills_lblPartyMember_Variable->setText(varText);
	equip_lblPartyMember_Variable->setText(varText);
	other_lblStartVariable->setText(varText);
	skills_wPartyMember_Variable->setEnabled(false);
	equip_wPartyMember_Variable->setEnabled(false);
	tableType = list->getTableType();
	if (tableType == MenuItemTable::TYPE_ITEMS)
	{
		MenuItemTable::ItemSettings settings;
		rbPartysItems->setChecked(true);
		list->getSettings(&settings);
		items_cbCommonItems->setChecked(settings.showCommonItems);
		items_cbWeapons->setChecked(settings.showWeapons);
		items_cbShields->setChecked(settings.showShields);
		items_cbArmor->setChecked(settings.showArmor);
		items_cbHelmets->setChecked(settings.showHelmets);
		items_cbAccessories->setChecked(settings.showAccessories);
		items_cbMedicine->setChecked(settings.showMedicine);
		items_cbSkillBooks->setChecked(settings.showSkillBooks);
		items_cbSeeds->setChecked(settings.showSeeds);
		items_cbSkillScrolls->setChecked(settings.showSkillScrolls);
		items_cbSwitches->setChecked(settings.showSwitches);
		items_cbEvents->setChecked(settings.showEvents);
	}
	else if (tableType == MenuItemTable::TYPE_SKILLS)
	{
		MenuItemTable::SkillSettings settings;
		rbSkills->setChecked(true);
		list->getSettings(&settings);
		if (settings.memberID == 0)
			skills_rbPartyMember1->setChecked(true);
		else if (settings.memberID == 1)
			skills_rbPartyMember2->setChecked(true);
		else if (settings.memberID == 2)
			skills_rbPartyMember3->setChecked(true);
		else if (settings.memberID == 3)
			skills_rbPartyMember3->setChecked(true);
		else
		{
			int varID = settings.memberID - 4;
			skills_rbPartyMemberVar->setChecked(true);
			skills_lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
		}
		for (int i = 0; i < lwSkillTypes->count(); ++i)
		{
			if (i < settings.skillTypesToShow.size())
				lwSkillTypes->item(i)->setCheckState((settings.skillTypesToShow[i]) ? Qt::Checked:Qt::Unchecked);
			else
				lwSkillTypes->item(i)->setCheckState(Qt::Checked);
		}
	}
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
	{
		MenuItemTable::EquipmentSettings settings;
		rbAvailableEquipment->setChecked(true);
		list->getSettings(&settings);
		if (settings.memberID == 0)
			equip_rbPartyMember1->setChecked(true);
		else if (settings.memberID == 1)
			equip_rbPartyMember2->setChecked(true);
		else if (settings.memberID == 2)
			equip_rbPartyMember3->setChecked(true);
		else if (settings.memberID == 3)
			equip_rbPartyMember3->setChecked(true);
		else
		{
			int varID = settings.memberID - 4;
			equip_rbPartyMemberVar->setChecked(true);
			equip_lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
		}
		if (settings.equipmentType == MenuItemTable::EquipmentSettings::TYPE_WEAPONS)
			equip_rbWeapons->setChecked(true);
		else if (settings.equipmentType == MenuItemTable::EquipmentSettings::TYPE_SHIELDS)
			equip_rbShields->setChecked(true);
		else if (settings.equipmentType == MenuItemTable::EquipmentSettings::TYPE_ARMOR)
			equip_rbArmor->setChecked(true);
		else if (settings.equipmentType == MenuItemTable::EquipmentSettings::TYPE_HELMETS)
			equip_rbHelmets->setChecked(true);
		else if (settings.equipmentType == MenuItemTable::EquipmentSettings::TYPE_ACCESSORIES)
			equip_rbAccessories->setChecked(true);
	}
	else
	{
		MenuItemTable::OtherSettings settings;
		list->getSettings(&settings);
		if (tableType == MenuItemTable::TYPE_CHARACTERS)
			rbCharacters->setChecked(true);
		else if (tableType == MenuItemTable::TYPE_CLASSES)
			rbClasses->setChecked(true);
		else if (tableType == MenuItemTable::TYPE_MONSTERS)
			rbMonsters->setChecked(true);
		else if (tableType == MenuItemTable::TYPE_VEHICLES)
			rbVehicles->setChecked(true);
		cbUseVariablesToSetState->setChecked(settings.useVariablesToSetState);
		other_lblStartVariable->setText(ProjectData::numberVariableIdToString(settings.startVariableID));
	}
}

ConfigureMenuItemList::~ConfigureMenuItemList()
{
}

void ConfigureMenuItemList::on_buttonBox_accepted()
{
	if (rbPartysItems->isChecked())
	{
		MenuItemTable::ItemSettings settings;
		list->setTableType(MenuItemTable::TYPE_ITEMS);
		settings.showCommonItems = items_cbCommonItems->isChecked();
		settings.showWeapons = items_cbWeapons->isChecked();
		settings.showShields = items_cbShields->isChecked();
		settings.showArmor = items_cbArmor->isChecked();
		settings.showHelmets = items_cbHelmets->isChecked();
		settings.showAccessories = items_cbAccessories->isChecked();
		settings.showMedicine = items_cbMedicine->isChecked();
		settings.showSkillBooks = items_cbSkillBooks->isChecked();
		settings.showSeeds = items_cbSeeds->isChecked();
		settings.showSkillScrolls = items_cbSkillScrolls->isChecked();
		settings.showSwitches = items_cbSwitches->isChecked();
		settings.showEvents = items_cbEvents->isChecked();
		list->setSettings(&settings);
	}
	else if (rbSkills->isChecked())
	{
		MenuItemTable::SkillSettings settings;
		list->setTableType(MenuItemTable::TYPE_SKILLS);
		if (skills_rbPartyMember1->isChecked())
			settings.memberID = 0;
		else if (skills_rbPartyMember2->isChecked())
			settings.memberID = 1;
		else if (skills_rbPartyMember3->isChecked())
			settings.memberID = 2;
		else if (skills_rbPartyMember4->isChecked())
			settings.memberID = 3;
		else if (skills_rbPartyMemberVar->isChecked())
			settings.memberID = ProjectData::numberVariableIdFromString(skills_lblPartyMember_Variable->text()) + 4;
		for (int i = 0; i < lwSkillTypes->count(); ++i)
			settings.skillTypesToShow.append((lwSkillTypes->item(i)->checkState() == Qt::Checked) ? true:false);
		list->setSettings(&settings);
	}
	else if (rbAvailableEquipment->isChecked())
	{
		MenuItemTable::EquipmentSettings settings;
		list->setTableType(MenuItemTable::TYPE_EQUIPMENT);
		if (equip_rbPartyMember1->isChecked())
			settings.memberID = 0;
		else if (equip_rbPartyMember2->isChecked())
			settings.memberID = 1;
		else if (equip_rbPartyMember3->isChecked())
			settings.memberID = 2;
		else if (equip_rbPartyMember4->isChecked())
			settings.memberID = 3;
		else if (equip_rbPartyMemberVar->isChecked())
			settings.memberID = ProjectData::numberVariableIdFromString(equip_lblPartyMember_Variable->text()) + 4;
		if (equip_rbWeapons->isChecked())
			settings.equipmentType = MenuItemTable::EquipmentSettings::TYPE_WEAPONS;
		else if (equip_rbShields->isChecked())
			settings.equipmentType = MenuItemTable::EquipmentSettings::TYPE_SHIELDS;
		else if (equip_rbArmor->isChecked())
			settings.equipmentType = MenuItemTable::EquipmentSettings::TYPE_ARMOR;
		else if (equip_rbHelmets->isChecked())
			settings.equipmentType = MenuItemTable::EquipmentSettings::TYPE_HELMETS;
		else if (equip_rbAccessories->isChecked())
			settings.equipmentType = MenuItemTable::EquipmentSettings::TYPE_ACCESSORIES;
		list->setSettings(&settings);
	}
	else
	{
		MenuItemTable::OtherSettings settings;
		if (rbCharacters->isChecked())
			list->setTableType(MenuItemTable::TYPE_CHARACTERS);
		else if (rbClasses->isChecked())
			list->setTableType(MenuItemTable::TYPE_CLASSES);
		else if (rbMonsters->isChecked())
			list->setTableType(MenuItemTable::TYPE_MONSTERS);
		else if (rbVehicles->isChecked())
			list->setTableType(MenuItemTable::TYPE_VEHICLES);
		settings.useVariablesToSetState = cbUseVariablesToSetState->isChecked();
		if (settings.useVariablesToSetState)
			settings.startVariableID = ProjectData::numberVariableIdFromString(other_lblStartVariable->text());
		else
			settings.startVariableID = 0;
		list->setSettings(&settings);
	}
}

void ConfigureMenuItemList::on_cbUseVariablesToSetState_toggled(bool checked)
{
	wUseVariablesToSetState->setEnabled(checked);
}

void ConfigureMenuItemList::on_rbAvailableEquipment_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wAvailableEquipment);
}

void ConfigureMenuItemList::on_rbCharacters_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wOther);
}

void ConfigureMenuItemList::on_rbClasses_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wOther);
}

void ConfigureMenuItemList::on_rbMonsters_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wOther);
}

void ConfigureMenuItemList::on_rbPartysItems_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wItems);
}

void ConfigureMenuItemList::on_rbSkills_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wSkills);
}

void ConfigureMenuItemList::on_rbVehicles_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wOther);
}

void ConfigureMenuItemList::on_skills_rbPartyMemberVar_toggled(bool checked)
{
	skills_wPartyMember_Variable->setEnabled(checked);
}

void ConfigureMenuItemList::on_equip_rbPartyMemberVar_toggled(bool checked)
{
	equip_wPartyMember_Variable->setEnabled(checked);
}

void ConfigureMenuItemList::on_skills_bPartyMember_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(skills_lblPartyMember_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		skills_lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureMenuItemList::on_equip_bPartyMember_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(equip_lblPartyMember_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		equip_lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureMenuItemList::on_other_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(other_lblStartVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		other_lblStartVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureMenuItemList::setupSkillTypeList()
{
	for (int i = 0; i < ProjectData::skillSubsets.size(); ++i)
	{
		QListWidgetItem *item = new QListWidgetItem(ProjectData::skillSubsets[i]->name);
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Checked);
		lwSkillTypes->addItem(item);
	}
}
