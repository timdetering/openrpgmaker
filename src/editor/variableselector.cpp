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

#include <QDialog>
#include "arraysize.h"
#include "projectdata.h"
#include "variableselector.h"

VariableSelector::VariableSelector(int type, int current, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	variableType = type;
	if (variableType == TYPE_STRINGS)
	{
		setWindowTitle("Strings");
		lblVariables->setText("Strings");
	}
	else if (variableType == TYPE_NUMBERS)
	{
		setWindowTitle("Variables");
		lblVariables->setText("Variables");
	}
	else
	{
		setWindowTitle("Switches");
		lblVariables->setText("Switches");
	}
	updateGroupList();
	lwGroupList->item(current / 100)->setSelected(true);
	lwVariableList->item(current % 100)->setSelected(true);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
}

VariableSelector::~VariableSelector()
{
}

int VariableSelector::getVariableID()
{
	return lwGroupList->currentRow() * 100 + lwVariableList->currentRow();
}

void VariableSelector::on_lwGroupList_itemSelectionChanged()
{
	int currentGroup = lwGroupList->currentRow();
	int currentRow = lwVariableList->currentRow();
	lwVariableList->clear();
	for (int i = currentGroup * 100; i < (currentGroup + 1) * 100; ++i)
	{
		if (variableType == TYPE_STRINGS && i < ProjectData::numStringVariables())
			lwVariableList->addItem(ProjectData::stringVariableIdToString(i));
		else if (variableType == TYPE_NUMBERS && i < ProjectData::numNumberVariables())
			lwVariableList->addItem(ProjectData::numberVariableIdToString(i));
		else if (variableType == TYPE_SWITCHES && i < ProjectData::numSwitchVariables())
			lwVariableList->addItem(ProjectData::switchVariableIdToString(i));
	}
	if (currentRow < 0)
		currentRow = 0;
	else if (currentRow >= lwVariableList->count())
		currentRow = lwVariableList->count() - 1;
	if (lwVariableList->count() > 0)
	{
		lwVariableList->setCurrentRow(currentRow);
		lwVariableList->item(currentRow)->setSelected(true);
	}
}

void VariableSelector::on_bArraySize_clicked()
{
	ArraySize *arraySize;
	int currentSize;
	if (variableType == TYPE_STRINGS)
	{
		currentSize = ProjectData::numStringVariables();
		arraySize = new ArraySize(currentSize, 1, 5000);
		if (arraySize->exec())
			ProjectData::resizeStringVariables(arraySize->getValue());
		delete arraySize;
	}
	else if (variableType == TYPE_NUMBERS)
	{
		currentSize = ProjectData::numNumberVariables();
		arraySize = new ArraySize(currentSize, 1, 5000);
		if (arraySize->exec())
			ProjectData::resizeNumberVariables(arraySize->getValue());
		delete arraySize;
	}
	else if (variableType == TYPE_SWITCHES)
	{
		currentSize = ProjectData::numSwitchVariables();
		arraySize = new ArraySize(currentSize, 1, 5000);
		if (arraySize->exec())
			ProjectData::resizeSwitchVariables(arraySize->getValue());
		delete arraySize;
	}
	updateGroupList();
}

void VariableSelector::on_lwVariableList_itemSelectionChanged()
{
	int variableID = lwGroupList->currentRow() * 100 + lwVariableList->currentRow();
	if (variableID >= 0)
	{
		if (variableType == TYPE_STRINGS)
			leName->setText(ProjectData::stringVariableName(variableID));
		else if (variableType == TYPE_NUMBERS)
			leName->setText(ProjectData::numberVariableName(variableID));
		else if (variableType == TYPE_SWITCHES)
			leName->setText(ProjectData::switchVariableName(variableID));
	}
}

void VariableSelector::on_leName_textEdited(QString text)
{
	int variableID = lwGroupList->currentRow() * 100 + lwVariableList->currentRow();
	if (variableType == TYPE_STRINGS)
	{
		ProjectData::setStringVariableName(variableID, text);
		lwVariableList->currentItem()->setText(ProjectData::stringVariableIdToString(variableID));
	}
	else if (variableType == TYPE_NUMBERS)
	{
		ProjectData::setNumberVariableName(variableID, text);
		lwVariableList->currentItem()->setText(ProjectData::numberVariableIdToString(variableID));
	}
	else if (variableType == TYPE_SWITCHES)
	{
		ProjectData::setSwitchVariableName(variableID, text);
		lwVariableList->currentItem()->setText(ProjectData::switchVariableIdToString(variableID));
	}
}

void VariableSelector::updateGroupList()
{
	int numVariables, currentGroup = lwGroupList->currentRow();
	lwGroupList->clear();
	if (variableType == TYPE_STRINGS)
		numVariables = ProjectData::numStringVariables();
	else if (variableType == TYPE_NUMBERS)
		numVariables = ProjectData::numNumberVariables();
	else if (variableType == TYPE_SWITCHES)
		numVariables = ProjectData::numSwitchVariables();
	for (int i = 0; i < numVariables; i += 100)
	{
		if (i + 100 < numVariables)
			lwGroupList->addItem(QString("[%1 - %2]").arg(i + 1, 4, 10, QChar('0')).arg(i + 100, 4, 10, QChar('0')));
		else
			lwGroupList->addItem(QString("[%1 - %2]").arg(i + 1, 4, 10, QChar('0')).arg(numVariables, 4, 10, QChar('0')));
	}
	if (currentGroup < 0)
		currentGroup = 0;
	else if (currentGroup >= lwGroupList->count())
		currentGroup = lwGroupList->count() - 1;
	if (lwGroupList->count() > 0)
	{
		lwGroupList->setCurrentRow(currentGroup);
		lwGroupList->item(currentGroup)->setSelected(true);
	}
}
