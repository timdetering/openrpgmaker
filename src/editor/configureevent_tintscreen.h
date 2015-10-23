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

#ifndef CONFIGUREEVENT_TINTSCREEN_H
#define CONFIGUREEVENT_TINTSCREEN_H

#include "ui_configureevent_tintscreen.h"

class ConfigureEvent_TintScreen : public QDialog, public Ui::ConfigureEvent_TintScreen
{
	Q_OBJECT
	public:
		ConfigureEvent_TintScreen(QWidget *parent=0);
		~ConfigureEvent_TintScreen();
		QColor getColor();
		void setColor(QColor color);
		int getOpacity();
		void setOpacity(int value);
		int getTransitionTime();
		void setTransitionTime(int value);
		bool isOpacityStoredInVariable();
		void setOpacityStoredInVariable(bool stored);
		bool isTimeStoredInVariable();
		void setTimeStoredInVariable(bool stored);
		bool getHaltOtherProcesses();
		void setHaltOtherProcesses(bool value);
	protected slots:
		void on_bBrowseOpacityVariables_clicked();
		void on_bBrowseTimeVariables_clicked();
		void on_rbSpecificOpacity_toggled(bool checked);
		void on_rbSpecificTime_toggled(bool checked);
};

#endif // CONFIGUREEVENT_TINTSCREEN_H
