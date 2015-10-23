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

#ifndef CONFIGUREEVENT_PANSCREEN_H
#define CONFIGUREEVENT_PANSCREEN_H

#include "ui_configureevent_panscreen.h"
#include "projectdata.h"
#include "projectdata_map.h"

class ConfigureEvent_PanScreen : public QDialog, public Ui::ConfigureEvent_PanScreen
{
	Q_OBJECT
	public:
		enum Operation {LOCK_PAN_STATE=0, UNLOCK_PAN_STATE, PAN_SCREEN, RETURN_TO_ORIGIN};
		enum Direction {UP=0, DOWN, LEFT, RIGHT};
		enum Pan {DIRECTION=0, LOCATION};
		enum TransitionSpeed {SPEED_ONEEIGHTHNORMAL=0, SPEED_ONEFOURTHNORMAL, SPEED_ONEHALFNORMAL, SPEED_NORMAL, SPEED_TWICENORMAL, SPEED_FOURTIMESNORMAL};
		ConfigureEvent_PanScreen(int map, QWidget *parent=0);
		~ConfigureEvent_PanScreen();
		int getOperation();
		void setOperation(int value);
		int getPan();
		void setPan(int value);
		int getDirection();
		void setDirection(int value);
		int getXLocation();
		void setXLocation(int value);
		int getYLocation();
		void setYLocation(int value);
		bool getCoordinatesStoredInVariables();
		void setCoordinatesStoredInVariables(bool stored);
		int getTransitionSpeed();
		void setTransitionSpeed(int value);
		int getTiles();
		void setTiles(int value);
		bool getHaltOtherProcesses();
		void setHaltOtherProcesses(bool value);
	protected slots:
		void on_bSelectLocation_clicked();
		void on_rbDirection_toggled(bool checked);
		void on_rbLockPanState_toggled(bool checked);
		void on_rbPanScreen_toggled(bool checked);
		void on_rbReturnToOrigin_toggled(bool checked);
		void on_rbUnlockPanState_toggled(bool checked);
	private:
		int mapID;
		int currentX;
		int currentY;
		bool coordinatesStoredInVariables;
};

#endif // CONFIGUREEVENT_PANSCREEN_H
