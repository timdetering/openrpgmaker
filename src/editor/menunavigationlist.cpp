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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QListWidget>
#include <QPoint>
#include <QPixmap>
#include <QMouseEvent>
#include <QApplication>
#include <QBitmap>
#include "menunavigationlist.h"
#include "projectdata_font.h"

MenuNavigationList::MenuNavigationList(QWidget *parent) : QListWidget(parent)
{
	font = NULL;
	dragStartPos = QPoint(-1, -1);
}

MenuNavigationList::~MenuNavigationList()
{
}

void MenuNavigationList::setFont(Font *f)
{
	font = f;
}

void MenuNavigationList::addMenuObjects(QStringList objectNames)
{
	QListWidgetItem *lwItem;
	objectNames.sort();
	for (int i = 0; i < objectNames.size(); ++i)
	{
		lwItem = new QListWidgetItem(objectNames[i]);
		lwItem->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsEnabled);
		addItem(lwItem);
	}
}

void MenuNavigationList::enableMenuObject(QString name)
{
	bool foundOne = false;
	for (int i = 0; i < count() && !foundOne; ++i)
	{
		if (item(i)->text() == name)
		{
			item(i)->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsEnabled);
			foundOne = true;
		}
	}
}

void MenuNavigationList::disableMenuObject(QString name)
{
	bool foundOne = false;
	for (int i = 0; i < count() && !foundOne; ++i)
	{
		if (item(i)->text() == name)
		{
			item(i)->setFlags(Qt::NoItemFlags);
			foundOne = true;
		}
	}
}

void MenuNavigationList::mousePressEvent(QMouseEvent *event)
{
	QListWidget::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
		dragStartPos = event->pos();
}

void MenuNavigationList::mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) != 0 && dragStartPos.x() >= 0 && (event->pos() - dragStartPos).manhattanLength() >= QApplication::startDragDistance() && currentItem() != NULL)
	{
		if ((currentItem()->flags() & Qt::ItemIsEnabled) != 0)
		{
			QDrag *drag = new QDrag(this);
			QMimeData *mimeData = new QMimeData;
			QPixmap pixmap = QPixmap::fromImage(font->getRenderedText(currentItem()->text(), QColor(126, 126, 126), QColor(6, 6, 6), false));
			Qt::DropAction dropAction;
			currentItem()->setFlags(Qt::NoItemFlags);
			pixmap.setMask(pixmap.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
			mimeData->setText(currentItem()->text());
			drag->setMimeData(mimeData);
			drag->setPixmap(pixmap);
			drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 4));
			dragStartPos = QPoint(-1, -1);
			dropAction = drag->exec(Qt::CopyAction);
			if (dropAction == Qt::IgnoreAction)
				currentItem()->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsEnabled);
		}
	}
}
