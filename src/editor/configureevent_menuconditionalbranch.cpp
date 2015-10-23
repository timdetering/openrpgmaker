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
#include "configureevent_menuconditionalbranch.h"
#include "configureevent_menuconditionalbranch_configurecondition.h"

ConfigureEvent_MenuConditionalBranch::ConfigureEvent_MenuConditionalBranch(int menu, int type, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	bNumConditionsSideButton->setSpinBox(sbNumConditions);
	wNumConditions->setEnabled(false);
	bConfigureCondition->setEnabled(false);
	bRemoveCondition->setEnabled(false);
	menuID = menu;
	menuType = type;
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

ConfigureEvent_MenuConditionalBranch::~ConfigureEvent_MenuConditionalBranch()
{
	for (int i = 0; i < conditions.size(); ++i)
	{
		delete (conditions[i]);
		conditions[i] = NULL;
	}
	conditions.clear();
}

void ConfigureEvent_MenuConditionalBranch::setConditionsToMatch(int conditionsToMatch)
{
	if (conditionsToMatch == ALL)
		rbMatchAll->setChecked(true);
	else if (conditionsToMatch == AT_LEAST)
		rbMatchAtLeast->setChecked(true);
	else
		rbMatchNone->setChecked(true);
}

void ConfigureEvent_MenuConditionalBranch::setNumConditionsToMatch(int numConditions)
{
	sbNumConditions->setValue(numConditions);
}

void ConfigureEvent_MenuConditionalBranch::setExecuteCustomHandler(bool executeCustomHandler)
{
	cbExecuteCustomHandler->setChecked(executeCustomHandler);
}

void ConfigureEvent_MenuConditionalBranch::addConditions(QList<BaseEvent::ConditionalBranch_Menu::Condition*> values)
{
	for (int i = 0; i < values.size(); ++i)
	{
		conditions.append(new BaseEvent::ConditionalBranch_Menu::Condition(values[i]));
		lwConditions->addItem(values[i]->getViewableText(0).remove(0, 2));
	}
	sbNumConditions->setMaximum(conditions.size());
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((conditions.size() != 0));
}

int ConfigureEvent_MenuConditionalBranch::getConditionsToMatch()
{
	if (rbMatchAll->isChecked())
		return ALL;
	else if (rbMatchNone->isChecked())
		return NONE;
	return AT_LEAST;
}

int ConfigureEvent_MenuConditionalBranch::getNumConditionsToMatch()
{
	return sbNumConditions->value();
}

bool ConfigureEvent_MenuConditionalBranch::executeCustomHandler()
{
	return cbExecuteCustomHandler->isChecked();
}

QList<BaseEvent::ConditionalBranch_Menu::Condition*> ConfigureEvent_MenuConditionalBranch::getConditions()
{
	return conditions;
}

void ConfigureEvent_MenuConditionalBranch::on_bAddCondition_clicked()
{
	ConfigureEvent_MenuConditionalBranch_ConfigureCondition *configureCondition = new ConfigureEvent_MenuConditionalBranch_ConfigureCondition(menuID, menuType, this);
	if (configureCondition->exec())
	{
		BaseEvent::ConditionalBranch_Menu::Condition *condition = new BaseEvent::ConditionalBranch_Menu::Condition();
		configureCondition->getCondition(condition);
		conditions.append(condition);
		lwConditions->addItem(condition->getViewableText(0).remove(0, 2));
	}
	sbNumConditions->setMaximum(conditions.size());
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((conditions.size() != 0));
}

void ConfigureEvent_MenuConditionalBranch::on_bConfigureCondition_clicked()
{
	int conditionID = lwConditions->currentRow();
	if (conditionID != -1)
	{
		ConfigureEvent_MenuConditionalBranch_ConfigureCondition *configureCondition = new ConfigureEvent_MenuConditionalBranch_ConfigureCondition(menuID, menuType, this);
		configureCondition->setCondition(conditions[conditionID]);
		if (configureCondition->exec())
		{
			configureCondition->getCondition(conditions[conditionID]);
			lwConditions->item(conditionID)->setText(conditions[conditionID]->getViewableText(0).remove(0, 2));
		}
	}
}

void ConfigureEvent_MenuConditionalBranch::on_bRemoveCondition_clicked()
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

void ConfigureEvent_MenuConditionalBranch::on_lwConditions_itemDoubleClicked(QListWidgetItem *item)
{
	int conditionID = lwConditions->row(item);
	if (conditionID != -1)
	{
		ConfigureEvent_MenuConditionalBranch_ConfigureCondition *configureCondition = new ConfigureEvent_MenuConditionalBranch_ConfigureCondition(menuID, menuType, this);
		configureCondition->setCondition(conditions[conditionID]);
		if (configureCondition->exec())
		{
			configureCondition->getCondition(conditions[conditionID]);
			item->setText(conditions[conditionID]->getViewableText(0).remove(0, 2));
		}
	}
}

void ConfigureEvent_MenuConditionalBranch::on_lwConditions_itemSelectionChanged()
{
	int conditionID = lwConditions->currentRow();
	bConfigureCondition->setEnabled((conditionID != -1));
	bRemoveCondition->setEnabled((conditionID != -1));
}

void ConfigureEvent_MenuConditionalBranch::on_rbMatchAtLeast_toggled(bool checked)
{
	wNumConditions->setEnabled(checked);
}
