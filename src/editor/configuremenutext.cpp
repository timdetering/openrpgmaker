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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configuremenutext.h"
#include "menutext.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureMenuText::ConfigureMenuText(MenuText *textToConfigure, QWidget *parent, bool enableSelectSaveSlot) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	QString switchText = ProjectData::switchVariableIdToString(0);
	QString stringText = ProjectData::stringVariableIdToString(0);
	int textType;
	int saveSlotID = textToConfigure->getSaveSlotID();
	text = textToConfigure;
	setupUi(this);
	bSaveSlotSideButton->setSpinBox(sbSaveSlot);
	cboxCharacter->addItems(ProjectData::characterNames);
	cboxItem->addItems(ProjectData::itemNames);
	cboxMonster->addItems(ProjectData::monsterNames);
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
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
	cbUseValueInSaveSlot->setChecked((saveSlotID != 0));
	if (saveSlotID > 0)
		sbSaveSlot->setValue(saveSlotID);
	frmSaveSlot->setVisible(enableSelectSaveSlot);
	textType = text->getTextType();
	if (textType == MenuText::TYPE_PLAINTEXT)
	{
		QString plainText = "";
		text->getText(&plainText);
		rbPlainText->setChecked(true);
		lePlainText->setText(plainText);
		lePlainText->setFocus();
	}
	else if (textType == MenuText::TYPE_PARTYMEMBER)
	{
		MenuText::PartyMemberText partyMemberText;
		text->getText(&partyMemberText);
		rbPartyMember->setChecked(true);
		if (partyMemberText.memberID == 0)
			rbPartyMember1->setChecked(true);
		else if (partyMemberText.memberID == 1)
			rbPartyMember2->setChecked(true);
		else if (partyMemberText.memberID == 2)
			rbPartyMember3->setChecked(true);
		else if (partyMemberText.memberID == 3)
			rbPartyMember4->setChecked(true);
		else
		{
			int varID = partyMemberText.memberID - 4;
			rbPartyMemberVar->setChecked(true);
			lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(varID));
		}
		if (partyMemberText.textType == MenuText::PARTYMEMBER_NAME)
			rbPartyMemberName->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_CLASS)
			rbPartyMemberClass->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_CONDITION)
			rbPartyMemberCondition->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_LEVEL)
			rbPartyMemberLevel->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_HP)
			rbPartyMemberHP->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_MAXHP)
			rbPartyMemberMaxHP->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_MP)
			rbPartyMemberMP->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_MAXMP)
			rbPartyMemberMaxMP->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_EXPERIENCE)
			rbPartyMemberTotalExperience->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_EXPERIENCETONEXTLEVEL)
			rbPartyMemberExperienceToNextLevel->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_BASEATTACK)
			rbPartyMemberBaseAttack->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_TOTALATTACK)
			rbPartyMemberTotalAttack->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_BASEDEFENSE)
			rbPartyMemberBaseDefense->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_TOTALDEFENSE)
			rbPartyMemberTotalDefense->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_BASEWISDOM)
			rbPartyMemberBaseWisdom->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_TOTALWISDOM)
			rbPartyMemberTotalWisdom->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_BASEAGILITY)
			rbPartyMemberBaseAgility->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_TOTALAGILITY)
			rbPartyMemberTotalAgility->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_EQUIPPEDWEAPON)
			rbPartyMemberEquippedWeapon->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_EQUIPPEDSHIELD)
			rbPartyMemberEquippedShield->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_EQUIPPEDARMOR)
			rbPartyMemberEquippedArmor->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_EQUIPPEDHELMET)
			rbPartyMemberEquippedHelmet->setChecked(true);
		else if (partyMemberText.textType == MenuText::PARTYMEMBER_EQUIPPEDACCESSORY)
			rbPartyMemberEquippedAccessory->setChecked(true);
		if (partyMemberText.textType >= MenuText::PARTYMEMBER_EQUIPPEDWEAPON && partyMemberText.textType <= MenuText::PARTYMEMBER_EQUIPPEDACCESSORY)
		{
			if (partyMemberText.equipmentTextType == MenuText::EQUIPMENT_NAME)
				rbPartyMemberEquipmentName->setChecked(true);
			else if (partyMemberText.equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
				rbPartyMemberEquipmentAttack->setChecked(true);
			else if (partyMemberText.equipmentTextType == MenuText::EQUIPMENT_ATTACK)
				rbPartyMemberEquipmentWisdom->setChecked(true);
			else if (partyMemberText.equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
				rbPartyMemberEquipmentDescription->setChecked(true);
			else if (partyMemberText.equipmentTextType == MenuText::EQUIPMENT_WISDOM)
				rbPartyMemberEquipmentDefense->setChecked(true);
			else if (partyMemberText.equipmentTextType == MenuText::EQUIPMENT_AGILITY)
				rbPartyMemberEquipmentAgility->setChecked(true);
		}
	}
	else if (textType == MenuText::TYPE_MONSTER)
	{
		MenuText::MonsterText monsterText;
		text->getText(&monsterText);
		rbMonster->setChecked(true);
		if (monsterText.idIsReferencedInVariable)
		{
			int varID = monsterText.monsterID;
			rbMonsterVariable->setChecked(true);
			lblMonsterVariable->setText(ProjectData::numberVariableIdToString(varID));
		}
		else
		{
			rbMonsterSpecific->setChecked(true);
			cboxMonster->setCurrentIndex(monsterText.monsterID);
		}
		if (monsterText.textType == MenuText::MONSTER_NAME)
			rbMonsterName->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_MONEY)
			rbMonsterMoney->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_HP)
			rbMonsterHP->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_MP)
			rbMonsterMP->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_ATTACK)
			rbMonsterAttack->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_DEFENSE)
			rbMonsterDefense->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_WISDOM)
			rbMonsterWisdom->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_AGILITY)
			rbMonsterAgility->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_STRONGAGAINST)
			rbMonsterStrongAgainst->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_WEAKAGAINST)
			rbMonsterWeakAgainst->setChecked(true);
		else if (monsterText.textType == MenuText::MONSTER_EXPERIENCE)
			rbMonsterExperience->setChecked(true);
	}
	else if (textType == MenuText::TYPE_ITEM)
	{
		MenuText::ItemText itemText;
		text->getText(&itemText);
		rbItem->setChecked(true);
		if (itemText.idIsReferencedInVariable)
		{
			int varID = itemText.itemID;
			rbItemVariable->setChecked(true);
			lblItemVariable->setText(ProjectData::numberVariableIdToString(varID));
		}
		else
		{
			rbItemSpecific->setChecked(true);
			cboxItem->setCurrentIndex(itemText.itemID);
		}
		if (itemText.textType == MenuText::ITEM_NAME)
			rbItemName->setChecked(true);
		else if (itemText.textType == MenuText::ITEM_DESCRIPTION)
			rbItemDescription->setChecked(true);
		else if (itemText.textType == MenuText::ITEM_PRICE)
			rbItemPrice->setChecked(true);
	}
	else if (textType == MenuText::TYPE_CHARACTER)
	{
		MenuText::CharacterText characterText;
		text->getText(&characterText);
		rbCharacter->setChecked(true);
		if (characterText.idIsReferencedInVariable)
		{
			int varID = characterText.characterID;
			rbCharacterVariable->setChecked(true);
			lblCharacterVariable->setText(ProjectData::numberVariableIdToString(varID));
		}
		else
		{
			rbCharacterSpecific->setChecked(true);
			cboxCharacter->setCurrentIndex(characterText.characterID);
		}
		if (characterText.textType == MenuText::CHARACTER_NAME)
			rbCharacterName->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_CLASS)
			rbCharacterClass->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_CONDITION)
			rbCharacterCondition->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_LEVEL)
			rbCharacterLevel->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_HP)
			rbCharacterHP->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_MAXHP)
			rbCharacterMaxHP->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_MP)
			rbCharacterMP->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_MAXMP)
			rbCharacterMaxMP->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_EXPERIENCE)
			rbCharacterTotalExperience->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_EXPERIENCETONEXTLEVEL)
			rbCharacterExperienceToNextLevel->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_BASEATTACK)
			rbCharacterBaseAttack->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_TOTALATTACK)
			rbCharacterTotalAttack->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_BASEDEFENSE)
			rbCharacterBaseDefense->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_TOTALDEFENSE)
			rbCharacterTotalDefense->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_BASEWISDOM)
			rbCharacterBaseWisdom->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_TOTALWISDOM)
			rbCharacterTotalWisdom->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_BASEAGILITY)
			rbCharacterBaseAgility->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_TOTALAGILITY)
			rbCharacterTotalAgility->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_EQUIPPEDWEAPON)
			rbCharacterEquippedWeapon->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_EQUIPPEDSHIELD)
			rbCharacterEquippedShield->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_EQUIPPEDARMOR)
			rbCharacterEquippedArmor->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_EQUIPPEDHELMET)
			rbCharacterEquippedHelmet->setChecked(true);
		else if (characterText.textType == MenuText::CHARACTER_EQUIPPEDACCESSORY)
			rbCharacterEquippedAccessory->setChecked(true);
		if (characterText.textType >= MenuText::CHARACTER_EQUIPPEDWEAPON && characterText.textType <= MenuText::CHARACTER_EQUIPPEDACCESSORY)
		{
			if (characterText.equipmentTextType == MenuText::EQUIPMENT_NAME)
				rbCharacterEquipmentName->setChecked(true);
			else if (characterText.equipmentTextType == MenuText::EQUIPMENT_DESCRIPTION)
				rbCharacterEquipmentAttack->setChecked(true);
			else if (characterText.equipmentTextType == MenuText::EQUIPMENT_ATTACK)
				rbCharacterEquipmentWisdom->setChecked(true);
			else if (characterText.equipmentTextType == MenuText::EQUIPMENT_DEFENSE)
				rbCharacterEquipmentDescription->setChecked(true);
			else if (characterText.equipmentTextType == MenuText::EQUIPMENT_WISDOM)
				rbCharacterEquipmentDefense->setChecked(true);
			else if (characterText.equipmentTextType == MenuText::EQUIPMENT_AGILITY)
				rbCharacterEquipmentAgility->setChecked(true);
		}
	}
	else if (textType == MenuText::TYPE_VARIABLE)
	{
		MenuText::VariableText variableText;
		text->getText(&variableText);
		rbVariable->setChecked(true);
		lblVariable->setText(ProjectData::numberVariableIdToString(variableText.variableID));
		cbAlwaysShowPlusSign->setChecked(variableText.alwaysShowPlusSign);
	}
	else if (textType == MenuText::TYPE_SWITCH)
	{
		MenuText::SwitchText switchText;
		text->getText(&switchText);
		rbSwitch->setChecked(true);
		lblSwitch->setText(ProjectData::switchVariableIdToString(switchText.switchID));
		leSwitchOnText->setText(switchText.onText);
		leSwitchOffText->setText(switchText.offText);
	}
	else if (textType == MenuText::TYPE_STRING)
	{
		MenuText::StringText stringText;
		text->getText(&stringText);
		rbString->setChecked(true);
		if (stringText.systemString)
		{
			rbStringSystem->setChecked(true);
			cboxStrings->setCurrentIndex(stringText.stringID);
		}
		else
		{
			rbStringVariable->setChecked(true);
			lblStringVariable->setText(ProjectData::stringVariableIdToString(stringText.stringID));
		}
	}
}

