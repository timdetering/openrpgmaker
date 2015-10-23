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

#include "configureevent_setscreentransition.h"
#include "projectdata.h"

ConfigureEvent_SetScreenTransition::ConfigureEvent_SetScreenTransition(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	cboxShowTransitions->setVisible(false);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_SetScreenTransition::~ConfigureEvent_SetScreenTransition()
{
}

void ConfigureEvent_SetScreenTransition::setScreenTransition(int screenTransition)
{
	cboxScreenTransition->setCurrentIndex(screenTransition);
}

void ConfigureEvent_SetScreenTransition::setTransition(int transition)
{
	if (cboxScreenTransition->currentIndex() % 2 == 0)
		cboxEraseTransitions->setCurrentIndex(transition);
	else
		cboxShowTransitions->setCurrentIndex(transition);
}

int ConfigureEvent_SetScreenTransition::getScreenTransition()
{
	return cboxScreenTransition->currentIndex();
}

int ConfigureEvent_SetScreenTransition::getTransition()
{
	if (cboxScreenTransition->currentIndex() % 2 == 0)
		return cboxEraseTransitions->currentIndex();
	return cboxShowTransitions->currentIndex();
}

void ConfigureEvent_SetScreenTransition::on_cboxScreenTransition_currentIndexChanged(int index)
{
	if (index % 2 == 0)
	{
		cboxEraseTransitions->setVisible(true);
		cboxShowTransitions->setVisible(false);
	}
	else
	{
		cboxEraseTransitions->setVisible(false);
		cboxShowTransitions->setVisible(true);
	}
}
