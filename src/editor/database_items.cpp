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
 *  Copyright (C) 2010, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QScrollBar>
#include <QTimer>
#include <QMessageBox>
#include <QHeaderView>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "clipboard.h"
#include "database_items.h"
#include "projectdata.h"
#include "projectdata_item.h"
#include "variableselector.h"
#include "xmlParser.h"

Database_Items::Database_Items(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	bPriceSpinButton->setSpinBox(sbPrice);
	weapon_bAttackChangeSpinButton->setSpinBox(weapon_sbAttackChange);
	weapon_bDefenseChangeSpinButton->setSpinBox(weapon_sbDefenseChange);
	weapon_bWisdomChangeSpinButton->setSpinBox(weapon_sbWisdomChange);
	weapon_bAgilityChangeSpinButton->setSpinBox(weapon_sbAgilityChange);
	weapon_bMPConsumedSpinButton->setSpinBox(weapon_sbMPConsumed);
	weapon_bHitProbabilitySpinButton->setSpinBox(weapon_sbHitProbability);
	weapon_bCriticalBonusSpinButton->setSpinBox(weapon_sbCriticalBonus);
	weapon_bConditionEffectivenessSpinButton->setSpinBox(weapon_sbConditionEffectiveness);
	weapon_twCharacterClassList->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	weapon_twAttributeList->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	weapon_twConditionList->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	weapon_wBattleAnimationPreview->setScale(1.0/3.0);
	medicine_bPercentHPRecoverySpinButton->setSpinBox(medicine_sbPercentHPRecovery);
	medicine_bAmountHPRecoverySpinButton->setSpinBox(medicine_sbAmountHPRecovery);
	medicine_bPercentMPRecoverySpinButton->setSpinBox(medicine_sbPercentMPRecovery);
	medicine_bAmountMPRecoverySpinButton->setSpinBox(medicine_sbAmountMPRecovery);
	medicine_twCharacterClassList->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	seed_bMaxHPChangeSpinButton->setSpinBox(seed_sbMaxHPChange);
	seed_bMaxMPChangeSpinButton->setSpinBox(seed_sbMaxMPChange);
	seed_bAttackChangeSpinButton->setSpinBox(seed_sbAttackChange);
	seed_bDefenseChangeSpinButton->setSpinBox(seed_sbDefenseChange);
	seed_bWisdomChangeSpinButton->setSpinBox(seed_sbWisdomChange);
	seed_bAgilityChangeSpinButton->setSpinBox(seed_sbAgilityChange);
	seed_twCharacterClassList->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	equipment_bAttackChangeSpinButton->setSpinBox(equipment_sbAttackChange);
	equipment_bDefenseChangeSpinButton->setSpinBox(equipment_sbDefenseChange);
	equipment_bWisdomChangeSpinButton->setSpinBox(equipment_sbWisdomChange);
	equipment_bAgilityChangeSpinButton->setSpinBox(equipment_sbAgilityChange);
	equipment_bResistanceSpinButton->setSpinBox(equipment_sbResistance);
	equipment_twCharacterClassList->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	battleCharacterTimer = new QTimer(this);
	itemRef = NULL;
	animRef = NULL;
	ignoreEvents = true;
	QObject::connect(battleCharacterTimer, SIGNAL(timeout()), this, SLOT(animateBattleCharacter()));
}

