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

#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QKeyEvent>
#include <QCoreApplication>
#include "clipboard.h"
#include "mainwindow.h"
#include "mapeventeditor.h"
#include "mapview.h"
#include "projectdata.h"
#include "projectdata_baseevent.h"
#include "projectdata_character.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "projectdata_vehicle.h"
#include "selectvehicle.h"
#include "xmlParser.h"

MapView::MapView(QWidget *parent) : QGraphicsView(parent)
{
	QScrollBar *hScrollBar = horizontalScrollBar();
	QScrollBar *vScrollBar = verticalScrollBar();
	QVector<qreal> selectionDashPattern;
	selectionDashPattern << 4.0 << 2.0;
	selectionDashOffset = 0;
	ignoreevents = true;
	ignoreContextMenu = false;
	setCursor(Qt::CrossCursor);
	hScrollBar->setCursor(Qt::ArrowCursor);
	vScrollBar->setCursor(Qt::ArrowCursor);
	hScrollBar->setSingleStep(16);
	vScrollBar->setSingleStep(16);
	scrollEvent.setX(0);
	scrollEvent.setY(0);
	timer = new QTimer(this);
	selectionTimer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(scrollArea()));
	QObject::connect(selectionTimer, SIGNAL(timeout()), this, SLOT(animateSelection()));
	QObject::connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoved()));
	QObject::connect(vScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoved()));
	timer->start(100);
	waitevents = false;
	setScene(new QGraphicsScene(0.0, 0.0, 32.0, 32.0));
	setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	arrowBG = QImage(":/icons/arrowBG_tiling.png");
	arrow_nw = QImage(":/icons/upleftarrow.png");
	arrow_n = QImage(":/icons/uparrow.png");
	arrow_ne = QImage(":/icons/uprightarrow.png");
	arrow_e = QImage(":/icons/rightarrow.png");
	arrow_se = QImage(":/icons/downrightarrow.png");
	arrow_s = QImage(":/icons/downarrow.png");
	arrow_sw = QImage(":/icons/downleftarrow.png");
	arrow_w = QImage(":/icons/leftarrow.png");
	areaMarker = QImage(":/icons/areaMarker.png");
	layerText = QImage(":/icons/layer.png");
	layerNumbers = QImage(":/icons/numbersHighlighted.png");
	invalidTile = QImage(":/icons/invalidtile.png");
	invalidTileset = QImage(":/icons/invalidtileset.png");
	grid = QImage(":/icons/grid.png");
	selectedEvent = QImage(":/icons/selectedevent.png");
	mapEventMarker = QImage(":/icons/mapEventMarker.png");
	mapEventBG = QImage(":/icons/mapEventBG.png");
	partyStart = QImage(":/icons/partyStart.png");
	passabilityMarker = QImage(":/icons/passabilityMarker.png");
	vehicleStart = QImage(":/icons/vehicleStart.png");
	numbers = QImage(":/icons/vehicleNumbers.png");
	tempLayerData_AutoCalc = QList<Map::TileData>();
	tempLayerData_NoAutoCalc = QList<Map::TileData>();
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
	currentSelection->setVisible(false);
	scene()->addItem(currentSelection);
	mapRef = NULL;
	currentEditingMode = EDITMODE_LAYERS;
	currentLayer = 0;
	currentZoom = ZOOM_1TO1;
	currentTool = TOOL_PENCIL;
	currentPhase = 0;
	eventID = -1;
	startLocationID = -2;
	currentMapID = -1;
	tilingMode = TILING_ON;
	passabilityMode = PASSMODE_SET;
	shutdown = true;
	showGrid = false;
	selectionData.tileData = QList<Map::TileData>();
	selectionData.x = -1;
	selectionData.y = -1;
	selectionData.width = 0;
	selectionData.height = 0;
	selectionData.anchored = true;
	aCopy = new QAction(QIcon(":/icons/editcopy.png"), "Copy", this);
	aCopy->setShortcutContext(Qt::WidgetShortcut);
	aCut = new QAction(QIcon(":/icons/editcut.png"), "Cut", this);
	aCut->setShortcutContext(Qt::WidgetShortcut);
	aPaste = new QAction(QIcon(":/icons/editpaste.png"), "Paste", this);
	aPaste->setShortcutContext(Qt::WidgetShortcut);
	aDelete = new QAction(QIcon(":/icons/editdelete.png"), "Delete", this);
	aDelete->setShortcutContext(Qt::WidgetShortcut);
	aDuplicate = new QAction(QIcon(":/icons/editcopy.png"), "Duplicate", this);
	aClear = new QAction(QIcon(":/icons/clearselection.png"), "Clear Selection", this);
	aAnchor = new QAction(QIcon(":/icons/anchorselection.png"), "Anchor Selection", this);
	aDetach = new QAction(QIcon(":/icons/detachselection.png"), "Detach Selection", this);
	aNewEvent = new QAction(QIcon(":/icons/newevent.png"), "New Event", this);
	aNewEvent->setShortcutContext(Qt::WidgetShortcut);
	aEditEvent = new QAction(QIcon(":/icons/editevent.png"), "Edit Event", this);
	aEditEvent->setShortcutContext(Qt::WidgetShortcut);
	aCutEvent = new QAction(QIcon(":/icons/cutevent.png"), "Cut", this);
	aCutEvent->setShortcutContext(Qt::WidgetShortcut);
	aCopyEvent = new QAction(QIcon(":/icons/copyevent.png"), "Copy", this);
	aCopyEvent->setShortcutContext(Qt::WidgetShortcut);
	aPasteEvent = new QAction(QIcon(":/icons/pasteevent.png"), "Paste", this);
	aPasteEvent->setShortcutContext(Qt::WidgetShortcut);
	aDeleteEvent = new QAction(QIcon(":/icons/deleteevent.png"), "Delete", this);
	aDeleteEvent->setShortcutContext(Qt::WidgetShortcut);
	aPlaceTeleportEvent = new QAction(QIcon(":/icons/teleportevent.png"), "Place Teleport Event", this);
	aPlacePartyStartLocation = new QAction(QIcon(":/icons/partyStart.png"), "Place Party's Starting Location", this);
	aPlaceVehicleStartLocation = new QAction(QIcon(":/icons/vehicleStart.png"), "Place Vehicle's Starting Location", this);
	aMoveUpLayer = new QAction(QIcon(":/icons/uparrow2.png"), "Move Up Layer", this);
	aMoveDownLayer = new QAction(QIcon(":/icons/downarrow2.png"), "Move Down Layer", this);
	aCropToSelection = new QAction(QIcon(":/icons/crop.png"), "Crop to Selection", this);
	addAction(aCopy);
	addAction(aCut);
	addAction(aPaste);
	addAction(aDelete);
	addAction(aDuplicate);
	addAction(aClear);
	addAction(aAnchor);
	addAction(aDetach);
	addAction(aNewEvent);
	addAction(aEditEvent);
	addAction(aCutEvent);
	addAction(aCopyEvent);
	addAction(aPasteEvent);
	addAction(aDeleteEvent);
	addAction(aPlaceTeleportEvent);
	addAction(aPlacePartyStartLocation);
	addAction(aPlaceVehicleStartLocation);
	addAction(aMoveUpLayer);
	addAction(aMoveDownLayer);
	addAction(aCropToSelection);
	QObject::connect(aCopy, SIGNAL(triggered(bool)), this, SLOT(copySelection()));
	QObject::connect(aCut, SIGNAL(triggered(bool)), this, SLOT(cutSelection()));
	QObject::connect(aDelete, SIGNAL(triggered(bool)), this, SLOT(deleteSelection()));
	QObject::connect(aPaste, SIGNAL(triggered(bool)), this, SLOT(pasteSelection()));
	QObject::connect(aDuplicate, SIGNAL(triggered(bool)), this, SLOT(duplicateSelection()));
	QObject::connect(aClear, SIGNAL(triggered(bool)), this, SLOT(clearSelection()));
	QObject::connect(aAnchor, SIGNAL(triggered(bool)), this, SLOT(anchorSelection()));
	QObject::connect(aDetach, SIGNAL(triggered(bool)), this, SLOT(detachSelection()));
	QObject::connect(aNewEvent, SIGNAL(triggered(bool)), this, SLOT(newEvent()));
	QObject::connect(aEditEvent, SIGNAL(triggered(bool)), this, SLOT(editEvent()));
	QObject::connect(aCutEvent, SIGNAL(triggered(bool)), this, SLOT(cutEvent()));
	QObject::connect(aCopyEvent, SIGNAL(triggered(bool)), this, SLOT(copyEvent()));
	QObject::connect(aPasteEvent, SIGNAL(triggered(bool)), this, SLOT(pasteEvent()));
	QObject::connect(aDeleteEvent, SIGNAL(triggered(bool)), this, SLOT(deleteEvent()));
	QObject::connect(aPlaceTeleportEvent, SIGNAL(triggered(bool)), this, SLOT(placeTeleportEvent()));
	QObject::connect(aPlacePartyStartLocation, SIGNAL(triggered(bool)), this, SLOT(placePartyStartLocation()));
	QObject::connect(aPlaceVehicleStartLocation, SIGNAL(triggered(bool)), this, SLOT(placeVehicleStartLocation()));
	QObject::connect(aMoveUpLayer, SIGNAL(triggered(bool)), this, SLOT(moveUpLayer()));
	QObject::connect(aMoveDownLayer, SIGNAL(triggered(bool)), this, SLOT(moveDownLayer()));
	QObject::connect(aCropToSelection, SIGNAL(triggered(bool)), this, SLOT(cropToSelection()));
}

MapView::~MapView()
{
	if (mapRef != NULL)
	{
		delete mapRef;
		mapRef = NULL;
	}
}

void MapView::setupView()
{
	Map::TileData tileData;
	QRect updateRect;
	int x, y;
	updateTilesetList();
	autocalc = false;
	tileData.tilesetID = 0x0;
	tileData.tileID = 0x0;
	tileData.tileFlags = 0x0;
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		tileData.phaseDirections[i] = 0x0;
		tileData.phasePassability[i] = false;
	}
	tileSelection.append(tileData);
	tileSelectionWidth = 1;
	tileSelectionHeight = 1;
	currentSelection->setRect(1.0, 1.0, 30.0, 30.0);
	currentSelection->setVisible(false);
	ignoreContextMenu = true;
	rectStart = QPoint(-1, -1);
	rectEnd = QPoint(-1, -1);
	eventID = -1;
	if (mapRef == NULL && ProjectData::mapNames.contains(currentMapID))
		mapRef = new ProjectData::DataReference<Map>(currentMapID, __FILE__, __LINE__);
	if (mapRef != NULL)
	{
		mapWidth = (*mapRef)->getWidth();
		mapHeight = (*mapRef)->getHeight();
		item = new QGraphicsPixmapItem();
		item->setZValue(0.0);
		item->setFlags(0x0);
		item->setVisible(true);
		scene()->addItem(item);
		setSceneRect(0.0, 0.0, float(mapWidth * currentZoom), float(mapHeight * currentZoom));
		selectionRect.setX(0);
		selectionRect.setY(0);
		selectionRect.setWidth(-1);
		selectionRect.setHeight(-1);
		tempLayerData_AutoCalc = tempLayerData_NoAutoCalc = (*mapRef)->getLayerData(currentLayer)->tileData;
		undoHistory = QList<UndoEvent>();
		undoLocation = -1;
		decrementUndoLocation = false;
		incrementUndoLocation = false;
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
		mapViewImage = QImage(updateRect.width() * currentZoom, updateRect.height() * currentZoom, QImage::Format_ARGB32);
		updateMapView();
		ignoreevents = false;
		canceled = false;
		shutdown = false;
	}
}

void MapView::shutdownView()
{
	if (!shutdown)
	{
		for (int i = 0; i < tilesets.size(); ++i)
			delete tilesets[i];
		tilesets.clear();
		if (mapRef != NULL)
		{
			delete mapRef;
			mapRef = NULL;
		}
		ignoreevents = true;
		ignoreContextMenu = true;
		item->setVisible(false);
		currentSelection->setVisible(false);
		timer->stop();
		shutdown = true;
	}
}

void MapView::updateTilesetList()
{
	if (tilesets.size() > 0)
	{
		if (tilesets.size() <= ProjectData::tilesetNames.size())
		{
			for (int i = 0; i < tilesets.size(); ++i)
				delete tilesets[i];
		}
		else
		{
			for (int i = 0; i < ProjectData::tilesetNames.size(); ++i)
				delete tilesets[i];
		}
	}
	tilesets.clear();
	for (int i = 0; i < ProjectData::tilesetNames.size(); ++i)
		tilesets.append(new ProjectData::DataReference<Tileset>(i, __FILE__, __LINE__));
}

void MapView::updateMapView()
{
	if (mapRef != NULL && !shutdown)
	{
		QPainter painter;
		QRect updateRect;
		int x, y;
		int numLayers = (*mapRef)->numLayers();
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
		painter.setCompositionMode(QPainter::CompositionMode_Clear);
		painter.fillRect(0, 0, mapViewImage.width(), mapViewImage.height(), Qt::transparent);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		painter.scale(currentZoom / 32.0, currentZoom / 32.0);
		if (currentEditingMode == EDITMODE_LAYERS || currentEditingMode == EDITMODE_PASSABILITY)
		{
			if (currentTool != TOOL_TILING)
			{
				painter.setOpacity(1.0);
				for (int i = 0; i < currentLayer; ++i)
				{
					if ((*mapRef)->isLayerVisible(i))
						renderLayer(i, updateRect, updateRect.topLeft(), painter);
					renderEvents(i, updateRect, updateRect.topLeft(), painter);
					renderStartLocations(i, updateRect, updateRect.topLeft(), painter);
				}
				if (currentLayer > 0)
				{
					painter.setOpacity(0.5);
					painter.fillRect(0, 0, updateRect.width() * 32, updateRect.height() * 32, Qt::black);
					painter.setOpacity(1.0);
				}
				renderLayer(currentLayer, updateRect, updateRect.topLeft(), painter);
				renderEvents(currentLayer, updateRect, updateRect.topLeft(), painter);
				renderStartLocations(currentLayer, updateRect, updateRect.topLeft(), painter);
				painter.setOpacity(0.5);
				for (int i = currentLayer + 1; i < numLayers; ++i)
				{
					if ((*mapRef)->isLayerVisible(i))
						renderLayer(i, updateRect, updateRect.topLeft(), painter);
					renderEvents(i, updateRect, updateRect.topLeft(), painter);
					renderStartLocations(i, updateRect, updateRect.topLeft(), painter);
				}
				painter.setOpacity(1.0);
				if (currentEditingMode == EDITMODE_PASSABILITY)
					renderPassability(updateRect, updateRect.topLeft(), painter);
			}
			else
				renderLayer(currentLayer, updateRect, updateRect.topLeft(), painter);
		}
		else if (currentEditingMode == EDITMODE_EVENTS)
		{
			painter.setOpacity(1.0);
			for (int i = 0; i < numLayers; ++i)
			{
				if ((*mapRef)->isLayerVisible(i))
					renderLayer(i, updateRect, updateRect.topLeft(), painter);
			}
			renderEvents(-1, updateRect, updateRect.topLeft(), painter);
			renderStartLocations(-1, updateRect, updateRect.topLeft(), painter);
			if (updateRect.contains(rectStart))
				painter.drawImage((rectStart.x() - updateRect.x()) * 32, (rectStart.y() - updateRect.y()) * 32, selectedEvent);
		}
		else if (currentEditingMode == EDITMODE_PREVIEW)
		{
			QPixmap bgImage(ProjectData::getAbsoluteResourcePath((*mapRef)->getBGLocation()));
			painter.setOpacity(1.0);
			if (!bgImage.isNull())
			{
				int xOffset = (updateRect.x() * 32) % bgImage.width();
				int yOffset = (updateRect.y() * 32) % bgImage.height();
				painter.drawTiledPixmap(0, 0, mapViewImage.width() * 32, mapViewImage.height() * 32, bgImage, xOffset, yOffset);
			}
			for (int i = 0; i < numLayers; ++i)
			{
				if ((*mapRef)->isLayerVisible(i))
					renderLayer(i, updateRect, updateRect.topLeft(), painter);
				renderEvents(i, updateRect, updateRect.topLeft(), painter);
			}
		}
		painter.setOpacity(1.0);
		if (currentEditingMode != EDITMODE_PREVIEW && currentTool != TOOL_TILING && showGrid)
		{
			for (y = 0; y < updateRect.height(); ++y)
			{
				for (x = 0; x < updateRect.width(); ++x)
					painter.drawImage(x * 32, y * 32, grid);
			}
		}
		painter.end();
		item->setPixmap(QPixmap::fromImage(mapViewImage));
		item->setPos(updateRect.x() * float(currentZoom), updateRect.y() * float(currentZoom));
	}
}

