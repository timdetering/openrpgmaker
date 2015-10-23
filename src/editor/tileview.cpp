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
#include <QMouseEvent>
#include <cmath>
#include "clipboard.h"
#include "projectdata.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "tileview.h"

TileView::TileView(QWidget *parent) : QGraphicsView(parent)
{
	tileScene = new QGraphicsScene(0.0, 0.0, 0.0, 0.0);
	setScene(tileScene);
	for (int i = 0; i < 256; ++i)
	{
		QGraphicsPixmapItem *item = new QGraphicsPixmapItem();
		item->setFlags(0x0);
		item->setVisible(false);
		item->setPos((i % 6) * 32.0, (i / 6) * 32.0);
		tileScene->addItem(item);
		tileItems.append(item);
	}
	QPen pen(Qt::DashLine);
	pen.setColor(QColor(Qt::cyan));
	pen.setWidth(3);
	currentSelection = new QGraphicsRectItem(1.0, 1.0, 30.0, 30.0);
	currentSelection->setZValue(1.0);
	currentSelection->setPen(pen);
	currentSelection->setBrush(QBrush(Qt::transparent));
	currentSelection->setVisible(false);
	tileScene->addItem(currentSelection);
	tilesetRef = NULL;
	mouseCursorTileID = -1;
	currentView = VIEW_NORMAL;
	currentTileID = -1;
	currentTileGroup = -1;
	currentFPS = 8;
	ignoreevents = true;
	ignoreContextMenu = true;
	rectStart = QPoint(0, 0);
	rectEnd = QPoint(0, 0);
	setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	QObject::connect(verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SIGNAL(scrollBarRangeChanged(int, int)));
	QObject::connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(scrollBarValueChanged(int)));
	shutdown = true;
}

TileView::~TileView()
{
	if (tilesetRef != NULL)
	{
		delete tilesetRef;
		tilesetRef = NULL;
	}
}

void TileView::setupView()
{
	if (ProjectData::open)
	{
		ignoreevents = false;
		ignoreContextMenu = false;
		numbers.load(":/icons/numbers.png");
		numbersHighlighted.load(":/icons/numbersHighlighted.png");
		passability.load(":/icons/pass.png");
		leftArrow.load(":/icons/leftarrow.png");
		rightArrow.load(":/icons/rightarrow.png");
		upArrow.load(":/icons/uparrow.png");
		downArrow.load(":/icons/downarrow.png");
		arrowBG.load(":/icons/arrowBG.png");
		currentTileID = -1;
		updateTileView();
		ensureVisible(tileItems[0], 0, 0);
		shutdown = false;
	}
}

void TileView::shutdownView()
{
	if (tilesetRef != NULL)
	{
		delete tilesetRef;
		tilesetRef = NULL;
	}
	ignoreevents = true;
	ignoreContextMenu = true;
	for (int i = 0; i < 256; ++i)
		tileItems[i]->setVisible(false);
	currentSelection->setVisible(false);
	currentTileID = -1;
	shutdown = true;
}

