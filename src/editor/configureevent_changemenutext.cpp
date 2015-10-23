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

#include "configureevent_changemenutext.h"
#include "menuobject.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"
#include "storagefile.h"
#include "variableselector.h"

ConfigureEvent_ChangeMenuText::ConfigureEvent_ChangeMenuText(int saveSlotID, int textType, MenuText::TextData *data, int menuID, int menuType, QWidget *parent) : QDialog(parent)
{
	QList<MenuObject*> menuObjects;
	QString varText = ProjectData::numberVariableIdToString(0);
	QString switchText = ProjectData::switchVariableIdToString(0);
	QString stringText = ProjectData::stringVariableIdToString(0);
	textData = data;
	type = textType;
	setupUi(this);
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	if (menuType == MenuObject::MENUTYPE_MAINMENU)
	{
		ProjectData::DataReference<MainMenuLayout> *layoutRef = new ProjectData::DataReference<MainMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_TEXT);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuText->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_POPUPMENU)
	{
		ProjectData::DataReference<PopupMenuLayout> *layoutRef = new ProjectData::DataReference<PopupMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_TEXT);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuText->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_TITLESCREENMENU)
	{
		TitleMenuLayout *layout = ProjectData::titleMenuLayout;
		menuObjects = layout->getObjectsByType(MenuObject::TYPE_TEXT);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuText->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		layout = NULL;
	}
	if (cboxMenuText->count() == 0)
	{
		cboxMenuText->addItem("No Images To Configure");
		frmSaveSlot->setEnabled(false);
		wConfigureText->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	cbUseValueInSaveSlot->setChecked((saveSlotID != 0));
	if (saveSlotID > 0)
		sbSaveSlot->setValue(saveSlotID);
	bSaveSlotSideButton->setSpinBox(sbSaveSlot);
	cboxCharacter->addItems(ProjectData::characterNames);
	cboxItem->addItems(ProjectData::itemNames);
	cboxMonster->addItems(ProjectData::monsterNames);
	lblPartyMember_Variable->setText(varText);
	lblMonsterVariable->setText(varText);
	lblItemVariable->setText(varText);
	lblCharacterVariable->setText(varText);
	lblVariable->setText(varText);
	lblSwitch->setText(switchText);
	lblStringVariable->setText(stringText);
	wPartyMember_Variable->setEnabled(false);
	frmPartyMemberEquipment->setEnabled(false);
	wMonster_Variable->setEnabled(false);
	wItem_Variable->setEnabled(false);
	wCharacter_Variable->setEnabled(false);
	frmCharacterEquipment->setEnabled(false);
	wStringVariable->setEnabled(false);
	if (textType == MenuText::TYPE_PLAINTEXT)
	{
		rbPlainText->setChecked(true);
		lePlainText->setText(*(textData->plainText));
		lePlainText->setFocus();
	}
	else if (textType == MenuText::TYPE_PARTYMEMBER)
	{
		rbPartyMember->setChecked(true);
		if (textData->partyMemberText->memberID == 0)
			rbPartyMember1->setChecked(true);
		else if (textData->partyMemberText->memberID == 1)
			rbPartyMember2->setChecked(true);
		else if (textData->partyMemberText->memberID == 2)
			rbPartyMember3->setChecked(true);
		else if (textData->partyMemberText->memberID == 3)
			rbPartyMember4->setChecked(true);
		else
		{
			int varID = textData->partyMemberText->memberID - 4;
			rbPartyMemberVar->setChecked(true);
			lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
		}
		if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_NAME)
			rbPartyMemberName->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_CLASS)
			rbPartyMemberClass->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_CONDITION)
			rbPartyMemberCondition->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_LEVEL)
			rbPartyMemberLevel->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_HP)
			rbPartyMemberHP->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_MAXHP)
			rbPartyMemberMaxHP->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_MP)
			rbPartyMemberMP->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_MAXMP)
			rbPartyMemberMaxMP->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_EXPERIENCE)
			rbPartyMemberTotalExperience->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_EXPERIENCETONEXTLEVEL)
			rbPartyMemberExperienceToNextLevel->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_BASEATTACK)
			rbPartyMemberBaseAttack->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_TOTALATTACK)
			rbPartyMemberTotalAttack->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_BASEDEFENSE)
			rbPartyMemberBaseDefense->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_TOTALDEFENSE)
			rbPartyMemberTotalDefense->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_BASEWISDOM)
			rbPartyMemberBaseWisdom->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_TOTALWISDOM)
			rbPartyMemberTotalWisdom->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_BASEAGILITY)
			rbPartyMemberBaseAgility->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_TOTALAGILITY)
			rbPartyMemberTotalAgility->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDWEAPON)
			rbPartyMemberEquippedWeapon->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDSHIELD)
			rbPartyMemberEquippedShield->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDARMOR)
			rbPartyMemberEquippedArmor->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDHELMET)
			rbPartyMemberEquippedHelmet->setChecked(true);
		else if (textData->partyMemberText->textType == MenuText::PARTYMEMBER_EQUIPPEDACCESSORY)
			rbPartyMemberEquippedAccessory->setChecked(true);
		if (textData->partyMemberText->textType >= MenuText::PARTYMEMBER_EQUIPPEDWEAPON && textData->partyMemberText->textType <= MenuText::PARTYMEMBER_EQUIPPEDACCESSORY)
		{
			if (textData->partyMemberText->equipmentTextType == MenuText::EQUIPMENT_NAME)
				rbPartyMemberEquipmentName->setChecked(true);
			else if (textData->partyMemberText->equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
				rbPartyMemberEquipmentAttack->setChecked(true);
			else if (textData->partyMemberText->equipmentTextType == MenuText::EQUIPMENT_ATTACK)
				rbPartyMemberEquipmentWisdom->setChecked(true);
			else if (textData->partyMemberText->equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
				rbPartyMemberEquipmentDescription->setChecked(true);
			else if (textData->partyMemberText->equipmentTextType == MenuText::EQUIPMENT_WISDOM)
				rbPartyMemberEquipmentDefense->setChecked(true);
			else if (textData->partyMemberText->equipmentTextType == MenuText::EQUIPMENT_AGILITY)
				rbPartyMemberEquipmentAgility->setChecked(true);
		}
	}
	else if (textType == MenuText::TYPE_MONSTER)
	{
		rbMonster->setChecked(true);
		if (textData->monsterText->idIsReferencedInVariable)
		{
			int varID = textData->monsterText->monsterID;
			rbMonsterVariable->setChecked(true);
			lblMonsterVariable->setText(ProjectData::numberVariableIdToString(varID));
		}
		else
		{
			rbMonsterSpecific->setChecked(true);
			cboxMonster->setCurrentIndex(textData->monsterText->monsterID);
		}
		if (textData->monsterText->textType == MenuText::MONSTER_NAME)
			rbMonsterName->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_MONEY)
			rbMonsterMoney->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_HP)
			rbMonsterHP->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_MP)
			rbMonsterMP->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_ATTACK)
			rbMonsterAttack->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_DEFENSE)
			rbMonsterDefense->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_WISDOM)
			rbMonsterWisdom->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_AGILITY)
			rbMonsterAgility->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_STRONGAGAINST)
			rbMonsterStrongAgainst->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_WEAKAGAINST)
			rbMonsterWeakAgainst->setChecked(true);
		else if (textData->monsterText->textType == MenuText::MONSTER_EXPERIENCE)
			rbMonsterExperience->setChecked(true);
	}
	else if (textType == MenuText::TYPE_ITEM)
	{
		rbItem->setChecked(true);
		if (textData->itemText->idIsReferencedInVariable)
		{
			int varID = textData->itemText->itemID;
			rbItemVariable->setChecked(true);
			lblItemVariable->setText(ProjectData::numberVariableIdToString(varID));
		}
		else
		{
			rbItemSpecific->setChecked(true);
			cboxItem->setCurrentIndex(textData->itemText->itemID);
		}
		if (textData->itemText->textType == MenuText::ITEM_NAME)
			rbItemName->setChecked(true);
		else if (textData->itemText->textType == MenuText::ITEM_DESCRIPTION)
			rbItemDescription->setChecked(true);
		else if (textData->itemText->textType == MenuText::ITEM_PRICE)
			rbItemPrice->setChecked(true);
	}
	else if (textType == MenuText::TYPE_CHARACTER)
	{
		rbCharacter->setChecked(true);
		if (textData->characterText->idIsReferencedInVariable)
		{
			int varID = textData->characterText->characterID;
			rbCharacterVariable->setChecked(true);
			lblCharacterVariable->setText(ProjectData::numberVariableIdToString(varID));
		}
		else
		{
			rbCharacterSpecific->setChecked(true);
			cboxCharacter->setCurrentIndex(textData->characterText->characterID);
		}
		if (textData->characterText->textType == MenuText::CHARACTER_NAME)
			rbCharacterName->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_CLASS)
			rbCharacterClass->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_CONDITION)
			rbCharacterCondition->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_LEVEL)
			rbCharacterLevel->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_HP)
			rbCharacterHP->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_MAXHP)
			rbCharacterMaxHP->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_MP)
			rbCharacterMP->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_MAXMP)
			rbCharacterMaxMP->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_EXPERIENCE)
			rbCharacterTotalExperience->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_EXPERIENCETONEXTLEVEL)
			rbCharacterExperienceToNextLevel->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_BASEATTACK)
			rbCharacterBaseAttack->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_TOTALATTACK)
			rbCharacterTotalAttack->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_BASEDEFENSE)
			rbCharacterBaseDefense->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_TOTALDEFENSE)
			rbCharacterTotalDefense->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_BASEWISDOM)
			rbCharacterBaseWisdom->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_TOTALWISDOM)
			rbCharacterTotalWisdom->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_BASEAGILITY)
			rbCharacterBaseAgility->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_TOTALAGILITY)
			rbCharacterTotalAgility->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_EQUIPPEDWEAPON)
			rbCharacterEquippedWeapon->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_EQUIPPEDSHIELD)
			rbCharacterEquippedShield->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_EQUIPPEDARMOR)
			rbCharacterEquippedArmor->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_EQUIPPEDHELMET)
			rbCharacterEquippedHelmet->setChecked(true);
		else if (textData->characterText->textType == MenuText::CHARACTER_EQUIPPEDACCESSORY)
			rbCharacterEquippedAccessory->setChecked(true);
		if (textData->characterText->textType >= MenuText::CHARACTER_EQUIPPEDWEAPON && textData->characterText->textType <= MenuText::CHARACTER_EQUIPPEDACCESSORY)
		{
			if (textData->characterText->equipmentTextType == MenuText::EQUIPMENT_NAME)
				rbCharacterEquipmentName->setChecked(true);
			else if (textData->characterText->equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
				rbCharacterEquipmentAttack->setChecked(true);
			else if (textData->characterText->equipmentTextType == MenuText::EQUIPMENT_ATTACK)
				rbCharacterEquipmentWisdom->setChecked(true);
			else if (textData->characterText->equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
				rbCharacterEquipmentDescription->setChecked(true);
			else if (textData->characterText->equipmentTextType == MenuText::EQUIPMENT_WISDOM)
				rbCharacterEquipmentDefense->setChecked(true);
			else if (textData->characterText->equipmentTextType == MenuText::EQUIPMENT_AGILITY)
				rbCharacterEquipmentAgility->setChecked(true);
		}
	}
	else if (textType == MenuText::TYPE_VARIABLE)
	{
		rbVariable->setChecked(true);
		lblVariable->setText(ProjectData::numberVariableIdToString(textData->variableText->variableID));
		cbAlwaysShowPlusSign->setChecked(textData->variableText->alwaysShowPlusSign);
	}
	else if (textType == MenuText::TYPE_SWITCH)
	{
		rbSwitch->setChecked(true);
		lblSwitch->setText(ProjectData::switchVariableIdToString(textData->switchText->switchID));
		leSwitchOnText->setText(textData->switchText->onText);
		leSwitchOffText->setText(textData->switchText->offText);
	}
	else if (textType == MenuText::TYPE_STRING)
	{
		rbString->setChecked(true);
		if (textData->stringText->systemString)
		{
			rbStringSystem->setChecked(true);
			cboxStrings->setCurrentIndex(textData->stringText->stringID);
		}
		else
		{
			rbStringVariable->setChecked(true);
			lblStringVariable->setText(ProjectData::stringVariableIdToString(textData->stringText->stringID));
		}
	}
}

