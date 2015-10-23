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

#ifndef CONFIGUREEVENT_CALLMAPEVENT_H
#define CONFIGUREEVENT_CALLMAPEVENT_H

#include "ui_configureevent_callmapevent.h"
#include "projectdata.h"
#include "projectdata_map.h"

class ConfigureEvent_CallMapEvent : public QDialog, public Ui::ConfigureEvent_CallMapEvent
{
	Q_OBJECT
	public:
		ConfigureEvent_CallMapEvent(int mapID, QWidget *parent=0);
		~ConfigureEvent_CallMapEvent();
		void setEventID(int eventID);
		int getEventID();
		void setEventReferencedInVariable(bool referenced);
		bool isEventReferencedInVariable();
		void setPageID(int pageID);
		int getPageID();
		void setPageReferencedInVariable(bool referenced);
		bool isPageReferencedInVariable();
	protected slots:
		void on_bBrowseEventVariables_clicked();
		void on_bBrowsePageVariables_clicked();
		void on_cboxEvent_currentIndexChanged(int index);
		void on_rbSpecificEvent_toggled(bool checked);
		void on_rbSpecificPage_toggled(bool checked);
	private:
		ProjectData::DataReference<Map> *mapRef;
};

#endif // CONFIGUREEVENT_CALLMAPEVENT_H
