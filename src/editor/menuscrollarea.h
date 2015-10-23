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

#ifndef MENUSCROLLAREA_H
#define MENUSCROLLAREA_H

#include "menuobject.h"
#include "xmlParser.h"

class MenuScrollArea : public MenuObject
{
	public:
		friend class MenuObject;
		friend class MainMenuLayout;
		friend class PopupMenuLayout;
		friend class TitleMenuLayout;
		friend class MenuItemList;
		MenuScrollArea(MenuObject *parent=0);
		MenuScrollArea(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuScrollArea(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuScrollArea(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuScrollArea(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuScrollArea(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuScrollArea(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		~MenuScrollArea();
		void getCopy(MenuObject *object);
		XMLNode getObjectNode();
		void saveToStorageFile(StorageFile &storageFile);
		int getInternalWidth();
		void setInternalWidth(int value);
		int getInternalHeight();
		void setInternalHeight(int value);
		int getCurrentScrollX();
		int getCurrentScrollY();
		void scrollTo(int x, int y);
		void scrollBy(int dx, int dy);
		void ensureVisible(QRect region);
		static bool tryLoad(XMLNode ObjectNode);
	protected:
		virtual void resized();
		void updateChildLocations();
		QGraphicsPixmapItem *scrollArrowsItem;
		int internalWidth;
		int internalHeight;
		int currentScrollX;
		int currentScrollY;
	private:
		void loadXMLData(XMLNode ObjectNode);
		void loadStorageData(StorageFile &storageFile);
};

#endif // MENUFRAME_H
