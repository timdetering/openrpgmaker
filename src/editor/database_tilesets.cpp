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

#include <QMessageBox>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "clipboard.h"
#include "database_tilesets.h"
#include "projectdata.h"
#include "projectdata_map.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "tilebrowser.h"
#include "xmlParser.h"

Database_Tilesets::Database_Tilesets(QWidget *parent) : QWidget(parent)
{
	QGraphicsScene *scene;
	setupUi(this);
	twTilesetList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	twTilesetList->horizontalHeader()->setHidden(true);
	twTilesetList->verticalHeader()->setHidden(true);
	twTerrainList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	twTerrainList->horizontalHeader()->setHidden(true);
	twTerrainList->verticalHeader()->setHidden(true);
	cboxPhase->setEnabled(false);
	gvTileEditor->changeView(TileView::VIEW_TERRAIN);
	tilesetRef = NULL;
	ignoreEvents = true;
	bTileGroupSideButton->setSpinBox(sbTileGroup);
	bFPSSideButton->setSpinBox(sbFPS);
	gvTilePreview->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	scene = new QGraphicsScene(0.0, 0.0, 32.0, 32.0);
	scene->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	gvTilePreview->setScene(scene);
	tilePreviewItem = new QGraphicsPixmapItem;
	tilePreviewItem->setPos(0.0, 0.0);
	tilePreviewItem->setVisible(false);
	scene->addItem(tilePreviewItem);
	tilePreviewTimer = new QTimer(this);
	tilePreviewFrame = 0;
	tilePreviewDir = 1;
	currentTileID = 0;
	QObject::connect(sbTileView, SIGNAL(rangeChanged(int, int)), gvTileEditor, SLOT(setScrollBarRange(int, int)));
	QObject::connect(sbTileView, SIGNAL(valueChanged(int)), gvTileEditor, SLOT(setScrollBarValue(int)));
	QObject::connect(tilePreviewTimer, SIGNAL(timeout()), this, SLOT(animateTilePreview()));
}

Database_Tilesets::~Database_Tilesets()
{
	if (tilesetRef != NULL)
	{
		delete tilesetRef;
		tilesetRef = NULL;
	}
}

void Database_Tilesets::setupTilesets()
{
	ignoreEvents = true;
	tilesetRef = new ProjectData::DataReference<Tileset>(0, __FILE__, __LINE__);
	leName->setText(ProjectData::tilesetNames[0]);
	gvTileEditor->changeTileset(0);
	gvTileEditor->setupView();
	updateTilesetList();
	updateTerrainList();
	lblFile->setText(QString("tilesets/tileset%1.xml").arg(twTilesetList->currentRow(), 2, 16, QChar('0')));
	lblNumTiles->setText(QString("%1/256").arg((*tilesetRef)->numTiles()));
	ignoreEvents = false;
	sbTileView->setRange(gvTileEditor->verticalScrollBar()->minimum(), gvTileEditor->verticalScrollBar()->maximum());
	sbTileView->setValue(gvTileEditor->verticalScrollBar()->value());
	bSetTerrain->setChecked(true);
	sbTileGroup->setEnabled(false);
	bTileGroupSideButton->setEnabled(false);
	sbFPS->setEnabled(false);
	bFPSSideButton->setEnabled(false);
	tilePreviewItem->setVisible(false);
	for (int i = 0; i < NUM_PHASES; ++i)
		cboxPhase->addItem(ProjectData::phaseNames[i]);
}

