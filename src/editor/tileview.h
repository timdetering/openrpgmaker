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

#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QGraphicsView>
#include "clipboard.h"
#include "projectdata.h"
#include "projectdata_map.h"

class TileView : public QGraphicsView
{
	Q_OBJECT
	public:
		TileView(QWidget *parent=0);
		~TileView();
		void setupView();
		void shutdownView();
		void updateTileView();
		void updateTileView(int tileID);
		void changeTileset(int newTileset, bool forceupdate=false);
		void changeSelection(QRect newSelection);
		void changeSelection(int tileID);
		void changeView(int newView);
		void changeTerrain(int newID);
		void changeTileGroup(int newGroup);
		void changeFPS(int newFPS);
		void changePhase(int newPhase);
		int getCurrentTilesetID() const;
		QRect getCurrentSelection() const;
		int getCurrentView() const;
		int getCurrentTerrain() const;
		int getCurrentTileGroup() const;
		int getCurrentFPS() const;
		int getCurrentPhase() const;
		void scrollToTile(int tileID);
		enum View {VIEW_NORMAL=0, VIEW_TERRAIN, VIEW_PHASEPASSABILITY, VIEW_ADJUSTLAYOUT, VIEW_TILINGGROUPS, VIEW_FPS, VIEW_PINGPONG, VIEW_CUSTOM};
		enum PassDir {PASS_UP=0x1, PASS_RIGHT=0x2, PASS_DOWN=0x4, PASS_LEFT=0x8};
		bool ignoreContextMenu;
	signals:
		void tileSelectionChanged(int selectionWidth, int selectionHeight, QList<Map::TileData> selectionData);
		void mouseDoubleClicked(QMouseEvent *event);
		void mouseMoved(QMouseEvent *event);
		void mouseButtonPressed(QMouseEvent *event, int tileID);
		void mouseButtonReleased(QMouseEvent *event, int tileID);
		void scrollBarRangeChanged(int min, int max);
		void scrollBarValueChanged(int value);
		void tilesetChanged(int tilesetID);
		void tilesSwapped(int oldID, int newID);
		void changePreviewTile(int tileID);
	public slots:
		void changeTileSelection(int selectionWidth, int selectionHeight, QList<Map::TileData> selectionData);
		void setScrollBarRange(int min, int max);
		void setScrollBarValue(int value);
	protected:
		//void mouseDoubleClickEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void leaveEvent(QEvent *event);
		QGraphicsScene *tileScene;
		QList<QGraphicsPixmapItem*> tileItems;
		QGraphicsRectItem *currentSelection;
		ProjectData::DataReference<Tileset> *tilesetRef;
		QImage numbers;
		QImage numbersHighlighted;
		QImage passability;
		QImage leftArrow;
		QImage rightArrow;
		QImage upArrow;
		QImage downArrow;
		QImage arrowBG;
		QPoint rectStart;
		QPoint rectEnd;
		int mouseCursorTileID;
		int currentView;
		int currentTileID;
		int currentTerrain;
		int currentTileGroup;
		int currentFPS;
		int currentPhase;
		bool ignoreevents;
		bool shutdown;
};

#endif // TILEVIEW_H