ConfigureMenuText::~ConfigureMenuText()
{
}

void ConfigureMenuText::on_bBrowseSwitches_clicked()
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

void ConfigureMenuText::on_bBrowseVariables_clicked()
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

void ConfigureMenuText::on_bCharacterBrowseVariables_clicked()
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

void ConfigureMenuText::on_bItemBrowseVariables_clicked()
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

void ConfigureMenuText::on_bMonsterBrowseVariables_clicked()
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

void ConfigureMenuText::on_bPartyMember_BrowseVariables_clicked()
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

void ConfigureMenuText::on_bStringBrowseVariables_clicked()
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

void ConfigureMenuText::on_rbPlainText_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wPlainText);
}

void ConfigureMenuText::on_rbPartyMember_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wPartyMember);
}

void ConfigureMenuText::on_rbCharacter_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wCharacter);
}

void ConfigureMenuText::on_rbMonster_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wMonster);
}

void ConfigureMenuText::on_rbItem_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wItem);
}

void ConfigureMenuText::on_rbString_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wString);
}

void ConfigureMenuText::on_rbVariable_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wVariable);
}

void ConfigureMenuText::on_rbSwitch_toggled(bool on)
{
	if (on)
		stackedWidget->setCurrentWidget(wSwitch);
}

void ConfigureMenuText::on_rbCharacterEquippedAccessory_toggled(bool on)
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

