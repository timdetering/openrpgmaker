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

#include <QMenu>
#include <QAction>
#include "configureevent_variableoperation.h"
#include "getitemdialog.h"
#include "getnumberdialog.h"
#include "getpartymemberdialog.h"
#include "getrandomnumberdialog.h"
#include "projectdata.h"
#include "projectdata_item.h"
#include "variableselector.h"

ConfigureEvent_VariableOperation::ConfigureEvent_VariableOperation(QWidget *parent) : QDialog(parent)
{
	QMenu *menu = new QMenu(this);
	QMenu *subMenu;
	QAction *action;
	QString varText = ProjectData::numberVariableIdToString(0);
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	wVariableRange->setEnabled(false);
	wReferenceVariable->setEnabled(false);
	variable_wReferenceVariable->setEnabled(false);
	rand_wVariables->setEnabled(false);
	item_wVariable->setEnabled(false);
	partyMember_wVariable->setEnabled(false);
	character_wVariable->setEnabled(false);
	monster_wVariable->setEnabled(false);
	timer_wVariable->setEnabled(false);
	lblSpecificVariable->setText(varText);
	lblStartVariable->setText(varText);
	lblEndVariable->setText(varText);
	lblReferenceVariable->setText(varText);
	variable_lblVariable->setText(varText);
	variable_lblReferenceVariable->setText(varText);
	rand_lblMinVariable->setText(varText);
	rand_lblMaxVariable->setText(varText);
	item_lblVariable->setText(varText);
	partyMember_lblVariable->setText(varText);
	character_lblVariable->setText(varText);
	monster_lblVariable->setText(varText);
	timer_lblVariable->setText(varText);
	value_bValueSideButton->setSpinBox(value_sbValue);
	rand_bMinSideButton->setSpinBox(rand_sbMin);
	rand_bMaxSideButton->setSpinBox(rand_sbMax);
	timer_bTimerSideButton->setSpinBox(timer_sbTimer);
	item_cboxItems->addItems(ProjectData::itemNames);
	character_cboxCharacters->addItems(ProjectData::characterNames);
	monster_cboxMonsters->addItems(ProjectData::monsterNames);
	action = menu->addAction("Variable");
	action->setObjectName("variable");
	action = menu->addAction("Random Number");
	action->setObjectName("randomNumber");
	subMenu = menu->addMenu("Item");
	action = subMenu->addAction("Number Held");
	action->setObjectName("item_numHeld");
	action = subMenu->addAction("Number Equipped");
	action->setObjectName("item_numEquipped");
	action = subMenu->addAction("Buy Price");
	action->setObjectName("item_buyPrice");
	action = subMenu->addAction("Sell Price");
	action->setObjectName("item_sellPrice");
	action = subMenu->addAction("Attack");
	action->setObjectName("item_attack");
	action = subMenu->addAction("Defense");
	action->setObjectName("item_defense");
	action = subMenu->addAction("Wisdom");
	action->setObjectName("item_wisdom");
	action = subMenu->addAction("Agility");
	action->setObjectName("item_agility");
	subMenu = menu->addMenu("Party Member");
	action = subMenu->addAction("Level");
	action->setObjectName("partyMember_level");
	action = subMenu->addAction("Experience");
	action->setObjectName("partyMember_experience");
	action = subMenu->addAction("HP");
	action->setObjectName("partyMember_hp");
	action = subMenu->addAction("Mp");
	action->setObjectName("partyMember_mp");
	action = subMenu->addAction("Max HP");
	action->setObjectName("partyMember_maxHP");
	action = subMenu->addAction("Max MP");
	action->setObjectName("partyMember_maxMP");
	action = subMenu->addAction("Attack");
	action->setObjectName("partyMember_attack");
	action = subMenu->addAction("Defense");
	action->setObjectName("partyMember_defense");
	action = subMenu->addAction("Wisdom");
	action->setObjectName("partyMember_wisdom");
	action = subMenu->addAction("Agility");
	action->setObjectName("partyMember_agility");
	action = subMenu->addAction("Weapon ID");
	action->setObjectName("partyMember_weaponID");
	action = subMenu->addAction("Shield ID");
	action->setObjectName("partyMember_shieldID");
	action = subMenu->addAction("Armor ID");
	action->setObjectName("partyMember_armorID");
	action = subMenu->addAction("Helmet ID");
	action->setObjectName("partyMember_helmetID");
	action = subMenu->addAction("Accessory ID");
	action->setObjectName("partyMember_accessoryID");
	subMenu = menu->addMenu("Character");
	action = subMenu->addAction("Level");
	action->setObjectName("character_level");
	action = subMenu->addAction("Experience");
	action->setObjectName("character_experience");
	action = subMenu->addAction("HP");
	action->setObjectName("character_hp");
	action = subMenu->addAction("Mp");
	action->setObjectName("character_mp");
	action = subMenu->addAction("Max HP");
	action->setObjectName("character_maxHP");
	action = subMenu->addAction("Max MP");
	action->setObjectName("character_maxMP");
	action = subMenu->addAction("Attack");
	action->setObjectName("character_attack");
	action = subMenu->addAction("Defense");
	action->setObjectName("character_defense");
	action = subMenu->addAction("Wisdom");
	action->setObjectName("character_wisdom");
	action = subMenu->addAction("Agility");
	action->setObjectName("character_agility");
	action = subMenu->addAction("Weapon ID");
	action->setObjectName("character_weaponID");
	action = subMenu->addAction("Shield ID");
	action->setObjectName("character_shieldID");
	action = subMenu->addAction("Armor ID");
	action->setObjectName("character_armorID");
	action = subMenu->addAction("Helmet ID");
	action->setObjectName("character_helmetID");
	action = subMenu->addAction("Accessory ID");
	action->setObjectName("character_accessoryID");
	subMenu = menu->addMenu("Monster");
	action = subMenu->addAction("Experience Gain");
	action->setObjectName("monster_experienceGain");
	action = subMenu->addAction("Money Gain");
	action->setObjectName("monster_moneyGain");
	action = subMenu->addAction("Max HP");
	action->setObjectName("monster_maxHP");
	action = subMenu->addAction("Max MP");
	action->setObjectName("monster_maxMP");
	action = subMenu->addAction("Attack");
	action->setObjectName("monster_attack");
	action = subMenu->addAction("Defense");
	action->setObjectName("monster_defense");
	action = subMenu->addAction("Wisdom");
	action->setObjectName("monster_wisdom");
	action = subMenu->addAction("Agility");
	action->setObjectName("monster_agility");
	action = menu->addAction("Timer Seconds");
	action->setObjectName("timerSeconds");
	subMenu = menu->addMenu("Other");
	action = subMenu->addAction("Money");
	action->setObjectName("other_money");
	action = subMenu->addAction("Size of Party");
	action->setObjectName("other_sizeOfParty");
	action = subMenu->addAction("Number of Saves");
	action->setObjectName("other_numSaves");
	action = subMenu->addAction("Number of Battles");
	action->setObjectName("other_numBattles");
	action = subMenu->addAction("Number of Victories");
	action->setObjectName("other_numVictories");
	action = subMenu->addAction("Number of Defeats");
	action->setObjectName("other_numDefeats");
	action = subMenu->addAction("Number of Escapes");
	action->setObjectName("other_numEscapes");
	action = subMenu->addAction("Music Milliseconds");
	action->setObjectName("other_musicMilliseconds");
	expression_bInsertSpecialValue->setMenu(menu);
}

