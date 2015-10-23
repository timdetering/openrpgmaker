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
#include <QStringList>
#include <QMessageBox>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSize>
#include <QSizeF>
#include <QMouseEvent>
#include <QScrollBar>
#include <QApplication>
#include "menunavigationcreator.h"
#include "projectdata.h"
#include "projectdata_font.h"
#include "projectdata_menunavigation.h"

MenuNavigationCreator::MenuNavigationCreator(QStringList objects, MenuNavigation *nav, QWidget *parent) : QDialog(parent)
{
	QImage bgImage = QImage(":/images/menuNavBorder.png");
	QPainter painter;
	QStringList objectNames;
	setupUi(this);
	imageWidth = 153;
	lwMenuObjects->addMenuObjects(objects);
	navigation = nav;
	font = new Font();
	font->setData(":/images/font.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~!@#$%^&*()_-+=[]{};':\",.<>/?\\|", 16, 16, 8, 24, 2, 2);
	lwMenuObjects->setFont(font);
	objectNames = navigation->getObjectNames();
	for (int i = 0; i < objectNames.size(); ++i)
		lwMenuObjects->disableMenuObject(objectNames[i]);
	for (int i = 0; i < objects.size(); ++i)
	{
		int width = font->getTextWidth(objects[i]);
		if (width > imageWidth)
			imageWidth = width;
	}
	imageWidth += 8;
	scene = new QGraphicsScene(0, 0, 0, 0);
	gvLayout->setScene(scene);
	gvLayout->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	bgPixmap = QPixmap(imageWidth, 24);
	bgPixmapMouseOver = QPixmap(imageWidth, 24);
	painter.begin(&bgPixmap);
	for (int x = 4; x < imageWidth - 4; x += 16)
	{
		painter.drawImage(x, 0, bgImage, 4, 0, 16, 4);
		painter.drawImage(x, 4, bgImage, 4, 4, 16, 16);
		painter.drawImage(x, 20, bgImage, 4, 20, 16, 4);
	}
	painter.drawImage(0, 4, bgImage, 0, 4, 4, 16);
	painter.drawImage(imageWidth - 4, 4, bgImage, 20, 4, 4, 16);
	painter.drawImage(0, 0, bgImage, 0, 0, 4, 4);
	painter.drawImage(imageWidth - 4, 0, bgImage, 20, 0, 4, 4);
	painter.drawImage(0, 20, bgImage, 0, 20, 4, 4);
	painter.drawImage(imageWidth - 4, 20, bgImage, 20, 20, 4, 4);
	painter.end();
	painter.begin(&bgPixmapMouseOver);
	for (int x = 4; x < imageWidth - 4; x += 16)
	{
		painter.drawImage(x, 0, bgImage, 4, 24, 16, 4);
		painter.drawImage(x, 4, bgImage, 4, 28, 16, 16);
		painter.drawImage(x, 20, bgImage, 4, 44, 16, 4);
	}
	painter.drawImage(0, 4, bgImage, 0, 28, 4, 16);
	painter.drawImage(imageWidth - 4, 4, bgImage, 20, 28, 4, 16);
	painter.drawImage(0, 0, bgImage, 0, 24, 4, 4);
	painter.drawImage(imageWidth - 4, 0, bgImage, 20, 24, 4, 4);
	painter.drawImage(0, 20, bgImage, 0, 44, 4, 4);
	painter.drawImage(imageWidth - 4, 20, bgImage, 20, 44, 4, 4);
	painter.end();
	menuNavX = QPixmap(":/images/menuNavX.png");
	emptyText = QPixmap(":/images/menuNavDropText.png");
	cbWrapHorizontally->setChecked(navigation->wrapsHorizontally());
	cbWrapVertically->setChecked(navigation->wrapsVertically());
	updateNavigationLayout();
}

MenuNavigationCreator::~MenuNavigationCreator()
{
	delete font;
}

void MenuNavigationCreator::on_gvLayout_dragEnterEvent(QDragEnterEvent *event)
{
	if (event->source() == lwMenuObjects)
		event->acceptProposedAction();
}

