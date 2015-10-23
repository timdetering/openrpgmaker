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

#ifndef CONFIGUREEVENT_SWITCHOPERATION_H
#define CONFIGUREEVENT_SWITCHOPERATION_H

#include "ui_configureevent_switchoperation.h"

class ConfigureEvent_SwitchOperation : public QDialog, public Ui::ConfigureEvent_SwitchOperation
{
	Q_OBJECT
	public:
		enum SwitchToChange {SINGLE=0, RANGE, REFERENCE};
		enum Operation {TURN_ON=0, TURN_OFF, TOGGLE};
		ConfigureEvent_SwitchOperation(QWidget *parent=0);
		~ConfigureEvent_SwitchOperation();
		void setSwitchToChange(int switchToChange);
		void setSwitchIDs(int switch1ID, int switch2ID);
		void setOperation(int operation);
		int getSwitchToChange();
		int getSwitch1ID();
		int getSwitch2ID();
		int getOperation();
	protected slots:
		void on_bBrowseEndSwitches_clicked();
		void on_bBrowseStartSwitches_clicked();
		void on_bBrowseSwitches_clicked();
		void on_bBrowseVariables_clicked();
		void on_rbRangeOfSwitches_toggled(bool checked);
		void on_rbReferencedSwitch_toggled(bool checked);
		void on_rbSpecificSwitch_toggled(bool checked);
};

#endif // CONFIGUREEVENT_SWITCHOPERATION_H
