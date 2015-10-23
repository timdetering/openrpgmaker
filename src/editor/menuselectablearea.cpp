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
#include "menuselectablearea.h"
#include "menuscrollarea.h"
#include "menutext.h"
#include "projectdata.h"
#include "projectdata_baseevent.h"
#include "storagefile.h"
#include "xmlParser.h"
#include "xmlTools.h"

MenuSelectableArea::MenuSelectableArea(MenuObject *parent) : MenuObject(parent)
{
	objectName = "selectableArea";
	type = MenuObject::TYPE_SELECTABLEAREA;
	onHighlightEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onLeftArrowEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onRightArrowEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onUpArrowEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onDownArrowEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onSelectedEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onCancelEvent = new BaseEvent(BaseEvent::MENUEVENT);
	x = 0;
	y = 0;
	width = 64;
	height = 64;
}

MenuSelectableArea::MenuSelectableArea(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	QString objectType;
	XMLNode TempNode;
	type = MenuObject::TYPE_SELECTABLEAREA;
	loadXMLData(ObjectNode);
	for (int i = 0; i < ObjectNode.nChildNode("object"); ++i)
	{
		TempNode = ObjectNode.getChildNode("object", i);
		objectType = TempNode.getAttribute("type");
		if (objectType == "frame")
			childObjects.append(new MenuFrame(layout, TempNode, this));
		else if (objectType == "image")
			childObjects.append(new MenuImage(layout, TempNode, this));
		else if (objectType == "item list")
			childObjects.append(new MenuItemTable(layout, TempNode, this));
		else if (objectType == "selectable area")
			childObjects.append(new MenuSelectableArea(layout, TempNode, this));
		else if (objectType == "text")
			childObjects.append(new MenuText(layout, TempNode, this));
	}
}

MenuSelectableArea::MenuSelectableArea(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	QString objectType;
	XMLNode TempNode;
	type = MenuObject::TYPE_SELECTABLEAREA;
	loadXMLData(ObjectNode);
	for (int i = 0; i < ObjectNode.nChildNode("object"); ++i)
	{
		TempNode = ObjectNode.getChildNode("object", i);
		objectType = TempNode.getAttribute("type");
		if (objectType == "frame")
			childObjects.append(new MenuFrame(layout, TempNode, this));
		else if (objectType == "image")
			childObjects.append(new MenuImage(layout, TempNode, this));
		else if (objectType == "item list")
			childObjects.append(new MenuItemTable(layout, TempNode, this));
		else if (objectType == "selectable area")
			childObjects.append(new MenuSelectableArea(layout, TempNode, this));
		else if (objectType == "text")
			childObjects.append(new MenuText(layout, TempNode, this));
	}
}

MenuSelectableArea::MenuSelectableArea(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	QString objectType;
	XMLNode TempNode;
	type = MenuObject::TYPE_SELECTABLEAREA;
	loadXMLData(ObjectNode);
	for (int i = 0; i < ObjectNode.nChildNode("object"); ++i)
	{
		TempNode = ObjectNode.getChildNode("object", i);
		objectType = TempNode.getAttribute("type");
		if (objectType == "frame")
			childObjects.append(new MenuFrame(layout, TempNode, this));
		else if (objectType == "image")
			childObjects.append(new MenuImage(layout, TempNode, this));
		else if (objectType == "item list")
			childObjects.append(new MenuItemTable(layout, TempNode, this));
		else if (objectType == "selectable area")
			childObjects.append(new MenuSelectableArea(layout, TempNode, this));
		else if (objectType == "text")
			childObjects.append(new MenuText(layout, TempNode, this));
	}
}

MenuSelectableArea::MenuSelectableArea(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	int numChildren;
	type = MenuObject::TYPE_SELECTABLEAREA;
	loadStorageData(storageFile);
	numChildren = storageFile.getSignedInt();
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
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects.append(new MenuText(layout, storageFile, this));
	}
}

