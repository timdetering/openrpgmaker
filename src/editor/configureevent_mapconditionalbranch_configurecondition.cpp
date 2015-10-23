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

#include <QPushButton>
#include "configureevent_mapconditionalbranch_configurecondition.h"
#include "projectdata.h"
#include "projectdata_baseevent.h"
#include "projectdata_item.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"
#include "variableselector.h"

QList<int> ConfigureEvent_MapConditionalBranch_ConfigureCondition::sprite_directionIDs = QList<int>() << MapEvent::DIRECTION_UP << MapEvent::DIRECTION_RIGHT << MapEvent::DIRECTION_DOWN << MapEvent::DIRECTION_LEFT;

ConfigureEvent_MapConditionalBranch_ConfigureCondition::ConfigureEvent_MapConditionalBranch_ConfigureCondition(int mapID, QWidget *parent) : QDialog(parent)
{
	QList<MenuObject*> menuObjects;
	ProjectData::DataReference<Map> *mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	QString varText = ProjectData::numberVariableIdToString(0);
	QString switchText = ProjectData::switchVariableIdToString(0);
	QString stringText = ProjectData::stringVariableIdToString(0);
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	variable_bValueSideButton->setSpinBox(variable_sbValue);
	timer_bTimerSideButton->setSpinBox(timer_sbTimer);
	timer_bMinutesSideButton->setSpinBox(timer_sbMinutes);
	timer_bSecondsSideButton->setSpinBox(timer_sbSeconds);
	money_bValueSideButton->setSpinBox(money_sbValue);
	herolevel_bValueSideButton->setSpinBox(herolevel_sbValue);
	herohp_bValueSideButton->setSpinBox(herohp_sbValue);
	item_cboxItems->addItems(ProjectData::itemNames);
	riding_cboxVehicles->addItems(ProjectData::vehicleNames);
	heroskill_cboxSkills->addItems(ProjectData::skillNames);
	heroitem_cboxItems->addItems(getEquipItems());
	herocondition_cboxConditions->addItems(ProjectData::conditionNames);
	for (int i = 0; i < (*mapRef)->numEvents(); ++i)
		sprite_cboxEvents->addItem((*mapRef)->getEvent(i)->getName());
	sprite_cboxVehicles->addItems(ProjectData::vehicleNames);
	switch_lblSwitch->setText(switchText);
	switch_lblVariable->setText(varText);
	variable_lblVariable->setText(varText);
	variable_lblValueVariable->setText(varText);
	sprite_lblVehicleVariable->setText(varText);
	sprite_lblEventVariable->setText(varText);
	string_lblString->setText(stringText);
	string_lblVariable->setText(varText);
	string_lblValueString->setText(stringText);
	string_lblReferencedString->setText(varText);
	timer_lblTimerVariable->setText(varText);
	timer_lblTimeVariable->setText(varText);
	money_lblVariable->setText(varText);
	item_lblVariable->setText(varText);
	riding_lblVariable->setText(varText);
	hero_lblVariable->setText(varText);
	heroname_lblString->setText(stringText);
	heroname_lblVariable->setText(varText);
	herolevel_lblVariable->setText(varText);
	herohp_lblVariable->setText(varText);
	heroskill_lblVariable->setText(varText);
	heroitem_lblVariable->setText(varText);
	herocondition_lblVariable->setText(varText);
	sprite_wVehicleVariable->setEnabled(false);
	sprite_wEventVariable->setEnabled(false);
	switch_wVariable->setEnabled(false);
	variable_wValueVariable->setEnabled(false);
	string_wVariable->setEnabled(false);
	string_wValueString->setEnabled(false);
	string_wReferencedString->setEnabled(false);
	timer_wTimerVariable->setEnabled(false);
	timer_wTimeVariable->setEnabled(false);
	money_wVariable->setEnabled(false);
	item_wVariable->setEnabled(false);
	riding_wVariable->setEnabled(false);
	hero_wVariable->setEnabled(false);
	heroname_wStrings->setEnabled(false);
	heroname_wVariable->setEnabled(false);
	herolevel_wVariable->setEnabled(false);
	herohp_wVariable->setEnabled(false);
	heroskill_wVariable->setEnabled(false);
	heroitem_wVariable->setEnabled(false);
	herocondition_wVariable->setEnabled(false);
}