ConfigureEvent_VariableOperation::~ConfigureEvent_VariableOperation()
{
}

void ConfigureEvent_VariableOperation::setVariableToChange(int varToChange)
{
	if (varToChange == SINGLE)
		rbSpecificVariable->setChecked(true);
	else if (varToChange == RANGE)
		rbRangeOfVariables->setChecked(true);
	else
		rbReferencedVariable->setChecked(true);
}

void ConfigureEvent_VariableOperation::setVariableIDs(int var1ID, int var2ID)
{
	if (rbSpecificVariable->isChecked())
		lblSpecificVariable->setText(ProjectData::numberVariableIdToString(var1ID));
	else if (rbRangeOfVariables->isChecked())
	{
		lblStartVariable->setText(ProjectData::numberVariableIdToString(var1ID));
		lblEndVariable->setText(ProjectData::numberVariableIdToString(var2ID));
	}
	else
		lblReferenceVariable->setText(ProjectData::numberVariableIdToString(var1ID));
}

void ConfigureEvent_VariableOperation::setOperation(int operation)
{
	if (operation == SET)
		rbSet->setChecked(true);
	else if (operation == ADD)
		rbAdd->setChecked(true);
	else if (operation == SUB)
		rbSubtract->setChecked(true);
	else if (operation == MUL)
		rbMultiply->setChecked(true);
	else if (operation == DIV)
		rbDivide->setChecked(true);
	else
		rbModulus->setChecked(true);
}

void ConfigureEvent_VariableOperation::setOperand(int operand)
{
	cboxOperand->setCurrentIndex(operand);
}

