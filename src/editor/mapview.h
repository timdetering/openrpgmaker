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

#ifndef MAPVIEW_H
#define MAPVIEW_H

class Map;

#include <QGraphicsView>
#include "projectdata.h"
#include "projectdata_map.h"

class MapView : public QGraphicsView
{
	Q_OBJECT
	public:
		MapView(QWidget *parent=0);
		~MapView();
		void setupView();
		void shutdownView();
		void updateTilesetList();
		void updateMapView();
		void updateMapView(QRect rectangle);
		void updateMapView(QPoint location);
		void changeMap(int mapID, bool forceUpdate=false);
		void changeEditingMode(int newMode);
		void changeLayer(int newLayer);
		void changeZoom(int newZoom);
		void changeTool(int newTool);
		void changePhase(int newPhase);
		void setShowGrid(bool show);
		void setPassabilityMode(int mode);
		int getCurrentMap() const;
		int getCurrentTileSelectionWidth() const;
		int getCurrentTileSelectionHeight() const;
		QList<Map::TileData> getCurrentTileSelection() const;
		int getCurrentEditingMode() const;
		int getCurrentLayer() const;
		int getCurrentZoom() const;
		int getCurrentTool() const;
		int getCurrentPhase() const;
		int getMapWidth() const;
		int getMapHeight() const;
		void savePreview(QString filelocation) const;
		enum EditMode{EDITMODE_LAYERS, EDITMODE_PASSABILITY, EDITMODE_EVENTS, EDITMODE_PREVIEW};
		enum Zoom{ZOOM_1TO1=32, ZOOM_1TO2=16, ZOOM_1TO4=8, ZOOM_1TO8=4, ZOOM_3TO2=48};
		enum Tool{TOOL_SELECTION=0, TOOL_RECTANGLE, TOOL_CIRCLE, TOOL_FILL, TOOL_PENCIL, TOOL_TILING};
		enum TilingMode {TILING_ON=0, TILING_OFF, TILING_AUTO};
		enum PassMode {PASSMODE_SET=0, PASSMODE_CLEAR, PASSMODE_DIRECTIONS};
		enum PassDir {PASS_UP=0x1, PASS_RIGHT=0x2, PASS_DOWN=0x4, PASS_LEFT=0x8};
		struct UndoEvent
		{
			QList<Map::TileData> oldData;
			QList<Map::TileData> newData;
			int layer;
		};
		struct SelectionData
		{
			QList<Map::TileData> tileData;
			int x, y;
			int width, height;
			bool anchored;
		};
		bool ignoreContextMenu;
	signals:
		void tileSelectionChanged(int selectionWidth, int selectionHeight, QList<Map::TileData> selectionData);
		void mouseDoubleClicked(QMouseEvent *event);
		void mouseMoved(QMouseEvent *event);
		void mouseButtonPressed(QMouseEvent *event);
		void mouseButtonReleased(QMouseEvent *event);
		void undoEnabled(bool yes);
		void redoEnabled(bool yes);
	protected slots:
		void changeTileSelection(int selectionWidth, int selectionHeight, QList<Map::TileData> selectionData);
		void scrollArea();
		void scrollBarMoved();
		void undo();
		void redo();
		void animateSelection();
		void copySelection();
		void cutSelection();
		void pasteSelection();
		void deleteSelection();
		void duplicateSelection();
		void clearSelection();
		void anchorSelection();
		void detachSelection();
		void newEvent();
		void editEvent();
		void cutEvent();
		void copyEvent();
		void pasteEvent();
		void deleteEvent();
		void placeTeleportEvent();
		void placePartyStartLocation();
		void placeVehicleStartLocation();
		void moveUpLayer();
		void moveDownLayer();
		void cropToSelection();
	protected:
		void contextMenuEvent(QContextMenuEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);
		void leaveEvent(QEvent *event);
		void mouseDoubleClickEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void resizeEvent(QResizeEvent *event);
		QRect addRectData();
		QRect subRectData();
		QRect addCircleData();
		void applyTempLayerData();
		void fill(int x, int y, bool autocalcflags);
		void autoCalcFlags(int x, int y, bool adjustNeighbors);
		void autoCalcFlags(QRect rectangle, bool adjustNeighbors);
		void autoCalcFlags(bool adjustNeighbors);
		void autoCalcPassabilityDirections(int x, int y, bool adjustNeighbors);
		void autoCalcPassabilityDirections(QRect rectangle, bool adjustNeighbors);
		void autoCalcPassabilityDirections(bool adjustNeighbors);
		unsigned char getPhaseFlags(int x, int y);
		void drawAreaMarker(QPainter &painter, QImage &markerImage, QPoint location, unsigned char flags);
		void renderLayer(int layer, QRect region, QPoint offset, QPainter &painter);
		void renderEvents(int layer, QRect region, QPoint offset, QPainter &painter);
		void renderStartLocations(int layer, QRect region, QPoint offset, QPainter &painter);
		void renderPassability(QRect region, QPoint offset, QPainter &painter);
		QAction *aCopy;
		QAction *aCut;
		QAction *aPaste;
		QAction *aDelete;
		QAction *aDuplicate;
		QAction *aClear;
		QAction *aAnchor;
		QAction *aDetach;
		QAction *aNewEvent;
		QAction *aEditEvent;
		QAction *aCutEvent;
		QAction *aCopyEvent;
		QAction *aPasteEvent;
		QAction *aDeleteEvent;
		QAction *aPlaceTeleportEvent;
		QAction *aPlacePartyStartLocation;
		QAction *aPlaceVehicleStartLocation;
		QAction *aMoveUpLayer;
		QAction *aMoveDownLayer;
		QAction *aCropToSelection;
		QGraphicsPixmapItem *item;
		QGraphicsRectItem *currentSelection;
		QImage mapViewImage;
		QImage arrowBG;
		QImage arrow_nw;
		QImage arrow_n;
		QImage arrow_ne;
		QImage arrow_e;
		QImage arrow_se;
		QImage arrow_s;
		QImage arrow_sw;
		QImage arrow_w;
		QImage areaMarker;
		QImage layerText;
		QImage layerNumbers;
		QImage invalidTile;
		QImage invalidTileset;
		QImage grid;
		QImage selectedEvent;
		QImage mapEventMarker;
		QImage mapEventBG;
		QImage partyStart;
		QImage passabilityMarker;
		QImage vehicleStart;
		QImage numbers;
		QTimer *timer;
		QTimer *selectionTimer;
		QPen selectionPen;
		QRect selectionRect;
		QPoint rectStart;
		QPoint rectEnd;
		QPoint scrollEvent;
		QList<UndoEvent> undoHistory;
		QList<Map::TileData> tempLayerData_AutoCalc;
		QList<Map::TileData> tempLayerData_NoAutoCalc;
		QList<Map::TileData> tileSelection;
		QList<ProjectData::DataReference<Tileset>*> tilesets;
		ProjectData::DataReference<Map> *mapRef;
		SelectionData selectionData;
		int tileSelectionWidth;
		int tileSelectionHeight;
		int currentEditingMode;
		int currentLayer;
		int currentZoom;
		int currentTool;
		int currentPhase;
		int mapWidth;
		int mapHeight;
		int eventID;
		int startLocationID;
		int undoLocation;
		int selectionDashOffset;
		int currentMapID;
		int tilingMode;
		int passabilityMode;
		bool ignoreevents;
		bool waitevents;
		bool autocalc;
		bool canceled;
		bool decrementUndoLocation;
		bool incrementUndoLocation;
		bool shutdown;
		bool showGrid;
};

#endif // MAPVIEW_H