void MenuNavigationCreator::on_gvLayout_dragMoveEvent(QDragMoveEvent *event)
{
	QRectF rect;
	QPointF point = gvLayout->mapToScene(event->pos());
	for (int i = 0; i < emptyBGItems.size(); ++i)
	{
		rect = QRectF(emptyBGItems[i]->pos(), QSizeF(imageWidth, 24));
		if (rect.contains(point))
			emptyBGItems[i]->setPixmap(bgPixmapMouseOver);
		else
			emptyBGItems[i]->setPixmap(bgPixmap);
	}
}

void MenuNavigationCreator::on_gvLayout_dropEvent(QDropEvent *event)
{
	QRectF rect;
	QPointF point = gvLayout->mapToScene(event->pos());
	bool foundOne = false;
	for (int i = 0; i < emptyBGItems.size(); ++i)
	{
		rect = QRectF(emptyBGItems[i]->pos(), QSizeF(imageWidth, 24));
		if (rect.contains(point))
		{
			QString objectName = event->mimeData()->text();
			int minRow = navigation->getMinRow() - 1;
			int minColumn = navigation->getMinColumn() - 1;
			int numRows = navigation->getNumRows();
			int numColumns = navigation->getNumColumns();
			int row = (int)(emptyBGItems[i]->pos().y()) / 24 + minRow;
			int column = (int)(emptyBGItems[i]->pos().x()) / imageWidth + minColumn;
			if (numRows == 0 || numColumns == 0)
			{
				row = 0;
				column = 0;
			}
			foundOne = true;
			navigation->addObject(objectName, QPoint(column, row));
			ProjectData::saved = false;
		}
	}
	if (foundOne)
	{
		updateNavigationLayout();
		event->acceptProposedAction();
	}
	else
		event->ignore();
}

void MenuNavigationCreator::on_gvLayout_mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		lastPos = event->pos();
}

void MenuNavigationCreator::on_gvLayout_mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && !dragging)
	{
		QRectF rect;
		QPointF point = gvLayout->mapToScene(event->pos());
		bool foundOne = false;
		for (int i = 0; i < xButtons.size() && !foundOne; ++i)
		{
			rect = QRectF(xButtons[i]->pos(), QSizeF(12, 12));
			if (rect.contains(point))
			{
				foundOne = true;
				lwMenuObjects->enableMenuObject(navigation->objectNameAtLocation(xButtonLocations[i]));
				navigation->removeObject(xButtonLocations[i]);
				ProjectData::saved = false;
			}
		}
		if (foundOne)
			updateNavigationLayout();
	}
	dragging = false;
}

void MenuNavigationCreator::on_gvLayout_mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) != 0)
	{
		QScrollBar *hScrollBar = gvLayout->horizontalScrollBar();
		QScrollBar *vScrollBar = gvLayout->verticalScrollBar();
		int dx = lastPos.x() - event->pos().x();
		int dy = lastPos.y() - event->pos().y();
		dragging = true;
		hScrollBar->setValue(hScrollBar->value() + dx);
		vScrollBar->setValue(vScrollBar->value() + dy);
		lastPos = event->pos();
	}
}

