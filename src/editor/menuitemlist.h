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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef MENUITEMLIST_H
#define MENUITEMLIST_H

#include <QListWidget>
#include <QMouseEvent>
#include <QWidget>
#include <QDrag>
#include <QMimeData>
#include <QMetaType>
#include <QByteArray>
#include "menuobject.h"

class MenuItemList : public QListWidget
{
	public:
		enum MenuItem
		{
			MENU_FRAME=0,
			MENU_TEXT=1,
			MENU_IMAGE=2,
			MENU_SELECTABLEAREA=3,
			MENU_SCROLLAREA=4,
			MENU_ITEMLIST=5
		};
		MenuItemList(QWidget *parent=0);
		~MenuItemList();
		void setScaleFactor(float factor);
		MenuObject *getDropObject();
		int getDropObjectType();
	protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
	private:
		void createDropObject();
		void deleteDropObject();
		QPoint dragStartPos;
		MenuObject *dropObject;
		float scaleFactor;
		int dropObjectType;
};

#endif // MENUITEMLIST_H
