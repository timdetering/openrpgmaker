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

#ifndef GETITEMDIALOG2_H
#define GETITEMDIALOG2_H

#include "ui_getitemdialog2.h"

class GetItemDialog2 : public QDialog, public Ui::GetItemDialog2
{
	Q_OBJECT
	public:
		GetItemDialog2(QString title, QString type, QStringList items, QWidget *parent=0);
		~GetItemDialog2();
		int getItemID();
		void setItemID(int value);
};

#endif // GETITEMDIALOG2_H
