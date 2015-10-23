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

#ifndef SELECTMAPLOCATION_H
#define SELECTMAPLOCATION_H

#include "ui_selectmaplocation.h"
#include "projectdata.h"
#include "projectdata_map.h"

class SelectMapLocation : public QDialog, public Ui::SelectMapLocation
{
	Q_OBJECT
	public:
		SelectMapLocation(int mapID, QWidget *parent=0);
		~SelectMapLocation();
		void setLocation(int x, int y, int layer);
		int getXLocation();
		int getYLocation();
		int getLayer();
		void setLocationStoredInVariables(bool stored);
		bool isLocationStoredInVariables();
	protected slots:
		void on_bBrowseXVariables_clicked();
		void on_bBrowseYVariables_clicked();
		void on_bBrowseLayerVariables_clicked();
		void on_rbSpecificLocation_toggled(bool checked);
};

#endif // SELECTMAPLOCATION_H
