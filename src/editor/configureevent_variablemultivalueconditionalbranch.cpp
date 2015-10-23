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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configureevent_variablemultivalueconditionalbranch.h"
#include "getnumberdialog.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_VariableMultiValueConditionalBranch::ConfigureEvent_VariableMultiValueConditionalBranch(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	bConfigureValue->setEnabled(false);
	bRemoveValue->setEnabled(false);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	lblVariable->setText(ProjectData::numberVariableIdToString(0));
}

ConfigureEvent_VariableMultiValueConditionalBranch::~ConfigureEvent_VariableMultiValueConditionalBranch()
{
	for (int i = 0; i < values.size(); ++i)
	{
		for (int j = 0; j < values[i]->events.size(); ++j)
			delete (values[i]->events[j]);
		values[i]->events.clear();
		delete (values[i]);
	}
	values.clear();
}

int ConfigureEvent_VariableMultiValueConditionalBranch::getVariableID()
{
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

void ConfigureEvent_VariableMultiValueConditionalBranch::setVariableID(int value)
{
	lblVariable->setText(ProjectData::numberVariableIdToString(value));
}

bool ConfigureEvent_VariableMultiValueConditionalBranch::getExecuteCustomHandlerIfNoValueMet()
{
	return cbExecuteCustomHandler->isChecked();
}

void ConfigureEvent_VariableMultiValueConditionalBranch::setExecuteCustomHandlerIfNoValueMet(bool value)
{
	cbExecuteCustomHandler->setChecked(value);
}

void ConfigureEvent_VariableMultiValueConditionalBranch::addValues(QList<BaseEvent::VariableMultiValueConditionalBranch::VariableValue*> variableValues)
{
	for (int i = 0; i < variableValues.size(); ++i)
	{
		BaseEvent::VariableMultiValueConditionalBranch::VariableValue *value = new BaseEvent::VariableMultiValueConditionalBranch::VariableValue;
		value->value = variableValues[i]->value;
		value->valueStoredInVariable = variableValues[i]->valueStoredInVariable;
		for (int j = 0; j < variableValues[i]->events.size(); ++j)
			value->events.append(new BaseEvent(variableValues[i]->events[j]));
		values.append(value);
		if (value->valueStoredInVariable)
			lwValues->addItem(ProjectData::numberVariableIdToString(value->value));
		else
			lwValues->addItem(QString::number(value->value));
	}
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((values.size() != 0));
}

QList<BaseEvent::VariableMultiValueConditionalBranch::VariableValue*> ConfigureEvent_VariableMultiValueConditionalBranch::getValues()
{
	return values;
}

void ConfigureEvent_VariableMultiValueConditionalBranch::on_bAddValue_clicked()
{
	GetNumberDialog *getNumberDialog = new GetNumberDialog("Value", -2147483647, 2147483647, this);
	if (getNumberDialog->exec())
	{
		BaseEvent::VariableMultiValueConditionalBranch::VariableValue *value = new BaseEvent::VariableMultiValueConditionalBranch::VariableValue;
		value->value = getNumberDialog->getValue();
		value->valueStoredInVariable = getNumberDialog->isValueStoredInVariable();
		values.append(value);
		if (value->valueStoredInVariable)
			lwValues->addItem(ProjectData::numberVariableIdToString(value->value));
		else
			lwValues->addItem(QString::number(value->value));
	}
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((values.size() != 0));
}

void ConfigureEvent_VariableMultiValueConditionalBranch::on_bBrowseVariables_clicked()
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

void ConfigureEvent_VariableMultiValueConditionalBranch::on_bConfigureValue_clicked()
{
	int valueID = lwValues->currentRow();
	if (valueID != -1)
	{
		GetNumberDialog *getNumberDialog = new GetNumberDialog("Value", -2147483647, 2147483647, this);
		getNumberDialog->setValue(values[valueID]->value);
		getNumberDialog->setValueStoredInVariable(values[valueID]->valueStoredInVariable);
		if (getNumberDialog->exec())
		{
			values[valueID]->value = getNumberDialog->getValue();
			values[valueID]->valueStoredInVariable = getNumberDialog->isValueStoredInVariable();
			if (values[valueID]->valueStoredInVariable)
				lwValues->item(valueID)->setText(ProjectData::numberVariableIdToString(values[valueID]->value));
			else
				lwValues->item(valueID)->setText(QString::number(values[valueID]->value));
		}
		delete getNumberDialog;
	}
}

void ConfigureEvent_VariableMultiValueConditionalBranch::on_bRemoveValue_clicked()
{
	int valueID = lwValues->currentRow();
	if (valueID != -1)
	{
		for (int i = 0; i < values[valueID]->events.size(); ++i)
			delete (values[valueID]->events[i]);
		values[valueID]->events.clear();
		delete (values[valueID]);
		delete (lwValues->takeItem(valueID));
		bRemoveValue->setEnabled((values.size() != 0));
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled((values.size() != 0));
	}
}

void ConfigureEvent_VariableMultiValueConditionalBranch::on_lwValues_itemDoubleClicked(QListWidgetItem *item)
{
	int valueID = lwValues->row(item);
	if (valueID != -1)
	{
		GetNumberDialog *getNumberDialog = new GetNumberDialog("Value", -2147483647, 2147483647, this);
		getNumberDialog->setValue(values[valueID]->value);
		getNumberDialog->setValueStoredInVariable(values[valueID]->valueStoredInVariable);
		if (getNumberDialog->exec())
		{
			values[valueID]->value = getNumberDialog->getValue();
			values[valueID]->valueStoredInVariable = getNumberDialog->isValueStoredInVariable();
			if (values[valueID]->valueStoredInVariable)
				lwValues->item(valueID)->setText(ProjectData::numberVariableIdToString(values[valueID]->value));
			else
				lwValues->item(valueID)->setText(QString::number(values[valueID]->value));
		}
		delete getNumberDialog;
	}
}

void ConfigureEvent_VariableMultiValueConditionalBranch::on_lwValues_itemSelectionChanged()
{
	int valueID = lwValues->currentRow();
	bConfigureValue->setEnabled((valueID != -1));
	bRemoveValue->setEnabled((valueID != -1));
}