void ConfigureMenuText::on_rbCharacterEquippedArmor_toggled(bool on)
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

void ConfigureMenuText::on_rbCharacterEquippedHelmet_toggled(bool on)
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

void ConfigureMenuText::on_rbCharacterEquippedShield_toggled(bool on)
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

void ConfigureMenuText::on_rbCharacterEquippedWeapon_toggled(bool on)
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

void ConfigureMenuText::on_rbCharacterSpecific_toggled(bool on)
{
	cboxCharacter->setEnabled(on);
}

void ConfigureMenuText::on_rbCharacterVariable_toggled(bool on)
{
	wCharacter_Variable->setEnabled(on);
}

void ConfigureMenuText::on_rbItemSpecific_toggled(bool on)
{
	cboxItem->setEnabled(on);
}

void ConfigureMenuText::on_rbItemVariable_toggled(bool on)
{
	wItem_Variable->setEnabled(on);
}

void ConfigureMenuText::on_rbMonsterSpecific_toggled(bool on)
{
	cboxMonster->setEnabled(on);
}

void ConfigureMenuText::on_rbMonsterVariable_toggled(bool on)
{
	wMonster_Variable->setEnabled(on);
}

void ConfigureMenuText::on_rbPartyMemberEquippedArmor_toggled(bool on)
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

