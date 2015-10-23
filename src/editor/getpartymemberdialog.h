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

#ifndef GETPARTYMEMBERDIALOG_H
#define GETPARTYMEMBERDIALOG_H

#include "ui_getpartymemberdialog.h"

class GetPartyMemberDialog : public QDialog, public Ui::GetPartyMemberDialog
{
	Q_OBJECT
	public:
		GetPartyMemberDialog(QWidget *parent=0);
		~GetPartyMemberDialog();
		int getID();
		void setID(int id);
		bool isIDStoredInVariable();
		void setIDStoredInVariable(bool stored);
	protected slots:
		void on_bBrowseVariables_clicked();
		void on_rbVariable_toggled(bool checked);
};

#endif // GETPARTYMEMBERDIALOG_H
