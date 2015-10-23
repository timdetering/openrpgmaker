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
#include <QScrollBar>
#include <QResizeEvent>
#include "projectdata.h"
#include "projectdata_map.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "staticmapview.h"

StaticMapView::StaticMapView(QWidget *parent) : QGraphicsView(parent)
{
	QScrollBar *hScrollBar = horizontalScrollBar();
	QScrollBar *vScrollBar = verticalScrollBar();
	hScrollBar->setSingleStep(16);
	vScrollBar->setSingleStep(16);
	QObject::connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoved()));
	QObject::connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoved()));
	ignoreevents = true;
	shutdown = true;
	setScene(new QGraphicsScene(0.0, 0.0, 32.0, 32.0));
	setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	item = new QGraphicsPixmapItem();
	item->setZValue(0.0);
	item->setFlags(0x0);
	item->setVisible(true);
	scene()->addItem(item);
	currentMap = NULL;
	currentZoom = ZOOM_1TO1;
}

StaticMapView::~StaticMapView()
{
}

void StaticMapView::setupView()
{
	QRect updateRect;
	int x, y;
	for (int i = 0; i < ProjectData::tilesetNames.size(); ++i)
		tilesets.append(new ProjectData::DataReference<Tileset>(i, __FILE__, __LINE__));
	mapWidth = currentMap->getWidth();
	mapHeight = currentMap->getHeight();
	setSceneRect(0.0, 0.0, float(mapWidth * currentZoom), float(mapHeight * currentZoom));
	updateRect.setX(int(mapToScene(0, 0).x()) / currentZoom);
	updateRect.setY(int(mapToScene(0, 0).y()) / currentZoom);
	if (updateRect.x() < 0)
		updateRect.setX(0);
	if (updateRect.y() < 0)
		updateRect.setY(0);
	x = width();
	y = height();
	if (verticalScrollBar()->isVisible())
		x -= verticalScrollBar()->width();
	if (horizontalScrollBar()->isVisible())
		y -= horizontalScrollBar()->height();
	updateRect.setWidth((x / currentZoom) + 2);
	updateRect.setHeight((y / currentZoom) + 2);
	if (updateRect.x() + updateRect.width() > mapWidth)
		updateRect.setWidth(mapWidth - updateRect.x());
	if (updateRect.y() + updateRect.height() > mapHeight)
		updateRect.setHeight(mapHeight - updateRect.y());
	mapViewImage = QImage(updateRect.width() * currentZoom, updateRect.height() * currentZoom, QImage::Format_RGB32);
	updateMapView();
	ignoreevents = false;
	shutdown = false;
}

void StaticMapView::shutdownView()
{
	for (int i = 0; i < tilesets.size(); ++i)
		delete tilesets[i];
	tilesets.clear();
	currentMap = NULL;
	ignoreevents = true;
	shutdown = true;
}

void StaticMapView::updateMapView()
{
	QPainter painter;
	QImage invalidTile(":/icons/invalidtile.png");
	QImage invalidTileset(":/icons/invalidtileset.png");
	QRect updateRect;
	QPoint location;
	int tilesetID, tileID, tileFlags;
	int x, y;
	updateRect.setX(int(mapToScene(0, 0).x()) / currentZoom);
	updateRect.setY(int(mapToScene(0, 0).y()) / currentZoom);
	if (updateRect.x() < 0)
		updateRect.setX(0);
	if (updateRect.y() < 0)
		updateRect.setY(0);
	x = width();
	y = height();
	if (verticalScrollBar()->isVisible())
		x -= verticalScrollBar()->width();
	if (horizontalScrollBar()->isVisible())
		y -= horizontalScrollBar()->height();
	updateRect.setWidth((x / currentZoom) + 2);
	updateRect.setHeight((y / currentZoom) + 2);
	if (updateRect.x() + updateRect.width() > mapWidth)
		updateRect.setWidth(mapWidth - updateRect.x());
	if (updateRect.y() + updateRect.height() > mapHeight)
		updateRect.setHeight(mapHeight - updateRect.y());
	painter.begin(&mapViewImage);
	painter.setOpacity(1.0);
	painter.fillRect(0, 0, mapViewImage.width(), mapViewImage.height(), Qt::magenta);
	painter.scale(currentZoom / 32.0, currentZoom / 32.0);
	for (y = 0; y < updateRect.height(); ++y)
	{
		for (x = 0; x < updateRect.width(); ++x)
		{
			location.setX(x + updateRect.x());
			location.setY(y + updateRect.y());
			painter.setOpacity(1.0);
			tilesetID = currentMap->getTilesetID(location.x(), location.y(), LAYER_LOWER);
			tileID = currentMap->getTileID(location.x(), location.y(), LAYER_LOWER);
			tileFlags = currentMap->getTileFlags(location.x(), location.y(), LAYER_LOWER);
			if (tilesetID >= tilesets.size())
				painter.drawImage(x * 32, y * 32, invalidTileset);
			else if (tileID >= (*tilesets[tilesetID])->numTiles())
				painter.drawImage(x * 32, y * 32, invalidTile);
			else
				(*tilesets[tilesetID])->getTile(tileID)->drawTile(painter, QPoint(x * 32, y * 32), tileFlags);
			tilesetID = currentMap->getTilesetID(location.x(), location.y(), LAYER_HERO);
			tileID = currentMap->getTileID(location.x(), location.y(), LAYER_HERO);
			tileFlags = currentMap->getTileFlags(location.x(), location.y(), LAYER_HERO);
			if (tilesetID >= tilesets.size())
				painter.drawImage(x * 32, y * 32, invalidTileset);
			else if (tileID >= (*tilesets[tilesetID])->numTiles())
				painter.drawImage(x * 32, y * 32, invalidTile);
			else
				(*tilesets[tilesetID])->getTile(tileID)->drawTile(painter, QPoint(x * 32, y * 32), tileFlags);
			tilesetID = currentMap->getTilesetID(location.x(), location.y(), LAYER_UPPER);
			tileID = currentMap->getTileID(location.x(), location.y(), LAYER_UPPER);
			tileFlags = currentMap->getTileFlags(location.x(), location.y(), LAYER_UPPER);
			if (tilesetID >= tilesets.size())
				painter.drawImage(x * 32, y * 32, invalidTileset);
			else if (tileID >= (*tilesets[tilesetID])->numTiles())
				painter.drawImage(x * 32, y * 32, invalidTile);
			else
				(*tilesets[tilesetID])->getTile(tileID)->drawTile(painter, QPoint(x * 32, y * 32), tileFlags);
		}
	}
	painter.end();
	item->setPixmap(QPixmap::fromImage(mapViewImage));
	item->setPos(updateRect.x() * float(currentZoom), updateRect.y() * float(currentZoom));
}

