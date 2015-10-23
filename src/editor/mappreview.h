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
 *  Copyright (C) 2010, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef MAPPREVIEW_H
#define MAPPREVIEW_H

#include "ui_mappreview.h"
#include "projectdata.h"

class MapPreview: public QDialog, public Ui::MapPreview
{
	Q_OBJECT
	public:
		MapPreview(QWidget *parent=0);
		~MapPreview();
		void setupMapPreview(int mapID);
	protected slots:
		void on_twLayers_itemChanged(QTableWidgetItem *item);
		void on_buttonBox_clicked(QAbstractButton *abstractButton);
	private:
		ProjectData::DataReference<Map> *mapRef;
		bool ignoreEvents;
};

#endif // MAPPREVIEW_H
