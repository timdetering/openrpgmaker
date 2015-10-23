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

#include <QListWidget>
#include <QMouseEvent>
#include <QWidget>
#include <QDrag>
#include <QMimeData>
#include <QMetaType>
#include <QByteArray>
#include <QPixmap>
#include <QBitmap>
#include "menuframe.h"
#include "menuimage.h"
#include "menuitemlist.h"
#include "menuitemtable.h"
#include "menuobject.h"
#include "menuscrollarea.h"
#include "menuselectablearea.h"
#include "menutext.h"
#include "projectdata.h"

MenuItemList::MenuItemList(QWidget *parent) : QListWidget(parent)
{
	scaleFactor = 1.0;
	addItem(new QListWidgetItem(QIcon(":/icons/menuFrame.png"), "Frame"));
	addItem(new QListWidgetItem(QIcon(":/icons/menuText.png"), "Text"));
	addItem(new QListWidgetItem(QIcon(":/icons/menuImage.png"), "Image"));
	addItem(new QListWidgetItem(QIcon(":/icons/menuSelectionArea.png"), "Selectable Area"));
	addItem(new QListWidgetItem(QIcon(":/icons/menuScrollArea.png"), "Scroll Area"));
	addItem(new QListWidgetItem(QIcon(":/icons/menuList.png"), "Item List"));
}

MenuItemList::~MenuItemList()
{
}

void MenuItemList::setScaleFactor(float factor)
{
	scaleFactor = factor;
}

MenuObject *MenuItemList::getDropObject()
{
	return dropObject;
}

int MenuItemList::getDropObjectType()
{
	return dropObjectType;
}

void MenuItemList::mousePressEvent(QMouseEvent *event)
{
	QListWidget::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
		dragStartPos = event->pos();
}

void MenuItemList::mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) != 0 && dragStartPos.x() >= 0 && (event->pos() - dragStartPos).manhattanLength() >= QApplication::startDragDistance() && currentItem() != NULL)
	{
		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData;
		QPixmap pixmap;
		Qt::DropAction dropAction;
		createDropObject();
		pixmap = dropObject->getRenderedPixmap();
		mimeData->setData(QMetaType::typeName(QMetaType::Int), QByteArray::number(dropObjectType));
		drag->setMimeData(mimeData);
		drag->setPixmap(pixmap.scaled(pixmap.width() * scaleFactor, pixmap.height() * scaleFactor));
		drag->setHotSpot(QPoint(0, 0));
		dragStartPos = QPoint(-1, -1);
		dropAction = drag->exec(Qt::CopyAction);
		if (dropAction == Qt::IgnoreAction)
			deleteDropObject();
	}
}

void MenuItemList::createDropObject()
{
	dropObjectType = currentRow();
	if (dropObjectType == MENU_FRAME)
	{
		dropObject = new MenuFrame;
		((MenuFrame*)dropObject)->resized();
	}
	else if (dropObjectType == MENU_TEXT)
	{
		dropObject = new MenuText;
		((MenuText*)dropObject)->updatePixmap();
	}
	else if (dropObjectType == MENU_IMAGE)
	{
		dropObject = new MenuImage;
		((MenuImage*)dropObject)->updatePixmap();
	}
	else if (dropObjectType == MENU_SELECTABLEAREA)
	{
		dropObject = new MenuSelectableArea;
		((MenuSelectableArea*)dropObject)->resized();
	}
	else if (dropObjectType == MENU_SCROLLAREA)
	{
		dropObject = new MenuScrollArea;
		((MenuScrollArea*)dropObject)->resized();
	}
	else if (dropObjectType == MENU_ITEMLIST)
	{
		dropObject = new MenuItemTable;
		((MenuItemTable*)dropObject)->resized();
	}
}

void MenuItemList::deleteDropObject()
{
	if (dropObjectType == MENU_FRAME)
		delete ((MenuFrame*)dropObject);
	else if (dropObjectType == MENU_TEXT)
		delete ((MenuText*)dropObject);
	else if (dropObjectType == MENU_IMAGE)
		delete ((MenuImage*)dropObject);
	else if (dropObjectType == MENU_SELECTABLEAREA)
		delete ((MenuSelectableArea*)dropObject);
	else if (dropObjectType == MENU_SCROLLAREA)
		delete ((MenuScrollArea*)dropObject);
	else if (dropObjectType == MENU_ITEMLIST)
		delete ((MenuItemTable*)dropObject);
}
