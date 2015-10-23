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

#include <QGraphicsRectItem>
#include <QMouseEvent>
#include "projectdata.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "tilechooser.h"

TileChooser::TileChooser(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	ignoreEvents = true;
	QPen pen(Qt::DashLine);
	pen.setColor(QColor(Qt::cyan));
	pen.setWidth(3);
	tileSelection = new QGraphicsRectItem(1.0, 1.0, 30.0, 30.0);
	tileSelection->setZValue(1.0);
	tileSelection->setPen(pen);
	tileSelection->setBrush(QBrush(Qt::transparent));
	tileSelection->setVisible(true);
	gvTileView->scene()->addItem(tileSelection);
	currentTilesetID = 0;
	currentTileID = 0;
	updateTilesetList();
	numTilesets = tilesets.size();
	numTiles = (*tilesets[0])->numTiles();
}

TileChooser::~TileChooser()
{
	for (int i = 0; i < tilesets.size(); ++i)
		delete (tilesets[i]);
	tilesets.clear();
}

void TileChooser::setupTileChooser(unsigned short currentTile, int selectWidth, int selectHeight)
{
	currentTilesetID = int((currentTile >> 8) % 256);
	currentTileID = int(currentTile % 256);
	numTilesets = tilesets.size();
	if (currentTilesetID >= numTilesets)
	{
		currentTilesetID = 0;
		currentTileID = 0;
	}
	numTiles = (*tilesets[currentTilesetID])->numTiles();
	if (currentTileID >= numTiles)
		currentTileID = 0;
	selectionWidth = selectWidth;
	selectionHeight = selectHeight;
	QObject::connect(sbTileView, SIGNAL(rangeChanged(int, int)), gvTileView, SLOT(setScrollBarRange(int, int)));
	QObject::connect(sbTileView, SIGNAL(valueChanged(int)), gvTileView, SLOT(setScrollBarValue(int)));
	cboxTilesets->setCurrentIndex(currentTilesetID);
	tileSelection->setRect(((currentTileID % 6) * 32.0) + 1.0, ((currentTileID / 6) * 32.0) + 1.0, (selectionWidth * 32.0) - 2.0, (selectionHeight * 32.0) - 2.0);
	gvTileView->changeTileset(currentTilesetID);
	gvTileView->setupView();
	gvTileView->changeView(TileView::VIEW_CUSTOM);
	gvTileView->scrollToTile(currentTileID);
	ignoreEvents = false;
}

unsigned short TileChooser::getSelectedTile(int xOffset, int yOffset)
{
	if (currentTilesetID >= 0 && currentTilesetID < tilesets.size())
	{
		if (currentTileID >= 0 && currentTileID < (*tilesets[currentTilesetID])->numTiles())
		{
			int tileXCoord = (currentTileID % 6) + xOffset;
			int tileYCoord = (currentTileID / 6) + yOffset;
			int tileID = (tileYCoord * 6) + tileXCoord;
			unsigned short tile = ((unsigned short)currentTilesetID << 8) + (unsigned short)tileID;
			return tile;
		}
	}
	return 0u;
}

void TileChooser::on_cboxTilesets_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		currentTilesetID = index;
		numTiles = (*tilesets[currentTilesetID])->numTiles();
		if (currentTileID >= numTiles)
		{
			currentTileID = numTiles - 1;
			tileSelection->setRect(((currentTileID % 6) * 32.0) + 1.0, ((currentTileID / 6) * 32.0) + 1.0, (selectionWidth * 32.0) - 2.0, (selectionHeight * 32.0) - 2.0);
		}
		gvTileView->changeTileset(currentTilesetID);
	}
}

void TileChooser::on_gvTileView_mouseButtonPressed(QMouseEvent *event, int tileID)
{
	if (event->button() == Qt::LeftButton && tileID >= 0 && tileID < numTiles && !ignoreEvents)
	{
		int tileXCoord = tileID % 6;
		int tileYCoord = tileID / 6;
		bool changeTile = true;
		for (int y = 0; y < selectionHeight && changeTile; ++y)
		{
			for (int x = 0; x < selectionWidth && changeTile; ++x)
			{
				int xCoord = tileXCoord + y;
				int yCoord = tileYCoord + y;
				int newTileID = (yCoord * 6) + xCoord;
				if (newTileID < 0 || newTileID >= numTiles || xCoord < 0 || xCoord >= 6 || yCoord < 0)
					changeTile = false;
			}
		}
		if (changeTile)
		{
			currentTileID = tileID;
			tileSelection->setRect(((currentTileID % 6) * 32.0) + 1.0, ((currentTileID / 6) * 32.0) + 1.0, (selectionWidth * 32.0) - 2.0, (selectionHeight * 32.0) - 2.0);
		}
		gvTileView->updateTileView();
	}
}

void TileChooser::on_gvTileView_scrollBarRangeChanged(int min, int max)
{
	if (min != sbTileView->minimum() || max != sbTileView->maximum())
		sbTileView->setRange(min, max);
}

void TileChooser::on_gvTileView_scrollBarValueChanged(int value)
{
	if (value != sbTileView->value())
		sbTileView->setValue(value);
}

void TileChooser::updateTilesetList()
{
	cboxTilesets->addItems(ProjectData::tilesetNames);
	for (int i = 0; i < ProjectData::tilesetNames.size(); ++i)
		tilesets.append(new ProjectData::DataReference<Tileset>(i, __FILE__, __LINE__));
}
