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

#ifndef CONFIGUREMAPONLOADEVENT_H
#define CONFIGUREMAPONLOADEVENT_H

class BaseEvent;

#include "ui_configuremaponloadevent.h"
#include "projectdata.h"
#include "projectdata_map.h"

class ConfigureMapOnLoadEvent : public QDialog, public Ui::ConfigureMapOnLoadEvent
{
	Q_OBJECT
	public:
		ConfigureMapOnLoadEvent(int mapID, QWidget *parent=0);
		~ConfigureMapOnLoadEvent();
	protected slots:
		void on_buttonBox_clicked(QAbstractButton *button);
	private:
		ProjectData::DataReference<Map> *mapRef;
};

#endif // CONFIGUREMAPONLOADEVENT_H