void ConfigureEvent_VariableOperation::setOperandValues(long value1, long value2, int operandID, int operandValue, QString expression)
{
	int operand = cboxOperand->currentIndex();
	if (operand == VALUE)
		value_sbValue->setValue(operandValue);
	else if (operand == VARIABLE)
	{
		if (variable_rbSpecificVariable->isChecked())
			variable_lblVariable->setText(ProjectData::numberVariableIdToString(operandID));
		else
			variable_lblReferenceVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == RANDOM_NUMBER)
	{
		if (rand_rbSpecificRange->isChecked())
		{
			rand_sbMin->setValue(value1);
			rand_sbMax->setValue(value2);
		}
		else
		{
			rand_lblMinVariable->setText(ProjectData::numberVariableIdToString(value1));
			rand_lblMaxVariable->setText(ProjectData::numberVariableIdToString(value2));
		}
	}
	else if (operand == ITEM)
	{
		if (item_rbSpecificItem->isChecked())
			item_cboxItems->setCurrentIndex(operandID);
		else
			item_lblVariable->setText(ProjectData::numberVariableIdToString(operandID));
		if (operandValue == ITEM_NUM_HELD)
			item_rbNumHeld->setChecked(true);
		else if (operandValue == ITEM_NUM_EQUIPPED)
			item_rbNumEquipped->setChecked(true);
		else if (operandValue == ITEM_BUY_PRICE)
			item_rbBuyPrice->setChecked(true);
		else if (operandValue == ITEM_SELL_PRICE)
			item_rbSellPrice->setChecked(true);
		else if (operandValue == ITEM_ATTACK)
			item_rbAttack->setChecked(true);
		else if (operandValue == ITEM_DEFENSE)
			item_rbDefense->setChecked(true);
		else if (operandValue == ITEM_WISDOM)
			item_rbWisdom->setChecked(true);
		else if (operandValue == ITEM_AGILITY)
			item_rbAgility->setChecked(true);
	}
	else if (operand == PARTY_MEMBER)
	{
		if (operandID == 0)
			partyMember_rbPartyMember1->setChecked(true);
		else if (operandID == 1)
			partyMember_rbPartyMember2->setChecked(true);
		else if (operandID == 2)
			partyMember_rbPartyMember3->setChecked(true);
		else if (operandID == 3)
			partyMember_rbPartyMember3->setChecked(true);
		else
		{
			partyMember_rbPartyMemberVar->setChecked(true);
			partyMember_lblVariable->setText(ProjectData::numberVariableIdToString(operandID - 4));
		}
		if (operandValue == CHARACTER_LEVEL)
			partyMember_rbLevel->setChecked(true);
		else if (operandValue == CHARACTER_EXPERIENCE)
			partyMember_rbExperience->setChecked(true);
		else if (operandValue == CHARACTER_HP)
			partyMember_rbHP->setChecked(true);
		else if (operandValue == CHARACTER_MP)
			partyMember_rbMP->setChecked(true);
		else if (operandValue == CHARACTER_MAX_HP)
			partyMember_rbMaxHP->setChecked(true);
		else if (operandValue == CHARACTER_MAX_MP)
			partyMember_rbMaxMP->setChecked(true);
		else if (operandValue == CHARACTER_ATTACK)
			partyMember_rbAttack->setChecked(true);
		else if (operandValue == CHARACTER_DEFENSE)
			partyMember_rbDefense->setChecked(true);
		else if (operandValue == CHARACTER_WISDOM)
			partyMember_rbWisdom->setChecked(true);
		else if (operandValue == CHARACTER_AGILITY)
			partyMember_rbAgility->setChecked(true);
		else if (operandValue == CHARACTER_WEAPON_ID)
			partyMember_rbWeaponID->setChecked(true);
		else if (operandValue == CHARACTER_SHIELD_ID)
			partyMember_rbShieldID->setChecked(true);
		else if (operandValue == CHARACTER_ARMOR_ID)
			partyMember_rbArmorID->setChecked(true);
		else if (operandValue == CHARACTER_HELMET_ID)
			partyMember_rbHelmetID->setChecked(true);
		else if (operandValue == CHARACTER_ACCESSORY_ID)
			partyMember_rbAccessoryID->setChecked(true);
	}
	else if (operand == CHARACTER)
	{
		if (character_rbSpecificCharacter->isChecked())
			character_cboxCharacters->setCurrentIndex(operandID);
		else
			character_lblVariable->setText(ProjectData::numberVariableIdToString(operandID));
		if (operandValue == CHARACTER_LEVEL)
			character_rbLevel->setChecked(true);
		else if (operandValue == CHARACTER_EXPERIENCE)
			character_rbExperience->setChecked(true);
		else if (operandValue == CHARACTER_HP)
			character_rbHP->setChecked(true);
		else if (operandValue == CHARACTER_MP)
			character_rbMP->setChecked(true);
		else if (operandValue == CHARACTER_MAX_HP)
			character_rbMaxHP->setChecked(true);
		else if (operandValue == CHARACTER_MAX_MP)
			character_rbMaxMP->setChecked(true);
		else if (operandValue == CHARACTER_ATTACK)
			character_rbAttack->setChecked(true);
		else if (operandValue == CHARACTER_DEFENSE)
			character_rbDefense->setChecked(true);
		else if (operandValue == CHARACTER_WISDOM)
			character_rbWisdom->setChecked(true);
		else if (operandValue == CHARACTER_AGILITY)
			character_rbAgility->setChecked(true);
		else if (operandValue == CHARACTER_WEAPON_ID)
			character_rbWeaponID->setChecked(true);
		else if (operandValue == CHARACTER_SHIELD_ID)
			character_rbShieldID->setChecked(true);
		else if (operandValue == CHARACTER_ARMOR_ID)
			character_rbArmorID->setChecked(true);
		else if (operandValue == CHARACTER_HELMET_ID)
			character_rbHelmetID->setChecked(true);
		else if (operandValue == CHARACTER_ACCESSORY_ID)
			character_rbAccessoryID->setChecked(true);
	}
	else if (operand == MONSTER)
	{
		if (monster_rbSpecificMonster->isChecked())
			monster_cboxMonsters->setCurrentIndex(operandID);
		else
			monster_lblVariable->setText(ProjectData::numberVariableIdToString(operandID));
		if (operandValue == MONSTER_EXPERIENCE_GAIN)
			monster_rbExperienceGain->setChecked(true);
		else if (operandValue == MONSTER_MONEY_GAIN)
			monster_rbMoneyGain->setChecked(true);
		else if (operandValue == MONSTER_MAX_HP)
			monster_rbMaxHP->setChecked(true);
		else if (operandValue == MONSTER_MAX_MP)
			monster_rbMaxMP->setChecked(true);
		else if (operandValue == MONSTER_ATTACK)
			monster_rbAttack->setChecked(true);
		else if (operandValue == MONSTER_DEFENSE)
			monster_rbDefense->setChecked(true);
		else if (operandValue == MONSTER_WISDOM)
			monster_rbWisdom->setChecked(true);
		else if (operandValue == MONSTER_AGILITY)
			monster_rbAgility->setChecked(true);
	}
	else if (operand == TIMER_SECONDS)
	{
		if (timer_rbSpecificTimer->isChecked())
			timer_sbTimer->setValue(operandID);
		else
			timer_lblVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == COMPLEX_EXPRESSION)
		expression_leValue->setText(expression);
	else if (operand == OTHER)
	{
		if (operandValue == OTHER_MONEY)
			other_rbMoney->setChecked(true);
		else if (operandValue == OTHER_SIZE_OF_PARTY)
			other_rbSizeOfParty->setChecked(true);
		else if (operandValue == OTHER_NUM_SAVES)
			other_rbNumSaves->setChecked(true);
		else if (operandValue == OTHER_NUM_BATTLES)
			other_rbNumBattles->setChecked(true);
		else if (operandValue == OTHER_NUM_VICTORIES)
			other_rbNumVictories->setChecked(true);
		else if (operandValue == OTHER_NUM_DEFEATS)
			other_rbNumDefeats->setChecked(true);
		else if (operandValue == OTHER_NUM_ESCAPES)
			other_rbNumEscapes->setChecked(true);
		else if (operandValue == OTHER_MUSIC_MILLISECONDS)
			other_rbMusicMilliseconds->setChecked(true);
	}
}

