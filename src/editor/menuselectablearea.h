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

#ifndef MENUSELECTABLEAREA_H
#define MENUSELECTABLEAREA_H

class BaseEvent;

#include "menuobject.h"
#include "xmlParser.h"

class MenuSelectableArea : public MenuObject
{
	public:
		friend class MenuObject;
		friend class MainMenuLayout;
		friend class PopupMenuLayout;
		friend class TitleMenuLayout;
		friend class MenuItemList;
		MenuSelectableArea(MenuObject *parent=0);
		MenuSelectableArea(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuSelectableArea(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuSelectableArea(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuSelectableArea(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuSelectableArea(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuSelectableArea(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		~MenuSelectableArea();
		void getCopy(MenuObject *object);
		XMLNode getObjectNode();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		inline BaseEvent *getHighlightEvent() {return onHighlightEvent;}
		inline BaseEvent *getLeftArrowEvent() {return onLeftArrowEvent;}
		inline BaseEvent *getRightArrowEvent() {return onRightArrowEvent;}
		inline BaseEvent *getUpArrowEvent() {return onUpArrowEvent;}
		inline BaseEvent *getDownArrowEvent() {return onDownArrowEvent;}
		inline BaseEvent *getSelectedEvent() {return onSelectedEvent;}
		inline BaseEvent *getCancelEvent() {return onCancelEvent;}
		void setHighlightEvent(BaseEvent *event);
		void setLeftArrowEvent(BaseEvent *event);
		void setRightArrowEvent(BaseEvent *event);
		void setUpArrowEvent(BaseEvent *event);
		void setDownArrowEvent(BaseEvent *event);
		void setSelectedEvent(BaseEvent *event);
		void setCancelEvent(BaseEvent *event);
		static bool tryLoad(XMLNode ObjectNode);
	protected:
		virtual void resized();
		BaseEvent *onHighlightEvent;
		BaseEvent *onLeftArrowEvent;
		BaseEvent *onRightArrowEvent;
		BaseEvent *onUpArrowEvent;
		BaseEvent *onDownArrowEvent;
		BaseEvent *onSelectedEvent;
		BaseEvent *onCancelEvent;
	private:
		void loadXMLData(XMLNode ObjectNode);
		void loadStorageData(StorageFile &storageFile);
};

#endif // MENUSELECTABLEAREA_H