void Database_Tilesets::shutdownTilesets()
{
	if (tilesetRef != NULL)
	{
		delete tilesetRef;
		tilesetRef = NULL;
	}
	cboxPhase->clear();
	tilePreviewTimer->stop();
	tilePreviewItem->setVisible(false);
	gvTileEditor->shutdownView();
	twTilesetList->clearContents();
	twTilesetList->setRowCount(0);
	twTerrainList->clearContents();
	twTerrainList->setRowCount(0);
	if (swappedTiles.size() > 0)
	{
		int confirmation = QMessageBox::question(this, "Apply Tile Layout Adjustment", "You've adjusted the layout of some of the tiles.\nWould you like Open RPG Maker to automatically adjust all the maps to reflect the changes?\n\nWARNING: This could take a bit depending on the size of the project.", QMessageBox::Yes|QMessageBox::No);
		if (confirmation == QMessageBox::Yes)
		{
			for (ProjectData::mapNamesIter = ProjectData::mapNames.begin(); ProjectData::mapNamesIter != ProjectData::mapNames.end(); ++ProjectData::mapNamesIter)
			{
				ProjectData::DataReference<Map> *mapRef = new ProjectData::DataReference<Map>(ProjectData::mapNamesIter.key(), __FILE__, __LINE__);
				(*mapRef)->applySwappedTiles(swappedTiles);
				delete mapRef;
			}
		}
		swappedTiles.clear();
	}
}

void Database_Tilesets::on_twTilesetList_cellClicked(int row)
{
	if (tilesetRef != NULL)
	{
		delete tilesetRef;
		tilesetRef = NULL;
	}
	tilesetRef = new ProjectData::DataReference<Tileset>(row, __FILE__, __LINE__);
	leName->setText(ProjectData::tilesetNames[row]);
	gvTileEditor->changeTileset(row);
	lblFile->setText(QString("tilesets/tileset%1.xml").arg(twTilesetList->currentRow(), 2, 16, QChar('0')));
	lblNumTiles->setText(QString("%1/256").arg((*tilesetRef)->numTiles()));
	currentTileID = 0;
	tilePreviewTimer->stop();
	tilePreviewFrame = 0;
	tilePreviewDir = 1;
	if ((*tilesetRef)->numTiles() > 0)
	{
		tilePreviewTimer->start(1000 / ((*tilesetRef)->getTile(currentTileID)->getFPS()));
		tilePreviewItem->setPixmap(QPixmap::fromImage((*tilesetRef)->getTile(currentTileID)->getTile(0xFF, 0)));
	}
	else
	{
		bSetTerrain->setEnabled(false);
		bSetPhasePassability->setEnabled(false);
		bAdjustLayout->setEnabled(false);
		bTilingGroups->setEnabled(false);
		bSetFPS->setEnabled(false);
		bTogglePingPong->setEnabled(false);
		bSetUniformTerrain->setEnabled(false);
		bDeleteTiles->setEnabled(false);
	}
}

void Database_Tilesets::on_bArraySize_clicked()
{
	int numTilesets = ProjectData::tilesetNames.size();
	ArraySize *arraysize = new ArraySize(numTilesets, 1, 256);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numTilesets)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				gvTileEditor->changeTileset(0);
				if (tilesetRef != NULL)
				{
					if (newSize <= tilesetRef->getDataID())
					{
						delete tilesetRef;
						tilesetRef = NULL;
					}
				}
				for (int i = numTilesets - 1; i >= newSize; --i)
					ProjectData::deleteTileset(i);
			}
		}
		else
		{
			for (int i = numTilesets; i < newSize; ++i)
			{
				Tileset *tileset = new Tileset;
				tileset->setName(QString("Tileset%1").arg(i, 2, 16, QChar('0')));
				ProjectData::addTileset(tileset);
				delete tileset;
			}
		}
		ignoreEvents = true;
		updateTilesetList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_Tilesets::on_leName_textEdited(QString text)
{
	int currentRow = twTilesetList->currentRow();
	ignoreEvents = true;
	twTilesetList->item(currentRow, 0)->setText(QString("%1: %2").arg(currentRow, 3, 10, QChar('0')).arg(text));
	ProjectData::tilesetNames[currentRow] = text;
	(*tilesetRef)->setName(text);
	ignoreEvents = false;
}

void Database_Tilesets::on_bSetTerrain_toggled(bool on)
{
	if (on)
	{
		bSetPhasePassability->setChecked(false);
		bAdjustLayout->setChecked(false);
		bDeleteTiles->setChecked(false);
		bTilingGroups->setChecked(false);
		bSetFPS->setChecked(false);
		bTogglePingPong->setChecked(false);
		gvTileEditor->changeView(TileView::VIEW_TERRAIN);
	}
	else if (!bSetPhasePassability->isChecked() && !bAdjustLayout->isChecked() && !bDeleteTiles->isChecked() && !bTilingGroups->isChecked() && !bSetFPS->isChecked() && !bTogglePingPong->isChecked())
		bSetTerrain->setChecked(true);
}

