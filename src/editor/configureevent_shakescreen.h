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

#ifndef CONFIGUREEVENT_SHAKESCREEN_H
#define CONFIGUREEVENT_SHAKESCREEN_H

#include "ui_configureevent_shakescreen.h"

class ConfigureEvent_ShakeScreen : public QDialog, public Ui::ConfigureEvent_ShakeScreen
{
	Q_OBJECT
	public:
		enum Action {SHAKE_ONCE=0, BEGIN_SHAKING, STOP_SHAKING};
		ConfigureEvent_ShakeScreen(QWidget *parent=0);
		~ConfigureEvent_ShakeScreen();
		int getAction();
		void setAction(int value);
		int getTime();
		void setTime(int value);
		int getStrength();
		void setStrength(int value);
		int getSpeed();
		void setSpeed(int value);
		bool isTimeStoredInVariable();
		void setTimeStoredInVariable(bool stored);
		bool getHaltOtherProcesses();
		void setHaltOtherProcesses(bool halt);
	protected slots:
		void on_bBrowseTimeVariables_clicked();
		void on_rbBeginShaking_toggled(bool checked);
		void on_rbShakeOnce_toggled(bool checked);
		void on_rbSpecificTime_toggled(bool checked);
		void on_rbStopShaking_toggled(bool checked);
};

#endif // CONFIGUREEVENT_SHAKESCREEN_H
