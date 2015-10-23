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

#ifndef CONFIGUREEVENT_CHANGEEVENTLOCATION_H
#define CONFIGUREEVENT_CHANGEEVENTLOCATION_H

#include "ui_configureevent_changeeventlocation.h"
#include "projectdata.h"
#include "projectdata_map.h"

class ConfigureEvent_ChangeEventLocation : public QDialog, public Ui::ConfigureEvent_ChangeEventLocation
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeEventLocation(int mapID, QWidget *parent=0);
		~ConfigureEvent_ChangeEventLocation();
		void setEventID(int eventID);
		int getEventID();
		void setEventReferencedInVariable(bool referenced);
		bool isEventReferencedInVariable();
		void setLocation(int x, int y, int layer);
		int getXLocation();
		int getYLocation();
		int getLayer();
		void setLocationStoredInVariables(bool stored);
		bool isLocationStoredInVariables();
	protected slots:
		void on_bBrowseEventVariables_clicked();
		void on_bBrowseXVariables_clicked();
		void on_bBrowseYVariables_clicked();
		void on_bBrowseLayerVariables_clicked();
		void on_rbSpecificEvent_toggled(bool checked);
		void on_rbSpecificLocation_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGEEVENTLOCATION_H