void MapView::updateMapView(QRect rectangle)
{
	QPainter painter;
	QRect updateRect;
	QRect croppedRect;
	QPoint location;
	int x, y;
	int numLayers = (*mapRef)->numLayers();
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
	croppedRect = rectangle;
	if (croppedRect.x() < 0)
		croppedRect.setX(0);
	if (croppedRect.x() + croppedRect.width() > mapWidth)
		croppedRect.setWidth(mapWidth - croppedRect.x());
	if (croppedRect.y() < 0)
		croppedRect.setY(0);
	if (croppedRect.y() + croppedRect.height() > mapHeight)
		croppedRect.setHeight(mapHeight - croppedRect.y());
	painter.begin(&mapViewImage);
	painter.scale(currentZoom / 32.0, currentZoom / 32.0);
	location.setX(croppedRect.x() - updateRect.x());
	location.setY(croppedRect.y() - updateRect.y());
	painter.setOpacity(1.0);
	painter.setCompositionMode(QPainter::CompositionMode_Clear);
	painter.fillRect(location.x() * 32, location.y() * 32, croppedRect.width() * 32, croppedRect.height() * 32, Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	if (currentEditingMode == EDITMODE_LAYERS || currentEditingMode == EDITMODE_PASSABILITY)
	{
		if (currentTool != TOOL_TILING)
		{
			painter.setOpacity(1.0);
			for (int i = 0; i < currentLayer; ++i)
			{
				if ((*mapRef)->isLayerVisible(i))
					renderLayer(i, croppedRect, updateRect.topLeft(), painter);
				renderEvents(i, croppedRect, updateRect.topLeft(), painter);
				renderStartLocations(i, croppedRect, updateRect.topLeft(), painter);
			}
			if (currentLayer > 0)
			{
				painter.setOpacity(0.5);
				painter.fillRect(location.x() * 32, location.y() * 32, croppedRect.width() * 32, croppedRect.height() * 32, Qt::black);
				painter.setOpacity(1.0);
			}
			renderLayer(currentLayer, croppedRect, updateRect.topLeft(), painter);
			renderEvents(currentLayer, croppedRect, updateRect.topLeft(), painter);
			renderStartLocations(currentLayer, croppedRect, updateRect.topLeft(), painter);
			painter.setOpacity(0.5);
			for (int i = currentLayer + 1; i < numLayers; ++i)
			{
				if ((*mapRef)->isLayerVisible(i))
					renderLayer(i, croppedRect, updateRect.topLeft(), painter);
				renderEvents(i, croppedRect, updateRect.topLeft(), painter);
				renderStartLocations(i, croppedRect, updateRect.topLeft(), painter);
			}
			painter.setOpacity(1.0);
			if (currentEditingMode == EDITMODE_PASSABILITY)
				renderPassability(croppedRect, updateRect.topLeft(), painter);
		}
		else
			renderLayer(currentLayer, croppedRect, updateRect.topLeft(), painter);
	}
	else if (currentEditingMode == EDITMODE_EVENTS)
	{
		painter.setOpacity(1.0);
		for (int i = 0; i < numLayers; ++i)
		{
			if ((*mapRef)->isLayerVisible(i))
				renderLayer(i, croppedRect, updateRect.topLeft(), painter);
		}
		renderEvents(-1, croppedRect, updateRect.topLeft(), painter);
		renderStartLocations(-1, croppedRect, updateRect.topLeft(), painter);
		if (croppedRect.contains(rectStart))
			painter.drawImage((rectStart.x() - updateRect.x()) * 32, (rectStart.y() - updateRect.y()) * 32, selectedEvent);
	}
	painter.setOpacity(1.0);
	if (currentEditingMode != EDITMODE_PREVIEW && currentTool != TOOL_TILING && showGrid)
	{
		for (y = 0; y < croppedRect.height(); ++y)
		{
			for (x = 0; x < croppedRect.width(); ++x)
				painter.drawImage((x + croppedRect.x() - updateRect.x()) * 32, (y + croppedRect.y() - updateRect.y()) * 32, grid);
		}
	}
	painter.end();
	item->setPixmap(QPixmap::fromImage(mapViewImage));
	item->setPos(updateRect.x() * float(currentZoom), updateRect.y() * float(currentZoom));
}

void MapView::updateMapView(QPoint location)
{
	QPainter painter;
	QRect updateRect;
	QRect region = QRect(location.x(), location.y(), 1, 1);
	int x, y;
	int numLayers = (*mapRef)->numLayers();
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
	if (updateRect.contains(location))
	{
		painter.begin(&mapViewImage);
		painter.scale(currentZoom / 32.0, currentZoom / 32.0);
		x = location.x() - updateRect.x();
		y = location.y() - updateRect.y();
		painter.setOpacity(1.0);
		painter.setCompositionMode(QPainter::CompositionMode_Clear);
		painter.fillRect(x * 32, y * 32, 32, 32, Qt::transparent);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		if (currentEditingMode == EDITMODE_LAYERS || currentEditingMode == EDITMODE_PASSABILITY)
		{
			if (currentTool != TOOL_TILING)
			{
				painter.setOpacity(1.0);
				for (int i = 0; i < currentLayer; ++i)
				{
					if ((*mapRef)->isLayerVisible(i))
						renderLayer(i, region, updateRect.topLeft(), painter);
					renderEvents(i, region, updateRect.topLeft(), painter);
					renderStartLocations(i, region, updateRect.topLeft(), painter);
				}
				if (currentLayer > 0)
				{
					painter.setOpacity(0.5);
					painter.fillRect(location.x() * 32, location.y() * 32, 32, 32, Qt::black);
					painter.setOpacity(1.0);
				}
				renderLayer(currentLayer, region, updateRect.topLeft(), painter);
				renderEvents(currentLayer, region, updateRect.topLeft(), painter);
				renderStartLocations(currentLayer, region, updateRect.topLeft(), painter);
				painter.setOpacity(0.5);
				for (int i = currentLayer + 1; i < numLayers; ++i)
				{
					if ((*mapRef)->isLayerVisible(i))
						renderLayer(i, region, updateRect.topLeft(), painter);
					renderEvents(i, region, updateRect.topLeft(), painter);
					renderStartLocations(i, region, updateRect.topLeft(), painter);
				}
				painter.setOpacity(1.0);
				if (currentEditingMode == EDITMODE_PASSABILITY)
					renderPassability(region, updateRect.topLeft(), painter);
			}
			else
				renderLayer(currentLayer, region, updateRect.topLeft(), painter);
		}
		else if (currentEditingMode == EDITMODE_EVENTS)
		{
			painter.setOpacity(1.0);
			for (int i = 0; i < numLayers; ++i)
			{
				if ((*mapRef)->isLayerVisible(i))
					renderLayer(i, region, updateRect.topLeft(), painter);
			}
			renderEvents(-1, region, updateRect.topLeft(), painter);
			renderStartLocations(-1, region, updateRect.topLeft(), painter);
			if (location == rectStart)
				painter.drawImage(x * 32, y * 32, selectedEvent);
		}
		painter.setOpacity(1.0);
		if (currentEditingMode != EDITMODE_PREVIEW && currentTool != TOOL_TILING && showGrid)
			painter.drawImage(x * 32, y * 32, grid);
	}
	painter.end();
	item->setPixmap(QPixmap::fromImage(mapViewImage));
	item->setPos(updateRect.x() * float(currentZoom), updateRect.y() * float(currentZoom));
}

void MapView::changeMap(int mapID, bool forceUpdate)
{
	currentMapID = mapID;
	if (mapRef != NULL)
	{
		if (mapID == mapRef->getDataID() && !forceUpdate)
			return;
		delete mapRef;
		mapRef = NULL;
	}
	if (ProjectData::mapNames.contains(mapID) && !shutdown)
	{
		mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
		if (!shutdown)
		{
			QRect updateRect;
			QRectF selection;
			int x, y;
			waitevents = true;
			selection = currentSelection->rect();
			updateRect.setX(int((selection.x() - 1.0) / float(currentZoom)));
			updateRect.setY(int((selection.y() - 1.0) / float(currentZoom)));
			updateRect.setWidth(int((selection.width() + 2.0) / float(currentZoom)));
			updateRect.setHeight(int((selection.height() + 2.0) / float(currentZoom)));
			waitevents = true;
			if (!ignoreevents)
			{
				tempLayerData_AutoCalc = tempLayerData_NoAutoCalc = (*mapRef)->getLayerData(currentLayer)->tileData;
				for (int i = 0; i < undoHistory.size(); ++i)
				{
					undoHistory[i].oldData.clear();
					undoHistory[i].newData.clear();
				}
				undoHistory.clear();
				UndoEvent undoEvent;
				undoEvent.oldData = undoEvent.newData = tempLayerData_AutoCalc;
				undoEvent.layer = currentLayer;
				undoHistory = QList<UndoEvent>();
				undoHistory.append(undoEvent);
				undoLocation = 0;
				mapWidth = (*mapRef)->getWidth();
				mapHeight = (*mapRef)->getHeight();
				if (currentTool == TOOL_TILING)
				{
					updateRect.setX(int(mapToScene(0, 0).x()) / ZOOM_3TO2);
					updateRect.setY(int(mapToScene(0, 0).y()) / ZOOM_3TO2);
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
					updateRect.setWidth((x / ZOOM_3TO2) + 2);
					updateRect.setHeight((y / ZOOM_3TO2) + 2);
					mapViewImage = QImage(updateRect.width() * ZOOM_3TO2, updateRect.height() * ZOOM_3TO2, QImage::Format_ARGB32);
					setSceneRect(0.0, 0.0, float(mapWidth * ZOOM_3TO2), float(mapHeight * ZOOM_3TO2));
				}
				else
				{
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
					mapViewImage = QImage(updateRect.width() * currentZoom, updateRect.height() * currentZoom, QImage::Format_ARGB32);
					setSceneRect(0.0, 0.0, float(mapWidth * currentZoom), float(mapHeight * currentZoom));
				}
				ensureVisible(QRect(0.0, 0.0, 32.0, 32.0), 0, 0);
				updateMapView();
			}
			waitevents = false;
		}
	}
}

void MapView::changeEditingMode(int newMode)
{
	if (newMode != currentEditingMode && !shutdown)
	{
		currentEditingMode = newMode;
		waitevents = true;
		if (!ignoreevents)
		{
			if (currentEditingMode == EDITMODE_LAYERS)
			{
				aNewEvent->setShortcut(QKeySequence());
				aEditEvent->setShortcut(QKeySequence());
				aCutEvent->setShortcut(QKeySequence());
				aCopyEvent->setShortcut(QKeySequence());
				aPasteEvent->setShortcut(QKeySequence());
				aDeleteEvent->setShortcut(QKeySequence());
				aCopy->setShortcuts(QKeySequence::Copy);
				aCut->setShortcuts(QKeySequence::Cut);
				aPaste->setShortcuts(QKeySequence::Paste);
				aDelete->setShortcuts(QKeySequence::Delete);
				ignoreContextMenu = true;
				if (currentTool == TOOL_TILING)
				{
					currentSelection->setVisible(false);
					setCursor(Qt::ArrowCursor);
				}
				else
				{
					currentSelection->setRect(1.0, 1.0, (tileSelectionWidth * float(currentZoom)) - 2.0, (tileSelectionHeight * float(currentZoom)) - 2.0);
					currentSelection->setVisible(false);
					if (currentTool == TOOL_FILL || currentTool == TOOL_PENCIL)
						setCursor(Qt::ArrowCursor);
					else
						setCursor(Qt::CrossCursor);
				}
				tempLayerData_AutoCalc = tempLayerData_NoAutoCalc = (*mapRef)->getLayerData(currentLayer)->tileData;
			}
			else if (currentEditingMode == EDITMODE_PREVIEW)
			{
				ignoreContextMenu = true;
				currentSelection->setVisible(false);
				setCursor(Qt::ArrowCursor);
			}
			else if (currentEditingMode == EDITMODE_EVENTS)
			{
				aCopy->setShortcut(QKeySequence());
				aCut->setShortcut(QKeySequence());
				aPaste->setShortcut(QKeySequence());
				aDelete->setShortcut(QKeySequence());
				aNewEvent->setShortcut(QKeySequence());
				aEditEvent->setShortcut(QKeySequence());
				aCutEvent->setShortcuts(QKeySequence::Cut);
				aCopyEvent->setShortcuts(QKeySequence::Copy);
				aPasteEvent->setShortcuts(QKeySequence::Paste);
				aDeleteEvent->setShortcuts(QKeySequence::Delete);
				ignoreContextMenu = false;
				currentSelection->setVisible(false);
				setCursor(Qt::ArrowCursor);
				rectStart = QPoint(-1, -1);
			}
			updateMapView();
		}
		waitevents = false;
	}
	currentEditingMode = newMode;
}

void MapView::changeLayer(int newLayer)
{
	if (newLayer != currentLayer && !shutdown)
	{
		currentLayer = newLayer;
		waitevents = true;
		if (!ignoreevents)
		{
			tempLayerData_AutoCalc = tempLayerData_NoAutoCalc = (*mapRef)->getLayerData(currentLayer)->tileData;
			updateMapView();
		}
		waitevents = false;
	}
	currentLayer = newLayer;
}

void MapView::changeZoom(int newZoom)
{
	if (newZoom != currentZoom && !shutdown)
	{
		QRect updateRect;
		QRectF selection;
		int x, y;
		waitevents = true;
		selection = currentSelection->rect();
		updateRect.setX(int((selection.x() - 1.0) / float(currentZoom)));
		updateRect.setY(int((selection.y() - 1.0) / float(currentZoom)));
		updateRect.setWidth(int((selection.width() + 2.0) / float(currentZoom)));
		updateRect.setHeight(int((selection.height() + 2.0) / float(currentZoom)));
		currentZoom = newZoom;
		setSceneRect(0.0, 0.0, float(mapWidth * currentZoom), float(mapHeight * currentZoom));
		currentSelection->setRect((updateRect.x() * float(currentZoom)) + 1.0, (updateRect.y() * float(currentZoom)) + 1.0, (updateRect.width() * float(currentZoom)) - 2.0, (updateRect.height() * float(currentZoom)) - 2.0);
		horizontalScrollBar()->setSingleStep(currentZoom / 2);
		verticalScrollBar()->setSingleStep(currentZoom / 2);
		QCoreApplication::processEvents();
		if (!ignoreevents)
		{
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
			mapViewImage = QImage(updateRect.width() * currentZoom, updateRect.height() * currentZoom, QImage::Format_ARGB32);
			updateMapView();
		}
		waitevents = false;
	}
	currentZoom = newZoom;
}

void MapView::changeTool(int newTool)
{
	if (currentTool != newTool && !shutdown)
	{
		currentTool = newTool;
		currentSelection->setVisible(false);
		if (!ignoreevents)
		{
			selectionTimer->stop();
			selectionDashOffset = 0;
			selectionPen.setDashOffset(selectionDashOffset);
			currentSelection->setPen(selectionPen);
			if (currentTool == TOOL_SELECTION && currentEditingMode == EDITMODE_LAYERS)
			{
				selectionRect.setX(0);
				selectionRect.setY(0);
				selectionRect.setWidth(0);
				selectionRect.setHeight(0);
				currentSelection->setRect(1.0, 1.0, (tileSelectionWidth * float(currentZoom)) - 2.0, (tileSelectionHeight * float(currentZoom)) - 2.0);
				selectionTimer->start(100);
			}
			if (currentEditingMode == EDITMODE_LAYERS)
			{
				if (currentTool == TOOL_PENCIL || currentTool == TOOL_FILL || currentTool == TOOL_TILING)
					setCursor(Qt::ArrowCursor);
				else
					setCursor(Qt::CrossCursor);
			}
			else
				setCursor(Qt::ArrowCursor);
		}
	}
	currentTool = newTool;
}

void MapView::changePhase(int newPhase)
{
	if (newPhase != currentPhase && !shutdown)
	{
		currentPhase = newPhase;
		waitevents = true;
		if (!ignoreevents && currentEditingMode == EDITMODE_PASSABILITY)
			updateMapView();
		waitevents = false;
	}
	currentPhase = newPhase;
}

void MapView::setShowGrid(bool show)
{
	showGrid = show;
	if (!shutdown)
		updateMapView();
}

void MapView::setPassabilityMode(int mode)
{
	passabilityMode = mode;
	updateMapView();
}

int MapView::getCurrentMap() const
{
	if (mapRef != NULL)
		return mapRef->getDataID();
	return -1;
}

int MapView::getCurrentTileSelectionWidth() const
{
	return tileSelectionWidth;
}

int MapView::getCurrentTileSelectionHeight() const
{
	return tileSelectionHeight;
}

QList<Map::TileData> MapView::getCurrentTileSelection() const
{
	return tileSelection;
}

int MapView::getCurrentEditingMode() const
{
	return currentEditingMode;
}

int MapView::getCurrentLayer() const
{
	return currentLayer;
}

int MapView::getCurrentZoom() const
{
	return currentZoom;
}

int MapView::getCurrentTool() const
{
	return currentTool;
}

int MapView::getCurrentPhase() const
{
	return currentPhase;
}

int MapView::getMapWidth() const
{
	return (*mapRef)->getWidth();
}

int MapView::getMapHeight() const
{
	return (*mapRef)->getHeight();
}

void MapView::savePreview(QString filelocation) const
{
	QImage previewImage(mapWidth * 32, mapHeight * 32, QImage::Format_RGB32);
	QPainter painter;
	QImage invalidTile(":/icons/invalidtile.png");
	QImage invalidTileset(":/icons/invalidtileset.png");
	QPoint location;
	Map::TileData tileData;
	int x, y;
	int numLayers = (*mapRef)->numLayers();
	int numEvents = (*mapRef)->numEvents();
	painter.begin(&previewImage);
	painter.setPen(QPen(Qt::NoPen));
	painter.setBrush(QBrush(Qt::magenta));
	painter.fillRect(0, 0, previewImage.width(), previewImage.height(), Qt::magenta);
	for (int i = 0; i < numLayers; ++i)
	{
		if ((*mapRef)->isLayerVisible(i))
		{
			for (y = 0; y < mapHeight; ++y)
			{
				for (x = 0; x < mapWidth; ++x)
				{
					painter.setOpacity(1.0);
					tileData = (*mapRef)->getTileData(x, y, i);
					if (tileData.tilesetID >= tilesets.size())
						painter.drawImage(x * 32, y * 32, invalidTileset);
					else if (tileData.tileID >= (*tilesets[tileData.tilesetID])->numTiles())
						painter.drawImage(x * 32, y * 32, invalidTile);
					else
						(*tilesets[tileData.tilesetID])->getTile(tileData.tileID)->drawTile(painter, QPoint(x * 32, y * 32), tileData.tileFlags);
				}
			}
			for (int j = 0; j < numEvents; ++j)
			{
				MapEvent *event = (*mapRef)->getEvent(j);
				MapEvent::Page *page = event->getPage(0);
				if (page->getLayer() == i)
				{
					QImage eventImage = page->getImage(MapEvent::DIRECTION_DOWN, MapEvent::POSE_IDLE);
					x = event->getXLocation();
					y = event->getYLocation();
					if (eventImage.height() == 64)
						painter.drawImage(x * 32 - 8, (y * 32) - 32, eventImage);
					else
						painter.drawImage(x * 32, y * 32, eventImage);
				}
			}
		}
	}
	painter.end();
	previewImage.save(filelocation);
}

void MapView::changeTileSelection(int selectionWidth, int selectionHeight, QList<Map::TileData> selectionData)
{
	tileSelectionWidth = selectionWidth;
	tileSelectionHeight = selectionHeight;
	tileSelection = selectionData;
	if (currentTool != TOOL_TILING && currentTool != TOOL_SELECTION && currentEditingMode != EDITMODE_EVENTS && currentEditingMode != EDITMODE_PREVIEW)
		currentSelection->setRect(1.0, 1.0, (tileSelectionWidth * float(currentZoom)) - 2.0, (tileSelectionHeight * float(currentZoom)) - 2.0);
}

void MapView::scrollArea()
{
	QScrollBar *hScrollBar = horizontalScrollBar();
	QScrollBar *vScrollBar = verticalScrollBar();
	if (scrollEvent.x() == -1)
		hScrollBar->triggerAction(QAbstractSlider::SliderSingleStepSub);
	else if (scrollEvent.x() == 1)
		hScrollBar->triggerAction(QAbstractSlider::SliderSingleStepAdd);
	if (scrollEvent.y() == -1)
		vScrollBar->triggerAction(QAbstractSlider::SliderSingleStepSub);
	else if (scrollEvent.y() == 1)
		vScrollBar->triggerAction(QAbstractSlider::SliderSingleStepAdd);
}

void MapView::scrollBarMoved()
{
	if (!ignoreevents && !waitevents)
		updateMapView();
}

void MapView::undo()
{
	if (undoLocation > 0 || (undoLocation == 0 && !decrementUndoLocation))
	{
		if (decrementUndoLocation)
			--undoLocation;
		decrementUndoLocation = true;
		incrementUndoLocation = false;
		(*mapRef)->getLayerData(undoHistory[undoLocation].layer)->tileData = undoHistory[undoLocation].oldData;
		if (undoHistory[undoLocation].layer == currentLayer)
			tempLayerData_AutoCalc = tempLayerData_NoAutoCalc = undoHistory[undoLocation].oldData;
		if (undoLocation == 0)
			undoEnabled(false);
		redoEnabled(true);
		ProjectData::saved = false;
		updateMapView();
		mapRef->saveToStorage();
	}
}

void MapView::redo()
{
	if (undoLocation < undoHistory.size() - 1 || (undoLocation == undoHistory.size() - 1 && !incrementUndoLocation))
	{
		if (incrementUndoLocation)
			++undoLocation;
		decrementUndoLocation = false;
		incrementUndoLocation = true;
		(*mapRef)->getLayerData(undoHistory[undoLocation].layer)->tileData = undoHistory[undoLocation].newData;
		if (undoHistory[undoLocation].layer == currentLayer)
			tempLayerData_AutoCalc = tempLayerData_NoAutoCalc = undoHistory[undoLocation].newData;
		if (undoLocation == undoHistory.size() - 1)
			redoEnabled(false);
		undoEnabled(true);
		ProjectData::saved = false;
		updateMapView();
		mapRef->saveToStorage();
	}
}

void MapView::animateSelection()
{
	if (!selectionData.anchored)
	{
		selectionDashOffset = (selectionDashOffset + 1) % 6;
		selectionPen.setDashOffset(5 - selectionDashOffset);
		currentSelection->setPen(selectionPen);
	}
	else
	{
		selectionDashOffset = 0;
		selectionPen.setDashOffset(5);
		currentSelection->setPen(selectionPen);
	}
}

void MapView::copySelection()
{
	if (currentEditingMode == EDITMODE_EVENTS)
	{
		eventID = (*mapRef)->eventIDAtLocation(rectStart);
		if (eventID != -1)
		{
			Clipboard::clear();
			Clipboard::dataType = Clipboard::TYPE_MAPEVENT;
			Clipboard::data.mapEvent = new MapEvent((*mapRef)->getEvent(eventID));
		}
	}
	else if (currentTool == TOOL_SELECTION && !selectionRect.isNull())
	{
		Clipboard::clear();
		Clipboard::dataType = Clipboard::TYPE_TILEDATA;
		Clipboard::data.tileData.data = new QList<Map::TileData>();
		Clipboard::data.tileData.width = selectionRect.width();
		Clipboard::data.tileData.height = selectionRect.height();
		for (int y = 0; y < selectionRect.height(); ++y)
		{
			for (int x = 0; x < selectionRect.width(); ++x)
			{
				int location = (y + selectionRect.y()) * mapWidth + (x + selectionRect.x());
				Clipboard::data.tileData.data->append(tempLayerData_AutoCalc[location]);
			}
		}
	}
	clearSelection();
}

void MapView::cutSelection()
{
	if (currentEditingMode == EDITMODE_EVENTS)
	{
		eventID = (*mapRef)->eventIDAtLocation(rectStart);
		if (eventID != -1)
		{
			MapEvent *event = (*mapRef)->getEvent(eventID);
			QPoint updatePoint = QPoint(event->getXLocation(), event->getYLocation());
			Clipboard::clear();
			Clipboard::dataType = Clipboard::TYPE_MAPEVENT;
			Clipboard::data.mapEvent = new MapEvent(event);
			(*mapRef)->deleteEvent(eventID);
			updateMapView(updatePoint);
		}
	}
	else if (currentTool == TOOL_SELECTION && !selectionRect.isNull())
	{
		Clipboard::clear();
		Clipboard::dataType = Clipboard::TYPE_TILEDATA;
		Clipboard::data.tileData.data = new QList<Map::TileData>();
		Clipboard::data.tileData.width = selectionRect.width();
		Clipboard::data.tileData.height = selectionRect.height();
		for (int y = 0; y < selectionRect.height(); ++y)
		{
			for (int x = 0; x < selectionRect.width(); ++x)
			{
				int location = (y + selectionRect.y()) * mapWidth + (x + selectionRect.x());
				int tileSelectionLocation = (y % tileSelectionHeight) * tileSelectionWidth + (x % tileSelectionWidth);
				Clipboard::data.tileData.data->append(tempLayerData_AutoCalc[location]);
				tempLayerData_AutoCalc[location] = tileSelection[tileSelectionLocation];
				tempLayerData_NoAutoCalc[location] = tileSelection[tileSelectionLocation];
			}
		}
		autoCalcFlags(selectionRect, true);
		applyTempLayerData();
		updateMapView(QRect(selectionRect.x() - 1, selectionRect.y() - 1, selectionRect.width() + 2, selectionRect.height() + 2));
		ProjectData::saved = false;
	}
	clearSelection();
}

void MapView::pasteSelection()
{
	if (currentEditingMode == EDITMODE_EVENTS)
	{
		if (Clipboard::dataType == Clipboard::TYPE_MAPEVENT && rectStart.x() != -1 && rectStart.y() != -1 && !(*mapRef)->eventAtLocation(rectStart))
		{
			MapEvent *event = new MapEvent(Clipboard::data.mapEvent);
			event->setLocation(rectStart.x(), rectStart.y());
			(*mapRef)->addEvent(event);
			updateMapView(rectStart);
		}
	}
	else if (currentTool == TOOL_SELECTION && Clipboard::dataType == Clipboard::TYPE_TILEDATA)
	{
		QRect viewRect;
		QPoint pasteLocation = QPoint(-1, -1);
		viewRect.setLeft(((int)mapToScene(0, 0).x()) / currentZoom);
		viewRect.setRight(((int)mapToScene(width(), 0).x()) / currentZoom);
		viewRect.setTop(((int)mapToScene(0, 0).y()) / currentZoom);
		viewRect.setBottom(((int)mapToScene(0, height()).y()) / currentZoom);
		if (viewRect.left() < 0)
			viewRect.setLeft(0);
		if (viewRect.right() >= mapWidth)
			viewRect.setRight(mapWidth - 1);
		if (viewRect.top() < 0)
			viewRect.setTop(0);
		if (viewRect.bottom() >= mapHeight)
			viewRect.setBottom(mapHeight - 1);
		if (rectStart.x() != -1 && rectStart.y() != -1)
			pasteLocation = rectStart;
		else if (selectionRect.width() > 0 && selectionRect.height() > 0)
			pasteLocation = selectionRect.topLeft();
		else
			pasteLocation = QPoint(((viewRect.width() - Clipboard::data.tileData.width) / 2) + viewRect.x(), ((viewRect.height() - Clipboard::data.tileData.height) / 2) + viewRect.y());
		selectionData.tileData.clear();
		selectionData.width = Clipboard::data.tileData.width;
		selectionData.height = Clipboard::data.tileData.height;
		selectionData.x = pasteLocation.x();
		selectionData.y = pasteLocation.y();
		selectionData.anchored = false;
		selectionRect.setRect(selectionData.x, selectionData.y, selectionData.width, selectionData.height);
		currentSelection->setRect((selectionRect.x() * float(currentZoom)) + 1.0, (selectionRect.y() * float(currentZoom)) + 1.0, (selectionRect.width() * float(currentZoom)) - 2.0, (selectionRect.height() * float(currentZoom)) - 2.0);
		for (int y = 0; y < selectionData.height; ++y)
		{
			for (int x = 0; x < selectionData.width; ++x)
			{
				int location = y * selectionData.width + x;
				selectionData.tileData.append(Clipboard::data.tileData.data->at(location));
			}
		}
		currentSelection->setVisible(true);
		updateMapView(QRect(selectionRect.x(), selectionRect.y(), selectionRect.width(), selectionRect.height()));
	}
}

void MapView::deleteSelection()
{
	if (currentEditingMode == EDITMODE_EVENTS)
	{
		eventID = (*mapRef)->eventIDAtLocation(rectStart);
		if (ProjectData::partyStartLocation.mapID == mapRef->getDataID() && ProjectData::partyStartLocation.x == rectStart.x() && ProjectData::partyStartLocation.y == rectStart.y())
		{
			ProjectData::partyStartLocation.mapID = -1;
			ProjectData::saved = false;
			updateMapView(QPoint(ProjectData::partyStartLocation.x, ProjectData::partyStartLocation.y));
		}
		for (int i = 0; i < ProjectData::vehicleStartLocations.size(); ++i)
		{
			if (ProjectData::vehicleStartLocations[i].mapID == mapRef->getDataID() && ProjectData::vehicleStartLocations[i].x == rectStart.x() && ProjectData::vehicleStartLocations[i].y == rectStart.y())
			{
				ProjectData::vehicleStartLocations[i].mapID = -1;
				ProjectData::saved = false;
				updateMapView(QPoint(ProjectData::vehicleStartLocations[i].x, ProjectData::vehicleStartLocations[i].y));
			}
		}
		if (eventID != -1)
		{
			MapEvent *event = (*mapRef)->getEvent(eventID);
			QPoint updatePoint = QPoint(event->getXLocation(), event->getYLocation());
			(*mapRef)->deleteEvent(eventID);
			updateMapView(updatePoint);
		}
	}
	else if (currentTool == TOOL_SELECTION && !selectionRect.isNull())
	{
		if (selectionData.anchored)
		{
			for (int y = 0; y < selectionRect.height(); ++y)
			{
				for (int x = 0; x < selectionRect.width(); ++x)
				{
					int tileSelectionLocation = (y % tileSelectionHeight) * tileSelectionWidth + (x % tileSelectionWidth);
					int location = (y + selectionRect.y()) * mapWidth + (x + selectionRect.x());
					tempLayerData_AutoCalc[location] = tileSelection[tileSelectionLocation];
					tempLayerData_NoAutoCalc[location] = tileSelection[tileSelectionLocation];
				}
			}
			autoCalcFlags(selectionRect, true);
			applyTempLayerData();
			updateMapView(QRect(selectionRect.x() - 1, selectionRect.y() - 1, selectionRect.width() + 2, selectionRect.height() + 2));
			ProjectData::saved = false;
		}
		else
		{
			selectionData.anchored = true;
			selectionData.x = 0;
			selectionData.y = 0;
			selectionData.width = 0;
			selectionData.height = 0;
			selectionData.tileData.clear();
			updateMapView();
		}
	}
	clearSelection();
}

void MapView::duplicateSelection()
{
	if (currentTool == TOOL_SELECTION && !selectionRect.isNull())
	{
		selectionData.x = selectionRect.x();
		selectionData.y = selectionRect.y();
		selectionData.width = selectionRect.width();
		selectionData.height = selectionRect.height();
		selectionData.anchored = false;
		selectionData.tileData.clear();
		for (int y = 0; y < selectionData.height; ++y)
		{
			for (int x = 0; x < selectionData.width; ++x)
			{
				int location = (y + selectionData.y) * mapWidth + (x + selectionData.x);
				selectionData.tileData.append(tempLayerData_AutoCalc[location]);
			}
		}
		updateMapView(QRect(selectionRect.x() - 1, selectionRect.y() - 1, selectionRect.width() + 2, selectionRect.height() + 2));
	}
}

void MapView::clearSelection()
{
	if (currentTool == TOOL_SELECTION && !selectionRect.isNull())
	{
		selectionRect.setX(0);
		selectionRect.setY(0);
		selectionRect.setWidth(0);
		selectionRect.setHeight(0);
		currentSelection->setVisible(false);
	}
}

void MapView::anchorSelection()
{
	if (currentTool == TOOL_SELECTION && !selectionRect.isNull())
	{
		for (int y = 0; y < selectionData.height; ++y)
		{
			for (int x = 0; x < selectionData.width; ++x)
			{
				int location = (y + selectionData.y) * mapWidth + (x + selectionData.x);
				int selectionLocation = y * selectionData.width + x;
				tempLayerData_NoAutoCalc[location] = selectionData.tileData[selectionLocation];
				tempLayerData_AutoCalc[location] = selectionData.tileData[selectionLocation];
			}
		}
		selectionData.anchored = true;
		autoCalcFlags(selectionRect, true);
		applyTempLayerData();
		rectStart = selectionRect.topLeft();
		rectEnd = selectionRect.bottomRight();
		updateMapView(QRect(selectionRect.x() - 1, selectionRect.y() - 1, selectionRect.width() + 2, selectionRect.height() + 2));
	}
}

void MapView::detachSelection()
{
	if (currentTool == TOOL_SELECTION && !selectionRect.isNull())
	{
		selectionData.x = selectionRect.x();
		selectionData.y = selectionRect.y();
		selectionData.width = selectionRect.width();
		selectionData.height = selectionRect.height();
		selectionData.anchored = false;
		selectionData.tileData.clear();
		for (int y = 0; y < selectionData.height; ++y)
		{
			for (int x = 0; x < selectionData.width; ++x)
			{
				int location = (y + selectionData.y) * mapWidth + (x + selectionData.x);
				int tileLocation = (y % tileSelectionHeight) * tileSelectionWidth + (x % tileSelectionWidth);
				selectionData.tileData.append(tempLayerData_AutoCalc[location]);
				tempLayerData_AutoCalc[location] = tileSelection[tileLocation];
				tempLayerData_NoAutoCalc[location] = tileSelection[tileLocation];
			}
		}
		autoCalcFlags(selectionRect, true);
		applyTempLayerData();
		updateMapView(QRect(selectionRect.x() - 1, selectionRect.y() - 1, selectionRect.width() + 2, selectionRect.height() + 2));
	}
}

void MapView::newEvent()
{
	if (rectStart.x() != -1 && rectStart.y() != -1)
	{
		MapEventEditor *eventEditor;
		MapEvent *event = new MapEvent;
		int eventID = (*mapRef)->numEvents();
		event->setLocation(rectStart.x(), rectStart.y());
		event->getPage(0)->setLayer(currentLayer);
		(*mapRef)->addEvent(event);
		eventEditor = new MapEventEditor(mapRef->getDataID(), eventID);
		if (eventEditor->exec())
		{
			ProjectData::saved = false;
			updateMapView(rectStart);
		}
		else
			(*mapRef)->deleteEvent(eventID);
		delete eventEditor;
	}
}

void MapView::editEvent()
{
	if (eventID != -1)
	{
		MapEventEditor *eventEditor = new MapEventEditor(mapRef->getDataID(), eventID);
		if (eventEditor->exec())
		{
			ProjectData::saved = false;
			updateMapView(rectStart);
		}
		delete eventEditor;
	}
}

void MapView::cutEvent()
{
	if (eventID != -1)
	{
		MapEvent *event = (*mapRef)->getEvent(eventID);
		QPoint updatePoint = QPoint(event->getXLocation(), event->getYLocation());
		Clipboard::clear();
		Clipboard::dataType = Clipboard::TYPE_MAPEVENT;
		Clipboard::data.mapEvent = new MapEvent(event);
		(*mapRef)->deleteEvent(eventID);
		updateMapView(updatePoint);
	}
}

void MapView::copyEvent()
{
	if (eventID != -1)
	{
		Clipboard::clear();
		Clipboard::dataType = Clipboard::TYPE_MAPEVENT;
		Clipboard::data.mapEvent = new MapEvent((*mapRef)->getEvent(eventID));
	}
}

void MapView::pasteEvent()
{
	if (Clipboard::dataType == Clipboard::TYPE_MAPEVENT && rectStart.x() != -1 && rectStart.y() != -1)
	{
		MapEvent *event = new MapEvent(Clipboard::data.mapEvent);
		event->setLocation(rectStart.x(), rectStart.y());
		(*mapRef)->addEvent(event);
		ProjectData::saved = false;
		updateMapView(rectStart);
	}
}

void MapView::deleteEvent()
{
	if (ProjectData::partyStartLocation.mapID == mapRef->getDataID() && ProjectData::partyStartLocation.x == rectStart.x() && ProjectData::partyStartLocation.y == rectStart.y())
	{
		ProjectData::partyStartLocation.mapID = -1;
		ProjectData::saved = false;
		updateMapView(QPoint(ProjectData::partyStartLocation.x, ProjectData::partyStartLocation.y));
	}
	for (int i = 0; i < ProjectData::vehicleStartLocations.size(); ++i)
	{
		if (ProjectData::vehicleStartLocations[i].mapID == mapRef->getDataID() && ProjectData::vehicleStartLocations[i].x == rectStart.x() && ProjectData::vehicleStartLocations[i].y == rectStart.y())
		{
			ProjectData::vehicleStartLocations[i].mapID = -1;
			ProjectData::saved = false;
			updateMapView(QPoint(ProjectData::vehicleStartLocations[i].x, ProjectData::vehicleStartLocations[i].y));
		}
	}
	if (eventID != -1)
	{
		MapEvent *event = (*mapRef)->getEvent(eventID);
		QPoint updatePoint = QPoint(event->getXLocation(), event->getYLocation());
		(*mapRef)->deleteEvent(eventID);
		ProjectData::saved = false;
		updateMapView(updatePoint);
	}
}

void MapView::placeTeleportEvent()
{
	if (rectStart.x() != -1 && rectStart.y() != -1)
	{
		MapEvent *mapEvent = new MapEvent;
		MapEvent::Page *page = mapEvent->getPage(0);
		BaseEvent *teleportEvent = new BaseEvent(BaseEvent::MAPEVENT, BaseEvent::TYPE_TELEPORT);
		int eventID = (*mapRef)->numEvents();
		mapEvent->setLocation(rectStart.x(), rectStart.y());
		page->setLayer(currentLayer);
		page->getEvent()->addChildEvent(teleportEvent, -1, 0);
		(*mapRef)->addEvent(mapEvent);
		if (teleportEvent->configureEvent(0, 0))
		{
			ProjectData::saved = false;
			updateMapView(rectStart);
		}
		else
			(*mapRef)->deleteEvent(eventID);
	}
}

void MapView::placePartyStartLocation()
{
	ProjectData::partyStartLocation.mapID = mapRef->getDataID();
	ProjectData::partyStartLocation.x = rectStart.x();
	ProjectData::partyStartLocation.y = rectStart.y();
	ProjectData::partyStartLocation.layer = currentLayer;
	ProjectData::saved = false;
	updateMapView(rectStart);
}

void MapView::placeVehicleStartLocation()
{
	SelectVehicle *selectVehicle = new SelectVehicle;
	if (selectVehicle->exec())
	{
		QPoint updatePoint(-1, -1);
		int vehicleID = selectVehicle->getVehicleID();
		if (ProjectData::vehicleStartLocations[vehicleID].mapID == mapRef->getDataID())
			updatePoint = QPoint(ProjectData::vehicleStartLocations[vehicleID].x, ProjectData::vehicleStartLocations[vehicleID].y);
		ProjectData::vehicleStartLocations[vehicleID].mapID = mapRef->getDataID();
		ProjectData::vehicleStartLocations[vehicleID].x = rectStart.x();
		ProjectData::vehicleStartLocations[vehicleID].y = rectStart.y();
		ProjectData::vehicleStartLocations[vehicleID].layer = currentLayer;
		ProjectData::saved = false;
		updateMapView(rectStart);
		if (updatePoint.x() != -1)
			updateMapView(updatePoint);
	}
	delete selectVehicle;
}

void MapView::moveUpLayer()
{
	if (ProjectData::partyStartLocation.mapID == mapRef->getDataID() && ProjectData::partyStartLocation.x == rectStart.x() && ProjectData::partyStartLocation.y == rectStart.y())
	{
		ProjectData::partyStartLocation.layer += 1;
		ProjectData::saved = false;
		updateMapView(QPoint(ProjectData::partyStartLocation.x, ProjectData::partyStartLocation.y));
	}
	for (int i = 0; i < ProjectData::vehicleStartLocations.size(); ++i)
	{
		if (ProjectData::vehicleStartLocations[i].mapID == mapRef->getDataID() && ProjectData::vehicleStartLocations[i].x == rectStart.x() && ProjectData::vehicleStartLocations[i].y == rectStart.y())
		{
			ProjectData::vehicleStartLocations[i].layer += 1;
			ProjectData::saved = false;
			updateMapView(QPoint(ProjectData::vehicleStartLocations[i].x, ProjectData::vehicleStartLocations[i].y));
		}
	}
}

void MapView::moveDownLayer()
{
	if (ProjectData::partyStartLocation.mapID == mapRef->getDataID() && ProjectData::partyStartLocation.x == rectStart.x() && ProjectData::partyStartLocation.y == rectStart.y())
	{
		ProjectData::partyStartLocation.layer -= 1;
		ProjectData::saved = false;
		updateMapView(QPoint(ProjectData::partyStartLocation.x, ProjectData::partyStartLocation.y));
	}
	for (int i = 0; i < ProjectData::vehicleStartLocations.size(); ++i)
	{
		if (ProjectData::vehicleStartLocations[i].mapID == mapRef->getDataID() && ProjectData::vehicleStartLocations[i].x == rectStart.x() && ProjectData::vehicleStartLocations[i].y == rectStart.y())
		{
			ProjectData::vehicleStartLocations[i].layer -= 1;
			ProjectData::saved = false;
			updateMapView(QPoint(ProjectData::vehicleStartLocations[i].x, ProjectData::vehicleStartLocations[i].y));
		}
	}
}

void MapView::cropToSelection()
{
	if (mapRef != NULL)
	{
		(*mapRef)->cropMap(selectionRect.x(), selectionRect.y(), selectionRect.width(), selectionRect.height());
		selectionRect.setX(0);
		selectionRect.setY(0);
		selectionRect.setWidth(0);
		selectionRect.setHeight(0);
		currentSelection->setVisible(false);
		ProjectData::saved = false;
		changeMap(mapRef->getDataID(), true);
	}
}

void MapView::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu *contextMenu = NULL;
	if (!ignoreevents)
	{
		QPoint oldRectStart = rectStart;
		QPointF testPoint = mapToScene(event->pos());
		rectStart.setX(((int)mapToScene(event->pos()).x()) / currentZoom);
		rectStart.setY(((int)mapToScene(event->pos()).y()) / currentZoom);
		if (testPoint.x() <= 0.0 && rectStart.x() == 0)
			rectStart.setX(-1);
		if (testPoint.y() <= 0.0 && rectStart.y() == 0)
			rectStart.setY(-1);
		if (currentEditingMode == EDITMODE_EVENTS)
		{
			if (rectStart.x() >= 0 && rectStart.x() < mapWidth && rectStart.y() >= 0 && rectStart.y() < mapHeight)
			{
				contextMenu = new QMenu(this);
				int layer = ProjectData::partyStartLocation.layer;
				bool foundOne = false;
				if (ProjectData::partyStartLocation.mapID == mapRef->getDataID() && ProjectData::partyStartLocation.x == rectStart.x() && ProjectData::partyStartLocation.y == rectStart.y())
					foundOne = true;
				for (int i = 0; i < ProjectData::vehicleStartLocations.size() && !foundOne; ++i)
				{
					layer = ProjectData::vehicleStartLocations[i].layer;
					if (ProjectData::vehicleStartLocations[i].mapID == mapRef->getDataID() && ProjectData::vehicleStartLocations[i].x == rectStart.x() && ProjectData::vehicleStartLocations[i].y == rectStart.y())
						foundOne = true;
				}
				if (!foundOne)
				{
					if ((*mapRef)->eventAtLocation(rectStart))
					{
						aNewEvent->setEnabled(false);
						aEditEvent->setEnabled(true);
						aCutEvent->setEnabled(true);
						aCopyEvent->setEnabled(true);
						aPasteEvent->setEnabled(false);
						aDeleteEvent->setEnabled(true);
						aPlaceTeleportEvent->setEnabled(false);
						aPlacePartyStartLocation->setEnabled(false);
						aPlaceVehicleStartLocation->setEnabled(false);
						aMoveUpLayer->setEnabled(false);
						aMoveDownLayer->setEnabled(false);
						contextMenu->addAction(aEditEvent);
					}
					else
					{
						aNewEvent->setEnabled(true);
						aEditEvent->setEnabled(false);
						aCutEvent->setEnabled(false);
						aCopyEvent->setEnabled(false);
						if (Clipboard::dataType == Clipboard::TYPE_MAPEVENT)
							aPasteEvent->setEnabled(true);
						else
							aPasteEvent->setEnabled(false);
						aDeleteEvent->setEnabled(false);
						aPlaceTeleportEvent->setEnabled(true);
						aPlacePartyStartLocation->setEnabled(true);
						aPlaceVehicleStartLocation->setEnabled(true);
						contextMenu->addAction(aNewEvent);
					}
					contextMenu->addSeparator();
					contextMenu->addAction(aCutEvent);
					contextMenu->addAction(aCopyEvent);
					contextMenu->addAction(aPasteEvent);
					contextMenu->addAction(aDeleteEvent);
					contextMenu->addSeparator();
					contextMenu->addAction(aPlaceTeleportEvent);
					contextMenu->addSeparator();
					contextMenu->addAction(aPlacePartyStartLocation);
					contextMenu->addAction(aPlaceVehicleStartLocation);
					if ((*mapRef)->eventAtLocation(rectStart))
					{
						aNewEvent->setShortcut(QKeySequence());
						aEditEvent->setShortcut(QKeySequence(Qt::Key_Space));
					}
					else
					{
						aNewEvent->setShortcut(QKeySequence(Qt::Key_Space));
						aEditEvent->setShortcut(QKeySequence());
					}
				}
				else
				{
					if (layer == (*mapRef)->numLayers() - 1)
						aMoveUpLayer->setEnabled(false);
					else
						aMoveUpLayer->setEnabled(true);
					if (layer == 0)
						aMoveDownLayer->setEnabled(false);
					else
						aMoveDownLayer->setEnabled(true);
					contextMenu->addAction(aDeleteEvent);
					contextMenu->addSeparator();
					contextMenu->addAction(aMoveUpLayer);
					contextMenu->addAction(aMoveDownLayer);
					aDeleteEvent->setEnabled(true);
				}
				if (oldRectStart.x() != -1 && oldRectStart.y() != -1)
					updateMapView(oldRectStart);
				if (rectStart.x() != -1 && rectStart.y() != -1)
					updateMapView(rectStart);
			}
		}
		else if (currentEditingMode == EDITMODE_LAYERS && currentTool == TOOL_SELECTION)
		{
			contextMenu = new QMenu(this);
			contextMenu->addAction(aCopy);
			contextMenu->addAction(aCut);
			contextMenu->addAction(aPaste);
			contextMenu->addAction(aDuplicate);
			contextMenu->addSeparator();
			contextMenu->addAction(aDelete);
			contextMenu->addAction(aClear);
			contextMenu->addSeparator();
			contextMenu->addAction(aAnchor);
			contextMenu->addAction(aDetach);
			contextMenu->addSeparator();
			contextMenu->addAction(aCropToSelection);
			if (Clipboard::dataType == Clipboard::TYPE_TILEDATA)
				aPaste->setEnabled(true);
			else
				aPaste->setEnabled(false);
			if (selectionRect.width() > 0 && selectionRect.height() > 0)
			{
				aCopy->setEnabled(true);
				aCut->setEnabled(true);
				aDelete->setEnabled(true);
				aDuplicate->setEnabled(true);
				aClear->setEnabled(true);
				aCropToSelection->setEnabled(true);
				if (!selectionData.anchored)
				{
					aClear->setEnabled(false);
					aAnchor->setEnabled(true);
					aDetach->setEnabled(false);
					aCropToSelection->setEnabled(false);
				}
				else
				{
					aClear->setEnabled(true);
					aAnchor->setEnabled(false);
					aDetach->setEnabled(true);
				}
			}
			else
			{
				aCopy->setEnabled(false);
				aCut->setEnabled(false);
				aDelete->setEnabled(false);
				aDuplicate->setEnabled(false);
				aClear->setEnabled(false);
				aDetach->setEnabled(false);
				aAnchor->setEnabled(false);
				aCropToSelection->setEnabled(false);
			}
		}
		if (contextMenu != NULL)
			contextMenu->exec(event->globalPos());
	}
	else
		event->ignore();
}

