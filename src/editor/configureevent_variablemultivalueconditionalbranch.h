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

#ifndef CONFIGUREEVENT_VARIABLEMULTIVALUECONDITIONALBRANCH_H
#define CONFIGUREEVENT_VARIABLEMULTIVALUECONDITIONALBRANCH_H

#include "ui_configureevent_variablemultivalueconditionalbranch.h"
#include "projectdata_baseevent.h"

class ConfigureEvent_VariableMultiValueConditionalBranch : public QDialog, public Ui::ConfigureEvent_VariableMultiValueConditionalBranch
{
	Q_OBJECT
	public:
		ConfigureEvent_VariableMultiValueConditionalBranch(QWidget *parent=0);
		~ConfigureEvent_VariableMultiValueConditionalBranch();
		int getVariableID();
		void setVariableID(int value);
		bool getExecuteCustomHandlerIfNoValueMet();
		void setExecuteCustomHandlerIfNoValueMet(bool value);
		void addValues(QList<BaseEvent::VariableMultiValueConditionalBranch::VariableValue*> variableValues);
		QList<BaseEvent::VariableMultiValueConditionalBranch::VariableValue*> getValues();
	protected slots:
		void on_bAddValue_clicked();
		void on_bBrowseVariables_clicked();
		void on_bConfigureValue_clicked();
		void on_bRemoveValue_clicked();
		void on_lwValues_itemDoubleClicked(QListWidgetItem *item);
		void on_lwValues_itemSelectionChanged();
	private:
		QList<BaseEvent::VariableMultiValueConditionalBranch::VariableValue*> values;
};

#endif // CONFIGUREEVENT_VARIABLEMULTIVALUECONDITIONALBRANCH_H
