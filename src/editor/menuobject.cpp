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

#include <QPainter>
#include "menuframe.h"
#include "menuimage.h"
#include "menuitemtable.h"
#include "menuobject.h"
#include "menuscrollarea.h"
#include "menuselectablearea.h"
#include "menutext.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"
#include "storagefile.h"
#include "xmlParser.h"
#include "xmlTools.h"

MenuObject::MenuObject(MenuObject *parent)
{
	parentObject = parent;
	renderedPixmap = QPixmap(32, 32);
	objectName = "object";
	x = 0;
	y = 0;
	width = 32;
	height = 32;
	pixmapItem = new QGraphicsPixmapItem(renderedPixmap);
	pixmapItem->setPos(x, y);
	pixmapItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
	if (parent != NULL)
	{
		pixmapItem->setZValue(parent->childObjects.size());
		parent->childObjects.append(this);
		pixmapItem->setParentItem(parent->pixmapItem);
	}
	else
		pixmapItem->setZValue(0);
	zValue = 0;
	mainMenuLayout = NULL;
	popupMenuLayout = NULL;
	titleMenuLayout = NULL;
}

MenuObject::MenuObject(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent)
{
	parentObject = parent;
	objectName = ObjectNode.getAttribute("objectName");
	mainMenuLayout = layout;
	popupMenuLayout = NULL;
	titleMenuLayout = NULL;
	pixmapItem = new QGraphicsPixmapItem(renderedPixmap);
	pixmapItem->setPos(x, y);
	pixmapItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
	if (parent != NULL)
	{
		zValue = parent->childObjects.size();
		pixmapItem->setZValue(zValue);
		pixmapItem->setParentItem(parent->pixmapItem);
	}
	else
	{
		zValue = mainMenuLayout->numTopLevelObjects();
		pixmapItem->setZValue(zValue);
	}
	mainMenuLayout->addObject(this);
}

MenuObject::MenuObject(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent)
{
	parentObject = parent;
	objectName = ObjectNode.getAttribute("objectName");
	mainMenuLayout = NULL;
	popupMenuLayout = layout;
	titleMenuLayout = NULL;
	pixmapItem = new QGraphicsPixmapItem(renderedPixmap);
	pixmapItem->setPos(x, y);
	pixmapItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
	if (parent != NULL)
	{
		zValue = parent->childObjects.size();
		pixmapItem->setZValue(zValue);
		pixmapItem->setParentItem(parent->pixmapItem);
	}
	else
	{
		zValue = popupMenuLayout->numTopLevelObjects();
		pixmapItem->setZValue(zValue);
	}
	popupMenuLayout->addObject(this);
}

MenuObject::MenuObject(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent)
{
	parentObject = parent;
	objectName = ObjectNode.getAttribute("objectName");
	mainMenuLayout = NULL;
	popupMenuLayout = NULL;
	titleMenuLayout = layout;
	pixmapItem = new QGraphicsPixmapItem(renderedPixmap);
	pixmapItem->setPos(x, y);
	pixmapItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
	if (parent != NULL)
	{
		zValue = parent->childObjects.size();
		pixmapItem->setZValue(zValue);
		pixmapItem->setParentItem(parent->pixmapItem);
	}
	else
	{
		zValue = titleMenuLayout->numTopLevelObjects();
		pixmapItem->setZValue(zValue);
	}
	titleMenuLayout->addObject(this);
}

MenuObject::MenuObject(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent)
{
	char *temp_string;
	parentObject = parent;
	temp_string = storageFile.getString();
	objectName = temp_string;
	delete[] temp_string;
	mainMenuLayout = layout;
	popupMenuLayout = NULL;
	titleMenuLayout = NULL;
	x = 0;
	y = 0;
	pixmapItem = new QGraphicsPixmapItem(renderedPixmap);
	pixmapItem->setPos(x, y);
	pixmapItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
	if (parent != NULL)
	{
		zValue = parent->childObjects.size();
		pixmapItem->setZValue(zValue);
		pixmapItem->setParentItem(parent->pixmapItem);
	}
	else
	{
		zValue = mainMenuLayout->numTopLevelObjects();
		pixmapItem->setZValue(zValue);
	}
	mainMenuLayout->addObject(this);
}