void TileView::updateTileView()
{
	QImage image;
	QPainter painter;
	QString tempString;
	int i, numtiles = (*tilesetRef)->numTiles();
	if (currentView == VIEW_NORMAL)
		currentSelection->setVisible(true);
	else if (currentView == VIEW_ADJUSTLAYOUT && currentTileID != -1)
		currentSelection->setVisible(true);
	else
		currentSelection->setVisible(false);
	for (i = 0; i < 256; ++i)
	{
		if (i < numtiles)
		{
			image = QImage(32, 32, QImage::Format_ARGB32);
			image.fill(Qt::transparent);
			painter.begin(&image);
			(*tilesetRef)->getTile(i)->drawTile(painter, QPoint(0, 0));
			if (currentView == VIEW_TERRAIN)
			{
				tempString = QString::number((*tilesetRef)->getTile(i)->getTerrainID());
				for (int j = 0; j < tempString.size(); ++j)
				{
					int digit = tempString[j].digitValue();
					if (mouseCursorTileID == i)
						painter.drawImage(QRect(((32 - (tempString.size() * 8)) / 2) + (8 * j), 12, 8, 8), numbersHighlighted, QRect(8 * digit, 0, 8, 8));
					else
						painter.drawImage(QRect(((32 - (tempString.size() * 8)) / 2) + (8 * j), 12, 8, 8), numbers, QRect(8 * digit, 0, 8, 8));
				}
			}
			else if (currentView == VIEW_PHASEPASSABILITY)
			{
				unsigned char passability = (*tilesetRef)->getTile(i)->getPhasePassability(currentPhase);
				painter.drawImage(QPoint(0, 0), arrowBG);
				if ((passability & PASS_UP) != 0)
					painter.drawImage(QPoint(11, 0), upArrow);
				if ((passability & PASS_RIGHT) != 0)
					painter.drawImage(QPoint(21, 11), rightArrow);
				if ((passability & PASS_DOWN) != 0)
					painter.drawImage(QPoint(11, 21), downArrow);
				if ((passability & PASS_LEFT) != 0)
					painter.drawImage(QPoint(0, 11), leftArrow);
			}
			else if (currentView == VIEW_TILINGGROUPS)
			{
				tempString = QString::number((*tilesetRef)->getTile(i)->getTileGroup());
				for (int j = 0; j < tempString.size(); ++j)
				{
					int digit = tempString[j].digitValue();
					if (tempString[j] == QChar('-'))
						digit = 10;
					if (mouseCursorTileID == i)
						painter.drawImage(QRect(((32 - (tempString.size() * 8)) / 2) + (8 * j), 12, 8, 8), numbersHighlighted, QRect(8 * digit, 0, 8, 8));
					else
						painter.drawImage(QRect(((32 - (tempString.size() * 8)) / 2) + (8 * j), 12, 8, 8), numbers, QRect(8 * digit, 0, 8, 8));
				}
			}
			else if (currentView == VIEW_FPS && (*tilesetRef)->getTile(i)->isAnimatedTile())
			{
				int fps = (*tilesetRef)->getTile(i)->getFPS();
				if (fps < 10)
				{
					if (mouseCursorTileID == i)
						painter.drawImage(QRect(12, 12, 8, 8), numbersHighlighted, QRect(fps * 8, 0, 8, 8));
					else
						painter.drawImage(QRect(12, 12, 8, 8), numbers, QRect(fps * 8, 0, 8, 8));
				}
				else
				{
					if (mouseCursorTileID == i)
					{
						painter.drawImage(QRect(8, 12, 8, 8), numbersHighlighted, QRect((fps / 10) * 8, 0, 8, 8));
						painter.drawImage(QRect(16, 12, 8, 8), numbersHighlighted, QRect((fps % 10) * 8, 0, 8, 8));
					}
					else
					{
						painter.drawImage(QRect(8, 12, 8, 8), numbers, QRect((fps / 10) * 8, 0, 8, 8));
						painter.drawImage(QRect(16, 12, 8, 8), numbers, QRect((fps % 10) * 8, 0, 8, 8));
					}
				}
			}
			else if (currentView == VIEW_PINGPONG && (*tilesetRef)->getTile(i)->isAnimatedTile())
				painter.drawImage(QRect(0, 0, 32, 32), passability, QRect(((*tilesetRef)->getTile(i)->isPingPong()) ? 0:32, 0, 32, 32));
			painter.end();
			tileItems[i]->setPixmap(QPixmap::fromImage(image));
			tileItems[i]->setVisible(true);
		}
		else
			tileItems[i]->setVisible(false);
	}
	tileScene->setSceneRect(0.0, 0.0, (numtiles >= 6) ? 192.0:((numtiles % 6) * 32.0), (ceil(numtiles / 6.0)) * 32.0);
}

