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

#include "configureevent_changemenuitemlist.h"
#include "menuobject.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"
#include "storagefile.h"
#include "variableselector.h"

ConfigureEvent_ChangeMenuItemList::ConfigureEvent_ChangeMenuItemList(int tableType, MenuItemTable::Settings *settings, int menuID, int menuType, QWidget *parent) : QDialog(parent)
{
	QList<MenuObject*> menuObjects;
	QString varText = ProjectData::numberVariableIdToString(0);
	tableSettings = settings;
	type = tableType;
	setupUi(this);
	setupSkillTypeList();
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	if (menuType == MenuObject::MENUTYPE_MAINMENU)
	{
		ProjectData::DataReference<MainMenuLayout> *layoutRef = new ProjectData::DataReference<MainMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_ITEMLIST);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuItemList->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_POPUPMENU)
	{
		ProjectData::DataReference<PopupMenuLayout> *layoutRef = new ProjectData::DataReference<PopupMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_ITEMLIST);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuItemList->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_TITLESCREENMENU)
	{
		TitleMenuLayout *layout = ProjectData::titleMenuLayout;
		menuObjects = layout->getObjectsByType(MenuObject::TYPE_ITEMLIST);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuItemList->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		layout = NULL;
	}
	if (cboxMenuItemList->count() == 0)
	{
		cboxMenuItemList->addItem("No Item Lists To Configure");
		wConfigureItemList->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	skills_lblPartyMember_Variable->setText(varText);
	equip_lblPartyMember_Variable->setText(varText);
	other_lblStartVariable->setText(varText);
	if (tableType == MenuItemTable::TYPE_ITEMS)
	{
		rbPartysItems->setChecked(true);
		items_cbCommonItems->setChecked(tableSettings->items->showCommonItems);
		items_cbWeapons->setChecked(tableSettings->items->showWeapons);
		items_cbShields->setChecked(tableSettings->items->showShields);
		items_cbArmor->setChecked(tableSettings->items->showArmor);
		items_cbHelmets->setChecked(tableSettings->items->showHelmets);
		items_cbAccessories->setChecked(tableSettings->items->showAccessories);
		items_cbMedicine->setChecked(tableSettings->items->showMedicine);
		items_cbSkillBooks->setChecked(tableSettings->items->showSkillBooks);
		items_cbSeeds->setChecked(tableSettings->items->showSeeds);
		items_cbSkillScrolls->setChecked(tableSettings->items->showSkillScrolls);
		items_cbSwitches->setChecked(tableSettings->items->showSwitches);
		items_cbEvents->setChecked(tableSettings->items->showEvents);
	}
	else if (tableType == MenuItemTable::TYPE_SKILLS)
	{
		rbSkills->setChecked(true);
		if (tableSettings->skills->memberID == 0)
			skills_rbPartyMember1->setChecked(true);
		else if (tableSettings->skills->memberID == 1)
			skills_rbPartyMember2->setChecked(true);
		else if (tableSettings->skills->memberID == 2)
			skills_rbPartyMember3->setChecked(true);
		else if (tableSettings->skills->memberID == 3)
			skills_rbPartyMember3->setChecked(true);
		else
		{
			int varID = tableSettings->skills->memberID - 4;
			skills_rbPartyMemberVar->setChecked(true);
			skills_lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
		}
		for (int i = 0; i < lwSkillTypes->count(); ++i)
		{
			if (i < tableSettings->skills->skillTypesToShow.size())
				lwSkillTypes->item(i)->setCheckState((tableSettings->skills->skillTypesToShow[i]) ? Qt::Checked:Qt::Unchecked);
			else
				lwSkillTypes->item(i)->setCheckState(Qt::Checked);
		}
	}
	else if (tableType == MenuItemTable::TYPE_EQUIPMENT)
	{
		rbAvailableEquipment->setChecked(true);
		if (tableSettings->equipment->memberID == 0)
			equip_rbPartyMember1->setChecked(true);
		else if (tableSettings->equipment->memberID == 1)
			equip_rbPartyMember2->setChecked(true);
		else if (tableSettings->equipment->memberID == 2)
			equip_rbPartyMember3->setChecked(true);
		else if (tableSettings->equipment->memberID == 3)
			equip_rbPartyMember3->setChecked(true);
		else
		{
			int varID = tableSettings->equipment->memberID - 4;
			equip_rbPartyMemberVar->setChecked(true);
			equip_lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
		}
		if (tableSettings->equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_WEAPONS)
			equip_rbWeapons->setChecked(true);
		else if (tableSettings->equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_SHIELDS)
			equip_rbShields->setChecked(true);
		else if (tableSettings->equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_ARMOR)
			equip_rbArmor->setChecked(true);
		else if (tableSettings->equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_HELMETS)
			equip_rbHelmets->setChecked(true);
		else if (tableSettings->equipment->equipmentType == MenuItemTable::EquipmentSettings::TYPE_ACCESSORIES)
			equip_rbAccessories->setChecked(true);
	}
	else
	{
		if (tableType == MenuItemTable::TYPE_CHARACTERS)
			rbCharacters->setChecked(true);
		else if (tableType == MenuItemTable::TYPE_CLASSES)
			rbClasses->setChecked(true);
		else if (tableType == MenuItemTable::TYPE_MONSTERS)
			rbMonsters->setChecked(true);
		else if (tableType == MenuItemTable::TYPE_VEHICLES)
			rbVehicles->setChecked(true);
		cbUseVariablesToSetState->setChecked(tableSettings->other->useVariablesToSetState);
		other_lblStartVariable->setText(ProjectData::numberVariableIdToString(tableSettings->other->startVariableID));
	}
}