void Database_Tilesets::on_bSetPhasePassability_toggled(bool on)
{
	if (on)
	{
		bSetTerrain->setChecked(false);
		bAdjustLayout->setChecked(false);
		bDeleteTiles->setChecked(false);
		bTilingGroups->setChecked(false);
		bSetFPS->setChecked(false);
		bTogglePingPong->setChecked(false);
		gvTileEditor->changeView(TileView::VIEW_PHASEPASSABILITY);
	}
	else if (!bSetTerrain->isChecked() && !bAdjustLayout->isChecked() && !bDeleteTiles->isChecked() && !bTilingGroups->isChecked() && !bSetFPS->isChecked() && !bTogglePingPong->isChecked())
		bSetPhasePassability->setChecked(true);
	cboxPhase->setEnabled(on);
}

void Database_Tilesets::on_cboxPhase_currentIndexChanged(int index)
{
	gvTileEditor->changePhase(index);
}

void Database_Tilesets::on_bAdjustLayout_toggled(bool on)
{
	if (on)
	{
		bSetTerrain->setChecked(false);
		bSetPhasePassability->setChecked(false);
		bDeleteTiles->setChecked(false);
		bTilingGroups->setChecked(false);
		bSetFPS->setChecked(false);
		bTogglePingPong->setChecked(false);
		gvTileEditor->changeView(TileView::VIEW_ADJUSTLAYOUT);
	}
	else if (!bSetTerrain->isChecked() && !bSetPhasePassability->isChecked() && !bDeleteTiles->isChecked() && !bTilingGroups->isChecked() && !bSetFPS->isChecked() && !bTogglePingPong->isChecked())
		bAdjustLayout->setChecked(true);
}

void Database_Tilesets::on_bTilingGroups_toggled(bool on)
{
	if (on)
	{
		bSetTerrain->setChecked(false);
		bSetPhasePassability->setChecked(false);
		bDeleteTiles->setChecked(false);
		bAdjustLayout->setChecked(false);
		bSetFPS->setChecked(false);
		bTogglePingPong->setChecked(false);
		sbTileGroup->setEnabled(true);
		bTileGroupSideButton->setEnabled(true);
		gvTileEditor->changeView(TileView::VIEW_TILINGGROUPS);
	}
	else if (!bSetTerrain->isChecked() && !bSetPhasePassability->isChecked() && !bDeleteTiles->isChecked() && !bAdjustLayout->isChecked() && !bSetFPS->isChecked() && !bTogglePingPong->isChecked())
		bTilingGroups->setChecked(true);
	else
	{
		sbTileGroup->setEnabled(false);
		bTileGroupSideButton->setEnabled(false);
	}
}

void Database_Tilesets::on_sbTileGroup_valueChanged(int value)
{
	gvTileEditor->changeTileGroup(value);
}

void Database_Tilesets::on_bSetFPS_toggled(bool on)
{
	if (on)
	{
		tilePreviewTimer->stop();
		bSetTerrain->setChecked(false);
		bSetPhasePassability->setChecked(false);
		bDeleteTiles->setChecked(false);
		bAdjustLayout->setChecked(false);
		bTilingGroups->setChecked(false);
		bTogglePingPong->setChecked(false);
		sbFPS->setEnabled(true);
		bFPSSideButton->setEnabled(true);
		gvTileEditor->changeView(TileView::VIEW_FPS);
		tilePreviewItem->setVisible(true);
		tilePreviewFrame = 0;
		tilePreviewDir = 0;
		tilePreviewTimer->start(1000 / ((*tilesetRef)->getTile(currentTileID)->getFPS()));
		tilePreviewItem->setPixmap(QPixmap::fromImage((*tilesetRef)->getTile(currentTileID)->getTile(0xFF, 0)));
	}
	else if (!bSetTerrain->isChecked() && !bSetPhasePassability->isChecked() && !bAdjustLayout->isChecked() && !bDeleteTiles->isChecked() && !bTilingGroups->isChecked() && !bTogglePingPong->isChecked())
		bSetFPS->setChecked(true);
	else
	{
		if (!bTogglePingPong->isChecked())
			tilePreviewItem->setVisible(false);
		sbFPS->setEnabled(false);
		bFPSSideButton->setEnabled(false);
	}
}