void StaticMapView::changeMap(Map *map)
{
	QRect updateRect;
	int x, y;
	currentMap = map;
	if (!shutdown)
	{
		mapWidth = currentMap->getWidth();
		mapHeight = currentMap->getHeight();
		updateRect.setX(int(mapToScene(0, 0).x()) / currentZoom);
		updateRect.setY(int(mapToScene(0, 0).y()) / currentZoom);
		if (updateRect.x() < 0)
			updateRect.setX(0);
		if (updateRect.y() < 0)
			updateRect.setY(0);
		x = width();
		y = height();
		if (verticalScrollBar()->isVisible())
			x -= verticalScrollBar()->width();
		if (horizontalScrollBar()->isVisible())
			y -= horizontalScrollBar()->height();
		updateRect.setWidth((x / currentZoom) + 2);
		updateRect.setHeight((y / currentZoom) + 2);
		if (updateRect.x() + updateRect.width() > mapWidth)
			updateRect.setWidth(mapWidth - updateRect.x());
		if (updateRect.y() + updateRect.height() > mapHeight)
			updateRect.setHeight(mapHeight - updateRect.y());
		mapViewImage = QImage(updateRect.width() * currentZoom, updateRect.height() * currentZoom, QImage::Format_RGB32);
		setSceneRect(0.0, 0.0, float(mapWidth * currentZoom), float(mapHeight * currentZoom));
		ensureVisible(QRect(0.0, 0.0, 32.0, 32.0), 0, 0);
		updateMapView();
	}
}

void StaticMapView::changeZoom(int newZoom)
{
	if (newZoom != currentZoom)
	{
		QRect updateRect;
		int x, y;
		currentZoom = newZoom;
		setSceneRect(0.0, 0.0, float(mapWidth * currentZoom), float(mapHeight * currentZoom));
		horizontalScrollBar()->setSingleStep(currentZoom / 2);
		verticalScrollBar()->setSingleStep(currentZoom / 2);
		updateRect.setX(int(mapToScene(0, 0).x()) / currentZoom);
		updateRect.setY(int(mapToScene(0, 0).y()) / currentZoom);
		if (updateRect.x() < 0)
			updateRect.setX(0);
		if (updateRect.y() < 0)
			updateRect.setY(0);
		x = width();
		y = height();
		if (verticalScrollBar()->isVisible())
			x -= verticalScrollBar()->width();
		if (horizontalScrollBar()->isVisible())
			y -= horizontalScrollBar()->height();
		updateRect.setWidth((x / currentZoom) + 2);
		updateRect.setHeight((y / currentZoom) + 2);
		if (updateRect.x() + updateRect.width() > mapWidth)
			updateRect.setWidth(mapWidth - updateRect.x());
		if (updateRect.y() + updateRect.height() > mapHeight)
			updateRect.setHeight(mapHeight - updateRect.y());
		mapViewImage = QImage(updateRect.width() * currentZoom, updateRect.height() * currentZoom, QImage::Format_RGB32);
		updateMapView();
	}
	currentZoom = newZoom;
}

int StaticMapView::getCurrentZoom() const
{
	return currentZoom;
}

void StaticMapView::scrollBarMoved()
{
	if (!ignoreevents)
		updateMapView();
}

void StaticMapView::resizeEvent(QResizeEvent *event)
{
	if (!ignoreevents)
	{
		QRect updateRect;
		int x, y;
		updateRect.setX(int(mapToScene(0, 0).x()) / currentZoom);
		updateRect.setY(int(mapToScene(0, 0).y()) / currentZoom);
		if (updateRect.x() < 0)
			updateRect.setX(0);
		if (updateRect.y() < 0)
			updateRect.setY(0);
		x = width();
		y = height();
		if (verticalScrollBar()->isVisible())
			x -= verticalScrollBar()->width();
		if (horizontalScrollBar()->isVisible())
			y -= horizontalScrollBar()->height();
		updateRect.setWidth((x / currentZoom) + 2);
		updateRect.setHeight((y / currentZoom) + 2);
		if (updateRect.x() + updateRect.width() > mapWidth)
			updateRect.setWidth(mapWidth - updateRect.x());
		if (updateRect.y() + updateRect.height() > mapHeight)
			updateRect.setHeight(mapHeight - updateRect.y());
		mapViewImage = QImage(updateRect.width() * currentZoom, updateRect.height() * currentZoom, QImage::Format_RGB32);
		setSceneRect(0.0, 0.0, mapWidth * float(currentZoom), mapHeight * float(currentZoom));
		updateMapView();
		event->accept();
	}
	else
		event->ignore();
}
