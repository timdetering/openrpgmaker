/****************************************************************************
 *  Open RPG Maker is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  Open RPG Maker is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with Open RPG Maker. If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 *  ---                                                                     *
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configureevent_stringoperation.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_StringOperation::ConfigureEvent_StringOperation(QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	QString switchText = ProjectData::switchVariableIdToString(0);
	QString stringText = ProjectData::stringVariableIdToString(0);
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	wStringRange->setEnabled(false);
	wVariable->setEnabled(false);
	string_wVariable->setEnabled(false);
	variable_wReferenceVariable->setEnabled(false);
	switch_wVariable->setEnabled(false);
	wPartyMember_Variable->setEnabled(false);
	wCharacterVariable->setEnabled(false);
	wClassVariable->setEnabled(false);
	wTerrainVariable->setEnabled(false);
	wSkillVariable->setEnabled(false);
	wItemVariable->setEnabled(false);
	wAttributeVariable->setEnabled(false);
	wConditionVariable->setEnabled(false);
	wVehicleVariable->setEnabled(false);
	wMonsterVariable->setEnabled(false);
	wMonsterGroupVariable->setEnabled(false);
	lblString->setText(stringText);
	lblStartString->setText(stringText);
	lblEndString->setText(stringText);
	lblVariable->setText(varText);
	string_lblString->setText(stringText);
	string_lblVariable->setText(varText);
	variable_lblVariable->setText(varText);
	variable_lblReferenceVariable->setText(varText);
	switch_lblSwitch->setText(switchText);
	switch_lblVariable->setText(varText);
	lblPartyMember_Variable->setText(varText);
	lblCharacterVariable->setText(varText);
	lblClassVariable->setText(varText);
	lblTerrainVariable->setText(varText);
	lblSkillVariable->setText(varText);
	lblItemVariable->setText(varText);
	lblAttributeVariable->setText(varText);
	lblConditionVariable->setText(varText);
	lblVehicleVariable->setText(varText);
	lblMonsterVariable->setText(varText);
	lblMonsterGroupVariable->setText(varText);
	cboxCharacters->addItems(ProjectData::characterNames);
	cboxClasses->addItems(ProjectData::classNames);
	cboxTerrain->addItems(ProjectData::terrainNames);
	cboxSkills->addItems(ProjectData::skillNames);
	cboxItems->addItems(ProjectData::itemNames);
	cboxAttributes->addItems(ProjectData::attributeNames);
	cboxConditions->addItems(ProjectData::conditionNames);
	cboxVehicles->addItems(ProjectData::vehicleNames);
	cboxMonsters->addItems(ProjectData::monsterNames);
	cboxMonsterGroups->addItems(ProjectData::monsterGroupNames);
}

ConfigureEvent_StringOperation::~ConfigureEvent_StringOperation()
{
}

void ConfigureEvent_StringOperation::setStringToChange(int stringToChange)
{
	if (stringToChange == SINGLE)
		rbSpecificString->setChecked(true);
	else if (stringToChange == RANGE)
		rbRangeOfStrings->setChecked(true);
	else
		rbReferencedString->setChecked(true);
}

void ConfigureEvent_StringOperation::setStringIDs(int string1ID, int string2ID)
{
	if (rbSpecificString->isChecked())
		lblString->setText(ProjectData::stringVariableIdToString(string1ID));
	else if (rbReferencedString->isChecked())
		lblVariable->setText(ProjectData::numberVariableIdToString(string1ID));
	else
	{
		lblStartString->setText(ProjectData::stringVariableIdToString(string1ID));
		lblEndString->setText(ProjectData::stringVariableIdToString(string2ID));
	}
}

void ConfigureEvent_StringOperation::setOperation(int operation)
{
	if (operation == SET)
		rbSet->setChecked(true);
	else if (operation == ADD)
		rbAdd->setChecked(true);
	else if (operation == REMOVE_FIRST_CHARACTER)
		rbRemoveFirstCharacter->setChecked(true);
	else
		rbRemoveLastCharacter->setChecked(true);
}

void ConfigureEvent_StringOperation::setOperand(int operand)
{
	cboxOperand->setCurrentIndex(operand);
}

void ConfigureEvent_StringOperation::setOperandValues(QString value1, QString value2, int operandID)
{
	int operand = cboxOperand->currentIndex();
	if (operand == VALUE)
		value_leValue->setText(value1);
	else if (operand == STRING)
	{
		if (string_rbSpecificString)
			string_lblString->setText(ProjectData::stringVariableIdToString(operandID));
		else
			string_lblVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == VARIABLE)
	{
		QString text = ProjectData::numberVariableIdToString(operandID);
		if (variable_rbSpecificVariable->isChecked())
			variable_lblVariable->setText(text);
		else
			variable_lblReferenceVariable->setText(text);
	}
	else if (operand == SWITCH)
	{
		if (switch_rbSpecificSwitch->isChecked())
			switch_lblSwitch->setText(ProjectData::switchVariableIdToString(operandID));
		else
			switch_lblVariable->setText(ProjectData::numberVariableIdToString(operandID));
		switch_leOnText->setText(value1);
		switch_leOffText->setText(value2);
	}
	else if (operand == PARTY_MEMBER_NAME || operand == PARTY_MEMBER_CLASS)
	{
		if (operandID == 0)
			rbPartyMember1->setChecked(true);
		else if (operandID == 1)
			rbPartyMember2->setChecked(true);
		else if (operandID == 2)
			rbPartyMember3->setChecked(true);
		else if (operandID == 3)
			rbPartyMember4->setChecked(true);
		else
		{
			rbPartyMemberVar->setChecked(true);
			lblPartyMember_Variable->setText(ProjectData::numberVariableIdToString(operandID - 4));
		}
	}
	else if (operand == CHARACTER_NAME || operand == CHARACTER_CLASS)
	{
		if (rbSpecificCharacter->isChecked())
			cboxCharacters->setCurrentIndex(operandID);
		else
			lblCharacterVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == CLASS_NAME)
	{
		if (rbSpecificClass->isChecked())
			cboxClasses->setCurrentIndex(operandID);
		else
			lblClassVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == TERRAIN_NAME)
	{
		if (rbSpecificTerrain->isChecked())
			cboxTerrain->setCurrentIndex(operandID);
		else
			lblTerrainVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == SKILL_NAME || operand == SKILL_DESCRIPTION)
	{
		if (rbSpecificSkill->isChecked())
			cboxSkills->setCurrentIndex(operandID);
		else
			lblSkillVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == ITEM_NAME || operand == ITEM_DESCRIPTION)
	{
		if (rbSpecificItem->isChecked())
			cboxItems->setCurrentIndex(operandID);
		else
			lblItemVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == ATTRIBUTE_NAME)
	{
		if (rbSpecificAttribute->isChecked())
			cboxAttributes->setCurrentIndex(operandID);
		else
			lblAttributeVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == CONDITION_NAME)
	{
		if (rbSpecificCondition->isChecked())
			cboxConditions->setCurrentIndex(operandID);
		else
			lblConditionVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == SYSTEM_STRING)
		cboxSystemStrings->setCurrentIndex(operandID);
	else if (operand == VEHICLE_NAME)
	{
		if (rbSpecificVehicle->isChecked())
			cboxVehicles->setCurrentIndex(operandID);
		else
			lblVehicleVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == MONSTER_NAME)
	{
		if (rbSpecificMonster->isChecked())
			cboxMonsters->setCurrentIndex(operandID);
		else
			lblMonsterVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
	else if (operand == MONSTER_GROUP_NAME)
	{
		if (rbSpecificMonsterGroup->isChecked())
			cboxMonsterGroups->setCurrentIndex(operandID);
		else
			lblMonsterGroupVariable->setText(ProjectData::numberVariableIdToString(operandID));
	}
}

void ConfigureEvent_StringOperation::setIsReferencedOperand(bool referenced)
{
	int operand = cboxOperand->currentIndex();
	if (operand == STRING)
	{
		if (referenced)
			string_rbReferencedString->setChecked(true);
		else
			string_rbSpecificString->setChecked(true);
	}
	else if (operand == VARIABLE)
	{
		if (referenced)
			variable_rbReferencedVariable->setChecked(true);
		else
			variable_rbSpecificVariable->setChecked(true);
	}
	else if (operand == SWITCH)
	{
		if (referenced)
			switch_rbReferencedSwitch->setChecked(true);
		else
			switch_rbSpecificSwitch->setChecked(true);
	}
	else if (operand == CHARACTER_NAME || operand == CHARACTER_CLASS)
	{
		if (referenced)
			rbReferencedCharacter->setChecked(true);
		else
			rbSpecificCharacter->setChecked(true);
	}
	else if (operand == CLASS_NAME)
	{
		if (referenced)
			rbReferencedClass->setChecked(true);
		else
			rbSpecificClass->setChecked(true);
	}
	else if (operand == TERRAIN_NAME)
	{
		if (referenced)
			rbReferencedTerrain->setChecked(true);
		else
			rbSpecificTerrain->setChecked(true);
	}
	else if (operand == SKILL_NAME || operand == SKILL_DESCRIPTION)
	{
		if (referenced)
			rbReferencedSkill->setChecked(true);
		else
			rbSpecificSkill->setChecked(true);
	}
	else if (operand == ITEM_NAME || operand == ITEM_DESCRIPTION)
	{
		if (referenced)
			rbReferencedItem->setChecked(true);
		else
			rbSpecificItem->setChecked(true);
	}
	else if (operand == ATTRIBUTE_NAME)
	{
		if (referenced)
			rbReferencedAttribute->setChecked(true);
		else
			rbSpecificAttribute->setChecked(true);
	}
	else if (operand == CONDITION_NAME)
	{
		if (referenced)
			rbReferencedCondition->setChecked(true);
		else
			rbSpecificCondition->setChecked(true);
	}
	else if (operand == VEHICLE_NAME)
	{
		if (referenced)
			rbReferencedVehicle->setChecked(true);
		else
			rbSpecificVehicle->setChecked(true);
	}
	else if (operand == MONSTER_NAME)
	{
		if (referenced)
			rbReferencedMonster->setChecked(true);
		else
			rbSpecificMonster->setChecked(true);
	}
	else if (operand == MONSTER_GROUP_NAME)
	{
		if (referenced)
			rbReferencedMonsterGroup->setChecked(true);
		else
			rbSpecificMonsterGroup->setChecked(true);
	}
}

int ConfigureEvent_StringOperation::getStringToChange()
{
	if (rbSpecificString->isChecked())
		return SINGLE;
	else if (rbRangeOfStrings->isChecked())
		return RANGE;
	return REFERENCE;
}

int ConfigureEvent_StringOperation::getString1ID()
{
	if (rbSpecificString->isChecked())
		return ProjectData::stringVariableIdFromString(lblString->text());
	else if (rbRangeOfStrings->isChecked())
		return ProjectData::stringVariableIdFromString(lblStartString->text());
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

int ConfigureEvent_StringOperation::getString2ID()
{
	if (rbRangeOfStrings->isChecked())
		return ProjectData::stringVariableIdFromString(lblEndString->text());
	return 0;
}

int ConfigureEvent_StringOperation::getOperation()
{
	if (rbSet->isChecked())
		return SET;
	else if (rbAdd->isChecked())
		return ADD;
	else if (rbRemoveFirstCharacter->isChecked())
		return REMOVE_FIRST_CHARACTER;
	return REMOVE_LAST_CHARACTER;
}

int ConfigureEvent_StringOperation::getOperand()
{
	return cboxOperand->currentIndex();
}

QString ConfigureEvent_StringOperation::getOperandValue1()
{
	int operand = cboxOperand->currentIndex();
	if (operand == VALUE)
		return value_leValue->text();
	else if (operand == SWITCH)
		return switch_leOnText->text();
	return "";
}

QString ConfigureEvent_StringOperation::getOperandValue2()
{
	int operand = cboxOperand->currentIndex();
	if (operand == SWITCH)
		return switch_leOffText->text();
	return "";
}

int ConfigureEvent_StringOperation::getOperandID()
{
	int operand = cboxOperand->currentIndex();
	if (operand == STRING)
	{
		if (rbSpecificString->isChecked())
			return ProjectData::stringVariableIdFromString(string_lblString->text());
		else
			return ProjectData::numberVariableIdFromString(string_lblVariable->text());
	}
	else if (operand == VARIABLE)
	{
		if (variable_rbSpecificVariable->isChecked())
			return ProjectData::numberVariableIdFromString(variable_lblVariable->text());
		else
			return ProjectData::numberVariableIdFromString(variable_lblReferenceVariable->text());
	}
	else if (operand == SWITCH)
	{
		if (switch_rbSpecificSwitch->isChecked())
			return ProjectData::switchVariableIdFromString(switch_lblSwitch->text());
		else
			return ProjectData::numberVariableIdFromString(switch_lblVariable->text());
	}
	else if (operand == PARTY_MEMBER_NAME || operand == PARTY_MEMBER_CLASS)
	{
		if (rbPartyMember1->isChecked())
			return 0;
		else if (rbPartyMember2->isChecked())
			return 1;
		else if (rbPartyMember3->isChecked())
			return 2;
		else if (rbPartyMember4->isChecked())
			return 3;
		else
			return ProjectData::numberVariableIdFromString(lblPartyMember_Variable->text()) + 4;
	}
	else if (operand == CHARACTER_NAME || operand == CHARACTER_CLASS)
	{
		if (rbSpecificCharacter->isChecked())
			return cboxCharacters->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblCharacterVariable->text());
	}
	else if (operand == CLASS_NAME)
	{
		if (rbSpecificClass->isChecked())
			return cboxClasses->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblClassVariable->text());
	}
	else if (operand == TERRAIN_NAME)
	{
		if (rbSpecificTerrain->isChecked())
			return cboxTerrain->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblTerrainVariable->text());
	}
	else if (operand == SKILL_NAME || operand == SKILL_DESCRIPTION)
	{
		if (rbSpecificSkill->isChecked())
			return cboxSkills->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblSkillVariable->text());
	}
	else if (operand == ITEM_NAME || operand == ITEM_DESCRIPTION)
	{
		if (rbSpecificItem->isChecked())
			return cboxItems->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblItemVariable->text());
	}
	else if (operand == ATTRIBUTE_NAME)
	{
		if (rbSpecificAttribute->isChecked())
			return cboxAttributes->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblAttributeVariable->text());
	}
	else if (operand == CONDITION_NAME)
	{
		if (rbSpecificCondition->isChecked())
			return cboxConditions->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblConditionVariable->text());
	}
	else if (operand == SYSTEM_STRING)
		return cboxSystemStrings->currentIndex();
	else if (operand == VEHICLE_NAME)
	{
		if (rbSpecificVehicle->isChecked())
			return cboxVehicles->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblVehicleVariable->text());
	}
	else if (operand == MONSTER_NAME)
	{
		if (rbSpecificMonster->isChecked())
			return cboxMonsters->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblMonsterVariable->text());
	}
	else if (operand == MONSTER_GROUP_NAME)
	{
		if (rbSpecificMonsterGroup->isChecked())
			return cboxMonsterGroups->currentIndex();
		else
			return ProjectData::numberVariableIdFromString(lblMonsterGroupVariable->text());
	}
	return 0;
}

bool ConfigureEvent_StringOperation::isReferencedOperand()
{
	int operand = cboxOperand->currentIndex();
	if (operand == STRING)
		return string_rbReferencedString->isChecked();
	else if (operand == VARIABLE)
		return variable_rbReferencedVariable->isChecked();
	else if (operand == SWITCH)
		return switch_rbReferencedSwitch->isChecked();
	else if (operand == CHARACTER_NAME || operand == CHARACTER_CLASS)
		return rbReferencedCharacter->isChecked();
	else if (operand == CLASS_NAME)
		return rbReferencedClass->isChecked();
	else if (operand == TERRAIN_NAME)
		return rbReferencedTerrain->isChecked();
	else if (operand == SKILL_NAME || operand == SKILL_DESCRIPTION)
		return rbReferencedSkill->isChecked();
	else if (operand == ITEM_NAME || operand == ITEM_DESCRIPTION)
		return rbReferencedItem->isChecked();
	else if (operand == ATTRIBUTE_NAME)
		return rbReferencedAttribute->isChecked();
	else if (operand == CONDITION_NAME)
		return rbReferencedCondition->isChecked();
	else if (operand == VEHICLE_NAME)
		return rbReferencedVehicle->isChecked();
	else if (operand == MONSTER_NAME)
		return rbReferencedMonster->isChecked();
	else if (operand == MONSTER_GROUP_NAME)
		return rbReferencedMonsterGroup->isChecked();
	return false;
}

void ConfigureEvent_StringOperation::on_bBrowseAttributeVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblAttributeVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblAttributeVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bBrowseCharacterVariables_clicked()
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

void ConfigureEvent_StringOperation::on_bBrowseClassVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblClassVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblClassVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bBrowseConditionVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblConditionVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblConditionVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bBrowseEndString_clicked()
{
	VariableSelector *variableSelector;
	int stringID = ProjectData::stringVariableIdFromString(lblEndString->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_STRINGS, stringID);
	if (variableSelector->exec())
	{
		stringID = variableSelector->getVariableID();
		lblEndString->setText(ProjectData::stringVariableIdToString(stringID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bBrowseItemVariables_clicked()
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

void ConfigureEvent_StringOperation::on_bBrowseMonsterGroupVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblMonsterGroupVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblMonsterGroupVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bBrowseMonsterVariables_clicked()
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

void ConfigureEvent_StringOperation::on_bBrowseSkillVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblSkillVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblSkillVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bBrowseStartString_clicked()
{
	VariableSelector *variableSelector;
	int stringID = ProjectData::stringVariableIdFromString(lblStartString->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_STRINGS, stringID);
	if (variableSelector->exec())
	{
		stringID = variableSelector->getVariableID();
		lblStartString->setText(ProjectData::stringVariableIdToString(stringID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bBrowseStrings_clicked()
{
	VariableSelector *variableSelector;
	int stringID = ProjectData::stringVariableIdFromString(lblString->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_STRINGS, stringID);
	if (variableSelector->exec())
	{
		stringID = variableSelector->getVariableID();
		lblString->setText(ProjectData::stringVariableIdToString(stringID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bBrowseTerrainVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTerrainVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTerrainVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bBrowseVariables_clicked()
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

void ConfigureEvent_StringOperation::on_bBrowseVehicleVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblVehicleVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblVehicleVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_bPartyMember_BrowseVariables_clicked()
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

void ConfigureEvent_StringOperation::on_cboxOperand_currentIndexChanged(int index)
{
	if (index == VALUE)
		stackedWidget->setCurrentWidget(wValue);
	else if (index == STRING)
		stackedWidget->setCurrentWidget(wString_2);
	else if (index == VARIABLE)
		stackedWidget->setCurrentWidget(wVariable_2);
	else if (index == SWITCH)
		stackedWidget->setCurrentWidget(wSwitch);
	else if (index == PARTY_MEMBER_NAME || index == PARTY_MEMBER_CLASS)
		stackedWidget->setCurrentWidget(wPartyMember);
	else if (index == CHARACTER_NAME || index == CHARACTER_CLASS)
		stackedWidget->setCurrentWidget(wCharacter);
	else if (index == CLASS_NAME)
		stackedWidget->setCurrentWidget(wClass);
	else if (index == TERRAIN_NAME)
		stackedWidget->setCurrentWidget(wTerrain);
	else if (index == SKILL_NAME || index == SKILL_DESCRIPTION)
		stackedWidget->setCurrentWidget(wSkill);
	else if (index == ITEM_NAME || index == ITEM_DESCRIPTION)
		stackedWidget->setCurrentWidget(wItem);
	else if (index == ATTRIBUTE_NAME)
		stackedWidget->setCurrentWidget(wAttribute);
	else if (index == CONDITION_NAME)
		stackedWidget->setCurrentWidget(wCondition);
	else if (index == SYSTEM_STRING)
		stackedWidget->setCurrentWidget(wSystemString);
	else if (index == VEHICLE_NAME)
		stackedWidget->setCurrentWidget(wVehicle);
	else if (index == MONSTER_NAME)
		stackedWidget->setCurrentWidget(wMonster);
	else if (index == MONSTER_GROUP_NAME)
		stackedWidget->setCurrentWidget(wMonsterGroup);
}

void ConfigureEvent_StringOperation::on_rbAdd_toggled(bool checked)
{
	if (checked)
	{
		wOperand->setEnabled(true);
		stackedWidget->setEnabled(true);
	}
}

void ConfigureEvent_StringOperation::on_rbPartyMemberVar_toggled(bool checked)
{
	wPartyMember_Variable->setEnabled(checked);
}

void ConfigureEvent_StringOperation::on_rbRangeOfStrings_toggled(bool checked)
{
	wStringRange->setEnabled(checked);
}

void ConfigureEvent_StringOperation::on_rbReferencedString_toggled(bool checked)
{
	wVariable->setEnabled(checked);
}

void ConfigureEvent_StringOperation::on_rbRemoveFirstCharacter_toggled(bool checked)
{
	if (checked)
	{
		wOperand->setEnabled(false);
		stackedWidget->setEnabled(false);
	}
}

void ConfigureEvent_StringOperation::on_rbRemoveLastCharacter_toggled(bool checked)
{
	if (checked)
	{
		wOperand->setEnabled(false);
		stackedWidget->setEnabled(false);
	}
}

void ConfigureEvent_StringOperation::on_rbSet_toggled(bool checked)
{
	if (checked)
	{
		wOperand->setEnabled(true);
		stackedWidget->setEnabled(true);
	}
}

void ConfigureEvent_StringOperation::on_rbSpecificAttribute_toggled(bool checked)
{
	cboxAttributes->setEnabled(checked);
	wAttributeVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificCharacter_toggled(bool checked)
{
	cboxCharacters->setEnabled(checked);
	wCharacterVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificClass_toggled(bool checked)
{
	cboxClasses->setEnabled(checked);
	wClassVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificCondition_toggled(bool checked)
{
	cboxConditions->setEnabled(checked);
	wConditionVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificItem_toggled(bool checked)
{
	cboxItems->setEnabled(checked);
	wItemVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificMonsterGroup_toggled(bool checked)
{
	cboxMonsterGroups->setEnabled(checked);
	wMonsterGroupVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificMonster_toggled(bool checked)
{
	cboxMonsters->setEnabled(checked);
	wMonsterVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificSkill_toggled(bool checked)
{
	cboxSkills->setEnabled(checked);
	wSkillVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificString_toggled(bool checked)
{
	wString->setEnabled(checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificTerrain_toggled(bool checked)
{
	cboxTerrain->setEnabled(checked);
	wTerrainVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_rbSpecificVehicle_toggled(bool checked)
{
	cboxVehicles->setEnabled(checked);
	wVehicleVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_string_bBrowseStrings_clicked()
{
	VariableSelector *variableSelector;
	int stringID = ProjectData::stringVariableIdFromString(string_lblString->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_STRINGS, stringID);
	if (variableSelector->exec())
	{
		stringID = variableSelector->getVariableID();
		string_lblString->setText(ProjectData::stringVariableIdToString(stringID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_string_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(string_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		string_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_string_rbSpecificString_toggled(bool checked)
{
	string_wString->setEnabled(checked);
	string_wVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_switch_bBrowseSwitches_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(switch_lblSwitch->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		switchID = variableSelector->getVariableID();
		switch_lblSwitch->setText(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_switch_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(switch_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		switch_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_StringOperation::on_switch_rbSpecificSwitch_toggled(bool checked)
{
	switch_wSwitch->setEnabled(checked);
	switch_wVariable->setEnabled(!checked);
}

void ConfigureEvent_StringOperation::on_variable_bBrowseReferenceVariables_clicked()
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

void ConfigureEvent_StringOperation::on_variable_bBrowseVariables_clicked()
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

void ConfigureEvent_StringOperation::on_variable_rbSpecificVariable_toggled(bool checked)
{
	variable_wVariable->setEnabled(checked);
	variable_wReferenceVariable->setEnabled(!checked);
}