void Database_Tilesets::on_sbFPS_valueChanged(int value)
{
	gvTileEditor->changeFPS(value);
}

void Database_Tilesets::on_bTogglePingPong_toggled(bool on)
{
	if (on)
	{
		bSetTerrain->setChecked(false);
		bSetPhasePassability->setChecked(false);
		bDeleteTiles->setChecked(false);
		bAdjustLayout->setChecked(false);
		bTilingGroups->setChecked(false);
		bSetFPS->setChecked(false);
		gvTileEditor->changeView(TileView::VIEW_PINGPONG);
		tilePreviewItem->setVisible(true);
	}
	else if (!bSetTerrain->isChecked() && !bSetPhasePassability->isChecked() && !bAdjustLayout->isChecked() && !bDeleteTiles->isChecked() && !bTilingGroups->isChecked() && !bSetFPS->isChecked())
		bTogglePingPong->setChecked(true);
	else if (!bSetFPS->isChecked())
		tilePreviewItem->setVisible(false);
}

void Database_Tilesets::on_bSetUniformTerrain_clicked()
{
	int i, numtiles = (*tilesetRef)->numTiles();
	for (i = 0; i < numtiles; ++i)
		(*tilesetRef)->getTile(i)->setTerrainID(twTerrainList->currentRow());
	gvTileEditor->updateTileView();
}

void Database_Tilesets::on_bAddTiles_clicked()
{
	TileBrowser *tilebrowser = new TileBrowser(256 - (*tilesetRef)->numTiles());
	if (tilebrowser->exec())
	{
		QStringList tileLocations = tilebrowser->getTileLocations();
		int i, numTileLocations = tileLocations.size();
		for (i = 0; i < numTileLocations; ++i)
			(*tilesetRef)->addTile(new Tile(tileLocations[i]));
		lblNumTiles->setText(QString("%1/256").arg((*tilesetRef)->numTiles()));
		gvTileEditor->updateTileView();
		if ((*tilesetRef)->numTiles() > 0)
		{
			bSetTerrain->setEnabled(true);
			bSetPhasePassability->setEnabled(true);
			bAdjustLayout->setEnabled(true);
			bTilingGroups->setEnabled(true);
			bSetFPS->setEnabled(true);
			bTogglePingPong->setEnabled(true);
			bSetUniformTerrain->setEnabled(true);
			bDeleteTiles->setEnabled(true);
		}
	}
	delete tilebrowser;
}