void ConfigureEvent_VariableOperation::setIsReferencedOperand(bool referenced)
{
	int operand = cboxOperand->currentIndex();
	if (operand == VARIABLE)
	{
		if (referenced)
			variable_rbReferencedVariable->setChecked(true);
		else
			variable_rbSpecificVariable->setChecked(true);
	}
	else if (operand == RANDOM_NUMBER)
	{
		if (referenced)
			rand_rbStoredRange->setChecked(true);
		else
			rand_rbSpecificRange->setChecked(true);
	}
	else if (operand == ITEM)
	{
		if (referenced)
			item_rbReferencedItem->setChecked(true);
		else
			item_rbSpecificItem->setChecked(true);
	}
	else if (operand == CHARACTER)
	{
		if (referenced)
			character_rbReferencedCharacter->setChecked(true);
		else
			character_rbSpecificCharacter->setChecked(true);
	}
	else if (operand == MONSTER)
	{
		if (referenced)
			monster_rbReferencedMonster->setChecked(true);
		else
			monster_rbSpecificMonster->setChecked(true);
	}
	else if (operand == TIMER_SECONDS)
	{
		if (referenced)
			timer_rbReferencedTimer->setChecked(true);
		else
			timer_rbSpecificTimer->setChecked(true);
	}
}

int ConfigureEvent_VariableOperation::getVariableToChange()
{
	if (rbSpecificVariable->isChecked())
		return SINGLE;
	else if (rbRangeOfVariables->isChecked())
		return RANGE;
	return REFERENCE;
}

int ConfigureEvent_VariableOperation::getVariable1ID()
{
	if (rbSpecificVariable->isChecked())
		return ProjectData::numberVariableIdFromString(lblSpecificVariable->text());
	else if (rbRangeOfVariables->isChecked())
		return ProjectData::numberVariableIdFromString(lblStartVariable->text());
	return ProjectData::numberVariableIdFromString(lblReferenceVariable->text());
}

int ConfigureEvent_VariableOperation::getVariable2ID()
{
	if (rbRangeOfVariables->isChecked())
		return ProjectData::numberVariableIdFromString(lblEndVariable->text());
	return 0;
}

int ConfigureEvent_VariableOperation::getOperation()
{
	if (rbSet->isChecked())
		return SET;
	else if (rbAdd->isChecked())
		return ADD;
	else if (rbSubtract->isChecked())
		return SUB;
	else if (rbMultiply->isChecked())
		return MUL;
	else if (rbDivide->isChecked())
		return DIV;
	return MOD;
}

int ConfigureEvent_VariableOperation::getOperand()
{
	return cboxOperand->currentIndex();
}

long ConfigureEvent_VariableOperation::getValue1()
{
	int operand = cboxOperand->currentIndex();
	if (operand == RANDOM_NUMBER)
	{
		if (rand_rbSpecificRange->isChecked())
			return rand_sbMin->value();
		else
			return ProjectData::numberVariableIdFromString(rand_lblMinVariable->text());
	}
	return 0l;
}

long ConfigureEvent_VariableOperation::getValue2()
{
	int operand = cboxOperand->currentIndex();
	if (operand == RANDOM_NUMBER)
	{
		if (rand_rbSpecificRange->isChecked())
			return rand_sbMax->value();
		else
			return ProjectData::numberVariableIdFromString(rand_lblMaxVariable->text());
	}
	return 0l;
}

int ConfigureEvent_VariableOperation::getOperandID()
{
	int operand = cboxOperand->currentIndex();
	if (operand == VARIABLE)
	{
		if (variable_rbSpecificVariable->isChecked())
			return ProjectData::numberVariableIdFromString(variable_lblVariable->text());
		else
			return ProjectData::numberVariableIdFromString(variable_lblReferenceVariable->text());
	}
	else if (operand == ITEM)
	{
		if (item_rbSpecificItem->isChecked())
			return item_cboxItems->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(item_lblVariable->text());
	}
	else if (operand == PARTY_MEMBER)
	{
		if (partyMember_rbPartyMember1->isChecked())
			return 0;
		else if (partyMember_rbPartyMember2->isChecked())
			return 1;
		else if (partyMember_rbPartyMember3->isChecked())
			return 2;
		else if (partyMember_rbPartyMember4->isChecked())
			return 3;
		else
			return ProjectData::numberVariableIdFromString(partyMember_lblVariable->text()) + 4;
	}
	else if (operand == CHARACTER)
	{
		if (character_rbSpecificCharacter->isChecked())
			return character_cboxCharacters->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(character_lblVariable->text());
	}
	else if (operand == MONSTER)
	{
		if (monster_rbSpecificMonster->isChecked())
			return monster_cboxMonsters->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(monster_lblVariable->text());
	}
	else if (operand == TIMER_SECONDS)
	{
		if (timer_rbSpecificTimer->isChecked())
			return timer_sbTimer->value();
		else
			return ProjectData::numberVariableIdFromString(timer_lblVariable->text());
	}
	return 0;
}