void MapView::keyPressEvent(QKeyEvent *event)
{
	if (!ignoreevents)
	{
		rectStart = QPoint(-1, -1);
		if (event->key() == Qt::Key_Shift && currentEditingMode == EDITMODE_LAYERS && currentTool != TOOL_SELECTION && currentTool != TOOL_FILL && currentTool != TOOL_TILING)
		{
			autocalc = false;
			updateMapView();
			event->accept();
		}
		else if (event->matches(QKeySequence::Undo))
		{
			if (QApplication::mouseButtons() == Qt::NoButton && undoLocation > 0)
				undo();
			event->accept();
		}
		else if (event->matches(QKeySequence::Redo))
		{
			if (QApplication::mouseButtons() == Qt::NoButton && undoLocation < undoHistory.size())
				redo();
			event->accept();
		}
		else if (event->matches(QKeySequence::SelectAll) && currentTool == TOOL_SELECTION)
		{
			selectionRect.setX(0);
			selectionRect.setY(0);
			selectionRect.setWidth(mapWidth);
			selectionRect.setHeight(mapHeight);
			currentSelection->setRect(1.0, 1.0, (mapWidth * float(currentZoom)) - 2.0, (mapHeight * float(currentZoom)) - 2.0);
			currentSelection->setVisible(true);
			event->accept();
		}
		else if (event->matches(QKeySequence::Paste) && currentTool == TOOL_SELECTION)
			pasteSelection();
		else if (event->matches(QKeySequence::Copy) && currentEditingMode == EDITMODE_EVENTS)
			copyEvent();
		else if (event->matches(QKeySequence::Cut) && currentEditingMode == EDITMODE_EVENTS)
			cutEvent();
		else if (event->matches(QKeySequence::Paste) && currentEditingMode == EDITMODE_EVENTS)
			pasteEvent();
		else if (event->matches(QKeySequence::Delete) && currentEditingMode == EDITMODE_EVENTS)
			deleteEvent();
		else if (event->key() == Qt::Key_Space && currentEditingMode == EDITMODE_EVENTS)
		{
			if ((*mapRef)->eventAtLocation(rectStart))
				editEvent();
			else
				newEvent();
		}
		else
			QGraphicsView::keyPressEvent(event);
	}
	else
		event->ignore();
}