void MenuNavigationCreator::on_buttonBox_clicked(QAbstractButton *button)
{
	int buttonClicked = buttonBox->standardButton(button);
	if (buttonClicked == QDialogButtonBox::Close)
	{
		navigation->setWrapHorizontally(cbWrapHorizontally->isChecked());
		navigation->setWrapVertically(cbWrapVertically->isChecked());
		accept();
	}
	else if (buttonClicked == QDialogButtonBox::Apply)
	{
		int confirm = QMessageBox::question(this, "Confirm Overwrite", "Applying the navigation will overwrite certain events.\nDo you wish to continue?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
		if (confirm == QMessageBox::Yes)
		{
			navigation->setWrapHorizontally(cbWrapHorizontally->isChecked());
			navigation->setWrapVertically(cbWrapVertically->isChecked());
			ProjectData::saved = false;
			emit applyMenuNavigation();
		}
	}
	else if (buttonClicked == QDialogButtonBox::Help)
	{
		ProjectData::helpManual->loadIndex("MenuNavigationCreator");
		if (!ProjectData::helpManual->isVisible())
			ProjectData::helpManual->show();
		else
			ProjectData::helpManual->raise();
		ProjectData::helpManual->activateWindow();
	}
}

void MenuNavigationCreator::updateNavigationLayout()
{
	QGraphicsPixmapItem *pixmapItem;
	QPoint loc;
	QString objectName;
	int minRow = navigation->getMinRow();
	int minColumn = navigation->getMinColumn();
	int numRows = navigation->getNumRows();
	int numColumns = navigation->getNumColumns();
	scene->clear();
	objectBGItems.clear();
	objectTextItems.clear();
	emptyBGItems.clear();
	emptyTextItems.clear();
	xButtons.clear();
	xButtonLocations.clear();
	if (numRows == 0 || numColumns == 0)
	{
		scene->setSceneRect(0, 0, (numColumns + 1) * imageWidth, (numRows + 1) * 24);
		pixmapItem = new QGraphicsPixmapItem(bgPixmap);
		pixmapItem->setPos(0, 0);
		pixmapItem->setZValue(1);
		scene->addItem(pixmapItem);
		emptyBGItems.append(pixmapItem);
		pixmapItem = new QGraphicsPixmapItem(emptyText);
		pixmapItem->setPos((imageWidth - emptyText.width()) / 2, 4);
		pixmapItem->setZValue(2);
		scene->addItem(pixmapItem);
		emptyTextItems.append(pixmapItem);
	}
	else
	{
		minRow -= 1;
		minColumn -= 1;
		numRows += 2;
		numColumns += 2;
		scene->setSceneRect(0, 0, numColumns * imageWidth, numRows * 24);
		for (int row = 0; row < numRows; ++row)
		{
			for (int col = 0; col < numColumns; ++col)
			{
				QString objectName = navigation->objectNameAtLocation(QPoint(col + minColumn, row + minRow));
				bool hasNeighbor = navigation->hasNeighborAtLocation(QPoint(col + minColumn, row + minRow));
				if (!objectName.isEmpty())
				{
					QPixmap textPixmap = QPixmap::fromImage(font->getRenderedText(objectName, QColor(126, 126, 126), QColor(6, 6, 6), false));
					textPixmap = textPixmap.copy(0, 0, textPixmap.width(), 16);
					textPixmap.setMask(textPixmap.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
					pixmapItem = new QGraphicsPixmapItem(bgPixmap);
					pixmapItem->setPos(col * imageWidth, row * 24);
					pixmapItem->setZValue(1);
					scene->addItem(pixmapItem);
					objectBGItems.append(pixmapItem);
					pixmapItem = new QGraphicsPixmapItem(textPixmap);
					pixmapItem->setPos(col * imageWidth + ((imageWidth - textPixmap.width()) / 2), row * 24 + 4);
					pixmapItem->setZValue(2);
					scene->addItem(pixmapItem);
					objectTextItems.append(pixmapItem);
					if (navigation->canObjectBeRemoved(QPoint(col + minColumn, row + minRow)))
					{
						pixmapItem = new QGraphicsPixmapItem(menuNavX);
						pixmapItem->setPos((col + 1) * imageWidth - 12, row * 24);
						pixmapItem->setZValue(3);
						scene->addItem(pixmapItem);
						xButtons.append(pixmapItem);
						xButtonLocations.append(QPoint(col + minColumn, row + minRow));
					}
				}
				else if (hasNeighbor)
				{
					pixmapItem = new QGraphicsPixmapItem(bgPixmap);
					pixmapItem->setPos(col * imageWidth, row * 24);
					pixmapItem->setZValue(1);
					scene->addItem(pixmapItem);
					emptyBGItems.append(pixmapItem);
					pixmapItem = new QGraphicsPixmapItem(emptyText);
					pixmapItem->setPos(col * imageWidth + ((imageWidth - emptyText.width()) / 2), row * 24 + 4);
					pixmapItem->setZValue(2);
					scene->addItem(pixmapItem);
					emptyTextItems.append(pixmapItem);
				}
			}
		}
	}
}
