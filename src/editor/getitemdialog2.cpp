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

#include "getitemdialog2.h"

GetItemDialog2::GetItemDialog2(QString title, QString type, QStringList items, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setWindowTitle(title);
	lblType->setText(type);
	cboxItem->addItems(items);
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

GetItemDialog2::~GetItemDialog2()
{
}

int GetItemDialog2::getItemID()
{
	return cboxItem->currentIndex();
}

void GetItemDialog2::setItemID(int value)
{
	cboxItem->setCurrentIndex(value);
}
