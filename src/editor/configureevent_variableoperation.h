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

#ifndef CONFIGUREEVENT_VARIABLEOPERATION_H
#define CONFIGUREEVENT_VARIABLEOPERATION_H

#include "ui_configureevent_variableoperation.h"

class ConfigureEvent_VariableOperation : public QDialog, public Ui::ConfigureEvent_VariableOperation
{
	Q_OBJECT
	public:
		enum VariableToChange {SINGLE=0, RANGE, REFERENCE};
		enum Operation {SET=0, ADD, SUB, MUL, DIV, MOD};
		enum Operand {VALUE=0, VARIABLE, RANDOM_NUMBER, ITEM, PARTY_MEMBER, CHARACTER, MONSTER, TIMER_SECONDS, COMPLEX_EXPRESSION, OTHER};
		enum ItemValue {ITEM_NUM_HELD=0, ITEM_NUM_EQUIPPED, ITEM_BUY_PRICE, ITEM_SELL_PRICE, ITEM_ATTACK, ITEM_DEFENSE, ITEM_WISDOM, ITEM_AGILITY};
		enum CharacterValue {CHARACTER_LEVEL=0, CHARACTER_EXPERIENCE, CHARACTER_HP, CHARACTER_MP, CHARACTER_MAX_HP, CHARACTER_MAX_MP, CHARACTER_ATTACK, CHARACTER_DEFENSE, CHARACTER_WISDOM, CHARACTER_AGILITY, CHARACTER_WEAPON_ID, CHARACTER_SHIELD_ID, CHARACTER_ARMOR_ID, CHARACTER_HELMET_ID, CHARACTER_ACCESSORY_ID};
		enum MonsterValue {MONSTER_EXPERIENCE_GAIN=0, MONSTER_MONEY_GAIN, MONSTER_MAX_HP, MONSTER_MAX_MP, MONSTER_ATTACK, MONSTER_DEFENSE, MONSTER_WISDOM, MONSTER_AGILITY};
		enum OtherValue {OTHER_MONEY=0, OTHER_SIZE_OF_PARTY, OTHER_NUM_SAVES, OTHER_NUM_BATTLES, OTHER_NUM_VICTORIES, OTHER_NUM_DEFEATS, OTHER_NUM_ESCAPES, OTHER_MUSIC_MILLISECONDS};
		ConfigureEvent_VariableOperation(QWidget *parent=0);
		~ConfigureEvent_VariableOperation();
		void setVariableToChange(int varToChange);
		void setVariableIDs(int var1ID, int var2ID);
		void setOperation(int operation);
		void setOperand(int operand);
		void setOperandValues(long value1, long value2, int operandID, int operandValue, QString expression);
		void setIsReferencedOperand(bool referenced);
		int getVariableToChange();
		int getVariable1ID();
		int getVariable2ID();
		int getOperation();
		int getOperand();
		long getValue1();
		long getValue2();
		int getOperandID();
		int getOperandValue();
		bool isReferencedOperand();
		QString getExpression();
	protected slots:
		void on_bBrowseEndVariables_clicked();
		void on_bBrowseReferenceVariables_clicked();
		void on_bBrowseSpecificVariables_clicked();
		void on_bBrowseStartVariables_clicked();
		void on_cboxOperand_currentIndexChanged(int index);
		void on_character_bBrowseVariables_clicked();
		void on_character_rbSpecificCharacter_toggled(bool checked);
		void on_expression_bInsertSpecialValue_triggered(QAction *action);
		void on_item_bBrowseVariables_clicked();
		void on_item_cboxItems_currentIndexChanged(int index);
		void on_item_rbSpecificItem_toggled(bool checked);
		void on_monster_bBrowseVariables_clicked();
		void on_monster_rbSpecificMonster_toggled(bool checked);
		void on_partyMember_bBrowseVariables_clicked();
		void on_partyMember_rbPartyMemberVar_toggled(bool checked);
		void on_rand_bBrowseMaxVariables_clicked();
		void on_rand_bBrowseMinVariables_clicked();
		void on_rand_rbSpecificRange_toggled(bool checked);
		void on_rbRangeOfVariables_toggled(bool checked);
		void on_rbReferencedVariable_toggled(bool checked);
		void on_rbSpecificVariable_toggled(bool checked);
		void on_timer_bBrowseVariables_clicked();
		void on_timer_rbSpecificTimer_toggled(bool checked);
		void on_variable_bBrowseReferenceVariables_clicked();
		void on_variable_bBrowseVariables_clicked();
		void on_variable_rbSpecificVariable_toggled(bool checked);
};

#endif // CONFIGUREEVENT_VARIABLEOPERATION_H
