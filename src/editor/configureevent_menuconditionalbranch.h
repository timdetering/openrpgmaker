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

#ifndef CONFIGUREEVENT_MENUCONDITIONALBRANCH_H
#define CONFIGUREEVENT_MENUCONDITIONALBRANCH_H

#include "ui_configureevent_menuconditionalbranch.h"
#include "projectdata_baseevent.h"

class ConfigureEvent_MenuConditionalBranch : public QDialog, public Ui::ConfigureEvent_MenuConditionalBranch
{
	Q_OBJECT
	public:
		enum ConditionsToMatch {ALL=0, AT_LEAST, NONE};
		ConfigureEvent_MenuConditionalBranch(int menu, int type, QWidget *parent=0);
		~ConfigureEvent_MenuConditionalBranch();
		void setConditionsToMatch(int conditionsToMatch);
		void setNumConditionsToMatch(int numConditions);
		void setExecuteCustomHandler(bool executeCustomHandler);
		void addConditions(QList<BaseEvent::ConditionalBranch_Menu::Condition*> values);
		int getConditionsToMatch();
		int getNumConditionsToMatch();
		bool executeCustomHandler();
		QList<BaseEvent::ConditionalBranch_Menu::Condition*> getConditions();
	protected slots:
		void on_bAddCondition_clicked();
		void on_bConfigureCondition_clicked();
		void on_bRemoveCondition_clicked();
		void on_lwConditions_itemDoubleClicked(QListWidgetItem *item);
		void on_lwConditions_itemSelectionChanged();
		void on_rbMatchAtLeast_toggled(bool checked);
	private:
		QList<BaseEvent::ConditionalBranch_Menu::Condition*> conditions;
		int menuID;
		int menuType;
};

#endif // CONFIGUREEVENT_MENUCONDITIONALBRANCH_H