Database_Items::~Database_Items()
{
	if (itemRef != NULL)
	{
		updateCurrentItem();
		delete itemRef;
		itemRef = NULL;
	}
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

void Database_Items::setupItems()
{
	battleCharacterTimer->start(200);
	itemRef = NULL;
	animRef = NULL;
	stackedWidget1->setCurrentWidget(wPlaceHolder);
	stackedWidget2->setCurrentWidget(wCommonItem);
	updateItemList();
	weapon_updateCharacterClassList();
	weapon_updateSkillList();
	weapon_updateBattleAnimationList();
	weapon_updateAttributeList();
	weapon_updateConditionList();
	medicine_updateCharacterClassList();
	medicine_updateConditionList();
	skillBook_updateCharacterClassList();
	skillBook_updateSkillList();
	seed_updateCharacterClassList();
	skillScroll_updateCharacterClassList();
	skillScroll_updateSkillList();
	equipment_updateSkillList();
	equipment_updateCharacterClassList();
	equipment_updateAttributeList();
	equipment_updateConditionList();
	switch_lwSwitch->addItem(ProjectData::switchVariableIdToString(0));
	if (ProjectData::itemNames.size() > 0)
	{
		lwItemList->setCurrentRow(0);
		lwItemList->item(0)->setSelected(true);
	}
	ignoreEvents = false;
}

void Database_Items::shutdownItems()
{
	if (itemRef != NULL)
	{
		updateCurrentItem();
		delete itemRef;
		itemRef = NULL;
	}
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
	weapon_wBattleAnimationPreview->stopAnimation();
}

void Database_Items::on_lwItemList_itemSelectionChanged()
{
	Item::WeaponItem *weaponData;
	Item::EquipmentItem *equipmentData;
	Item::MedicineItem *medicineData;
	Item::SkillBookItem *skillBookData;
	Item::SeedItem *seedData;
	Item::SkillScrollItem *skillScrollData;
	Item::SwitchItem *switchData;
	Item::EventItem *eventData;
	int itemType;
	int itemID;
	updateCurrentItem();
	commonItem_clearData();
	equipment_clearData();
	event_clearData();
	medicine_clearData();
	seed_clearData();
	skillBook_clearData();
	skillScroll_clearData();
	switch_clearData();
	weapon_clearData();
	if (itemRef != NULL)
	{
		delete itemRef;
		itemRef = NULL;
	}
	itemID = lwItemList->currentRow();
	if (itemID >= 0)
	{
		itemRef = new ProjectData::DataReference<Item>(itemID, __FILE__, __LINE__);
		leName->setText((*itemRef)->getName());
		cboxType->setCurrentIndex((*itemRef)->getType());
		sbPrice->setValue((*itemRef)->getPrice());
		itemType = (*itemRef)->getType();
		if (itemType == Item::TYPE_COMMONITEM)
			commonItem_leDescription->setText((*itemRef)->getDescription());
		else if (itemType == Item::TYPE_WEAPON)
		{
			weapon_wBattleAnimationPreview->stopAnimation();
			weaponData = (*itemRef)->getWeaponData();
			if (weaponData->twoHanded)
				rbBothHands->setChecked(true);
			else
				rbOneHand->setChecked(true);
			weapon_leDescription->setText((*itemRef)->getDescription());
			weapon_cboxSkill->setCurrentIndex(weaponData->skillID + 1);
			if (weaponData->usableByCharacters)
				weapon_rbUsableByCharacter->setChecked(true);
			else
				weapon_rbUsableByClass->setChecked(true);
			for (int i = 0; i < weapon_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)weapon_twCharacterClassList->cellWidget(i, 0);
				if (i >= weaponData->usableBy.size())
					checkBox->setChecked(true);
				else
					checkBox->setChecked(weaponData->usableBy[i]);
			}
			weapon_sbAttackChange->setValue(weaponData->attackChange);
			weapon_sbDefenseChange->setValue(weaponData->defenseChange);
			weapon_sbWisdomChange->setValue(weaponData->wisdomChange);
			weapon_sbAgilityChange->setValue(weaponData->agilityChange);
			weapon_cbAttackFirst->setChecked(weaponData->alwaysAttackFirst);
			weapon_cbAttackTwice->setChecked(weaponData->attackTwice);
			weapon_cbAttackAllEnemies->setChecked(weaponData->attackAllEnemies);
			weapon_cbIgnoreEnemyEvasion->setChecked(weaponData->ignoreEnemyEvasion);
			weapon_cbCursed->setChecked(weaponData->cursed);
			weapon_cbUsableOnTheDead->setChecked(weaponData->usableOnTheDead);
			weapon_sbMPConsumed->setValue(weaponData->amountMPConsumed);
			weapon_sbHitProbability->setValue(weaponData->percentHitProbability);
			weapon_sbCriticalBonus->setValue(weaponData->percentCriticalBonus);
			for (int i = 0; i < weapon_twAttributeList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)weapon_twAttributeList->cellWidget(i, 0);
				if (i >= weaponData->attribute.size())
					checkBox->setChecked(false);
				else
					checkBox->setChecked(weaponData->attribute[i]);
			}
			for (int i = 0; i < weapon_twConditionList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)weapon_twConditionList->cellWidget(i, 0);
				if (i >= weaponData->conditions.size())
					checkBox->setChecked(false);
				else
					checkBox->setChecked(weaponData->conditions[i]);
			}
			if (weaponData->inflictConditions)
				weapon_rbInflictCondition->setChecked(true);
			else
				weapon_rbRemoveCondition->setChecked(true);
			weapon_sbConditionEffectiveness->setValue(weaponData->percentConditionEffectivenessProbability);
			weapon_cboxBattleAnimation->setCurrentIndex(weaponData->battleAnimationID);
			on_weapon_cboxBattleAnimation_currentIndexChanged(weaponData->battleAnimationID); // called just in case the new weapons battle animation ID is the same as the previous.
			TODO("Handle weaponData->animationSettings");
		}
		else if (itemType == Item::TYPE_SHIELD || itemType == Item::TYPE_ARMOR || itemType == Item::TYPE_HELMET || itemType == Item::TYPE_ACCESSORY)
		{
			equipmentData = (*itemRef)->getEquipmentData();
			equipment_leDescription->setText((*itemRef)->getDescription());
			equipment_cboxSkill->setCurrentIndex(equipmentData->skillID + 1);
			if (equipmentData->usableByCharacters)
				equipment_rbUsableByCharacter->setChecked(true);
			else
				equipment_rbUsableByClass->setChecked(true);
			for (int i = 0; i < equipment_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)equipment_twCharacterClassList->cellWidget(i, 0);
				if (i >= equipmentData->usableBy.size())
					checkBox->setChecked(true);
				else
					checkBox->setChecked(equipmentData->usableBy[i]);
			}
			equipment_sbAttackChange->setValue(equipmentData->attackChange);
			equipment_sbDefenseChange->setValue(equipmentData->defenseChange);
			equipment_sbWisdomChange->setValue(equipmentData->wisdomChange);
			equipment_sbAgilityChange->setValue(equipmentData->agilityChange);
			equipment_cbPreventCriticalHits->setChecked(equipmentData->preventCriticalHits);
			equipment_cbIncreaseEvasion->setChecked(equipmentData->increaseEvasion);
			equipment_cbHalfMPConsumption->setChecked(equipmentData->halfMPConsumption);
			equipment_cbCursed->setChecked(equipmentData->cursed);
			for (int i = 0; i < equipment_twResistance->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)equipment_twResistance->cellWidget(i, 0);
				if (i >= equipmentData->attributeResistance.size())
					checkBox->setChecked(false);
				else
					checkBox->setChecked(equipmentData->attributeResistance[i]);
			}
			for (int i = 0; i < equipment_twConditions->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)equipment_twConditions->cellWidget(i, 0);
				if (i >= equipmentData->conditions.size())
					checkBox->setChecked(false);
				else
					checkBox->setChecked(equipmentData->conditions[i]);
			}
			if (equipmentData->resistConditions)
				equipment_rbResistCondition->setChecked(true);
			else
				equipment_rbInflictCondition->setChecked(true);
			equipment_sbResistance->setValue(equipmentData->percentConditionResistanceEffectiveness);
		}
		else if (itemType == Item::TYPE_MEDICINE)
		{
			medicineData = (*itemRef)->getMedicineData();
			cboxNumberOfUses->setCurrentIndex(medicineData->numberOfUses);
			medicine_leDescription->setText((*itemRef)->getDescription());
			if (medicineData->targetSingleAlly)
				medicine_rbTargetSingleAlly->setChecked(true);
			else
				medicine_rbTargetAllAllies->setChecked(true);
			if (medicineData->usableByCharacters)
				medicine_rbUsableByCharacter->setChecked(true);
			else
				medicine_rbUsableByClass->setChecked(true);
			for (int i = 0; i < medicine_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)medicine_twCharacterClassList->cellWidget(i, 0);
				if (i >= medicineData->usableBy.size())
					checkBox->setChecked(true);
				else
					checkBox->setChecked(medicineData->usableBy[i]);
			}
			for (int i = 0; i < medicine_twConditionList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)medicine_twConditionList->cellWidget(i, 0);
				if (i >= medicineData->cureCondition.size())
					checkBox->setChecked(false);
				else
					checkBox->setChecked(medicineData->cureCondition[i]);
			}
			medicine_sbPercentHPRecovery->setValue(medicineData->percentRestoredHP);
			medicine_sbAmountHPRecovery->setValue(medicineData->amountRestoredHP);
			medicine_sbPercentMPRecovery->setValue(medicineData->percentRestoredMP);
			medicine_sbAmountMPRecovery->setValue(medicineData->amountRestoredMP);
			medicine_cbFieldMapOnly->setChecked(medicineData->onlyUsableOnFieldMap);
			medicine_cbDeadCharactersOnly->setChecked(medicineData->onlyUsableOnDeadCharacters);
		}
		else if (itemType == Item::TYPE_SKILLBOOK)
		{
			skillBookData = (*itemRef)->getSkillBookData();
			cboxNumberOfUses->setCurrentIndex(skillBookData->numberOfUses);
			skillBook_leDescription->setText((*itemRef)->getDescription());
			if (skillBookData->usableByCharacters)
				skillBook_rbUsableByCharacter->setChecked(true);
			else
				skillBook_rbUsableByClass->setChecked(true);
			for (int i = 0; i < skillBook_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)skillBook_twCharacterClassList->cellWidget(i, 0);
				if (i >= skillBookData->usableBy.size())
					checkBox->setChecked(true);
				else
					checkBox->setChecked(skillBookData->usableBy[i]);
			}
			skillBook_cboxSkillLearned->setCurrentIndex(skillBookData->skillID);
		}
		else if (itemType == Item::TYPE_SEED)
		{
			seedData = (*itemRef)->getSeedData();
			cboxNumberOfUses->setCurrentIndex(seedData->numberOfUses);
			seed_leDescription->setText((*itemRef)->getDescription());
			if (seedData->usableByCharacters)
				seed_rbUsableByCharacter->setChecked(true);
			else
				seed_rbUsableByClass->setChecked(true);
			for (int i = 0; i < seed_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)seed_twCharacterClassList->cellWidget(i, 0);
				if (i >= seedData->usableBy.size())
					checkBox->setChecked(true);
				else
					checkBox->setChecked(seedData->usableBy[i]);
			}
			seed_sbMaxHPChange->setValue(seedData->maxHPChange);
			seed_sbMaxMPChange->setValue(seedData->maxMPChange);
			seed_sbAttackChange->setValue(seedData->attackChange);
			seed_sbDefenseChange->setValue(seedData->defenseChange);
			seed_sbWisdomChange->setValue(seedData->wisdomChange);
			seed_sbAgilityChange->setValue(seedData->agilityChange);
		}
		else if (itemType == Item::TYPE_SKILLSCROLL)
		{
			skillScrollData = (*itemRef)->getSkillScrollData();
			cboxNumberOfUses->setCurrentIndex(skillScrollData->numberOfUses);
			skillScroll_leDescription->setText((*itemRef)->getDescription());
			if (skillScrollData->usableByCharacters)
				skillScroll_rbUsableByCharacter->setChecked(true);
			else
				skillScroll_rbUsableByClass->setChecked(true);
			for (int i = 0; i < skillScroll_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)skillScroll_twCharacterClassList->cellWidget(i, 0);
				if (i >= skillScrollData->usableBy.size())
					checkBox->setChecked(true);
				else
					checkBox->setChecked(skillScrollData->usableBy[i]);
			}
			skillScroll_cboxSkillInvoked->setCurrentIndex(skillScrollData->skillID);
			if (skillScrollData->showItemNameMessageWhenUsed)
				skillScroll_rbItemName->setChecked(true);
			else
				skillScroll_rbSkillName->setChecked(true);
		}
		else if (itemType == Item::TYPE_SWITCH)
		{
			switchData = (*itemRef)->getSwitchData();
			cboxNumberOfUses->setCurrentIndex(switchData->numberOfUses);
			switch_leDescription->setText((*itemRef)->getDescription());
			TODO("Handle switchData->switchID");
			switch_cbAllowField->setChecked(switchData->allowFieldActivation);
			switch_cbAllowBattle->setChecked(switchData->allowBattleActivation);
		}
		else if (itemType == Item::TYPE_EVENT)
		{
			eventData = (*itemRef)->getEventData();
			cboxNumberOfUses->setCurrentIndex(eventData->numberOfUses);
			event_leDescription->setText((*itemRef)->getDescription());
			event_cbAllowField->setChecked(eventData->allowFieldActivation);
			event_cbAllowBattle->setChecked(eventData->allowBattleActivation);
			TODO("Handle eventData->event");
		}
	}
}

