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

#ifndef CONFIGUREEVENT_CALLEVENT_H
#define CONFIGUREEVENT_CALLEVENT_H

#include "ui_configureevent_callevent.h"

class ConfigureEvent_CallEvent : public QDialog, public Ui::ConfigureEvent_CallEvent
{
	Q_OBJECT
	public:
		ConfigureEvent_CallEvent(QWidget *parent=0);
		~ConfigureEvent_CallEvent();
		void setEventID(int eventID);
		void setIsReferencedInVariable(bool referenced);
		int getEventID();
		bool isReferencedInVariable();
	protected slots:
		void on_bBrowseVariables_clicked();
		void on_rbSpecificEvent_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CALLEVENT_H