MenuObject::MenuObject(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent)
{
	char *temp_string;
	parentObject = parent;
	temp_string = storageFile.getString();
	objectName = temp_string;
	delete[] temp_string;
	mainMenuLayout = NULL;
	popupMenuLayout = layout;
	titleMenuLayout = NULL;
	x = 0;
	y = 0;
	pixmapItem = new QGraphicsPixmapItem(renderedPixmap);
	pixmapItem->setPos(x, y);
	pixmapItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
	if (parent != NULL)
	{
		zValue = parent->childObjects.size();
		pixmapItem->setZValue(zValue);
		pixmapItem->setParentItem(parent->pixmapItem);
	}
	else
	{
		zValue = popupMenuLayout->numTopLevelObjects();
		pixmapItem->setZValue(zValue);
	}
	popupMenuLayout->addObject(this);
}

MenuObject::MenuObject(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent)
{
	char *temp_string;
	parentObject = parent;
	temp_string = storageFile.getString();
	objectName = temp_string;
	delete[] temp_string;
	mainMenuLayout = NULL;
	popupMenuLayout = NULL;
	titleMenuLayout = layout;
	x = 0;
	y = 0;
	pixmapItem = new QGraphicsPixmapItem(renderedPixmap);
	pixmapItem->setPos(x, y);
	pixmapItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
	if (parent != NULL)
	{
		zValue = parent->childObjects.size();
		pixmapItem->setZValue(zValue);
		pixmapItem->setParentItem(parent->pixmapItem);
	}
	else
	{
		zValue = titleMenuLayout->numTopLevelObjects();
		pixmapItem->setZValue(zValue);
	}
	titleMenuLayout->addObject(this);
}

MenuObject::~MenuObject()
{
	for (int i = 0; i < childObjects.size(); ++i)
		delete (childObjects[i]);
	childObjects.clear();
	if (parentObject != NULL)
	{
		parentObject->childObjects.removeAll(this);
		parentObject->updateZValues();
		parentObject = NULL;
		if (mainMenuLayout != NULL)
			mainMenuLayout->removeObjectFromList(objectName);
		else if (popupMenuLayout != NULL)
			popupMenuLayout->removeObjectFromList(objectName);
		else if (titleMenuLayout != NULL)
			titleMenuLayout->removeObjectFromList(objectName);
	}
	else if (mainMenuLayout != NULL)
		mainMenuLayout->removeTopLevelObject(this);
	else if (popupMenuLayout != NULL)
		popupMenuLayout->removeTopLevelObject(this);
	else if (titleMenuLayout != NULL)
		titleMenuLayout->removeTopLevelObject(this);
	parentObject = NULL;
	mainMenuLayout = NULL;
	popupMenuLayout = NULL;
	titleMenuLayout = NULL;
	delete pixmapItem;
	pixmapItem = NULL;
}

void MenuObject::getCopy(MenuObject *object)
{
	object->objectName = objectName;
	object->type = type;
	object->x = x;
	object->y = y;
	object->width = width;
	object->height = height;
	object->zValue = zValue;
	object->enabled = enabled;
	for (int i = 0; i < childObjects.size(); ++i)
	{
		MenuObject *childObject = childObjects[i];
		MenuObject *copyObject;
		if (childObject->type == TYPE_FRAME)
		{
			copyObject = new MenuFrame(object);
			((MenuFrame*)childObject)->getCopy(copyObject);
		}
		else if (childObject->type == TYPE_TEXT)
		{
			copyObject = new MenuText(object);
			((MenuText*)childObject)->getCopy(copyObject);
		}
		else if (childObject->type == TYPE_IMAGE)
		{
			copyObject = new MenuImage(object);
			((MenuImage*)childObject)->getCopy(copyObject);
		}
		else if (childObject->type == TYPE_SELECTABLEAREA)
		{
			copyObject = new MenuSelectableArea(object);
			((MenuSelectableArea*)childObject)->getCopy(copyObject);
		}
		else if (childObject->type == TYPE_SCROLLAREA)
		{
			copyObject = new MenuScrollArea(object);
			((MenuScrollArea*)childObject)->getCopy(copyObject);
		}
		else if (childObject->type == TYPE_ITEMLIST)
		{
			copyObject = new MenuItemTable(object);
			((MenuItemTable*)childObject)->getCopy(copyObject);
		}
	}
}