ConfigureEvent_MapConditionalBranch_ConfigureCondition::~ConfigureEvent_MapConditionalBranch_ConfigureCondition()
{
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::setCondition(BaseEvent::ConditionalBranch_Map::Condition *condition)
{
	int conditionType = condition->getType();
	if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::SWITCH)
	{
		BaseEvent::ConditionalBranch_Map::SwitchCondition switchCondition;
		condition->getSwitchCondition(&switchCondition);
		rbSwitch->setChecked(true);
		if (switchCondition.switchReferencedInVariable)
		{
			switch_rbReferencedSwitch->setChecked(true);
			switch_lblVariable->setText(ProjectData::switchVariableIdToString(switchCondition.switchID));
		}
		else
		{
			switch_rbSpecificSwitch->setChecked(true);
			switch_lblSwitch->setText(ProjectData::switchVariableIdToString(switchCondition.switchID));
		}
		if (switchCondition.isOn)
			switch_rbOn->setChecked(true);
		else
			switch_rbOff->setChecked(true);
	}
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::VARIABLE)
	{
		BaseEvent::ConditionalBranch_Map::VariableCondition variableCondition;
		condition->getVariableCondition(&variableCondition);
		rbVariable->setChecked(true);
		variable_lblVariable->setText(ProjectData::numberVariableIdToString(variableCondition.srcVariableID));
		variable_cboxComparison->setCurrentIndex((int)variableCondition.comparison);
		if (variableCondition.dstValueStoredInVariable)
		{
			variable_rbStoredValue->setChecked(true);
			variable_lblValueVariable->setText(ProjectData::numberVariableIdToString(variableCondition.dstValue));
		}
		else
		{
			variable_rbSpecificValue->setChecked(true);
			variable_sbValue->setValue(variableCondition.dstValue);
		}
	}
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::STRING)
	{
		BaseEvent::ConditionalBranch_Map::StringCondition stringCondition;
		condition->getStringCondition(&stringCondition);
		rbString->setChecked(true);
		if (stringCondition.srcStringReferencedInVariable)
		{
			string_rbReferencedString->setChecked(true);
			string_lblVariable->setText(ProjectData::numberVariableIdToString(stringCondition.srcStringID));
		}
		else
		{
			string_rbSpecificString->setChecked(true);
			string_lblString->setText(ProjectData::stringVariableIdToString(stringCondition.srcStringID));
		}
		string_cboxComparison->setCurrentIndex(stringCondition.comparison);
		if (stringCondition.dstStringType == BaseEvent::ConditionalBranch_Map::StringCondition::STRING_VARIABLE)
		{
			string_rbValueInString->setChecked(true);
			string_lblValueString->setText(ProjectData::stringVariableIdToString(stringCondition.dstStringID));
		}
		else if (stringCondition.dstStringType == BaseEvent::ConditionalBranch_Map::StringCondition::REFERENCED_STRING)
		{
			string_rbValueInReferencedString->setChecked(true);
			string_lblReferencedString->setText(ProjectData::numberVariableIdToString(stringCondition.dstStringID));
		}
		else
		{
			string_rbSpecificValue->setChecked(true);
			string_leValue->setText(stringCondition.dstValue);
		}
	}
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::TIMER)
	{
		BaseEvent::ConditionalBranch_Map::TimerCondition timerCondition;
		condition->getTimerCondition(&timerCondition);
		rbTimer->setChecked(true);
		if (timerCondition.timerReferencedInVariable)
		{
			timer_rbReferencedTimer->setChecked(true);
			timer_lblTimerVariable->setText(ProjectData::numberVariableIdToString(timerCondition.timerID));
		}
		else
		{
			timer_rbSpecificTimer->setChecked(true);
			timer_sbTimer->setValue(timerCondition.timerID + 1);
		}
		timer_cboxComparison->setCurrentIndex(timerCondition.comparison);
		if (timerCondition.timeStoredInVariable)
		{
			timer_rbStoredTime->setChecked(true);
			timer_lblTimeVariable->setText(ProjectData::numberVariableIdToString(timerCondition.seconds));
		}
		else
		{
			timer_rbSpecificTime->setChecked(true);
			timer_sbMinutes->setValue(timerCondition.minutes);
			timer_sbSeconds->setValue(timerCondition.seconds);
		}
	}
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::MONEY)
	{
		BaseEvent::ConditionalBranch_Map::MoneyCondition moneyCondition;
		condition->getMoneyCondition(&moneyCondition);
		rbMoney->setChecked(true);
		money_cboxComparison->setCurrentIndex(moneyCondition.comparison);
		if (moneyCondition.valueStoredInVariable)
		{
			money_rbStoredValue->setChecked(true);
			money_lblVariable->setText(ProjectData::numberVariableIdToString(moneyCondition.value));
		}
		else
		{
			money_rbSpecificValue->setChecked(true);
			money_sbValue->setValue(moneyCondition.value);
		}
	}
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::ITEM)
	{
		BaseEvent::ConditionalBranch_Map::ItemCondition itemCondition;
		condition->getItemCondition(&itemCondition);
		rbItem->setChecked(true);
		if (itemCondition.partyPossesses)
			item_rbPartyPossesses->setChecked(true);
		else
			item_rbPartyDoesNotPossess->setChecked(true);
		if (itemCondition.itemReferencedInVariable)
		{
			item_rbReferencedItem->setChecked(true);
			item_lblVariable->setText(ProjectData::numberVariableIdToString(itemCondition.itemID));
		}
		else
		{
			item_rbSpecificItem->setChecked(true);
			item_cboxItems->setCurrentIndex(itemCondition.itemID);
		}
	}
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::HERO)
	{
		BaseEvent::ConditionalBranch_Map::HeroCondition heroCondition;
		condition->getHeroCondition(&heroCondition);
		int heroConditionType = heroCondition.conditionType;
		rbHero->setChecked(true);
		if (heroCondition.characterReferencedInVariable)
		{
			hero_rbReferencedCharacter->setChecked(true);
			hero_lblVariable->setText(ProjectData::numberVariableIdToString(heroCondition.characterID));
		}
		else
		{
			hero_rbSpecificCharacter->setChecked(true);
			hero_cboxCharacters->setCurrentIndex(heroCondition.characterID);
		}
		hero_cboxCondition->setCurrentIndex(heroConditionType);
		if (heroConditionType == HERO_NAME_IS)
		{
			if (!heroCondition.string.isEmpty())
			{
				heroname_rbSpecificValue->setChecked(true);
				heroname_leValue->setText(heroCondition.string);
			}
			else if (heroCondition.valueStoredInVariable)
			{
				heroname_rbValueInReferencedString->setChecked(true);
				heroname_lblVariable->setText(ProjectData::numberVariableIdToString(heroCondition.value));
			}
			else
			{
				heroname_rbValueInString->setChecked(true);
				heroname_lblString->setText(ProjectData::stringVariableIdToString(heroCondition.value));
			}
		}
		else if (heroConditionType == HERO_LEVEL_IS_AT_LEAST)
		{
			if (heroCondition.valueStoredInVariable)
			{
				herolevel_rbStoredValue->setChecked(true);
				herolevel_lblVariable->setText(ProjectData::numberVariableIdToString(heroCondition.value));
			}
			else
			{
				herolevel_rbSpecificValue->setChecked(true);
				herolevel_sbValue->setValue(heroCondition.value);
			}
		}
		else if (heroConditionType == HERO_HP_IS_AT_LEAST)
		{
			if (heroCondition.valueStoredInVariable)
			{
				herohp_rbStoredValue->setChecked(true);
				herohp_lblVariable->setText(ProjectData::numberVariableIdToString(heroCondition.value));
			}
			else
			{
				herohp_rbSpecificValue->setChecked(true);
				herohp_sbValue->setValue(heroCondition.value);
			}
		}
		else if (heroConditionType == HERO_CAN_USE_SKILL)
		{
			if (heroCondition.valueStoredInVariable)
			{
				heroskill_rbReferencedSkill->setChecked(true);
				heroskill_lblVariable->setText(ProjectData::numberVariableIdToString(heroCondition.value));
			}
			else
			{
				heroskill_rbSpecificSkill->setChecked(true);
				heroskill_cboxSkills->setCurrentIndex(heroCondition.value);
			}
		}
		else if (heroConditionType == HERO_HAS_ITEM_EQUIPPED)
		{
			if (heroCondition.valueStoredInVariable)
			{
				heroitem_rbReferencedItem->setChecked(true);
				heroitem_lblVariable->setText(ProjectData::numberVariableIdToString(heroCondition.value));
			}
			else
			{
				heroitem_rbSpecificItem->setChecked(true);
				heroitem_cboxItems->setCurrentIndex(heroitem_itemIDs.indexOf(heroCondition.value));
			}
		}
		else if (heroConditionType == HERO_AFFLICTED_WITH_CONDITION)
		{
			if (heroCondition.valueStoredInVariable)
			{
				herocondition_rbReferencedCondition->setChecked(true);
				herocondition_lblVariable->setText(ProjectData::numberVariableIdToString(heroCondition.value));
			}
			else
			{
				herocondition_rbSpecificCondition->setChecked(true);
				herocondition_cboxConditions->setCurrentIndex(heroCondition.value);
			}
		}
	}
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::SPRITE)
	{
		BaseEvent::ConditionalBranch_Map::SpriteCondition spriteCondition;
		condition->getSpriteCondition(&spriteCondition);
		sprite_cboxType->setCurrentIndex(spriteCondition.spriteType);
		if (spriteCondition.spriteType == SPRITE_VEHICLE)
		{
			if (spriteCondition.spriteReferencedInVariable)
			{
				sprite_lblVehicleVariable->setText(ProjectData::numberVariableIdToString(spriteCondition.spriteID));
				sprite_rbReferencedVehicle->setChecked(true);
			}
			else
			{
				sprite_cboxVehicles->setCurrentIndex(spriteCondition.spriteID);
				sprite_rbSpecificVehicle->setChecked(true);
			}
		}
		else if (spriteCondition.spriteType == SPRITE_EVENT)
		{
			if (spriteCondition.spriteReferencedInVariable)
			{
				sprite_lblEventVariable->setText(ProjectData::numberVariableIdToString(spriteCondition.spriteID));
				sprite_rbReferencedEvent->setChecked(true);
			}
			else
			{
				sprite_cboxEvents->setCurrentIndex(spriteCondition.spriteID);
				sprite_rbSpecificEvent->setChecked(true);
			}
		}
		sprite_cboxDirection->setCurrentIndex(sprite_directionIDs.indexOf(spriteCondition.direction));
	}
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::PARTY_IS_RIDING)
	{
		BaseEvent::ConditionalBranch_Map::PartyIsRidingCondition partyIsRidingCondition;
		condition->getPartyIsRidingCondition(&partyIsRidingCondition);
		rbPartyIsRiding->setChecked(true);
		if (partyIsRidingCondition.vehicleReferencedInVariable)
		{
			riding_rbReferencedVehicle->setChecked(true);
			riding_lblVariable->setText(ProjectData::numberVariableIdToString(partyIsRidingCondition.vehicleID));
		}
		else
		{
			riding_rbSpecificVehicle->setChecked(true);
			riding_cboxVehicles->setCurrentIndex(partyIsRidingCondition.vehicleID);
		}
	}
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::EVENT_STARTED_WITH_ACTION_KEY)
		rbEventStartedWithActionKey->setChecked(true);
	else if (conditionType == BaseEvent::ConditionalBranch_Map::Condition::BGM_PLAYED_THROUGH_ONCE)
		rbBGMPlayedThroughOnce->setChecked(true);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::getCondition(BaseEvent::ConditionalBranch_Map::Condition *condition)
{
	if (rbSwitch->isChecked())
	{
		BaseEvent::ConditionalBranch_Map::SwitchCondition switchCondition;
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::SWITCH);
		switchCondition.switchReferencedInVariable = switch_rbReferencedSwitch->isChecked();
		if (switch_rbSpecificSwitch->isChecked())
			switchCondition.switchID = ProjectData::switchVariableIdFromString(switch_lblSwitch->text());
		else
			switchCondition.switchID = ProjectData::numberVariableIdFromString(switch_lblVariable->text());
		switchCondition.isOn = switch_rbOn->isChecked();
		condition->setSwitchCondition(&switchCondition);
	}
	else if (rbVariable->isChecked())
	{
		BaseEvent::ConditionalBranch_Map::VariableCondition variableCondition;
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::VARIABLE);
		variableCondition.srcVariableID = ProjectData::numberVariableIdFromString(variable_lblVariable->text());
		variableCondition.comparison = variable_cboxComparison->currentIndex();
		variableCondition.dstValueStoredInVariable = variable_rbStoredValue->isChecked();
		if (variable_rbSpecificValue->isChecked())
			variableCondition.dstValue = variable_sbValue->value();
		else
			variableCondition.dstValue = ProjectData::numberVariableIdFromString(variable_lblValueVariable->text());
		condition->setVariableCondition(&variableCondition);
	}
	else if (rbString->isChecked())
	{
		BaseEvent::ConditionalBranch_Map::StringCondition stringCondition;
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::STRING);
		stringCondition.srcStringReferencedInVariable = string_rbReferencedString->isChecked();
		if (string_rbSpecificString->isChecked())
			stringCondition.srcStringID = ProjectData::stringVariableIdFromString(string_lblString->text());
		else
			stringCondition.srcStringID = ProjectData::numberVariableIdFromString(string_lblVariable->text());
		stringCondition.comparison = string_cboxComparison->currentIndex();
		if (string_rbSpecificValue->isChecked())
		{
			stringCondition.dstStringType = BaseEvent::ConditionalBranch_Map::StringCondition::VALUE;
			stringCondition.dstStringID = 0;
			stringCondition.dstValue = string_leValue->text();
		}
		else if (string_rbValueInString->isChecked())
		{
			stringCondition.dstStringType = BaseEvent::ConditionalBranch_Map::StringCondition::STRING_VARIABLE;
			stringCondition.dstStringID = ProjectData::stringVariableIdFromString(string_lblValueString->text());
			stringCondition.dstValue = "";
		}
		else
		{
			stringCondition.dstStringType = BaseEvent::ConditionalBranch_Map::StringCondition::REFERENCED_STRING;
			stringCondition.dstStringID = ProjectData::numberVariableIdFromString(string_lblReferencedString->text());
			stringCondition.dstValue = "";
		}
		condition->setStringCondition(&stringCondition);
	}
	else if (rbTimer->isChecked())
	{
		BaseEvent::ConditionalBranch_Map::TimerCondition timerCondition;
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::TIMER);
		timerCondition.timerReferencedInVariable = timer_rbReferencedTimer->isChecked();
		if (timer_rbSpecificTimer->isChecked())
			timerCondition.timerID = timer_sbTimer->value() - 1;
		else
			timerCondition.timerID = ProjectData::numberVariableIdFromString(timer_lblTimerVariable->text());
		timerCondition.comparison = timer_cboxComparison->currentIndex();
		timerCondition.timeStoredInVariable = timer_rbStoredTime->isChecked();
		if (timer_rbSpecificTime->isChecked())
		{
			timerCondition.minutes = timer_sbMinutes->value();
			timerCondition.seconds = timer_sbSeconds->value();
		}
		else
		{
			timerCondition.minutes = 0;
			timerCondition.seconds = ProjectData::numberVariableIdFromString(timer_lblTimeVariable->text());
		}
		condition->setTimerCondition(&timerCondition);
	}
	else if (rbMoney->isChecked())
	{
		BaseEvent::ConditionalBranch_Map::MoneyCondition moneyCondition;
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::MONEY);
		moneyCondition.comparison = money_cboxComparison->currentIndex();
		moneyCondition.valueStoredInVariable = money_rbStoredValue->isChecked();
		if (money_rbSpecificValue->isChecked())
			moneyCondition.value = money_sbValue->value();
		else
			moneyCondition.value = ProjectData::numberVariableIdFromString(money_lblVariable->text());
		condition->setMoneyCondition(&moneyCondition);
	}
	else if (rbItem->isChecked())
	{
		BaseEvent::ConditionalBranch_Map::ItemCondition itemCondition;
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::ITEM);
		itemCondition.partyPossesses = item_rbPartyPossesses->isChecked();
		itemCondition.itemReferencedInVariable = item_rbReferencedItem->isChecked();
		if (item_rbSpecificItem->isChecked())
			itemCondition.itemID = item_cboxItems->currentIndex();
		else
			itemCondition.itemID = ProjectData::numberVariableIdFromString(item_lblVariable->text());
		condition->setItemCondition(&itemCondition);
	}
	else if (rbHero->isChecked())
	{
		BaseEvent::ConditionalBranch_Map::HeroCondition heroCondition;
		int conditionType = hero_cboxCondition->currentIndex();
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::HERO);
		heroCondition.string = "";
		heroCondition.characterReferencedInVariable = hero_rbReferencedCharacter->isChecked();
		if (hero_rbSpecificCharacter->isChecked())
			heroCondition.characterID = hero_cboxCharacters->currentIndex();
		else
			heroCondition.characterID = ProjectData::numberVariableIdFromString(hero_lblVariable->text());
		heroCondition.conditionType = conditionType;
		if (conditionType == HERO_NAME_IS)
		{
			if (heroname_rbSpecificValue->isChecked())
			{
				heroCondition.string.append(heroname_leValue->text());
				heroCondition.value = 0;
				heroCondition.valueStoredInVariable = false;
			}
			else if (heroname_rbValueInString->isChecked())
			{
				heroCondition.value = ProjectData::stringVariableIdFromString(heroname_lblString->text());
				heroCondition.valueStoredInVariable = false;
			}
			else
			{
				heroCondition.value = ProjectData::numberVariableIdFromString(heroname_lblVariable->text());
				heroCondition.valueStoredInVariable = true;
			}
		}
		else if (conditionType == HERO_LEVEL_IS_AT_LEAST)
		{
			heroCondition.valueStoredInVariable = herolevel_rbStoredValue->isChecked();
			if (herolevel_rbSpecificValue->isChecked())
				heroCondition.value = herolevel_sbValue->value();
			else
				heroCondition.value = ProjectData::numberVariableIdFromString(herolevel_lblVariable->text());
		}
		else if (conditionType == HERO_HP_IS_AT_LEAST)
		{
			heroCondition.valueStoredInVariable = herohp_rbStoredValue->isChecked();
			if (herohp_rbSpecificValue->isChecked())
				heroCondition.value = herohp_sbValue->value();
			else
				heroCondition.value = ProjectData::numberVariableIdFromString(herohp_lblVariable->text());
		}
		else if (conditionType == HERO_CAN_USE_SKILL)
		{
			heroCondition.valueStoredInVariable = heroskill_rbReferencedSkill->isChecked();
			if (heroskill_rbSpecificSkill->isChecked())
				heroCondition.value = heroskill_cboxSkills->currentIndex();
			else
				heroCondition.value = ProjectData::numberVariableIdFromString(heroskill_lblVariable->text());
		}
		else if (conditionType == HERO_HAS_ITEM_EQUIPPED)
		{
			heroCondition.valueStoredInVariable = heroitem_rbReferencedItem->isChecked();
			if (heroitem_rbSpecificItem->isChecked())
				heroCondition.value = heroitem_itemIDs[heroitem_cboxItems->currentIndex()];
			else
				heroCondition.value = ProjectData::numberVariableIdFromString(heroitem_lblVariable->text());
		}
		else if (conditionType == HERO_AFFLICTED_WITH_CONDITION)
		{
			heroCondition.valueStoredInVariable = herocondition_rbReferencedCondition->isChecked();
			if (herocondition_rbSpecificCondition->isChecked())
				heroCondition.value = herocondition_cboxConditions->currentIndex();
			else
				heroCondition.value = ProjectData::numberVariableIdFromString(herocondition_lblVariable->text());
		}
		condition->setHeroCondition(&heroCondition);
	}
	else if (rbSprite->isChecked())
	{
		BaseEvent::ConditionalBranch_Map::SpriteCondition spriteCondition;
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::SPRITE);
		spriteCondition.spriteType = sprite_cboxType->currentIndex();
		if (spriteCondition.spriteType == SPRITE_HERO)
		{
			spriteCondition.spriteID = 0;
			spriteCondition.spriteReferencedInVariable = false;
		}
		else if (spriteCondition.spriteType == SPRITE_VEHICLE)
		{
			if (sprite_rbSpecificVehicle->isChecked())
			{
				spriteCondition.spriteID = sprite_cboxVehicles->currentIndex();
				spriteCondition.spriteReferencedInVariable = false;
			}
			else
			{
				spriteCondition.spriteID = ProjectData::numberVariableIdFromString(sprite_lblVehicleVariable->text());
				spriteCondition.spriteReferencedInVariable = true;
			}
		}
		else if (spriteCondition.spriteType == SPRITE_EVENT)
		{
			if (sprite_rbSpecificEvent->isChecked())
			{
				spriteCondition.spriteID = sprite_cboxEvents->currentIndex();
				spriteCondition.spriteReferencedInVariable = false;
			}
			else
			{
				spriteCondition.spriteID = ProjectData::numberVariableIdFromString(sprite_lblEventVariable->text());
				spriteCondition.spriteReferencedInVariable = true;
			}
		}
		spriteCondition.direction = sprite_directionIDs[sprite_cboxDirection->currentIndex()];
		condition->setSpriteCondition(&spriteCondition);
	}
	else if (rbPartyIsRiding->isChecked())
	{
		BaseEvent::ConditionalBranch_Map::PartyIsRidingCondition partyIsRidingCondition;
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::PARTY_IS_RIDING);
		partyIsRidingCondition.vehicleReferencedInVariable = riding_rbReferencedVehicle->isChecked();
		if (riding_rbSpecificVehicle->isChecked())
			partyIsRidingCondition.vehicleID = riding_cboxVehicles->currentIndex();
		else
			partyIsRidingCondition.vehicleID = ProjectData::numberVariableIdFromString(riding_lblVariable->text());
		condition->setPartyIsRidingCondition(&partyIsRidingCondition);
	}
	else if (rbEventStartedWithActionKey->isChecked())
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::EVENT_STARTED_WITH_ACTION_KEY);
	else if (rbBGMPlayedThroughOnce->isChecked())
		condition->setType(BaseEvent::ConditionalBranch_Map::Condition::BGM_PLAYED_THROUGH_ONCE);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_item_bBrowseVariables_clicked()
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

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_item_rbSpecificItem_toggled(bool checked)
{
	item_cboxItems->setEnabled(checked);
	item_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_money_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(money_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		money_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_money_rbSpecificValue_toggled(bool checked)
{
	money_wValue->setEnabled(checked);
	money_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbBGMPlayedThroughOnce_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wBlank);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbEventStartedWithActionKey_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wBlank);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbHero_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wHero);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbItem_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wItem);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbMoney_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wMoney);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbPartyIsRiding_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wPartyIsRiding);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbSprite_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wSprite);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbString_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wString);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbSwitch_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wSwitch);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbTimer_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wTimer);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_rbVariable_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wVariable);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_riding_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(riding_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		riding_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_riding_rbSpecificVehicle_toggled(bool checked)
{
	riding_cboxVehicles->setEnabled(checked);
	riding_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_sprite_bBrowseEventVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(sprite_lblEventVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		sprite_lblEventVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_sprite_bBrowseVehicleVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(sprite_lblVehicleVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		sprite_lblVehicleVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_sprite_cboxType_currentIndexChanged(int index)
{
	if (index == SPRITE_HERO)
		sprite_stackedWidget->setCurrentWidget(sprite_wHero);
	else if (index == SPRITE_VEHICLE)
		sprite_stackedWidget->setCurrentWidget(sprite_wVehicle);
	else if (index == SPRITE_EVENT)
		sprite_stackedWidget->setCurrentWidget(sprite_wEvent);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_sprite_rbSpecificEvent_toggled(bool checked)
{
	sprite_cboxEvents->setEnabled(checked);
	sprite_wEventVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_sprite_rbSpecificVehicle_toggled(bool checked)
{
	sprite_cboxVehicles->setEnabled(checked);
	sprite_wVehicleVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_string_bBrowseReferenceVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(string_lblReferencedString->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		string_lblReferencedString->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_string_bBrowseStrings_clicked()
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

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_string_bBrowseValueStrings_clicked()
{
	VariableSelector *variableSelector;
	int stringID = ProjectData::stringVariableIdFromString(string_lblValueString->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_STRINGS, stringID);
	if (variableSelector->exec())
	{
		stringID = variableSelector->getVariableID();
		string_lblValueString->setText(ProjectData::stringVariableIdToString(stringID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_string_bBrowseVariables_clicked()
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

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_string_rbSpecificString_toggled(bool checked)
{
	string_wString->setEnabled(checked);
	string_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_string_rbSpecificValue_toggled(bool checked)
{
	string_leValue->setEnabled(checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_string_rbValueInReferencedString_toggled(bool checked)
{
	string_wReferencedString->setEnabled(checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_string_rbValueInString_toggled(bool checked)
{
	string_wValueString->setEnabled(checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_switch_bBrowseSwitches_clicked()
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

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_switch_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::switchVariableIdFromString(switch_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		switch_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_switch_rbSpecificSwitch_toggled(bool checked)
{
	switch_wSwitch->setEnabled(checked);
	switch_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_timer_bBrowseTimeVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(timer_lblTimeVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		timer_lblTimeVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_timer_bBrowseTimerVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(timer_lblTimerVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		timer_lblTimerVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_timer_rbSpecificTime_toggled(bool checked)
{
	timer_wTime->setEnabled(checked);
	timer_wTimeVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_timer_rbSpecificTimer_toggled(bool checked)
{
	timer_wTimer->setEnabled(checked);
	timer_wTimerVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_variable_bBrowseValueVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(variable_lblValueVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		variable_lblValueVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_variable_bBrowseVariables_clicked()
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

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_variable_rbSpecificValue_toggled(bool checked)
{
	variable_wValue->setEnabled(checked);
	variable_wValueVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_hero_rbSpecificCharacter_toggled(bool checked)
{
	hero_cboxCharacters->setEnabled(checked);
	hero_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_hero_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(hero_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		hero_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_hero_cboxCondition_currentIndexChanged(int index)
{
	if (index == HERO_IN_PARTY)
		hero_stackedWidget->setCurrentWidget(hero_wInParty);
	else if (index == HERO_NAME_IS)
		hero_stackedWidget->setCurrentWidget(hero_wName);
	else if (index == HERO_LEVEL_IS_AT_LEAST)
		hero_stackedWidget->setCurrentWidget(hero_wLevel);
	else if (index == HERO_HP_IS_AT_LEAST)
		hero_stackedWidget->setCurrentWidget(hero_wHP);
	else if (index == HERO_CAN_USE_SKILL)
		hero_stackedWidget->setCurrentWidget(hero_wSkill);
	else if (index == HERO_HAS_ITEM_EQUIPPED)
		hero_stackedWidget->setCurrentWidget(hero_wItem);
	else if (index == HERO_AFFLICTED_WITH_CONDITION)
		hero_stackedWidget->setCurrentWidget(hero_wCondition);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_heroname_rbSpecificValue_toggled(bool checked)
{
	heroname_leValue->setEnabled(checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_heroname_rbValueInString_toggled(bool checked)
{
	heroname_wStrings->setEnabled(checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_heroname_rbValueInReferencedString_toggled(bool checked)
{
	heroname_wVariable->setEnabled(checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_heroname_bBrowseStrings_clicked()
{
	VariableSelector *variableSelector;
	int stringID = ProjectData::stringVariableIdFromString(heroname_lblString->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_STRINGS, stringID);
	if (variableSelector->exec())
	{
		stringID = variableSelector->getVariableID();
		heroname_lblString->setText(ProjectData::stringVariableIdToString(stringID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_heroname_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(heroname_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		heroname_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_herolevel_rbSpecificValue_toggled(bool checked)
{
	herolevel_wValue->setEnabled(checked);
	herolevel_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_herolevel_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(herolevel_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		herolevel_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_herohp_rbSpecificValue_toggled(bool checked)
{
	herohp_wValue->setEnabled(checked);
	herohp_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_herohp_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(herohp_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		herohp_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_heroskill_rbSpecificSkill_toggled(bool checked)
{
	heroskill_cboxSkills->setEnabled(checked);
	heroskill_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_heroskill_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(heroskill_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		heroskill_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_heroitem_rbSpecificItem_toggled(bool checked)
{
	heroitem_cboxItems->setEnabled(checked);
	heroitem_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_heroitem_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(heroitem_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		heroitem_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_herocondition_rbSpecificCondition_toggled(bool checked)
{
	herocondition_cboxConditions->setEnabled(checked);
	herocondition_wVariable->setEnabled(!checked);
}

void ConfigureEvent_MapConditionalBranch_ConfigureCondition::on_herocondition_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(herocondition_lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		herocondition_lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

QStringList ConfigureEvent_MapConditionalBranch_ConfigureCondition::getEquipItems()
{
	ProjectData::DataReference<Item> *itemRef;
	QStringList itemList;
	heroitem_itemIDs.clear();
	for (int i = 0, count = ProjectData::itemNames.size(); i < count; ++i)
	{
		itemRef = new ProjectData::DataReference<Item>(i, __FILE__, __LINE__);
		if ((*itemRef)->getType() >= Item::TYPE_WEAPON && (*itemRef)->getType() <= Item::TYPE_ACCESSORY)
		{
			itemList.append((*itemRef)->getName());
			heroitem_itemIDs.append(i);
		}
		delete itemRef;
	}
	return itemList;
}
