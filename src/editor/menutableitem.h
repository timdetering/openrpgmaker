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

#ifndef MENUTABLEITEM_H
#define MENUTABLEITEM_H

class MenuItemTable;

#include <QString>
#include <QPixmap>
#include "menuitemtable.h"
#include "storagefile.h"
#include "xmlParser.h"

class MenuTableItem
{
	public:
		friend class MenuObject;
		friend class MainMenuLayout;
		friend class PopupMenuLayout;
		friend class TitleMenuLayout;
		friend class MenuItemList;
		MenuTableItem(MenuItemTable *parentTable);
		MenuTableItem(MenuItemTable *parentTable, QString text="");
		MenuTableItem(MenuItemTable *parentTable, QString iconLocation, QString text="");
		MenuTableItem(MenuItemTable *parentTable, XMLNode ItemNode);
		MenuTableItem(MenuItemTable *parentTable, StorageFile &storageFile);
		~MenuTableItem();
		XMLNode getItemNode();
		void saveToStorageFile(StorageFile &storageFile);
		MenuItemTable *getTable();
		QPixmap getPixmap();
		QString getText();
		void setText(QString value);
		QString getIconLocation();
		void setIconLocation(QString value);
		bool isEnabled();
		void setEnabled(bool value);
		static bool tryLoad(XMLNode ItemNode);
	private:
		void updatePixmap();
		MenuItemTable *table;
		QPixmap pixmap;
		QString objectName;
		QString text;
		QString iconLocation;
		bool enabled;
};

#endif // MENUTABLEITEM