void TileView::updateTileView(int tileID)
{
	QImage image;
	QPainter painter;
	QString tempString;
	int numtiles = (*tilesetRef)->numTiles();
	if (currentView == VIEW_NORMAL)
		currentSelection->setVisible(true);
	else if (currentView == VIEW_ADJUSTLAYOUT && currentTileID != -1)
		currentSelection->setVisible(true);
	else
		currentSelection->setVisible(false);
	if (tileID < numtiles)
	{
		image = QImage(32, 32, QImage::Format_ARGB32);
		image.fill(Qt::transparent);
		painter.begin(&image);
		(*tilesetRef)->getTile(tileID)->drawTile(painter, QPoint(0, 0));
		if (currentView == VIEW_TERRAIN)
		{
			tempString = QString::number((*tilesetRef)->getTile(tileID)->getTerrainID());
			for (int j = 0; j < tempString.size(); ++j)
			{
				int digit = tempString[j].digitValue();
				if (mouseCursorTileID == tileID)
					painter.drawImage(QRect(((32 - (tempString.size() * 8)) / 2) + (8 * j), 12, 8, 8), numbersHighlighted, QRect(8 * digit, 0, 8, 8));
				else
					painter.drawImage(QRect(((32 - (tempString.size() * 8)) / 2) + (8 * j), 12, 8, 8), numbers, QRect(8 * digit, 0, 8, 8));
			}
		}
		else if (currentView == VIEW_PHASEPASSABILITY)
		{
			unsigned char passability = (*tilesetRef)->getTile(tileID)->getPhasePassability(currentPhase);
			painter.drawImage(QPoint(0, 0), arrowBG);
			if ((passability & PASS_UP) != 0)
				painter.drawImage(QPoint(11, 0), upArrow);
			if ((passability & PASS_RIGHT) != 0)
				painter.drawImage(QPoint(21, 11), rightArrow);
			if ((passability & PASS_DOWN) != 0)
				painter.drawImage(QPoint(11, 21), downArrow);
			if ((passability & PASS_LEFT) != 0)
				painter.drawImage(QPoint(0, 11), leftArrow);
		}
		else if (currentView == VIEW_TILINGGROUPS)
		{
			tempString = QString::number((*tilesetRef)->getTile(tileID)->getTileGroup());
			for (int j = 0; j < tempString.size(); ++j)
			{
				int digit = tempString[j].digitValue();
				if (tempString[j] == QChar('-'))
					digit = 10;
				if (mouseCursorTileID == tileID)
					painter.drawImage(QRect(((32 - (tempString.size() * 8)) / 2) + (8 * j), 12, 8, 8), numbersHighlighted, QRect(8 * digit, 0, 8, 8));
				else
					painter.drawImage(QRect(((32 - (tempString.size() * 8)) / 2) + (8 * j), 12, 8, 8), numbers, QRect(8 * digit, 0, 8, 8));
			}
		}
		else if (currentView == VIEW_FPS && (*tilesetRef)->getTile(tileID)->isAnimatedTile())
		{
			int fps = (*tilesetRef)->getTile(tileID)->getFPS();
			if (fps < 10)
			{
				if (mouseCursorTileID == tileID)
					painter.drawImage(QRect(12, 12, 8, 8), numbersHighlighted, QRect(fps * 8, 0, 8, 8));
				else
					painter.drawImage(QRect(12, 12, 8, 8), numbers, QRect(fps * 8, 0, 8, 8));
			}
			else
			{
				if (mouseCursorTileID == tileID)
				{
					painter.drawImage(QRect(8, 12, 8, 8), numbersHighlighted, QRect((fps / 10) * 8, 0, 8, 8));
					painter.drawImage(QRect(16, 12, 8, 8), numbersHighlighted, QRect((fps % 10) * 8, 0, 8, 8));
				}
				else
				{
					painter.drawImage(QRect(8, 12, 8, 8), numbers, QRect((fps / 10) * 8, 0, 8, 8));
					painter.drawImage(QRect(16, 12, 8, 8), numbers, QRect((fps % 10) * 8, 0, 8, 8));
				}
			}
		}
		else if (currentView == VIEW_PINGPONG && (*tilesetRef)->getTile(tileID)->isAnimatedTile())
			painter.drawImage(QRect(0, 0, 32, 32), passability, QRect(((*tilesetRef)->getTile(tileID)->isPingPong()) ? 0:32, 0, 32, 32));
		painter.end();
		tileItems[tileID]->setPixmap(QPixmap::fromImage(image));
		tileItems[tileID]->setVisible(true);
	}
	else
		tileItems[tileID]->setVisible(false);
	tileScene->setSceneRect(0.0, 0.0, (numtiles >= 6) ? 192.0:((numtiles % 6) * 32.0), (ceil(numtiles / 6.0)) * 32.0);
}