MenuSelectableArea::MenuSelectableArea(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	int numChildren;
	type = MenuObject::TYPE_SELECTABLEAREA;
	loadStorageData(storageFile);
	numChildren = storageFile.getSignedInt();
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
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects.append(new MenuText(layout, storageFile, this));
	}
}

MenuSelectableArea::MenuSelectableArea(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	int numChildren;
	type = MenuObject::TYPE_SELECTABLEAREA;
	loadStorageData(storageFile);
	numChildren = storageFile.getSignedInt();
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
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects.append(new MenuText(layout, storageFile, this));
	}
}

MenuSelectableArea::~MenuSelectableArea()
{
	if (onHighlightEvent != NULL)
	{
		delete onHighlightEvent;
		onHighlightEvent = NULL;
	}
	if (onLeftArrowEvent != NULL)
	{
		delete onLeftArrowEvent;
		onLeftArrowEvent = NULL;
	}
	if (onRightArrowEvent != NULL)
	{
		delete onRightArrowEvent;
		onRightArrowEvent = NULL;
	}
	if (onUpArrowEvent != NULL)
	{
		delete onUpArrowEvent;
		onUpArrowEvent = NULL;
	}
	if (onDownArrowEvent != NULL)
	{
		delete onDownArrowEvent;
		onDownArrowEvent = NULL;
	}
	if (onSelectedEvent != NULL)
	{
		delete onSelectedEvent;
		onSelectedEvent = NULL;
	}
	if (onCancelEvent != NULL)
	{
		delete onCancelEvent;
		onCancelEvent = NULL;
	}
}

void MenuSelectableArea::getCopy(MenuObject *object)
{
	MenuSelectableArea *selectableArea = (MenuSelectableArea*)object;
	MenuObject::getCopy(object);
	selectableArea->onHighlightEvent = new BaseEvent(onHighlightEvent);
	selectableArea->onLeftArrowEvent = new BaseEvent(onLeftArrowEvent);
	selectableArea->onRightArrowEvent = new BaseEvent(onRightArrowEvent);
	selectableArea->onUpArrowEvent = new BaseEvent(onUpArrowEvent);
	selectableArea->onDownArrowEvent = new BaseEvent(onDownArrowEvent);
	selectableArea->onSelectedEvent = new BaseEvent(onSelectedEvent);
	selectableArea->onCancelEvent = new BaseEvent(onCancelEvent);
	selectableArea->resized();
}

XMLNode MenuSelectableArea::getObjectNode()
{
	XMLNode TempNode1, TempNode2, ObjectNode = XMLNode::createXMLTopNode("object");
	ObjectNode.addAttribute("type", "selectable area");
	ObjectNode.addAttribute("objectName", objectName.toUtf8().data());
	ObjectNode.addAttribute("x", QString::number(x).toUtf8().data());
	ObjectNode.addAttribute("y", QString::number(y).toUtf8().data());
	ObjectNode.addAttribute("width", QString::number(width).toUtf8().data());
	ObjectNode.addAttribute("height", QString::number(height).toUtf8().data());
	TempNode1 = ObjectNode.addChild("events");
	TempNode2 = onHighlightEvent->getEventNode();
	TempNode2.updateName("highlighted");
	TempNode1.addChild(TempNode2);
	TempNode2 = onLeftArrowEvent->getEventNode();
	TempNode2.updateName("leftArrow");
	TempNode1.addChild(TempNode2);
	TempNode2 = onRightArrowEvent->getEventNode();
	TempNode2.updateName("rightArrow");
	TempNode1.addChild(TempNode2);
	TempNode2 = onUpArrowEvent->getEventNode();
	TempNode2.updateName("upArrow");
	TempNode1.addChild(TempNode2);
	TempNode2 = onDownArrowEvent->getEventNode();
	TempNode2.updateName("downArrow");
	TempNode1.addChild(TempNode2);
	TempNode2 = onSelectedEvent->getEventNode();
	TempNode2.updateName("selected");
	TempNode1.addChild(TempNode2);
	TempNode2 = onCancelEvent->getEventNode();
	TempNode2.updateName("canceled");
	TempNode1.addChild(TempNode2);
	for (int i = 0; i < childObjects.size(); ++i)
		ObjectNode.addChild(childObjects[i]->getObjectNode());
	return ObjectNode;
}

