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
*  Copyright (C) 2010, Justin Davis <tuxdavis@gmail.com>                   *
****************************************************************************/

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "menuframe.h"
#include "menuimage.h"
#include "menuitemtable.h"
#include "menuobject.h"
#include "menuselectablearea.h"
#include "menuscrollarea.h"
#include "menutableitem.h"
#include "menutext.h"
#include "projectdata.h"
#include "projectdata_baseevent.h"
#include "projectdata_menunavigation.h"
#include "projectdata_titlemenulayout.h"
#include "xmlParser.h"
#include "xmlTools.h"


TitleMenuLayout::TitleMenuLayout()
{
	onAcceptEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onCancelEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onLoadEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onLeaveEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onUpArrowEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onDownArrowEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onLeftArrowEvent = new BaseEvent(BaseEvent::MENUEVENT);
	onRightArrowEvent = new BaseEvent(BaseEvent::MENUEVENT);
	graphicsScene = NULL;
	navigation = new MenuNavigation();
}

TitleMenuLayout::TitleMenuLayout(XMLNode MenuNode)
{
	onAcceptEvent = new BaseEvent(BaseEvent::MENUEVENT, MenuNode.getChildNode("onAcceptEvent"));
	onCancelEvent = new BaseEvent(BaseEvent::MENUEVENT, MenuNode.getChildNode("onCancelEvent"));
	onLoadEvent = new BaseEvent(BaseEvent::MENUEVENT, MenuNode.getChildNode("onLoadEvent"));
	onLeaveEvent = new BaseEvent(BaseEvent::MENUEVENT, MenuNode.getChildNode("onLeaveEvent"));
	onUpArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, MenuNode.getChildNode("onUpArrowEvent"));
	onDownArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, MenuNode.getChildNode("onDownArrowEvent"));
	onLeftArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, MenuNode.getChildNode("onLeftArrowEvent"));
	onRightArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, MenuNode.getChildNode("onRightArrowEvent"));
	if (MenuNode.nChildNode("navigation") == 1)
		navigation = new MenuNavigation(MenuNode.getChildNode("navigation"));
	else
		navigation = new MenuNavigation();
	for (int i = 0; i < MenuNode.nChildNode("object"); ++i)
	{
		XMLNode TempNode = MenuNode.getChildNode("object", i);
		QString objectType = TempNode.getAttribute("type");
		if (objectType == "frame")
			topLevelObjects.append(new MenuFrame(this, TempNode, NULL));
		else if (objectType == "image")
			topLevelObjects.append(new MenuImage(this, TempNode, NULL));
		else if (objectType == "item list")
			topLevelObjects.append(new MenuItemTable(this, TempNode, NULL));
		else if (objectType == "selectable area")
			topLevelObjects.append(new MenuSelectableArea(this, TempNode, NULL));
		else if (objectType == "scroll area")
			topLevelObjects.append(new MenuScrollArea(this, TempNode, NULL));
		else if (objectType == "text")
			topLevelObjects.append(new MenuText(this, TempNode, NULL));
	}
	graphicsScene = NULL;
}

TitleMenuLayout::TitleMenuLayout(StorageFile &storageFile)
{
	int numObjects;
	onAcceptEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onCancelEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onLoadEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onLeaveEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onUpArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onDownArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onLeftArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	onRightArrowEvent = new BaseEvent(BaseEvent::MENUEVENT, storageFile);
	navigation = new MenuNavigation(storageFile);
	numObjects = storageFile.getSignedInt();
	for (int i = 0; i < numObjects; ++i)
	{
		int objectType = storageFile.getSignedInt();
		if (objectType == MenuObject::TYPE_FRAME)
			topLevelObjects.append(new MenuFrame(this, storageFile, NULL));
		else if (objectType == MenuObject::TYPE_IMAGE)
			topLevelObjects.append(new MenuImage(this, storageFile, NULL));
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			topLevelObjects.append(new MenuItemTable(this, storageFile, NULL));
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
			topLevelObjects.append(new MenuSelectableArea(this, storageFile, NULL));
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			topLevelObjects.append(new MenuScrollArea(this, storageFile, NULL));
		else if (objectType == MenuObject::TYPE_TEXT)
			topLevelObjects.append(new MenuText(this, storageFile, NULL));
	}
	graphicsScene = NULL;
}

