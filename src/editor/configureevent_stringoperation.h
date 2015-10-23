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

#ifndef CONFIGUREEVENT_STRINGOPERATION_H
#define CONFIGUREEVENT_STRINGOPERATION_H

#include "ui_configureevent_stringoperation.h"

class ConfigureEvent_StringOperation : public QDialog, public Ui::ConfigureEvent_StringOperation
{
	Q_OBJECT
	public:
		enum StringToChange {SINGLE=0, RANGE, REFERENCE};
		enum Operation {SET=0, ADD, REMOVE_FIRST_CHARACTER, REMOVE_LAST_CHARACTER};
		enum Operand {VALUE=0, STRING, VARIABLE, SWITCH, PARTY_MEMBER_NAME, PARTY_MEMBER_CLASS, CHARACTER_NAME, CHARACTER_CLASS, CLASS_NAME, TERRAIN_NAME, SKILL_NAME, SKILL_DESCRIPTION, ITEM_NAME, ITEM_DESCRIPTION, ATTRIBUTE_NAME, CONDITION_NAME, SYSTEM_STRING, VEHICLE_NAME, MONSTER_NAME, MONSTER_GROUP_NAME};
		ConfigureEvent_StringOperation(QWidget *parent=0);
		~ConfigureEvent_StringOperation();
		void setStringToChange(int stringToChange);
		void setStringIDs(int string1ID, int string2ID);
		void setOperation(int operation);
		void setOperand(int operand);
		void setOperandValues(QString value1, QString value2, int operandID);
		void setIsReferencedOperand(bool referenced);
		int getStringToChange();
		int getString1ID();
		int getString2ID();
		int getOperation();
		int getOperand();
		QString getOperandValue1();
		QString getOperandValue2();
		int getOperandID();
		bool isReferencedOperand();
	protected slots:
		void on_bBrowseAttributeVariables_clicked();
		void on_bBrowseCharacterVariables_clicked();
		void on_bBrowseClassVariables_clicked();
		void on_bBrowseConditionVariables_clicked();
		void on_bBrowseEndString_clicked();
		void on_bBrowseItemVariables_clicked();
		void on_bBrowseMonsterGroupVariables_clicked();
		void on_bBrowseMonsterVariables_clicked();
		void on_bBrowseSkillVariables_clicked();
		void on_bBrowseStartString_clicked();
		void on_bBrowseStrings_clicked();
		void on_bBrowseTerrainVariables_clicked();
		void on_bBrowseVariables_clicked();
		void on_bBrowseVehicleVariables_clicked();
		void on_bPartyMember_BrowseVariables_clicked();
		void on_cboxOperand_currentIndexChanged(int index);
		void on_rbAdd_toggled(bool checked);
		void on_rbPartyMemberVar_toggled(bool checked);
		void on_rbRangeOfStrings_toggled(bool checked);
		void on_rbReferencedString_toggled(bool checked);
		void on_rbRemoveFirstCharacter_toggled(bool checked);
		void on_rbRemoveLastCharacter_toggled(bool checked);
		void on_rbSet_toggled(bool checked);
		void on_rbSpecificAttribute_toggled(bool checked);
		void on_rbSpecificCharacter_toggled(bool checked);
		void on_rbSpecificClass_toggled(bool checked);
		void on_rbSpecificCondition_toggled(bool checked);
		void on_rbSpecificItem_toggled(bool checked);
		void on_rbSpecificMonsterGroup_toggled(bool checked);
		void on_rbSpecificMonster_toggled(bool checked);
		void on_rbSpecificSkill_toggled(bool checked);
		void on_rbSpecificString_toggled(bool checked);
		void on_rbSpecificTerrain_toggled(bool checked);
		void on_rbSpecificVehicle_toggled(bool checked);
		void on_string_bBrowseStrings_clicked();
		void on_string_bBrowseVariables_clicked();
		void on_string_rbSpecificString_toggled(bool checked);
		void on_switch_bBrowseSwitches_clicked();
		void on_switch_bBrowseVariables_clicked();
		void on_switch_rbSpecificSwitch_toggled(bool checked);
		void on_variable_bBrowseReferenceVariables_clicked();
		void on_variable_bBrowseVariables_clicked();
		void on_variable_rbSpecificVariable_toggled(bool checked);
};

#endif // CONFIGUREEVENT_STRINGOPERATION_H
