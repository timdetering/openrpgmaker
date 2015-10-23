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

#include <QPixmap>
#include "menuitemtable.h"
#include "menutableitem.h"

MenuTableItem::MenuTableItem(MenuItemTable *parentTable)
{
}

MenuTableItem::MenuTableItem(MenuItemTable *parentTable, QString text)
{
}

MenuTableItem::MenuTableItem(MenuItemTable *parentTable, QString iconLocation, QString text)
{
}

MenuTableItem::MenuTableItem(MenuItemTable *parentTable, XMLNode ItemNode)
{
}

MenuTableItem::MenuTableItem(MenuItemTable *parentTable, StorageFile &storageFile)
{
}

MenuTableItem::~MenuTableItem()
{
}

XMLNode MenuTableItem::getItemNode()
{
}

void MenuTableItem::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putString(text.toUtf8().data());
	storageFile.putString(iconLocation.toUtf8().data());
	storageFile.putBool(enabled);
}

MenuItemTable *MenuTableItem::getTable()
{
	return table;
}

QPixmap MenuTableItem::getPixmap()
{
	return pixmap;
}

QString MenuTableItem::getText()
{
	return text;
}

void MenuTableItem::setText(QString value)
{
	text = value;
}

QString MenuTableItem::getIconLocation()
{
	return iconLocation;
}

void MenuTableItem::setIconLocation(QString value)
{
	iconLocation = value;
}

bool MenuTableItem::isEnabled()
{
	return enabled;
}

void MenuTableItem::setEnabled(bool value)
{
	enabled = value;
}

bool MenuTableItem::tryLoad(XMLNode ItemNode)
{
	return true;
}

void MenuTableItem::updatePixmap()
{
}