int ConfigureEvent_VariableOperation::getOperandValue()
{
	int operand = cboxOperand->currentIndex();
	if (operand == VALUE)
		return value_sbValue->value();
	else if (operand == ITEM)
	{
		if (item_rbNumHeld->isChecked())
			return ITEM_NUM_HELD;
		else if (item_rbNumEquipped->isChecked())
			return ITEM_NUM_EQUIPPED;
		else if (item_rbBuyPrice->isChecked())
			return ITEM_BUY_PRICE;
		else if (item_rbSellPrice->isChecked())
			return ITEM_SELL_PRICE;
		else if (item_rbAttack->isChecked())
			return ITEM_ATTACK;
		else if (item_rbDefense->isChecked())
			return ITEM_DEFENSE;
		else if (item_rbWisdom->isChecked())
			return ITEM_WISDOM;
		else if (item_rbAgility->isChecked())
			return ITEM_AGILITY;
	}
	else if (operand == PARTY_MEMBER)
	{
		if (partyMember_rbLevel->isChecked())
			return CHARACTER_LEVEL;
		else if (partyMember_rbExperience->isChecked())
			return CHARACTER_EXPERIENCE;
		else if (partyMember_rbHP->isChecked())
			return CHARACTER_HP;
		else if (partyMember_rbMP->isChecked())
			return CHARACTER_MP;
		else if (partyMember_rbMaxHP->isChecked())
			return CHARACTER_MAX_HP;
		else if (partyMember_rbMaxMP->isChecked())
			return CHARACTER_MAX_MP;
		else if (partyMember_rbAttack->isChecked())
			return CHARACTER_ATTACK;
		else if (partyMember_rbDefense->isChecked())
			return CHARACTER_DEFENSE;
		else if (partyMember_rbWisdom->isChecked())
			return CHARACTER_WISDOM;
		else if (partyMember_rbAgility->isChecked())
			return CHARACTER_AGILITY;
		else if (partyMember_rbWeaponID->isChecked())
			return CHARACTER_WEAPON_ID;
		else if (partyMember_rbShieldID->isChecked())
			return CHARACTER_SHIELD_ID;
		else if (partyMember_rbArmorID->isChecked())
			return CHARACTER_ARMOR_ID;
		else if (partyMember_rbHelmetID->isChecked())
			return CHARACTER_HELMET_ID;
		else if (partyMember_rbAccessoryID->isChecked())
			return CHARACTER_ACCESSORY_ID;
	}
	else if (operand == CHARACTER)
	{
		if (character_rbLevel->isChecked())
			return CHARACTER_LEVEL;
		else if (character_rbExperience->isChecked())
			return CHARACTER_EXPERIENCE;
		else if (character_rbHP->isChecked())
			return CHARACTER_HP;
		else if (character_rbMP->isChecked())
			return CHARACTER_MP;
		else if (character_rbMaxHP->isChecked())
			return CHARACTER_MAX_HP;
		else if (character_rbMaxMP->isChecked())
			return CHARACTER_MAX_MP;
		else if (character_rbAttack->isChecked())
			return CHARACTER_ATTACK;
		else if (character_rbDefense->isChecked())
			return CHARACTER_DEFENSE;
		else if (character_rbWisdom->isChecked())
			return CHARACTER_WISDOM;
		else if (character_rbAgility->isChecked())
			return CHARACTER_AGILITY;
		else if (character_rbWeaponID->isChecked())
			return CHARACTER_WEAPON_ID;
		else if (character_rbShieldID->isChecked())
			return CHARACTER_SHIELD_ID;
		else if (character_rbArmorID->isChecked())
			return CHARACTER_ARMOR_ID;
		else if (character_rbHelmetID->isChecked())
			return CHARACTER_HELMET_ID;
		else if (character_rbAccessoryID->isChecked())
			return CHARACTER_ACCESSORY_ID;
	}
	else if (operand == MONSTER)
	{
		if (monster_rbExperienceGain->isChecked())
			return MONSTER_EXPERIENCE_GAIN;
		else if (monster_rbMoneyGain->isChecked())
			return MONSTER_MONEY_GAIN;
		else if (monster_rbMaxHP->isChecked())
			return MONSTER_MAX_HP;
		else if (monster_rbMaxMP->isChecked())
			return MONSTER_MAX_MP;
		else if (monster_rbAttack->isChecked())
			return MONSTER_ATTACK;
		else if (monster_rbDefense->isChecked())
			return MONSTER_DEFENSE;
		else if (monster_rbWisdom->isChecked())
			return MONSTER_WISDOM;
		else if (monster_rbAgility->isChecked())
			return MONSTER_AGILITY;
	}
	else if (operand == OTHER)
	{
		if (other_rbMoney->isChecked())
			return OTHER_MONEY;
		else if (other_rbSizeOfParty->isChecked())
			return OTHER_SIZE_OF_PARTY;
		else if (other_rbNumSaves->isChecked())
			return OTHER_NUM_SAVES;
		else if (other_rbNumBattles->isChecked())
			return OTHER_NUM_BATTLES;
		else if (other_rbNumVictories->isChecked())
			return OTHER_NUM_VICTORIES;
		else if (other_rbNumDefeats->isChecked())
			return OTHER_NUM_DEFEATS;
		else if (other_rbNumEscapes->isChecked())
			return OTHER_NUM_ESCAPES;
		else if (other_rbMusicMilliseconds->isChecked())
			return OTHER_MUSIC_MILLISECONDS;
	}
	return 0;
}

