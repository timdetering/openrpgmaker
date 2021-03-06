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

#include <QHeaderView>
#include <QGraphicsPixmapItem>
#include <QTableWidgetItem>
#include <QFileInfo>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QMetaType>
#include <QBitmap>
#include <QMenu>
#include <QToolTip>
#include <QHeaderView>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "animationautocomplete.h"
#include "animationcellproperties.h"
#include "animationmasscopyclear.h"
#include "animationmassmodify.h"
#include "arraysize.h"
#include "audiomanager.h"
#include "clipboard.h"
#include "configureanimationeffect.h"
#include "database_globalanimations.h"
#include "imagebrowser.h"
#include "projectdata.h"
#include "projectdata_globalanimation.h"
#include "xmlParser.h"

Database_GlobalAnimations::Database_GlobalAnimations(QWidget *parent) : QWidget(parent)
{
	QHeaderView *headerView;
	QPainter painter;
	ignoreEvents = true;
	setupUi(this);
	QObject::connect(hsbAnimationImagesScrollBar, SIGNAL(rangeChanged(int, int)), gvAnimationImages, SLOT(setHScrollBarRange(int, int)));
	QObject::connect(hsbAnimationImagesScrollBar, SIGNAL(valueChanged(int)), gvAnimationImages, SLOT(setHScrollBarValue(int)));
	QObject::connect(twAdditionalEffects, SIGNAL(cellDoubleClicked(int, int)), bConfigureEffect, SLOT(click()));
	headerView = twAdditionalEffects->horizontalHeader();
	headerView->resizeSections(QHeaderView::ResizeToContents);
	headerView->setResizeMode(0, QHeaderView::Fixed);
	headerView->setResizeMode(1, QHeaderView::Stretch);
	bFrameRateSideButton->setSpinBox(sbFrameRate);
	lwFrameList->setCurrentRow(0);
	animationScene = new QGraphicsScene(0, 0, 1024, 864);
	gvAnimation->setScene(animationScene);
	gvAnimation->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	imagesScene = new QGraphicsScene(0, 0, 192, 192);
	gvAnimationImages->setScene(imagesScene);
	gvAnimationImages->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	cbShowBGImage->setEnabled(false);
	animationFramePen = QPen(QBrush(QColor(128, 0, 0)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	animationFramePen_selected = QPen(QBrush(QColor(255, 255, 255)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	animationFramePen_old = QPen(QBrush(QColor(0, 0, 128)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	animationFrameBrush = QBrush(QColor(255, 0, 0));
	animationFrameBrush_selected = QBrush(QColor(128, 128, 128));
	animationFrameBrush_old = QBrush(QColor(0, 0, 255));
	animationImageFrame_numbers = QImage(":/images/animationImageFrameNumbers.png");
	for (int i = 0; i < 16; ++i)
	{
		QImage image(192, 192, QImage::Format_ARGB32);
		animationImageItemFrames[i] = new QGraphicsRectItem(-96, -96, 192, 192);
		animationImageItemFrames[i]->setZValue(i + 18);
		animationImageItemFrames[i]->setVisible(false);
		animationImageItemFrames[i]->setPen(animationFramePen);
		animationImageItemFrames[i]->setBrush(QBrush(Qt::NoBrush));
		animationImageItemFrames[i]->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
		animationScene->addItem(animationImageItemFrames[i]);
		animationImageTintEffects[i] = new MyGraphicsTintEffect();
		animationImageItems[i] = new QGraphicsPixmapItem(animationImageItemFrames[i]);
		animationImageItems[i]->setPos(-96, -96);
		animationImageItems[i]->setZValue(0);
		animationImageItems[i]->setVisible(false);
		animationImageItems[i]->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
		animationImageItems[i]->setGraphicsEffect(animationImageTintEffects[i]);
		animationImageItemFrameNumberBackground[i] = new QGraphicsEllipseItem(-16, -16, 32, 32, animationImageItemFrames[i]);
		animationImageItemFrameNumberBackground[i]->setPos(-96, -96);
		animationImageItemFrameNumberBackground[i]->setZValue(1);
		animationImageItemFrameNumberBackground[i]->setVisible(false);
		animationImageItemFrameNumberBackground[i]->setPen(animationFramePen);
		animationImageItemFrameNumberBackground[i]->setBrush(animationFrameBrush);
		animationImageItemFrameNumberBackground[i]->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		animationImageItemFrameNumber[i] = new QGraphicsPixmapItem(animationImageItemFrameNumberBackground[i]);
		animationImageItemFrameNumber[i]->setPixmap(QPixmap::fromImage(animationImageFrame_numbers.copy(i * 20, 0, 20, 12)));
		animationImageItemFrameNumber[i]->setPos(-10, -6);
		animationImageItemFrameNumber[i]->setVisible(false);
		animationImageItemFrameNumber[i]->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		animationPreviousFrames[i] = new QGraphicsRectItem(-96, -96, 192, 192);
		animationPreviousFrames[i]->setZValue(i + 2);
		animationPreviousFrames[i]->setVisible(false);
		animationPreviousFrames[i]->setPen(animationFramePen_old);
		animationPreviousFrames[i]->setBrush(QBrush(Qt::NoBrush));
		animationPreviousFrames[i]->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
		animationScene->addItem(animationPreviousFrames[i]);
		animationPreviousFrameNumberBackground[i] = new QGraphicsEllipseItem(-16, -16, 32, 32, animationPreviousFrames[i]);
		animationPreviousFrameNumberBackground[i]->setPos(-96, -96);
		animationPreviousFrameNumberBackground[i]->setZValue(0);
		animationPreviousFrameNumberBackground[i]->setVisible(false);
		animationPreviousFrameNumberBackground[i]->setPen(animationFramePen_old);
		animationPreviousFrameNumberBackground[i]->setBrush(animationFrameBrush_old);
		animationPreviousFrameNumberBackground[i]->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		animationPreviousFrameNumber[i] = new QGraphicsPixmapItem(animationPreviousFrameNumberBackground[i]);
		animationPreviousFrameNumber[i]->setPixmap(QPixmap::fromImage(animationImageFrame_numbers.copy(i * 20, 0, 20, 12)));
		animationPreviousFrameNumber[i]->setPos(-10, -6);
		animationPreviousFrameNumber[i]->setVisible(false);
		animationPreviousFrameNumber[i]->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	}
	animationBGItem = new QGraphicsPixmapItem;
	animationBGItem->setPos(192, 192);
	animationBGItem->setZValue(1);
	animationBGItem->setVisible(false);
	animationScene->addItem(animationBGItem);
	blackBackground = new QGraphicsRectItem(0, 0, 640, 480);
	blackBackground->setPos(192, 192);
	blackBackground->setZValue(0);
	blackBackground->setVisible(true);
	blackBackground->setPen(QPen(Qt::NoPen));
	blackBackground->setBrush(QBrush(Qt::black));
	animationScene->addItem(blackBackground);
	screenQuadrant1Rect1 = new QGraphicsRectItem(511, 192, 320, 240);
	screenQuadrant1Rect1->setPen(QPen(QBrush(QColor(0, 255, 0)), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	screenQuadrant1Rect1->setBrush(QBrush(Qt::NoBrush));
	screenQuadrant1Rect1->setZValue(100);
	animationScene->addItem(screenQuadrant1Rect1);
	screenQuadrant1Rect2 = new QGraphicsRectItem(512, 193, 318, 238);
	screenQuadrant1Rect2->setPen(QPen(QBrush(QColor(0, 255, 0)), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	screenQuadrant1Rect2->setBrush(QBrush(Qt::NoBrush));
	screenQuadrant1Rect2->setZValue(100);
	animationScene->addItem(screenQuadrant1Rect2);
	screenQuadrant2Rect1 = new QGraphicsRectItem(192, 192, 320, 240);
	screenQuadrant2Rect1->setPen(QPen(QBrush(QColor(0, 255, 0)), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	screenQuadrant2Rect1->setBrush(QBrush(Qt::NoBrush));
	screenQuadrant2Rect1->setZValue(100);
	animationScene->addItem(screenQuadrant2Rect1);
	screenQuadrant2Rect2 = new QGraphicsRectItem(193, 193, 318, 238);
	screenQuadrant2Rect2->setPen(QPen(QBrush(QColor(0, 255, 0)), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	screenQuadrant2Rect2->setBrush(QBrush(Qt::NoBrush));
	screenQuadrant2Rect2->setZValue(100);
	animationScene->addItem(screenQuadrant2Rect2);
	screenQuadrant3Rect1 = new QGraphicsRectItem(192, 431, 320, 240);
	screenQuadrant3Rect1->setPen(QPen(QBrush(QColor(0, 255, 0)), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	screenQuadrant3Rect1->setBrush(QBrush(Qt::NoBrush));
	screenQuadrant3Rect1->setZValue(100);
	animationScene->addItem(screenQuadrant3Rect1);
	screenQuadrant3Rect2 = new QGraphicsRectItem(193, 432, 318, 238);
	screenQuadrant3Rect2->setPen(QPen(QBrush(QColor(0, 255, 0)), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	screenQuadrant3Rect2->setBrush(QBrush(Qt::NoBrush));
	screenQuadrant3Rect2->setZValue(100);
	animationScene->addItem(screenQuadrant3Rect2);
	screenQuadrant4Rect1 = new QGraphicsRectItem(511, 431, 320, 240);
	screenQuadrant4Rect1->setPen(QPen(QBrush(QColor(0, 255, 0)), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	screenQuadrant4Rect1->setBrush(QBrush(Qt::NoBrush));
	screenQuadrant4Rect1->setZValue(100);
	animationScene->addItem(screenQuadrant4Rect1);
	screenQuadrant4Rect2 = new QGraphicsRectItem(512, 432, 318, 238);
	screenQuadrant4Rect2->setPen(QPen(QBrush(QColor(0, 255, 0)), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	screenQuadrant4Rect2->setBrush(QBrush(Qt::NoBrush));
	screenQuadrant4Rect2->setZValue(100);
	animationScene->addItem(screenQuadrant4Rect2);
	previewWidget = new GlobalAnimationPreview(this);
	previewWidget->setGeometry((width() - 640) / 2, (height() - 480) / 2, 640, 480);
	previewWidget->setAutoFillBackground(true);
	previewWidget->raise();
	previewWidget->setVisible(false);
	animationImageFrame_numbers = QImage(":/images/animationImageFrame_numbers.png");
	currentViewScale = 1.0;
	aCellProperties->setShortcut(QKeySequence(Qt::Key_Return));
	aCopy->setShortcut(QKeySequence(QKeySequence::Copy));
	aCut->setShortcut(QKeySequence(QKeySequence::Cut));
	aPaste->setShortcut(QKeySequence(QKeySequence::Paste));
	aDelete->setShortcut(QKeySequence(QKeySequence::Delete));
	aMoveBack->setShortcut(QKeySequence(Qt::Key_PageDown));
	aMoveForward->setShortcut(QKeySequence(Qt::Key_PageUp));
	aMoveToBack->setShortcut(QKeySequence(Qt::Key_End));
	aMoveToFront->setShortcut(QKeySequence(Qt::Key_Home));
	gvAnimation->addAction(aCellProperties);
	gvAnimation->addAction(aCopy);
	gvAnimation->addAction(aCut);
	gvAnimation->addAction(aPaste);
	gvAnimation->addAction(aDelete);
	gvAnimation->addAction(aMoveBack);
	gvAnimation->addAction(aMoveForward);
	gvAnimation->addAction(aMoveToBack);
	gvAnimation->addAction(aMoveToFront);
	contextMenuScenePos = QPoint(-1, -1);
	wSnapToGrid->setEnabled(false);
	animRef = NULL;
}

Database_GlobalAnimations::~Database_GlobalAnimations()
{
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

void Database_GlobalAnimations::setupGlobalAnimations()
{
	updateAnimationList();
	if (lwFrameList->currentRow() == 0)
		bClonePrevious->setEnabled(false);
	else
		bClonePrevious->setEnabled(true);
	ignoreEvents = false;
}

void Database_GlobalAnimations::shutdownGlobalAnimations()
{
	ignoreEvents = true;
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

void Database_GlobalAnimations::on_lwAnimationList_itemSelectionChanged()
{
	QGraphicsPixmapItem *item;
	int currentRow = lwAnimationList->currentRow();
	ignoreEvents = true;
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
	animRef = new ProjectData::DataReference<GlobalAnimation>(currentRow, __FILE__, __LINE__);
	leName->setText((*animRef)->getName());
	sbFrameRate->setValue((*animRef)->getFrameRate());
	leNumFrames->setText(QString::number((*animRef)->numFrames()));
	imagesScene->setSceneRect(0, 0, 192 * (*animRef)->numImages(), 192);
	lblNumImages->setText(QString("%1/256").arg((*animRef)->numImages()));
	if ((*animRef)->numImages() == 0)
		bRemoveImage->setEnabled(false);
	else if ((*animRef)->numImages() == 256)
		bAddImage->setEnabled(false);
	for (int i = 0; i < imageItems.size(); ++i)
	{
		imagesScene->removeItem(imageItems[i]);
		imagesScene->removeItem(imageItemGrids[i]);
		delete (imageItems[i]);
		delete (imageItemGrids[i]);
	}
	imageItems.clear();
	imageItemGrids.clear();
	effectIDs.clear();
	for (int i = 0; i < (*animRef)->numImages(); ++i)
	{
		item = new QGraphicsPixmapItem;
		item->setPixmap(QPixmap((*animRef)->getImageLocation(i)));
		item->setPos(192 * i, 0);
		item->setZValue(0);
		imagesScene->addItem(item);
		imageItems.append(item);
		if (i == 0)
			item = new QGraphicsPixmapItem(QPixmap(":/images/animationImageGrid_selected.png"));
		else
			item = new QGraphicsPixmapItem(QPixmap(":/images/animationImageGrid.png"));
		item->setPos(192 * i, 0);
		item->setZValue(1);
		imagesScene->addItem(item);
		imageItemGrids.append(item);
	}
	currentImageItem = 0;
	updateEffectList();
	lwFrameList->setCurrentRow(0);
	currentFrame = (*animRef)->getFrame(0);
	ignoreEvents = false;
	on_twAdditionalEffects_itemSelectionChanged();
	updateFrame();
}

void Database_GlobalAnimations::on_bArraySize_clicked()
{
	int numAnimations = ProjectData::globalAnimationNames.size();
	ArraySize *arraysize = new ArraySize(numAnimations, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numAnimations)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (animRef != NULL)
				{
					if (newSize >= animRef->getDataID())
					{
						delete animRef;
						animRef = NULL;
					}
				}
				for (int i = numAnimations - 1; i >= newSize; --i)
					ProjectData::deleteGlobalAnimation(i);
			}
		}
		else
		{
			for (int i = numAnimations; i < newSize; ++i)
			{
				GlobalAnimation *animation = new GlobalAnimation;
				animation->setName(QString("Anim%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addGlobalAnimation(animation);
				delete animation;
			}
		}
		ignoreEvents = true;
		updateAnimationList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_GlobalAnimations::on_gvAnimation_mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		int itemID = getItemUnderCursor(gvAnimation->mapToScene(event->pos()).toPoint(), true);
		if (itemID != -1)
		{
			dragStartPos = animationImageItemFrames[itemID]->pos().toPoint() - gvAnimation->mapToScene(event->pos()).toPoint();
			QToolTip::showText(gvAnimation->mapToGlobal(event->pos()), QString("(%1, %2)").arg(currentFrame->imageList[itemID]->x).arg(currentFrame->imageList[itemID]->y), gvAnimation);
		}
		else
			dragStartPos = QPoint(0, 0);
		if (itemID != currentAnimationImage && itemID != -1)
		{
			animationImageItemFrames[currentAnimationImage]->setPen(animationFramePen);
			animationImageItemFrameNumberBackground[currentAnimationImage]->setPen(animationFramePen);
			animationImageItemFrameNumberBackground[currentAnimationImage]->setBrush(animationFrameBrush);
			currentAnimationImage = itemID;
			animationImageItemFrames[currentAnimationImage]->setPen(animationFramePen_selected);
			animationImageItemFrameNumberBackground[currentAnimationImage]->setPen(animationFramePen_selected);
			animationImageItemFrameNumberBackground[currentAnimationImage]->setBrush(animationFrameBrush_selected);
		}
	}
}

void Database_GlobalAnimations::on_gvAnimation_mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		int itemID = getItemUnderCursor(gvAnimation->mapToScene(event->pos()).toPoint(), false);
		if (itemID != -1)
		{
			GlobalAnimation::ImageLocation *image = currentFrame->imageList[itemID];
			AnimationCellProperties *cellProperties = new AnimationCellProperties((*animRef)->getImageLocations(), image->imageID, (int)(image->scale * 100), (int)(image->rotation), int(image->opacity * 100), image->tintColor, image->tintAmount);
			if (cellProperties->exec())
			{
				image->tintColor = cellProperties->getTintColor();
				image->rotation = (float)cellProperties->getRotation();
				image->scale = cellProperties->getScale() / 100.0;
				image->opacity = cellProperties->getOpacity() / 100.0;
				image->imageID = cellProperties->getImageID();
				image->tintAmount = cellProperties->getTintAmount();
				updateFrame();
			}
			delete cellProperties;
		}
	}
}

void Database_GlobalAnimations::on_gvAnimation_mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) != 0 && dragStartPos != QPoint(0, 0))
	{
		QPoint currentPos = gvAnimation->mapToScene(event->pos()).toPoint() + dragStartPos;
		if (cbSnapToGrid->isChecked())
		{
			int gridSize = cboxGridSize->currentIndex();
			if (gridSize == SIZE_2x2)
			{
				currentPos.setX((currentPos.x() / 2) * 2);
				currentPos.setY((currentPos.y() / 2) * 2);
			}
			else if (gridSize == SIZE_4x4)
			{
				currentPos.setX((currentPos.x() / 4) * 4);
				currentPos.setY((currentPos.y() / 4) * 4);
			}
			else if (gridSize == SIZE_8x8)
			{
				currentPos.setX((currentPos.x() / 8) * 8);
				currentPos.setY((currentPos.y() / 8) * 8);
			}
			else if (gridSize == SIZE_16x16)
			{
				currentPos.setX((currentPos.x() / 16) * 16);
				currentPos.setY((currentPos.y() / 16) * 16);
			}
			else if (gridSize == SIZE_32x32)
			{
				currentPos.setX((currentPos.x() / 32) * 32);
				currentPos.setY((currentPos.y() / 32) * 32);
			}
			else if (gridSize == SIZE_64x64)
			{
				currentPos.setX((currentPos.x() / 64) * 64);
				currentPos.setY((currentPos.y() / 64) * 64);
			}
		}
		animationImageItemFrames[currentAnimationImage]->setPos(currentPos);
		QToolTip::showText(gvAnimation->mapToGlobal(event->pos()), QString("(%1, %2)").arg(currentPos.x() - 192).arg(currentPos.y() - 192), gvAnimation);
	}
}

void Database_GlobalAnimations::on_gvAnimation_mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && dragStartPos != QPoint(0, 0) && numImagesInFrame > 0)
	{
		GlobalAnimation::ImageLocation *image = currentFrame->imageList[currentAnimationImage];
		QPoint currentPos = gvAnimation->mapToScene(event->pos()).toPoint() + dragStartPos;
		if (cbSnapToGrid->isChecked())
		{
			int gridSize = cboxGridSize->currentIndex();
			if (gridSize == SIZE_2x2)
			{
				currentPos.setX((currentPos.x() / 2) * 2);
				currentPos.setY((currentPos.y() / 2) * 2);
			}
			else if (gridSize == SIZE_4x4)
			{
				currentPos.setX((currentPos.x() / 4) * 4);
				currentPos.setY((currentPos.y() / 4) * 4);
			}
			else if (gridSize == SIZE_8x8)
			{
				currentPos.setX((currentPos.x() / 8) * 8);
				currentPos.setY((currentPos.y() / 8) * 8);
			}
			else if (gridSize == SIZE_16x16)
			{
				currentPos.setX((currentPos.x() / 16) * 16);
				currentPos.setY((currentPos.y() / 16) * 16);
			}
			else if (gridSize == SIZE_32x32)
			{
				currentPos.setX((currentPos.x() / 32) * 32);
				currentPos.setY((currentPos.y() / 32) * 32);
			}
			else if (gridSize == SIZE_64x64)
			{
				currentPos.setX((currentPos.x() / 64) * 64);
				currentPos.setY((currentPos.y() / 64) * 64);
			}
		}
		animationImageItemFrames[currentAnimationImage]->setPos(currentPos);
		image->x = currentPos.x() - 192;
		image->y = currentPos.y() - 192;
		QToolTip::hideText();
		updateFrame();
	}
}

void Database_GlobalAnimations::on_gvAnimation_dragEnterEvent(QDragEnterEvent *event)
{
	if (event->source() == gvAnimationImages || event->source() == gvAnimation)
		event->acceptProposedAction();
}

void Database_GlobalAnimations::on_gvAnimation_dropEvent(QDropEvent *event)
{
	if (event->source() == gvAnimationImages && numImagesInFrame < 16)
	{
		GlobalAnimation::ImageLocation *imageLocation = new GlobalAnimation::ImageLocation;
		QPoint dropPoint = gvAnimation->mapToScene(event->pos()).toPoint() - (hotSpot / currentViewScale) + QPoint(96, 96);
		imageLocation->tintColor = QColor(0, 0, 0);
		imageLocation->rotation = 0.0;
		imageLocation->scale = 1.0;
		imageLocation->opacity = 1.0;
		imageLocation->imageID = currentImageItem;
		imageLocation->tintAmount = 0;
		imageLocation->x = dropPoint.x() - 192;
		imageLocation->y = dropPoint.y() - 192;
		currentAnimationImage = currentFrame->imageList.size();
		currentFrame->imageList.append(imageLocation);
		updateFrame();
	}
}

void Database_GlobalAnimations::on_gvAnimation_customContextMenuRequested(QPoint pos)
{
	QMenu *popupMenu = new QMenu(this);
	int itemID;
	contextMenuScenePos = gvAnimation->mapToScene(pos).toPoint();
	itemID = getItemUnderCursor(contextMenuScenePos, false);
	if (itemID != currentAnimationImage && itemID != -1)
	{
		animationImageItemFrames[currentAnimationImage]->setPen(animationFramePen);
		animationImageItemFrameNumberBackground[currentAnimationImage]->setPen(animationFramePen);
		animationImageItemFrameNumberBackground[currentAnimationImage]->setBrush(animationFrameBrush);
		currentAnimationImage = itemID;
		animationImageItemFrames[currentAnimationImage]->setPen(animationFramePen_selected);
		animationImageItemFrameNumberBackground[currentAnimationImage]->setPen(animationFramePen_selected);
		animationImageItemFrameNumberBackground[currentAnimationImage]->setBrush(animationFrameBrush_selected);
	}
	popupMenu->addAction(aCellProperties);
	popupMenu->addSeparator();
	popupMenu->addAction(aCopy);
	popupMenu->addAction(aCut);
	popupMenu->addAction(aPaste);
	popupMenu->addAction(aDelete);
	popupMenu->addSeparator();
	popupMenu->addAction(aMoveBack);
	popupMenu->addAction(aMoveForward);
	popupMenu->addAction(aMoveToBack);
	popupMenu->addAction(aMoveToFront);
	if (Clipboard::dataType == Clipboard::TYPE_GLOBALANIMATION_IMAGELOCATION)
		aPaste->setEnabled(true);
	else
		aPaste->setEnabled(false);
	if (itemID != -1)
	{
		aCellProperties->setEnabled(true);
		aCopy->setEnabled(true);
		aCut->setEnabled(true);
		aDelete->setEnabled(true);
		aMoveBack->setEnabled(true);
		aMoveForward->setEnabled(true);
		aMoveToBack->setEnabled(true);
		aMoveToFront->setEnabled(true);
	}
	else
	{
		aCellProperties->setEnabled(false);
		aCopy->setEnabled(false);
		aCut->setEnabled(false);
		aDelete->setEnabled(false);
		aMoveBack->setEnabled(false);
		aMoveForward->setEnabled(false);
		aMoveToBack->setEnabled(false);
		aMoveToFront->setEnabled(false);
	}
	if (numImagesInFrame <= 1)
	{
		aMoveBack->setEnabled(false);
		aMoveForward->setEnabled(false);
		aMoveToBack->setEnabled(false);
		aMoveToFront->setEnabled(false);
	}
	else
	{
		if (itemID > 0)
		{
			aMoveBack->setEnabled(true);
			aMoveToBack->setEnabled(true);
		}
		else
		{
			aMoveBack->setEnabled(false);
			aMoveToBack->setEnabled(false);
		}
		if (itemID < numImagesInFrame - 1)
		{
			aMoveForward->setEnabled(true);
			aMoveToFront->setEnabled(true);
		}
		else
		{
			aMoveForward->setEnabled(false);
			aMoveToFront->setEnabled(false);
		}
	}
	popupMenu->exec(gvAnimation->mapToGlobal(pos));
	delete popupMenu;
	contextMenuScenePos = QPoint(-1, -1);
	aCellProperties->setEnabled(true);
	aCopy->setEnabled(true);
	aCut->setEnabled(true);
	aPaste->setEnabled(true);
	aDelete->setEnabled(true);
	aMoveBack->setEnabled(true);
	aMoveForward->setEnabled(true);
	aMoveToBack->setEnabled(true);
	aMoveToFront->setEnabled(true);
}

void Database_GlobalAnimations::on_gvAnimationImages_mousePressEvent(QMouseEvent *event)
{
	int imageID = gvAnimationImages->mapToScene(event->pos()).x() / 192;
	if (event->button() == Qt::LeftButton)
	{
		imageItemGrids[currentImageItem]->setPixmap(QPixmap(":/images/animationImageGrid.png"));
		currentImageItem = imageID;
		dragStartPos = event->pos();
		imageItemGrids[currentImageItem]->setPixmap(QPixmap(":/images/animationImageGrid_selected.png"));
	}
}

void Database_GlobalAnimations::on_gvAnimationImages_mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) != 0 && dragStartPos.x() >= 0 && (event->pos() - dragStartPos).manhattanLength() >= QApplication::startDragDistance())
	{
		QDrag *drag = new QDrag(gvAnimationImages);
		QMimeData *mimeData = new QMimeData;
		QPixmap pixmap = QPixmap((*animRef)->getImageLocation(currentImageItem));
		pixmap.setMask(pixmap.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
		mimeData->setData(QMetaType::typeName(QMetaType::Int), QByteArray::number(currentImageItem));
		drag->setMimeData(mimeData);
		drag->setPixmap(pixmap.scaled((int)(192 * currentViewScale), (int)(192 * currentViewScale)));
		hotSpot = gvAnimationImages->mapToScene(dragStartPos).toPoint();
		hotSpot = QPoint((int)((hotSpot.x() % 192) * currentViewScale), (int)((hotSpot.y() % 192) * currentViewScale));
		drag->setHotSpot(hotSpot);
		dragStartPos = QPoint(-1, -1);
		drag->exec(Qt::CopyAction);
	}
}

void Database_GlobalAnimations::on_gvAnimationImages_hScrollBarRangeChanged(int min, int max)
{
	if (min != hsbAnimationImagesScrollBar->minimum() || max != hsbAnimationImagesScrollBar->maximum())
		hsbAnimationImagesScrollBar->setRange(min, max);
	hsbAnimationImagesScrollBar->setSingleStep(gvAnimationImages->horizontalScrollBar()->singleStep());
	hsbAnimationImagesScrollBar->setPageStep(gvAnimationImages->horizontalScrollBar()->pageStep());
}

void Database_GlobalAnimations::on_gvAnimationImages_hScrollBarValueChanged(int value)
{
	if (value != hsbAnimationImagesScrollBar->value())
		hsbAnimationImagesScrollBar->setValue(value);
}

void Database_GlobalAnimations::on_bRemoveImage_clicked()
{
	if (currentImageItem != -1)
	{
		int confirmation = QMessageBox::question(this, "Delete Image?", "Are you sure you want to remove the selected image from the animation?", QMessageBox::Yes|QMessageBox::No);
		if (confirmation == QMessageBox::Yes)
		{
			(*animRef)->removeImage(currentImageItem);
			imagesScene->removeItem(imageItems[currentImageItem]);
			imagesScene->removeItem(imageItemGrids[currentImageItem]);
			delete (imageItems[currentImageItem]);
			delete (imageItemGrids[currentImageItem]);
			imageItems.removeAt(currentImageItem);
			imageItemGrids.removeAt(currentImageItem);
			for (int i = currentImageItem; i < imageItems.size(); ++i)
			{
				imageItems[i]->setPos(i * 192, 0);
				imageItemGrids[i]->setPos(i * 192, 0);
			}
			if (currentImageItem == imageItems.size())
				--currentImageItem;
			imageItemGrids[currentImageItem]->setPixmap(QPixmap(":/images/animationImageGrid_selected.png"));
			updateFrame();
		}
	}
}

void Database_GlobalAnimations::on_bAddImage_clicked()
{
	QGraphicsPixmapItem *item;
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/animations", ImageBrowser::BlockNone, QList<QSize>() << QSize(192, 192));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		item = new QGraphicsPixmapItem(QPixmap(fileLocation));
		item->setPos(imageItems.size() * 192, 0);
		item->setZValue(0);
		imagesScene->addItem(item);
		imageItems.append(item);
		item = new QGraphicsPixmapItem(QPixmap(":/images/animationImageGrid.png"));
		item->setPos(imageItemGrids.size() * 192, 0);
		item->setZValue(1);
		imagesScene->addItem(item);
		imageItemGrids.append(item);
		(*animRef)->addImage(fileLocation);
	}
	delete imageBrowser;
	imagesScene->setSceneRect(0, 0, imageItems.size() * 192, 192);
}

void Database_GlobalAnimations::on_leNumFrames_textChanged(QString text)
{
	int currentRow = lwFrameList->currentRow();
	int numFrames = text.toInt();
	lwFrameList->clear();
	for (int i = 0; i < numFrames; ++i)
		lwFrameList->addItem(QString("< %1 >").arg(i + 1, 3));
	if (numFrames > currentRow)
		lwFrameList->setCurrentRow(currentRow);
	else
		lwFrameList->setCurrentRow(0);
	if (!ignoreEvents)
		updateFrame();
}

void Database_GlobalAnimations::on_bSetNumFrames_clicked()
{
	ArraySize *arraySize = new ArraySize(leNumFrames->text().toInt(), 1, 360);
	if (arraySize->exec())
		leNumFrames->setText(QString::number(arraySize->getValue()));
	delete arraySize;
}

void Database_GlobalAnimations::on_lwFrameList_itemSelectionChanged()
{
	if (!ignoreEvents)
	{
		currentFrame = (*animRef)->getFrame(lwFrameList->currentRow());
		if (lwFrameList->currentRow() == 0)
			bClonePrevious->setEnabled(false);
		else
			bClonePrevious->setEnabled(true);
		currentAnimationImage = currentFrame->imageList.size() - 1;
		updateFrame();
	}
}

void Database_GlobalAnimations::on_bClonePrevious_clicked()
{
	GlobalAnimation::AnimationFrame *previousFrame = (*animRef)->getFrame(lwFrameList->currentRow() - 1);
	for (int i = 0; i < currentFrame->imageList.size(); ++i)
		delete (currentFrame->imageList[i]);
	for (int i = 0; i < currentFrame->animationEffects.size(); ++i)
	{
		if (currentFrame->animationEffects[i]->soundEffect != NULL)
			delete (currentFrame->animationEffects[i]->soundEffect);
		delete (currentFrame->animationEffects[i]);
	}
	currentFrame->imageList.clear();
	currentFrame->animationEffects.clear();
	for (int i = 0; i < previousFrame->imageList.size(); ++i)
	{
		GlobalAnimation::ImageLocation *image = previousFrame->imageList[i];
		GlobalAnimation::ImageLocation *copy = new GlobalAnimation::ImageLocation;
		copy->tintColor = image->tintColor;
		copy->rotation = image->rotation;
		copy->scale = image->scale;
		copy->opacity = image->opacity;
		copy->imageID = image->imageID;
		copy->tintAmount = image->tintAmount;
		copy->x = image->x;
		copy->y = image->y;
		currentFrame->imageList.append(copy);
	}
	for (int i = 0; i < previousFrame->animationEffects.size(); ++i)
	{
		GlobalAnimation::AnimationEffect *effect = previousFrame->animationEffects[i];
		GlobalAnimation::AnimationEffect *copy = new GlobalAnimation::AnimationEffect;
		if (effect->soundEffect != NULL)
		{
			copy->soundEffect = new AudioManager::SoundEffectInfo;
			copy->soundEffect->location = effect->soundEffect->location;
			copy->soundEffect->volume = effect->soundEffect->volume;
			copy->soundEffect->panning = effect->soundEffect->panning;
			copy->soundEffect->speed = effect->soundEffect->speed;
			copy->soundEffect->numLoops = effect->soundEffect->numLoops;
			copy->soundEffect->retainPitch = effect->soundEffect->retainPitch;
		}
		copy->flashColor = effect->flashColor;
		copy->flashScope = effect->flashScope;
		copy->flashOpacity = effect->flashOpacity;
		copy->shakeScope = effect->shakeScope;
		copy->shakeMagnitude = effect->shakeMagnitude;
		currentFrame->animationEffects.append(copy);
	}
	currentAnimationImage = currentFrame->imageList.size() - 1;
	updateFrame();
}

void Database_GlobalAnimations::on_bMassCopyClear_clicked()
{
	AnimationMassCopyClear *massCopyClear = new AnimationMassCopyClear(lwFrameList->count(), this);
	if (massCopyClear->exec())
	{
		if (massCopyClear->massCopy())
		{
			GlobalAnimation::AnimationFrame *sourceFrame = (*animRef)->getFrame(massCopyClear->getSourceFrame());
			int startFrame = massCopyClear->getStartFrame();
			int endFrame = massCopyClear->getEndFrame();
			int sourceFrameNumber = massCopyClear->getSourceFrame();
			for (int i = startFrame; i <= endFrame; ++i)
			{
				if (i != sourceFrameNumber)
				{
					GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
					for (int j = 0; j < frame->imageList.size(); ++j)
						delete (frame->imageList[j]);
					for (int j = 0; j < frame->animationEffects.size(); ++j)
					{
						if (frame->animationEffects[j]->soundEffect != NULL)
							delete (frame->animationEffects[j]->soundEffect);
						delete (frame->animationEffects[j]);
					}
					frame->imageList.clear();
					frame->animationEffects.clear();
					for (int j = 0; j < sourceFrame->imageList.size(); ++j)
					{
						GlobalAnimation::ImageLocation *image = sourceFrame->imageList[j];
						GlobalAnimation::ImageLocation *copy = new GlobalAnimation::ImageLocation;
						copy->tintColor = image->tintColor;
						copy->rotation = image->rotation;
						copy->scale = image->scale;
						copy->opacity = image->opacity;
						copy->imageID = image->imageID;
						copy->tintAmount = image->tintAmount;
						copy->x = image->x;
						copy->y = image->y;
						frame->imageList.append(copy);
					}
					for (int j = 0; j < sourceFrame->animationEffects.size(); ++j)
					{
						GlobalAnimation::AnimationEffect *effect = sourceFrame->animationEffects[j];
						GlobalAnimation::AnimationEffect *copy = new GlobalAnimation::AnimationEffect;
						if (effect->soundEffect != NULL)
						{
							copy->soundEffect = new AudioManager::SoundEffectInfo;
							copy->soundEffect->location = effect->soundEffect->location;
							copy->soundEffect->volume = effect->soundEffect->volume;
							copy->soundEffect->panning = effect->soundEffect->panning;
							copy->soundEffect->speed = effect->soundEffect->speed;
							copy->soundEffect->numLoops = effect->soundEffect->numLoops;
							copy->soundEffect->retainPitch = effect->soundEffect->retainPitch;
						}
						copy->flashColor = effect->flashColor;
						copy->flashScope = effect->flashScope;
						copy->flashOpacity = effect->flashOpacity;
						copy->shakeScope = effect->shakeScope;
						copy->shakeMagnitude = effect->shakeMagnitude;
						frame->animationEffects.append(copy);
					}
				}
			}
		}
		else
		{
			int startFrame = massCopyClear->getStartFrame();
			int endFrame = massCopyClear->getEndFrame();
			for (int i = startFrame; i <= endFrame; ++i)
			{
				GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
				for (int j = 0; j < frame->imageList.size(); ++j)
					delete (frame->imageList[j]);
				frame->imageList.clear();
			}
		}
	}
	delete massCopyClear;
	updateFrame();
}

void Database_GlobalAnimations::on_bAutocomplete_clicked()
{
	AnimationAutoComplete *autoComplete = new AnimationAutoComplete(AnimationAutoComplete::TYPE_GLOBAL, animRef->getDataID(), this);
	if (autoComplete->exec())
	{
		GlobalAnimation::AnimationFrame *startFrame;
		GlobalAnimation::AnimationFrame *endFrame;
		GlobalAnimation::ImageLocation *startCell;
		GlobalAnimation::ImageLocation *endCell;
		float rotationDifference;
		float scaleDifference;
		float opacityDifference;
		int imageIDDifference;
		int tintColorRedDifference;
		int tintColorGreenDifference;
		int tintColorBlueDifference;
		int tintAmountDifference;
		int xDifference;
		int yDifference;
		int frameDifference;
		int startFrameID = autoComplete->getStartFrame();
		int endFrameID = autoComplete->getEndFrame();
		int startCellID = autoComplete->getStartCell();
		int endCellID = autoComplete->getEndCell();
		bool carryImageID = autoComplete->carryImageID();
		bool carryScale = autoComplete->carryScale();
		bool carryRotation = autoComplete->carryRotation();
		bool carryOpacity = autoComplete->carryOpacity();
		bool carryTintColor = autoComplete->carryTintColor();
		bool carryTintAmount = autoComplete->carryTintAmount();
		startFrame = (*animRef)->getFrame(startFrameID);
		endFrame = (*animRef)->getFrame(endFrameID);
		frameDifference = endFrameID - startFrameID;
		for (int i = startCellID; i <= endCellID; ++i)
		{
			startCell = startFrame->imageList[i];
			endCell = endFrame->imageList[i];
			rotationDifference = endCell->rotation - startCell->rotation;
			scaleDifference = endCell->scale - startCell->scale;
			opacityDifference = endCell->opacity - startCell->opacity;
			imageIDDifference = endCell->imageID - startCell->imageID;
			tintColorRedDifference = endCell->tintColor.red() - startCell->tintColor.red();
			tintColorGreenDifference = endCell->tintColor.green() - startCell->tintColor.green();
			tintColorBlueDifference = endCell->tintColor.blue() - startCell->tintColor.blue();
			tintAmountDifference = endCell->tintAmount - startCell->tintAmount;
			xDifference = endCell->x - startCell->x;
			yDifference = endCell->y - startCell->y;
			for (int j = startFrameID + 1; j < endFrameID; ++j)
			{
				GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(j);
				GlobalAnimation::ImageLocation *cell;
				int currentFrameID = j - startFrameID;
				if (frame->imageList.size() <= i)
				{
					cell = new GlobalAnimation::ImageLocation;
					frame->imageList.append(cell);
				}
				else
					cell = frame->imageList[i];
				cell->tintColor = startCell->tintColor;
				cell->rotation = startCell->rotation;
				cell->scale = startCell->scale;
				cell->opacity = startCell->opacity;
				cell->imageID = startCell->imageID;
				cell->tintAmount = startCell->tintAmount;
				cell->x = startCell->x;
				cell->y = startCell->y;
				if (carryImageID)
					cell->imageID += (currentFrameID * imageIDDifference) / frameDifference;
				if (carryScale)
					cell->scale += ((float)currentFrameID * scaleDifference) / (float)frameDifference;
				if (carryRotation)
					cell->rotation += ((float)currentFrameID * rotationDifference) / (float)frameDifference;
				if (carryOpacity)
					cell->opacity += ((float)currentFrameID * opacityDifference) / (float)frameDifference;
				if (carryTintColor)
				{
					int red = (currentFrameID * tintColorRedDifference) / frameDifference + cell->tintColor.red();
					int green = (currentFrameID * tintColorGreenDifference) / frameDifference + cell->tintColor.green();
					int blue = (currentFrameID * tintColorBlueDifference) / frameDifference + cell->tintColor.blue();
					cell->tintColor = QColor(red, green, blue);
				}
				if (carryTintAmount)
					cell->tintAmount += (currentFrameID * tintAmountDifference) / frameDifference;
				cell->x += (currentFrameID * xDifference) / frameDifference;
				cell->y += (currentFrameID * yDifference) / frameDifference;
			}
		}
	}
	delete autoComplete;
	updateFrame();
}

void Database_GlobalAnimations::on_bMassModify_clicked()
{
	AnimationMassModify *massModify = new AnimationMassModify(lwFrameList->count(), (*animRef)->numImages(), this);
	if (massModify->exec())
	{
		QList<bool> affectedFrames = massModify->getAffectedFrames();
		QList<bool> affectedCells = massModify->getAffectedCells();
		GlobalAnimation::ImageLocation modifiedCell;
		bool modifyImageID = massModify->modifyImageID();
		bool modifyScale = massModify->modifyScale();
		bool modifyRotation = massModify->modifyRotation();
		bool modifyOpacity = massModify->modifyOpacity();
		bool modifyTintColor = massModify->modifyTintColor();
		bool modifyTintAmount = massModify->modifyTintAmount();
		modifiedCell.tintColor = massModify->getTintColor();
		modifiedCell.rotation = (float)massModify->getRotation();
		modifiedCell.scale = massModify->getScale() / 100.0;
		modifiedCell.opacity = massModify->getOpacity() / 100.0;
		modifiedCell.imageID = massModify->getImageID() - 1;
		modifiedCell.tintAmount = massModify->getTintAmount();
		for (int i = 0; i < affectedFrames.size(); ++i)
		{
			GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
			if (affectedFrames[i])
			{
				for (int j = 0; j < frame->imageList.size(); ++j)
				{
					GlobalAnimation::ImageLocation *cell = frame->imageList[j];
					if (affectedCells[j])
					{
						if (modifyTintColor)
							cell->tintColor = modifiedCell.tintColor;
						if (modifyRotation)
							cell->rotation = modifiedCell.rotation;
						if (modifyScale)
							cell->scale = modifiedCell.scale;
						if (modifyOpacity)
							cell->opacity = modifiedCell.opacity;
						if (modifyImageID)
							cell->imageID = modifiedCell.imageID;
						if (modifyTintAmount)
							cell->tintAmount = modifiedCell.tintAmount;
					}
				}
			}
		}
	}
	delete massModify;
	updateFrame();
}

void Database_GlobalAnimations::on_bPlay_clicked()
{
	QPixmap pixmap;
	previewWidget->setVisible(true);
	if (cbShowBGImage->isChecked())
		pixmap = animationBGItem->pixmap();
	else
	{
		pixmap = QPixmap(640, 480);
		pixmap.fill(Qt::black);
	}
	topWidget->setEnabled(false);
	topFrame->setEnabled(false);
	previewWidget->playAnimation(animRef->getDataID(), pixmap);
	while (!previewWidget->done)
		QCoreApplication::processEvents();
	topWidget->setEnabled(true);
	topFrame->setEnabled(true);
	previewWidget->setVisible(false);
}

void Database_GlobalAnimations::on_leName_textChanged(QString text)
{
	int currentRow = lwAnimationList->currentRow();
	ignoreEvents = true;
	lwAnimationList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 3, 10, QChar('0')).arg(text));
	ProjectData::globalAnimationNames[currentRow] = text;
	(*animRef)->setName(text);
	ignoreEvents = false;
}

void Database_GlobalAnimations::on_bBrowseBGImage_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		leBGImage->setText(QFileInfo(fileLocation).fileName());
		animationBGItem->setPixmap(QPixmap(fileLocation));
	}
	if (!leBGImage->text().isEmpty())
		cbShowBGImage->setEnabled(true);
	delete imageBrowser;
}

void Database_GlobalAnimations::on_cbShowBGImage_toggled(bool on)
{
	animationBGItem->setVisible(on);
}

void Database_GlobalAnimations::on_cbShowGrid_toggled(bool on)
{
	screenQuadrant1Rect1->setVisible(on);
	screenQuadrant1Rect2->setVisible(on);
	screenQuadrant2Rect1->setVisible(on);
	screenQuadrant2Rect2->setVisible(on);
	screenQuadrant3Rect1->setVisible(on);
	screenQuadrant3Rect2->setVisible(on);
	screenQuadrant4Rect1->setVisible(on);
	screenQuadrant4Rect2->setVisible(on);
}

void Database_GlobalAnimations::on_cbSnapToGrid_toggled(bool on)
{
	wSnapToGrid->setEnabled(on);
}

void Database_GlobalAnimations::on_bBack_clicked()
{
	int currentRow = lwFrameList->currentRow();
	if (currentRow > 0)
		--currentRow;
	lwFrameList->setCurrentRow(currentRow);
}

void Database_GlobalAnimations::on_bNext_clicked()
{
	int currentRow = lwFrameList->currentRow();
	if (currentRow < lwFrameList->count() - 1)
		++currentRow;
	lwFrameList->setCurrentRow(currentRow);
}

void Database_GlobalAnimations::on_bDeleteEffect_clicked()
{
	GlobalAnimation::AnimationFrame *frame;
	GlobalAnimation::AnimationEffect *effect;
	int row = twAdditionalEffects->currentRow();
	if (row >= 0 && row < effectIDs.size())
	{
		frame = (*animRef)->getFrame(twAdditionalEffects->item(row, 0)->text().toInt() - 1);
		effect = frame->animationEffects[effectIDs[row]];
		if (effect->soundEffect != NULL)
			delete (effect->soundEffect);
		delete effect;
		frame->animationEffects.removeAt(effectIDs[row]);
		effectIDs.removeAt(row);
		twAdditionalEffects->removeRow(row);
	}
}

void Database_GlobalAnimations::on_bAddEffect_clicked()
{
	ConfigureAnimationEffect *configEffect = new ConfigureAnimationEffect(ConfigureAnimationEffect::ANIM_GLOBAL, leNumFrames->text().toInt(), this);
	int frame;
	if (configEffect->exec())
	{
		GlobalAnimation::AnimationEffect *effect = new GlobalAnimation::AnimationEffect;
		configEffect->getAnimationEffect(effect, &frame);
		(*animRef)->getFrame(frame - 1)->animationEffects.append(effect);
		updateEffectList();
	}
	delete configEffect;
}

void Database_GlobalAnimations::on_bConfigureEffect_clicked()
{
	ConfigureAnimationEffect *configEffect = new ConfigureAnimationEffect(ConfigureAnimationEffect::ANIM_GLOBAL, leNumFrames->text().toInt(), this);
	GlobalAnimation::AnimationFrame *frame;
	GlobalAnimation::AnimationEffect *effect;
	int row = twAdditionalEffects->currentRow();
	int frameID = twAdditionalEffects->item(row, 0)->text().toInt();
	int newFrameID;
	frame = (*animRef)->getFrame(frameID - 1);
	effect = frame->animationEffects[effectIDs[row]];
	configEffect->setAnimationEffect(effect, frameID);
	if (configEffect->exec())
	{
		configEffect->getAnimationEffect(effect, &newFrameID);
		if (newFrameID != frameID)
		{
			frame->animationEffects.removeAt(effectIDs[row]);
			frame = (*animRef)->getFrame(newFrameID - 1);
			frame->animationEffects.append(effect);
		}
		updateEffectList();
	}
	delete configEffect;
}

void Database_GlobalAnimations::on_cboxZoom_currentIndexChanged(int index)
{
	gvAnimation->resetMatrix();
	currentViewScale = 1.0;
	if (index == ZOOM_25)
	{
		gvAnimation->scale(.25, .25);
		currentViewScale = .25;
	}
	else if (index == ZOOM_50)
	{
		gvAnimation->scale(.5, .5);
		currentViewScale = .5;
	}
	else if (index == ZOOM_75)
	{
		gvAnimation->scale(.75, .75);
		currentViewScale = .75;
	}
	else if (index == ZOOM_FITTOWINDOW)
	{
		float xScale = (float)gvAnimation->width() / gvAnimation->sceneRect().width();
		float yScale = (float)gvAnimation->height() / gvAnimation->sceneRect().height();
		if (xScale > yScale)
		{
			gvAnimation->scale(yScale, yScale);
			currentViewScale = yScale;
		}
		else
		{
			gvAnimation->scale(xScale, xScale);
			currentViewScale = xScale;
		}
	}
}

void Database_GlobalAnimations::on_twAdditionalEffects_itemSelectionChanged()
{
	int row = twAdditionalEffects->currentRow();
	if (row >= 0 && row < effectIDs.size())
	{
		bDeleteEffect->setEnabled(true);
		bConfigureEffect->setEnabled(true);
	}
	else
	{
		bDeleteEffect->setEnabled(false);
		bConfigureEffect->setEnabled(false);
	}
}

void Database_GlobalAnimations::on_aCellProperties_triggered()
{
	if (gvAnimation->hasFocus())
	{
		GlobalAnimation::ImageLocation *image = currentFrame->imageList[currentAnimationImage];
		AnimationCellProperties *cellProperties = new AnimationCellProperties((*animRef)->getImageLocations(), image->imageID, (int)(image->scale * 100), (int)(image->rotation), int(image->opacity * 100), image->tintColor, image->tintAmount);
		if (cellProperties->exec())
		{
			image->tintColor = cellProperties->getTintColor();
			image->rotation = (float)cellProperties->getRotation();
			image->scale = cellProperties->getScale() / 100.0;
			image->opacity = cellProperties->getOpacity() / 100.0;
			image->imageID = cellProperties->getImageID();
			image->tintAmount = cellProperties->getTintAmount();
			updateFrame();
		}
		delete cellProperties;
	}
}

void Database_GlobalAnimations::on_aCopy_triggered()
{
	if (gvAnimation->hasFocus() && numImagesInFrame > 0)
	{
		GlobalAnimation::ImageLocation *image = currentFrame->imageList[currentAnimationImage];
		GlobalAnimation::ImageLocation *copy = new GlobalAnimation::ImageLocation;
		Clipboard::clear();
		copy->tintColor = image->tintColor;
		copy->rotation = image->rotation;
		copy->scale = image->scale;
		copy->opacity = image->opacity;
		copy->imageID = image->imageID;
		copy->tintAmount = image->tintAmount;
		copy->x = image->x;
		copy->y = image->y;
		Clipboard::dataType = Clipboard::TYPE_GLOBALANIMATION_IMAGELOCATION;
		Clipboard::data.globalAnimation_imageLocation = copy;
	}
}

void Database_GlobalAnimations::on_aCut_triggered()
{
	if (gvAnimation->hasFocus() && numImagesInFrame > 0)
	{
		GlobalAnimation::ImageLocation *image = currentFrame->imageList[currentAnimationImage];
		GlobalAnimation::ImageLocation *copy = new GlobalAnimation::ImageLocation;
		Clipboard::clear();
		copy->tintColor = image->tintColor;
		copy->rotation = image->rotation;
		copy->scale = image->scale;
		copy->opacity = image->opacity;
		copy->imageID = image->imageID;
		copy->tintAmount = image->tintAmount;
		copy->x = image->x;
		copy->y = image->y;
		Clipboard::dataType = Clipboard::TYPE_GLOBALANIMATION_IMAGELOCATION;
		Clipboard::data.globalAnimation_imageLocation = copy;
		delete (currentFrame->imageList[currentAnimationImage]);
		currentFrame->imageList.removeAt(currentAnimationImage);
		--numImagesInFrame;
		if (currentAnimationImage == numImagesInFrame)
			--currentAnimationImage;
		updateFrame();
	}
}

void Database_GlobalAnimations::on_aPaste_triggered()
{
	if (gvAnimation->hasFocus() && Clipboard::dataType == Clipboard::TYPE_GLOBALANIMATION_IMAGELOCATION && numImagesInFrame < 16)
	{
		if (contextMenuScenePos.x() >= 0)
		{
			GlobalAnimation::ImageLocation *image = Clipboard::data.globalAnimation_imageLocation;
			GlobalAnimation::ImageLocation *copy = new GlobalAnimation::ImageLocation;
			QPoint pastePoint = contextMenuScenePos + (QPoint(96, 96) * image->scale);
			copy->tintColor = image->tintColor;
			copy->rotation = image->rotation;
			copy->scale = image->scale;
			copy->opacity = image->opacity;
			copy->imageID = image->imageID;
			copy->tintAmount = image->tintAmount;
			copy->x = pastePoint.x();
			copy->y = pastePoint.y();
			currentAnimationImage = currentFrame->imageList.size();
			currentFrame->imageList.append(copy);
			updateFrame();
		}
		else
		{
			GlobalAnimation::ImageLocation *image = Clipboard::data.globalAnimation_imageLocation;
			GlobalAnimation::ImageLocation *copy = new GlobalAnimation::ImageLocation;
			copy->tintColor = image->tintColor;
			copy->rotation = image->rotation;
			copy->scale = image->scale;
			copy->opacity = image->opacity;
			copy->imageID = image->imageID;
			copy->tintAmount = image->tintAmount;
			copy->x = image->x;
			copy->y = image->y;
			currentAnimationImage = currentFrame->imageList.size();
			currentFrame->imageList.append(copy);
			updateFrame();
		}
	}
}

void Database_GlobalAnimations::on_aDelete_triggered()
{
	if (gvAnimation->hasFocus() && numImagesInFrame > 0)
	{
		delete (currentFrame->imageList[currentAnimationImage]);
		currentFrame->imageList.removeAt(currentAnimationImage);
		--numImagesInFrame;
		if (currentAnimationImage == numImagesInFrame)
			--currentAnimationImage;
		updateFrame();
	}
}

void Database_GlobalAnimations::on_aMoveBack_triggered()
{
	if (gvAnimation->hasFocus() && currentAnimationImage > 0)
	{
		GlobalAnimation::ImageLocation *image = currentFrame->imageList.takeAt(currentAnimationImage);
		--currentAnimationImage;
		currentFrame->imageList.insert(currentAnimationImage, image);
		updateFrame();
	}
}

void Database_GlobalAnimations::on_aMoveForward_triggered()
{
	if (gvAnimation->hasFocus() && currentAnimationImage < numImagesInFrame - 1)
	{
		GlobalAnimation::ImageLocation *image = currentFrame->imageList.takeAt(currentAnimationImage);
		++currentAnimationImage;
		currentFrame->imageList.insert(currentAnimationImage, image);
		updateFrame();
	}
}

void Database_GlobalAnimations::on_aMoveToBack_triggered()
{
	if (gvAnimation->hasFocus())
	{
		GlobalAnimation::ImageLocation *image = currentFrame->imageList.takeAt(currentAnimationImage);
		currentAnimationImage = 0;
		currentFrame->imageList.prepend(image);
		updateFrame();
	}
}

void Database_GlobalAnimations::on_aMoveToFront_triggered()
{
	if (gvAnimation->hasFocus())
	{
		GlobalAnimation::ImageLocation *image = currentFrame->imageList.takeAt(currentAnimationImage);
		currentAnimationImage = currentFrame->imageList.size();
		currentFrame->imageList.append(image);
		updateFrame();
	}
}

void Database_GlobalAnimations::on_sbFrameRate_valueChanged(int value)
{
	if (animRef != NULL && !ignoreEvents)
		(*animRef)->setFrameRate(value);
}

void Database_GlobalAnimations::updateAnimationList()
{
	int currentRow = lwAnimationList->currentRow();
	lwAnimationList->clear();
	for (int i = 0; i < ProjectData::globalAnimationNames.size(); ++i)
		lwAnimationList->addItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::globalAnimationNames[i]));
	if (currentRow >= ProjectData::globalAnimationNames.size())
		currentRow = ProjectData::globalAnimationNames.size();
	else if (currentRow < 0)
		currentRow = 0;
	lwAnimationList->setCurrentRow(currentRow);
}

void Database_GlobalAnimations::updateEffectList()
{
	QTableWidgetItem *item;
	int row;
	twAdditionalEffects->clearContents();
	twAdditionalEffects->setRowCount(0);
	effectIDs.clear();
	for (int i = 0; i < (*animRef)->numFrames(); ++i)
	{
		GlobalAnimation::AnimationFrame *frame = (*animRef)->getFrame(i);
		for (int j = 0; j < frame->animationEffects.size(); ++j)
		{
			GlobalAnimation::AnimationEffect *effect = frame->animationEffects[j];
			QString effectText = "";
			if (effect->soundEffect != NULL)
				effectText += QString("Play Sound: %1; ").arg(QFileInfo(effect->soundEffect->location).fileName());
			if (effect->flashScope == GlobalAnimation::SCOPE_SCREEN)
				effectText += QString("Flash Screen: rgba(%1, %2, %3, %4); ").arg(effect->flashColor.red()).arg(effect->flashColor.green()).arg(effect->flashColor.blue()).arg(effect->flashOpacity);
			if (effect->shakeScope == GlobalAnimation::SCOPE_SCREEN)
				effectText += QString("Shake Screen: %1; ").arg(effect->shakeMagnitude);
			row = twAdditionalEffects->rowCount();
			twAdditionalEffects->insertRow(row);
			item = new QTableWidgetItem;
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			item->setText(QString::number(i + 1));
			twAdditionalEffects->setItem(row, 0, item);
			item = new QTableWidgetItem;
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			item->setText(effectText);
			twAdditionalEffects->setItem(row, 1, item);
			effectIDs.append(j);
			twAdditionalEffects->setRowHeight(row, 15);
		}
	}
}

void Database_GlobalAnimations::updateFrame()
{
	int currentFrameID = lwFrameList->currentRow();
	numImagesInFrame = currentFrame->imageList.size();
	for (int i = 0; i < 16; ++i)
	{
		animationImageItems[i]->setVisible(false);
		animationImageTintEffects[i]->setColor(0, 0, 0, 0);
		animationImageItemFrames[i]->setVisible(false);
		animationImageItemFrameNumberBackground[i]->setVisible(false);
		animationImageItemFrameNumber[i]->setVisible(false);
		animationPreviousFrames[i]->setVisible(false);
		animationPreviousFrameNumberBackground[i]->setVisible(false);
		animationPreviousFrameNumber[i]->setVisible(false);
	}
	for (int i = 0; i < numImagesInFrame; ++i)
	{
		GlobalAnimation::ImageLocation *image = currentFrame->imageList[i];
		if (i == currentAnimationImage)
		{
			animationImageItemFrames[i]->setPen(animationFramePen_selected);
			animationImageItemFrameNumberBackground[i]->setPen(animationFramePen_selected);
			animationImageItemFrameNumberBackground[i]->setBrush(animationFrameBrush_selected);
		}
		else
		{
			animationImageItemFrames[i]->setPen(animationFramePen);
			animationImageItemFrameNumberBackground[i]->setPen(animationFramePen);
			animationImageItemFrameNumberBackground[i]->setBrush(animationFrameBrush);
		}
		animationImageTintEffects[i]->setColor(image->tintColor.red(), image->tintColor.green(), image->tintColor.blue(), image->tintAmount);
		animationImageItemFrames[i]->setRect((int)(image->scale * -96.0), (int)(image->scale * -96.0), (int)(image->scale * 192.0), (int)(image->scale * 192.0));
		animationImageItemFrames[i]->setPos(image->x + 192, image->y + 192);
		animationImageItemFrames[i]->setOpacity(image->opacity);
		animationImageItemFrames[i]->setRotation(image->rotation);
		animationImageItemFrames[i]->setVisible(true);
		animationImageItems[i]->setPixmap(QPixmap((*animRef)->getImageLocation(image->imageID)));
		animationImageItems[i]->setPos((int)(image->scale * -96.0), (int)(image->scale * -96.0));
		animationImageItems[i]->setOpacity(image->opacity);
		animationImageItems[i]->setScale(image->scale);
		animationImageItems[i]->setVisible(true);
		animationImageItemFrameNumberBackground[i]->setPos((int)(image->scale * -96.0), (int)(image->scale * -96.0));
		animationImageItemFrameNumberBackground[i]->setVisible(true);
		animationImageItemFrameNumber[i]->setVisible(true);
	}
	if (currentFrameID != 0)
	{
		GlobalAnimation::AnimationFrame *previousFrame = (*animRef)->getFrame(currentFrameID - 1);
		for (int i = 0; i < previousFrame->imageList.size(); ++i)
		{
			GlobalAnimation::ImageLocation *image = previousFrame->imageList[i];
			animationPreviousFrames[i]->setRect((int)(image->scale * -96.0), (int)(image->scale * -96.0), (int)(image->scale * 192.0), (int)(image->scale * 192.0));
			animationPreviousFrames[i]->setPos(image->x + 192, image->y + 192);
			animationPreviousFrames[i]->setOpacity(image->opacity);
			animationPreviousFrames[i]->setRotation(image->rotation);
			animationPreviousFrames[i]->setVisible(true);
			animationPreviousFrameNumberBackground[i]->setPos((int)(image->scale * -96.0), (int)(image->scale * -96.0));
			animationPreviousFrameNumberBackground[i]->setVisible(true);
			animationPreviousFrameNumber[i]->setVisible(true);
		}
	}
}

int Database_GlobalAnimations::getItemUnderCursor(QPoint scenePos, bool cycleSelection)
{
	QList<int> imagesUnderCursor;
	for (int i = 0; i < numImagesInFrame; ++i)
	{
		if (animationImageItemFrames[i]->contains(animationImageItemFrames[i]->mapFromScene(scenePos).toPoint()))
			imagesUnderCursor.append(i);
		else if (animationImageItemFrameNumberBackground[i]->contains(animationImageItemFrameNumberBackground[i]->mapFromScene(scenePos).toPoint()))
			imagesUnderCursor.append(i);
	}
	if (!imagesUnderCursor.isEmpty())
	{
		if (imagesUnderCursor.contains(currentAnimationImage))
		{
			if (cycleSelection)
			{
				int id = imagesUnderCursor.indexOf(currentAnimationImage) - 1;
				if (id == -1)
					id += imagesUnderCursor.size();
				return imagesUnderCursor[id];
			}
			else
				return currentAnimationImage;
		}
		else
			return imagesUnderCursor.last();
	}
	return -1;
}
