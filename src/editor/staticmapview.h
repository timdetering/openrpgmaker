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

#ifndef STATICMAPVIEW_H
#define STATICMAPVIEW_H

class Map;

#include <QGraphicsView>
#include "projectdata.h"

class StaticMapView : public QGraphicsView
{
	Q_OBJECT
	public:
		StaticMapView(QWidget *parent=0);
		~StaticMapView();
		void setupView();
		void shutdownView();
		void updateMapView();
		void changeMap(Map *map);
		void changeZoom(int newZoom);
		int getCurrentZoom() const;
		enum Layer{LAYER_LOWER=0, LAYER_HERO, LAYER_UPPER};
		enum Zoom{ZOOM_1TO1=32, ZOOM_1TO2=16, ZOOM_1TO4=8, ZOOM_1TO8=4, ZOOM_3TO2=48};
	protected slots:
		void scrollBarMoved();
	protected:
		void resizeEvent(QResizeEvent *event);
		QGraphicsPixmapItem *item;
		QImage mapViewImage;
		QList<ProjectData::DataReference<Tileset>*> tilesets;
		Map *currentMap;
		int currentZoom;
		int mapWidth;
		int mapHeight;
		bool ignoreevents;
		bool shutdown;
};

#endif // STATICMAPVIEW_H