void ConfigureMenuText::on_rbPartyMemberEquippedHelmet_toggled(bool on)
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

void ConfigureMenuText::on_rbPartyMemberEquippedShield_toggled(bool on)
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

void ConfigureMenuText::on_rbPartyMemberEquippedWeapon_toggled(bool on)
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

void ConfigureMenuText::on_rbPartyMemberEquippedAccessory_toggled(bool on)
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

void ConfigureMenuText::on_rbPartyMemberVar_toggled(bool on)
{
	wPartyMember_Variable->setEnabled(on);
}

void ConfigureMenuText::on_rbStringSystem_toggled(bool on)
{
	cboxStrings->setEnabled(on);
}

void ConfigureMenuText::on_rbStringVariable_toggled(bool on)
{
	wStringVariable->setEnabled(on);
}

void ConfigureMenuText::on_cbUseValueInSaveSlot_toggled(bool on)
{
	wSaveSlot->setEnabled(on);
}

void ConfigureMenuText::on_buttonBox_accepted()
{
	if (cbUseValueInSaveSlot->isChecked())
		text->setSaveSlotID(sbSaveSlot->value());
	else
		text->setSaveSlotID(0);
	if (rbPlainText->isChecked())
	{
		text->setTextType(MenuText::TYPE_PLAINTEXT);
		text->setText(lePlainText->text());
	}
	else if (rbPartyMember->isChecked())
	{
		MenuText::PartyMemberText partyMemberText;
		text->setTextType(MenuText::TYPE_PARTYMEMBER);
		if (rbPartyMember1->isChecked())
			partyMemberText.memberID = 0;
		else if (rbPartyMember2->isChecked())
			partyMemberText.memberID = 1;
		else if (rbPartyMember3->isChecked())
			partyMemberText.memberID = 2;
		else if (rbPartyMember4->isChecked())
			partyMemberText.memberID = 3;
		else
			partyMemberText.memberID = ProjectData::numberVariableIdFromString(lblPartyMember_Variable->text()) + 4;
		if (rbPartyMemberName->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_NAME;
		else if (rbPartyMemberClass->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_CLASS;
		else if (rbPartyMemberCondition->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_CONDITION;
		else if (rbPartyMemberLevel->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_LEVEL;
		else if (rbPartyMemberHP->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_HP;
		else if (rbPartyMemberMaxHP->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_MAXHP;
		else if (rbPartyMemberMP->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_MP;
		else if (rbPartyMemberMaxMP->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_MAXMP;
		else if (rbPartyMemberTotalExperience->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_EXPERIENCE;
		else if (rbPartyMemberExperienceToNextLevel->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_EXPERIENCETONEXTLEVEL;
		else if (rbPartyMemberBaseAttack->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_BASEATTACK;
		else if (rbPartyMemberTotalAttack->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_TOTALATTACK;
		else if (rbPartyMemberBaseDefense->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_BASEDEFENSE;
		else if (rbPartyMemberTotalDefense->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_TOTALDEFENSE;
		else if (rbPartyMemberBaseWisdom->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_BASEWISDOM;
		else if (rbPartyMemberTotalWisdom->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_TOTALWISDOM;
		else if (rbPartyMemberBaseAgility->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_BASEAGILITY;
		else if (rbPartyMemberTotalAgility->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_TOTALAGILITY;
		else if (rbPartyMemberEquippedWeapon->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_EQUIPPEDWEAPON;
		else if (rbPartyMemberEquippedShield->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_EQUIPPEDSHIELD;
		else if (rbPartyMemberEquippedArmor->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_EQUIPPEDARMOR;
		else if (rbPartyMemberEquippedHelmet->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_EQUIPPEDHELMET;
		else if (rbPartyMemberEquippedAccessory->isChecked())
			partyMemberText.textType = MenuText::PARTYMEMBER_EQUIPPEDACCESSORY;
		if (rbPartyMemberEquipmentName->isChecked())
		 partyMemberText.equipmentTextType = MenuText::EQUIPMENT_NAME;
		else if (rbPartyMemberEquipmentDescription->isChecked())
			partyMemberText.equipmentTextType = MenuText::EQUIPMENT_DESCRIPTION;
		else if (rbPartyMemberEquipmentAttack->isChecked())
			partyMemberText.equipmentTextType = MenuText::EQUIPMENT_ATTACK;
		else if (rbPartyMemberEquipmentDefense->isChecked())
			partyMemberText.equipmentTextType = MenuText::EQUIPMENT_DEFENSE;
		else if (rbPartyMemberEquipmentWisdom->isChecked())
			partyMemberText.equipmentTextType = MenuText::EQUIPMENT_WISDOM;
		else if (rbPartyMemberEquipmentAgility->isChecked())
			partyMemberText.equipmentTextType = MenuText::EQUIPMENT_AGILITY;
		text->setText(&partyMemberText);
	}
	else if (rbMonster->isChecked())
	{
		MenuText::MonsterText monsterText;
		text->setTextType(MenuText::TYPE_MONSTER);
		monsterText.idIsReferencedInVariable = rbMonsterVariable->isChecked();
		if (monsterText.idIsReferencedInVariable)
			monsterText.monsterID = ProjectData::numberVariableIdFromString(lblMonsterVariable->text());
		else
			monsterText.monsterID = cboxMonster->currentIndex();
		if (rbMonsterName->isChecked())
			monsterText.textType = MenuText::MONSTER_NAME;
		else if (rbMonsterMoney->isChecked())
			monsterText.textType = MenuText::MONSTER_MONEY;
		else if (rbMonsterHP->isChecked())
			monsterText.textType = MenuText::MONSTER_HP;
		else if (rbMonsterMP->isChecked())
			monsterText.textType = MenuText::MONSTER_MP;
		else if (rbMonsterAttack->isChecked())
			monsterText.textType = MenuText::MONSTER_ATTACK;
		else if (rbMonsterDefense->isChecked())
			monsterText.textType = MenuText::MONSTER_DEFENSE;
		else if (rbMonsterWisdom->isChecked())
			monsterText.textType = MenuText::MONSTER_WISDOM;
		else if (rbMonsterAgility->isChecked())
			monsterText.textType = MenuText::MONSTER_AGILITY;
		else if (rbMonsterStrongAgainst->isChecked())
			monsterText.textType = MenuText::MONSTER_STRONGAGAINST;
		else if (rbMonsterWeakAgainst->isChecked())
			monsterText.textType = MenuText::MONSTER_WEAKAGAINST;
		else if (rbMonsterExperience->isChecked())
			monsterText.textType = MenuText::MONSTER_EXPERIENCE;
		text->setText(&monsterText);
	}
	else if (rbItem->isChecked())
	{
		MenuText::ItemText itemText;
		text->setTextType(MenuText::TYPE_ITEM);
		itemText.idIsReferencedInVariable = rbItemVariable->isChecked();
		if (itemText.idIsReferencedInVariable)
			itemText.itemID = ProjectData::numberVariableIdFromString(lblItemVariable->text());
		else
			itemText.itemID = cboxItem->currentIndex();
		if (rbItemName->isChecked())
			itemText.textType = MenuText::ITEM_NAME;
		else if (rbItemDescription->isChecked())
			itemText.textType = MenuText::ITEM_DESCRIPTION;
		else if (rbItemPrice->isChecked())
			itemText.textType = MenuText::ITEM_PRICE;
		text->setText(&itemText);
	}
	else if (rbCharacter->isChecked())
	{
		MenuText::CharacterText characterText;
		text->setTextType(MenuText::TYPE_CHARACTER);
		characterText.idIsReferencedInVariable = rbCharacterVariable->isChecked();
		if (characterText.idIsReferencedInVariable)
			characterText.characterID = ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
		else
			characterText.characterID = cboxCharacter->currentIndex();
		if (rbCharacterName->isChecked())
			characterText.textType = MenuText::CHARACTER_NAME;
		else if (rbCharacterClass->isChecked())
			characterText.textType = MenuText::CHARACTER_CLASS;
		else if (rbCharacterCondition->isChecked())
			characterText.textType = MenuText::CHARACTER_CONDITION;
		else if (rbCharacterLevel->isChecked())
			characterText.textType = MenuText::CHARACTER_LEVEL;
		else if (rbCharacterHP->isChecked())
			characterText.textType = MenuText::CHARACTER_HP;
		else if (rbCharacterMaxHP->isChecked())
			characterText.textType = MenuText::CHARACTER_MAXHP;
		else if (rbCharacterMP->isChecked())
			characterText.textType = MenuText::CHARACTER_MP;
		else if (rbCharacterMaxMP->isChecked())
			characterText.textType = MenuText::CHARACTER_MAXMP;
		else if (rbCharacterTotalExperience->isChecked())
			characterText.textType = MenuText::CHARACTER_EXPERIENCE;
		else if (rbCharacterExperienceToNextLevel->isChecked())
			characterText.textType = MenuText::CHARACTER_EXPERIENCETONEXTLEVEL;
		else if (rbCharacterBaseAttack->isChecked())
			characterText.textType = MenuText::CHARACTER_BASEATTACK;
		else if (rbCharacterTotalAttack->isChecked())
			characterText.textType = MenuText::CHARACTER_TOTALATTACK;
		else if (rbCharacterBaseDefense->isChecked())
			characterText.textType = MenuText::CHARACTER_BASEDEFENSE;
		else if (rbCharacterTotalDefense->isChecked())
			characterText.textType = MenuText::CHARACTER_TOTALDEFENSE;
		else if (rbCharacterBaseWisdom->isChecked())
			characterText.textType = MenuText::CHARACTER_BASEWISDOM;
		else if (rbCharacterTotalWisdom->isChecked())
			characterText.textType = MenuText::CHARACTER_TOTALWISDOM;
		else if (rbCharacterBaseAgility->isChecked())
			characterText.textType = MenuText::CHARACTER_BASEAGILITY;
		else if (rbCharacterTotalAgility->isChecked())
			characterText.textType = MenuText::CHARACTER_TOTALAGILITY;
		else if (rbCharacterEquippedWeapon->isChecked())
			characterText.textType = MenuText::CHARACTER_EQUIPPEDWEAPON;
		else if (rbCharacterEquippedShield->isChecked())
			characterText.textType = MenuText::CHARACTER_EQUIPPEDSHIELD;
		else if (rbCharacterEquippedArmor->isChecked())
			characterText.textType = MenuText::CHARACTER_EQUIPPEDARMOR;
		else if (rbCharacterEquippedHelmet->isChecked())
			characterText.textType = MenuText::CHARACTER_EQUIPPEDHELMET;
		else if (rbCharacterEquippedAccessory->isChecked())
			characterText.textType = MenuText::CHARACTER_EQUIPPEDACCESSORY;
		if (rbCharacterEquipmentName->isChecked())
			characterText.equipmentTextType = MenuText::EQUIPMENT_NAME;
		else if (rbCharacterEquipmentDescription->isChecked())
			characterText.equipmentTextType = MenuText::EQUIPMENT_DESCRIPTION;
		else if (rbCharacterEquipmentAttack->isChecked())
			characterText.equipmentTextType = MenuText::EQUIPMENT_ATTACK;
		else if (rbCharacterEquipmentDefense->isChecked())
			characterText.equipmentTextType = MenuText::EQUIPMENT_DEFENSE;
		else if (rbCharacterEquipmentWisdom->isChecked())
			characterText.equipmentTextType = MenuText::EQUIPMENT_WISDOM;
		else if (rbCharacterEquipmentAgility->isChecked())
			characterText.equipmentTextType = MenuText::EQUIPMENT_AGILITY;
		text->setText(&characterText);
	}
	else if (rbVariable->isChecked())
	{
		MenuText::VariableText variableText;
		text->setTextType(MenuText::TYPE_VARIABLE);
		variableText.variableID = ProjectData::numberVariableIdFromString(lblVariable->text());
		variableText.alwaysShowPlusSign = cbAlwaysShowPlusSign->isChecked();
		text->setText(&variableText);
	}
	else if (rbSwitch->isChecked())
	{
		MenuText::SwitchText switchText;
		text->setTextType(MenuText::TYPE_SWITCH);
		switchText.switchID = ProjectData::switchVariableIdFromString(lblSwitch->text());
		switchText.onText = leSwitchOnText->text();
		switchText.offText = leSwitchOffText->text();
		text->setText(&switchText);
	}
	else if (rbString->isChecked())
	{
		MenuText::StringText stringText;
		text->setTextType(MenuText::TYPE_STRING);
		stringText.systemString = rbStringSystem->isChecked();
		if (stringText.systemString)
			stringText.stringID = cboxStrings->currentIndex();
		else
			stringText.stringID = ProjectData::stringVariableIdFromString(lblStringVariable->text());
		text->setText(&stringText);
	}
}