ConfigureEvent_ChangeMenuText::~ConfigureEvent_ChangeMenuText()
{
}

void ConfigureEvent_ChangeMenuText::setTextObject(QString object)
{
	int index = cboxMenuText->findText(object);
	if (index != -1)
		cboxMenuText->setCurrentIndex(index);
}

int ConfigureEvent_ChangeMenuText::getTextType()
{
	if (rbPlainText->isChecked())
		return MenuText::TYPE_PLAINTEXT;
	else if (rbPartyMember->isChecked())
		return MenuText::TYPE_PARTYMEMBER;
	else if (rbCharacter->isChecked())
		return MenuText::TYPE_CHARACTER;
	else if (rbMonster->isChecked())
		return MenuText::TYPE_MONSTER;
	else if (rbItem->isChecked())
		return MenuText::TYPE_ITEM;
	else if (rbString->isChecked())
		return MenuText::TYPE_STRING;
	else if (rbVariable->isChecked())
		return MenuText::TYPE_VARIABLE;
	return MenuText::TYPE_SWITCH;
}

QString ConfigureEvent_ChangeMenuText::getTextObject()
{
	return cboxMenuText->currentText();
}

int ConfigureEvent_ChangeMenuText::getSaveSlotID()
{
	if (cbUseValueInSaveSlot->isChecked())
		return sbSaveSlot->value();
	return 0;
}

