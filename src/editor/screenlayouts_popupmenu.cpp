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
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QBrush>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QCheckBox>
#include <QMimeData>
#include <QToolTip>
#include <QMenu>
#include <QMessageBox>
#include "changepopupmenusize.h"
#include "clipboard.h"
#include "configuremenuimage.h"
#include "configuremenuitemlist.h"
#include "configuremenutext.h"
#include "menueventeditor.h"
#include "menuframe.h"
#include "menuimage.h"
#include "menuitemtable.h"
#include "menunavigationcreator.h"
#include "menuobject.h"
#include "menupreview.h"
#include "menuselectablearea.h"
#include "menuscrollarea.h"
#include "menutext.h"
#include "projectdata.h"
#include "projectdata_popupmenulayout.h"
#include "screenlayouts_popupmenu.h"
#include "storagefile.h"

ScreenLayouts_PopupMenu::ScreenLayouts_PopupMenu(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	popupMenuScene = new QGraphicsScene(0, 0, 64, 64);
	gvPopupMenuLayout->setScene(popupMenuScene);
	gvPopupMenuLayout->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	resizeInProgress = false;
	repositionInProgress = false;
	ignoreEvents = false;
	objectMoved = false;
	wSnapToGrid->setEnabled(false);
	aDeleteItem->setShortcut(QKeySequence::Delete);
	aCut->setShortcut(QKeySequence::Cut);
	aCopy->setShortcut(QKeySequence::Copy);
	aPaste->setShortcut(QKeySequence::Paste);
	addAction(aDeleteItem);
	addAction(aCut);
	addAction(aCopy);
	addAction(aPaste);
	layoutRef = NULL;
}

ScreenLayouts_PopupMenu::~ScreenLayouts_PopupMenu()
{
	if (layoutRef != NULL)
	{
		delete layoutRef;
		layoutRef = NULL;
	}
}

void ScreenLayouts_PopupMenu::setupLayout()
{
	layoutRef = NULL;
	menuBackgroundItem = new QGraphicsRectItem(0, 0, 64, 64);
	menuBackgroundItem->setPen(Qt::NoPen);
	menuBackgroundItem->setBrush(QBrush(QColor(255, 0, 255)));
	menuBackgroundItem->setZValue(-2);
	popupMenuScene->addItem(menuBackgroundItem);
	menuGridItem = new QGraphicsRectItem(0, 0, 64, 64);
	menuGridItem->setBrush(QBrush(QImage(":/images/menugrid_2x2.png")));
	menuGridItem->setZValue(-1);
	menuGridItem->setVisible(false);
	popupMenuScene->addItem(menuGridItem);
	resizeItem_topLeft = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_topLeft->setPen(QPen(Qt::NoPen));
	resizeItem_topLeft->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_topLeft->setCursor(Qt::SizeFDiagCursor);
	resizeItem_topLeft->setPos(0, 0);
	resizeItem_topLeft->setZValue(1000);
	popupMenuScene->addItem(resizeItem_topLeft);
	resizeItem_top = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_top->setPen(QPen(Qt::NoPen));
	resizeItem_top->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_top->setCursor(Qt::SizeVerCursor);
	resizeItem_top->setPos(32, 0);
	resizeItem_top->setZValue(1000);
	popupMenuScene->addItem(resizeItem_top);
	resizeItem_topRight = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_topRight->setPen(QPen(Qt::NoPen));
	resizeItem_topRight->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_topRight->setCursor(Qt::SizeBDiagCursor);
	resizeItem_topRight->setPos(64, 0);
	resizeItem_topRight->setZValue(1000);
	popupMenuScene->addItem(resizeItem_topRight);
	resizeItem_left = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_left->setPen(QPen(Qt::NoPen));
	resizeItem_left->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_left->setCursor(Qt::SizeHorCursor);
	resizeItem_left->setPos(0, 32);
	resizeItem_left->setZValue(1000);
	popupMenuScene->addItem(resizeItem_left);
	resizeItem_right = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_right->setPen(QPen(Qt::NoPen));
	resizeItem_right->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_right->setCursor(Qt::SizeHorCursor);
	resizeItem_right->setPos(64, 32);
	resizeItem_right->setZValue(1000);
	popupMenuScene->addItem(resizeItem_right);
	resizeItem_bottomLeft = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_bottomLeft->setPen(QPen(Qt::NoPen));
	resizeItem_bottomLeft->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_bottomLeft->setCursor(Qt::SizeBDiagCursor);
	resizeItem_bottomLeft->setPos(0, 64);
	resizeItem_bottomLeft->setZValue(1000);
	popupMenuScene->addItem(resizeItem_bottomLeft);
	resizeItem_bottom = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_bottom->setPen(QPen(Qt::NoPen));
	resizeItem_bottom->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_bottom->setCursor(Qt::SizeVerCursor);
	resizeItem_bottom->setPos(32, 64);
	resizeItem_bottom->setZValue(1000);
	popupMenuScene->addItem(resizeItem_bottom);
	resizeItem_bottomRight = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_bottomRight->setPen(QPen(Qt::NoPen));
	resizeItem_bottomRight->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_bottomRight->setCursor(Qt::SizeFDiagCursor);
	resizeItem_bottomRight->setPos(64, 64);
	resizeItem_bottomRight->setZValue(1000);
	popupMenuScene->addItem(resizeItem_bottomRight);
	selectionRect = new QGraphicsRectItem(0, 0, 64, 64);
	selectionRect->setPen(QPen(QBrush(QColor(0, 255, 255)), 3, Qt::DashLine));
	selectionRect->setBrush(QBrush(Qt::NoBrush));
	selectionRect->setPos(0, 0);
	selectionRect->setZValue(1000);
	selectionRect->setVisible(false);
	popupMenuScene->addItem(selectionRect);
	highlightedObject = new QGraphicsRectItem(0, 0, 0, 0);
	highlightedObject->setPen(QPen(Qt::NoPen));
	highlightedObject->setBrush(QBrush(QColor(255, 255, 255)));
	highlightedObject->setPos(0, 0);
	highlightedObject->setZValue(1000);
	highlightedObject->setOpacity(.25);
	highlightedObject->setVisible(false);
	popupMenuScene->addItem(highlightedObject);
}

void ScreenLayouts_PopupMenu::shutdownLayout()
{
	if (layoutRef != NULL)
	{
		delete layoutRef;
		layoutRef = NULL;
	}
	twObjectList->clear();
	twProperties->clearContents();
	ignoreEvents = false;
}

void ScreenLayouts_PopupMenu::unloadCurrentLayout()
{
	if (layoutRef != NULL)
	{
		delete layoutRef;
		layoutRef = NULL;
	}
}

