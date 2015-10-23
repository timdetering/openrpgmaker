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

MenuFrame::MenuFrame(MenuObject *parent) : MenuObject(parent)
{
	objectName = "frame";
	type = MenuObject::TYPE_FRAME;
	x = 0;
	y = 0;
	width = 64;
	height = 64;
}

MenuFrame::MenuFrame(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_FRAME;
	loadXMLData(ObjectNode);
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
}

MenuFrame::MenuFrame(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_FRAME;
	loadXMLData(ObjectNode);
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
}

MenuFrame::MenuFrame(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent) : MenuObject(layout, ObjectNode, parent)
{
	type = MenuObject::TYPE_FRAME;
	loadXMLData(ObjectNode);
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
}

MenuFrame::MenuFrame(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	int numChildren;
	type = MenuObject::TYPE_FRAME;
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
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			childObjects.append(new MenuScrollArea(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects.append(new MenuText(layout, storageFile, this));
	}
}

MenuFrame::MenuFrame(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	int numChildren;
	type = MenuObject::TYPE_FRAME;
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
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			childObjects.append(new MenuScrollArea(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects.append(new MenuText(layout, storageFile, this));
	}
}

MenuFrame::MenuFrame(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent) : MenuObject(layout, storageFile, parent)
{
	int numChildren;
	type = MenuObject::TYPE_FRAME;
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
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			childObjects.append(new MenuScrollArea(layout, storageFile, this));
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects.append(new MenuText(layout, storageFile, this));
	}
}

MenuFrame::~MenuFrame()
{
}

void MenuFrame::getCopy(MenuObject *object)
{
	MenuFrame *frame = (MenuFrame*)object;
	MenuObject::getCopy(object);
	frame->resized();
}

XMLNode MenuFrame::getObjectNode()
{
	XMLNode ObjectNode = XMLNode::createXMLTopNode("object");
	ObjectNode.addAttribute("type", "frame");
	ObjectNode.addAttribute("objectName", objectName.toUtf8().data());
	ObjectNode.addAttribute("x", QString::number(x).toUtf8().data());
	ObjectNode.addAttribute("y", QString::number(y).toUtf8().data());
	ObjectNode.addAttribute("width", QString::number(width).toUtf8().data());
	ObjectNode.addAttribute("height", QString::number(height).toUtf8().data());
	for (int i = 0; i < childObjects.size(); ++i)
		ObjectNode.addChild(childObjects[i]->getObjectNode());
	return ObjectNode;
}

void MenuFrame::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putSignedInt(type);
	storageFile.putString(objectName.toUtf8().data());
	storageFile.putSignedInt(x);
	storageFile.putSignedInt(y);
	storageFile.putSignedInt(width);
	storageFile.putSignedInt(height);
	storageFile.putSignedInt(childObjects.size());
	for (int i = 0; i < childObjects.size(); ++i)
	{
		int objectType = childObjects[i]->getType();
		if (objectType == MenuObject::TYPE_FRAME)
			childObjects[i]->saveToStorageFile(storageFile);
		else if (objectType == MenuObject::TYPE_TEXT)
			childObjects[i]->saveToStorageFile(storageFile);
		else if (objectType == MenuObject::TYPE_IMAGE)
			childObjects[i]->saveToStorageFile(storageFile);
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
			childObjects[i]->saveToStorageFile(storageFile);
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			childObjects[i]->saveToStorageFile(storageFile);
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			childObjects[i]->saveToStorageFile(storageFile);
	}
}

bool MenuFrame::tryLoad(XMLNode ObjectNode)
{
	if (!XMLTools::attributeExists(ObjectNode, "x"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "y"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "width"))
		return false;
	if (!XMLTools::attributeExists(ObjectNode, "height"))
		return false;
	for (int i = 0; i < ObjectNode.nChildNode("object"); ++i)
	{
		if (!MenuObject::tryLoad(ObjectNode.getChildNode("object", i)))
			return false;
	}
	return true;
}

void MenuFrame::resized()
{
	QPainter painter;
	QPixmap frameBG = QPixmap(ProjectData::systemGraphics.frameBGLocation);
	QPixmap frameBorder = QPixmap(ProjectData::systemGraphics.frameBorderLocation);
	QPixmap frameBorder_ul = frameBorder.copy(0, 0, 16, 16);
	QPixmap frameBorder_u = frameBorder.copy(16, 0, 32, 16);
	QPixmap frameBorder_ur = frameBorder.copy(48, 0, 16, 16);
	QPixmap frameBorder_l = frameBorder.copy(0, 16, 16, 32);
	QPixmap frameBorder_r = frameBorder.copy(48, 16, 16, 32);
	QPixmap frameBorder_bl = frameBorder.copy(0, 48, 16, 16);
	QPixmap frameBorder_b = frameBorder.copy(16, 48, 32, 16);
	QPixmap frameBorder_br = frameBorder.copy(48, 48, 16, 16);
	frameBG.setMask(frameBG.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	frameBorder_ul.setMask(frameBorder_ul.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	frameBorder_u.setMask(frameBorder_u.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	frameBorder_ur.setMask(frameBorder_ur.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	frameBorder_l.setMask(frameBorder_l.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	frameBorder_r.setMask(frameBorder_r.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	frameBorder_bl.setMask(frameBorder_bl.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	frameBorder_b.setMask(frameBorder_b.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	frameBorder_br.setMask(frameBorder_br.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	renderedPixmap = QPixmap(width, height);
	renderedPixmap.fill(QColor(255, 0, 255));
	frameBorder = QPixmap(width, height);
	frameBorder.fill(QColor(255, 0, 255));
	painter.begin(&frameBorder);
	painter.drawTiledPixmap(16, 0, width - 32, 16, frameBorder_u);
	painter.drawTiledPixmap(16, height - 16, width - 32, 16, frameBorder_b);
	painter.drawTiledPixmap(0, 16, 16, height - 32, frameBorder_l);
	painter.drawTiledPixmap(width - 16, 16, 16, height - 32, frameBorder_r);
	painter.drawTiledPixmap(0, 0, 16, 16, frameBorder_ul);
	painter.drawTiledPixmap(width - 16, 0, 16, 16, frameBorder_ur);
	painter.drawTiledPixmap(0, height - 16, 16, 16, frameBorder_bl);
	painter.drawTiledPixmap(width - 16, height - 16, 16, 16, frameBorder_br);
	painter.end();
	frameBorder.setMask(frameBorder.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
	painter.begin(&renderedPixmap);
	if (ProjectData::systemGraphics.stretchToFit)
		painter.drawPixmap(0, 0, width, height, frameBG);
	else
		painter.drawTiledPixmap(0, 0, width, height, frameBG);
	painter.drawPixmap(0, 0, frameBorder);
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

void MenuFrame::loadXMLData(XMLNode ObjectNode)
{
	x = QString(ObjectNode.getAttribute("x")).toInt();
	y = QString(ObjectNode.getAttribute("y")).toInt();
	width = QString(ObjectNode.getAttribute("width")).toInt();
	height = QString(ObjectNode.getAttribute("height")).toInt();
}

void MenuFrame::loadStorageData(StorageFile &storageFile)
{
	x = storageFile.getSignedInt();
	y = storageFile.getSignedInt();
	width = storageFile.getSignedInt();
	height = storageFile.getSignedInt();
}