void TileView::changeTileset(int newTileset, bool forceupdate)
{
	if (tilesetRef != NULL)
	{
		if (tilesetRef->getDataID() == newTileset && !forceupdate)
			return;
		delete tilesetRef;
		tilesetRef = NULL;
	}
	tilesetRef = new ProjectData::DataReference<Tileset>(newTileset, __FILE__, __LINE__);
	if (!shutdown)
	{
		QList<Map::TileData> selectionData;
		Map::TileData tileData;
		tileData.tilesetID = (unsigned char)(tilesetRef->getDataID());
		tileData.tileID = 0x0;
		tileData.tileFlags = 0xff;
		if ((*tilesetRef)->numTiles() > 0)
		{
			for (int i = 0; i < NUM_PHASES; ++i)
			{
				tileData.phaseDirections[i] = (*tilesetRef)->getTile(0)->getPhaseDirections(i);
				tileData.phasePassability[i] = (*tilesetRef)->getTile(0)->getPhasePassability(i);
			}
		}
		else
		{
			for (int i = 0; i < NUM_PHASES; ++i)
			{
				tileData.phaseDirections[i] = 0x0;
				tileData.phasePassability[i] = false;
			}
		}
		selectionData.append(tileData);
		tileSelectionChanged(1, 1, selectionData);
		currentSelection->setRect(QRectF(1.0, 1.0, 30.0, 30.0));
		if (!ignoreevents)
			updateTileView();
	}
}

void TileView::changeSelection(QRect newSelection)
{
	currentSelection->setRect(QRectF((newSelection.x() * 32.0) + 1.0, (newSelection.y() * 32.0) + 1.0, (newSelection.width() * 32.0) - 2.0, (newSelection.height() * 32.0) - 2.0));
}

void TileView::changeSelection(int tileID)
{
	currentSelection->setRect(QRectF(((tileID % 6) * 32.0) + 1.0, ((tileID / 6) * 32.0) + 1.0, 30.0, 30.0));
}

void TileView::changeView(int newView)
{
	if (currentView != newView && !shutdown)
	{
		currentView = newView;
		if (currentView == VIEW_NORMAL)
			currentSelection->setVisible(true);
		else if (currentView == VIEW_ADJUSTLAYOUT)
		{
			currentTileID = -1;
			currentSelection->setVisible(false);
		}
		else
			currentSelection->setVisible(false);
		if (!ignoreevents)
			updateTileView();
	}
}

void TileView::changeTerrain(int newID)
{
	currentTerrain = newID;
}

void TileView::changeTileGroup(int newGroup)
{
	currentTileGroup = newGroup;
}

void TileView::changeFPS(int newFPS)
{
	currentFPS = newFPS;
}

void TileView::changePhase(int newPhase)
{
	if (currentPhase != newPhase && !shutdown)
	{
		currentPhase = newPhase;
		if (currentView == VIEW_PHASEPASSABILITY && !ignoreevents)
			updateTileView();
	}
	currentPhase = newPhase;
}

int TileView::getCurrentTilesetID() const
{
	if (tilesetRef != NULL)
		return tilesetRef->getDataID();
	return -1;
}

QRect TileView::getCurrentSelection() const
{
	QRectF selection(currentSelection->rect());
	return QRect(int((selection.x() - 1.0) / 32.0), int((selection.y() - 1.0) / 32.0), int((selection.width() + 2.0) / 32.0), int((selection.height() + 2.0) / 32.0));
}

int TileView::getCurrentView() const
{
	return currentView;
}

int TileView::getCurrentTerrain() const
{
	return currentTerrain;
}

int TileView::getCurrentTileGroup() const
{
	return currentTileGroup;
}

int TileView::getCurrentFPS() const
{
	return currentFPS;
}