void MapView::keyReleaseEvent(QKeyEvent *event)
{
	if (!ignoreevents)
	{
		if (event->key() == Qt::Key_Shift && currentEditingMode == EDITMODE_LAYERS && currentTool != TOOL_SELECTION && currentTool != TOOL_FILL && currentTool != TOOL_TILING)
		{
			autocalc = true;
			updateMapView();
			event->accept();
		}
		else
			QGraphicsView::keyReleaseEvent(event);
	}
	else
		event->ignore();
}

void MapView::leaveEvent(QEvent *event)
{
	Q_UNUSED(event);
	if (currentTool != TOOL_SELECTION)
		currentSelection->setVisible(false);
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (!ignoreevents)
	{
		QPoint tileLocation;
		QPointF testPoint;
		tileLocation.setX(int(mapToScene(event->x(), event->y()).x()) / currentZoom);
		tileLocation.setY(int(mapToScene(event->x(), event->y()).y()) / currentZoom);
		testPoint = mapToScene(event->pos());
		if (testPoint.x() < 0.0 && tileLocation.x() == 0)
			tileLocation.setX(-1);
		if (testPoint.y() < 0.0 && tileLocation.y() == 0)
			tileLocation.setY(-1);
		if (currentEditingMode == EDITMODE_EVENTS)
		{
			if ((*mapRef)->eventAtLocation(tileLocation))
				editEvent();
			else
				newEvent();
		}
		else if (currentEditingMode == EDITMODE_LAYERS && currentTool == TOOL_SELECTION && selectionRect.contains(tileLocation))
		{
			if (selectionData.anchored)
				detachSelection();
			else
				anchorSelection();
		}
		mouseDoubleClicked(event);
	}
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
	if (!ignoreevents)
	{
		QPoint tileLocation, sceneCoordinates;
		int buttons = event->buttons();
		int x = event->x();
		int y = event->y();
		if (currentEditingMode != EDITMODE_PREVIEW)
		{
			sceneCoordinates.setX(int(mapToScene(x, y).x()) / currentZoom);
			sceneCoordinates.setY(int(mapToScene(x, y).y()) / currentZoom);
			tileLocation = sceneCoordinates;
			if (tileLocation.x() < 0)
				tileLocation.setX(0);
			else if (tileLocation.x() >= mapWidth)
				tileLocation.setX(mapWidth - 1);
			if (tileLocation.y() < 0)
				tileLocation.setY(0);
			else if (tileLocation.y() >= mapHeight)
				tileLocation.setY(mapHeight - 1);
			if ((buttons & Qt::LeftButton) != 0 && (buttons & Qt::RightButton) == 0 && !canceled)
			{
				if (x < 0)
					scrollEvent.setX(-1);
				else if (x >= width())
					scrollEvent.setX(1);
				else
					scrollEvent.setX(0);
				if (y < 0)
					scrollEvent.setY(-1);
				else if (y >= height())
					scrollEvent.setY(1);
				else
					scrollEvent.setY(0);
				if (currentEditingMode != EDITMODE_EVENTS)
				{
					if (currentTool == TOOL_SELECTION)
					{
						if (selectionData.anchored && tileLocation != rectEnd)
						{
							rectEnd = tileLocation;
							if (rectStart.x() > rectEnd.x())
							{
								selectionRect.setLeft(rectEnd.x());
								selectionRect.setRight(rectStart.x());
							}
							else
							{
								selectionRect.setLeft(rectStart.x());
								selectionRect.setRight(rectEnd.x());
							}
							if (rectStart.y() > rectEnd.y())
							{
								selectionRect.setTop(rectEnd.y());
								selectionRect.setBottom(rectStart.y());
							}
							else
							{
								selectionRect.setTop(rectStart.y());
								selectionRect.setBottom(rectEnd.y());
							}
							currentSelection->setRect((selectionRect.x() * float(currentZoom)) + 1.0, (selectionRect.y() * float(currentZoom)) + 1.0, (selectionRect.width() * float(currentZoom)) - 2.0, (selectionRect.height() * float(currentZoom)) - 2.0);
						}
						else if (!selectionData.anchored)
						{
							selectionData.x = tileLocation.x() - rectStart.x();
							selectionData.y = tileLocation.y() - rectStart.y();
							if (selectionData.x < 0)
								selectionData.x = 0;
							if (selectionData.x + selectionData.width > mapWidth)
								selectionData.x = mapWidth - selectionData.width;
							if (selectionData.y < 0)
								selectionData.y = 0;
							if (selectionData.y + selectionData.height > mapHeight)
								selectionData.y = mapHeight - selectionData.height;
							selectionRect.setRect(selectionData.x, selectionData.y, selectionData.width, selectionData.height);
							currentSelection->setRect((selectionRect.x() * float(currentZoom)) + 1.0, (selectionRect.y() * float(currentZoom)) + 1.0, (selectionRect.width() * float(currentZoom)) - 2.0, (selectionRect.height() * float(currentZoom)) - 2.0);
							updateMapView();
						}
					}
					else if (currentTool == TOOL_RECTANGLE && sceneCoordinates != rectEnd)
					{
						QRect oldRect, newRect, totalRect;
						oldRect = subRectData();
						rectEnd = sceneCoordinates;
						newRect = addRectData();
						if (oldRect.left() < newRect.left())
							totalRect.setLeft(oldRect.left());
						else
							totalRect.setLeft(newRect.left());
						if (oldRect.right() > newRect.right())
							totalRect.setRight(oldRect.right());
						else
							totalRect.setRight(newRect.right());
						if (oldRect.top() < newRect.top())
							totalRect.setTop(oldRect.top());
						else
							totalRect.setTop(newRect.top());
						if (oldRect.bottom() > newRect.bottom())
							totalRect.setBottom(oldRect.bottom());
						else
							totalRect.setBottom(newRect.bottom());
						updateMapView(totalRect);
					}
					else if (currentTool == TOOL_CIRCLE && sceneCoordinates != rectEnd)
					{
						QRect oldRect, newRect, totalRect;
						oldRect = subRectData();
						rectEnd = sceneCoordinates;
						newRect = addCircleData();
						if (oldRect.left() < newRect.left())
							totalRect.setLeft(oldRect.left());
						else
							totalRect.setLeft(newRect.left());
						if (oldRect.right() > newRect.right())
							totalRect.setRight(oldRect.right());
						else
							totalRect.setRight(newRect.right());
						if (oldRect.top() < newRect.top())
							totalRect.setTop(oldRect.top());
						else
							totalRect.setTop(newRect.top());
						if (oldRect.bottom() > newRect.bottom())
							totalRect.setBottom(oldRect.bottom());
						else
							totalRect.setBottom(newRect.bottom());
						updateMapView(totalRect);
					}
					else if (currentTool == TOOL_PENCIL)
					{
						for (int y = 0; y < tileSelectionHeight; ++y)
						{
							for (int x = 0; x < tileSelectionWidth; ++x)
							{
								QPoint location(tileLocation.x() + x, tileLocation.y() + y);
								if (currentEditingMode == EDITMODE_LAYERS)
								{
									QPoint tileSelectionLocation;
									tileSelectionLocation.setX((tileLocation.x() + x - rectStart.x()) % tileSelectionWidth);
									tileSelectionLocation.setY((tileLocation.y() + y - rectStart.y()) % tileSelectionHeight);
									if (tileSelectionLocation.x() < 0)
										tileSelectionLocation.setX(tileSelectionLocation.x() + tileSelectionWidth);
									if (tileSelectionLocation.y() < 0)
										tileSelectionLocation.setY(tileSelectionLocation.y() + tileSelectionHeight);
									if (location.x() < mapWidth && location.y() < mapHeight)
									{
										tempLayerData_AutoCalc[location.y() * mapWidth + location.x()] = tileSelection[tileSelectionLocation.y() * tileSelectionWidth + tileSelectionLocation.x()];
										tempLayerData_NoAutoCalc[location.y() * mapWidth + location.x()] = tileSelection[tileSelectionLocation.y() * tileSelectionWidth + tileSelectionLocation.x()];
									}
								}
								else if (currentEditingMode == EDITMODE_PASSABILITY)
								{
									int index = location.y() * mapWidth + location.x();
									if (passabilityMode == PASSMODE_SET)
									{
										tempLayerData_AutoCalc[index].phasePassability[currentPhase] = true;
										tempLayerData_NoAutoCalc[index].phasePassability[currentPhase] = true;
									}
									else if (passabilityMode == PASSMODE_CLEAR)
									{
										tempLayerData_AutoCalc[index].phasePassability[currentPhase] = false;
										tempLayerData_NoAutoCalc[index].phasePassability[currentPhase] = false;
									}
									else if (passabilityMode == PASSMODE_DIRECTIONS)
									{
										int localX = (int)(mapToScene(event->x(), event->y()).x()) % currentZoom;
										int localY = (int)(mapToScene(event->x(), event->y()).y()) % currentZoom;
										unsigned char phaseDirections = tempLayerData_AutoCalc[index].phaseDirections[currentPhase];
										if (localX >= 8 && localX <= 23 && localY >= 0 && localY <= 7)
										{
											if (tilingMode == TILING_ON)
												phaseDirections |= PASS_UP;
											else if ((phaseDirections & PASS_UP) != 0 && tilingMode == TILING_OFF)
												phaseDirections -= PASS_UP;
										}
										else if (localX >= 24 && localX <= 31 && localY >= 8 && localY <= 23)
										{
											if (tilingMode == TILING_ON)
												phaseDirections |= PASS_RIGHT;
											else if ((phaseDirections & PASS_RIGHT) != 0 && tilingMode == TILING_OFF)
												phaseDirections -= PASS_RIGHT;
										}
										else if (localX >= 8 && localX <= 23 && localY >= 24 && localY <= 31)
										{
											if (tilingMode == TILING_ON)
												phaseDirections |= PASS_DOWN;
											else if ((phaseDirections & PASS_DOWN) != 0 && tilingMode == TILING_OFF)
												phaseDirections -= PASS_DOWN;
										}
										else if (localX >= 0 && localX <= 7 && localY >= 8 && localY <= 23)
										{
											if (tilingMode == TILING_ON)
												phaseDirections |= PASS_LEFT;
											else if ((phaseDirections & PASS_LEFT) != 0 && tilingMode == TILING_OFF)
												phaseDirections -= PASS_LEFT;
										}
										else if (localX >= 8 && localX <= 23 && localY >= 8 && localY <= 23 && tilingMode == TILING_AUTO)
										{
											autoCalcPassabilityDirections(tileLocation.x(), tileLocation.y(), false);
											phaseDirections = tempLayerData_AutoCalc[index].phaseDirections[currentPhase];
										}
										tempLayerData_AutoCalc[index].phaseDirections[currentPhase] = phaseDirections;
										tempLayerData_NoAutoCalc[index].phaseDirections[currentPhase] = phaseDirections;
									}
								}
							}
						}
						if (currentEditingMode == EDITMODE_LAYERS)
						{
							autoCalcFlags(QRect(tileLocation.x(), tileLocation.y(), tileSelectionWidth, tileSelectionHeight), true);
							updateMapView(QRect(tileLocation.x() - 1, tileLocation.y() - 1, tileSelectionWidth + 2, tileSelectionHeight + 2));
						}
						else
							updateMapView(QRect(tileLocation.x() - 1, tileLocation.y() - 1, 3, 3));
					}
					else if (currentTool == TOOL_TILING)
					{
						int index = tileLocation.y() * mapWidth + tileLocation.x();
						unsigned char tilesetID = tempLayerData_AutoCalc[index].tilesetID;
						unsigned char tileID = tempLayerData_AutoCalc[index].tileID;
						unsigned char tileFlags = tempLayerData_AutoCalc[index].tileFlags;
						if (tilesetID < tilesets.size())
						{
							if (tileID < (*tilesets[tilesetID])->numTiles())
							{
								if ((*tilesets[tilesetID])->getTile(tileID)->isAutoTile())
								{
									int localX = (int)(mapToScene(event->x(), event->y()).x()) % ZOOM_3TO2;
									int localY = (int)(mapToScene(event->x(), event->y()).y()) % ZOOM_3TO2;
									if (localX >= 2 && localX <= 12 && localY >= 2 && localY <= 12)
									{
										if (tilingMode == TILING_ON)
											tileFlags |= Tile::Tile_nw;
										else if ((tileFlags & Tile::Tile_nw) != 0 && tilingMode == TILING_OFF)
											tileFlags -= Tile::Tile_nw;
									}
									else if (localX >= 19 && localX <= 28 && localY >= 2 && localY <= 12)
									{
										if (tilingMode == TILING_ON)
											tileFlags |= Tile::Tile_n;
										else if ((tileFlags & Tile::Tile_n) != 0 && tilingMode == TILING_OFF)
											tileFlags -= Tile::Tile_n;
									}
									else if (localX >= 35 && localX <= 45 && localY >= 2 && localY <= 12)
									{
										if (tilingMode == TILING_ON)
											tileFlags |= Tile::Tile_ne;
										else if ((tileFlags & Tile::Tile_ne) != 0 && tilingMode == TILING_OFF)
											tileFlags -= Tile::Tile_ne;
									}
									else if (localX >= 35 && localX <= 45 && localY >= 19 && localY <= 28)
									{
										if (tilingMode == TILING_ON)
											tileFlags |= Tile::Tile_e;
										else if ((tileFlags & Tile::Tile_e) != 0 && tilingMode == TILING_OFF)
											tileFlags -= Tile::Tile_e;
									}
									else if (localX >= 35 && localX <= 45 && localY >= 35 && localY <= 45)
									{
										if (tilingMode == TILING_ON)
											tileFlags |= Tile::Tile_se;
										else if ((tileFlags & Tile::Tile_se) != 0 && tilingMode == TILING_OFF)
											tileFlags -= Tile::Tile_se;
									}
									else if (localX >= 19 && localX <= 28 && localY >= 35 && localY <= 45)
									{
										if (tilingMode == TILING_ON)
											tileFlags |= Tile::Tile_s;
										else if ((tileFlags & Tile::Tile_s) != 0 && tilingMode == TILING_OFF)
											tileFlags -= Tile::Tile_s;
									}
									else if (localX >= 2 && localX <= 12 && localY >= 35 && localY <= 45)
									{
										if (tilingMode == TILING_ON)
											tileFlags |= Tile::Tile_sw;
										else if ((tileFlags & Tile::Tile_sw) != 0 && tilingMode == TILING_OFF)
											tileFlags -= Tile::Tile_sw;
									}
									else if (localX >= 2 && localX <= 12 && localY >= 19 && localY <= 28)
									{
										if (tilingMode == TILING_ON)
											tileFlags |= Tile::Tile_w;
										else if ((tileFlags & Tile::Tile_w) != 0 && tilingMode == TILING_OFF)
											tileFlags -= Tile::Tile_w;
									}
									else if (localX >= 18 && localX <= 29 && localY >= 18 && localY <= 29 && tilingMode == TILING_AUTO)
									{
										autoCalcFlags(tileLocation.x(), tileLocation.y(), false);
										tileFlags = tempLayerData_AutoCalc[index].tileFlags;
									}
									tempLayerData_AutoCalc[index].tileFlags = tileFlags;
									tempLayerData_NoAutoCalc[index].tileFlags = tileFlags;
									updateMapView(tileLocation);
								}
							}
						}
					}
				}
				else if (tileLocation != rectStart)
				{
					QPoint oldRectStart = rectStart;
					rectStart = tileLocation;
					if (currentEditingMode == EDITMODE_EVENTS && (eventID != -1 || startLocationID != -2))
					{
						bool foundOne = false;
						if ((*mapRef)->eventAtLocation(rectStart))
							foundOne = true;
						if (ProjectData::partyStartLocation.mapID == mapRef->getDataID() && ProjectData::partyStartLocation.x == rectStart.x() && ProjectData::partyStartLocation.y == rectStart.y())
							foundOne = true;
						for (int i = 0; i < ProjectData::vehicleStartLocations.size() && !foundOne; ++i)
						{
							if (ProjectData::vehicleStartLocations[i].mapID == mapRef->getDataID() && ProjectData::vehicleStartLocations[i].x == rectStart.x() && ProjectData::vehicleStartLocations[i].y == rectStart.y())
								foundOne = true;
						}
						if (!foundOne)
						{
							if (eventID != -1)
								(*mapRef)->getEvent(eventID)->setLocation(rectStart.x(), rectStart.y());
							else if (startLocationID == -1)
							{
								ProjectData::partyStartLocation.x = rectStart.x();
								ProjectData::partyStartLocation.y = rectStart.y();
								ProjectData::saved = false;
							}
							else if (startLocationID >= 0)
							{
								ProjectData::vehicleStartLocations[startLocationID].x = rectStart.x();
								ProjectData::vehicleStartLocations[startLocationID].y = rectStart.y();
								ProjectData::saved = false;
							}
						}
						else
							rectStart = oldRectStart;
					}
					if (oldRectStart.x() != -1 && oldRectStart.y() != -1)
						updateMapView(oldRectStart);
					if (rectStart.x() != -1 && rectStart.y() != -1)
						updateMapView(rectStart);
				}
			}
			else if ((buttons & Qt::RightButton) != 0 && (buttons & Qt::LeftButton) == 0 && !canceled && currentEditingMode == EDITMODE_LAYERS && currentTool != TOOL_SELECTION && currentTool != TOOL_TILING)
			{
				QRect selectionRect;
				rectEnd = tileLocation;
				if (rectStart.x() < rectEnd.x())
				{
					selectionRect.setLeft(rectStart.x());
					selectionRect.setRight(rectEnd.x());
					if (selectionRect.width() > 6)
					{
						selectionRect.setRight(rectStart.x() + 5);
						rectEnd.setX(rectStart.x() + 5);
					}
				}
				else
				{
					selectionRect.setLeft(rectEnd.x());
					selectionRect.setRight(rectStart.x());
					if (selectionRect.width() > 6)
					{
						selectionRect.setLeft(rectStart.x() - 5);
						rectEnd.setX(rectStart.x() - 5);
					}
				}
				if (rectStart.y() < rectEnd.y())
				{
					selectionRect.setTop(rectStart.y());
					selectionRect.setBottom(rectEnd.y());
					if (selectionRect.height() > 6)
					{
						selectionRect.setBottom(rectStart.y() + 5);
						rectEnd.setY(rectStart.y() + 5);
					}
				}
				else
				{
					selectionRect.setTop(rectEnd.y());
					selectionRect.setBottom(rectStart.y());
					if (selectionRect.height() > 6)
					{
						selectionRect.setTop(rectStart.y() - 5);
						rectEnd.setY(rectStart.y() - 5);
					}
				}
				currentSelection->setRect((selectionRect.x() * float(currentZoom)) + 1.0, (selectionRect.y() * float(currentZoom)) + 1.0, (selectionRect.width() * float(currentZoom)) - 2.0, (selectionRect.height() * float(currentZoom)) - 2.0);
			}
			if ((((event->buttons() & Qt::LeftButton) != 0 && (event->buttons() & Qt::RightButton) == 0 && currentTool == TOOL_PENCIL) || event->buttons() == Qt::NoButton) && currentEditingMode == EDITMODE_LAYERS && currentTool != TOOL_SELECTION && currentTool != TOOL_TILING)
			{
				QPoint sceneLocation;
				sceneLocation.setX(int(mapToScene(event->x(), event->y()).x()));
				sceneLocation.setY(int(mapToScene(event->x(), event->y()).y()));
				if (sceneLocation.x() < 0 || sceneLocation.x() >= mapWidth * currentZoom || sceneLocation.y() < 0 || sceneLocation.y() >= mapHeight * currentZoom)
					currentSelection->setVisible(false);
				else
				{
					currentSelection->setRect((tileLocation.x() * float(currentZoom)) + 1.0, (tileLocation.y() * float(currentZoom)) + 1.0, (tileSelectionWidth * float(currentZoom)) - 2.0, (tileSelectionHeight * float(currentZoom)) - 2.0);
					currentSelection->setVisible(true);
				}
			}
		}
		if (event->button() != Qt::NoButton)
			mouseMoved(event);
	}
}