void Database_Tilesets::on_bDeleteTiles_toggled(bool on)
{
	if (on)
	{
		int confirmdelete = QMessageBox::warning(0, "Delete Tiles?", "Clicking on tiles will now result in their deletion.\nIs this OK?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
		if (confirmdelete == QMessageBox::Yes)
		{
			bSetTerrain->setChecked(false);
			bSetPhasePassability->setChecked(false);
			bAdjustLayout->setChecked(false);
			bTilingGroups->setChecked(false);
			gvTileEditor->changeView(TileView::VIEW_CUSTOM);
		}
		else
			bDeleteTiles->setChecked(false);
	}
	else if (!bSetTerrain->isChecked() && !bSetPhasePassability->isChecked() && !bAdjustLayout->isChecked() && !bTilingGroups->isChecked())
		bDeleteTiles->setChecked(true);
}

void Database_Tilesets::on_gvTileEditor_mouseButtonPressed(QMouseEvent *event, int tileID)
{
	if (event->button() == Qt::LeftButton && bDeleteTiles->isChecked())
	{
		int numtiles = (*tilesetRef)->numTiles();
		if (numtiles > 1 && tileID >= 0 && tileID < numtiles)
		{
			(*tilesetRef)->deleteTile(tileID);
			for (int i = tileID; i < numtiles; ++i)
				gvTileEditor->updateTileView(i);
		}
	}
	currentTileID = tileID;
}

void Database_Tilesets::on_gvTileEditor_scrollBarRangeChanged(int min, int max)
{
	if (min != sbTileView->minimum() || max != sbTileView->maximum())
		sbTileView->setRange(min, max);
}

void Database_Tilesets::on_gvTileEditor_scrollBarValueChanged(int value)
{
	if (value != sbTileView->value())
		sbTileView->setValue(value);
}

void Database_Tilesets::on_gvTileEditor_tilesSwapped(int tile1, int tile2)
{
	Tileset::SwapTileIDs swappedTile;
	swappedTile.tileset = tilesetRef->getDataID();
	swappedTile.tile1 = tile1;
	swappedTile.tile2 = tile2;
	swappedTiles.append(swappedTile);
}

void Database_Tilesets::on_gvTileEditor_changePreviewTile(int tileID)
{
	currentTileID = tileID;
	tilePreviewTimer->stop();
	tilePreviewFrame = 0;
	tilePreviewDir = 1;
	tilePreviewTimer->start(1000 / ((*tilesetRef)->getTile(currentTileID)->getFPS()));
	tilePreviewItem->setPixmap(QPixmap::fromImage((*tilesetRef)->getTile(currentTileID)->getTile(0xFF, 0)));
}

void Database_Tilesets::on_twTerrainList_cellClicked(int row)
{
	gvTileEditor->changeTerrain(row);
}

void Database_Tilesets::animateTilePreview()
{
	Tile *tile = (*tilesetRef)->getTile(currentTileID);
	int numFrames = tile->getNumFrames();
	if (tile->isPingPong())
	{
		tilePreviewFrame += tilePreviewDir;
		if (tilePreviewFrame == 0 || tilePreviewFrame == numFrames - 1)
			tilePreviewDir *= -1;
	}
	else
		tilePreviewFrame = (tilePreviewFrame + 1) % numFrames;
	tilePreviewItem->setPixmap(QPixmap::fromImage(tile->getTile(0xFF, tilePreviewFrame)));
}

void Database_Tilesets::updateTilesetList()
{
	QTableWidgetItem *item;
	int i, numtilesets = ProjectData::tilesetNames.size();
	int currentRow = twTilesetList->currentRow();
	twTilesetList->clearContents();
	twTilesetList->setRowCount(numtilesets);
	for (i = 0; i < numtilesets; ++i)
	{
		item = new QTableWidgetItem(QString("%1: %2").arg(i, 3, 10, QChar('0')).arg(ProjectData::tilesetNames[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
		twTilesetList->setItem(i, 0, item);
		twTilesetList->setRowHeight(i, 15);
	}
	if (currentRow >= numtilesets)
		currentRow = numtilesets - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (numtilesets > 0)
	{
		twTilesetList->setCurrentCell(currentRow, 0);
		twTilesetList->item(currentRow, 0)->setSelected(true);
		gvTileEditor->changeTileset(currentRow, true);
	}
}

void Database_Tilesets::updateTerrainList()
{
	QTableWidgetItem *item;
	int i, numterrain = ProjectData::terrainNames.size();
	int currentRow = twTerrainList->currentRow();
	twTerrainList->setRowCount(numterrain);
	for (i = 0; i < numterrain; ++i)
	{
		item = twTerrainList->item(i, 0);
		if (item)
		{
			item->setText(QString("%1: %2").arg(i, 3, 10, QChar('0')).arg(ProjectData::terrainNames[i]));
			item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
		}
		else
		{
			item = new QTableWidgetItem(QString("%1: %2").arg(i, 3, 10, QChar('0')).arg(ProjectData::terrainNames[i]));
			item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
			twTerrainList->setItem(i, 0, item);
		}
		twTerrainList->setRowHeight(i, 15);
	}
	if (currentRow >= twTerrainList->rowCount())
		currentRow = twTerrainList->rowCount() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::terrainNames.size() > 0)
	{
		twTerrainList->setCurrentCell(currentRow, 0);
		twTerrainList->item(currentRow, 0)->setSelected(true);
	}
}
