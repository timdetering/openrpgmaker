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

#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include "configureevent_mapconditionalbranch.h"
#include "configureevent_mapconditionalbranch_configurecondition.h"

ConfigureEvent_MapConditionalBranch::ConfigureEvent_MapConditionalBranch(int map, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	bNumConditionsSideButton->setSpinBox(sbNumConditions);
	wNumConditions->setEnabled(false);
	bConfigureCondition->setEnabled(false);
	bRemoveCondition->setEnabled(false);
	mapID = map;
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

ConfigureEvent_MapConditionalBranch::~ConfigureEvent_MapConditionalBranch()
{
	for (int i = 0; i < conditions.size(); ++i)
	{
		delete (conditions[i]);
		conditions[i] = NULL;
	}
	conditions.clear();
}

void ConfigureEvent_MapConditionalBranch::setConditionsToMatch(int conditionsToMatch)
{
	if (conditionsToMatch == ALL)
		rbMatchAll->setChecked(true);
	else if (conditionsToMatch == AT_LEAST)
		rbMatchAtLeast->setChecked(true);
	else
		rbMatchNone->setChecked(true);
}

void ConfigureEvent_MapConditionalBranch::setNumConditionsToMatch(int numConditions)
{
	sbNumConditions->setValue(numConditions);
}

void ConfigureEvent_MapConditionalBranch::setExecuteCustomHandler(bool executeCustomHandler)
{
	cbExecuteCustomHandler->setChecked(executeCustomHandler);
}

void ConfigureEvent_MapConditionalBranch::addConditions(QList<BaseEvent::ConditionalBranch_Map::Condition*> values)
{
	for (int i = 0; i < values.size(); ++i)
	{
		conditions.append(new BaseEvent::ConditionalBranch_Map::Condition(values[i]));
		lwConditions->addItem(values[i]->getViewableText(0).remove(0, 2));
	}
	sbNumConditions->setMaximum(conditions.size());
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((conditions.size() != 0));
}

int ConfigureEvent_MapConditionalBranch::getConditionsToMatch()
{
	if (rbMatchAll->isChecked())
		return ALL;
	else if (rbMatchNone->isChecked())
		return NONE;
	return AT_LEAST;
}

int ConfigureEvent_MapConditionalBranch::getNumConditionsToMatch()
{
	return sbNumConditions->value();
}

bool ConfigureEvent_MapConditionalBranch::executeCustomHandler()
{
	return cbExecuteCustomHandler->isChecked();
}

QList<BaseEvent::ConditionalBranch_Map::Condition*> ConfigureEvent_MapConditionalBranch::getConditions()
{
	return conditions;
}

void ConfigureEvent_MapConditionalBranch::on_bAddCondition_clicked()
{
	ConfigureEvent_MapConditionalBranch_ConfigureCondition *configureCondition = new ConfigureEvent_MapConditionalBranch_ConfigureCondition(mapID, this);
	if (configureCondition->exec())
	{
		BaseEvent::ConditionalBranch_Map::Condition *condition = new BaseEvent::ConditionalBranch_Map::Condition();
		configureCondition->getCondition(condition);
		conditions.append(condition);
		lwConditions->addItem(condition->getViewableText(0).remove(0, 2));
	}
	delete configureCondition;
	sbNumConditions->setMaximum(conditions.size());
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((conditions.size() != 0));
}

void ConfigureEvent_MapConditionalBranch::on_bConfigureCondition_clicked()
{
	int conditionID = lwConditions->currentRow();
	if (conditionID != -1)
	{
		ConfigureEvent_MapConditionalBranch_ConfigureCondition *configureCondition = new ConfigureEvent_MapConditionalBranch_ConfigureCondition(mapID, this);
		configureCondition->setCondition(conditions[conditionID]);
		if (configureCondition->exec())
		{
			configureCondition->getCondition(conditions[conditionID]);
			lwConditions->item(conditionID)->setText(conditions[conditionID]->getViewableText(0).remove(0, 2));
		}
		delete configureCondition;
	}
}

void ConfigureEvent_MapConditionalBranch::on_bRemoveCondition_clicked()
{
	int conditionID = lwConditions->currentRow();
	if (conditionID != -1)
	{
		delete (conditions[conditionID]);
		conditions.removeAt(conditionID);
		delete (lwConditions->takeItem(conditionID));
		bRemoveCondition->setEnabled((conditions.size() != 0));
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled((conditions.size() != 0));
		sbNumConditions->setMaximum(conditions.size());
	}
}

void ConfigureEvent_MapConditionalBranch::on_lwConditions_itemDoubleClicked(QListWidgetItem *item)
{
	int conditionID = lwConditions->row(item);
	if (conditionID != -1)
	{
		ConfigureEvent_MapConditionalBranch_ConfigureCondition *configureCondition = new ConfigureEvent_MapConditionalBranch_ConfigureCondition(mapID, this);
		configureCondition->setCondition(conditions[conditionID]);
		if (configureCondition->exec())
		{
			configureCondition->getCondition(conditions[conditionID]);
			item->setText(conditions[conditionID]->getViewableText(0).remove(0, 2));
		}
		delete configureCondition;
	}
}

void ConfigureEvent_MapConditionalBranch::on_lwConditions_itemSelectionChanged()
{
	int conditionID = lwConditions->currentRow();
	bConfigureCondition->setEnabled((conditionID != -1));
	bRemoveCondition->setEnabled((conditionID != -1));
}

void ConfigureEvent_MapConditionalBranch::on_rbMatchAtLeast_toggled(bool checked)
{
	wNumConditions->setEnabled(checked);
}
