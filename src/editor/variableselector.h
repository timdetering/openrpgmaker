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

#ifndef VARIABLESELECTOR_H
#define VARIABLESELECTOR_H

#include "ui_variableselector.h"

class VariableSelector : public QDialog, public Ui::VariableSelector
{
	Q_OBJECT
	public:
		enum VariableType {TYPE_STRINGS=0, TYPE_NUMBERS, TYPE_SWITCHES};
		VariableSelector(int type, int current, QWidget *parent=0);
		~VariableSelector();
		int getVariableID();
	protected slots:
		void on_lwGroupList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_lwVariableList_itemSelectionChanged();
		void on_leName_textEdited(QString text);
	private:
		void updateGroupList();
		int variableType;
};

#endif // VARIABLESELECTOR_H