ConfigureEvent_ChangeMenuItemList::~ConfigureEvent_ChangeMenuItemList()
{
}

void ConfigureEvent_ChangeMenuItemList::setItemListObject(QString object)
{
	int index = cboxMenuItemList->findText(object);
	if (index != -1)
		cboxMenuItemList->setCurrentIndex(index);
}

int ConfigureEvent_ChangeMenuItemList::getTableType()
{
	if (rbPartysItems->isChecked())
		return MenuItemTable::TYPE_ITEMS;
	else if (rbSkills->isChecked())
		return MenuItemTable::TYPE_SKILLS;
	else if (rbAvailableEquipment->isChecked())
		return MenuItemTable::TYPE_EQUIPMENT;
	else if (rbCharacters->isChecked())
		return MenuItemTable::TYPE_CHARACTERS;
	else if (rbClasses->isChecked())
		return MenuItemTable::TYPE_CLASSES;
	else if (rbMonsters->isChecked())
		return MenuItemTable::TYPE_MONSTERS;
	return MenuItemTable::TYPE_VEHICLES;
}

QString ConfigureEvent_ChangeMenuItemList::getItemListObject()
{
	return cboxMenuItemList->currentText();
}

void ConfigureEvent_ChangeMenuItemList::on_buttonBox_accepted()
{
	if (type == MenuItemTable::TYPE_ITEMS)
		delete (tableSettings->items);
	else if (type == MenuItemTable::TYPE_SKILLS)
		delete (tableSettings->skills);
	else if (type == MenuItemTable::TYPE_EQUIPMENT)
		delete (tableSettings->equipment);
	else
		delete (tableSettings->other);
	if (rbPartysItems->isChecked())
	{
		tableSettings->items = new MenuItemTable::ItemSettings;
		tableSettings->items->showCommonItems = items_cbCommonItems->isChecked();
		tableSettings->items->showWeapons = items_cbWeapons->isChecked();
		tableSettings->items->showShields = items_cbShields->isChecked();
		tableSettings->items->showArmor = items_cbArmor->isChecked();
		tableSettings->items->showHelmets = items_cbHelmets->isChecked();
		tableSettings->items->showAccessories = items_cbAccessories->isChecked();
		tableSettings->items->showMedicine = items_cbMedicine->isChecked();
		tableSettings->items->showSkillBooks = items_cbSkillBooks->isChecked();
		tableSettings->items->showSeeds = items_cbSeeds->isChecked();
		tableSettings->items->showSkillScrolls = items_cbSkillScrolls->isChecked();
		tableSettings->items->showSwitches = items_cbSwitches->isChecked();
		tableSettings->items->showEvents = items_cbEvents->isChecked();
	}
	else if (rbSkills->isChecked())
	{
		tableSettings->skills = new MenuItemTable::SkillSettings;
		if (skills_rbPartyMember1->isChecked())
			tableSettings->skills->memberID = 0;
		else if (skills_rbPartyMember2->isChecked())
			tableSettings->skills->memberID = 1;
		else if (skills_rbPartyMember3->isChecked())
			tableSettings->skills->memberID = 2;
		else if (skills_rbPartyMember4->isChecked())
			tableSettings->skills->memberID = 3;
		else if (skills_rbPartyMemberVar->isChecked())
			tableSettings->skills->memberID = ProjectData::numberVariableIdFromString(skills_lblPartyMember_Variable->text()) + 4;
		for (int i = 0; i < lwSkillTypes->count(); ++i)
			tableSettings->skills->skillTypesToShow.append((lwSkillTypes->item(i)->checkState() == Qt::Checked) ? true:false);
	}
	else if (rbAvailableEquipment->isChecked())
	{
		tableSettings->equipment = new MenuItemTable::EquipmentSettings;
		if (equip_rbPartyMember1->isChecked())
			tableSettings->equipment->memberID = 0;
		else if (equip_rbPartyMember2->isChecked())
			tableSettings->equipment->memberID = 1;
		else if (equip_rbPartyMember3->isChecked())
			tableSettings->equipment->memberID = 2;
		else if (equip_rbPartyMember4->isChecked())
			tableSettings->equipment->memberID = 3;
		else if (equip_rbPartyMemberVar->isChecked())
			tableSettings->equipment->memberID = ProjectData::numberVariableIdFromString(equip_lblPartyMember_Variable->text()) + 4;
		if (equip_rbWeapons->isChecked())
			tableSettings->equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_WEAPONS;
		else if (equip_rbShields->isChecked())
			tableSettings->equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_SHIELDS;
		else if (equip_rbArmor->isChecked())
			tableSettings->equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_ARMOR;
		else if (equip_rbHelmets->isChecked())
			tableSettings->equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_HELMETS;
		else if (equip_rbAccessories->isChecked())
			tableSettings->equipment->equipmentType = MenuItemTable::EquipmentSettings::TYPE_ACCESSORIES;
	}
	else
	{
		tableSettings->other = new MenuItemTable::OtherSettings;
		tableSettings->other->useVariablesToSetState = cbUseVariablesToSetState->isChecked();
		if (tableSettings->other->useVariablesToSetState)
			tableSettings->other->startVariableID = ProjectData::numberVariableIdFromString(other_lblStartVariable->text());
		else
			tableSettings->other->startVariableID = 0;
	}
}

