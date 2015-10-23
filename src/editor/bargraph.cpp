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

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QString>
#include <QColor>
#include <QImage>
#include <QList>
#include <QMouseEvent>
#include <QResizeEvent>
#include <cstdio>
#include "bargraph.h"

BarGraph::BarGraph(QWidget *parent) : QGraphicsView(parent)
{
	backgroundColor = QColor(Qt::black);
	foregroundColor = QColor(Qt::red);
	textColor = QColor(Qt::white);
	setScene(new QGraphicsScene(0.0, 0.0, (float)width(), (float)height()));
	maxValue = 99;
	displayValueID = 0;
	values.append(0);
	modifiable = false;
	ignoreEvents = true;
}

BarGraph::~BarGraph()
{
}

void BarGraph::setupGraph()
{
	setSceneRect(0.0, 0.0, (float)width(), (float)height());
	graph = QImage(width(), height(), QImage::Format_RGB32);
	graph.fill(backgroundColor.rgb());
	graphItem = new QGraphicsPixmapItem(QPixmap::fromImage(graph));
	graphItem->setPos(0.0, 0.0);
	graphItem->setZValue(0.0);
	graphItem->setFlags(0x0);
	graphItem->setVisible(true);
	valueDisplayItem = new QGraphicsTextItem("Now: 0");
	valueDisplayItem->setPos(0.0, 0.0);
	valueDisplayItem->setZValue(1.0);
	valueDisplayItem->setFlags(0x0);
	valueDisplayItem->setVisible(true);
	valueDisplayItem->setDefaultTextColor(textColor);
	scene()->addItem(graphItem);
	scene()->addItem(valueDisplayItem);
	ignoreEvents = false;
	updateView();
}

void BarGraph::setBackgroundColor(QColor color)
{
	backgroundColor = color;
}

void BarGraph::setForegroundColor(QColor color)
{
	foregroundColor = color;
}

void BarGraph::setTextColor(QColor color)
{
	textColor = color;
	valueDisplayItem->setDefaultTextColor(textColor);
}

void BarGraph::updateView()
{
	QPainter painter;
	int numValues = values.size();
	int graphWidth = graph.width();
	int graphHeight = graph.height();
	int barWidth, barHeight;
	int x, y;
	painter.begin(&graph);
	painter.fillRect(0, 0, graphWidth, graphHeight, backgroundColor);
	if (modifiable)
	{
		barWidth = graphWidth / numValues;
		for (int i = 0; i < numValues; ++i)
		{
			if (values[i] == -1)
			{
				barHeight = graphHeight;
				x = i * barWidth;
				y = 0;
				painter.fillRect(x, y, barWidth - 1, barHeight, QColor(255 - foregroundColor.red(), 255 - foregroundColor.green(), 255 - foregroundColor.blue()));
			}
			else
			{
				barHeight = (values[i] * graphHeight) / maxValue;
				x = i * barWidth;
				y = graphHeight - barHeight;
				painter.fillRect(x, y, barWidth - 1, barHeight, foregroundColor);
			}
		}
	}
	else
	{
		for (int i = 0; i < graphWidth; ++i)
		{
			int valueID = (i * numValues) / graphWidth;
			if (values[valueID] == -1)
			{
				barWidth = 1;
				barHeight = graphHeight;
				x = i;
				y = 0;
				painter.fillRect(x, y, barWidth, barHeight, QColor(255 - foregroundColor.red(), 255 - foregroundColor.green(), 255 - foregroundColor.blue()));
			}
			else
			{
				barWidth = 1;
				barHeight = (int)(((float)(values[valueID]) / (float)maxValue) * (float)graphHeight);
				x = i;
				y = graphHeight - barHeight;
				painter.fillRect(x, y, barWidth, barHeight, foregroundColor);
			}
		}
	}
	painter.end();
	graphItem->setPixmap(QPixmap::fromImage(graph));
}

void BarGraph::setNumValues(int num)
{
	values.clear();
	for (int i = 0; i < num; ++i)
		values.append(0);
}

int BarGraph::numValues()
{
	return values.size();
}

void BarGraph::changeValue(int valueID, int newValue)
{
	if (newValue > maxValue)
	{
		values[valueID] = -1;
		if (valueID == displayValueID)
			valueDisplayItem->setPlainText("Now: OVERFLOW");
	}
	else
	{
		values[valueID] = newValue;
		if (valueID == displayValueID)
			valueDisplayItem->setPlainText(QString("Now: %1").arg(values[displayValueID]));
	}
}

int BarGraph::getValue(int valueID)
{
	return values[valueID];
}

void BarGraph::setMaxValue(int value)
{
	maxValue = value;
}

void BarGraph::setDisplayValue(int valueID)
{
	displayValueID = valueID;
	if (displayValueID != -1)
	{
		valueDisplayItem->setVisible(true);
		if (values[displayValueID] == -1)
			valueDisplayItem->setPlainText("Now: OVERFLOW");
		else
			valueDisplayItem->setPlainText(QString("Now: %1").arg(values[displayValueID]));
	}
	else
		valueDisplayItem->setVisible(false);
}

void BarGraph::setModifiable(bool on)
{
	modifiable = on;
}

void BarGraph::mouseMoveEvent(QMouseEvent *event)
{
	int x = event->x();
	int y = event->y();
	if (y < 0)
		y = 0;
	if (y >= graph.height())
		y = graph.height() - 1;
	if ((event->buttons() & Qt::LeftButton) != 0 && modifiable && x >= 0 && x < graph.width())
	{
		int barWidth = graph.width() / values.size();
		int valueID = x / barWidth;
		int levelValue = ((graph.height() - y) * maxValue) / graph.height();
		values[valueID] = levelValue;
		valueChanged(valueID + 1, levelValue);
		updateView();
	}
	mouseMoved(event);
}

void BarGraph::mousePressEvent(QMouseEvent *event)
{
	int x = event->x();
	int y = event->y();
	if (y < 0)
		y = 0;
	else if (y >= graph.height())
		y = graph.height() - 1;
	if (event->button() == Qt::LeftButton && modifiable && x >= 0 && x < graph.width())
	{
		int barWidth = graph.width() / values.size();
		int valueID = x / barWidth;
		int levelValue = ((graph.height() - y) * maxValue) / graph.height();
		values[valueID] = levelValue;
		valueChanged(valueID + 1, levelValue);
		updateView();
	}
	mousePressed(event);
}

void BarGraph::mouseReleaseEvent(QMouseEvent *event)
{
	mouseReleased(event);
}

void BarGraph::mouseDoubleClickEvent(QMouseEvent *event)
{
	mouseDoubleClicked(event);
}

void BarGraph::resizeEvent(QResizeEvent *event)
{
	if (!ignoreEvents)
	{
		setSceneRect(0.0, 0.0, (float)width(), (float)height());
		graph = QImage(width(), height(), QImage::Format_RGB32);
		updateView();
		event->accept();
	}
	else
		event->ignore();
}
