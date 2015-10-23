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

#ifndef CONFIGUREEVENT_FLASHSCREEN_H
#define CONFIGUREEVENT_FLASHSCREEN_H

#include "ui_configureevent_flashscreen.h"

class ConfigureEvent_FlashScreen : public QDialog, public Ui::ConfigureEvent_FlashScreen
{
	Q_OBJECT
	public:
		enum Flash {ONCE=0, BEGIN, STOP};
		ConfigureEvent_FlashScreen(QWidget *parent=0);
		~ConfigureEvent_FlashScreen();
		int getFlash();
		void setFlash(int value);
		QColor getColor();
		void setColor(QColor color);
		int getOpacity();
		void setOpacity(int value);
		int getDelayBetweenFlashes();
		void setDelayBetweenFlashes(int value);
		bool isOpacityStoredInVariable();
		void setOpacityStoredInVariable(bool stored);
		bool isDelayStoredInVariable();
		void setDelayStoredInVariable(bool stored);
		bool getHaltOtherProcesses();
		void setHaltOtherProcesses(bool value);
	protected slots:
		void on_bBrowseDelayVariables_clicked();
		void on_bBrowseOpacityVariables_clicked();
		void on_cboxFlash_currentIndexChanged(int index);
		void on_rbSpecificDelay_toggled(bool checked);
		void on_rbSpecificOpacity_toggled(bool checked);
};

#endif // CONFIGUREEVENT_FLASHSCREEN_H
