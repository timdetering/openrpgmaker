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

#ifndef MENUOBJECT_H
#define MENUOBJECT_H

#include <QString>
#include <QPixmap>
#include <QList>
#include <QGraphicsPixmapItem>
#include "storagefile.h"
#include "xmlParser.h"

class MainMenuLayout;
class PopupMenuLayout;
class TitleMenuLayout;
class QGraphicsPixmapItem;

class MenuObject
{
	public:
		friend class MainMenuLayout;
		friend class PopupMenuLayout;
		friend class TitleMenuLayout;
		friend class MenuFrame;
		friend class MenuImage;
		friend class MenuItemTable;
		friend class MenuScrollArea;
		friend class MenuSelectableArea;
		friend class MenuText;
		friend class MenuItemList;
		enum MenuObjectType
		{
			TYPE_FRAME=0,
			TYPE_TEXT=1,
			TYPE_IMAGE=2,
			TYPE_SELECTABLEAREA=3,
			TYPE_SCROLLAREA=4,
			TYPE_ITEMLIST=5
		};
		enum MenuType
		{
			MENUTYPE_MAINMENU=0,
			MENUTYPE_POPUPMENU,
			MENUTYPE_TITLESCREENMENU
		};
		MenuObject(MenuObject *parent=0);
		MenuObject(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuObject(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuObject(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuObject(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuObject(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuObject(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		virtual ~MenuObject();
		virtual void getCopy(MenuObject *object);
		virtual XMLNode getObjectNode()=0;
		virtual void saveToStorageFile(StorageFile &storageFile)=0;
		virtual QStringList getResources();
		QGraphicsPixmapItem *getPixmapItem();
		QPixmap getRenderedPixmap();
		QPixmap getPreviewPixmap();
		QString getName();
		void setName(QString value, bool adjustLayout=true);
		int getX();
		void setX(int value);
		int getY();
		void setY(int value);
		int getWidth();
		void setWidth(int value);
		int getHeight();
		void setHeight(int value);
		void setPos(int newX, int newY);
		void resize(int newWidth, int newHeight);
		int getType();
		MenuObject *getParent();
		void setParent(MenuObject *parent);
		void changeParent(MenuObject *newParent, bool adjust=false);
		void setLayout(MainMenuLayout *layout);
		void setLayout(PopupMenuLayout *layout);
		void setLayout(TitleMenuLayout *layout);
		int numChildObjects();
		int numChildObjectsByType(int type);
		MenuObject *getChildObject(int childID);
		QList<MenuObject*> getChildObjectsByType(int type);
		QList<MenuObject*> getChildObjects();
		bool isEnabled();
		void setEnabled(bool value);
		int getZValue();
		void setZValue(int value);
		void updateZValues();
		void addToLayout();
		void removeFromLayout();
		static bool tryLoad(XMLNode ObjectNode);
	protected:
		virtual void resized() {}
		MenuObject *parentObject;
		QList<MenuObject*> childObjects;
		QGraphicsPixmapItem *pixmapItem;
		QPixmap renderedPixmap;
		QString objectName;
		int type;
		int x;
		int y;
		int width;
		int height;
		int zValue;
		bool enabled;
	private:
		MainMenuLayout *mainMenuLayout;
		PopupMenuLayout *popupMenuLayout;
		TitleMenuLayout *titleMenuLayout;
};

#endif // MENUOBJECT_H
