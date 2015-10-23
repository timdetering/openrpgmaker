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

#include <QEvent>
#include <QScrollBar>
#include "imageview.h"

ImageView::ImageView(QWidget * parent) : QGraphicsView(parent)
{
	if (isEnabled())
		setForegroundBrush(QBrush(Qt::NoBrush));
	else
		setForegroundBrush(QBrush(QColor(0, 0, 0, 128)));
	QObject::connect(horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SIGNAL(hScrollBarRangeChanged(int, int)));
	QObject::connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(hScrollBarValueChanged(int)));
	QObject::connect(verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SIGNAL(vScrollBarRangeChanged(int, int)));
	QObject::connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(vScrollBarValueChanged(int)));
}

ImageView::ImageView(QGraphicsScene *scene, QWidget * parent) : QGraphicsView(scene, parent)
{
	if (isEnabled())
		setForegroundBrush(QBrush(Qt::NoBrush));
	else
		setForegroundBrush(QBrush(QColor(0, 0, 0, 128)));
}

void ImageView::setHScrollBarRange(int min, int max)
{
	if (min != horizontalScrollBar()->minimum() || max != horizontalScrollBar()->maximum())
		horizontalScrollBar()->setRange(min, max);
}

void ImageView::setHScrollBarValue(int value)
{
	if (value != horizontalScrollBar()->value())
		horizontalScrollBar()->setValue(value);
}

void ImageView::setVScrollBarRange(int min, int max)
{
	if (min != verticalScrollBar()->minimum() || max != verticalScrollBar()->maximum())
		verticalScrollBar()->setRange(min, max);
}

void ImageView::setVScrollBarValue(int value)
{
	if (value != verticalScrollBar()->value())
		verticalScrollBar()->setValue(value);
}

void ImageView::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::EnabledChange)
	{
		if (isEnabled())
			setForegroundBrush(QBrush(Qt::NoBrush));
		else
			setForegroundBrush(QBrush(QColor(0, 0, 0, 128)));
	}
}
