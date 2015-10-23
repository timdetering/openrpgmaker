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


#ifndef ABOUT_H
#define ABOUT_H

#include "ui_about.h"

class About : public QDialog, public Ui::About
{
	Q_OBJECT
	public:
		About(QWidget *parent=0) : QDialog(parent)
		{
			setupUi(this);
			resize(sizeHint());
			setMinimumWidth(width());
			setMaximumWidth(width());
			setMinimumHeight(height());
			setMaximumHeight(height());
		}
};

#endif // ABOUT_H