bool ConfigureEvent_VariableOperation::isReferencedOperand()
{
	int operand = cboxOperand->currentIndex();
	if (operand == VARIABLE)
		return variable_rbReferencedVariable->isChecked();
	else if (operand == RANDOM_NUMBER)
		return rand_rbStoredRange->isChecked();
	else if (operand == ITEM)
		return item_rbReferencedItem->isChecked();
	else if (operand == CHARACTER)
		return character_rbReferencedCharacter->isChecked();
	else if (operand == MONSTER)
		return monster_rbReferencedMonster->isChecked();
	else if (operand == TIMER_SECONDS)
		return timer_rbReferencedTimer->isChecked();
	return false;
}

QString ConfigureEvent_VariableOperation::getExpression()
{
	return expression_leValue->text();
}

void ConfigureEvent_VariableOperation::on_bBrowseEndVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblEndVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblEndVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_bBrowseReferenceVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblReferenceVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblReferenceVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_bBrowseSpecificVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblSpecificVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblSpecificVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_bBrowseStartVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblStartVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblStartVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_cboxOperand_currentIndexChanged(int index)
{
	if (index == VALUE)
		stackedWidget->setCurrentWidget(wValue);
	else if (index == VARIABLE)
		stackedWidget->setCurrentWidget(wVariable);
	else if (index == RANDOM_NUMBER)
		stackedWidget->setCurrentWidget(wRandomNumber);
	else if (index == ITEM)
		stackedWidget->setCurrentWidget(wItem);
	else if (index == PARTY_MEMBER)
		stackedWidget->setCurrentWidget(wPartyMember);
	else if (index == CHARACTER)
		stackedWidget->setCurrentWidget(wCharacter);
	else if (index == MONSTER)
		stackedWidget->setCurrentWidget(wMonster);
	else if (index == TIMER_SECONDS)
		stackedWidget->setCurrentWidget(wTimerSeconds);
	else if (index == COMPLEX_EXPRESSION)
		stackedWidget->setCurrentWidget(wComplexExpression);
	else if (index == OTHER)
		stackedWidget->setCurrentWidget(wOther);
}