QStringList MenuObject::getResources()
{
	QStringList resources;
	for (int i = 0; i < childObjects.size(); ++i)
		resources.append(childObjects[i]->getResources());
	return resources;
}

QGraphicsPixmapItem *MenuObject::getPixmapItem()
{
	return pixmapItem;
}

QPixmap MenuObject::getRenderedPixmap()
{
	return renderedPixmap;
}

QPixmap MenuObject::getPreviewPixmap()
{
	QPixmap pixmap = QPixmap(width, height);
	QPainter painter;
	pixmap.fill(QColor(255, 0, 255));
	painter.begin(&pixmap);
	painter.drawPixmap(0, 0, renderedPixmap);
	for (int i = 0; i < childObjects.size(); ++i)
	{
		QPixmap childPixmap = childObjects[i]->getPreviewPixmap();
		int x = childObjects[i]->getX();
		int y = childObjects[i]->getY();
		painter.drawPixmap(x, y, childPixmap);
	}
	painter.end();
	return pixmap;
}

QString MenuObject::getName()
{
	return objectName;
}

void MenuObject::setName(QString value, bool adjustLayout)
{
	if (adjustLayout)
	{
		if (mainMenuLayout != NULL)
			mainMenuLayout->changeObjectName(objectName, value);
		else if (popupMenuLayout != NULL)
			popupMenuLayout->objectList.insert(value, popupMenuLayout->objectList.take(objectName));
		else if (titleMenuLayout != NULL)
			titleMenuLayout->objectList.insert(value, titleMenuLayout->objectList.take(objectName));
	}
	else
		objectName = value;
}

int MenuObject::getX()
{
	return x;
}

void MenuObject::setX(int value)
{
	x = value;
	if (parentObject != NULL)
	{
		if (parentObject->getType() == TYPE_SCROLLAREA)
		{
			MenuScrollArea *scrollArea = (MenuScrollArea*)parentObject;
			pixmapItem->setPos(x - scrollArea->getCurrentScrollX(), y - scrollArea->getCurrentScrollY());
		}
		else
			pixmapItem->setPos(x, y);
	}
	else
		pixmapItem->setPos(x, y);
}

int MenuObject::getY()
{
	return y;
}

void MenuObject::setY(int value)
{
	y = value;
	if (parentObject != NULL)
	{
		if (parentObject->getType() == TYPE_SCROLLAREA)
		{
			MenuScrollArea *scrollArea = (MenuScrollArea*)parentObject;
			pixmapItem->setPos(x - scrollArea->getCurrentScrollX(), y - scrollArea->getCurrentScrollY());
		}
		else
			pixmapItem->setPos(x, y);
	}
	else
		pixmapItem->setPos(x, y);
}

int MenuObject::getWidth()
{
	return width;
}

void MenuObject::setWidth(int value)
{
	width = value;
	resized();
}

int MenuObject::getHeight()
{
	return height;
}

void MenuObject::setHeight(int value)
{
	height = value;
	resized();
}

void MenuObject::setPos(int newX, int newY)
{
	x = newX;
	y = newY;
	if (parentObject != NULL)
	{
		if (parentObject->getType() == TYPE_SCROLLAREA)
		{
			MenuScrollArea *scrollArea = (MenuScrollArea*)parentObject;
			pixmapItem->setPos(x - scrollArea->getCurrentScrollX(), y - scrollArea->getCurrentScrollY());
		}
		else
			pixmapItem->setPos(x, y);
	}
	else
		pixmapItem->setPos(x, y);
}

void MenuObject::resize(int newWidth, int newHeight)
{
	if (newWidth != width || newHeight != height)
	{
		width = newWidth;
		height = newHeight;
		resized();
	}
}

int MenuObject::getType()
{
	return type;
}

MenuObject *MenuObject::getParent()
{
	return parentObject;
}

