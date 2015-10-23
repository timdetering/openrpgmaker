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

#ifndef CONFIGUREMOVEMENTPATTERN_WAIT_H
#define CONFIGUREMOVEMENTPATTERN_WAIT_H

#include "ui_configuremovementpattern_wait.h"

class ConfigureMovementPattern_Wait : public QDialog, public Ui::ConfigureMovementPattern_Wait
{
	Q_OBJECT
	public:
		ConfigureMovementPattern_Wait(QWidget *parent) : QDialog(parent)
		{
			setupUi(this);
			resize(sizeHint());
			setMinimumWidth(width());
			setMaximumWidth(width());
			setMinimumHeight(height());
			setMaximumHeight(height());
			bTimeSideButton->setSpinBox(sbTime);
		}
		~ConfigureMovementPattern_Wait() {}
		int getTime() {return sbTime->value();}
};

#endif // CONFIGUREMOVEMENTPATTERN_WAIT_H