void ConfigureEvent_ChangeMenuItemList::on_cbUseVariablesToSetState_toggled(bool checked)
{
	wUseVariablesToSetState->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuItemList::on_rbAvailableEquipment_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wAvailableEquipment);
}

void ConfigureEvent_ChangeMenuItemList::on_rbCharacters_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wOther);
}

void ConfigureEvent_ChangeMenuItemList::on_rbClasses_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wOther);
}

void ConfigureEvent_ChangeMenuItemList::on_rbMonsters_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wOther);
}

void ConfigureEvent_ChangeMenuItemList::on_rbPartysItems_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wItems);
}

void ConfigureEvent_ChangeMenuItemList::on_rbSkills_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wSkills);
}

void ConfigureEvent_ChangeMenuItemList::on_rbVehicles_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wOther);
}

void ConfigureEvent_ChangeMenuItemList::on_skills_rbPartyMemberVar_toggled(bool checked)
{
	skills_wPartyMember_Variable->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuItemList::on_equip_rbPartyMemberVar_toggled(bool checked)
{
	equip_wPartyMember_Variable->setEnabled(checked);
}

void ConfigureEvent_ChangeMenuItemList::on_skills_bPartyMember_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(skills_lblPartyMember_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID - 1);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		skills_lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuItemList::on_equip_bPartyMember_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(equip_lblPartyMember_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID - 1);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		equip_lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuItemList::on_other_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(other_lblStartVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID - 1);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		other_lblStartVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuItemList::setupSkillTypeList()
{
	QListWidgetItem *item;
	item = new QListWidgetItem("Normal");
	item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
	item->setCheckState(Qt::Checked);
	lwSkillTypes->addItem(item);
	item = new QListWidgetItem("Teleport");
	item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
	item->setCheckState(Qt::Checked);
	lwSkillTypes->addItem(item);
	item = new QListWidgetItem("Escape");
	item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
	item->setCheckState(Qt::Checked);
	lwSkillTypes->addItem(item);
	item = new QListWidgetItem("Switch");
	item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
	item->setCheckState(Qt::Checked);
	lwSkillTypes->addItem(item);
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (ProjectData::battleCommands[i]->type == ProjectData::COMMAND_SKILLSUBSET)
		{
			item = new QListWidgetItem(ProjectData::battleCommands[i]->name);
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
			item->setCheckState(Qt::Checked);
			lwSkillTypes->addItem(item);
		}
	}
}
