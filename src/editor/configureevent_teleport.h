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

#ifndef CONFIGUREEVENT_TELEPORT_H
#define CONFIGUREEVENT_TELEPORT_H

#include <QGraphicsRectItem>
#include "ui_configureevent_teleport.h"

class ConfigureEvent_Teleport : public QDialog, public Ui::ConfigureEvent_Teleport
{
	Q_OBJECT
	public:
		enum Facing {RETAIN_CURRENT_FACING=0, FACE_UP, FACE_RIGHT, FACE_DOWN, FACE_LEFT};
		ConfigureEvent_Teleport(QWidget *parent=0);
		~ConfigureEvent_Teleport();
		void setLocation(int mapID, int x, int y, int layer);
		void setIsStoredLocation(bool stored);
		void setFacing(int facing);
		int getMapID();
		int getX();
		int getY();
		int getLayer();
		bool isStoredLocation();
		int getFacing();
	protected slots:
		void on_bBrowseMapVariables_clicked();
		void on_bBrowseXVariables_clicked();
		void on_bBrowseYVariables_clicked();
		void on_bBrowseLayerVariables_clicked();
		void on_rbSpecificLocation_toggled(bool checked);
		void on_twMapList_itemCollapsed(QTreeWidgetItem *item);
		void on_twMapList_itemSelectionChanged();
	private:
		void buildMapTree(QTreeWidgetItem *parentItem, QTreeWidgetItem *parentMapItem);
		bool selectMap(QTreeWidgetItem *parentItem, int mapID);
};

#endif // CONFIGUREEVENT_TELEPORT_H
