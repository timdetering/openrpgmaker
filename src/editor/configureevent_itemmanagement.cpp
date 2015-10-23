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

#include "configureevent_itemmanagement.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ItemManagement::ConfigureEvent_ItemManagement(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxItems->addItems(ProjectData::itemNames);
	lblItemVariable->setText(ProjectData::numberVariableIdToString(0));
	lblAmountVariable->setText(ProjectData::numberVariableIdToString(0));
	wItemVariable->setEnabled(false);
	wAmountVariable->setEnabled(false);
	bAmountSideButton->setSpinBox(sbAmount);
}

ConfigureEvent_ItemManagement::~ConfigureEvent_ItemManagement()
{
}

void ConfigureEvent_ItemManagement::setItemID(int itemID)
{
	if (rbSpecificItem->isChecked())
		cboxItems->setCurrentIndex(itemID);
	else
		lblItemVariable->setText(ProjectData::numberVariableIdToString(itemID));
}

void ConfigureEvent_ItemManagement::setIsReferencedItem(bool referenced)
{
	if (referenced)
		rbReferencedItem->setChecked(true);
	else
		rbSpecificItem->setChecked(true);
}

void ConfigureEvent_ItemManagement::setOperation(bool addItem)
{
	if (addItem)
		rbAddItems->setChecked(true);
	else
		rbRemoveItems->setChecked(true);
}

void ConfigureEvent_ItemManagement::setAmount(int amount)
{
	if (rbSpecificAmount->isChecked())
		sbAmount->setValue(amount);
	else
		lblAmountVariable->setText(ProjectData::numberVariableIdToString(amount));
}

void ConfigureEvent_ItemManagement::setIsStoredAmount(bool stored)
{
	if (stored)
		rbStoredAmount->setChecked(true);
	else
		rbSpecificAmount->setChecked(true);
}

int ConfigureEvent_ItemManagement::getItemID()
{
	if (rbSpecificItem->isChecked())
		return cboxItems->currentIndex();
	return ProjectData::numberVariableIdFromString(lblItemVariable->text());
}

bool ConfigureEvent_ItemManagement::isReferencedItem()
{
	return rbReferencedItem->isChecked();
}

bool ConfigureEvent_ItemManagement::isAddItemOperation()
{
	return rbAddItems->isChecked();
}

int ConfigureEvent_ItemManagement::getAmount()
{
	if (rbSpecificAmount->isChecked())
		return sbAmount->value();
	return ProjectData::numberVariableIdFromString(lblAmountVariable->text());
}

bool ConfigureEvent_ItemManagement::isStoredAmount()
{
	return rbStoredAmount->isChecked();
}

void ConfigureEvent_ItemManagement::on_bBrowseAmountVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblAmountVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblAmountVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ItemManagement::on_bBrowseItemVariables_clicked()
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

void ConfigureEvent_ItemManagement::on_rbSpecificAmount_toggled(bool checked)
{
	wAmount->setEnabled(checked);
	wAmountVariable->setEnabled(!checked);
}

void ConfigureEvent_ItemManagement::on_rbSpecificItem_toggled(bool checked)
{
	cboxItems->setEnabled(checked);
	wItemVariable->setEnabled(!checked);
}