void ScreenLayouts_PopupMenu::setCurrentLayout(int layoutID)
{
	int layoutWidth, layoutHeight;
	ignoreEvents = true;
	if (layoutRef != NULL)
	{
		delete layoutRef;
		layoutRef = NULL;
	}
	twObjectList->clear();
	layoutRef = new ProjectData::DataReference<PopupMenuLayout>(layoutID, __FILE__, __LINE__);
	(*layoutRef)->setGraphicsScene(popupMenuScene);
	layoutWidth = (*layoutRef)->getWidth();
	layoutHeight = (*layoutRef)->getHeight();
	popupMenuScene->setSceneRect(0, 0, layoutWidth, layoutHeight);
	menuBackgroundItem->setRect(0, 0, layoutWidth, layoutHeight);
	menuGridItem->setRect(0, 0, layoutWidth, layoutHeight);
	resizeItem_topLeft->setVisible(false);
	resizeItem_top->setVisible(false);
	resizeItem_topRight->setVisible(false);
	resizeItem_left->setVisible(false);
	resizeItem_right->setVisible(false);
	resizeItem_bottomLeft->setVisible(false);
	resizeItem_bottom->setVisible(false);
	resizeItem_bottomRight->setVisible(false);
	highlightedObject->setVisible(false);
	for (int i = 0; i < (*layoutRef)->numTopLevelObjects(); ++i)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(twObjectList);
		MenuObject *object = (*layoutRef)->getTopLevelObject(i);
		int objectType = object->getType();
		item->setText(0, object->getName());
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
		objectItemList.insert(object->getName(), item);
		if (objectType == MenuObject::TYPE_FRAME)
		{
			item->setText(1, "Frame");
			buildObjectTree(object, item);
		}
		else if (objectType == MenuObject::TYPE_TEXT)
			item->setText(1, "Text");
		else if (objectType == MenuObject::TYPE_IMAGE)
			item->setText(1, "Image");
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
		{
			item->setText(1, "Selectable Area");
			buildObjectTree(object, item);
		}
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
		{
			item->setText(1, "Scroll Area");
			buildObjectTree(object, item);
		}
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			item->setText(1, "Item List");
	}
	(*layoutRef)->updateMenuObjects();
	ignoreEvents = false;
}

void ScreenLayouts_PopupMenu::changeCurrentLayoutsName(QString newName)
{
	(*layoutRef)->setName(newName);
	ProjectData::popupMenuLayoutNames[layoutRef->getDataID()] = newName;
}

void ScreenLayouts_PopupMenu::on_aCopy_triggered()
{
	if (selectedObject != NULL)
	{
		MenuObject *copyObject;
		int objectType = selectedObject->getType();
		if (objectType == MenuObject::TYPE_FRAME)
		{
			copyObject = new MenuFrame;
			((MenuFrame*)selectedObject)->getCopy(copyObject);
		}
		else if (objectType == MenuObject::TYPE_TEXT)
		{
			copyObject = new MenuText;
			((MenuText*)selectedObject)->getCopy(copyObject);
		}
		else if (objectType == MenuObject::TYPE_IMAGE)
		{
			copyObject = new MenuImage;
			((MenuImage*)selectedObject)->getCopy(copyObject);
		}
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
		{
			copyObject = new MenuSelectableArea;
			((MenuSelectableArea*)selectedObject)->getCopy(copyObject);
		}
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
		{
			copyObject = new MenuScrollArea;
			((MenuScrollArea*)selectedObject)->getCopy(copyObject);
		}
		else if (objectType == MenuObject::TYPE_ITEMLIST)
		{
			copyObject = new MenuItemTable;
			((MenuItemTable*)selectedObject)->getCopy(copyObject);
		}
		Clipboard::clear();
		Clipboard::dataType = Clipboard::TYPE_MENUOBJECT;
		Clipboard::data.menuObject = copyObject;
	}
}

void ScreenLayouts_PopupMenu::on_aCut_triggered()
{
	MenuObject *objectToDelete = selectedObject;
	QTreeWidgetItem *objectItem = objectItemList[objectToDelete->getName()];
	aCopy->trigger();
	selectedObject = NULL;
	deleteChildItems(objectItem);
	objectItemList.remove(objectToDelete->getName());
	delete objectItem;
	delete objectToDelete;
	selectedObjectChanged();
}

void ScreenLayouts_PopupMenu::on_aDeleteItem_triggered()
{
	MenuObject *objectToDelete = selectedObject;
	int confirm = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete the selected object and all of it's children?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
	if (confirm == QMessageBox::Yes)
	{
		QTreeWidgetItem *objectItem = objectItemList[objectToDelete->getName()];
		selectedObject = NULL;
		deleteChildItems(objectItem);
		objectItemList.remove(objectToDelete->getName());
		delete objectItem;
		delete objectToDelete;
		selectedObjectChanged();
	}
}

void ScreenLayouts_PopupMenu::on_aPaste_triggered()
{
	if (Clipboard::dataType == Clipboard::TYPE_MENUOBJECT)
	{
		MenuObject *copyObject = Clipboard::data.menuObject;
		MenuObject *parentObject;
		QTreeWidgetItem *objectItem;
		int objectType = copyObject->getType();
		Clipboard::data.menuObject = NULL;
		Clipboard::dataType = Clipboard::TYPE_NONE;
		parentObject = getObjectUnderCursor(hotspot);
		while (parentObject != NULL && parentObject->getType() != MenuObject::TYPE_FRAME && parentObject->getType() != MenuObject::TYPE_SELECTABLEAREA && parentObject->getType() != MenuObject::TYPE_SCROLLAREA)
			parentObject = parentObject->getParent();
		copyObject->setPos((int)hotspot.x(), (int)hotspot.y());
		if (parentObject != NULL)
			copyObject->setParent(parentObject);
		else
			copyObject->setLayout(layoutRef->getDataPointer());
		for (int i = 0; i < copyObject->numChildObjects(); ++i)
			addObjectToLayout(copyObject->getChildObject(i));
		objectItem = new QTreeWidgetItem;
		objectItem->setText(0, copyObject->getName());
		if (objectType == MenuObject::TYPE_FRAME)
			objectItem->setText(1, "Frame");
		else if (objectType == MenuObject::TYPE_TEXT)
			objectItem->setText(1, "Text");
		else if (objectType == MenuObject::TYPE_IMAGE)
			objectItem->setText(1, "Image");
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
			objectItem->setText(1, "Selectable Area");
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			objectItem->setText(1, "Scroll Area");
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			objectItem->setText(1, "Item List");
		objectItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		objectItemList.insert(copyObject->getName(), objectItem);
		buildObjectTree(copyObject, objectItem);
		if (parentObject != NULL)
			objectItemList[parentObject->getName()]->addChild(objectItem);
		else
			twObjectList->addTopLevelItem(objectItem);
	}
}

void ScreenLayouts_PopupMenu::on_bPreview_clicked()
{
	MenuPreview *preview = new MenuPreview(this);
	preview->setupPreview(layoutRef->getDataID(), MenuPreview::MENUTYPE_POPUPMENU);
	preview->exec();
	delete preview;
}

