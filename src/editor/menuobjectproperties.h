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

#ifndef MENUOBJECTPROPERTIES_H
#define MENUOBJECTPROPERTIES_H

#include <QTableWidget>

class MenuFrame;
class MenuImage;
class MenuItemTable;
class MenuObject;
class MenuObjectDelegate;
class MenuSelectableArea;
class MenuScrollArea;
class MenuText;

class MenuObjectProperties : public QTableWidget
{
	Q_OBJECT
	public:
		MenuObjectProperties(QWidget *parent);
		~MenuObjectProperties();
		void setCurrentObject(MenuObject *menuObject);
		void objectsNameChanged(QString newName);
	protected slots:
		void editMenuObjectItem(QTableWidgetItem *item);
	private:
		MenuObjectDelegate *objectDelegate;
		union
		{
			MenuFrame *frame;
			MenuImage *image;
			MenuItemTable *itemList;
			MenuScrollArea *scrollArea;
			MenuSelectableArea *selectableArea;
			MenuText *text;
		} object;
		int objectType;
};

#endif // MENUOBJECTPROPERTIES_H
