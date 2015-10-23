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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef MAPLOCATION_H
#define MAPLOCATION_H

#include "ui_maplocation.h"

class MapLocation : public QWidget, public Ui::MapLocation
{
	Q_OBJECT
	public:
		MapLocation(QWidget *parent=0);
		~MapLocation();
		void setupMapLocation(int map);
		void changeMap(int mapID);
		void setLocation(int x, int y, int layer);
		int getXLocation();
		int getYLocation();
		int getLayer();
	protected slots:
		void on_bZoom1to1_toggled(bool checked);
		void on_bZoom1to2_toggled(bool checked);
		void on_bZoom1to4_toggled(bool checked);
		void on_bZoom1to8_toggled(bool checked);
		void on_gvMapView_mouseButtonPressed(QMouseEvent *event);
		void on_twLayers_itemChanged(QTableWidgetItem *item);
		void on_twLayers_itemSelectionChanged();
	private:
		ProjectData::DataReference<Map> *mapRef;
		QGraphicsRectItem *currentSelection;
		int currentX;
		int currentY;
		int currentZoom;
		bool ignoreEvents;
};

#endif // MAPLOCATION_H