void ScreenLayouts_PopupMenu::on_cboxZoom_currentIndexChanged(int index)
{
	gvPopupMenuLayout->resetMatrix();
	lwMenuItems->setScaleFactor(1.0);
	if (index == ZOOM_25)
	{
		gvPopupMenuLayout->scale(.25, .25);
		lwMenuItems->setScaleFactor(.25);
	}
	else if (index == ZOOM_50)
	{
		gvPopupMenuLayout->scale(.5, .5);
		lwMenuItems->setScaleFactor(.5);
	}
	else if (index == ZOOM_75)
	{
		gvPopupMenuLayout->scale(.75, .75);
		lwMenuItems->setScaleFactor(.75);
	}
	else if (index == ZOOM_150)
	{
		gvPopupMenuLayout->scale(1.5, 1.5);
		lwMenuItems->setScaleFactor(1.5);
	}
	else if (index == ZOOM_200)
	{
		gvPopupMenuLayout->scale(2.0, 2.0);
		lwMenuItems->setScaleFactor(2.0);
	}
	else if (index == ZOOM_FITTOWINDOW)
	{
		float xScale = (float)gvPopupMenuLayout->width() / 640.0;
		float yScale = (float)gvPopupMenuLayout->height() / 480.0;
		if (xScale > yScale)
		{
			gvPopupMenuLayout->scale(yScale, yScale);
			lwMenuItems->setScaleFactor(yScale);
		}
		else
		{
			gvPopupMenuLayout->scale(xScale, xScale);
			lwMenuItems->setScaleFactor(xScale);
		}
	}
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_customContextMenuRequested(const QPoint &pos)
{
	MenuObject *object;
	QPointF scenePos = gvPopupMenuLayout->mapToScene(pos);
	QPoint globalPos = gvPopupMenuLayout->mapToGlobal(pos);
	object = getObjectUnderCursor(scenePos);
	if (object != NULL)
	{
		QMenu *popupMenu = new QMenu(this);
		selectedObject = object;
		selectedObjectChanged();
		popupMenu->addAction(aCut);
		popupMenu->addAction(aCopy);
		popupMenu->addAction(aPaste);
		popupMenu->addSeparator();
		popupMenu->addAction(aDeleteItem);
		aCut->setEnabled(true);
		aCopy->setEnabled(true);
		aPaste->setEnabled(Clipboard::dataType == Clipboard::TYPE_MENUOBJECT);
		aDeleteItem->setEnabled(true);
		hotspot = scenePos;
		popupMenu->exec(globalPos);
		delete popupMenu;
	}
	else if (Clipboard::dataType == Clipboard::TYPE_MENUOBJECT)
	{
		QMenu *popupMenu = new QMenu(this);
		popupMenu->addAction(aPaste);
		aPaste->setEnabled(true);
		hotspot = scenePos;
		popupMenu->exec(globalPos);
		delete popupMenu;
	}
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_dragEnterEvent(QDragEnterEvent *event)
{
	if (event->source() == lwMenuItems && layoutRef != NULL)
		event->acceptProposedAction();
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_dragLeaveEvent(QDragLeaveEvent *event)
{
	Q_UNUSED(event);
	highlightedObject->setVisible(false);
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_dragMoveEvent(QDragMoveEvent *event)
{
	if (event->source() == lwMenuItems && layoutRef != NULL)
	{
		MenuObject *parentObject, *tempObject;
		QGraphicsPixmapItem *pixmapItem;
		QPointF scenePos = gvPopupMenuLayout->mapToScene(event->pos());
		int x = (int)scenePos.x();
		int y = (int)scenePos.y();
		int localX, localY;
		parentObject = getObjectUnderCursor(scenePos);
		while (parentObject != NULL && parentObject->getType() != MenuObject::TYPE_FRAME && parentObject->getType() != MenuObject::TYPE_SELECTABLEAREA && parentObject->getType() != MenuObject::TYPE_SCROLLAREA)
			parentObject = parentObject->getParent();
		tempObject = parentObject;
		localX = x;
		localY = y;
		while (tempObject != NULL)
		{
			localX -= tempObject->getX();
			localY -= tempObject->getY();
			if (tempObject->getType() == MenuObject::TYPE_SCROLLAREA)
			{
				localX += ((MenuScrollArea*)tempObject)->getCurrentScrollX();
				localY += ((MenuScrollArea*)tempObject)->getCurrentScrollY();
			}
			tempObject = tempObject->getParent();
		}
		if (parentObject != NULL)
		{
			pixmapItem = parentObject->getPixmapItem();
			highlightedObject->setRect(pixmapItem->scenePos().x(), pixmapItem->scenePos().y(), parentObject->getWidth(), parentObject->getHeight());
			highlightedObject->setVisible(true);
		}
		else
			highlightedObject->setVisible(false);
	}
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_dropEvent(QDropEvent *event)
{
	highlightedObject->setVisible(false);
	if (event->source() == lwMenuItems && layoutRef != NULL)
	{
		MenuObject *droppedObject;
		MenuObject *parentObject;
		QTreeWidgetItem *objectItem;
		int objectType = lwMenuItems->getDropObjectType();
		QPointF scenePos = gvPopupMenuLayout->mapToScene(event->pos());
		parentObject = getObjectUnderCursor(scenePos);
		while (parentObject != NULL && parentObject->getType() != MenuObject::TYPE_FRAME && parentObject->getType() != MenuObject::TYPE_SELECTABLEAREA && parentObject->getType() != MenuObject::TYPE_SCROLLAREA)
			parentObject = parentObject->getParent();
		droppedObject = lwMenuItems->getDropObject();
		droppedObject->setPos((int)scenePos.x(), (int)scenePos.y());
		if (parentObject != NULL)
			droppedObject->setParent(parentObject);
		else
			droppedObject->setLayout(layoutRef->getDataPointer());
		objectItem = new QTreeWidgetItem;
		objectItem->setText(0, droppedObject->getName());
		if (objectType == MenuObject::TYPE_FRAME)
			objectItem->setText(1, "Frame");
		else if (objectType == MenuObject::TYPE_TEXT)
			objectItem->setText(1, "Text");
		else if (objectType == MenuObject::TYPE_IMAGE)
			objectItem->setText(1, "Image");
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
			objectItem->setText(1, "Selectable Area");
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
			objectItem->setText(1, "Scroll Area");
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			objectItem->setText(1, "Item List");
		objectItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		objectItemList.insert(droppedObject->getName(), objectItem);
		if (parentObject != NULL)
			objectItemList[parentObject->getName()]->addChild(objectItem);
		else
			twObjectList->addTopLevelItem(objectItem);
		event->acceptProposedAction();
		highlightedObject->setVisible(false);
	}
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_leaveEvent(QEvent *event)
{
	Q_UNUSED(event);
	QApplication::restoreOverrideCursor();
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_mouseDoubleClickEvent(QMouseEvent *event)
{
	Q_UNUSED(event);
	if (selectedObject != NULL)
	{
		int objectType = selectedObject->getType();
		if (objectType == MenuObject::TYPE_IMAGE)
		{
			ConfigureMenuImage *configImage = new ConfigureMenuImage((MenuImage*)selectedObject, this);
			configImage->exec();
			selectedObjectChanged();
			delete configImage;
		}
		else if (objectType == MenuObject::TYPE_TEXT)
		{
			ConfigureMenuText *configText = new ConfigureMenuText((MenuText*)selectedObject, this);
			configText->exec();
			selectedObjectChanged();
			delete configText;
		}
		else if (objectType == MenuObject::TYPE_ITEMLIST)
		{
			ConfigureMenuItemList *configItemList = new ConfigureMenuItemList((MenuItemTable*)selectedObject, this);
			configItemList->exec();
			selectedObjectChanged();
			delete configItemList;
		}
	}
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_mouseMoveEvent(QMouseEvent *event)
{
	QPointF scenePos = gvPopupMenuLayout->mapToScene(event->pos());
	QPoint globalPos = gvPopupMenuLayout->mapToGlobal(event->pos());
	QString toolTip = "";
	int offsetX = (int)(hotspot.x() - scenePos.x());
	int offsetY = (int)(hotspot.y() - scenePos.y());
	int snapAmount, gridSize = cboxGridSize->currentIndex();
	if (cbSnapToGrid->isChecked())
	{
		if (gridSize == SIZE_2x2)
			snapAmount = 2;
		else if (gridSize == SIZE_4x4)
			snapAmount = 4;
		else if (gridSize == SIZE_8x8)
			snapAmount = 8;
		else if (gridSize == SIZE_16x16)
			snapAmount = 16;
		else if (gridSize == SIZE_32x32)
			snapAmount = 32;
		else if (gridSize == SIZE_64x64)
			snapAmount = 64;
	}
	else
		snapAmount = 1;
	if ((event->buttons() & Qt::LeftButton) == 0)
	{
		QApplication::restoreOverrideCursor();
		if (resizeItem_topLeft->isVisible())
		{
			QRectF topLeftRect = QRectF(resizeItem_topLeft->pos(), QSizeF(4, 4));
			QRectF topRect = QRectF(resizeItem_top->pos(), QSizeF(4, 4));
			QRectF topRightRect = QRectF(resizeItem_topRight->pos(), QSizeF(4, 4));
			QRectF leftRect = QRectF(resizeItem_left->pos(), QSizeF(4, 4));
			QRectF rightRect = QRectF(resizeItem_right->pos(), QSizeF(4, 4));
			QRectF bottomLeftRect = QRectF(resizeItem_bottomLeft->pos(), QSizeF(4, 4));
			QRectF bottomRect = QRectF(resizeItem_bottom->pos(), QSizeF(4, 4));
			QRectF bottomRightRect = QRectF(resizeItem_bottomRight->pos(), QSizeF(4, 4));
			if (topLeftRect.contains(scenePos))
				QApplication::setOverrideCursor(resizeItem_topLeft->cursor());
			else if (topRect.contains(scenePos))
				QApplication::setOverrideCursor(resizeItem_top->cursor());
			else if (topRightRect.contains(scenePos))
				QApplication::setOverrideCursor(resizeItem_topRight->cursor());
			else if (leftRect.contains(scenePos))
				QApplication::setOverrideCursor(resizeItem_left->cursor());
			else if (rightRect.contains(scenePos))
				QApplication::setOverrideCursor(resizeItem_right->cursor());
			else if (bottomLeftRect.contains(scenePos))
				QApplication::setOverrideCursor(resizeItem_bottomLeft->cursor());
			else if (bottomRect.contains(scenePos))
				QApplication::setOverrideCursor(resizeItem_bottom->cursor());
			else if (bottomRightRect.contains(scenePos))
				QApplication::setOverrideCursor(resizeItem_bottomRight->cursor());
		}
	}
	else if ((event->buttons() & Qt::LeftButton) != 0 && selectedObject != NULL)
	{
		if (resizeInProgress)
		{
			MenuObject *parentObject = NULL;
			int newX, newY, newWidth, newHeight;
			int adjustX = 0;
			int adjustY = 0;
			if (activeResizeHandle == RESIZE_TOPLEFT)
			{
				resizeItem_topLeft->setPos(scenePos.x() + offsetX, scenePos.y() + offsetY);
				newX = (int)resizeItem_topLeft->pos().x() + 2;
				newY = (int)resizeItem_topLeft->pos().y() + 2;
				newX -= newX % snapAmount;
				newY -= newY % snapAmount;
				resizeItem_topLeft->setPos(newX - 2, newY - 2);
			}
			else if (activeResizeHandle == RESIZE_TOP)
			{
				resizeItem_topLeft->setY(scenePos.y() + offsetY);
				newY = (int)resizeItem_topLeft->y() + 2;
				newY -= newY % snapAmount;
				resizeItem_topLeft->setY(newY - 2);
			}
			else if (activeResizeHandle == RESIZE_TOPRIGHT)
			{
				resizeItem_topLeft->setY(scenePos.y() + offsetY);
				resizeItem_bottomRight->setX(scenePos.x() + offsetX);
				newX = (int)resizeItem_bottomRight->x() + 2;
				newY = (int)resizeItem_topLeft->y() + 2;
				newX -= newX % snapAmount;
				newY -= newY % snapAmount;
				resizeItem_topLeft->setY(newY - 2);
				resizeItem_bottomRight->setX(newX - 2);
			}
			else if (activeResizeHandle == RESIZE_LEFT)
			{
				resizeItem_topLeft->setX(scenePos.x() + offsetX);
				newX = (int)resizeItem_topLeft->x() + 2;
				newX -= newX % snapAmount;
				resizeItem_topLeft->setX(newX - 2);
			}
			else if (activeResizeHandle == RESIZE_RIGHT)
			{
				resizeItem_bottomRight->setX(scenePos.x() + offsetX);
				newX = (int)resizeItem_bottomRight->x() + 2;
				newX -= newX % snapAmount;
				resizeItem_bottomRight->setX(newX - 2);
			}
			else if (activeResizeHandle == RESIZE_BOTTOMLEFT)
			{
				resizeItem_topLeft->setX(scenePos.x() + offsetX);
				resizeItem_bottomRight->setY(scenePos.y() + offsetY);
				newX = (int)resizeItem_topLeft->x() + 2;
				newY = (int)resizeItem_bottomRight->pos().y() + 2;
				newX -= newX % snapAmount;
				newY -= newY % snapAmount;
				resizeItem_topLeft->setX(newX - 2);
				resizeItem_bottomRight->setY(newY - 2);
			}
			else if (activeResizeHandle == RESIZE_BOTTOM)
			{
				resizeItem_bottomRight->setY(scenePos.y() + offsetY);
				newY = (int)resizeItem_bottomRight->pos().y() + 2;
				newY -= newY % snapAmount;
				resizeItem_bottomRight->setY(newY - 2);
			}
			else if (activeResizeHandle == RESIZE_BOTTOMRIGHT)
			{
				resizeItem_bottomRight->setPos(scenePos.x() + offsetX, scenePos.y() + offsetY);
				newX = (int)resizeItem_bottomRight->pos().x() + 2;
				newY = (int)resizeItem_bottomRight->pos().y() + 2;
				newX -= newX % snapAmount;
				newY -= newY % snapAmount;
				resizeItem_bottomRight->setPos(newX - 2, newY - 2);
			}
			newX = (int)resizeItem_topLeft->pos().x() + 2;
			newY = (int)resizeItem_topLeft->pos().y() + 2;
			newWidth = (int)resizeItem_bottomRight->pos().x() + 2 - newX;
			newHeight = (int)resizeItem_bottomRight->pos().y() + 2 - newY;
			if (newWidth < 32)
				adjustX = 32 - newWidth;
			if (newHeight < 32)
				adjustY = 32 - newHeight;
			newWidth += adjustX;
			newHeight += adjustY;
			if (activeResizeHandle == RESIZE_TOPLEFT)
			{
				newX -= adjustX;
				newY -= adjustY;
			}
			else if (activeResizeHandle == RESIZE_TOP)
				newY -= adjustY;
			else if (activeResizeHandle == RESIZE_TOPRIGHT)
				newY -= adjustY;
			else if (activeResizeHandle == RESIZE_LEFT)
				newX -= adjustX;
			else if (activeResizeHandle == RESIZE_BOTTOMLEFT)
				newX -= adjustX;
			resizeItem_topLeft->setPos(newX - 2, newY - 2);
			resizeItem_top->setPos(newX + (newWidth / 2) - 2, newY - 2);
			resizeItem_topRight->setPos(newX + newWidth - 2, newY - 2);
			resizeItem_left->setPos(newX - 2, newY + (newHeight / 2) - 2);
			resizeItem_right->setPos(newX + newWidth - 2, newY + (newHeight / 2) - 2);
			resizeItem_bottomLeft->setPos(newX - 2, newY + newHeight - 2);
			resizeItem_bottom->setPos(newX + (newWidth / 2) - 2, newY + newHeight - 2);
			resizeItem_bottomRight->setPos(newX + newWidth - 2, newY + newHeight - 2);
			parentObject = selectedObject->getParent();
			while (parentObject != NULL)
			{
				newX -= parentObject->getX();
				newY -= parentObject->getY();
				parentObject = parentObject->getParent();
			}
			selectedObject->setPos(newX, newY);
			selectedObject->resize(newWidth, newHeight);
			hotspot = scenePos;
			toolTip += QString("%1x%2").arg(selectedObject->getWidth()).arg(selectedObject->getHeight());
			QToolTip::showText(globalPos, toolTip);
		}
		else if (repositionInProgress)
		{
			MenuObject *parentObject, *tempObject;
			QGraphicsPixmapItem *pixmapItem;
			int x = (int)scenePos.x() - (int)hotspot.x();
			int y = (int)scenePos.y() - (int)hotspot.y();
			int localX, localY;
			x -= x % snapAmount;
			y -= y % snapAmount;
			parentObject = getObjectUnderCursor(scenePos);
			while (parentObject != NULL && parentObject->getType() != MenuObject::TYPE_FRAME && parentObject->getType() != MenuObject::TYPE_SELECTABLEAREA && parentObject->getType() != MenuObject::TYPE_SCROLLAREA)
				parentObject = parentObject->getParent();
			tempObject = parentObject;
			localX = x;
			localY = y;
			while (tempObject != NULL)
			{
				localX -= tempObject->getX();
				localY -= tempObject->getY();
				if (tempObject->getType() == MenuObject::TYPE_SCROLLAREA)
				{
					localX += ((MenuScrollArea*)tempObject)->getCurrentScrollX();
					localY += ((MenuScrollArea*)tempObject)->getCurrentScrollY();
				}
				tempObject = tempObject->getParent();
			}
			if (parentObject != NULL)
			{
				pixmapItem = parentObject->getPixmapItem();
				highlightedObject->setRect(pixmapItem->scenePos().x(), pixmapItem->scenePos().y(), parentObject->getWidth(), parentObject->getHeight());
				highlightedObject->setVisible(true);
			}
			else
				highlightedObject->setVisible(false);
			selectedObject->getPixmapItem()->setPos(x, y);
			toolTip += QString("Global: (%1, %2)\n").arg(x).arg(y);
			toolTip += QString("Local: (%1, %2)").arg(localX).arg(localY);
			QToolTip::showText(globalPos, toolTip);
			objectMoved = true;
		}
	}
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_mousePressEvent(QMouseEvent *event)
{
	QPointF scenePos = gvPopupMenuLayout->mapToScene(event->pos());
	QPoint globalPos = gvPopupMenuLayout->mapToGlobal(event->pos());
	hotspot = scenePos;
	objectMoved = false;
	if ((event->buttons() & Qt::LeftButton) != 0)
	{
		if (resizeItem_topLeft->isVisible())
		{
			QRectF topLeftRect = QRectF(resizeItem_topLeft->pos(), QSizeF(4, 4));
			QRectF topRect = QRectF(resizeItem_top->pos(), QSizeF(4, 4));
			QRectF topRightRect = QRectF(resizeItem_topRight->pos(), QSizeF(4, 4));
			QRectF leftRect = QRectF(resizeItem_left->pos(), QSizeF(4, 4));
			QRectF rightRect = QRectF(resizeItem_right->pos(), QSizeF(4, 4));
			QRectF bottomLeftRect = QRectF(resizeItem_bottomLeft->pos(), QSizeF(4, 4));
			QRectF bottomRect = QRectF(resizeItem_bottom->pos(), QSizeF(4, 4));
			QRectF bottomRightRect = QRectF(resizeItem_bottomRight->pos(), QSizeF(4, 4));
			if (topLeftRect.contains(scenePos))
			{
				resizeInProgress = true;
				QApplication::setOverrideCursor(resizeItem_topLeft->cursor());
				activeResizeHandle = RESIZE_TOPLEFT;
				QToolTip::showText(globalPos, QString("%1x%2").arg(selectedObject->getWidth()).arg(selectedObject->getHeight()));
			}
			else if (topRect.contains(scenePos))
			{
				resizeInProgress = true;
				activeResizeHandle = RESIZE_TOP;
				QApplication::setOverrideCursor(resizeItem_top->cursor());
				QToolTip::showText(globalPos, QString("%1x%2").arg(selectedObject->getWidth()).arg(selectedObject->getHeight()));
			}
			else if (topRightRect.contains(scenePos))
			{
				resizeInProgress = true;
				activeResizeHandle = RESIZE_TOPRIGHT;
				QApplication::setOverrideCursor(resizeItem_topRight->cursor());
				QToolTip::showText(globalPos, QString("%1x%2").arg(selectedObject->getWidth()).arg(selectedObject->getHeight()));
			}
			else if (leftRect.contains(scenePos))
			{
				resizeInProgress = true;
				activeResizeHandle = RESIZE_LEFT;
				QApplication::setOverrideCursor(resizeItem_left->cursor());
				QToolTip::showText(globalPos, QString("%1x%2").arg(selectedObject->getWidth()).arg(selectedObject->getHeight()));
			}
			else if (rightRect.contains(scenePos))
			{
				resizeInProgress = true;
				activeResizeHandle = RESIZE_RIGHT;
				QApplication::setOverrideCursor(resizeItem_right->cursor());
				QToolTip::showText(globalPos, QString("%1x%2").arg(selectedObject->getWidth()).arg(selectedObject->getHeight()));
			}
			else if (bottomLeftRect.contains(scenePos))
			{
				resizeInProgress = true;
				activeResizeHandle = RESIZE_BOTTOMLEFT;
				QApplication::setOverrideCursor(resizeItem_bottomLeft->cursor());
				QToolTip::showText(globalPos, QString("%1x%2").arg(selectedObject->getWidth()).arg(selectedObject->getHeight()));
			}
			else if (bottomRect.contains(scenePos))
			{
				resizeInProgress = true;
				activeResizeHandle = RESIZE_BOTTOM;
				QApplication::setOverrideCursor(resizeItem_bottom->cursor());
				QToolTip::showText(globalPos, QString("%1x%2").arg(selectedObject->getWidth()).arg(selectedObject->getHeight()));
			}
			else if (bottomRightRect.contains(scenePos))
			{
				resizeInProgress = true;
				activeResizeHandle = RESIZE_BOTTOMRIGHT;
				QApplication::setOverrideCursor(resizeItem_bottomRight->cursor());
				QToolTip::showText(globalPos, QString("%1x%2").arg(selectedObject->getWidth()).arg(selectedObject->getHeight()));
			}
			else
			{
				selectedObject = getObjectUnderCursor(scenePos);
				if (selectedObject != NULL)
				{
					QGraphicsPixmapItem *item = selectedObject->getPixmapItem();
					QPointF itemPos = item->scenePos();
					repositionInProgress = true;
					item->setParentItem(NULL);
					item->setPos(itemPos);
					item->setZValue(5000);
					resizeItem_topLeft->setVisible(false);
					resizeItem_top->setVisible(false);
					resizeItem_topRight->setVisible(false);
					resizeItem_left->setVisible(false);
					resizeItem_right->setVisible(false);
					resizeItem_bottomLeft->setVisible(false);
					resizeItem_bottom->setVisible(false);
					resizeItem_bottomRight->setVisible(false);
					selectionRect->setVisible(false);
					selectedObject->changeParent(NULL);
					(*layoutRef)->removeTopLevelObject(selectedObject);
					popupMenuScene->addItem(item);
					hotspot -= itemPos;
				}
				else
					repositionInProgress = false;
			}
		}
		else
		{
			selectedObject = getObjectUnderCursor(scenePos);
			if (selectedObject != NULL)
			{
				QGraphicsPixmapItem *item = selectedObject->getPixmapItem();
				QPointF itemPos = item->scenePos();
				repositionInProgress = true;
				item->setParentItem(NULL);
				item->setPos(itemPos);
				item->setZValue(5000);
				resizeItem_topLeft->setVisible(false);
				resizeItem_top->setVisible(false);
				resizeItem_topRight->setVisible(false);
				resizeItem_left->setVisible(false);
				resizeItem_right->setVisible(false);
				resizeItem_bottomLeft->setVisible(false);
				resizeItem_bottom->setVisible(false);
				resizeItem_bottomRight->setVisible(false);
				selectionRect->setVisible(false);
				selectedObject->changeParent(NULL);
				(*layoutRef)->removeTopLevelObject(selectedObject);
				popupMenuScene->addItem(item);
				hotspot -= itemPos;
			}
			else
				repositionInProgress = false;
		}
	}
}

void ScreenLayouts_PopupMenu::on_gvPopupMenuLayout_mouseReleaseEvent(QMouseEvent *event)
{
	int snapAmount, gridSize = cboxGridSize->currentIndex();
	if (cbSnapToGrid->isChecked())
	{
		if (gridSize == SIZE_2x2)
			snapAmount = 2;
		else if (gridSize == SIZE_4x4)
			snapAmount = 4;
		else if (gridSize == SIZE_8x8)
			snapAmount = 8;
		else if (gridSize == SIZE_16x16)
			snapAmount = 16;
		else if (gridSize == SIZE_32x32)
			snapAmount = 32;
		else if (gridSize == SIZE_64x64)
			snapAmount = 64;
	}
	else
		snapAmount = 1;
	ignoreEvents = true;
	if (event->button() == Qt::LeftButton)
	{
		if (repositionInProgress)
		{
			MenuObject *parentObject, *tempObject;
			QTreeWidgetItem *parentItem, *item = objectItemList[selectedObject->getName()];
			QPointF scenePos = gvPopupMenuLayout->mapToScene(event->pos());
			int x = (int)scenePos.x() - (int)hotspot.x();
			int y = (int)scenePos.y() - (int)hotspot.y();
			int localX, localY;
			if (objectMoved)
			{
				x -= x % snapAmount;
				y -= y % snapAmount;
			}
			parentObject = getObjectUnderCursor(scenePos);
			while (parentObject != NULL && parentObject->getType() != MenuObject::TYPE_FRAME && parentObject->getType() != MenuObject::TYPE_SELECTABLEAREA && parentObject->getType() != MenuObject::TYPE_SCROLLAREA)
				parentObject = parentObject->getParent();
			tempObject = parentObject;
			localX = x;
			localY = y;
			while (tempObject != NULL)
			{
				localX -= tempObject->getX();
				localY -= tempObject->getY();
				if (tempObject->getType() == MenuObject::TYPE_SCROLLAREA)
				{
					localX += ((MenuScrollArea*)tempObject)->getCurrentScrollX();
					localY += ((MenuScrollArea*)tempObject)->getCurrentScrollY();
				}
				tempObject = tempObject->getParent();
			}
			selectedObject->changeParent(parentObject, true);
			selectedObject->setPos(localX, localY);
			if (item->parent() != NULL)
				item->parent()->removeChild(item);
			if (parentObject != NULL)
			{
				parentItem = objectItemList[parentObject->getName()];
				parentItem->addChild(item);
			}
			else
				twObjectList->addTopLevelItem(item);
			highlightedObject->setVisible(false);
			(*layoutRef)->updateZValues();
		}
	}
	QToolTip::showText(gvPopupMenuLayout->mapToGlobal(event->pos()), "");
	resizeInProgress = false;
	repositionInProgress = false;
	ignoreEvents = false;
	selectedObjectChanged();
}

void ScreenLayouts_PopupMenu::on_twObjectList_customContextMenuRequested(const QPoint &pos)
{
	QPoint globalPos = twObjectList->viewport()->mapToGlobal(pos);
	if (selectedObject != NULL)
	{
		QMenu *popupMenu = new QMenu(this);
		popupMenu->addAction(aCut);
		popupMenu->addAction(aCopy);
		popupMenu->addSeparator();
		popupMenu->addAction(aDeleteItem);
		aCut->setEnabled(true);
		aCopy->setEnabled(true);
		aDeleteItem->setEnabled(true);
		popupMenu->exec(globalPos);
		delete popupMenu;
	}
}

void ScreenLayouts_PopupMenu::on_twObjectList_itemSelectionChanged()
{
	if (!ignoreEvents)
	{
		QTreeWidgetItem *item = twObjectList->currentItem();
		if (item != NULL)
			selectedObject = (*layoutRef)->getObjectByName(item->text(0));
		else
			selectedObject = NULL;
		selectedObjectChanged();
		if (selectedObject != NULL)
			gvPopupMenuLayout->ensureVisible(selectedObject->getPixmapItem());
	}
}

void ScreenLayouts_PopupMenu::on_twObjectList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (column == 0 && selectedObject != NULL)
		twObjectList->editItem(item, column);
}

void ScreenLayouts_PopupMenu::on_twObjectList_itemChanged(QTreeWidgetItem *item, int column)
{
	if (!ignoreEvents && column == 0 && selectedObject != NULL)
	{
		QTreeWidgetItem *item2;
		QString newName, oldName = selectedObject->getName();
		ignoreEvents = true;
		selectedObject->setName(item->text(0));
		newName = selectedObject->getName();
		twProperties->objectsNameChanged(newName);
		item2 = objectItemList.take(oldName);
		objectItemList.insert(newName, item2);
		item->setText(0, newName);
		ignoreEvents = false;
	}
}

void ScreenLayouts_PopupMenu::on_twProperties_itemChanged(QTableWidgetItem *item)
{
	if (!ignoreEvents)
	{
		if (item->row() == 0 && item->column() == 1)
		{
			QTreeWidgetItem *objectItem;
			QString oldName = objectItemList.key(twObjectList->currentItem());
			ignoreEvents = true;
			twObjectList->currentItem()->setText(0, item->text());
			objectItem = objectItemList.take(oldName);
			objectItemList.insert(item->text(), objectItem);
			ignoreEvents = false;
		}
		else
			selectedObjectChanged();
	}
}

void ScreenLayouts_PopupMenu::on_bEasyNavigationCreator_clicked()
{
	MenuNavigationCreator *navCreator = new MenuNavigationCreator((*layoutRef)->getObjectNamesByType(MenuObject::TYPE_SELECTABLEAREA), (*layoutRef)->getNavigation(), this);
	QObject::connect(navCreator, SIGNAL(applyMenuNavigation()), this, SLOT(applyMenuNavigation()));
	navCreator->exec();
	QObject::disconnect(navCreator, SIGNAL(applyMenuNavigation()), this, SLOT(applyMenuNavigation()));
	delete navCreator;
}

void ScreenLayouts_PopupMenu::on_bEditMenusEvents_clicked()
{
	MenuEventEditor *editor = new MenuEventEditor(layoutRef->getDataID(), MenuObject::MENUTYPE_POPUPMENU);
	editor->exec();
	delete editor;
}

void ScreenLayouts_PopupMenu::on_cbSnapToGrid_toggled(bool checked)
{
	wSnapToGrid->setEnabled(checked);
	menuGridItem->setVisible(checked);
}

void ScreenLayouts_PopupMenu::on_cboxGridSize_currentIndexChanged(int index)
{
	if (index == SIZE_2x2)
		menuGridItem->setBrush(QBrush(QImage(":/images/menugrid_2x2.png")));
	else if (index == SIZE_4x4)
		menuGridItem->setBrush(QBrush(QImage(":/images/menugrid_4x4.png")));
	else if (index == SIZE_8x8)
		menuGridItem->setBrush(QBrush(QImage(":/images/menugrid_8x8.png")));
	else if (index == SIZE_16x16)
		menuGridItem->setBrush(QBrush(QImage(":/images/menugrid_16x16.png")));
	else if (index == SIZE_32x32)
		menuGridItem->setBrush(QBrush(QImage(":/images/menugrid_32x32.png")));
	else if (index == SIZE_64x64)
		menuGridItem->setBrush(QBrush(QImage(":/images/menugrid_64x64.png")));
}

void ScreenLayouts_PopupMenu::on_bChangeSize_clicked()
{
	ChangePopupMenuSize *changeSize = new ChangePopupMenuSize(this);
	int newWidth, newHeight;
	changeSize->setSize((*layoutRef)->getWidth(), (*layoutRef)->getHeight());
	if (changeSize->exec())
	{
		newWidth = changeSize->getWidth();
		newHeight = changeSize->getHeight();
		(*layoutRef)->resize(newWidth, newHeight);
		menuBackgroundItem->setRect(0, 0, newWidth, newHeight);
		menuGridItem->setRect(0, 0, newWidth, newHeight);
	}
}

void ScreenLayouts_PopupMenu::applyMenuNavigation()
{
	(*layoutRef)->applyMenuNavigation();
}

void ScreenLayouts_PopupMenu::selectedObjectChanged()
{
	if (selectedObject != NULL)
	{
		QGraphicsPixmapItem *pixmapItem = selectedObject->getPixmapItem();
		MenuObject *parent = selectedObject->getParent();
		if (parent != NULL)
		{
			if (parent->getType() == MenuObject::TYPE_SCROLLAREA)
				((MenuScrollArea*)parent)->ensureVisible(QRect(selectedObject->getX(), selectedObject->getY(), selectedObject->getWidth(), selectedObject->getHeight()));
		}
		int x = (int)(pixmapItem->scenePos().x());
		int y = (int)(pixmapItem->scenePos().y());
		int width = selectedObject->getWidth();
		int height = selectedObject->getHeight();
		int objectType = selectedObject->getType();
		if (objectType == MenuObject::TYPE_FRAME || objectType == MenuObject::TYPE_ITEMLIST || objectType == MenuObject::TYPE_SELECTABLEAREA || objectType == MenuObject::TYPE_SCROLLAREA)
		{
			resizeItem_topLeft->setPos(x - 2, y - 2);
			resizeItem_topLeft->setVisible(true);
			resizeItem_top->setPos(x + (width / 2) - 2, y - 2);
			resizeItem_top->setVisible(true);
			resizeItem_topRight->setPos(x + width - 2, y - 2);
			resizeItem_topRight->setVisible(true);
			resizeItem_left->setPos(x - 2, y + (height / 2) - 2);
			resizeItem_left->setVisible(true);
			resizeItem_right->setPos(x + width - 2, y + (height / 2) - 2);
			resizeItem_right->setVisible(true);
			resizeItem_bottomLeft->setPos(x - 2, y + height - 2);
			resizeItem_bottomLeft->setVisible(true);
			resizeItem_bottom->setPos(x + (width / 2) - 2, y + height - 2);
			resizeItem_bottom->setVisible(true);
			resizeItem_bottomRight->setPos(x + width - 2, y + height - 2);
			resizeItem_bottomRight->setVisible(true);
			selectionRect->setVisible(false);
		}
		else
		{
			resizeItem_topLeft->setVisible(false);
			resizeItem_top->setVisible(false);
			resizeItem_topRight->setVisible(false);
			resizeItem_left->setVisible(false);
			resizeItem_right->setVisible(false);
			resizeItem_bottomLeft->setVisible(false);
			resizeItem_bottom->setVisible(false);
			resizeItem_bottomRight->setVisible(false);
			selectionRect->setRect(0, 0, width, height);
			selectionRect->setPos(x, y);
			selectionRect->setVisible(true);
		}
		ignoreEvents = true;
		if (objectItemList.contains(selectedObject->getName()))
			twObjectList->setCurrentItem(objectItemList[selectedObject->getName()]);
		else
			twObjectList->setCurrentItem(NULL);
		ignoreEvents = false;
	}
	else
	{
		resizeItem_topLeft->setVisible(false);
		resizeItem_top->setVisible(false);
		resizeItem_topRight->setVisible(false);
		resizeItem_left->setVisible(false);
		resizeItem_right->setVisible(false);
		resizeItem_bottomLeft->setVisible(false);
		resizeItem_bottom->setVisible(false);
		resizeItem_bottomRight->setVisible(false);
		selectionRect->setVisible(false);
		ignoreEvents = true;
		twObjectList->setCurrentItem(NULL);
		ignoreEvents = false;
	}
	ignoreEvents = true;
	twProperties->setCurrentObject(selectedObject);
	ignoreEvents = false;
}

MenuObject *ScreenLayouts_PopupMenu::getObjectUnderCursor(QPointF scenePos)
{
	QList<MenuObject*> objectList = (*layoutRef)->getTopLevelObjects();
	MenuObject *objectUnderCursor = NULL;
	int x = (int)scenePos.x();
	int y = (int)scenePos.y();
	bool done = false;
	while (!done)
	{
		done = true;
		for (int i = objectList.size() - 1; i >= 0 && done; --i)
		{
			int objectX = objectList[i]->getX();
			int objectY = objectList[i]->getY();
			int objectWidth = objectList[i]->getWidth();
			int objectHeight = objectList[i]->getHeight();
			if (x >= objectX && x < objectX + objectWidth && y >= objectY && y < objectY + objectHeight)
			{
				x -= objectX;
				y -= objectY;
				if (objectList[i]->getType() == MenuObject::TYPE_SCROLLAREA)
				{
					x += ((MenuScrollArea*)(objectList[i]))->getCurrentScrollX();
					y += ((MenuScrollArea*)(objectList[i]))->getCurrentScrollY();
				}
				objectUnderCursor = objectList[i];
				objectList.clear();
				objectList.append(objectUnderCursor->getChildObjects());
				done = false;
			}
		}
	}
	return objectUnderCursor;
}

void ScreenLayouts_PopupMenu::buildObjectTree(MenuObject *parentObject, QTreeWidgetItem *parentItem)
{
	QTreeWidgetItem *item;
	MenuObject *object;
	int objectType;
	for (int i = 0; i < parentObject->numChildObjects(); ++i)
	{
		object = parentObject->getChildObject(i);
		item = new QTreeWidgetItem(parentItem);
		item->setText(0, object->getName());
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
		objectItemList.insert(object->getName(), item);
		objectType = object->getType();
		if (objectType == MenuObject::TYPE_FRAME)
		{
			item->setText(1, "Frame");
			buildObjectTree(object, item);
		}
		else if (objectType == MenuObject::TYPE_TEXT)
			item->setText(1, "Text");
		else if (objectType == MenuObject::TYPE_IMAGE)
			item->setText(1, "Image");
		else if (objectType == MenuObject::TYPE_SELECTABLEAREA)
		{
			item->setText(1, "Selectable Area");
			buildObjectTree(object, item);
		}
		else if (objectType == MenuObject::TYPE_SCROLLAREA)
		{
			item->setText(1, "Scroll Area");
			buildObjectTree(object, item);
		}
		else if (objectType == MenuObject::TYPE_ITEMLIST)
			item->setText(1, "Item List");
	}
}

void ScreenLayouts_PopupMenu::deleteChildItems(QTreeWidgetItem *item)
{
	for (int i = 0; i < item->childCount(); ++i)
	{
		QTreeWidgetItem *childItem = item->takeChild(i);
		deleteChildItems(childItem);
		objectItemList.remove(childItem->text(0));
		delete childItem;
	}
}

void ScreenLayouts_PopupMenu::addObjectToLayout(MenuObject *object)
{
	object->setLayout(layoutRef->getDataPointer());
	for (int i = 0; i < object->numChildObjects(); ++i)
		addObjectToLayout(object->getChildObject(i));
}

void ScreenLayouts_PopupMenu::clearGraphicsScene()
{
	popupMenuScene->clear();
	menuBackgroundItem = new QGraphicsRectItem(0, 0, 640, 480);
	menuBackgroundItem->setPen(Qt::NoPen);
	if (ProjectData::systemGraphics.stretchToFit)
		menuBackgroundItem->setBrush(QBrush(QImage(ProjectData::systemGraphics.menuBGLocation).scaled(640, 480)));
	else
		menuBackgroundItem->setBrush(QBrush(QImage(ProjectData::systemGraphics.menuBGLocation)));
	menuBackgroundItem->setZValue(-2);
	popupMenuScene->addItem(menuBackgroundItem);
	menuGridItem = new QGraphicsRectItem(0, 0, 640, 480);
	menuGridItem->setBrush(QBrush(QImage(":/images/menugrid_2x2.png")));
	menuGridItem->setZValue(-1);
	menuGridItem->setVisible(false);
	popupMenuScene->addItem(menuGridItem);
	resizeItem_topLeft = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_topLeft->setPen(QPen(Qt::NoPen));
	resizeItem_topLeft->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_topLeft->setCursor(Qt::SizeFDiagCursor);
	resizeItem_topLeft->setPos(0, 0);
	resizeItem_topLeft->setZValue(1000);
	popupMenuScene->addItem(resizeItem_topLeft);
	resizeItem_top = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_top->setPen(QPen(Qt::NoPen));
	resizeItem_top->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_top->setCursor(Qt::SizeVerCursor);
	resizeItem_top->setPos(32, 0);
	resizeItem_top->setZValue(1000);
	popupMenuScene->addItem(resizeItem_top);
	resizeItem_topRight = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_topRight->setPen(QPen(Qt::NoPen));
	resizeItem_topRight->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_topRight->setCursor(Qt::SizeBDiagCursor);
	resizeItem_topRight->setPos(64, 0);
	resizeItem_topRight->setZValue(1000);
	popupMenuScene->addItem(resizeItem_topRight);
	resizeItem_left = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_left->setPen(QPen(Qt::NoPen));
	resizeItem_left->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_left->setCursor(Qt::SizeHorCursor);
	resizeItem_left->setPos(0, 32);
	resizeItem_left->setZValue(1000);
	popupMenuScene->addItem(resizeItem_left);
	resizeItem_right = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_right->setPen(QPen(Qt::NoPen));
	resizeItem_right->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_right->setCursor(Qt::SizeHorCursor);
	resizeItem_right->setPos(64, 32);
	resizeItem_right->setZValue(1000);
	popupMenuScene->addItem(resizeItem_right);
	resizeItem_bottomLeft = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_bottomLeft->setPen(QPen(Qt::NoPen));
	resizeItem_bottomLeft->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_bottomLeft->setCursor(Qt::SizeBDiagCursor);
	resizeItem_bottomLeft->setPos(0, 64);
	resizeItem_bottomLeft->setZValue(1000);
	popupMenuScene->addItem(resizeItem_bottomLeft);
	resizeItem_bottom = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_bottom->setPen(QPen(Qt::NoPen));
	resizeItem_bottom->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_bottom->setCursor(Qt::SizeVerCursor);
	resizeItem_bottom->setPos(32, 64);
	resizeItem_bottom->setZValue(1000);
	popupMenuScene->addItem(resizeItem_bottom);
	resizeItem_bottomRight = new QGraphicsRectItem(0, 0, 4, 4);
	resizeItem_bottomRight->setPen(QPen(Qt::NoPen));
	resizeItem_bottomRight->setBrush(QBrush(QColor(255, 0, 0)));
	resizeItem_bottomRight->setCursor(Qt::SizeFDiagCursor);
	resizeItem_bottomRight->setPos(64, 64);
	resizeItem_bottomRight->setZValue(1000);
	popupMenuScene->addItem(resizeItem_bottomRight);
	selectionRect = new QGraphicsRectItem(0, 0, 64, 64);
	selectionRect->setPen(QPen(QBrush(QColor(0, 255, 255)), 3, Qt::DashLine));
	selectionRect->setBrush(QBrush(Qt::NoBrush));
	selectionRect->setPos(0, 0);
	selectionRect->setZValue(1000);
	selectionRect->setVisible(false);
	popupMenuScene->addItem(selectionRect);
	highlightedObject = new QGraphicsRectItem(0, 0, 0, 0);
	highlightedObject->setPen(QPen(Qt::NoPen));
	highlightedObject->setBrush(QBrush(QColor(255, 255, 255)));
	highlightedObject->setPos(0, 0);
	highlightedObject->setZValue(1000);
	highlightedObject->setOpacity(.25);
	highlightedObject->setVisible(false);
	popupMenuScene->addItem(highlightedObject);
}