void ConfigureEvent_ChangeMenuText::on_bBrowseSwitches_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lblSwitch->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		switchID = variableSelector->getVariableID();
		lblSwitch->setText(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuText::on_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuText::on_bCharacterBrowseVariables_clicked()
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

void ConfigureEvent_ChangeMenuText::on_bItemBrowseVariables_clicked()
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

void ConfigureEvent_ChangeMenuText::on_bMonsterBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblMonsterVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblMonsterVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuText::on_bPartyMember_BrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblPartyMember_Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuText::on_bStringBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int stringID = ProjectData::stringVariableIdFromString(lblStringVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_STRINGS, stringID);
	if (variableSelector->exec())
	{
		stringID = variableSelector->getVariableID();
		lblStringVariable->setText(ProjectData::stringVariableIdToString(stringID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeMenuText::on_rbPlainText_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wPlainText);
}

void ConfigureEvent_ChangeMenuText::on_rbPartyMember_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wPartyMember);
}

void ConfigureEvent_ChangeMenuText::on_rbCharacter_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wCharacter);
}

void ConfigureEvent_ChangeMenuText::on_rbMonster_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wMonster);
}

void ConfigureEvent_ChangeMenuText::on_rbItem_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wItem);
}

void ConfigureEvent_ChangeMenuText::on_rbString_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wString);
}