void MenuSelectableArea::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(type);
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(width);
	storageFile.putSignedInt(height);
	onHighlightEvent->saveToStorageFile(storageFile);
	onLeftArrowEvent->saveToStorageFile(storageFile);
	onRightArrowEvent->saveToStorageFile(storageFile);
	onUpArrowEvent->saveToStorageFile(storageFile);
	onDownArrowEvent->saveToStorageFile(storageFile);
	onSelectedEvent->saveToStorageFile(storageFile);
	onCancelEvent->saveToStorageFile(storageFile);
	storageFile.putSignedInt(childObjects.size());
	for (int i = 0; i < childObjects.size(); ++i)
	{
		int objectType = childObjects[i]->getType();
		if (objectType == MenuObject::TYPE_FRAME)
			((MenuFrame*)childObjects[i])->saveToStorageFile(storageFile);
		else if (objectType == MenuObject::TYPE_TEXT)
			((MenuText*)childObjects[i])->saveToStorageFile(storageFile);
		else if (objectType == MenuObject::TYPE_IMAGE)
			((MenuImage*)childObjects[i])->saveToStorageFile(storageFile);
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
			((MenuImage*)childObjects[i])->saveToStorageFile(storageFile);
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			((MenuItemTable*)childObjects[i])->saveToStorageFile(storageFile);
	}
}

QStringList MenuSelectableArea::getResources()
{
	QStringList resources;
	resources.append(onHighlightEvent->getResources());
	resources.append(onLeftArrowEvent->getResources());
	resources.append(onRightArrowEvent->getResources());
	resources.append(onUpArrowEvent->getResources());
	resources.append(onDownArrowEvent->getResources());
	resources.append(onSelectedEvent->getResources());
	resources.append(onCancelEvent->getResources());
	return resources;
}

void MenuSelectableArea::setHighlightEvent(BaseEvent *event)
{
	if (onHighlightEvent != NULL)
		delete onHighlightEvent;
	onHighlightEvent = event;
}

void MenuSelectableArea::setLeftArrowEvent(BaseEvent *event)
{
	if (onLeftArrowEvent != NULL)
		delete onLeftArrowEvent;
	onLeftArrowEvent = event;
}

void MenuSelectableArea::setRightArrowEvent(BaseEvent *event)
{
	if (onRightArrowEvent != NULL)
		delete onRightArrowEvent;
	onRightArrowEvent = event;
}

void MenuSelectableArea::setUpArrowEvent(BaseEvent *event)
{
	if (onUpArrowEvent != NULL)
		delete onUpArrowEvent;
	onUpArrowEvent = event;
}

void MenuSelectableArea::setDownArrowEvent(BaseEvent *event)
{
	if (onDownArrowEvent != NULL)
		delete onDownArrowEvent;
	onDownArrowEvent = event;
}

void MenuSelectableArea::setSelectedEvent(BaseEvent *event)
{
	if (onSelectedEvent != NULL)
		delete onSelectedEvent;
	onSelectedEvent = event;
}

void MenuSelectableArea::setCancelEvent(BaseEvent *event)
{
	if (onCancelEvent != NULL)
		delete onCancelEvent;
	onCancelEvent = event;
}

