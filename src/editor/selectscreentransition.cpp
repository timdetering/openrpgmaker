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

#include "selectscreentransition.h"

SelectScreenTransition::SelectScreenTransition(bool selectEraseTransition, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	showScreen = !selectEraseTransition;
	if (showScreen)
		cboxEraseTransitions->setVisible(false);
	else
		cboxShowTransitions->setVisible(false);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

SelectScreenTransition::~SelectScreenTransition()
{
}

void SelectScreenTransition::setTransition(int transition)
{
	if (showScreen)
		cboxShowTransitions->setCurrentIndex(transition);
	else
		cboxEraseTransitions->setCurrentIndex(transition);
}

int SelectScreenTransition::getTransition()
{
	if (showScreen)
		return cboxShowTransitions->currentIndex();
	return cboxEraseTransitions->currentIndex();
}