void MapView::mousePressEvent(QMouseEvent *event)
{
	if (!ignoreevents)
	{
		QPoint sceneCoordinates, tileLocation;
		if (currentEditingMode != EDITMODE_PREVIEW)
		{
			if (event->button() == Qt::LeftButton)
			{
				if ((event->buttons() & Qt::RightButton) == 0)
				{
					QPointF testPoint = mapToScene(event->x(), event->y());
					autocalc = ((QApplication::keyboardModifiers() & Qt::ShiftModifier) == 0);
					ignoreContextMenu = true;
					sceneCoordinates.setX(int(mapToScene(event->x(), event->y()).x()) / currentZoom);
					sceneCoordinates.setY(int(mapToScene(event->x(), event->y()).y()) / currentZoom);
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
					if (currentEditingMode != EDITMODE_EVENTS)
					{
						if (currentTool == TOOL_SELECTION)
						{
							if (selectionData.anchored && !selectionRect.contains(tileLocation))
							{
								rectStart = tileLocation;
								rectEnd = tileLocation;
								if (rectStart.x() > rectEnd.x())
								{
									selectionRect.setLeft(rectEnd.x());
									selectionRect.setRight(rectStart.x());
								}
								else
								{
									selectionRect.setLeft(rectStart.x());
									selectionRect.setRight(rectEnd.x());
								}
								if (rectStart.y() > rectEnd.y())
								{
									selectionRect.setTop(rectEnd.y());
									selectionRect.setBottom(rectStart.y());
								}
								else
								{
									selectionRect.setTop(rectStart.y());
									selectionRect.setBottom(rectEnd.y());
								}
								currentSelection->setRect((selectionRect.x() * float(currentZoom)) + 1.0, (selectionRect.y() * float(currentZoom)) + 1.0, (selectionRect.width() * float(currentZoom)) - 2.0, (selectionRect.height() * float(currentZoom)) - 2.0);
								currentSelection->setVisible(true);
							}
							else if (!selectionData.anchored && selectionRect.contains(tileLocation))
							{
								rectStart.setX(tileLocation.x() - selectionData.x);
								rectStart.setY(tileLocation.y() - selectionData.y);
							}
						}
						else if (currentTool == TOOL_RECTANGLE)
						{
							rectStart = sceneCoordinates;
							rectEnd = sceneCoordinates;
							currentSelection->setVisible(false);
							addRectData();
						}
						else if (currentTool == TOOL_CIRCLE)
						{
							rectStart = sceneCoordinates;
							rectEnd = sceneCoordinates;
							currentSelection->setVisible(false);
							addCircleData();
						}
						else if (currentTool == TOOL_FILL)
						{
							if (sceneCoordinates.x() >= 0 && sceneCoordinates.x() < mapWidth && sceneCoordinates.y() >= 0 && sceneCoordinates.y() < mapHeight)
							{
								rectStart = tileLocation;
								fill(tileLocation.x(), tileLocation.y(), autocalc);
								applyTempLayerData();
								ProjectData::saved = false;
								currentSelection->setVisible(false);
								updateMapView();
							}
						}
						else if (currentTool == TOOL_PENCIL)
						{
							rectStart = sceneCoordinates;
							if (sceneCoordinates.x() >= 0 && sceneCoordinates.x() < mapWidth && sceneCoordinates.y() >= 0 && sceneCoordinates.y() < mapHeight)
							{
								for (int y = 0; y < tileSelectionHeight; ++y)
								{
									for (int x = 0; x < tileSelectionWidth; ++x)
									{
										QPoint location(tileLocation.x() + x, tileLocation.y() + y);
										if (currentEditingMode == EDITMODE_LAYERS)
										{
											QPoint tileSelectionLocation;
											tileSelectionLocation.setX((tileLocation.x() + x - rectStart.x()) % tileSelectionWidth);
											tileSelectionLocation.setY((tileLocation.y() + y - rectStart.y()) % tileSelectionHeight);
											if (tileSelectionLocation.x() < 0)
												tileSelectionLocation.setX(tileSelectionLocation.x() + tileSelectionWidth);
											if (tileSelectionLocation.y() < 0)
												tileSelectionLocation.setY(tileSelectionLocation.y() + tileSelectionHeight);
											if (location.x() < mapWidth && location.y() < mapHeight)
											{
												tempLayerData_AutoCalc[location.y() * mapWidth + location.x()] = tileSelection[tileSelectionLocation.y() * tileSelectionWidth + tileSelectionLocation.x()];
												tempLayerData_NoAutoCalc[location.y() * mapWidth + location.x()] = tileSelection[tileSelectionLocation.y() * tileSelectionWidth + tileSelectionLocation.x()];
											}
										}
										else if (currentEditingMode == EDITMODE_PASSABILITY)
										{
											int index = location.y() * mapWidth + location.x();
											if (passabilityMode == PASSMODE_SET)
											{
												tempLayerData_AutoCalc[index].phasePassability[currentPhase] = true;
												tempLayerData_NoAutoCalc[index].phasePassability[currentPhase] = true;
											}
											else if (passabilityMode == PASSMODE_CLEAR)
											{
												tempLayerData_AutoCalc[index].phasePassability[currentPhase] = false;
												tempLayerData_NoAutoCalc[index].phasePassability[currentPhase] = false;
											}
											else if (passabilityMode == PASSMODE_DIRECTIONS)
											{
												int localX = (int)(mapToScene(event->x(), event->y()).x()) % currentZoom;
												int localY = (int)(mapToScene(event->x(), event->y()).y()) % currentZoom;
												unsigned char oldDirections = tempLayerData_AutoCalc[index].phaseDirections[currentPhase];
												unsigned char newDirections = tempLayerData_AutoCalc[index].phaseDirections[currentPhase];
												tilingMode = TILING_ON;
												if (localX >= 8 && localX <= 23 && localY >= 0 && localY <= 7)
													newDirections ^= PASS_UP;
												else if (localX >= 24 && localX <= 31 && localY >= 8 && localY <= 23)
													newDirections ^= PASS_RIGHT;
												else if (localX >= 8 && localX <= 23 && localY >= 24 && localY <= 31)
													newDirections ^= PASS_DOWN;
												else if (localX >= 0 && localX <= 7 && localY >= 8 && localY <= 23)
													newDirections ^= PASS_LEFT;
												else if (localX >= 8 && localX <= 23 && localY >= 8 && localY <= 23)
												{
													tilingMode = TILING_AUTO;
													autoCalcPassabilityDirections(tileLocation.x(), tileLocation.y(), false);
													newDirections = tempLayerData_AutoCalc[index].phaseDirections[currentPhase];
												}
												if (tilingMode != TILING_AUTO)
													tilingMode = (((oldDirections ^ newDirections) & oldDirections) != 0) ? TILING_OFF:TILING_ON;
												tempLayerData_AutoCalc[index].phaseDirections[currentPhase] = newDirections;
												tempLayerData_NoAutoCalc[index].phaseDirections[currentPhase] = newDirections;
											}
										}
									}
								}
								if (currentEditingMode == EDITMODE_LAYERS)
								{
									autoCalcFlags(QRect(tileLocation.x(), tileLocation.y(), tileSelectionWidth, tileSelectionHeight), true);
									updateMapView(QRect(tileLocation.x() - 1, tileLocation.y() - 1, tileSelectionWidth + 2, tileSelectionHeight + 2));
								}
								else
								{
									if (passabilityMode != PASSMODE_DIRECTIONS)
										autoCalcPassabilityDirections(tileLocation.x(), tileLocation.y(), true);
									updateMapView(QRect(tileLocation.x() - 1, tileLocation.y() - 1, 3, 3));
								}
								currentSelection->setVisible(false);
							}
						}
						else if (currentTool == TOOL_TILING)
						{
							int index = tileLocation.y() * mapWidth + tileLocation.x();
							unsigned char tilesetID = tempLayerData_AutoCalc[index].tilesetID;
							unsigned char tileID = tempLayerData_AutoCalc[index].tileID;
							unsigned char oldFlags = tempLayerData_AutoCalc[index].tileFlags;
							unsigned char newFlags = tempLayerData_AutoCalc[index].tileFlags;
							if (tilesetID < tilesets.size())
							{
								if (tileID < (*tilesets[tilesetID])->numTiles())
								{
									if ((*tilesets[tilesetID])->getTile(tileID)->isAutoTile())
									{
										int localX = int(float(int(mapToScene(event->x(), event->y()).x()) % ZOOM_3TO2) / float(ZOOM_3TO2) * 48.0);
										int localY = int(float(int(mapToScene(event->x(), event->y()).y()) % ZOOM_3TO2) / float(ZOOM_3TO2) * 48.0);
										tilingMode = TILING_ON;
										if (localX >= 2 && localX <= 12 && localY >= 2 && localY <= 12)
											newFlags ^= Tile::Tile_nw;
										else if (localX >= 19 && localX <= 28 && localY >= 2 && localY <= 12)
											newFlags ^= Tile::Tile_n;
										else if (localX >= 35 && localX <= 45 && localY >= 2 && localY <= 12)
											newFlags ^= Tile::Tile_ne;
										else if (localX >= 35 && localX <= 45 && localY >= 19 && localY <= 28)
											newFlags ^= Tile::Tile_e;
										else if (localX >= 35 && localX <= 45 && localY >= 35 && localY <= 45)
											newFlags ^= Tile::Tile_se;
										else if (localX >= 19 && localX <= 28 && localY >= 35 && localY <= 45)
											newFlags ^= Tile::Tile_s;
										else if (localX >= 2 && localX <= 12 && localY >= 35 && localY <= 45)
											newFlags ^= Tile::Tile_sw;
										else if (localX >= 2 && localX <= 12 && localY >= 19 && localY <= 28)
											newFlags ^= Tile::Tile_w;
										else if (localX >= 18 && localX <= 29 && localY >= 18 && localY <= 29)
										{
											tilingMode = TILING_AUTO;
											autoCalcFlags(tileLocation.x(), tileLocation.y(), false);
											newFlags = tempLayerData_AutoCalc[index].tileFlags;
										}
										if (tilingMode != TILING_AUTO)
											tilingMode = (((oldFlags ^ newFlags) & oldFlags) != 0) ? TILING_OFF:TILING_ON;
										tempLayerData_AutoCalc[index].tileFlags = newFlags;
										tempLayerData_NoAutoCalc[index].tileFlags = newFlags;
										updateMapView(tileLocation);
									}
								}
							}
						}
						canceled = false;
					}
					else if (currentEditingMode == EDITMODE_EVENTS)
					{
						QPoint oldRectStart = rectStart;
						rectStart = sceneCoordinates;
						if ((*mapRef)->eventAtLocation(tileLocation))
						{
							eventID = (*mapRef)->eventIDAtLocation(tileLocation);
							aNewEvent->setShortcut(QKeySequence());
							aEditEvent->setShortcut(QKeySequence(Qt::Key_Space));
						}
						else
						{
							eventID = -1;
							startLocationID = -2;
							if (ProjectData::partyStartLocation.mapID == mapRef->getDataID() && ProjectData::partyStartLocation.x == rectStart.x() && ProjectData::partyStartLocation.y == rectStart.y())
								startLocationID = -1;
							else
							{
								for (int i = 0; i < ProjectData::vehicleStartLocations.size(); ++i)
								{
									if (ProjectData::vehicleStartLocations[i].mapID == mapRef->getDataID() && ProjectData::vehicleStartLocations[i].x == rectStart.x() && ProjectData::vehicleStartLocations[i].y == rectStart.y())
										startLocationID = i;
								}
							}
							if (startLocationID == -2)
							{
								aNewEvent->setShortcut(QKeySequence(Qt::Key_Space));
								aEditEvent->setShortcut(QKeySequence());
							}
						}
						if (oldRectStart.x() != -1 && oldRectStart.y() != -1)
							updateMapView(oldRectStart);
						if (rectStart.x() != -1 && rectStart.y() != -1)
							updateMapView(rectStart);
						canceled = false;
					}
				}
				else
				{
					rectStart = QPoint(-1, -1);
					rectEnd = QPoint(-1, -1);
					tileLocation.setX(int(mapToScene(event->x(), event->y()).x()) / currentZoom);
					tileLocation.setY(int(mapToScene(event->x(), event->y()).y()) / currentZoom);
					if (tileLocation.x() < 0)
						tileLocation.setX(0);
					else if (tileLocation.x() >= mapWidth)
						tileLocation.setX(mapWidth - 1);
					if (tileLocation.y() < 0)
						tileLocation.setY(0);
					else if (tileLocation.y() >= mapHeight)
						tileLocation.setY(mapHeight - 1);
					currentSelection->setRect((tileLocation.x() * float(currentZoom)) + 1.0, (tileLocation.y() * float(currentZoom)) + 1.0, (tileSelectionWidth * float(currentZoom)) - 2.0, (tileSelectionHeight * float(currentZoom)) - 2.0);
					canceled = true;
				}
			}
			else if (event->button() == Qt::RightButton)
			{
				if ((event->buttons() & Qt::LeftButton) != 0)
				{
					rectStart = QPoint(-1, -1);
					rectEnd = QPoint(-1, -1);
					eventID = -1;
					tempLayerData_AutoCalc = tempLayerData_NoAutoCalc = (*mapRef)->getLayerData(currentLayer)->tileData;
					currentSelection->setVisible(false);
					canceled = true;
					if (currentTool != TOOL_SELECTION)
						updateMapView();
				}
				else if (currentEditingMode != EDITMODE_EVENTS)
				{
					if (currentTool != TOOL_TILING && currentTool != TOOL_SELECTION)
					{
						QPoint tileLocation;
						tileLocation.setX(int(mapToScene(event->x(), event->y()).x()) / currentZoom);
						tileLocation.setY(int(mapToScene(event->x(), event->y()).y()) / currentZoom);
						if (tileLocation.x() < 0)
							tileLocation.setX(0);
						else if (tileLocation.x() >= mapWidth)
							tileLocation.setX(mapWidth - 1);
						if (tileLocation.y() < 0)
							tileLocation.setY(0);
						else if (tileLocation.y() >= mapHeight)
							tileLocation.setY(mapHeight - 1);
						rectStart = tileLocation;
						rectEnd = tileLocation;
						currentSelection->setRect((tileLocation.x() * float(currentZoom)) + 1.0, (tileLocation.y() * float(currentZoom)) + 1.0, float(currentZoom) - 2.0, float(currentZoom) - 2.0);
					}
				}
			}
			else if (event->button() == Qt::MiddleButton)
			{
				QPointF testPoint = mapToScene(event->x(), event->y());
				ignoreContextMenu = true;
				sceneCoordinates.setX(int(mapToScene(event->x(), event->y()).x()) / currentZoom);
				sceneCoordinates.setY(int(mapToScene(event->x(), event->y()).y()) / currentZoom);
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
			}
		}
		mouseButtonPressed(event);
	}
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
	if (!ignoreevents)
	{
		if (event->button() == Qt::LeftButton && (event->buttons() & Qt::RightButton) == 0 && currentEditingMode != EDITMODE_PREVIEW)
		{
			ignoreContextMenu = false;
			scrollEvent.setX(0);
			scrollEvent.setY(0);
			if (currentEditingMode != EDITMODE_EVENTS)
			{
				if (currentTool == TOOL_RECTANGLE || currentTool == TOOL_CIRCLE || currentTool == TOOL_PENCIL)
				{
					applyTempLayerData();
					ProjectData::saved = false;
					QPoint sceneLocation;
					sceneLocation.setX(int(mapToScene(event->x(), event->y()).x()) / currentZoom);
					sceneLocation.setY(int(mapToScene(event->x(), event->y()).y()) / currentZoom);
					if (sceneLocation.x() < 0 || sceneLocation.x() >= mapWidth || sceneLocation.y() < 0 || sceneLocation.y() >= mapHeight)
						currentSelection->setVisible(false);
					else
					{
						currentSelection->setRect((sceneLocation.x() * float(currentZoom)) + 1.0, (sceneLocation.y() * float(currentZoom)) + 1.0, (tileSelectionWidth * float(currentZoom)) - 2.0, (tileSelectionHeight * float(currentZoom)) - 2.0);
						if (currentEditingMode == EDITMODE_LAYERS)
							currentSelection->setVisible(true);
						else
							currentSelection->setVisible(false);
					}
				}
				else if (currentTool == TOOL_FILL)
				{
					QPoint sceneLocation;
					sceneLocation.setX(int(mapToScene(event->x(), event->y()).x()) / currentZoom);
					sceneLocation.setY(int(mapToScene(event->x(), event->y()).y()) / currentZoom);
					if (sceneLocation.x() < 0 || sceneLocation.x() >= mapWidth || sceneLocation.y() < 0 || sceneLocation.y() >= mapHeight)
						currentSelection->setVisible(false);
					else
					{
						currentSelection->setRect((sceneLocation.x() * float(currentZoom)) + 1.0, (sceneLocation.y() * float(currentZoom)) + 1.0, (tileSelectionWidth * float(currentZoom)) - 2.0, (tileSelectionHeight * float(currentZoom)) - 2.0);
						if (currentEditingMode == EDITMODE_LAYERS)
							currentSelection->setVisible(true);
						else
							currentSelection->setVisible(false);
					}
				}
				else if (currentTool == TOOL_TILING)
					applyTempLayerData();
			}
		}
		else if (event->button() == Qt::RightButton && (event->buttons() & Qt::LeftButton) == 0 && currentEditingMode != EDITMODE_PREVIEW)
		{
			if (currentEditingMode == EDITMODE_LAYERS)
			{
				if (rectStart.x() < rectEnd.x())
				{
					selectionRect.setLeft(rectStart.x());
					selectionRect.setRight(rectEnd.x());
				}
				else
				{
					selectionRect.setLeft(rectEnd.x());
					selectionRect.setRight(rectStart.x());
				}
				if (rectStart.y() < rectEnd.y())
				{
					selectionRect.setTop(rectStart.y());
					selectionRect.setBottom(rectEnd.y());
				}
				else
				{
					selectionRect.setTop(rectEnd.y());
					selectionRect.setBottom(rectStart.y());
				}
				currentSelection->setRect((selectionRect.x() * float(currentZoom)) + 1.0, (selectionRect.y() * float(currentZoom)) + 1.0, (selectionRect.width() * float(currentZoom)) - 2.0, (selectionRect.height() * float(currentZoom)) - 2.0);
				tileSelection.clear();
				tileSelectionWidth = selectionRect.width();
				tileSelectionHeight = selectionRect.height();
				for (int y = 0; y < tileSelectionHeight; ++y)
				{
					for (int x = 0; x < tileSelectionWidth; ++x)
						tileSelection.append((*mapRef)->getTileData(x + selectionRect.x(), y + selectionRect.y(), currentLayer));
				}
				tileSelectionChanged(tileSelectionWidth, tileSelectionHeight, tileSelection);
			}
		}
		if (event->buttons() == Qt::NoButton)
			canceled = false;
		mouseButtonReleased(event);
	}
}

