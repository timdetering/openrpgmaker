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

#ifndef CONFIGUREEVENT_MENUCONDITIONALBRANCH_CONFIGURECONDITION_H
#define CONFIGUREEVENT_MENUCONDITIONALBRANCH_CONFIGURECONDITION_H

#include "ui_configureevent_menuconditionalbranch_configurecondition.h"
#include "projectdata_baseevent.h"

class ConfigureEvent_MenuConditionalBranch_ConfigureCondition : public QDialog, public Ui::ConfigureEvent_MenuConditionalBranch_ConfigureCondition
{
	Q_OBJECT
	public:
		enum HeroConditionType {HERO_IN_PARTY=0, HERO_NAME_IS, HERO_LEVEL_IS_AT_LEAST, HERO_HP_IS_AT_LEAST, HERO_CAN_USE_SKILL, HERO_HAS_ITEM_EQUIPPED, HERO_AFFLICTED_WITH_CONDITION};
		ConfigureEvent_MenuConditionalBranch_ConfigureCondition(int menuID, int menuType, QWidget *parent=0);
		~ConfigureEvent_MenuConditionalBranch_ConfigureCondition();
		void setCondition(BaseEvent::ConditionalBranch_Menu::Condition *condition);
		void getCondition(BaseEvent::ConditionalBranch_Menu::Condition *condition);
	protected slots:
		void on_item_bBrowseVariables_clicked();
		void on_item_rbSpecificItem_toggled(bool checked);
		void on_money_bBrowseVariables_clicked();
		void on_money_rbSpecificValue_toggled(bool checked);
		void on_rbBGMPlayedThroughOnce_toggled(bool checked);
		void on_rbCurrentHighlightedArea_toggled(bool checked);
		void on_rbHero_toggled(bool checked);
		void on_rbItem_toggled(bool checked);
		void on_rbMoney_toggled(bool checked);
		void on_rbPartyIsRiding_toggled(bool checked);
		void on_rbPreviousSelectedArea_toggled(bool checked);
		void on_rbString_toggled(bool checked);
		void on_rbSwitch_toggled(bool checked);
		void on_rbTimer_toggled(bool checked);
		void on_rbVariable_toggled(bool checked);
		void on_riding_bBrowseVariables_clicked();
		void on_riding_rbSpecificVehicle_toggled(bool checked);
		void on_string_bBrowseReferenceVariables_clicked();
		void on_string_bBrowseStrings_clicked();
		void on_string_bBrowseValueStrings_clicked();
		void on_string_bBrowseVariables_clicked();
		void on_string_rbSpecificString_toggled(bool checked);
		void on_string_rbSpecificValue_toggled(bool checked);
		void on_string_rbValueInReferencedString_toggled(bool checked);
		void on_string_rbValueInString_toggled(bool checked);
		void on_switch_bBrowseSwitches_clicked();
		void on_switch_bBrowseVariables_clicked();
		void on_switch_rbSpecificSwitch_toggled(bool checked);
		void on_timer_bBrowseTimeVariables_clicked();
		void on_timer_bBrowseTimerVariables_clicked();
		void on_timer_rbSpecificTime_toggled(bool checked);
		void on_timer_rbSpecificTimer_toggled(bool checked);
		void on_variable_bBrowseValueVariables_clicked();
		void on_variable_bBrowseVariables_clicked();
		void on_variable_rbSpecificValue_toggled(bool checked);
		void on_hero_rbSpecificCharacter_toggled(bool checked);
		void on_hero_bBrowseVariables_clicked();
		void on_hero_cboxCondition_currentIndexChanged(int index);
		void on_heroname_rbSpecificValue_toggled(bool checked);
		void on_heroname_rbValueInString_toggled(bool checked);
		void on_heroname_rbValueInReferencedString_toggled(bool checked);
		void on_heroname_bBrowseStrings_clicked();
		void on_heroname_bBrowseVariables_clicked();
		void on_herolevel_rbSpecificValue_toggled(bool checked);
		void on_herolevel_bBrowseVariables_clicked();
		void on_herohp_rbSpecificValue_toggled(bool checked);
		void on_herohp_bBrowseVariables_clicked();
		void on_heroskill_rbSpecificSkill_toggled(bool checked);
		void on_heroskill_bBrowseVariables_clicked();
		void on_heroitem_rbSpecificItem_toggled(bool checked);
		void on_heroitem_bBrowseVariables_clicked();
		void on_herocondition_rbSpecificCondition_toggled(bool checked);
		void on_herocondition_bBrowseVariables_clicked();
	private:
		QStringList getEquipItems();
		QList<int> heroitem_itemIDs;
};

#endif // CONFIGUREEVENT_MENUCONDITIONALBRANCH_CONFIGURECONDITION_H