void Database_Items::on_bArraySize_clicked()
{
	int numItems = ProjectData::itemNames.size();
	ArraySize *arraysize = new ArraySize(numItems, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numItems)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (itemRef != NULL)
				{
					if (newSize >= itemRef->getDataID())
					{
						delete itemRef;
						itemRef = NULL;
					}
				}
				for (int i = numItems - 1; i >= newSize; --i)
					ProjectData::deleteItem(i);
			}
		}
		else
		{
			for (int i = numItems; i < newSize; ++i)
			{
				Item *item = new Item;
				item->setName(QString("Item%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addItem(item);
				delete item;
			}
		}
		ignoreEvents = true;
		updateItemList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_Items::on_leName_textChanged(QString text)
{
	if (!ignoreEvents)
	{
		int currentRow = lwItemList->currentRow();
		ignoreEvents = true;
		lwItemList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 4, 10, QChar('0')).arg(text));
		ProjectData::itemNames[currentRow] = text;
		ignoreEvents = false;
	}
}

void Database_Items::on_cboxType_currentIndexChanged(int index)
{
	if (index == Item::TYPE_COMMONITEM)
	{
		stackedWidget1->setCurrentWidget(wPlaceHolder);
		stackedWidget2->setCurrentWidget(wCommonItem);
	}
	else if (index == Item::TYPE_WEAPON)
	{
		stackedWidget1->setCurrentWidget(wHandsRequired);
		stackedWidget2->setCurrentWidget(wWeapon);
	}
	else if (index == Item::TYPE_SHIELD)
	{
		stackedWidget1->setCurrentWidget(wPlaceHolder);
		stackedWidget2->setCurrentWidget(wEquipment);
	}
	else if (index == Item::TYPE_ARMOR)
	{
		stackedWidget1->setCurrentWidget(wPlaceHolder);
		stackedWidget2->setCurrentWidget(wEquipment);
	}
	else if (index == Item::TYPE_HELMET)
	{
		stackedWidget1->setCurrentWidget(wPlaceHolder);
		stackedWidget2->setCurrentWidget(wEquipment);
	}
	else if (index == Item::TYPE_ACCESSORY)
	{
		stackedWidget1->setCurrentWidget(wPlaceHolder);
		stackedWidget2->setCurrentWidget(wEquipment);
	}
	else if (index == Item::TYPE_MEDICINE)
	{
		stackedWidget1->setCurrentWidget(wNumberOfUses);
		stackedWidget2->setCurrentWidget(wMedicine);
	}
	else if (index == Item::TYPE_SKILLBOOK)
	{
		stackedWidget1->setCurrentWidget(wNumberOfUses);
		stackedWidget2->setCurrentWidget(wSkillBook);
	}
	else if (index == Item::TYPE_SEED)
	{
		stackedWidget1->setCurrentWidget(wNumberOfUses);
		stackedWidget2->setCurrentWidget(wSeed);
	}
	else if (index == Item::TYPE_SKILLSCROLL)
	{
		stackedWidget1->setCurrentWidget(wNumberOfUses);
		stackedWidget2->setCurrentWidget(wSkillScroll);
	}
	else if (index == Item::TYPE_SWITCH)
	{
		stackedWidget1->setCurrentWidget(wNumberOfUses);
		stackedWidget2->setCurrentWidget(wSwitch);
	}
	else if (index == Item::TYPE_EVENT)
	{
		stackedWidget1->setCurrentWidget(wNumberOfUses);
		stackedWidget2->setCurrentWidget(wEvent);
	}
}