int TileView::getCurrentPhase() const
{
	return currentPhase;
}

void TileView::scrollToTile(int tileID)
{
	ensureVisible(tileItems[tileID], 0, 0);
}

void TileView::changeTileSelection(int selectionWidth, int selectionHeight, QList<Map::TileData> selectionData)
{
	QList<unsigned char> expectedTileIDs;
	QRect selectionRect;
	QPoint tileLocation;
	unsigned char tilesetID1, tilesetID2;
	unsigned char tileID;
	int numTilesets, numTiles, viewWidth, viewHeight, newTilesetID;
	bool valid = true;
	bool update = false;
	if (selectionData.size() == 0)
		valid = false;
	else
		tilesetID1 = selectionData[0].tilesetID;
	numTilesets = ProjectData::tilesetNames.size();
	if (tilesetID1 >= numTilesets)
		valid = false;
	// Check to make sure all the tilesets match
	for (int y = 0; y < selectionHeight && valid; ++y)
	{
		for (int x = 0; x < selectionWidth && valid; ++x)
		{
			tilesetID2 = selectionData[y * selectionWidth + x].tilesetID;
			if (tilesetID1 != tilesetID2)
				valid = false;
		}
	}
	if (valid) // If all tilesets match -> change combo box to the new tilesetID
	{
		ignoreevents = true;
		newTilesetID = (int)selectionData[0].tilesetID;
		if (tilesetRef->getDataID() != newTilesetID)
			update = true;
		if (tilesetRef != NULL)
		{
			delete tilesetRef;
			tilesetRef = NULL;
		}
		tilesetRef = new ProjectData::DataReference<Tileset>(newTilesetID, __FILE__, __LINE__);
		numTiles = (*tilesetRef)->numTiles();
		if (numTiles > 6)
			viewWidth = 6;
		else
			viewWidth = numTiles;
		viewHeight = int(ceil(numTiles / 6.0));
		tilesetChanged(newTilesetID);
		ignoreevents = false;
		tileID = selectionData[0].tileID;
		selectionRect.setX(tileID % 6);
		selectionRect.setY(tileID / 6);
		selectionRect.setWidth(selectionWidth);
		selectionRect.setHeight(selectionHeight);
		// Check to make sure the new selection fits in the view
		for (int y = 0; y < selectionRect.height() && valid; ++y)
		{
			for (int x = 0; x < selectionRect.width() && valid; ++x)
			{
				tileLocation.setX(x + selectionRect.x());
				tileLocation.setY(y + selectionRect.y());
				tileID = (unsigned char)(tileLocation.y() * 6 + tileLocation.x());
				if (tileLocation.x() >= viewWidth || tileLocation.y() >= viewHeight || (int)tileID >= numTiles)
					valid = false;
			}
		}
		if (valid) // If the new selection fits in the view -> create list of the expected tileIDs
		{
			for (int y = 0; y < selectionRect.height(); ++y)
			{
				for (int x = 0; x < selectionRect.width(); ++x)
				{
					int viewX = (unsigned char)(x + selectionRect.x());
					int viewY = (unsigned char)(y + selectionRect.y());
					expectedTileIDs.append((unsigned char)(viewY * 6 + viewX));
				}
			}
			// Check to make sure the selected tileIDs match the expected tileIDs
			for (int i = 0; i < selectionData.size() && valid; ++i)
			{
				tileID = selectionData[i].tileID;
				if (tileID != expectedTileIDs[i])
					valid = false;
			}
			if (valid) // If selected tileIDs == expected tileIDs -> set selection item to reflect the selection rect
			{
				currentSelection->setRect((selectionRect.x() * 32.0) + 1.0, (selectionRect.y() * 32.0) + 1.0, (selectionRect.width() * 32.0) - 2.0, (selectionRect.height() * 32.0) - 2.0);
				currentSelection->setVisible(true);
			}
		}
	}
	if (!valid) // If any of the checks failed -> hide selection item
		currentSelection->setVisible(false);
	if (update && !ignoreevents)
		updateTileView();
}