void MapView::resizeEvent(QResizeEvent *event)
{
	if (!ignoreevents && !waitevents)
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
		mapViewImage = QImage(updateRect.width() * currentZoom, updateRect.height() * currentZoom, QImage::Format_ARGB32);
		setSceneRect(0.0, 0.0, mapWidth * float(currentZoom), mapHeight * float(currentZoom));
		updateMapView();
		event->accept();
	}
	else
		event->ignore();
}

QRect MapView::addRectData()
{
	QRect rectangle;
	QPoint location;
	QPoint tileSelectionLocation;
	int index;
	if (rectStart.x() < rectEnd.x())
	{
		rectangle.setLeft(rectStart.x());
		rectangle.setRight(rectEnd.x());
	}
	else
	{
		rectangle.setLeft(rectEnd.x());
		rectangle.setRight(rectStart.x());
	}
	if (rectStart.y() < rectEnd.y())
	{
		rectangle.setTop(rectStart.y());
		rectangle.setBottom(rectEnd.y());
	}
	else
	{
		rectangle.setTop(rectEnd.y());
		rectangle.setBottom(rectStart.y());
	}
	for (int y = 0; y < rectangle.height(); ++y)
	{
		for (int x = 0; x < rectangle.width(); ++x)
		{
			location.setX(x + rectangle.x());
			location.setY(y + rectangle.y());
			if (location.x() >= 0 && location.x() < mapWidth && location.y() >= 0 && location.y() < mapHeight)
			{
				index = location.y() * mapWidth + location.x();
				if (currentEditingMode == EDITMODE_LAYERS)
				{
					tileSelectionLocation.setX((location.x() - rectStart.x()) % tileSelectionWidth);
					tileSelectionLocation.setY((location.y() - rectStart.y()) % tileSelectionHeight);
					if (tileSelectionLocation.x() < 0)
						tileSelectionLocation.setX(tileSelectionLocation.x() + tileSelectionWidth);
					else if (tileSelectionLocation.y() < 0)
						tileSelectionLocation.setY(tileSelectionLocation.y() + tileSelectionHeight);
					tempLayerData_AutoCalc[index] = tileSelection[tileSelectionLocation.y() * tileSelectionWidth + tileSelectionLocation.x()];
					tempLayerData_NoAutoCalc[index] = tileSelection[tileSelectionLocation.y() * tileSelectionWidth + tileSelectionLocation.x()];
					tempLayerData_AutoCalc[index].tileFlags = 0xff;
					tempLayerData_NoAutoCalc[index].tileFlags = 0xff;
				}
				else if (currentEditingMode == EDITMODE_PASSABILITY)
				{
					if (passabilityMode == PASSMODE_SET)
					{
						tempLayerData_AutoCalc[index].phasePassability[currentPhase] = true;
						tempLayerData_NoAutoCalc[index].phasePassability[currentPhase] = true;
					}
					else if (passabilityMode == PASSMODE_CLEAR)
					{
						tempLayerData_AutoCalc[index].phasePassability[currentPhase] = false;
						tempLayerData_NoAutoCalc[index].phasePassability[currentPhase] = false;
					}
				}
			}
		}
	}
	if (currentEditingMode == EDITMODE_LAYERS)
		autoCalcFlags(rectangle, true);
	else if (currentEditingMode == EDITMODE_PASSABILITY)
		autoCalcPassabilityDirections(rectangle, true);
	return QRect(rectangle.x() - 1, rectangle.y() - 1, rectangle.width() + 2, rectangle.height() + 2);
}

QRect MapView::subRectData()
{
	QRect rectangle;
	QPoint location;
	int index;
	if (rectStart.x() < rectEnd.x())
	{
		rectangle.setLeft(rectStart.x());
		rectangle.setRight(rectEnd.x());
	}
	else
	{
		rectangle.setLeft(rectEnd.x());
		rectangle.setRight(rectStart.x());
	}
	if (rectStart.y() < rectEnd.y())
	{
		rectangle.setTop(rectStart.y());
		rectangle.setBottom(rectEnd.y());
	}
	else
	{
		rectangle.setTop(rectEnd.y());
		rectangle.setBottom(rectStart.y());
	}
	rectangle = QRect(rectangle.x() - 1, rectangle.y() - 1, rectangle.width() + 2, rectangle.height() + 2);
	for (int y = 0; y < rectangle.height(); ++y)
	{
		for (int x = 0; x < rectangle.width(); ++x)
		{
			location.setX(x + rectangle.x());
			location.setY(y + rectangle.y());
			if (location.x() >= 0 && location.x() < mapWidth && location.y() >= 0 && location.y() < mapHeight)
			{
				index = location.y() * mapWidth + location.x();
				tempLayerData_AutoCalc[index] = (*mapRef)->getTileData(location.x(), location.y(), currentLayer);
				tempLayerData_NoAutoCalc[index] = tempLayerData_AutoCalc[index];
			}
		}
	}
	return rectangle;
}

QRect MapView::addCircleData()
{
	QPainter painter;
	QImage image;
	QRect rectangle;
	QPoint location;
	QPoint tileSelectionLocation;
	QList<QPoint> autoCalcLocations;
	QRgb color = qRgb(255, 255, 255);
	int index;
	if (rectStart.x() < rectEnd.x())
	{
		rectangle.setLeft(rectStart.x());
		rectangle.setRight(rectEnd.x());
	}
	else
	{
		rectangle.setLeft(rectEnd.x());
		rectangle.setRight(rectStart.x());
	}
	if (rectStart.y() < rectEnd.y())
	{
		rectangle.setTop(rectStart.y());
		rectangle.setBottom(rectEnd.y());
	}
	else
	{
		rectangle.setTop(rectEnd.y());
		rectangle.setBottom(rectStart.y());
	}
	image = QImage(rectangle.width(), rectangle.height(), QImage::Format_RGB32);
	painter.begin(&image);
	painter.setPen(QPen(Qt::NoPen));
	painter.setBrush(QBrush(Qt::black));
	painter.drawRect(0, 0, image.width(), image.height());
	painter.setBrush(QBrush(Qt::white));
	painter.drawEllipse(0, -1, image.width(), image.height() + 1);
	painter.end();
	for (int y = 0; y < rectangle.height(); ++y)
	{
		for (int x = 0; x < rectangle.width(); ++x)
		{
			if (image.pixel(x, y) == color)
			{
				location.setX(x + rectangle.x());
				location.setY(y + rectangle.y());
				if (location.x() >= 0 && location.x() < mapWidth && location.y() >= 0 && location.y() < mapHeight)
				{
					index = location.y() * mapWidth + location.x();
					if (currentEditingMode == EDITMODE_LAYERS)
					{
						tileSelectionLocation.setX((location.x() - rectStart.x()) % tileSelectionWidth);
						tileSelectionLocation.setY((location.y() - rectStart.y()) % tileSelectionHeight);
						if (tileSelectionLocation.x() < 0)
							tileSelectionLocation.setX(tileSelectionLocation.x() + tileSelectionWidth);
						else if (tileSelectionLocation.y() < 0)
							tileSelectionLocation.setY(tileSelectionLocation.y() + tileSelectionHeight);
						tempLayerData_AutoCalc[index] = tileSelection[tileSelectionLocation.y() * tileSelectionWidth + tileSelectionLocation.x()];
						tempLayerData_NoAutoCalc[index] = tileSelection[tileSelectionLocation.y() * tileSelectionWidth + tileSelectionLocation.x()];
						tempLayerData_AutoCalc[index].tileFlags = 0xff;
						tempLayerData_NoAutoCalc[index].tileFlags = 0xff;
					}
					else if (currentEditingMode == EDITMODE_PASSABILITY)
					{
						if (passabilityMode == PASSMODE_SET)
						{
							tempLayerData_AutoCalc[index].phasePassability[currentPhase] = true;
							tempLayerData_NoAutoCalc[index].phasePassability[currentPhase] = true;
						}
						else if (passabilityMode == PASSMODE_CLEAR)
						{
							tempLayerData_AutoCalc[index].phasePassability[currentPhase] = false;
							tempLayerData_NoAutoCalc[index].phasePassability[currentPhase] = false;
						}
					}
					autoCalcLocations.append(location);
				}
			}
		}
	}
	if (currentEditingMode == EDITMODE_LAYERS)
	{
		for (int i = 0; i < autoCalcLocations.size(); ++i)
			autoCalcFlags(autoCalcLocations[i].x(), autoCalcLocations[i].y(), true);
	}
	else if (currentEditingMode == EDITMODE_PASSABILITY)
	{
		for (int i = 0; i < autoCalcLocations.size(); ++i)
			autoCalcPassabilityDirections(autoCalcLocations[i].x(), autoCalcLocations[i].y(), true);
	}
	return QRect(rectangle.x() - 1, rectangle.y() - 1, rectangle.width() + 2, rectangle.height() + 2);
}