void Database_Items::on_weapon_rbUsableByCharacter_toggled()
{
	weapon_updateCharacterClassList();
}

void Database_Items::on_weapon_twCharacterClassList_cellClicked(int row)
{
	QCheckBox *checkBox = (QCheckBox*)weapon_twCharacterClassList->cellWidget(row, 0);
	checkBox->toggle();
}

void Database_Items::on_weapon_bSetAnimation_clicked()
{
	QMessageBox::information(window(), "Not Implemented", "This feature is not yet implemented.");
	TODO("Implement function");
}

void Database_Items::on_weapon_cboxBattleAnimation_currentIndexChanged(int index)
{
	if (index >= 0 && index < ProjectData::battleAnimationNames.size())
	{
		weapon_wBattleAnimationPreview->stopAnimation();
		if (animRef != NULL)
		{
			delete animRef;
			animRef = NULL;
		}
		animRef = new ProjectData::DataReference<BattleAnimation>(index, __FILE__, __LINE__);
		if (stackedWidget2->currentWidget() == wWeapon)
			weapon_wBattleAnimationPreview->cycleAnimation(animRef->getDataID(), QPixmap(), false);
	}
}

void Database_Items::on_weapon_rbInflictCondition_toggled(bool on)
{
	if (on)
		weapon_lblEffectivenessProbability->setText("Effectiveness");
}

void Database_Items::on_weapon_rbRemoveCondition_toggled(bool on)
{
	if (on)
		weapon_lblEffectivenessProbability->setText("Probability");
}

void Database_Items::on_medicine_rbUsableByCharacter_toggled()
{
	medicine_updateCharacterClassList();
}

void Database_Items::on_medicine_twCharacterClassList_cellClicked(int row)
{
	QCheckBox *checkBox = (QCheckBox*)medicine_twCharacterClassList->cellWidget(row, 0);
	checkBox->toggle();
}

void Database_Items::on_skillBook_rbUsableByCharacter_toggled()
{
	skillBook_updateCharacterClassList();
}

void Database_Items::on_skillBook_twCharacterClassList_cellClicked(int row)
{
	QCheckBox *checkBox = (QCheckBox*)skillBook_twCharacterClassList->cellWidget(row, 0);
	checkBox->toggle();
}

void Database_Items::on_seed_rbUsableByCharacter_toggled()
{
	seed_updateCharacterClassList();
}

void Database_Items::on_seed_twCharacterClassList_cellClicked(int row)
{
	QCheckBox *checkBox = (QCheckBox*)seed_twCharacterClassList->cellWidget(row, 0);
	checkBox->toggle();
}

void Database_Items::on_skillScroll_rbUsableByCharacter_toggled()
{
	skillScroll_updateCharacterClassList();
}

void Database_Items::on_skillScroll_twCharacterClassList_cellClicked(int row)
{
	QCheckBox *checkBox = (QCheckBox*)skillScroll_twCharacterClassList->cellWidget(row, 0);
	checkBox->toggle();
}