void TileView::setScrollBarRange(int min, int max)
{
	if (min != verticalScrollBar()->minimum() || max != verticalScrollBar()->maximum())
		verticalScrollBar()->setRange(min, max);
}

void TileView::setScrollBarValue(int value)
{
	if (value != verticalScrollBar()->value())
		verticalScrollBar()->setValue(value);
}

/*void TileView::mouseDoubleClickEvent(QMouseEvent *event)
{
	QGraphicsView::mouseDoubleClickEvent(event);
	if (!ignoreevents)
		mouseDoubleClicked(event);
}*/

void TileView::mouseMoveEvent(QMouseEvent *event)
{
	if (!ignoreevents)
	{
		if (currentView != VIEW_NORMAL)
		{
			QRect rect;
			QPointF pos = mapToScene(event->pos());
			int tileID, numtiles = (*tilesetRef)->numTiles();
			if (pos.x() < 0.0)
				pos.setX(0.0);
			else if (pos.x() >= ((numtiles >= 6) ? 191.0:((numtiles % 6) * 32.0 - 1.0)))
				pos.setX((numtiles >= 6) ? 191.0:((numtiles % 6) * 32.0 - 1.0));
			if (pos.y() < 0.0)
				pos.setY(0.0);
			else if (pos.y() > (ceil(numtiles / 6.0)) * 32.0 - 16.0)
				pos.setY((ceil(numtiles / 6.0)) * 32.0 - 16.0);
			tileID = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
			if (tileID != mouseCursorTileID)
			{
				int oldID = mouseCursorTileID;
				mouseCursorTileID = tileID;
				if (currentView == VIEW_TERRAIN || currentView == VIEW_TILINGGROUPS || currentView == VIEW_FPS)
				{
					if (oldID >= 0 && oldID < numtiles)
						updateTileView(oldID);
					if (mouseCursorTileID >= 0 && mouseCursorTileID < numtiles)
						updateTileView(mouseCursorTileID);
				}
			}
		}
		if ((event->buttons() & Qt::LeftButton) != 0 && currentView == VIEW_NORMAL)
		{
			QRect rect;
			QPointF pos = mapToScene(event->pos());
			int numtiles = (*tilesetRef)->numTiles();
			if (pos.x() < 0.0)
				pos.setX(0.0);
			else if (pos.x() >= ((numtiles >= 6) ? 191.0:((numtiles % 6) * 32.0 - 1.0)))
				pos.setX((numtiles >= 6) ? 191.0:((numtiles % 6) * 32.0 - 1.0));
			if (pos.y() < 0.0)
				pos.setY(0.0);
			else if (pos.y() > (ceil(numtiles / 6.0)) * 32.0 - 16.0)
				pos.setY((ceil(numtiles / 6.0)) * 32.0 - 16.0);
			rectEnd = QPoint(int(pos.x()) / 32, int(pos.y()) / 32);
			if (rectStart.x() < rectEnd.x())
			{
				rect.setLeft(rectStart.x());
				rect.setRight(rectEnd.x());
				if (rect.width() > 6)
				{
					rect.setRight(rectStart.x() + 5);
					rectEnd.setX(rectStart.x() + 5);
				}
			}
			else
			{
				rect.setLeft(rectEnd.x());
				rect.setRight(rectStart.x());
				if (rect.width() > 6)
				{
					rect.setLeft(rectStart.x() - 5);
					rectEnd.setX(rectStart.x() - 5);
				}
			}
			if (rectStart.y() < rectEnd.y())
			{
				rect.setTop(rectStart.y());
				rect.setBottom(rectEnd.y());
				if (rect.height() > 6)
				{
					rect.setBottom(rectStart.y() + 5);
					rectEnd.setY(rectStart.y() + 5);
				}
			}
			else
			{
				rect.setTop(rectEnd.y());
				rect.setBottom(rectStart.y());
				if (rect.height() > 6)
				{
					rect.setTop(rectStart.y() - 5);
					rectEnd.setY(rectStart.y() - 5);
				}
			}
			currentSelection->setRect((rect.x() * 32.0) + 1.0, (rect.y() * 32.0) + 1.0, (rect.width() * 32.0) - 2.0, (rect.height() * 32.0) - 2.0);
		}
		mouseMoved(event);
	}
}

