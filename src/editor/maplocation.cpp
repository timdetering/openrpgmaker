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

#include <QGraphicsRectItem>
#include <QHeaderView>
#include <QPen>
#include <QRect>
#include <QMouseEvent>
#include "maplocation.h"
#include "projectdata.h"
#include "projectdata_map.h"

MapLocation::MapLocation(QWidget *parent) : QWidget(parent)
{
	QHeaderView *header;
	ignoreEvents = true;
	setupUi(this);
	header = twLayers->horizontalHeader();
	header->setResizeMode(0, QHeaderView::Fixed);
	header->setResizeMode(1, QHeaderView::Stretch);
	header->resizeSections(QHeaderView::ResizeToContents);
	ignoreEvents = false;
}

void MapLocation::setupMapLocation(int map)
{
	QVector<qreal> selectionDashPattern;
	QPen selectionPen;
	QRect selectionRect;
	ignoreEvents = true;
	gvMapView->changeMap(map);
	gvMapView->setupView();
	gvMapView->changeLayer(0);
	gvMapView->changeEditingMode(MapView::EDITMODE_PREVIEW);
	selectionDashPattern << 4.0 << 2.0;
	selectionPen.setStyle(Qt::CustomDashLine);
	selectionPen.setColor(QColor(Qt::cyan));
	selectionPen.setWidth(3);
	selectionPen.setCapStyle(Qt::FlatCap);
	selectionPen.setJoinStyle(Qt::MiterJoin);
	selectionPen.setDashPattern(selectionDashPattern);
	currentSelection = new QGraphicsRectItem(1.0, 1.0, 30.0, 30.0);
	currentSelection->setZValue(1.0);
	currentSelection->setFlags(0x0);
	currentSelection->setPen(selectionPen);
	currentSelection->setBrush(QBrush(Qt::transparent));
	currentSelection->setVisible(true);
	gvMapView->scene()->addItem(currentSelection);
	currentX = 0;
	currentY = 0;
	currentZoom = MapView::ZOOM_1TO1;
	mapRef = NULL;
	changeMap(map);
	setLocation(0, 0, 0);
	ignoreEvents = false;
}

void MapLocation::changeMap(int mapID)
{
	QTableWidgetItem *twItem;
	int numLayers;
	if (mapRef != NULL)
	{
		delete mapRef;
		mapRef = NULL;
	}
	mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	numLayers = (*mapRef)->numLayers();
	gvMapView->changeMap(mapID);
	gvMapView->changeLayer(0);
	gvMapView->changeEditingMode(MapView::EDITMODE_PREVIEW);
	twLayers->clearContents();
	twLayers->setRowCount(numLayers);
	for (int i = 0; i < numLayers; ++i)
	{
		Map::LayerData *layerData = (*mapRef)->getLayerData(i);
		twItem = new QTableWidgetItem();
		twItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable);
		twItem->setCheckState((layerData->visible) ? Qt::Checked:Qt::Unchecked);
		twLayers->setItem(i, 0, twItem);
		twItem = new QTableWidgetItem(layerData->name);
		twItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		twLayers->setItem(i, 1, twItem);
		twLayers->setRowHeight(i, 16);
	}
	twLayers->item(0, 0)->setSelected(true);
	twLayers->item(0, 1)->setSelected(true);
	twLayers->setCurrentCell(0, 1);
}

MapLocation::~MapLocation()
{
	gvMapView->shutdownView();
	delete mapRef;
}

void MapLocation::setLocation(int x, int y, int layer)
{
	currentX = x;
	currentY = y;
	currentSelection->setRect(currentX * float(currentZoom) + 1.0, currentY * float(currentZoom) + 1.0, float(currentZoom) - 2.0, float(currentZoom) - 2.0);
	gvMapView->centerOn(currentSelection);
	twLayers->setCurrentCell(layer, 1);
	twLayers->item(layer, 1)->setSelected(true);
}

int MapLocation::getXLocation()
{
	return currentX;
}

int MapLocation::getYLocation()
{
	return currentY;
}

