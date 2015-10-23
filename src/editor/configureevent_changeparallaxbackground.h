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

#ifndef CONFIGUREEVENT_CHANGEPARALLAXBACKGROUND_H
#define CONFIGUREEVENT_CHANGEPARALLAXBACKGROUND_H

#include "ui_configureevent_changeparallaxbackground.h"

class ConfigureEvent_ChangeParallaxBackground : public QDialog, public Ui::ConfigureEvent_ChangeParallaxBackground
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeParallaxBackground(QWidget *parent=0);
		~ConfigureEvent_ChangeParallaxBackground();
		QString getBackgroundLocation();
		void setBackgroundLocation(QString value);
		int getHScrollSpeed();
		void setHScrollSpeed(int value);
		bool getAutoHScroll();
		void setAutoHScroll(bool value);
		int getVScrollSpeed();
		void setVScrollSpeed(int value);
		bool getAutoVScroll();
		void setAutoVScroll(bool value);
	protected slots:
		void on_bSetBG_clicked();
	private:
		QString bgLocation;
};

#endif // CONFIGUREEVENT_CHANGEPARALLAXBACKGROUND_H