void TileView::mousePressEvent(QMouseEvent *event)
{
	if (!ignoreevents)
	{
		QPointF pos = mapToScene(event->pos());
		int tileID, numtiles = (*tilesetRef)->numTiles();
		if (event->button() == Qt::LeftButton)
		{
			if (currentView == VIEW_NORMAL)
			{
				QRect rect;
				if (pos.x() < 0.0)
					pos.setX(0.0);
				else if (pos.x() >= ((numtiles >= 6) ? 191.0:((numtiles % 6) * 32.0 - 1.0)))
					pos.setX((numtiles >= 6) ? 191.0:((numtiles % 6) * 32.0 - 1.0));
				if (pos.y() < 0.0)
					pos.setY(0.0);
				else if (pos.y() > (ceil(numtiles / 6.0)) * 32.0 - 16.0)
					pos.setY((ceil(numtiles / 6.0)) * 32.0 - 16.0);
				tileID = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
				if (tileID >= 0 && tileID < numtiles)
				{
					rectStart = QPoint(int(pos.x()) / 32, int(pos.y()) / 32);
					rectEnd = rectStart;
					if (rectStart.x() < rectEnd.x())
					{
						rect.setLeft(rectStart.x());
						rect.setRight(rectEnd.x());
						if (rect.width() > 6)
						{
							rect.setRight(rectStart.x() + 5);
							rectEnd.setX(rectStart.x() + 5);
						}
					}
					else
					{
						rect.setLeft(rectEnd.x());
						rect.setRight(rectStart.x());
						if (rect.width() > 6)
						{
							rect.setLeft(rectStart.x() - 5);
							rectEnd.setX(rectStart.x() - 5);
						}
					}
					if (rectStart.y() < rectEnd.y())
					{
						rect.setTop(rectStart.y());
						rect.setBottom(rectEnd.y());
						if (rect.height() > 6)
						{
							rect.setBottom(rectStart.y() + 5);
							rectEnd.setY(rectStart.y() + 5);
						}
					}
					else
					{
						rect.setTop(rectEnd.y());
						rect.setBottom(rectStart.y());
						if (rect.height() > 6)
						{
							rect.setTop(rectStart.y() - 5);
							rectEnd.setY(rectStart.y() - 5);
						}
					}
					currentSelection->setRect((rect.x() * 32.0) + 1.0, (rect.y() * 32.0) + 1.0, (rect.width() * 32.0) - 2.0, (rect.height() * 32.0) - 2.0);
					currentSelection->setVisible(true);
				}
			}
			else if (currentView == VIEW_TERRAIN)
			{
				tileID = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
				if (tileID >= 0 && tileID < numtiles)
				{
					(*tilesetRef)->getTile(tileID)->setTerrainID(currentTerrain);
					updateTileView(tileID);
				}
			}
			else if (currentView == VIEW_PHASEPASSABILITY)
			{
				QPoint tilePos = QPoint((int)pos.x() % 32, (int)pos.y() % 32);
				QRect upArrowRect = QRect(11, 0, 10, 11);
				QRect rightArrowRect = QRect(21, 11, 11, 10);
				QRect downArrowRect = QRect(11, 21, 10, 11);
				QRect leftArrowRect = QRect(0, 11, 11, 10);
				tileID = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
				if (tileID >= 0 && tileID < numtiles)
				{
					unsigned char phasePassability = (*tilesetRef)->getTile(tileID)->getPhasePassability(currentPhase);
					if (upArrowRect.contains(tilePos))
						phasePassability ^= PASS_UP;
					else if (rightArrowRect.contains(tilePos))
						phasePassability ^= PASS_RIGHT;
					else if (downArrowRect.contains(tilePos))
						phasePassability ^= PASS_DOWN;
					else if (leftArrowRect.contains(tilePos))
						phasePassability ^= PASS_LEFT;
					(*tilesetRef)->getTile(tileID)->setPhasePassability(currentPhase, phasePassability);
					updateTileView(tileID);
				}
			}
			else if (currentView == VIEW_ADJUSTLAYOUT)
			{
				tileID = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
				if (tileID >= 0 && tileID < numtiles)
				{
					if (currentTileID == -1)
					{
						currentTileID = tileID;
						currentSelection->setRect(((currentTileID % 6) * 32.0) + 1, ((currentTileID / 6) * 32.0) + 1, 30.0, 30.0);
						currentSelection->setVisible(true);
					}
					else
					{
						(*tilesetRef)->swapTiles(currentTileID, tileID);
						emit tilesSwapped(currentTileID, tileID);
						updateTileView(tileID);
						updateTileView(currentTileID);
						currentTileID = -1;
						currentSelection->setVisible(false);
					}
				}
			}
			else if (currentView == VIEW_TILINGGROUPS)
			{
				tileID = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
				if (tileID >= 0 && tileID < numtiles)
				{
					(*tilesetRef)->getTile(tileID)->setTileGroup(currentTileGroup);
					updateTileView(tileID);
				}
			}
			else if (currentView == VIEW_FPS)
			{
				tileID = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
				if (tileID >= 0 && tileID < numtiles)
				{
					(*tilesetRef)->getTile(tileID)->setFPS(currentFPS);
					updateTileView(tileID);
					emit changePreviewTile(tileID);
				}
			}
			else if (currentView == VIEW_PINGPONG)
			{
				tileID = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
				if (tileID >= 0 && tileID < numtiles)
				{
					Tile *tile = (*tilesetRef)->getTile(tileID);
					tile->setPingPong(!tile->isPingPong());
					updateTileView(tileID);
					emit changePreviewTile(tileID);
				}
			}
		}
		tileID = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
		mouseButtonPressed(event, tileID);
	}
}