int MapLocation::getLayer()
{
	return twLayers->currentRow();
}

void MapLocation::on_bZoom1to1_toggled(bool checked)
{
	if (checked)
	{
		currentZoom = MapView::ZOOM_1TO1;
		gvMapView->changeZoom(currentZoom);
		currentSelection->setRect(currentX * float(currentZoom) + 1.0, currentY * float(currentZoom) + 1.0, float(currentZoom) - 2.0, float(currentZoom) - 2.0);
	}
}

void MapLocation::on_bZoom1to2_toggled(bool checked)
{
	if (checked)
	{
		currentZoom = MapView::ZOOM_1TO2;
		gvMapView->changeZoom(currentZoom);
		currentSelection->setRect(currentX * float(currentZoom) + 1.0, currentY * float(currentZoom) + 1.0, float(currentZoom) - 2.0, float(currentZoom) - 2.0);
	}
}

void MapLocation::on_bZoom1to4_toggled(bool checked)
{
	if (checked)
	{
		currentZoom = MapView::ZOOM_1TO4;
		gvMapView->changeZoom(currentZoom);
		currentSelection->setRect(currentX * float(currentZoom) + 1.0, currentY * float(currentZoom) + 1.0, float(currentZoom) - 2.0, float(currentZoom) - 2.0);
	}
}

void MapLocation::on_bZoom1to8_toggled(bool checked)
{
	if (checked)
	{
		currentZoom = MapView::ZOOM_1TO8;
		gvMapView->changeZoom(currentZoom);
		currentSelection->setRect(currentX * float(currentZoom) + 1.0, currentY * float(currentZoom) + 1.0, float(currentZoom) - 2.0, float(currentZoom) - 2.0);
	}
}

void MapLocation::on_gvMapView_mouseButtonPressed(QMouseEvent *event)
{
	QPoint sceneCoordinates, tileLocation;
	if (event->button() == Qt::LeftButton)
	{
		QPointF testPoint = gvMapView->mapToScene(event->x(), event->y());
		int mapWidth = gvMapView->getMapWidth();
		int mapHeight = gvMapView->getMapHeight();
		sceneCoordinates.setX(int(gvMapView->mapToScene(event->x(), event->y()).x()) / currentZoom);
		sceneCoordinates.setY(int(gvMapView->mapToScene(event->x(), event->y()).y()) / currentZoom);
		if (testPoint.x() < 0.0 && sceneCoordinates.x() == 0)
			sceneCoordinates.setX(-1);
		if (testPoint.y() < 0.0 && sceneCoordinates.y() == 0)
			sceneCoordinates.setY(-1);
		tileLocation = sceneCoordinates;
		if (tileLocation.x() < 0)
			tileLocation.setX(0);
		else if (tileLocation.x() >= mapWidth)
			tileLocation.setX(mapWidth - 1);
		if (tileLocation.y() < 0)
			tileLocation.setY(0);
		else if (tileLocation.y() >= mapHeight)
			tileLocation.setY(mapHeight - 1);
		currentX = tileLocation.x();
		currentY = tileLocation.y();
		currentSelection->setRect(currentX * float(currentZoom) + 1.0, currentY * float(currentZoom) + 1.0, float(currentZoom) - 2.0, float(currentZoom) - 2.0);
	}
}

void MapLocation::on_twLayers_itemChanged(QTableWidgetItem *item)
{
	int row = item->row();
	int column = item->column();
	if (column == 0 && mapRef != NULL && !ignoreEvents)
	{
		ignoreEvents = true;
		if (item->checkState() == Qt::Checked)
			(*mapRef)->setLayerVisible(row, true);
		else
			(*mapRef)->setLayerVisible(row, false);
		gvMapView->updateMapView();
		ignoreEvents = false;
	}
}

void MapLocation::on_twLayers_itemSelectionChanged()
{
	int row = twLayers->currentRow();
	if (!ignoreEvents && mapRef != NULL)
	{
		if (row >= 0 && row < (*mapRef)->numLayers())
			gvMapView->changeLayer(twLayers->currentRow());
	}
}