TitleMenuLayout::~TitleMenuLayout()
{
	if (onAcceptEvent != NULL)
	{
		delete onAcceptEvent;
		onAcceptEvent = NULL;
	}
	if (onCancelEvent != NULL)
	{
		delete onCancelEvent;
		onCancelEvent = NULL;
	}
	if (onLoadEvent != NULL)
	{
		delete onLoadEvent;
		onLoadEvent = NULL;
	}
	if (onLeaveEvent != NULL)
	{
		delete onLeaveEvent;
		onLeaveEvent = NULL;
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
	if (graphicsScene != NULL)
		unsetGraphicsScene();
	if (navigation != NULL)
	{
		delete navigation;
		navigation = NULL;
	}
	while (topLevelObjects.size() > 0)
		delete (topLevelObjects[0]);
	objectList.clear();
}

void TitleMenuLayout::saveToStorageFile(StorageFile &storageFile)
{
	onAcceptEvent->saveToStorageFile(storageFile);
	onCancelEvent->saveToStorageFile(storageFile);
	onLoadEvent->saveToStorageFile(storageFile);
	onLeaveEvent->saveToStorageFile(storageFile);
	onUpArrowEvent->saveToStorageFile(storageFile);
	onDownArrowEvent->saveToStorageFile(storageFile);
	onLeftArrowEvent->saveToStorageFile(storageFile);
	onRightArrowEvent->saveToStorageFile(storageFile);
	navigation->saveToStorageFile(storageFile);
	storageFile.putSignedInt(topLevelObjects.size());
	for (int i = 0; i < topLevelObjects.size(); ++i)
		topLevelObjects[i]->saveToStorageFile(storageFile);
}

QStringList TitleMenuLayout::getResources()
{
	QStringList resources;
	for (int i = 0; i < topLevelObjects.size(); ++i)
		resources.append(topLevelObjects[i]->getResources());
	resources.append(onAcceptEvent->getResources());
	resources.append(onCancelEvent->getResources());
	resources.append(onLoadEvent->getResources());
	resources.append(onLeaveEvent->getResources());
	resources.append(onUpArrowEvent->getResources());
	resources.append(onDownArrowEvent->getResources());
	resources.append(onLeftArrowEvent->getResources());
	resources.append(onRightArrowEvent->getResources());
	return resources;
}

XMLNode TitleMenuLayout::getMenuNode()
{
	XMLNode TempNode, MenuNode = XMLNode::createXMLTopNode("titlemenulayout");
	TempNode = onAcceptEvent->getEventNode();
	TempNode.updateName("onAcceptEvent");
	MenuNode.addChild(TempNode);
	TempNode = onCancelEvent->getEventNode();
	TempNode.updateName("onCancelEvent");
	MenuNode.addChild(TempNode);
	TempNode = onLoadEvent->getEventNode();
	TempNode.updateName("onLoadEvent");
	MenuNode.addChild(TempNode);
	TempNode = onLeaveEvent->getEventNode();
	TempNode.updateName("onLeaveEvent");
	MenuNode.addChild(TempNode);
	TempNode = onUpArrowEvent->getEventNode();
	TempNode.updateName("onUpArrowEvent");
	MenuNode.addChild(TempNode);
	TempNode = onDownArrowEvent->getEventNode();
	TempNode.updateName("onDownArrowEvent");
	MenuNode.addChild(TempNode);
	TempNode = onLeftArrowEvent->getEventNode();
	TempNode.updateName("onLeftArrowEvent");
	MenuNode.addChild(TempNode);
	TempNode = onRightArrowEvent->getEventNode();
	TempNode.updateName("onRightArrowEvent");
	MenuNode.addChild(TempNode);
	TempNode = navigation->getNavigationNode();
	if (TempNode.nChildNode("object") > 0)
		MenuNode.addChild(TempNode);
	for (int i = 0; i < topLevelObjects.size(); ++i)
		MenuNode.addChild(topLevelObjects[i]->getObjectNode());
	return MenuNode;
}

MenuObject *TitleMenuLayout::getObjectByName(QString name)
{
	if (objectList.contains(name))
		return objectList[name];
	return NULL;
}

MenuObject *TitleMenuLayout::getTopLevelObject(int objectID)
{
	return topLevelObjects[objectID];
}

void TitleMenuLayout::changeObjectName(QString oldName, QString newName)
{
	if (objectList.contains(oldName))
	{
		MenuObject *object = objectList.take(oldName);
		object->setName(newName, false);
		addObject(object);
	}
	navigation->changeObjectName(oldName, newName);
	onAcceptEvent->applyChangeMenuObjectName(oldName, newName);
	onCancelEvent->applyChangeMenuObjectName(oldName, newName);
	onLoadEvent->applyChangeMenuObjectName(oldName, newName);
	onLeaveEvent->applyChangeMenuObjectName(oldName, newName);
	onUpArrowEvent->applyChangeMenuObjectName(oldName, newName);
	onDownArrowEvent->applyChangeMenuObjectName(oldName, newName);
	onLeftArrowEvent->applyChangeMenuObjectName(oldName, newName);
	onRightArrowEvent->applyChangeMenuObjectName(oldName, newName);
}

void TitleMenuLayout::addTopLevelObject(MenuObject *object)
{
	addObject(object);
	topLevelObjects.append(object);
	if (graphicsScene != NULL)
		graphicsScene->addItem(object->getPixmapItem());
}

void TitleMenuLayout::addObject(MenuObject *object)
{
	QString name = object->getName();
	int objectID = 1;
	while (objectList.contains(name))
	{
		++objectID;
		name.remove(QRegExp("_\\d+$"));
		name += QString("_%1").arg(objectID);
	}
	object->setName(name, false);
	objectList.insert(name, object);
}

void TitleMenuLayout::deleteTopLevelObject(MenuObject *object)
{
	int objectID = -1;
	for (int i = 0; i < topLevelObjects.size() && objectID == -1; ++i)
	{
		if (topLevelObjects[i]->getName() == object->getName())
			objectID = i;
	}
	if (objectID != -1)
	{
		delete (topLevelObjects[objectID]);
		topLevelObjects[objectID] = NULL;
		topLevelObjects.removeAt(objectID);
	}
}

void TitleMenuLayout::removeTopLevelObject(MenuObject *object)
{
	int objectID = topLevelObjects.indexOf(object);
	if (objectID != -1)
	{
		if (graphicsScene != NULL)
			graphicsScene->removeItem(topLevelObjects[objectID]->getPixmapItem());
		removeObjectFromList(topLevelObjects[objectID]->getName());
		topLevelObjects.removeAt(objectID);
	}
}

void TitleMenuLayout::removeObjectFromList(QString objectName)
{
	if (objectList.contains(objectName))
	{
		MenuObject *object = objectList[objectName];
		for (int i = 0; i < object->childObjects.size(); ++i)
			removeObjectFromList(object->childObjects[i]->getName());
		objectList.remove(objectName);
	}
}

int TitleMenuLayout::numTopLevelObjects()
{
	return topLevelObjects.size();
}

int TitleMenuLayout::numObjectsByType(int type)
{
	QMap<QString, MenuObject*>::iterator objectIter;
	int num = 0;
	for (objectIter = objectList.begin(); objectIter != objectList.end(); ++objectIter)
	{
		if (objectIter.value()->getType() == type)
			++num;
	}
	return num;
}

QList<MenuObject*> TitleMenuLayout::getObjectsByType(int type)
{
	QMap<QString, MenuObject*>::iterator objectIter;
	QList<MenuObject*> objects;
	for (objectIter = objectList.begin(); objectIter != objectList.end(); ++objectIter)
	{
		if (objectIter.value()->getType() == type)
			objects.append(objectIter.value());
	}
	return objects;
}

QList<MenuObject*> TitleMenuLayout::getTopLevelObjectsByType(int type)
{
	QList<MenuObject*> objects;
	for (int i = 0; i < topLevelObjects.size(); ++i)
	{
		if (topLevelObjects[i]->getType() == type)
			objects.append(topLevelObjects[i]);
	}
	return objects;
}

QList<MenuObject*> TitleMenuLayout::getTopLevelObjects()
{
	return topLevelObjects;
}

QStringList TitleMenuLayout::getObjectNamesByType(int type)
{
	QMap<QString, MenuObject*>::iterator objectIter;
	QStringList objectNames;
	for (objectIter = objectList.begin(); objectIter != objectList.end(); ++objectIter)
	{
		if (objectIter.value()->getType() == type)
			objectNames.append(objectIter.key());
	}
	return objectNames;
}

QStringList TitleMenuLayout::getTopLevelObjectNamesByType(int type)
{
	QStringList objectNames;
	for (int i = 0; i < topLevelObjects.size(); ++i)
	{
		if (topLevelObjects[i]->getType() == type)
			objectNames.append(topLevelObjects[i]->getName());
	}
	return objectNames;
}

QStringList TitleMenuLayout::getTopLevelObjectNames()
{
	QStringList objectNames;
	for (int i = 0; i < topLevelObjects.size(); ++i)
		objectNames.append(topLevelObjects[i]->getName());
	return objectNames;
}

BaseEvent *TitleMenuLayout::getOnAcceptEvent()
{
	return onAcceptEvent;
}

BaseEvent *TitleMenuLayout::getOnCancelEvent()
{
	return onCancelEvent;
}

BaseEvent *TitleMenuLayout::getOnLoadEvent()
{
	return onLoadEvent;
}

BaseEvent *TitleMenuLayout::getOnLeaveEvent()
{
	return onLeaveEvent;
}

BaseEvent *TitleMenuLayout::getOnUpArrowEvent()
{
	return onUpArrowEvent;
}

BaseEvent *TitleMenuLayout::getOnDownArrowEvent()
{
	return onDownArrowEvent;
}

BaseEvent *TitleMenuLayout::getOnLeftArrowEvent()
{
	return onLeftArrowEvent;
}

BaseEvent *TitleMenuLayout::getOnRightArrowEvent()
{
	return onRightArrowEvent;
}

MenuNavigation *TitleMenuLayout::getNavigation()
{
	return navigation;
}

void TitleMenuLayout::setGraphicsScene(QGraphicsScene *value)
{
	graphicsScene = value;
	for (int i = 0; i < topLevelObjects.size(); ++i)
		graphicsScene->addItem(topLevelObjects[i]->getPixmapItem());
}

void TitleMenuLayout::unsetGraphicsScene()
{
	for (int i = 0; i < topLevelObjects.size(); ++i)
		graphicsScene->removeItem(topLevelObjects[i]->getPixmapItem());
	graphicsScene = NULL;
}

void TitleMenuLayout::updateMenuObjects()
{
	QMap<QString, MenuObject*>::iterator objectIter;
	for (objectIter = objectList.begin(); objectIter != objectList.end(); ++objectIter)
	{
		MenuObject *object = objectIter.value();
		if (object->getType() == MenuObject::TYPE_FRAME)
			((MenuFrame*)object)->resized();
		else if (object->getType() == MenuObject::TYPE_TEXT)
			((MenuText*)object)->updatePixmap();
		else if (object->getType() == MenuObject::TYPE_IMAGE)
			((MenuImage*)object)->updatePixmap();
		else if (object->getType() == MenuObject::TYPE_SELECTABLEAREA)
			((MenuSelectableArea*)object)->resized();
		else if (object->getType() == MenuObject::TYPE_SCROLLAREA)
			((MenuScrollArea*)object)->resized();
		else if (object->getType() == MenuObject::TYPE_ITEMLIST)
			((MenuItemTable*)object)->resized();
	}
}

void TitleMenuLayout::updateZValues()
{
	for (int i = 0; i < topLevelObjects.size(); ++i)
	{
		topLevelObjects[i]->setZValue(i);
		topLevelObjects[i]->updateZValues();
	}
}

void TitleMenuLayout::applyMenuNavigation()
{
	MenuSelectableArea *sourceObject;
	QString sourceObjectName, destObjectName;
	int minRow = navigation->getMinRow();
	int minColumn = navigation->getMinColumn();
	int numRows = navigation->getNumRows();
	int numColumns = navigation->getNumColumns();
	for (int row = 0; row < numRows; ++row)
	{
		for (int column = 0; column < numColumns; ++column)
		{
			sourceObjectName = navigation->objectNameAtLocation(QPoint(column + minColumn, row + minRow));
			if (!sourceObjectName.isEmpty())
			{
				sourceObject = (MenuSelectableArea*)(objectList[sourceObjectName]);
				destObjectName = getObjectNameAbovePoint(row + minRow, column + minColumn);
				if (destObjectName != sourceObjectName)
					sourceObject->setUpArrowEvent(createArrowEvent(destObjectName));
				destObjectName = getObjectNameBelowPoint(row + minRow, column + minColumn);
				if (destObjectName != sourceObjectName)
					sourceObject->setDownArrowEvent(createArrowEvent(destObjectName));
				destObjectName = getObjectNameLeftOfPoint(row + minRow, column + minColumn);
				if (destObjectName != sourceObjectName)
					sourceObject->setLeftArrowEvent(createArrowEvent(destObjectName));
				destObjectName = getObjectNameRightOfPoint(row + minRow, column + minColumn);
				if (destObjectName != sourceObjectName)
					sourceObject->setRightArrowEvent(createArrowEvent(destObjectName));
			}
		}
	}
}

bool TitleMenuLayout::tryLoad(XMLNode MenuNode)
{
	XMLNode TempNode;
	QStringList objectTypeList;
	objectTypeList << "frame";
	objectTypeList << "text";
	objectTypeList << "image";
	objectTypeList << "selectable area";
	objectTypeList << "scroll area";
	objectTypeList << "item list";
	if (!XMLTools::nodeExists(MenuNode, "onAcceptEvent"))
		return false;
	if (!XMLTools::nodeExists(MenuNode, "onCancelEvent"))
		return false;
	if (!XMLTools::nodeExists(MenuNode, "onLoadEvent"))
		return false;
	if (!XMLTools::nodeExists(MenuNode, "onLeaveEvent"))
		return false;
	if (!XMLTools::nodeExists(MenuNode, "onUpArrowEvent"))
		return false;
	if (!XMLTools::nodeExists(MenuNode, "onDownArrowEvent"))
		return false;
	if (!XMLTools::nodeExists(MenuNode, "onLeftArrowEvent"))
		return false;
	if (!XMLTools::nodeExists(MenuNode, "onRightArrowEvent"))
		return false;
	if (!BaseEvent::tryLoad(MenuNode.getChildNode("onAcceptEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(MenuNode.getChildNode("onCancelEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(MenuNode.getChildNode("onLoadEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(MenuNode.getChildNode("onLeaveEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(MenuNode.getChildNode("onUpArrowEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(MenuNode.getChildNode("onDownArrowEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(MenuNode.getChildNode("onLeftArrowEvent"), BaseEvent::MENUEVENT))
		return false;
	if (!BaseEvent::tryLoad(MenuNode.getChildNode("onRightArrowEvent"), BaseEvent::MENUEVENT))
		return false;
	if (MenuNode.nChildNode("navigation") == 1)
	{
		if (!MenuNavigation::tryLoad(MenuNode.getChildNode("navigation")))
			return false;
	}
	for (int i = 0, count = MenuNode.nChildNode("object"); i < count; ++i)
	{
		if (!MenuObject::tryLoad(MenuNode.getChildNode("object", i)))
			return false;
	}
	return true;
}

BaseEvent *TitleMenuLayout::createArrowEvent(QString objectName)
{
	XMLNode TempNode, EventNode = XMLNode::createXMLTopNode("event");
	QList<MenuObject*> objectTree;
	MenuObject *parent = objectList[objectName]->getParent();
	objectTree.append(objectList[objectName]);
	while (parent != NULL)
	{
		objectTree.append(parent);
		parent = parent->getParent();
	}
	EventNode.addAttribute("type", "topLevelEvent");
	TempNode = EventNode.addChild("event");
	TempNode.addAttribute("type", "playSystemSoundEffect");
	TempNode.addAttribute("effect", "Cursor");
	TempNode = EventNode.addChild("event");
	TempNode.addAttribute("type", "changeSelectedArea");
	TempNode.addAttribute("objectName", objectName.toUtf8().data());
	for (int i = 1; i < objectTree.size(); ++i)
	{
		if (objectTree[i]->getType() == MenuObject::TYPE_SCROLLAREA)
		{
			TempNode = EventNode.addChild("event");
			TempNode.addAttribute("type", "scrollArea");
			TempNode.addAttribute("objectName", objectTree[i]->getName().toUtf8().data());
			TempNode.addAttribute("scrollTo", "childObject");
			TempNode.addAttribute("childName", objectTree[i - 1]->getName().toUtf8().data());
		}
	}
	objectTree.clear();
	return new BaseEvent(BaseEvent::MENUEVENT, EventNode);
}

QString TitleMenuLayout::getObjectNameAbovePoint(int row, int column)
{
	QString objectName = "";
	int minRow = navigation->getMinRow();
	int numRows = navigation->getNumRows();
	int curRow = row;
	while (objectName.isEmpty())
	{
		--curRow;
		if (curRow < minRow)
			curRow += numRows;
		objectName = navigation->objectNameAtLocation(QPoint(column, curRow));
	}
	return objectName;
}

QString TitleMenuLayout::getObjectNameBelowPoint(int row, int column)
{
	QString objectName = "";
	int minRow = navigation->getMinRow();
	int numRows = navigation->getNumRows();
	int curRow = row;
	while (objectName.isEmpty())
	{
		++curRow;
		if (curRow - minRow >= numRows)
			curRow -= numRows;
		objectName = navigation->objectNameAtLocation(QPoint(column, curRow));
	}
	return objectName;
}

QString TitleMenuLayout::getObjectNameLeftOfPoint(int row, int column)
{
	QString objectName = "";
	int minColumn = navigation->getMinColumn();
	int numColumns = navigation->getNumColumns();
	int curColumn = column;
	while (objectName.isEmpty())
	{
		--curColumn;
		if (curColumn < minColumn)
			curColumn += numColumns;
		objectName = navigation->objectNameAtLocation(QPoint(curColumn, row));
	}
	return objectName;
}

QString TitleMenuLayout::getObjectNameRightOfPoint(int row, int column)
{
	QString objectName = "";
	int minColumn = navigation->getMinColumn();
	int numColumns = navigation->getNumColumns();
	int curColumn = column;
	while (objectName.isEmpty())
	{
		++curColumn;
		if (curColumn - minColumn >= numColumns)
			curColumn -= numColumns;
		objectName = navigation->objectNameAtLocation(QPoint(curColumn, row));
	}
	return objectName;
}

void TitleMenuLayout::checkObjectList()
{
	QMap<QString, MenuObject*>::iterator objectIter;
	for (objectIter = objectList.begin(); objectIter != objectList.end(); ++objectIter)
	{
		if (objectIter.value() == NULL)
			printf("  error: \"%s\"\n", objectIter.key().toUtf8().data());
	}
}
