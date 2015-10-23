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

#ifndef CONFIGUREEVENT_SWAPTILE_H
#define CONFIGUREEVENT_SWAPTILE_H

#include "ui_configureevent_swaptile.h"

class ConfigureEvent_SwapTile : public QDialog, public Ui::ConfigureEvent_SwapTile
{
	Q_OBJECT
	public:
		ConfigureEvent_SwapTile(int map, QWidget *parent=0);
		~ConfigureEvent_SwapTile();
		int getXLocation();
		void setXLocation(int value);
		int getYLocation();
		void setYLocation(int value);
		int getLayer();
		void setLayer(int value);
		int getTilesetID();
		int getTileID();
		void setTile(int tilesetID, int tileID);
		bool isLocationStoredInVariables();
		void setLocationStoredInVariables(bool value);
		bool isTileReferencedInVariables();
		void setTileReferencedInVariables(bool referenced);
	protected slots:
		void on_bBrowseTileVariables_clicked();
		void on_bBrowseTilesetVariables_clicked();
		void on_bSelectLocation_clicked();
		void on_rbSpecificTile_toggled(bool checked);
	private:
		int mapID;
		int currentX;
		int currentY;
		int layer;
		bool locationStoredInVariables;
};

#endif // CONFIGUREEVENT_SWAPTILE_H