void Database_Items::on_switch_lwSwitch_itemDoubleClicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(switch_lwSwitch->item(0)->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		int switchID = variableSelector->getVariableID();
		switch_lwSwitch->clear();
		switch_lwSwitch->addItem(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void Database_Items::on_switch_tbBrowseSwitches_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(switch_lwSwitch->item(0)->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		int switchID = variableSelector->getVariableID();
		switch_lwSwitch->clear();
		switch_lwSwitch->addItem(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void Database_Items::on_equipment_rbUsableByCharacter_toggled()
{
	equipment_updateCharacterClassList();
}

void Database_Items::on_equipment_twCharacterClassList_cellClicked(int row)
{
	QCheckBox *checkBox = (QCheckBox*)equipment_twCharacterClassList->cellWidget(row, 0);
	checkBox->toggle();
}

void Database_Items::on_equipment_rbResistCondition_toggled(bool on)
{
	equipment_frmResistance->setEnabled(on);
}

void Database_Items::animateBattleCharacter()
{
	TODO("Implement function");
}

void Database_Items::updateItemList()
{
	QListWidgetItem *item;
	int i, numItems = ProjectData::itemNames.size();
	int currentRow = lwItemList->currentRow();
	lwItemList->clear();
	for (i = 0; i < numItems; ++i)
	{
		item = new QListWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::itemNames[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		lwItemList->addItem(item);
	}
	if (currentRow >= lwItemList->count())
		currentRow = lwItemList->count() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (lwItemList->count() > 0)
	{
		lwItemList->setCurrentRow(currentRow);
		lwItemList->item(currentRow)->setSelected(true);
	}
}

void Database_Items::commonItem_clearData()
{
	commonItem_leDescription->setText("No description");
}

void Database_Items::equipment_clearData()
{
	equipment_leDescription->setText("No description");
	equipment_cboxSkill->setCurrentIndex(0);
	equipment_rbUsableByCharacter->setChecked(true);
	equipment_sbAttackChange->setValue(0);
	equipment_sbDefenseChange->setValue(0);
	equipment_sbWisdomChange->setValue(0);
	equipment_sbAgilityChange->setValue(0);
	equipment_cbPreventCriticalHits->setChecked(false);
	equipment_cbIncreaseEvasion->setChecked(false);
	equipment_cbHalfMPConsumption->setChecked(false);
	equipment_cbNoTerrainDamage->setChecked(false);
	equipment_cbCursed->setChecked(false);
	equipment_rbResistCondition->setChecked(true);
	equipment_sbResistance->setValue(100);
	equipment_updateCharacterClassList();
	equipment_updateAttributeList();
	equipment_updateConditionList();
}

void Database_Items::event_clearData()
{
	cboxNumberOfUses->setCurrentIndex(1);
	event_leDescription->setText("No description");
	event_cbAllowField->setChecked(true);
	event_cbAllowBattle->setChecked(true);
	TODO("Clear event");
}

void Database_Items::medicine_clearData()
{
	cboxNumberOfUses->setCurrentIndex(1);
	medicine_leDescription->setText("No description");
	medicine_rbTargetSingleAlly->setChecked(true);
	medicine_rbUsableByCharacter->setChecked(true);
	medicine_sbPercentHPRecovery->setValue(0);
	medicine_sbAmountHPRecovery->setValue(0);
	medicine_sbPercentMPRecovery->setValue(0);
	medicine_sbAmountMPRecovery->setValue(0);
	medicine_cbFieldMapOnly->setChecked(false);
	medicine_cbDeadCharactersOnly->setChecked(false);
	medicine_updateCharacterClassList();
	medicine_updateConditionList();
}

void Database_Items::seed_clearData()
{
	cboxNumberOfUses->setCurrentIndex(1);
	seed_leDescription->setText("No description");
	seed_rbUsableByCharacter->setChecked(true);
	seed_sbMaxHPChange->setValue(0);
	seed_sbMaxMPChange->setValue(0);
	seed_sbAttackChange->setValue(0);
	seed_sbDefenseChange->setValue(0);
	seed_sbWisdomChange->setValue(0);
	seed_sbAgilityChange->setValue(0);
	seed_updateCharacterClassList();
}

void Database_Items::skillBook_clearData()
{
	cboxNumberOfUses->setCurrentIndex(1);
	skillBook_leDescription->setText("No description");
	skillBook_cboxSkillLearned->setCurrentIndex(0);
	skillBook_rbUsableByCharacter->setChecked(true);
	skillBook_updateCharacterClassList();
}

void Database_Items::skillScroll_clearData()
{
	cboxNumberOfUses->setCurrentIndex(1);
	skillScroll_leDescription->setText("No description");
	skillScroll_cboxSkillInvoked->setCurrentIndex(0);
	skillScroll_rbUsableByCharacter->setChecked(true);
	skillScroll_rbItemName->setChecked(true);
	skillScroll_updateCharacterClassList();
}

void Database_Items::switch_clearData()
{
	cboxNumberOfUses->setCurrentIndex(1);
	switch_leDescription->setText("No description");
	switch_lwSwitch->clear();
	switch_lwSwitch->addItem(ProjectData::switchVariableIdToString(0));
	switch_cbAllowField->setChecked(true);
	switch_cbAllowBattle->setChecked(true);
}

void Database_Items::weapon_clearData()
{
	rbOneHand->setChecked(true);
	weapon_leDescription->setText("No description");
	weapon_cboxSkill->setCurrentIndex(0);
	weapon_rbUsableByCharacter->setChecked(true);
	weapon_sbAttackChange->setValue(0);
	weapon_sbDefenseChange->setValue(0);
	weapon_sbWisdomChange->setValue(0);
	weapon_sbAgilityChange->setValue(0);
	weapon_cbAttackFirst->setChecked(false);
	weapon_cbAttackTwice->setChecked(false);
	weapon_cbAttackAllEnemies->setChecked(false);
	weapon_cbIgnoreEnemyEvasion->setChecked(false);
	weapon_cbCursed->setChecked(false);
	weapon_cbUsableOnTheDead->setChecked(false);
	weapon_sbMPConsumed->setValue(0);
	weapon_sbHitProbability->setValue(100);
	weapon_sbCriticalBonus->setValue(0);
	weapon_cboxBattleAnimation->setCurrentIndex(0);
	weapon_rbInflictCondition->setChecked(true);
	weapon_sbConditionEffectiveness->setValue(100);
	weapon_updateCharacterClassList();
	weapon_updateAttributeList();
	weapon_updateConditionList();
}

void Database_Items::weapon_updateCharacterClassList()
{
	QCheckBox *checkBox;
	weapon_twCharacterClassList->clear();
	if (weapon_rbUsableByCharacter->isChecked())
	{
		int numCharacters = ProjectData::characterNames.size();
		weapon_twCharacterClassList->setRowCount(numCharacters);
		for (int i = 0; i < numCharacters; ++i)
		{
			checkBox = new QCheckBox(ProjectData::characterNames[i]);
			checkBox->setChecked(true);
			weapon_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	else
	{
		int numClasses = ProjectData::classNames.size();
		weapon_twCharacterClassList->setRowCount(numClasses);
		for (int i = 0; i < numClasses; ++i)
		{
			checkBox = new QCheckBox(ProjectData::classNames[i]);
			checkBox->setChecked(true);
			weapon_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	weapon_twCharacterClassList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::weapon_updateSkillList()
{
	int numSkills = ProjectData::skillNames.size();
	weapon_cboxSkill->clear();
	weapon_cboxSkill->addItem("(NONE)");
	for (int i = 0; i < numSkills; ++i)
		weapon_cboxSkill->addItem(ProjectData::skillNames[i]);
}

void Database_Items::weapon_updateBattleAnimationList()
{
	weapon_cboxBattleAnimation->clear();
	weapon_cboxBattleAnimation->addItems(ProjectData::battleAnimationNames);
}

void Database_Items::weapon_updateAttributeList()
{
	int numAttributes = ProjectData::attributeNames.size();
	weapon_twAttributeList->clear();
	weapon_twAttributeList->setRowCount(numAttributes);
	for (int i = 0; i < numAttributes; ++i)
		weapon_twAttributeList->setCellWidget(i, 0, new QCheckBox(ProjectData::attributeNames[i]));
	weapon_twAttributeList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::weapon_updateConditionList()
{
	int numConditions = ProjectData::conditionNames.size();
	weapon_twConditionList->clear();
	weapon_twConditionList->setRowCount(numConditions);
	for (int i = 0; i < numConditions; ++i)
		weapon_twConditionList->setCellWidget(i, 0, new QCheckBox(ProjectData::conditionNames[i]));
	weapon_twConditionList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::medicine_updateCharacterClassList()
{
	QCheckBox *checkBox;
	medicine_twCharacterClassList->clear();
	if (medicine_rbUsableByCharacter->isChecked())
	{
		int numCharacters = ProjectData::characterNames.size();
		medicine_twCharacterClassList->setRowCount(numCharacters);
		for (int i = 0; i < numCharacters; ++i)
		{
			checkBox = new QCheckBox(ProjectData::characterNames[i]);
			checkBox->setChecked(true);
			medicine_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	else
	{
		int numClasses = ProjectData::classNames.size();
		medicine_twCharacterClassList->setRowCount(numClasses);
		for (int i = 0; i < numClasses; ++i)
		{
			checkBox = new QCheckBox(ProjectData::classNames[i]);
			checkBox->setChecked(true);
			medicine_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	medicine_twCharacterClassList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::medicine_updateConditionList()
{
	int numConditions = ProjectData::conditionNames.size();
	medicine_twConditionList->clear();
	medicine_twConditionList->setRowCount(numConditions);
	for (int i = 0; i < numConditions; ++i)
		medicine_twConditionList->setCellWidget(i, 0, new QCheckBox(ProjectData::conditionNames[i]));
	medicine_twConditionList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::skillBook_updateCharacterClassList()
{
	QCheckBox *checkBox;
	skillBook_twCharacterClassList->clear();
	if (skillBook_rbUsableByCharacter->isChecked())
	{
		int numCharacters = ProjectData::characterNames.size();
		skillBook_twCharacterClassList->setRowCount(numCharacters);
		for (int i = 0; i < numCharacters; ++i)
		{
			checkBox = new QCheckBox(ProjectData::characterNames[i]);
			checkBox->setChecked(true);
			skillBook_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	else
	{
		int numClasses = ProjectData::classNames.size();
		skillBook_twCharacterClassList->setRowCount(numClasses);
		for (int i = 0; i < numClasses; ++i)
		{
			checkBox = new QCheckBox(ProjectData::classNames[i]);
			checkBox->setChecked(true);
			skillBook_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	skillBook_twCharacterClassList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::skillBook_updateSkillList()
{
	int numSkills = ProjectData::skillNames.size();
	skillBook_cboxSkillLearned->clear();
	for (int i = 0; i < numSkills; ++i)
		skillBook_cboxSkillLearned->addItem(ProjectData::skillNames[i]);
}

void Database_Items::seed_updateCharacterClassList()
{
	QCheckBox *checkBox;
	seed_twCharacterClassList->clear();
	if (seed_rbUsableByCharacter->isChecked())
	{
		int numCharacters = ProjectData::characterNames.size();
		seed_twCharacterClassList->setRowCount(numCharacters);
		for (int i = 0; i < numCharacters; ++i)
		{
			checkBox = new QCheckBox(ProjectData::characterNames[i]);
			checkBox->setChecked(true);
			seed_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	else
	{
		int numClasses = ProjectData::classNames.size();
		seed_twCharacterClassList->setRowCount(numClasses);
		for (int i = 0; i < numClasses; ++i)
		{
			checkBox = new QCheckBox(ProjectData::classNames[i]);
			checkBox->setChecked(true);
			seed_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	seed_twCharacterClassList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::skillScroll_updateCharacterClassList()
{
	QCheckBox *checkBox;
	skillScroll_twCharacterClassList->clear();
	if (skillScroll_rbUsableByCharacter->isChecked())
	{
		int numCharacters = ProjectData::characterNames.size();
		skillScroll_twCharacterClassList->setRowCount(numCharacters);
		for (int i = 0; i < numCharacters; ++i)
		{
			checkBox = new QCheckBox(ProjectData::characterNames[i]);
			checkBox->setChecked(true);
			skillScroll_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	else
	{
		int numClasses = ProjectData::classNames.size();
		skillScroll_twCharacterClassList->setRowCount(numClasses);
		for (int i = 0; i < numClasses; ++i)
		{
			checkBox = new QCheckBox(ProjectData::classNames[i]);
			checkBox->setChecked(true);
			skillScroll_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	skillScroll_twCharacterClassList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::skillScroll_updateSkillList()
{
	int numSkills = ProjectData::skillNames.size();
	skillScroll_cboxSkillInvoked->clear();
	for (int i = 0; i < numSkills; ++i)
		skillScroll_cboxSkillInvoked->addItem(ProjectData::skillNames[i]);
}

void Database_Items::equipment_updateSkillList()
{
	int numSkills = ProjectData::skillNames.size();
	equipment_cboxSkill->clear();
	equipment_cboxSkill->addItem("(NONE)");
	for (int i = 0; i < numSkills; ++i)
		equipment_cboxSkill->addItem(ProjectData::skillNames[i]);
}

void Database_Items::equipment_updateCharacterClassList()
{
	QCheckBox *checkBox;
	equipment_twCharacterClassList->clear();
	if (equipment_rbUsableByCharacter->isChecked())
	{
		int numCharacters = ProjectData::characterNames.size();
		equipment_twCharacterClassList->setRowCount(numCharacters);
		for (int i = 0; i < numCharacters; ++i)
		{
			checkBox = new QCheckBox(ProjectData::characterNames[i]);
			checkBox->setChecked(true);
			equipment_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	else
	{
		int numClasses = ProjectData::classNames.size();
		equipment_twCharacterClassList->setRowCount(numClasses);
		for (int i = 0; i < numClasses; ++i)
		{
			checkBox = new QCheckBox(ProjectData::classNames[i]);
			checkBox->setChecked(true);
			equipment_twCharacterClassList->setCellWidget(i, 0, checkBox);
		}
	}
	equipment_twCharacterClassList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::equipment_updateAttributeList()
{
	int numAttributes = ProjectData::attributeNames.size();
	equipment_twResistance->clear();
	equipment_twResistance->setRowCount(numAttributes);
	for (int i = 0; i < numAttributes; ++i)
		equipment_twResistance->setCellWidget(i, 0, new QCheckBox(ProjectData::attributeNames[i]));
	equipment_twResistance->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::equipment_updateConditionList()
{
	int numConditions = ProjectData::conditionNames.size();
	equipment_twConditions->clear();
	equipment_twConditions->setRowCount(numConditions);
	for (int i = 0; i < numConditions; ++i)
		equipment_twConditions->setCellWidget(i, 0, new QCheckBox(ProjectData::conditionNames[i]));
	equipment_twConditions->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Items::updateCurrentItem()
{
	if (itemRef != NULL)
	{
		Item::WeaponItem *weaponData;
		Item::EquipmentItem *equipmentData;
		Item::MedicineItem *medicineData;
		Item::SkillBookItem *skillBookData;
		Item::SeedItem *seedData;
		Item::SkillScrollItem *skillScrollData;
		Item::SwitchItem *switchData;
		Item::EventItem *eventData;
		StorageFile storageFile;
		int itemType = cboxType->currentIndex();
		(*itemRef)->setName(leName->text());
		(*itemRef)->setPrice(sbPrice->value());
		(*itemRef)->setType(itemType);
		if (itemType == Item::TYPE_COMMONITEM)
			(*itemRef)->setDescription(commonItem_leDescription->text());
		else if (itemType == Item::TYPE_WEAPON)
		{
			weaponData = (*itemRef)->getWeaponData();
			weaponData->twoHanded = rbBothHands->isChecked();
			(*itemRef)->setDescription(weapon_leDescription->text());
			weaponData->skillID = weapon_cboxSkill->currentIndex() - 1;
			weaponData->usableByCharacters = weapon_rbUsableByCharacter->isChecked();
			weaponData->usableBy.clear();
			for (int i = 0; i < weapon_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)weapon_twCharacterClassList->cellWidget(i, 0);
				weaponData->usableBy.append(checkBox->isChecked());
			}
			weaponData->attackChange = weapon_sbAttackChange->value();
			weaponData->defenseChange = weapon_sbDefenseChange->value();
			weaponData->wisdomChange = weapon_sbWisdomChange->value();
			weaponData->agilityChange = weapon_sbAgilityChange->value();
			weaponData->alwaysAttackFirst = weapon_cbAttackFirst->isChecked();
			weaponData->attackTwice = weapon_cbAttackTwice->isChecked();
			weaponData->attackAllEnemies = weapon_cbAttackAllEnemies->isChecked();
			weaponData->ignoreEnemyEvasion = weapon_cbIgnoreEnemyEvasion->isChecked();
			weaponData->cursed = weapon_cbCursed->isChecked();
			weaponData->usableOnTheDead = weapon_cbUsableOnTheDead->isChecked();
			weaponData->battleAnimationID = weapon_cboxBattleAnimation->currentIndex();
			weaponData->amountMPConsumed = weapon_sbMPConsumed->value();
			weaponData->percentHitProbability = weapon_sbHitProbability->value();
			weaponData->percentCriticalBonus = weapon_sbCriticalBonus->value();
			weaponData->attribute.clear();
			for (int i = 0; i < weapon_twAttributeList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)weapon_twAttributeList->cellWidget(i, 0);
				weaponData->attribute.append(checkBox->isChecked());
			}
			weaponData->conditions.clear();
			for (int i = 0; i < weapon_twConditionList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)weapon_twConditionList->cellWidget(i, 0);
				weaponData->conditions.append(checkBox->isChecked());
			}
			weaponData->inflictConditions = weapon_rbInflictCondition->isChecked();
			weaponData->percentConditionEffectivenessProbability = weapon_sbConditionEffectiveness->value();
			TODO("Handle weaponData->animationSettings");
		}
		else if (itemType == Item::TYPE_SHIELD || itemType == Item::TYPE_ARMOR || itemType == Item::TYPE_HELMET || itemType == Item::TYPE_ACCESSORY)
		{
			equipmentData = (*itemRef)->getEquipmentData();
			(*itemRef)->setDescription(equipment_leDescription->text());
			equipmentData->usableByCharacters = equipment_rbUsableByCharacter->isChecked();
			equipmentData->usableBy.clear();
			for (int i = 0; i < equipment_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)equipment_twCharacterClassList->cellWidget(i, 0);
				equipmentData->usableBy.append(checkBox->isChecked());
			}
			equipmentData->attackChange = equipment_sbAttackChange->value();
			equipmentData->defenseChange = equipment_sbDefenseChange->value();
			equipmentData->wisdomChange = equipment_sbWisdomChange->value();
			equipmentData->agilityChange = equipment_sbAgilityChange->value();
			equipmentData->preventCriticalHits = equipment_cbPreventCriticalHits->isChecked();
			equipmentData->increaseEvasion = equipment_cbIncreaseEvasion->isChecked();
			equipmentData->halfMPConsumption = equipment_cbHalfMPConsumption->isChecked();
			equipmentData->cursed = equipment_cbCursed->isChecked();
			equipmentData->attributeResistance.clear();
			for (int i = 0; i < equipment_twResistance->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)equipment_twResistance->cellWidget(i, 0);
				equipmentData->attributeResistance.append(checkBox->isChecked());
			}
			equipmentData->conditions.clear();
			for (int i = 0; i < equipment_twConditions->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)equipment_twConditions->cellWidget(i, 0);
				equipmentData->conditions.append(checkBox->isChecked());
			}
			equipmentData->resistConditions = equipment_rbResistCondition->isChecked();
			equipmentData->percentConditionResistanceEffectiveness = equipment_sbResistance->value();
		}
		else if (itemType == Item::TYPE_MEDICINE)
		{
			medicineData = (*itemRef)->getMedicineData();
			medicineData->numberOfUses = cboxNumberOfUses->currentIndex();
			(*itemRef)->setDescription(medicine_leDescription->text());
			medicineData->targetSingleAlly = medicine_rbTargetSingleAlly->isChecked();
			medicineData->usableByCharacters = medicine_rbUsableByCharacter->isChecked();
			medicineData->usableBy.clear();
			for (int i = 0; i < medicine_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)medicine_twCharacterClassList->cellWidget(i, 0);
				medicineData->usableBy.append(checkBox->isChecked());
			}
			medicineData->cureCondition.clear();
			for (int i = 0; i < medicine_twConditionList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)medicine_twConditionList->cellWidget(i, 0);
				medicineData->cureCondition.append(checkBox->isChecked());
			}
			medicineData->percentRestoredHP = medicine_sbPercentHPRecovery->value();
			medicineData->amountRestoredHP = medicine_sbAmountHPRecovery->value();
			medicineData->percentRestoredMP = medicine_sbPercentMPRecovery->value();
			medicineData->amountRestoredMP = medicine_sbAmountMPRecovery->value();
			medicineData->onlyUsableOnFieldMap = medicine_cbFieldMapOnly->isChecked();
			medicineData->onlyUsableOnDeadCharacters = medicine_cbDeadCharactersOnly->isChecked();
		}
		else if (itemType == Item::TYPE_SKILLBOOK)
		{
			skillBookData = (*itemRef)->getSkillBookData();
			skillBookData->numberOfUses = cboxNumberOfUses->currentIndex();
			(*itemRef)->setDescription(skillBook_leDescription->text());
			skillBookData->usableByCharacters = skillBook_rbUsableByCharacter->isChecked();
			skillBookData->usableBy.clear();
			for (int i = 0; i < skillBook_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)skillBook_twCharacterClassList->cellWidget(i, 0);
				skillBookData->usableBy.append(checkBox->isChecked());
			}
			skillBookData->skillID = skillBook_cboxSkillLearned->currentIndex();
		}
		else if (itemType == Item::TYPE_SEED)
		{
			seedData = (*itemRef)->getSeedData();
			seedData->numberOfUses = cboxNumberOfUses->currentIndex();
			(*itemRef)->setDescription(seed_leDescription->text());
			seedData->usableByCharacters = seed_rbUsableByCharacter->isChecked();
			seedData->usableBy.clear();
			for (int i = 0; i < seed_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)seed_twCharacterClassList->cellWidget(i, 0);
				seedData->usableBy.append(checkBox->isChecked());
			}
			seedData->maxHPChange = seed_sbMaxHPChange->value();
			seedData->maxMPChange = seed_sbMaxMPChange->value();
			seedData->attackChange = seed_sbAttackChange->value();
			seedData->defenseChange = seed_sbDefenseChange->value();
			seedData->wisdomChange = seed_sbWisdomChange->value();
			seedData->agilityChange = seed_sbAgilityChange->value();
		}
		else if (itemType == Item::TYPE_SKILLSCROLL)
		{
			skillScrollData = (*itemRef)->getSkillScrollData();
			skillScrollData->numberOfUses = cboxNumberOfUses->currentIndex();
			(*itemRef)->setDescription(skillScroll_leDescription->text());
			skillScrollData->usableByCharacters = skillScroll_rbUsableByCharacter->isChecked();
			skillScrollData->usableBy.clear();
			for (int i = 0; i < skillScroll_twCharacterClassList->rowCount(); ++i)
			{
				QCheckBox *checkBox = (QCheckBox*)skillScroll_twCharacterClassList->cellWidget(i, 0);
				skillScrollData->usableBy.append(checkBox->isChecked());
			}
			skillScrollData->skillID = skillScroll_cboxSkillInvoked->currentIndex();
			skillScrollData->showItemNameMessageWhenUsed = skillScroll_rbItemName->isChecked();
		}
		else if (itemType == Item::TYPE_SWITCH)
		{
			switchData = (*itemRef)->getSwitchData();
			switchData->numberOfUses = cboxNumberOfUses->currentIndex();
			(*itemRef)->setDescription(switch_leDescription->text());
			switchData->switchID = ProjectData::switchVariableIdFromString(switch_lwSwitch->item(0)->text());
			switchData->allowFieldActivation = switch_cbAllowField->isChecked();
			switchData->allowBattleActivation = switch_cbAllowBattle->isChecked();
		}
		else if (itemType == Item::TYPE_EVENT)
		{
			eventData = (*itemRef)->getEventData();
			eventData->numberOfUses = cboxNumberOfUses->currentIndex();
			(*itemRef)->setDescription(event_leDescription->text());
			eventData->allowFieldActivation = event_cbAllowField->isChecked();
			eventData->allowBattleActivation = event_cbAllowBattle->isChecked();
			TODO("Handle eventData->event");
		}
	}
}
