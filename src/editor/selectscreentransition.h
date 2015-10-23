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

#ifndef SELECTSCREENTRANSITION_H
#define SELECTSCREENTRANSITION_H

#include "ui_selectscreentransition.h"

class SelectScreenTransition : public QDialog, public Ui::SelectScreenTransition
{
	Q_OBJECT
	public:
		SelectScreenTransition(bool selectEraseTransition, QWidget *parent=0);
		~SelectScreenTransition();
		void setTransition(int transition);
		int getTransition();
	private:
		bool showScreen;
};

#endif // SELECTSCREENTRANSITION_H
