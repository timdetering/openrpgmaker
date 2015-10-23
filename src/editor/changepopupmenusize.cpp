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

#include "changepopupmenusize.h"

ChangePopupMenuSize::ChangePopupMenuSize(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	bWidthSideButton->setSpinBox(sbWidth);
	bHeightSideButton->setSpinBox(sbHeight);
}

ChangePopupMenuSize::~ChangePopupMenuSize()
{
}

void ChangePopupMenuSize::setSize(int width, int height)
{
	sbWidth->setValue(width);
	sbHeight->setValue(height);
}

int ChangePopupMenuSize::getWidth()
{
	return sbWidth->value();
}

int ChangePopupMenuSize::getHeight()
{
	return sbHeight->value();
}
