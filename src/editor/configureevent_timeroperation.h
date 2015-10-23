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

#ifndef CONFIGUREEVENT_TIMEROPERATION_H
#define CONFIGUREEVENT_TIMEROPERATION_H

#include "ui_configureevent_timeroperation.h"

class ConfigureEvent_TimerOperation : public QDialog, public Ui::ConfigureEvent_TimerOperation
{
	Q_OBJECT
	public:
		enum Operation {SET=0, START, STOP};
		ConfigureEvent_TimerOperation(QWidget *parent=0);
		~ConfigureEvent_TimerOperation();
		void setTimerID(int timerID);
		void setIsReferencedTimer(bool referenced);
		void setOperation(int operation);
		void setTime(int minutes, int seconds);
		void setIsStoredTime(bool stored);
		void setAlignment(int halign, int valign);
		void setDisplayTimerOnScreen(bool display);
		void setTimerRunsDuringBattle(bool runsDuringBattle);
		int getTimerID();
		bool isReferencedTimer();
		int getOperation();
		int getMinutes();
		int getSeconds();
		bool isStoredTime();
		int getHAlignment();
		int getVAlignment();
		bool displayTimerOnScreen();
		bool timerRunsDuringBattle();
	protected slots:
		void on_bBrowseTimeVariables_clicked();
		void on_bBrowseTimerVariables_clicked();
		void on_cbDisplayTimerOnScreen_toggled(bool checked);
		void on_rbSet_toggled(bool checked);
		void on_rbSpecificTime_toggled(bool checked);
		void on_rbSpecificTimer_toggled(bool checked);
		void on_rbStart_toggled(bool checked);
		void on_rbStop_toggled(bool checked);
};

#endif // CONFIGUREEVENT_TIMEROPERATION_H