void ConfigureEvent_ChangeMenuText::on_rbVariable_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wVariable);
}

void ConfigureEvent_ChangeMenuText::on_rbSwitch_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wSwitch);
}

void ConfigureEvent_ChangeMenuText::on_rbCharacterEquippedAccessory_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbCharacterEquippedWeapon->isChecked())
		enable = true;
	else if (rbCharacterEquippedShield->isChecked())
		enable = true;
	else if (rbCharacterEquippedArmor->isChecked())
		enable = true;
	else if (rbCharacterEquippedHelmet->isChecked())
		enable = true;
	frmCharacterEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbCharacterEquippedArmor_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbCharacterEquippedWeapon->isChecked())
		enable = true;
	else if (rbCharacterEquippedShield->isChecked())
		enable = true;
	else if (rbCharacterEquippedHelmet->isChecked())
		enable = true;
	else if (rbCharacterEquippedAccessory->isChecked())
		enable = true;
	frmCharacterEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbCharacterEquippedHelmet_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbCharacterEquippedWeapon->isChecked())
		enable = true;
	else if (rbCharacterEquippedShield->isChecked())
		enable = true;
	else if (rbCharacterEquippedArmor->isChecked())
		enable = true;
	else if (rbCharacterEquippedAccessory->isChecked())
		enable = true;
	frmCharacterEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbCharacterEquippedShield_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbCharacterEquippedWeapon->isChecked())
		enable = true;
	else if (rbCharacterEquippedArmor->isChecked())
		enable = true;
	else if (rbCharacterEquippedHelmet->isChecked())
		enable = true;
	else if (rbCharacterEquippedAccessory->isChecked())
		enable = true;
	frmCharacterEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbCharacterEquippedWeapon_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbCharacterEquippedShield->isChecked())
		enable = true;
	else if (rbCharacterEquippedArmor->isChecked())
		enable = true;
	else if (rbCharacterEquippedHelmet->isChecked())
		enable = true;
	else if (rbCharacterEquippedAccessory->isChecked())
		enable = true;
	frmCharacterEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbCharacterSpecific_toggled(bool on)
{
	cboxCharacter->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_rbCharacterVariable_toggled(bool on)
{
	wCharacter_Variable->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_rbItemSpecific_toggled(bool on)
{
	cboxItem->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_rbItemVariable_toggled(bool on)
{
	wItem_Variable->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_rbMonsterSpecific_toggled(bool on)
{
	cboxMonster->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_rbMonsterVariable_toggled(bool on)
{
	wMonster_Variable->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_rbPartyMemberEquippedArmor_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbPartyMemberEquippedWeapon->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedShield->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedHelmet->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedAccessory->isChecked())
		enable = true;
	frmPartyMemberEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbPartyMemberEquippedHelmet_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbPartyMemberEquippedWeapon->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedShield->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedArmor->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedAccessory->isChecked())
		enable = true;
	frmPartyMemberEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbPartyMemberEquippedShield_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbPartyMemberEquippedWeapon->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedArmor->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedHelmet->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedAccessory->isChecked())
		enable = true;
	frmPartyMemberEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbPartyMemberEquippedWeapon_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbPartyMemberEquippedShield->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedArmor->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedHelmet->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedAccessory->isChecked())
		enable = true;
	frmPartyMemberEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbPartyMemberEquippedAccessory_toggled(bool on)
{
	bool enable = false;
	if (on)
		enable = true;
	else if (rbPartyMemberEquippedWeapon->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedShield->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedArmor->isChecked())
		enable = true;
	else if (rbPartyMemberEquippedHelmet->isChecked())
		enable = true;
	frmPartyMemberEquipment->setEnabled(enable);
}

void ConfigureEvent_ChangeMenuText::on_rbPartyMemberVar_toggled(bool on)
{
	wPartyMember_Variable->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_rbStringSystem_toggled(bool on)
{
	cboxStrings->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_rbStringVariable_toggled(bool on)
{
	wStringVariable->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_cbUseValueInSaveSlot_toggled(bool on)
{
	wSaveSlot->setEnabled(on);
}

void ConfigureEvent_ChangeMenuText::on_buttonBox_accepted()
{
	if (type == MenuText::TYPE_PLAINTEXT)
		delete (textData->plainText);
	else if (type == MenuText::TYPE_PARTYMEMBER)
		delete (textData->partyMemberText);
	else if (type == MenuText::TYPE_MONSTER)
		delete (textData->monsterText);
	else if (type == MenuText::TYPE_ITEM)
		delete (textData->itemText);
	else if (type == MenuText::TYPE_CHARACTER)
		delete (textData->characterText);
	else if (type == MenuText::TYPE_VARIABLE)
		delete (textData->variableText);
	else if (type == MenuText::TYPE_SWITCH)
		delete (textData->switchText);
	else if (type == MenuText::TYPE_STRING)
		delete (textData->stringText);
	if (rbPlainText->isChecked())
		textData->plainText = new QString(lePlainText->text());
	else if (rbPartyMember->isChecked())
	{
		textData->partyMemberText = new MenuText::PartyMemberText;
		if (rbPartyMember1->isChecked())
			textData->partyMemberText->memberID = 0;
		else if (rbPartyMember2->isChecked())
			textData->partyMemberText->memberID = 1;
		else if (rbPartyMember3->isChecked())
			textData->partyMemberText->memberID = 2;
		else if (rbPartyMember4->isChecked())
			textData->partyMemberText->memberID = 3;
		else
			textData->partyMemberText->memberID = ProjectData::numberVariableIdFromString(lblPartyMember_Variable->text()) + 4;
		if (rbPartyMemberName->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_NAME;
		else if (rbPartyMemberClass->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_CLASS;
		else if (rbPartyMemberCondition->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_CONDITION;
		else if (rbPartyMemberLevel->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_LEVEL;
		else if (rbPartyMemberHP->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_HP;
		else if (rbPartyMemberMaxHP->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_MAXHP;
		else if (rbPartyMemberMP->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_MP;
		else if (rbPartyMemberMaxMP->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_MAXMP;
		else if (rbPartyMemberTotalExperience->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_EXPERIENCE;
		else if (rbPartyMemberExperienceToNextLevel->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_EXPERIENCETONEXTLEVEL;
		else if (rbPartyMemberBaseAttack->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_BASEATTACK;
		else if (rbPartyMemberTotalAttack->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_TOTALATTACK;
		else if (rbPartyMemberBaseDefense->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_BASEDEFENSE;
		else if (rbPartyMemberTotalDefense->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_TOTALDEFENSE;
		else if (rbPartyMemberBaseWisdom->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_BASEWISDOM;
		else if (rbPartyMemberTotalWisdom->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_TOTALWISDOM;
		else if (rbPartyMemberBaseAgility->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_BASEAGILITY;
		else if (rbPartyMemberTotalAgility->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_TOTALAGILITY;
		else if (rbPartyMemberEquippedWeapon->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDWEAPON;
		else if (rbPartyMemberEquippedShield->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDSHIELD;
		else if (rbPartyMemberEquippedArmor->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDARMOR;
		else if (rbPartyMemberEquippedHelmet->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDHELMET;
		else if (rbPartyMemberEquippedAccessory->isChecked())
			textData->partyMemberText->textType = MenuText::PARTYMEMBER_EQUIPPEDACCESSORY;
		if (rbPartyMemberEquipmentName->isChecked())
			textData->partyMemberText->equipmentTextType = MenuText::EQUIPMENT_NAME;
		else if (rbPartyMemberEquipmentDescription->isChecked())
			textData->partyMemberText->equipmentTextType = MenuText::EQUIPMENT_DESCRIPTION;
		else if (rbPartyMemberEquipmentAttack->isChecked())
			textData->partyMemberText->equipmentTextType = MenuText::EQUIPMENT_ATTACK;
		else if (rbPartyMemberEquipmentDefense->isChecked())
			textData->partyMemberText->equipmentTextType = MenuText::EQUIPMENT_DEFENSE;
		else if (rbPartyMemberEquipmentWisdom->isChecked())
			textData->partyMemberText->equipmentTextType = MenuText::EQUIPMENT_WISDOM;
		else if (rbPartyMemberEquipmentAgility->isChecked())
			textData->partyMemberText->equipmentTextType = MenuText::EQUIPMENT_AGILITY;
	}
	else if (rbMonster->isChecked())
	{
		textData->monsterText = new MenuText::MonsterText;
		textData->monsterText->idIsReferencedInVariable = rbMonsterVariable->isChecked();
		if (textData->monsterText->idIsReferencedInVariable)
			textData->monsterText->monsterID = ProjectData::numberVariableIdFromString(lblMonsterVariable->text());
		else
			textData->monsterText->monsterID = cboxMonster->currentIndex();
		if (rbMonsterName->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_NAME;
		else if (rbMonsterMoney->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_MONEY;
		else if (rbMonsterHP->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_HP;
		else if (rbMonsterMP->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_MP;
		else if (rbMonsterAttack->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_ATTACK;
		else if (rbMonsterDefense->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_DEFENSE;
		else if (rbMonsterWisdom->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_WISDOM;
		else if (rbMonsterAgility->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_AGILITY;
		else if (rbMonsterStrongAgainst->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_STRONGAGAINST;
		else if (rbMonsterWeakAgainst->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_WEAKAGAINST;
		else if (rbMonsterExperience->isChecked())
			textData->monsterText->textType = MenuText::MONSTER_EXPERIENCE;
	}
	else if (rbItem->isChecked())
	{
		textData->itemText = new MenuText::ItemText;
		textData->itemText->idIsReferencedInVariable = rbItemVariable->isChecked();
		if (textData->itemText->idIsReferencedInVariable)
			textData->itemText->itemID = ProjectData::numberVariableIdFromString(lblItemVariable->text());
		else
			textData->itemText->itemID = cboxItem->currentIndex();
		if (rbItemName->isChecked())
			textData->itemText->textType = MenuText::ITEM_NAME;
		else if (rbItemDescription->isChecked())
			textData->itemText->textType = MenuText::ITEM_DESCRIPTION;
		else if (rbItemPrice->isChecked())
			textData->itemText->textType = MenuText::ITEM_PRICE;
	}
	else if (rbCharacter->isChecked())
	{
		textData->characterText = new MenuText::CharacterText;
		textData->characterText->idIsReferencedInVariable = rbCharacterVariable->isChecked();
		if (textData->characterText->idIsReferencedInVariable)
			textData->characterText->characterID = ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
		else
			textData->characterText->characterID = cboxCharacter->currentIndex();
		if (rbCharacterName->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_NAME;
		else if (rbCharacterClass->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_CLASS;
		else if (rbCharacterCondition->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_CONDITION;
		else if (rbCharacterLevel->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_LEVEL;
		else if (rbCharacterHP->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_HP;
		else if (rbCharacterMaxHP->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_MAXHP;
		else if (rbCharacterMP->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_MP;
		else if (rbCharacterMaxMP->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_MAXMP;
		else if (rbCharacterTotalExperience->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_EXPERIENCE;
		else if (rbCharacterExperienceToNextLevel->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_EXPERIENCETONEXTLEVEL;
		else if (rbCharacterBaseAttack->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_BASEATTACK;
		else if (rbCharacterTotalAttack->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_TOTALATTACK;
		else if (rbCharacterBaseDefense->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_BASEDEFENSE;
		else if (rbCharacterTotalDefense->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_TOTALDEFENSE;
		else if (rbCharacterBaseWisdom->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_BASEWISDOM;
		else if (rbCharacterTotalWisdom->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_TOTALWISDOM;
		else if (rbCharacterBaseAgility->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_BASEAGILITY;
		else if (rbCharacterTotalAgility->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_TOTALAGILITY;
		else if (rbCharacterEquippedWeapon->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_EQUIPPEDWEAPON;
		else if (rbCharacterEquippedShield->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_EQUIPPEDSHIELD;
		else if (rbCharacterEquippedArmor->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_EQUIPPEDARMOR;
		else if (rbCharacterEquippedHelmet->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_EQUIPPEDHELMET;
		else if (rbCharacterEquippedAccessory->isChecked())
			textData->characterText->textType = MenuText::CHARACTER_EQUIPPEDACCESSORY;
		if (rbCharacterEquipmentName->isChecked())
			textData->characterText->equipmentTextType = MenuText::EQUIPMENT_NAME;
		else if (rbCharacterEquipmentDescription->isChecked())
			textData->characterText->equipmentTextType = MenuText::EQUIPMENT_DESCRIPTION;
		else if (rbCharacterEquipmentAttack->isChecked())
			textData->characterText->equipmentTextType = MenuText::EQUIPMENT_ATTACK;
		else if (rbCharacterEquipmentDefense->isChecked())
			textData->characterText->equipmentTextType = MenuText::EQUIPMENT_DEFENSE;
		else if (rbCharacterEquipmentWisdom->isChecked())
			textData->characterText->equipmentTextType = MenuText::EQUIPMENT_WISDOM;
		else if (rbCharacterEquipmentAgility->isChecked())
			textData->characterText->equipmentTextType = MenuText::EQUIPMENT_AGILITY;
	}
	else if (rbVariable->isChecked())
	{
		textData->variableText = new MenuText::VariableText;
		textData->variableText->variableID = ProjectData::numberVariableIdFromString(lblVariable->text());
		textData->variableText->alwaysShowPlusSign = cbAlwaysShowPlusSign->isChecked();
	}
	else if (rbSwitch->isChecked())
	{
		textData->switchText = new MenuText::SwitchText;
		textData->switchText->switchID = ProjectData::switchVariableIdFromString(lblSwitch->text());
		textData->switchText->onText = leSwitchOnText->text();
		textData->switchText->offText = leSwitchOffText->text();
	}
	else if (rbString->isChecked())
	{
		textData->stringText = new MenuText::StringText;
		textData->stringText->systemString = rbStringSystem->isChecked();
		if (textData->stringText->systemString)
			textData->stringText->stringID = cboxStrings->currentIndex();
		else
			textData->stringText->stringID = ProjectData::stringVariableIdFromString(lblStringVariable->text());
	}
}