void TileView::mouseReleaseEvent(QMouseEvent *event)
{
	if (!ignoreevents)
	{
		QPointF pos = mapToScene(event->pos());
		int tile;
		if (event->button() == Qt::LeftButton && currentView == VIEW_NORMAL)
		{
			QList<Map::TileData> selectionData;
			QRect rect;
			if (rectStart.x() > rectEnd.x())
			{
				rect.setLeft(rectEnd.x());
				rect.setRight(rectStart.x());
			}
			else
			{
				rect.setLeft(rectStart.x());
				rect.setRight(rectEnd.x());
			}
			if (rectStart.y() > rectEnd.y())
			{
				rect.setTop(rectEnd.y());
				rect.setBottom(rectStart.y());
			}
			else
			{
				rect.setTop(rectStart.y());
				rect.setBottom(rectEnd.y());
			}
			for (int y = 0; y < rect.height(); ++y)
			{
				for (int x = 0; x < rect.width(); ++x)
				{
					QPoint sceneCoordinates(x + rect.x(), y + rect.y());
					Map::TileData tileData;
					tileData.tilesetID = (unsigned char)tilesetRef->getDataID();
					tileData.tileID = (unsigned char)(sceneCoordinates.y() * 6 + sceneCoordinates.x());
					tileData.tileFlags = 0xff;
					for (int i = 0; i < NUM_PHASES; ++i)
					{
						tileData.phaseDirections[i] = (*tilesetRef)->getTile(tileData.tileID)->getPhaseDirections(i);
						tileData.phasePassability[i] = (*tilesetRef)->getTile(tileData.tileID)->getPhasePassability(i);
					}
					selectionData.append(tileData);
				}
			}
			tileSelectionChanged(rect.width(), rect.height(), selectionData);
		}
		tile = (int(pos.y()) - (int(pos.y()) % 32)) * 6 / 32 + (int(pos.x()) - (int(pos.x()) % 32)) / 32;
		mouseButtonReleased(event, tile);
	}
}

void TileView::leaveEvent(QEvent *event)
{
	Q_UNUSED(event);
	if (!ignoreevents && currentView != VIEW_NORMAL)
	{
		int numtiles = (*tilesetRef)->numTiles();
		int oldID = mouseCursorTileID;
		mouseCursorTileID = -1;
		if (oldID >= 0 && oldID < numtiles)
			updateTileView(oldID);
	}
}
