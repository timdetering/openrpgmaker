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

#include "configureevent_allowdisallow.h"

ConfigureEvent_AllowDisallow::ConfigureEvent_AllowDisallow(QString typeText, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setWindowTitle(QString("Allow/Disallow ") + typeText);
	rbAllow->setText(QString("Allow ") + typeText);
	rbForbid->setText(QString("Forbid ") + typeText);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_AllowDisallow::~ConfigureEvent_AllowDisallow()
{
}

void ConfigureEvent_AllowDisallow::setAllow(bool allow)
{
	if (allow)
		rbAllow->setChecked(true);
	else
		rbForbid->setChecked(true);
}

bool ConfigureEvent_AllowDisallow::getAllow()
{
	return rbAllow->isChecked();
}