void MapView::applyTempLayerData()
{
	UndoEvent undoEvent;
	undoEvent.oldData = (*mapRef)->getLayerData(currentLayer)->tileData;
	if (autocalc)
	{
		(*mapRef)->getLayerData(currentLayer)->tileData = tempLayerData_AutoCalc;
		tempLayerData_NoAutoCalc = tempLayerData_AutoCalc;
	}
	else
	{
		(*mapRef)->getLayerData(currentLayer)->tileData = tempLayerData_NoAutoCalc;
		tempLayerData_AutoCalc = tempLayerData_NoAutoCalc;
	}
	while (undoLocation + 1 < undoHistory.size())
	{
		undoHistory[undoLocation + 1].oldData.clear();
		undoHistory[undoLocation + 1].newData.clear();
		undoHistory.removeAt(undoLocation + 1);
	}
	undoEnabled(true);
	redoEnabled(false);
	undoEvent.newData = (*mapRef)->getLayerData(currentLayer)->tileData;
	undoEvent.layer = currentLayer;
	undoHistory.append(undoEvent);
	++undoLocation;
	decrementUndoLocation = false;
	incrementUndoLocation = false;
	mapRef->saveToStorage();
}

void MapView::fill(int x, int y, bool autocalcflags)
{
	QList<QPoint> fillPoints;
	QPoint tileSelectionLocation;
	Map::TileData fillTile, tileData1, tileData2;
	int selectionX, selectionY;
	int x2, y2;
	tileSelectionLocation.setX((x - rectStart.x()) % tileSelectionWidth);
	tileSelectionLocation.setY((y - rectStart.y()) % tileSelectionHeight);
	if (tileSelectionLocation.x() < 0)
		tileSelectionLocation.setX(tileSelectionLocation.x() + tileSelectionWidth);
	if (tileSelectionLocation.y() < 0)
		tileSelectionLocation.setY(tileSelectionLocation.y() + tileSelectionHeight);
	fillPoints.append(QPoint(x, y));
	tileData1 = tempLayerData_AutoCalc[y * mapWidth + x];
	for (int i = 0; i < fillPoints.size(); ++i)
	{
		QPoint point = fillPoints[i];
		selectionX = point.x() - x;
		selectionY = point.y() - y;
		while (selectionX < 0)
			selectionX += tileSelectionWidth;
		while (selectionX >= tileSelectionWidth)
			selectionX -= tileSelectionWidth;
		while (selectionY < 0)
			selectionY += tileSelectionHeight;
		while (selectionY >= tileSelectionHeight)
			selectionY -= tileSelectionHeight;
		fillTile = tileSelection[selectionY * tileSelectionWidth + selectionX];
		fillTile.tileFlags = 0x0;
		if (currentEditingMode == EDITMODE_LAYERS)
		{
			tempLayerData_AutoCalc[point.y() * mapWidth + point.x()] = fillTile;
			tempLayerData_NoAutoCalc[point.y() * mapWidth + point.x()] = fillTile;
		}
		else if (currentEditingMode == EDITMODE_PASSABILITY)
		{
			if (passabilityMode == PASSMODE_SET)
			{
				tempLayerData_AutoCalc[point.y() * mapWidth + point.x()].phasePassability[currentPhase] = true;
				tempLayerData_NoAutoCalc[point.y() * mapWidth + point.x()].phasePassability[currentPhase] = true;
			}
			else if (passabilityMode == PASSMODE_CLEAR)
			{
				tempLayerData_AutoCalc[point.y() * mapWidth + point.x()].phasePassability[currentPhase] = false;
				tempLayerData_NoAutoCalc[point.y() * mapWidth + point.x()].phasePassability[currentPhase] = false;
			}
		}
		x2 = point.x();
		y2 = point.y() - 1;
		if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight && !fillPoints.contains(QPoint(x2, y2)))
		{
			tileData2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
			if (tileData2 == tileData1)
				fillPoints.append(QPoint(x2, y2));
		}
		x2 = point.x() - 1;
		y2 = point.y();
		if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight && !fillPoints.contains(QPoint(x2, y2)))
		{
			tileData2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
			if (tileData2 == tileData1)
				fillPoints.append(QPoint(x2, y2));
		}
		x2 = point.x() + 1;
		y2 = point.y();
		if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight && !fillPoints.contains(QPoint(x2, y2)))
		{
			tileData2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
			if (tileData2 == tileData1)
				fillPoints.append(QPoint(x2, y2));
		}
		x2 = point.x();
		y2 = point.y() + 1;
		if (x2 >= 0 && x2 < mapWidth && y2 >= 0 && y2 < mapHeight && !fillPoints.contains(QPoint(x2, y2)))
		{
			tileData2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
			if (tileData2 == tileData1)
				fillPoints.append(QPoint(x2, y2));
		}
	}
	if (autocalcflags && currentEditingMode == EDITMODE_LAYERS)
	{
		for (int i = 0; i < fillPoints.size(); ++i)
			autoCalcFlags(fillPoints[i].x(), fillPoints[i].y(), true);
	}
	else if (currentEditingMode == EDITMODE_PASSABILITY)
	{
		for (int i = 0; i < fillPoints.size(); ++i)
			autoCalcPassabilityDirections(fillPoints[i].x(), fillPoints[i].y(), true);
	}
}

void MapView::autoCalcFlags(int x, int y, bool adjustNeighbors)
{
	Map::TileData tile1, tile2, borderTile;
	int x2, y2, tileGroup1, tileGroup2, borderTileGroup;
	int horizontalMode = (*mapRef)->getHorizontalMode();
	int verticalMode = (*mapRef)->getVerticalMode();
	unsigned char flags = 0x0;
	borderTile.tilesetID = ((*mapRef)->getBorderTile() >> 8) % 256;
	borderTile.tileID = (*mapRef)->getBorderTile() % 256;
	borderTileGroup = (*tilesets[(int)borderTile.tilesetID])->getTile((int)borderTile.tileID)->getTileGroup();
	tile1 = tempLayerData_AutoCalc[y * mapWidth + x];
	if (tile1.tilesetID < tilesets.size())
	{
		if (tile1.tileID < (*tilesets[(int)tile1.tilesetID])->numTiles())
		{
			tileGroup1 = (*tilesets[(int)tile1.tilesetID])->getTile((int)tile1.tileID)->getTileGroup();
			x2 = x - 1;
			y2 = y - 1;
			if (horizontalMode == Map::MODE_WRAPAROUND && x2 < 0)
				x2 += mapWidth;
			if (verticalMode == Map::MODE_WRAPAROUND && y2 < 0)
				y2 += mapHeight;
			if (x2 >= 0 && y2 >= 0)
			{
				tile2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
				if (tile2.tilesetID < tilesets.size())
				{
					if (tile2.tileID < (*tilesets[(int)tile2.tilesetID])->numTiles())
					{
						tileGroup2 = (*tilesets[(int)tile2.tilesetID])->getTile((int)tile2.tileID)->getTileGroup();
						if (tile1 == tile2 || (tile1.tilesetID == tile2.tilesetID && tileGroup1 != -1 && tileGroup1 == tileGroup2))
						{
							tile2.tileFlags |= Tile::Tile_se;
							flags += Tile::Tile_nw;
						}
						else if ((tile2.tileFlags & Tile::Tile_se) != 0)
							tile2.tileFlags -= Tile::Tile_se;
					}
					else
						tile2.tileFlags = 0x0;
				}
				else
					tile2.tileFlags = 0x0;
				if (adjustNeighbors)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].tileFlags = tile2.tileFlags;
			}
			else if (tile1 == borderTile || (tile1.tilesetID == borderTile.tilesetID && tileGroup1 != -1 && tileGroup1 == borderTileGroup))
				flags += Tile::Tile_nw;
			x2 = x;
			y2 = y - 1;
			if (verticalMode == Map::MODE_WRAPAROUND && y2 < 0)
				y2 += mapHeight;
			if (y2 >= 0)
			{
				tile2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
				if (tile2.tilesetID < tilesets.size())
				{
					if (tile2.tileID < (*tilesets[(int)tile2.tilesetID])->numTiles())
					{
						tileGroup2 = (*tilesets[(int)tile2.tilesetID])->getTile((int)tile2.tileID)->getTileGroup();
						if (tile1 == tile2 || (tile1.tilesetID == tile2.tilesetID && tileGroup1 != -1 && tileGroup1 == tileGroup2))
						{
							tile2.tileFlags |= Tile::Tile_s;
							flags += Tile::Tile_n;
						}
						else if ((tile2.tileFlags & Tile::Tile_s) != 0)
							tile2.tileFlags -= Tile::Tile_s;
					}
					else
						tile2.tileFlags = 0x0;
				}
				else
					tile2.tileFlags = 0x0;
				if (adjustNeighbors)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].tileFlags = tile2.tileFlags;
			}
			else if (tile1 == borderTile || (tile1.tilesetID == borderTile.tilesetID && tileGroup1 != -1 && tileGroup1 == borderTileGroup))
				flags += Tile::Tile_n;
			x2 = x + 1;
			y2 = y - 1;
			if (horizontalMode == Map::MODE_WRAPAROUND && x2 >= mapWidth)
				x2 -= mapWidth;
			if (verticalMode == Map::MODE_WRAPAROUND && y2 < 0)
				y2 += mapHeight;
			if (x2 < mapWidth && y2 >= 0)
			{
				tile2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
				if (tile2.tilesetID < tilesets.size())
				{
					if (tile2.tileID < (*tilesets[(int)tile2.tilesetID])->numTiles())
					{
						tileGroup2 = (*tilesets[(int)tile2.tilesetID])->getTile((int)tile2.tileID)->getTileGroup();
						if (tile1 == tile2 || (tile1.tilesetID == tile2.tilesetID && tileGroup1 != -1 && tileGroup1 == tileGroup2))
						{
							tile2.tileFlags |= Tile::Tile_sw;
							flags += Tile::Tile_ne;
						}
						else if ((tile2.tileFlags & Tile::Tile_sw) != 0)
							tile2.tileFlags -= Tile::Tile_sw;
					}
					else
						tile2.tileFlags = 0x0;
				}
				else
					tile2.tileFlags = 0x0;
				if (adjustNeighbors)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].tileFlags = tile2.tileFlags;
			}
			else if (tile1 == borderTile || (tile1.tilesetID == borderTile.tilesetID && tileGroup1 != -1 && tileGroup1 == borderTileGroup))
				flags += Tile::Tile_ne;
			x2 = x - 1;
			y2 = y;
			if (horizontalMode == Map::MODE_WRAPAROUND && x2 < 0)
				x2 += mapWidth;
			if (x2 >= 0)
			{
				tile2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
				if (tile2.tilesetID < tilesets.size())
				{
					if (tile2.tileID < (*tilesets[(int)tile2.tilesetID])->numTiles())
					{
						tileGroup2 = (*tilesets[(int)tile2.tilesetID])->getTile((int)tile2.tileID)->getTileGroup();
						if (tile1 == tile2 || (tile1.tilesetID == tile2.tilesetID && tileGroup1 != -1 && tileGroup1 == tileGroup2))
						{
							tile2.tileFlags |= Tile::Tile_e;
							flags += Tile::Tile_w;
						}
						else if ((tile2.tileFlags & Tile::Tile_e) != 0)
							tile2.tileFlags -= Tile::Tile_e;
					}
					else
						tile2.tileFlags = 0x0;
				}
				else
					tile2.tileFlags = 0x0;
				if (adjustNeighbors)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].tileFlags = tile2.tileFlags;
			}
			else if (tile1 == borderTile || (tile1.tilesetID == borderTile.tilesetID && tileGroup1 != -1 && tileGroup1 == borderTileGroup))
				flags += Tile::Tile_w;
			x2 = x + 1;
			y2 = y;
			if (horizontalMode == Map::MODE_WRAPAROUND && x2 >= mapWidth)
				x2 -= mapWidth;
			if (x2 < mapWidth)
			{
				tile2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
				if (tile2.tilesetID < tilesets.size())
				{
					if (tile2.tileID < (*tilesets[(int)tile2.tilesetID])->numTiles())
					{
						tileGroup2 = (*tilesets[(int)tile2.tilesetID])->getTile((int)tile2.tileID)->getTileGroup();
						if (tile1 == tile2 || (tile1.tilesetID == tile2.tilesetID && tileGroup1 != -1 && tileGroup1 == tileGroup2))
						{
							tile2.tileFlags |= Tile::Tile_w;
							flags += Tile::Tile_e;
						}
						else if ((tile2.tileFlags & Tile::Tile_w) != 0)
							tile2.tileFlags -= Tile::Tile_w;
					}
					else
						tile2.tileFlags = 0x0;
				}
				else
					tile2.tileFlags = 0x0;
				if (adjustNeighbors)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].tileFlags = tile2.tileFlags;
			}
			else if (tile1 == borderTile || (tile1.tilesetID == borderTile.tilesetID && tileGroup1 != -1 && tileGroup1 == borderTileGroup))
				flags += Tile::Tile_e;
			x2 = x - 1;
			y2 = y + 1;
			if (horizontalMode == Map::MODE_WRAPAROUND && x2 < 0)
				x2 += mapWidth;
			if (verticalMode == Map::MODE_WRAPAROUND && y2 >= mapHeight)
				y2 -= mapHeight;
			if (x2 >= 0 && y2 < mapHeight)
			{
				tile2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
				if (tile2.tilesetID < tilesets.size())
				{
					if (tile2.tileID < (*tilesets[(int)tile2.tilesetID])->numTiles())
					{
						tileGroup2 = (*tilesets[(int)tile2.tilesetID])->getTile((int)tile2.tileID)->getTileGroup();
						if (tile1 == tile2 || (tile1.tilesetID == tile2.tilesetID && tileGroup1 != -1 && tileGroup1 == tileGroup2))
						{
							tile2.tileFlags |= Tile::Tile_ne;
							flags += Tile::Tile_sw;
						}
						else if ((tile2.tileFlags & Tile::Tile_ne) != 0)
							tile2.tileFlags -= Tile::Tile_ne;
					}
					else
						tile2.tileFlags = 0x0;
				}
				else
					tile2.tileFlags = 0x0;
				if (adjustNeighbors)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].tileFlags = tile2.tileFlags;
			}
			else if (tile1 == borderTile || (tile1.tilesetID == borderTile.tilesetID && tileGroup1 != -1 && tileGroup1 == borderTileGroup))
				flags += Tile::Tile_sw;
			x2 = x;
			y2 = y + 1;
			if (verticalMode == Map::MODE_WRAPAROUND && y2 >= mapHeight)
				y2 -= mapHeight;
			if (y2 < mapHeight)
			{
				tile2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
				if (tile2.tilesetID < tilesets.size())
				{
					if (tile2.tileID < (*tilesets[(int)tile2.tilesetID])->numTiles())
					{
						tileGroup2 = (*tilesets[(int)tile2.tilesetID])->getTile((int)tile2.tileID)->getTileGroup();
						if (tile1 == tile2 || (tile1.tilesetID == tile2.tilesetID && tileGroup1 != -1 && tileGroup1 == tileGroup2))
						{
							tile2.tileFlags |= Tile::Tile_n;
							flags += Tile::Tile_s;
						}
						else if ((tile2.tileFlags & Tile::Tile_n) != 0)
							tile2.tileFlags -= Tile::Tile_n;
					}
					else
						tile2.tileFlags = 0x0;
				}
				else
					tile2.tileFlags = 0x0;
				if (adjustNeighbors)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].tileFlags = tile2.tileFlags;
			}
			else if (tile1 == borderTile || (tile1.tilesetID == borderTile.tilesetID && tileGroup1 != -1 && tileGroup1 == borderTileGroup))
				flags += Tile::Tile_s;
			x2 = x + 1;
			y2 = y + 1;
			if (horizontalMode == Map::MODE_WRAPAROUND && x2 >= mapWidth)
				x2 -= mapWidth;
			if (verticalMode == Map::MODE_WRAPAROUND && y2 >= mapWidth)
				y2 -= mapHeight;
			if (x2 < mapWidth && y2 < mapHeight)
			{
				tile2 = tempLayerData_AutoCalc[y2 * mapWidth + x2];
				if (tile2.tilesetID < tilesets.size())
				{
					if (tile2.tileID < (*tilesets[(int)tile2.tilesetID])->numTiles())
					{
						tileGroup2 = (*tilesets[(int)tile2.tilesetID])->getTile((int)tile2.tileID)->getTileGroup();
						if (tile1 == tile2 || (tile1.tilesetID == tile2.tilesetID && tileGroup1 != -1 && tileGroup1 == tileGroup2))
						{
							tile2.tileFlags |= Tile::Tile_nw;
							flags += Tile::Tile_se;
						}
						else if ((tile2.tileFlags & Tile::Tile_nw) != 0)
							tile2.tileFlags -= Tile::Tile_nw;
					}
					else
						tile2.tileFlags = 0x0;
				}
				else
					tile2.tileFlags = 0x0;
				if (adjustNeighbors)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].tileFlags = tile2.tileFlags;
			}
			else if (tile1 == borderTile || (tile1.tilesetID == borderTile.tilesetID && tileGroup1 != -1 && tileGroup1 == borderTileGroup))
				flags += Tile::Tile_se;
			if ((*tilesets[(int)tile1.tilesetID])->getTile((int)tile1.tileID)->isAutoTile())
				tempLayerData_AutoCalc[y * mapWidth + x].tileFlags = flags;
		}
	}
}

void MapView::autoCalcFlags(QRect rectangle, bool adjustNeighbors)
{
	for (int y = 0; y < rectangle.height(); ++y)
	{
		for (int x = 0; x < rectangle.width(); ++x)
		{
			if ((x + rectangle.x()) >= 0 && (x + rectangle.x()) < mapWidth && (y + rectangle.y()) >= 0 && (y + rectangle.y()) < mapHeight)
				autoCalcFlags(x + rectangle.x(), y + rectangle.y(), adjustNeighbors);
		}
	}
}

void MapView::autoCalcFlags(bool adjustNeighbors)
{
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
			autoCalcFlags(x, y, adjustNeighbors);
	}
}