void ConfigureEvent_VariableOperation::on_character_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(character_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		character_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_character_rbSpecificCharacter_toggled(bool checked)
{
	character_cboxCharacters->setEnabled(checked);
	character_wVariable->setEnabled(!checked);
}

void ConfigureEvent_VariableOperation::on_expression_bInsertSpecialValue_triggered(QAction *action)
{
	QString type = action->objectName();
	if (type == "variable")
	{
		VariableSelector *variableSelector;
		int varID;
		variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, 0);
		if (variableSelector->exec())
		{
			varID = variableSelector->getVariableID();
			expression_leValue->insert(QString("$variable(%1)").arg(ProjectData::numberVariableIdToString(varID, false)));
		}
		delete variableSelector;
	}
	else if (type == "randomNumber")
	{
		GetRandomNumberDialog *getDialog = new GetRandomNumberDialog(this);
		int min, max;
		bool isStored;
		if (getDialog->exec())
		{
			QString minString, maxString;
			min = getDialog->getMin();
			max = getDialog->getMax();
			isStored = getDialog->isStoredRange();
			if (isStored)
			{
				minString = QString("$variable(%1)").arg(ProjectData::numberVariableIdToString(min, false));
				maxString = QString("$variable(%1)").arg(ProjectData::numberVariableIdToString(max, false));
			}
			else
			{
				minString = QString("%1").arg(min);
				maxString = QString("%1").arg(max);
			}
			expression_leValue->insert(QString("$randomNumber(%1, %2)").arg(minString).arg(maxString));
		}
	}
	else if (type.startsWith("item"))
	{
		QStringList selectableItems = ProjectData::itemNames;
		QList<int> itemIDs;
		GetItemDialog *getItem;
		int itemID;
		bool isReferenced;
		for (int i = 0; i < selectableItems.size(); ++i)
			itemIDs.append(i);
		if (type.endsWith("attack") || type.endsWith("defense") || type.endsWith("wisdom") || type.endsWith("agility"))
		{
			ProjectData::DataReference<Item> *itemRef;
			for (int i = selectableItems.size() - 1; i >= 0; --i)
			{
				itemRef = new ProjectData::DataReference<Item>(i, __FILE__, __LINE__);
				if ((*itemRef)->getType() < Item::TYPE_WEAPON || (*itemRef)->getType() > Item::TYPE_ACCESSORY)
				{
					selectableItems.removeAt(i);
					itemIDs.removeAt(i);
				}
				delete itemRef;
			}
		}
		getItem = new GetItemDialog("Choose an Item", "Item", selectableItems, this);
		if (getItem->exec())
		{
			QString itemString;
			itemID = itemIDs[getItem->getItemID()];
			isReferenced = getItem->isItemReferencedInVariable();
			if (isReferenced)
				itemString = QString("$variable(%1)").arg(ProjectData::numberVariableIdToString(itemID, false));
			else
				itemString = QString("%1").arg(itemID);
			if (type.endsWith("numHeld"))
				expression_leValue->insert(QString("$item(%1, numHeld)").arg(itemString));
			else if (type.endsWith("numEquipped"))
				expression_leValue->insert(QString("$item(%1, numEquipped)").arg(itemString));
			else if (type.endsWith("buyPrice"))
				expression_leValue->insert(QString("$item(%1, buyPrice)").arg(itemString));
			else if (type.endsWith("sellPrice"))
				expression_leValue->insert(QString("$item(%1, sellPrice)").arg(itemString));
			else if (type.endsWith("attack"))
				expression_leValue->insert(QString("$item(%1, attack)").arg(itemString));
			else if (type.endsWith("defense"))
				expression_leValue->insert(QString("$item(%1, defense)").arg(itemString));
			else if (type.endsWith("wisdom"))
				expression_leValue->insert(QString("$item(%1, wisdom)").arg(itemString));
			else if (type.endsWith("agility"))
				expression_leValue->insert(QString("$item(%1, agility)").arg(itemString));
		}
		delete getItem;
	}
	else if (type.startsWith("partyMember"))
	{
		GetPartyMemberDialog *getDialog = new GetPartyMemberDialog(this);
		int memberID;
		bool isReferenced;
		if (getDialog->exec())
		{
			QString memberString;
			memberID = getDialog->getID();
			isReferenced = getDialog->isIDStoredInVariable();
			if (isReferenced)
				memberString = QString("$variable(%1)").arg(ProjectData::numberVariableIdToString(memberID, false));
			else
				memberString = QString("%1").arg(memberID);
			if (type.endsWith("level"))
				expression_leValue->insert(QString("$partyMember(%1, level)").arg(memberString));
			else if (type.endsWith("experience"))
				expression_leValue->insert(QString("$partyMember(%1, experience)").arg(memberString));
			else if (type.endsWith("hp"))
				expression_leValue->insert(QString("$partyMember(%1, hp)").arg(memberString));
			else if (type.endsWith("mp"))
				expression_leValue->insert(QString("$partyMember(%1, mp)").arg(memberString));
			else if (type.endsWith("maxHP"))
				expression_leValue->insert(QString("$partyMember(%1, maxHP)").arg(memberString));
			else if (type.endsWith("maxMP"))
				expression_leValue->insert(QString("$partyMember(%1, maxMP)").arg(memberString));
			else if (type.endsWith("attack"))
				expression_leValue->insert(QString("$partyMember(%1, attack)").arg(memberString));
			else if (type.endsWith("defense"))
				expression_leValue->insert(QString("$partyMember(%1, defense)").arg(memberString));
			else if (type.endsWith("wisdom"))
				expression_leValue->insert(QString("$partyMember(%1, wisdom)").arg(memberString));
			else if (type.endsWith("agility"))
				expression_leValue->insert(QString("$partyMember(%1, agility)").arg(memberString));
			else if (type.endsWith("weaponID"))
				expression_leValue->insert(QString("$partyMember(%1, weaponID)").arg(memberString));
			else if (type.endsWith("shieldID"))
				expression_leValue->insert(QString("$partyMember(%1, shieldID)").arg(memberString));
			else if (type.endsWith("armorID"))
				expression_leValue->insert(QString("$partyMember(%1, armorID)").arg(memberString));
			else if (type.endsWith("helmetID"))
				expression_leValue->insert(QString("$partyMember(%1, helmetID)").arg(memberString));
			else if (type.endsWith("accessoryID"))
				expression_leValue->insert(QString("$partyMember(%1, accessoryID)").arg(memberString));
		}
	}
	else if (type.startsWith("character"))
	{
		GetItemDialog *getCharacter = new GetItemDialog("Choose a Character", "Character", ProjectData::characterNames, this);;
		int charID;
		bool isReferenced;
		if (getCharacter->exec())
		{
			QString charString;
			charID = getCharacter->getItemID();
			isReferenced = getCharacter->isItemReferencedInVariable();
			if (isReferenced)
				charString = QString("$variable(%1)").arg(ProjectData::numberVariableIdToString(charID, false));
			else
				charString = QString("%1").arg(charID);
			if (type.endsWith("level"))
				expression_leValue->insert(QString("$character(%1, level)").arg(charString));
			else if (type.endsWith("experience"))
				expression_leValue->insert(QString("$character(%1, experience)").arg(charString));
			else if (type.endsWith("hp"))
				expression_leValue->insert(QString("$character(%1, hp)").arg(charString));
			else if (type.endsWith("mp"))
				expression_leValue->insert(QString("$character(%1, mp)").arg(charString));
			else if (type.endsWith("maxHP"))
				expression_leValue->insert(QString("$character(%1, maxHP)").arg(charString));
			else if (type.endsWith("maxMP"))
				expression_leValue->insert(QString("$character(%1, maxMP)").arg(charString));
			else if (type.endsWith("attack"))
				expression_leValue->insert(QString("$character(%1, attack)").arg(charString));
			else if (type.endsWith("defense"))
				expression_leValue->insert(QString("$character(%1, defense)").arg(charString));
			else if (type.endsWith("wisdom"))
				expression_leValue->insert(QString("$character(%1, wisdom)").arg(charString));
			else if (type.endsWith("agility"))
				expression_leValue->insert(QString("$character(%1, agility)").arg(charString));
			else if (type.endsWith("weaponID"))
				expression_leValue->insert(QString("$character(%1, weaponID)").arg(charString));
			else if (type.endsWith("shieldID"))
				expression_leValue->insert(QString("$character(%1, shieldID)").arg(charString));
			else if (type.endsWith("armorID"))
				expression_leValue->insert(QString("$character(%1, armorID)").arg(charString));
			else if (type.endsWith("helmetID"))
				expression_leValue->insert(QString("$character(%1, helmetID)").arg(charString));
			else if (type.endsWith("accessoryID"))
				expression_leValue->insert(QString("$character(%1, accessoryID)").arg(charString));
		}
	}
	else if (type.startsWith("monster"))
	{
		GetItemDialog *getMonster = new GetItemDialog("Choose a Monster", "Monster", ProjectData::monsterNames, this);;
		int monsterID;
		bool isReferenced;
		if (getMonster->exec())
		{
			QString monsterString;
			monsterID = getMonster->getItemID();
			isReferenced = getMonster->isItemReferencedInVariable();
			if (isReferenced)
				monsterString = QString("$variable(%1)").arg(ProjectData::numberVariableIdToString(monsterID, false));
			else
				monsterString = QString("%1").arg(monsterID);
			if (type.endsWith("experienceGain"))
				expression_leValue->insert(QString("$monster(%1, experienceGain)").arg(monsterString));
			else if (type.endsWith("moneyGain"))
				expression_leValue->insert(QString("$monster(%1, moneyGain)").arg(monsterString));
			else if (type.endsWith("maxHP"))
				expression_leValue->insert(QString("$monster(%1, maxHP)").arg(monsterString));
			else if (type.endsWith("maxMP"))
				expression_leValue->insert(QString("$monster(%1, maxMP)").arg(monsterString));
			else if (type.endsWith("attack"))
				expression_leValue->insert(QString("$monster(%1, attack)").arg(monsterString));
			else if (type.endsWith("defense"))
				expression_leValue->insert(QString("$monster(%1, defense)").arg(monsterString));
			else if (type.endsWith("wisdom"))
				expression_leValue->insert(QString("$monster(%1, wisdom)").arg(monsterString));
			else if (type.endsWith("agility"))
				expression_leValue->insert(QString("$monster(%1, agility)").arg(monsterString));
		}
	}
	else if (type == "other_money")
		expression_leValue->insert("$money()");
	else if (type == "other_sizeOfParty")
		expression_leValue->insert("$sizeOfParty()");
	else if (type == "other_numSaves")
		expression_leValue->insert("$numSaves()");
	else if (type == "other_numBattles")
		expression_leValue->insert("$numBattles()");
	else if (type == "other_numVictories")
		expression_leValue->insert("$numVictories()");
	else if (type == "other_numDefeats")
		expression_leValue->insert("$numDefeats()");
	else if (type == "other_numEscapes")
		expression_leValue->insert("$numEscapes()");
	else if (type == "other_musicMilliseconds")
		expression_leValue->insert("$musicMilliseconds()");
}