bool MenuSelectableArea::tryLoad(XMLNode ObjectNode)
{
	XMLNode TempNode;
	if (!XMLTools::attributeExists(ObjectNode, "x"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "y"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "width"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "height"))
		return false;
	if (!XMLTools::nodeExists(ObjectNode, "events"))
		return false;
	TempNode = ObjectNode.getChildNode("events");
	if (!XMLTools::nodeExists(TempNode, "highlighted"))
		return false;
	if (!XMLTools::nodeExists(TempNode, "leftArrow"))
		return false;
	if (!XMLTools::nodeExists(TempNode, "rightArrow"))
		return false;
	if (!XMLTools::nodeExists(TempNode, "upArrow"))
		return false;
	if (!XMLTools::nodeExists(TempNode, "downArrow"))
		return false;
	if (!XMLTools::nodeExists(TempNode, "selected"))
		return false;
	if (!XMLTools::nodeExists(TempNode, "canceled"))
		return false;
	if (!BaseEvent::tryLoad(TempNode.getChildNode("highlighted"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(TempNode.getChildNode("leftArrow"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(TempNode.getChildNode("rightArrow"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(TempNode.getChildNode("upArrow"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(TempNode.getChildNode("downArrow"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(TempNode.getChildNode("selected"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(TempNode.getChildNode("canceled"), BaseEvent::MENUEVENT))
		return false;
	for (int i = 0; i < ObjectNode.nChildNode("object"); ++i)
	{
		if (!MenuObject::tryLoad(ObjectNode.getChildNode("object", i)))
			return false;
	}
	return true;
}

void MenuSelectableArea::resized()
{
	QPixmap selectionBorder = QPixmap(ProjectData::systemGraphics.selectionBorderLocation);
	QPainter painter;
	renderedPixmap = QPixmap(width, height);
	painter.begin(&renderedPixmap);
	painter.fillRect(0, 0, width, height, QColor(255, 0, 255));
	for (int tempY = 16; tempY < height - 16; tempY += 32)
	{
		for (int tempX = 16; tempX < width - 16; tempX += 32)
			painter.drawPixmap(tempX, tempY, selectionBorder, 16, 16, 32, 32);
	}
	for (int i = 16; i < width - 16; i += 32)
	{
		painter.drawPixmap(i, 0, selectionBorder, 16, 0, 32, 16);
		painter.drawPixmap(i, height - 16, selectionBorder, 16, 48, 32, 16);
	}
	for (int i = 16; i < height - 16; i += 32)
	{
		painter.drawPixmap(0, i, selectionBorder, 0, 16, 16, 32);
		painter.drawPixmap(width - 16, i, selectionBorder, 48, 16, 16, 32);
	}
	painter.drawPixmap(0, 0, selectionBorder, 0, 0, 16, 16);
	painter.drawPixmap(width - 16, 0, selectionBorder, 48, 0, 16, 16);
	painter.drawPixmap(0, height - 16, selectionBorder, 0, 48, 16, 16);
	painter.drawPixmap(width - 16, height - 16, selectionBorder, 48, 48, 16, 16);
	painter.end();
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

void MenuSelectableArea::loadXMLData(XMLNode ObjectNode)
{
	XMLNode TempNode;
	x = QString(ObjectNode.getAttribute("x")).toInt();
	y = QString(ObjectNode.getAttribute("y")).toInt();
	width = QString(ObjectNode.getAttribute("width")).toInt();
	height = QString(ObjectNode.getAttribute("height")).toInt();
	TempNode = ObjectNode.getChildNode("events");
	onHighlightEvent = new BaseEvent(BaseEvent::MENUEVENT, TempNode.getChildNode("highlighted"));
	onLeftArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, TempNode.getChildNode("leftArrow"));
	onRightArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, TempNode.getChildNode("rightArrow"));
	onUpArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, TempNode.getChildNode("upArrow"));
	onDownArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, TempNode.getChildNode("downArrow"));
	onSelectedEvent = new BaseEvent(BaseEvent::MENUEVENT, TempNode.getChildNode("selected"));
	onCancelEvent = new BaseEvent(BaseEvent::MENUEVENT, TempNode.getChildNode("canceled"));
}

void MenuSelectableArea::loadStorageData(StorageFile &storageFile)
{
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	width = storageFile.getSignedInt();
	height = storageFile.getSignedInt();
	onHighlightEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onLeftArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onRightArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onUpArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onDownArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onSelectedEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onCancelEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
}