void MenuObject::setParent(MenuObject *parent)
{
	MenuObject *object;
	if (parentObject != NULL)
		parentObject->childObjects.removeAll(this);
	parentObject = parent;
	if (parentObject != NULL)
	{
		pixmapItem->setZValue(parentObject->childObjects.size());
		parentObject->childObjects.append(this);
		pixmapItem->setParentItem(parentObject->pixmapItem);
		mainMenuLayout = parent->mainMenuLayout;
		popupMenuLayout = parent->popupMenuLayout;
		titleMenuLayout = parent->titleMenuLayout;
		if (mainMenuLayout != NULL)
			mainMenuLayout->addObject(this);
		else if (popupMenuLayout != NULL)
			popupMenuLayout->addObject(this);
		else if (titleMenuLayout != NULL)
			titleMenuLayout->addObject(this);
	}
	else if (mainMenuLayout != NULL)
		mainMenuLayout->topLevelObjects.append(this);
	else if (popupMenuLayout != NULL)
		popupMenuLayout->topLevelObjects.append(this);
	else if (titleMenuLayout != NULL)
		titleMenuLayout->topLevelObjects.append(this);
	object = parentObject;
	while (object != NULL)
	{
		x -= object->x;
		y -= object->y;
		if (object->getType() == TYPE_SCROLLAREA)
		{
			x += ((MenuScrollArea*)object)->getCurrentScrollX();
			y += ((MenuScrollArea*)object)->getCurrentScrollY();
		}
		object = object->parentObject;
	}
	if (parentObject == NULL)
		pixmapItem->setPos(x, y);
	else if (parentObject->getType() == MenuObject::TYPE_SCROLLAREA)
	{
		MenuScrollArea *scrollArea = (MenuScrollArea*)parentObject;
		pixmapItem->setPos(x - scrollArea->getCurrentScrollX(), y - scrollArea->getCurrentScrollY());
	}
	else
		pixmapItem->setPos(x, y);
}

void MenuObject::changeParent(MenuObject *newParent, bool adjust)
{
	if (parentObject != NULL)
		parentObject->childObjects.removeAll(this);
	else if (mainMenuLayout != NULL)
		mainMenuLayout->topLevelObjects.removeAll(this);
	else if (popupMenuLayout != NULL)
		popupMenuLayout->topLevelObjects.removeAll(this);
	else if (titleMenuLayout != NULL)
		titleMenuLayout->topLevelObjects.removeAll(this);
	parentObject = newParent;
	if (parentObject != NULL)
	{
		pixmapItem->setZValue(parentObject->childObjects.size());
		pixmapItem->setParentItem(parentObject->pixmapItem);
		parentObject->childObjects.append(this);
	}
	else if (mainMenuLayout != NULL)
	{
		pixmapItem->setZValue(mainMenuLayout->topLevelObjects.size());
		pixmapItem->setParentItem(NULL);
		mainMenuLayout->topLevelObjects.append(this);
	}
	else if (popupMenuLayout != NULL)
	{
		pixmapItem->setZValue(popupMenuLayout->topLevelObjects.size());
		pixmapItem->setParentItem(NULL);
		popupMenuLayout->topLevelObjects.append(this);
	}
	else if (titleMenuLayout != NULL)
	{
		pixmapItem->setZValue(titleMenuLayout->topLevelObjects.size());
		pixmapItem->setParentItem(NULL);
		titleMenuLayout->topLevelObjects.append(this);
	}
	if (adjust)
		addToLayout();
}

void MenuObject::setLayout(MainMenuLayout *layout)
{
	mainMenuLayout = layout;
	popupMenuLayout = NULL;
	titleMenuLayout = NULL;
	if (parentObject == NULL)
	{
		pixmapItem->setZValue(layout->numTopLevelObjects());
		layout->addTopLevelObject(this);
	}
	else
		layout->addObject(this);
	for (int i = 0; i < childObjects.size(); ++i)
		childObjects[i]->setLayout(layout);
}

void MenuObject::setLayout(PopupMenuLayout *layout)
{
	mainMenuLayout = NULL;
	popupMenuLayout = layout;
	titleMenuLayout = NULL;
	if (parentObject == NULL)
	{
		pixmapItem->setZValue(layout->numTopLevelObjects());
		layout->addTopLevelObject(this);
	}
	else
		layout->addObject(this);
	for (int i = 0; i < childObjects.size(); ++i)
		childObjects[i]->setLayout(layout);
}

