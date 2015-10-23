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
#include <QPainter>
#include <QBitmap>
#include "menuframe.h"
#include "menuimage.h"
#include "menuitemtable.h"
#include "menuobject.h"
#include "menuscrollarea.h"
#include "menuselectablearea.h"
#include "menutext.h"
#include "projectdata.h"
#include "storagefile.h"
#include "xmlParser.h"
#include "xmlTools.h"

MenuScrollArea::MenuScrollArea(MenuObject *parent) : MenuObject(parent)
{
	objectName = "scrollArea";
	type = MenuObject::TYPE_SCROLLAREA;
	x = 0;
	y = 0;
	width = 64;
	height = 64;
	internalWidth = 64;
	internalHeight = 64;
	currentScrollX = 0;
	currentScrollY = 0;
	scrollArrowsItem = new QGraphicsPixmapItem(QPixmap(32, 32));
	scrollArrowsItem->setPos(width - 32, 0);
	scrollArrowsItem->setZValue(5000);
	scrollArrowsItem->setParentItem(pixmapItem);
}

MenuScrollArea::MenuScrollArea(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_SCROLLAREA;
	loadXMLData(ObjectNode);
	currentScrollX = 0;
	currentScrollY = 0;
	for (int i = 0; i < ObjectNode.nChildNode("object"); ++i)
	{
		XMLNode TempNode = ObjectNode.getChildNode("object", i);
		QString objectType = TempNode.getAttribute("type");
		if (objectType == "frame")
			childObjects.append(new MenuFrame(layout, TempNode, this));
		else if (objectType == "image")
			childObjects.append(new MenuImage(layout, TempNode, this));
		else if (objectType == "item list")
			childObjects.append(new MenuItemTable(layout, TempNode, this));
		else if (objectType == "selectable area")
			childObjects.append(new MenuSelectableArea(layout, TempNode, this));
		else if (objectType == "scroll area")
			childObjects.append(new MenuScrollArea(layout, TempNode, this));
		else if (objectType == "text")
			childObjects.append(new MenuText(layout, TempNode, this));
	}
	scrollArrowsItem = new QGraphicsPixmapItem(QPixmap(32, 32));
	scrollArrowsItem->setPos(width - 32, 0);
	scrollArrowsItem->setZValue(5000);
	scrollArrowsItem->setParentItem(pixmapItem);
}

MenuScrollArea::MenuScrollArea(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_SCROLLAREA;
	loadXMLData(ObjectNode);
	currentScrollX = 0;
	currentScrollY = 0;
	for (int i = 0; i < ObjectNode.nChildNode("object"); ++i)
	{
		XMLNode TempNode = ObjectNode.getChildNode("object", i);
		QString objectType = TempNode.getAttribute("type");
		if (objectType == "frame")
			childObjects.append(new MenuFrame(layout, TempNode, this));
		else if (objectType == "image")
			childObjects.append(new MenuImage(layout, TempNode, this));
		else if (objectType == "item list")
			childObjects.append(new MenuItemTable(layout, TempNode, this));
		else if (objectType == "selectable area")
			childObjects.append(new MenuSelectableArea(layout, TempNode, this));
		else if (objectType == "scroll area")
			childObjects.append(new MenuScrollArea(layout, TempNode, this));
		else if (objectType == "text")
			childObjects.append(new MenuText(layout, TempNode, this));
	}
	scrollArrowsItem = new QGraphicsPixmapItem(QPixmap(32, 32));
	scrollArrowsItem->setPos(width - 32, 0);
	scrollArrowsItem->setZValue(5000);
	scrollArrowsItem->setParentItem(pixmapItem);
}

MenuScrollArea::MenuScrollArea(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_SCROLLAREA;
	loadXMLData(ObjectNode);
	currentScrollX = 0;
	currentScrollY = 0;
	for (int i = 0; i < ObjectNode.nChildNode("object"); ++i)
	{
		XMLNode TempNode = ObjectNode.getChildNode("object", i);
		QString objectType = TempNode.getAttribute("type");
		if (objectType == "frame")
			childObjects.append(new MenuFrame(layout, TempNode, this));
		else if (objectType == "image")
			childObjects.append(new MenuImage(layout, TempNode, this));
		else if (objectType == "item list")
			childObjects.append(new MenuItemTable(layout, TempNode, this));
		else if (objectType == "selectable area")
			childObjects.append(new MenuSelectableArea(layout, TempNode, this));
		else if (objectType == "scroll area")
			childObjects.append(new MenuScrollArea(layout, TempNode, this));
		else if (objectType == "text")
			childObjects.append(new MenuText(layout, TempNode, this));
	}
	scrollArrowsItem = new QGraphicsPixmapItem(QPixmap(32, 32));
	scrollArrowsItem->setPos(width - 32, 0);
	scrollArrowsItem->setZValue(5000);
	scrollArrowsItem->setParentItem(pixmapItem);
}

