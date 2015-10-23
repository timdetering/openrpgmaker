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

#ifndef DATABASE_TILESETS_H
#define DATABASE_TILESETS_H

#include <QList>
#include "ui_database_tilesets.h"
#include "projectdata.h"
#include "projectdata_tileset.h"

class Database_Tilesets : public QWidget, public Ui::Database_Tilesets
{
	Q_OBJECT
	public:
		Database_Tilesets(QWidget *parent=0);
		~Database_Tilesets();
		void setupTilesets();
		void shutdownTilesets();
	protected slots:
		void on_twTilesetList_cellClicked(int row);
		void on_bArraySize_clicked();
		void on_leName_textEdited(QString text);
		void on_bSetTerrain_toggled(bool on);
		void on_bSetPhasePassability_toggled(bool on);
		void on_cboxPhase_currentIndexChanged(int index);
		void on_bAdjustLayout_toggled(bool on);
		void on_bTilingGroups_toggled(bool on);
		void on_sbTileGroup_valueChanged(int value);
		void on_bSetFPS_toggled(bool on);
		void on_sbFPS_valueChanged(int value);
		void on_bTogglePingPong_toggled(bool on);
		void on_bSetUniformTerrain_clicked();
		void on_bAddTiles_clicked();
		void on_bDeleteTiles_toggled(bool on);
		void on_gvTileEditor_mouseButtonPressed(QMouseEvent *event, int tileID);
		void on_gvTileEditor_scrollBarRangeChanged(int min, int max);
		void on_gvTileEditor_scrollBarValueChanged(int value);
		void on_gvTileEditor_tilesSwapped(int tile1, int tile2);
		void on_gvTileEditor_changePreviewTile(int tileID);
		void on_twTerrainList_cellClicked(int row);
		void animateTilePreview();
	private:
		void updateTilesetList();
		void updateTerrainList();
		QGraphicsPixmapItem *tilePreviewItem;
		QTimer *tilePreviewTimer;
		ProjectData::DataReference<Tileset> *tilesetRef;
		QList<Tileset::SwapTileIDs> swappedTiles;
		int currentTileID;
		int tilePreviewFrame;
		int tilePreviewDir;
		bool ignoreEvents;
};

#endif // DATABASE_TILESETS_H