void MenuObject::setLayout(TitleMenuLayout *layout)
{
	mainMenuLayout = NULL;
	popupMenuLayout = NULL;
	titleMenuLayout = layout;
	if (parentObject == NULL)
	{
		pixmapItem->setZValue(layout->numTopLevelObjects());
		layout->addTopLevelObject(this);
	}
	else
		layout->addObject(this);
	for (int i = 0; i < childObjects.size(); ++i)
		childObjects[i]->setLayout(layout);
}

int MenuObject::numChildObjects()
{
	return childObjects.size();
}

int MenuObject::numChildObjectsByType(int type)
{
	int num = 0;
	for (int i = 0; i < childObjects.size(); ++i)
	{
		if (childObjects[i]->type == type)
			++num;
	}
	return num;
}

MenuObject *MenuObject::getChildObject(int childID)
{
	return childObjects[childID];
}

QList<MenuObject*> MenuObject::getChildObjectsByType(int type)
{
	QList<MenuObject*> objects;
	for (int i = 0; i < childObjects.size(); ++i)
	{
		if (childObjects[i]->type == type)
			objects.append(childObjects[i]);
	}
	return objects;
}

QList<MenuObject*> MenuObject::getChildObjects()
{
	return childObjects;
}

bool MenuObject::isEnabled()
{
	return enabled;
}

void MenuObject::setEnabled(bool value)
{
	if (enabled != value)
	{
		enabled = value;
		if (type == TYPE_FRAME)
			((MenuFrame*)this)->resized();
		else if (type == TYPE_TEXT)
			((MenuText*)this)->updatePixmap();
		else if (type == TYPE_IMAGE)
			((MenuImage*)this)->updatePixmap();
		else if (type == TYPE_ITEMLIST)
			((MenuItemTable*)this)->resized();
	}
}

int MenuObject::getZValue()
{
	return zValue;
}

void MenuObject::setZValue(int value)
{
	zValue = value;
	//pixmapItem->setZValue(value);
}

void MenuObject::updateZValues()
{
	for (int i = 0; i < childObjects.size(); ++i)
	{
		childObjects[i]->setZValue(i);
		childObjects[i]->updateZValues();
	}
}

void MenuObject::addToLayout()
{
	if (mainMenuLayout != NULL)
		mainMenuLayout->addObject(this);
	else if (popupMenuLayout != NULL)
		popupMenuLayout->addObject(this);
	else if (titleMenuLayout != NULL)
		titleMenuLayout->addObject(this);
	for (int i = 0; i < childObjects.size(); ++i)
		childObjects[i]->addToLayout();
}

void MenuObject::removeFromLayout()
{
	mainMenuLayout = NULL;
	popupMenuLayout = NULL;
	titleMenuLayout = NULL;
	for (int i = 0; i < childObjects.size(); ++i)
		childObjects[i]->removeFromLayout();
}

bool MenuObject::tryLoad(XMLNode ObjectNode)
{
	QStringList typeList;
	QString objectType;
	typeList << "frame";
	typeList << "image";
	typeList << "item list";
	typeList << "selectable area";
	typeList << "scroll area";
	typeList << "text";
	if (!XMLTools::attributeExists(ObjectNode, "type"))
		return false;
	if (!XMLTools::attributeStringValid(ObjectNode, "type", typeList))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "objectName"))
		return false;
	objectType = ObjectNode.getAttribute("type");
	if (objectType == "frame")
		return MenuFrame::tryLoad(ObjectNode);
	else if (objectType == "image")
		return MenuImage::tryLoad(ObjectNode);
	else if (objectType == "item list")
		return MenuItemTable::tryLoad(ObjectNode);
	else if (objectType == "selectable area")
		return MenuSelectableArea::tryLoad(ObjectNode);
	else if (objectType == "scroll area")
		return MenuScrollArea::tryLoad(ObjectNode);
	else if (objectType == "text")
		return MenuText::tryLoad(ObjectNode);
	return false; // Shouldn't reach this point
}