MenuScrollArea::MenuScrollArea(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	int numChildren;
	type = MenuObject::TYPE_SCROLLAREA;
	loadStorageData(storageFile);
	numChildren = storageFile.getSignedInt();
	currentScrollX = 0;
	currentScrollY = 0;
	for (int i = 0; i < numChildren; ++i)
	{
		int objectType = storageFile.getSignedInt();
		if (objectType == MenuObject::TYPE_FRAME)
			childObjects.append(new MenuFrame(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_IMAGE)
			childObjects.append(new MenuImage(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			childObjects.append(new MenuItemTable(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
			childObjects.append(new MenuSelectableArea(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			childObjects.append(new MenuScrollArea(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects.append(new MenuText(layout, storageFile, this));
	}
	scrollArrowsItem = new QGraphicsPixmapItem(QPixmap(32, 32));
	scrollArrowsItem->setPos(width - 32, 0);
	scrollArrowsItem->setZValue(5000);
	scrollArrowsItem->setParentItem(pixmapItem);
}

MenuScrollArea::MenuScrollArea(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	int numChildren;
	type = MenuObject::TYPE_SCROLLAREA;
	loadStorageData(storageFile);
	numChildren = storageFile.getSignedInt();
	currentScrollX = 0;
	currentScrollY = 0;
	for (int i = 0; i < numChildren; ++i)
	{
		int objectType = storageFile.getSignedInt();
		if (objectType == MenuObject::TYPE_FRAME)
			childObjects.append(new MenuFrame(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_IMAGE)
			childObjects.append(new MenuImage(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			childObjects.append(new MenuItemTable(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
			childObjects.append(new MenuSelectableArea(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			childObjects.append(new MenuScrollArea(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects.append(new MenuText(layout, storageFile, this));
	}
	scrollArrowsItem = new QGraphicsPixmapItem(QPixmap(32, 32));
	scrollArrowsItem->setPos(width - 32, 0);
	scrollArrowsItem->setZValue(5000);
	scrollArrowsItem->setParentItem(pixmapItem);
}

MenuScrollArea::MenuScrollArea(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	int numChildren;
	type = MenuObject::TYPE_SCROLLAREA;
	loadStorageData(storageFile);
	numChildren = storageFile.getSignedInt();
	currentScrollX = 0;
	currentScrollY = 0;
	for (int i = 0; i < numChildren; ++i)
	{
		int objectType = storageFile.getSignedInt();
		if (objectType == MenuObject::TYPE_FRAME)
			childObjects.append(new MenuFrame(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_IMAGE)
			childObjects.append(new MenuImage(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			childObjects.append(new MenuItemTable(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
			childObjects.append(new MenuSelectableArea(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			childObjects.append(new MenuScrollArea(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects.append(new MenuText(layout, storageFile, this));
	}
	scrollArrowsItem = new QGraphicsPixmapItem(QPixmap(32, 32));
	scrollArrowsItem->setPos(width - 32, 0);
	scrollArrowsItem->setZValue(5000);
	scrollArrowsItem->setParentItem(pixmapItem);
}

MenuScrollArea::~MenuScrollArea()
{
	delete scrollArrowsItem;
	scrollArrowsItem = NULL;
}

void MenuScrollArea::getCopy(MenuObject *object)
{
	MenuScrollArea *scrollArea = (MenuScrollArea*)object;
	MenuObject::getCopy(object);
	scrollArea->internalWidth = internalWidth;
	scrollArea->internalHeight = internalHeight;
	scrollArea->currentScrollX = currentScrollX;
	scrollArea->currentScrollY = currentScrollY;
	scrollArea->resized();
}

XMLNode MenuScrollArea::getObjectNode()
{
	XMLNode ObjectNode = XMLNode::createXMLTopNode("object");
	ObjectNode.addAttribute("type", "scroll area");
	ObjectNode.addAttribute("objectName", objectName.toUtf8().data());
	ObjectNode.addAttribute("x", QString::number(x).toUtf8().data());
	ObjectNode.addAttribute("y", QString::number(y).toUtf8().data());
	ObjectNode.addAttribute("width", QString::number(width).toUtf8().data());
	ObjectNode.addAttribute("height", QString::number(height).toUtf8().data());
	ObjectNode.addAttribute("internalWidth", QString::number(internalWidth).toUtf8().data());
	ObjectNode.addAttribute("internalHeight", QString::number(internalHeight).toUtf8().data());
	for (int i = 0; i < childObjects.size(); ++i)
		ObjectNode.addChild(childObjects[i]->getObjectNode());
	return ObjectNode;
}

void MenuScrollArea::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(type);
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(width);
	storageFile.putSignedInt(height);
	storageFile.putSignedInt(internalWidth);
	storageFile.putSignedInt(internalHeight);
	storageFile.putSignedInt(childObjects.size());
	for (int i = 0; i < childObjects.size(); ++i)
		childObjects[i]->saveToStorageFile(storageFile);
}

int MenuScrollArea::getInternalWidth()
{
	return internalWidth;
}

void MenuScrollArea::setInternalWidth(int value)
{
	internalWidth = value;
	if (internalWidth < width)
		internalWidth = width;
}

int MenuScrollArea::getInternalHeight()
{
	return internalHeight;
}

void MenuScrollArea::setInternalHeight(int value)
{
	internalHeight = value;
	if (internalHeight < height)
		internalHeight = height;
}

int MenuScrollArea::getCurrentScrollX()
{
	return currentScrollX;
}

int MenuScrollArea::getCurrentScrollY()
{
	return currentScrollY;
}

void MenuScrollArea::scrollTo(int x, int y)
{
	currentScrollX = x;
	currentScrollY = y;
	if (currentScrollX < 0)
		currentScrollX = 0;
	if (currentScrollX + width > internalWidth)
		currentScrollX = internalWidth - width;
	if (currentScrollY < 0)
		currentScrollY = 0;
	if (currentScrollY + height > internalHeight)
		currentScrollY = internalHeight - height;
	updateChildLocations();
}

void MenuScrollArea::scrollBy(int dx, int dy)
{
	currentScrollX += dx;
	currentScrollY += dy;
	if (currentScrollX < 0)
		currentScrollX = 0;
	if (currentScrollX + width > internalWidth)
		currentScrollX = internalWidth - width;
	if (currentScrollY < 0)
		currentScrollY = 0;
	if (currentScrollY + height > internalHeight)
		currentScrollY = internalHeight - height;
	updateChildLocations();
}

void MenuScrollArea::ensureVisible(QRect region)
{
	if (region.width() > width || region.x() < currentScrollX)
		currentScrollX = region.x();
	else if (region.x() > currentScrollX && region.x() - currentScrollX + region.width() > width)
		currentScrollX = region.x() - width + region.width();
	if (region.height() > height || region.y() < currentScrollY)
		currentScrollY = region.y();
	else if (region.y() > currentScrollY && region.y() - currentScrollY + region.height() > height)
		currentScrollY = region.y() - height + region.height();
	updateChildLocations();
}

bool MenuScrollArea::tryLoad(XMLNode ObjectNode)
{
	if (!XMLTools::attributeExists(ObjectNode, "x"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "y"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "width"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "height"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "internalWidth"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "internalHeight"))
		return false;
	for (int i = 0; i < ObjectNode.nChildNode("object"); ++i)
	{
		if (!MenuObject::tryLoad(ObjectNode.getChildNode("object", i)))
			return false;
	}
	return true;
}

void MenuScrollArea::resized()
{
	QPainter painter;
	QPixmap scrollArrows = QPixmap(ProjectData::systemGraphics.scrollArrowsLocation);
	QPixmap scrollArrowsPixmap = QPixmap(32, height);
	if (internalWidth < width)
		internalWidth = width;
	if (internalHeight < height)
		internalHeight = height;
	scrollArrowsPixmap.fill(QColor(255, 0, 255));
	scrollArrows.setMask(scrollArrows.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	painter.begin(&scrollArrowsPixmap);
	painter.drawPixmap(0, 0, scrollArrows, 0, 0, 32, 16);
	painter.drawPixmap(0, height - 16, scrollArrows, 0, 16, 32, 16);
	painter.end();
	scrollArrowsPixmap.setMask(scrollArrowsPixmap.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	scrollArrowsItem->setPixmap(scrollArrowsPixmap);
	scrollArrowsItem->setPos(width - 32, 0);
	renderedPixmap = QPixmap(width, height);
	renderedPixmap.fill(QColor(255, 0, 255));
	if (parentObject == NULL)
		pixmapItem->setPos(x, y);
	else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
	{
		MenuScrollArea *scrollArea = (MenuScrollArea*)parentObject;
		pixmapItem->setPos(x - scrollArea->getCurrentScrollX(), y - scrollArea->getCurrentScrollY());
	}
	else
		pixmapItem->setPos(x, y);
	pixmapItem->setPixmap(renderedPixmap);
}

void MenuScrollArea::updateChildLocations()
{
	for (int i = 0; i < childObjects.size(); ++i)
	{
		QGraphicsPixmapItem *item = childObjects[i]->getPixmapItem();
		int childX = childObjects[i]->getX();
		int childY = childObjects[i]->getY();
		item->setPos(childX - currentScrollX, childY - currentScrollY);
	}
}

void MenuScrollArea::loadXMLData(XMLNode ObjectNode)
{
	x = QString(ObjectNode.getAttribute("x")).toInt();
	y = QString(ObjectNode.getAttribute("y")).toInt();
	width = QString(ObjectNode.getAttribute("width")).toInt();
	height = QString(ObjectNode.getAttribute("height")).toInt();
	internalWidth = QString(ObjectNode.getAttribute("internalWidth")).toInt();
	internalHeight = QString(ObjectNode.getAttribute("internalHeight")).toInt();
}

void MenuScrollArea::loadStorageData(StorageFile &storageFile)
{
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	width = storageFile.getSignedInt();
	height = storageFile.getSignedInt();
	internalWidth = storageFile.getSignedInt();
	internalHeight = storageFile.getSignedInt();
}