void MapView::autoCalcPassabilityDirections(int x, int y, bool adjustNeighbors)
{
	int x2, y2;
	int horizontalMode = (*mapRef)->getHorizontalMode();
	int verticalMode = (*mapRef)->getVerticalMode();
	unsigned char directions = 0x0;
	bool passability = tempLayerData_AutoCalc[y * mapWidth + x].phasePassability[currentPhase];
	x2 = x;
	y2 = y - 1;
	if (verticalMode == Map::MODE_WRAPAROUND && y2 < 0)
		y2 += mapHeight;
	if (y2 >= 0)
	{
		if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
		{
			if (adjustNeighbors)
			{
				if (passability)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] |= PASS_DOWN;
				else if ((tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] & PASS_DOWN) != 0)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] -= PASS_DOWN;
			}
			directions |= PASS_UP;
		}
	}
	x2 = x + 1;
	y2 = y;
	if (horizontalMode == Map::MODE_WRAPAROUND && x2 == mapWidth)
		x2 -= mapWidth;
	if (x2 < mapWidth)
	{
		if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
		{
			if (adjustNeighbors)
			{
				if (passability)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] |= PASS_LEFT;
				else if ((tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] & PASS_LEFT) != 0)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] -= PASS_LEFT;
			}
			directions |= PASS_RIGHT;
		}
	}
	x2 = x;
	y2 = y + 1;
	if (verticalMode == Map::MODE_WRAPAROUND && y2 == mapHeight)
		y2 -= mapHeight;
	if (y2 < mapHeight)
	{
		if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
		{
			if (adjustNeighbors)
			{
				if (passability)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] |= PASS_UP;
				else if ((tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] & PASS_UP) != 0)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] -= PASS_UP;
			}
			directions |= PASS_DOWN;
		}
	}
	x2 = x - 1;
	y2 = y;
	if (horizontalMode == Map::MODE_WRAPAROUND && x2 < 0)
		x2 += mapWidth;
	if (x2 >= 0)
	{
		if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
		{
			if (adjustNeighbors)
			{
				if (passability)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] |= PASS_RIGHT;
				else if ((tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] & PASS_RIGHT) != 0)
					tempLayerData_AutoCalc[y2 * mapWidth + x2].phaseDirections[currentPhase] -= PASS_RIGHT;
			}
			directions |= PASS_LEFT;
		}
	}
	tempLayerData_AutoCalc[y * mapWidth + x].phaseDirections[currentPhase] = directions;
}

void MapView::autoCalcPassabilityDirections(QRect rectangle, bool adjustNeighbors)
{
	for (int y = 0; y < rectangle.height(); ++y)
	{
		for (int x = 0; x < rectangle.width(); ++x)
		{
			if ((x + rectangle.x()) >= 0 && (x + rectangle.x()) < mapWidth && (y + rectangle.y()) >= 0 && (y + rectangle.y()) < mapHeight)
				autoCalcPassabilityDirections(x + rectangle.x(), y + rectangle.y(), adjustNeighbors);
		}
	}
}

void MapView::autoCalcPassabilityDirections(bool adjustNeighbors)
{
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
			autoCalcPassabilityDirections(x, y, adjustNeighbors);
	}
}

unsigned char MapView::getPhaseFlags(int x, int y)
{
	int x2, y2;
	int horizontalMode = (*mapRef)->getHorizontalMode();
	int verticalMode = (*mapRef)->getVerticalMode();
	unsigned char directions = tempLayerData_AutoCalc[y * mapWidth + x].phaseDirections[currentPhase];
	unsigned char flags = 0x0;
	if (passabilityMode == PASSMODE_DIRECTIONS)
	{
		if ((directions & PASS_UP) != 0)
			flags |= Tile::Tile_n;
		if ((directions & PASS_RIGHT) != 0)
			flags |= Tile::Tile_e;
		if ((directions & PASS_DOWN) != 0)
			flags |= Tile::Tile_s;
		if ((directions & PASS_LEFT) != 0)
			flags |= Tile::Tile_w;
	}
	else
	{
		x2 = x - 1;
		y2 = y - 1;
		if (horizontalMode == Map::MODE_WRAPAROUND && x2 < 0)
			x2 += mapWidth;
		if (verticalMode == Map::MODE_WRAPAROUND && y2 < 0)
			y2 += mapHeight;
		if (x2 >= 0 && y2 >= 0)
		{
			if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
				flags |= Tile::Tile_nw;
		}
		x2 = x;
		y2 = y - 1;
		if (verticalMode == Map::MODE_WRAPAROUND && y2 < 0)
			y2 += mapHeight;
		if (y2 >= 0)
		{
			if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
				flags |= Tile::Tile_n;
		}
		x2 = x + 1;
		y2 = y - 1;
		if (horizontalMode == Map::MODE_WRAPAROUND && x2 == mapWidth)
			x2 -= mapWidth;
		if (verticalMode == Map::MODE_WRAPAROUND && y2 < 0)
			y2 += mapHeight;
		if (x2 < mapWidth && y2 >= 0)
		{
			if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
				flags |= Tile::Tile_ne;
		}
		x2 = x - 1;
		y2 = y;
		if (horizontalMode == Map::MODE_WRAPAROUND && x2 < 0)
			x2 += mapWidth;
		if (x2 >= 0)
		{
			if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
				flags |= Tile::Tile_w;
		}
		x2 = x + 1;
		y2 = y;
		if (horizontalMode == Map::MODE_WRAPAROUND && x2 == mapWidth)
			x2 -= mapWidth;
		if (x2 < mapWidth)
		{
			if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
				flags |= Tile::Tile_e;
		}
		x2 = x - 1;
		y2 = y + 1;
		if (horizontalMode == Map::MODE_WRAPAROUND && x2 < 0)
			x2 += mapWidth;
		if (verticalMode == Map::MODE_WRAPAROUND && y2 == mapHeight)
			y2 -= mapHeight;
		if (x2 >= 0 && y2 < mapHeight)
		{
			if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
				flags |= Tile::Tile_sw;
		}
		x2 = x;
		y2 = y + 1;
		if (verticalMode == Map::MODE_WRAPAROUND && y2 == mapHeight)
			y2 -= mapHeight; 
		if (y2 < mapHeight)
		{
			if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
				flags |= Tile::Tile_s;
		}
		x2 = x + 1;
		y2 = y + 1;
		if (horizontalMode == Map::MODE_WRAPAROUND && x2 == mapWidth)
			x2 -= mapWidth;
		if (verticalMode == Map::MODE_WRAPAROUND && y2 == mapHeight)
			y2 -= mapHeight;
		if (x2 < mapWidth && y2 < mapHeight)
		{
			if (tempLayerData_AutoCalc[y2 * mapWidth + x2].phasePassability[currentPhase])
				flags |= Tile::Tile_se;
		}
	}
	return flags;
}

void MapView::drawAreaMarker(QPainter &painter, QImage &markerImage, QPoint location, unsigned char flags)
{
	QRect srcTopLeft, srcTopRight, srcBottomLeft, srcBottomRight;
	QRect dstTopLeft(location.x(), location.y(), 16, 16);
	QRect dstTopRight(location.x() + 16, location.y(), 16, 16);
	QRect dstBottomLeft(location.x(), location.y() + 16, 16, 16);
	QRect dstBottomRight(location.x() + 16, location.y() + 16, 16, 16);
	srcTopLeft = QRect(0, 128, 16, 16);
	srcTopRight = QRect(16, 128, 16, 16);
	srcBottomLeft = QRect(0, 144, 16, 16);
	srcBottomRight = QRect(16, 144, 16, 16);
	if ((flags & Tile::Tile_nw) == 0)
		srcTopLeft = QRect(0, 96, 16, 16);
	if ((flags & Tile::Tile_ne) == 0)
		srcTopRight = QRect(16, 96, 16, 16);
	if ((flags & Tile::Tile_sw) == 0)
		srcBottomLeft = QRect(0, 112, 16, 16);
	if ((flags & Tile::Tile_se) == 0)
		srcBottomRight = QRect(16, 112, 16, 16);
	if ((flags & Tile::Tile_w) == 0)
	{
		srcTopLeft = QRect(0, 32, 16, 16);
		srcBottomLeft = QRect(0, 48, 16, 16);
	}
	if ((flags & Tile::Tile_e) == 0)
	{
		srcTopRight = QRect(16, 32, 16, 16);
		srcBottomRight = QRect(16, 48, 16, 16);
	}
	if ((flags & Tile::Tile_n) == 0)
	{
		srcTopLeft = QRect(0, 64, 16, 16);
		srcTopRight = QRect(16, 64, 16, 16);
	}
	if ((flags & Tile::Tile_s) == 0)
	{
		srcBottomLeft = QRect(0, 80, 16, 16);
		srcBottomRight = QRect(16, 80, 16, 16);
	}
	if ((flags & Tile::Tile_n) == 0 && (flags & Tile::Tile_w) == 0)
		srcTopLeft = QRect(0, 0, 16, 16);
	if ((flags & Tile::Tile_n) == 0 && (flags & Tile::Tile_e) == 0)
		srcTopRight = QRect(16, 0, 16, 16);
	if ((flags & Tile::Tile_s) == 0 && (flags & Tile::Tile_w) == 0)
		srcBottomLeft = QRect(0, 16, 16, 16);
	if ((flags & Tile::Tile_s) == 0 && (flags & Tile::Tile_e) == 0)
		srcBottomRight = QRect(16, 16, 16, 16);
	painter.drawImage(dstTopLeft, markerImage, srcTopLeft);
	painter.drawImage(dstTopRight, markerImage, srcTopRight);
	painter.drawImage(dstBottomLeft, markerImage, srcBottomLeft);
	painter.drawImage(dstBottomRight, markerImage, srcBottomRight);
}

void MapView::renderLayer(int layer, QRect region, QPoint offset, QPainter &painter)
{
	Map::TileData tileData;
	int tileX, tileY;
	int pixelX, pixelY;
	for (int y = 0; y < region.height(); ++y)
	{
		for (int x = 0; x < region.width(); ++x)
		{
			tileX = x + region.x();
			tileY = y + region.y();
			if (tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight)
			{
				pixelX = tileX - offset.x();
				pixelY = tileY - offset.y();
				if (currentTool == TOOL_TILING && layer == currentLayer)
				{
					tileData = tempLayerData_NoAutoCalc[tileY * mapWidth + tileX];
					if (tileData.tilesetID >= tilesets.size())
						painter.drawImage(pixelX * 32, pixelY * 32, invalidTileset);
					else if (tileData.tileID >= (*tilesets[tileData.tilesetID])->numTiles())
						painter.drawImage(pixelX * 32, pixelY * 32, invalidTile);
					else
					{
						(*tilesets[tileData.tilesetID])->getTile(tileData.tileID)->drawTile(painter, QPoint(pixelX * 32, pixelY * 32), tileData.tileFlags);
						painter.resetTransform();
						if ((*tilesets[tileData.tilesetID])->getTile(tileData.tileID)->isAutoTile())
						{
							painter.drawImage(pixelX * 48, pixelY * 48, arrowBG);
							if ((tileData.tileFlags & Tile::Tile_nw) != 0)
								painter.drawImage(pixelX * 48 + 2, pixelY * 48 + 2, arrow_nw);
							if ((tileData.tileFlags & Tile::Tile_n) != 0)
								painter.drawImage(pixelX * 48 + 19, pixelY * 48 + 2, arrow_n);
							if ((tileData.tileFlags & Tile::Tile_ne) != 0)
								painter.drawImage(pixelX * 48 + 35, pixelY * 48 + 2, arrow_ne);
							if ((tileData.tileFlags & Tile::Tile_e) != 0)
								painter.drawImage(pixelX * 48 + 35, pixelY * 48 + 19, arrow_e);
							if ((tileData.tileFlags & Tile::Tile_se) != 0)
								painter.drawImage(pixelX * 48 + 35, pixelY * 48 + 35, arrow_se);
							if ((tileData.tileFlags & Tile::Tile_s) != 0)
								painter.drawImage(pixelX * 48 + 19, pixelY * 48 + 35, arrow_s);
							if ((tileData.tileFlags & Tile::Tile_sw) != 0)
								painter.drawImage(pixelX * 48 + 2, pixelY * 48 + 35, arrow_sw);
							if ((tileData.tileFlags & Tile::Tile_w) != 0)
								painter.drawImage(pixelX * 48 + 2, pixelY * 48 + 19, arrow_w);
						}
						painter.scale(currentZoom / 32.0, currentZoom / 32.0);
					}
				}
				else
				{
					if (layer == currentLayer)
					{
						if (currentTool == TOOL_SELECTION && !selectionData.anchored && selectionRect.contains(tileX, tileY))
							tileData = selectionData.tileData[(tileY - selectionData.y) * selectionData.width + (tileX - selectionData.x)];
						else if (autocalc)
							tileData = tempLayerData_AutoCalc[tileY * mapWidth + tileX];
						else
							tileData = tempLayerData_NoAutoCalc[tileY * mapWidth + tileX];
					}
					else
						tileData = (*mapRef)->getTileData(tileX, tileY, layer);
					if (tileData.tilesetID >= tilesets.size())
						painter.drawImage(pixelX * 32, pixelY * 32, invalidTileset);
					else if (tileData.tileID >= (*tilesets[tileData.tilesetID])->numTiles())
						painter.drawImage(pixelX * 32, pixelY * 32, invalidTile);
					else
						(*tilesets[tileData.tilesetID])->getTile(tileData.tileID)->drawTile(painter, QPoint(pixelX * 32, pixelY * 32), tileData.tileFlags);
				}
			}
		}
	}
}

void MapView::renderEvents(int layer, QRect region, QPoint offset, QPainter &painter)
{
	int numEvents = (*mapRef)->numEvents();
	int tileX, tileY;
	int pixelX, pixelY;
	int eventLayer;
	for (int i = 0; i < numEvents; ++i)
	{
		MapEvent *event = (*mapRef)->getEvent(i);
		MapEvent::Page *page = event->getPage(0);
		tileX = event->getXLocation();
		tileY = event->getYLocation();
		eventLayer = page->getLayer();
		if (region.contains(tileX, tileY) && (eventLayer == layer || layer == -1))
		{
			pixelX = tileX - offset.x();
			pixelY = tileY - offset.y();
			if (currentEditingMode == EDITMODE_PREVIEW)
			{
				QImage eventImage = page->getImage(MapEvent::DIRECTION_DOWN, MapEvent::POSE_IDLE);
				if (!eventImage.isNull())
				{
					if (eventImage.height() == 64)
						painter.drawImage(pixelX * 32 - 8, pixelY * 32 - 32, eventImage);
					else
						painter.drawImage(pixelX * 32, pixelY * 32, eventImage);
				}
			}
			else if (currentEditingMode == EDITMODE_EVENTS)
			{
				QImage eventImage = page->getImage(MapEvent::DIRECTION_DOWN, MapEvent::POSE_IDLE);
				painter.drawImage(pixelX * 32, pixelY * 32, mapEventBG);
				if (!eventImage.isNull())
				{
					if (eventImage.height() == 64)
						painter.drawImage(pixelX * 32 + 7, pixelY * 32 + 4, eventImage.scaled(18, 24));
					else
						painter.drawImage(pixelX * 32 + 4, pixelY * 32 + 4, eventImage.scaled(24, 24));
				}
				painter.drawImage(pixelX * 32, pixelY * 32 + 24, layerText);
				if (eventLayer >= 10)
					painter.drawImage(pixelX * 32 + 16, pixelY * 32 + 24, layerNumbers, (eventLayer / 10) * 8, 0, 8, 8);
				painter.drawImage(pixelX * 32 + 24, pixelY * 32 + 24, layerNumbers, (eventLayer % 10) * 8, 0, 8, 8);
			}
			else
				painter.drawImage(pixelX * 32, pixelY * 32, mapEventMarker);
		}
	}
}

void MapView::renderStartLocations(int layer, QRect region, QPoint offset, QPainter &painter)
{
	int startLayer;
	int tileX, tileY;
	int pixelX, pixelY;
	if (ProjectData::partyStartLocation.mapID == mapRef->getDataID())
	{
		startLayer = ProjectData::partyStartLocation.layer;
		tileX = ProjectData::partyStartLocation.x;
		tileY = ProjectData::partyStartLocation.y;
		if (region.contains(tileX, tileY) && (startLayer == layer || layer == -1))
		{
			pixelX = tileX - offset.x();
			pixelY = tileY - offset.y();
			painter.drawImage(pixelX * 32, pixelY * 32, partyStart);
			painter.drawImage(pixelX * 32, pixelY * 32 + 24, layerText);
			if (startLayer >= 10)
				painter.drawImage(pixelX * 32 + 16, pixelY * 32 + 24, layerNumbers, (startLayer / 10) * 8, 0, 8, 8);
			painter.drawImage(pixelX * 32 + 24, pixelY * 32 + 24, layerNumbers, (startLayer % 10) * 8, 0, 8, 8);
		}
	}
	for (int i = 0; i < ProjectData::vehicleStartLocations.size(); ++i)
	{
		if (ProjectData::vehicleStartLocations[i].mapID == mapRef->getDataID())
		{
			startLayer = ProjectData::vehicleStartLocations[i].layer;
			tileX = ProjectData::vehicleStartLocations[i].x;
			tileY = ProjectData::vehicleStartLocations[i].y;
			if (region.contains(tileX, tileY) && (startLayer == layer || layer == -1))
			{
				pixelX = tileX - offset.x();
				pixelY = tileY - offset.y();
				painter.drawImage(pixelX * 32, pixelY * 32, vehicleStart);
				painter.drawImage(pixelX * 32 + 4, pixelY * 32 + 4, numbers, i * 10, 0, 10, 12);
				painter.drawImage(pixelX * 32, pixelY * 32 + 24, layerText);
				if (startLayer >= 10)
					painter.drawImage(pixelX * 32 + 16, pixelY * 32 + 24, layerNumbers, (startLayer / 10) * 8, 0, 8, 8);
				painter.drawImage(pixelX * 32 + 24, pixelY * 32 + 24, layerNumbers, (startLayer % 10) * 8, 0, 8, 8);
			}
		}
	}
}

void MapView::renderPassability(QRect region, QPoint offset, QPainter &painter)
{
	int tileX, tileY;
	int pixelX, pixelY;
	for (int y = 0; y < region.height(); ++y)
	{
		for (int x = 0; x < region.width(); ++x)
		{
			tileX = x + region.x();
			tileY = y + region.y();
			if (tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight)
			{
				pixelX = tileX - offset.x();
				pixelY = tileY - offset.y();
				if (tempLayerData_AutoCalc[tileY * mapWidth + tileX].phasePassability[currentPhase])
				{
					if (passabilityMode == PASSMODE_DIRECTIONS)
						drawAreaMarker(painter, passabilityMarker, QPoint(pixelX * 32, pixelY * 32), getPhaseFlags(tileX, tileY));
					else
						drawAreaMarker(painter, areaMarker, QPoint(pixelX * 32, pixelY * 32), getPhaseFlags(tileX, tileY));
				}
			}
		}
	}
}
