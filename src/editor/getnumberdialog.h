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

#ifndef GETNUMBERDIALOG_H
#define GETNUMBERDIALOG_H

#include "ui_getnumberdialog.h"

class GetNumberDialog : public QDialog, public Ui::GetNumberDialog
{
	Q_OBJECT
	public:
		GetNumberDialog(QString title, int minValue, int maxValue, QWidget *parent=0);
		~GetNumberDialog();
		int getValue();
		void setValue(int value);
		bool isValueStoredInVariable();
		void setValueStoredInVariable(bool stored);
	protected slots:
		void on_bBrowseVariables_clicked();
		void on_rbSpecificValue_toggled(bool checked);
};

#endif // GETNUMBERDIALOG_H