void ConfigureEvent_VariableOperation::on_item_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(item_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		item_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_item_cboxItems_currentIndexChanged(int index)
{
	ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>(index, __FILE__, __LINE__);
	if ((*itemRef)->getType() >= Item::TYPE_WEAPON && (*itemRef)->getType() <= Item::TYPE_ACCESSORY)
	{
		item_rbAttack->setEnabled(true);
		item_rbDefense->setEnabled(true);
		item_rbWisdom->setEnabled(true);
		item_rbAgility->setEnabled(true);
	}
	else
	{
		item_rbAttack->setEnabled(false);
		item_rbDefense->setEnabled(false);
		item_rbWisdom->setEnabled(false);
		item_rbAgility->setEnabled(false);
		if (item_rbAttack->isChecked() || item_rbDefense->isChecked() || item_rbWisdom->isChecked() || item_rbAgility->isChecked())
			item_rbNumHeld->setChecked(true);
	}
	delete itemRef;
}

void ConfigureEvent_VariableOperation::on_item_rbSpecificItem_toggled(bool checked)
{
	item_cboxItems->setEnabled(checked);
	item_wVariable->setEnabled(!checked);
}

void ConfigureEvent_VariableOperation::on_monster_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(monster_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		monster_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_monster_rbSpecificMonster_toggled(bool checked)
{
	monster_cboxMonsters->setEnabled(checked);
	monster_wVariable->setEnabled(!checked);
}

void ConfigureEvent_VariableOperation::on_partyMember_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(partyMember_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		partyMember_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_partyMember_rbPartyMemberVar_toggled(bool checked)
{
	partyMember_wVariable->setEnabled(checked);
}

void ConfigureEvent_VariableOperation::on_rand_bBrowseMaxVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(rand_lblMaxVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		rand_lblMaxVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_rand_bBrowseMinVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(rand_lblMinVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		rand_lblMinVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_rand_rbSpecificRange_toggled(bool checked)
{
	rand_wRange->setEnabled(checked);
	rand_wVariables->setEnabled(!checked);
}

void ConfigureEvent_VariableOperation::on_rbRangeOfVariables_toggled(bool checked)
{
	wVariableRange->setEnabled(checked);
}

void ConfigureEvent_VariableOperation::on_rbReferencedVariable_toggled(bool checked)
{
	wReferenceVariable->setEnabled(checked);
}

void ConfigureEvent_VariableOperation::on_rbSpecificVariable_toggled(bool checked)
{
	wSpecificVariable->setEnabled(checked);
}

void ConfigureEvent_VariableOperation::on_timer_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(timer_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		timer_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_timer_rbSpecificTimer_toggled(bool checked)
{
	timer_wTimer->setEnabled(checked);
	timer_wVariable->setEnabled(!checked);
}

void ConfigureEvent_VariableOperation::on_variable_bBrowseReferenceVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(variable_lblReferenceVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		variable_lblReferenceVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_variable_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(variable_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		variable_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_VariableOperation::on_variable_rbSpecificVariable_toggled(bool checked)
{
	variable_wVariable->setEnabled(checked);
	variable_wReferenceVariable->setEnabled(!checked);
}
