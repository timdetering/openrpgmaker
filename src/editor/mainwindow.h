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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "ui_mainwindow.h"
#include "clipboard.h"
#include "projectdata.h"

class Database;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
		friend class ProjectData;
		MainWindow();
		void setupWindow(int argc, char *argv[]);
		void setCurrentTileset(int tilesetID);
		void setCurrentTile(int tileID);
	private slots:
		// Toolbar Buttons
		void on_aNewProject_triggered();
		void on_aOpenProject_triggered();
		void on_aCloseProject_triggered();
		// SEPERATOR
		void on_aChangeProjectName_triggered();
		void on_aCreateGameDisk_triggered();
		// SEPERATOR
		void on_aSaveChanges_triggered();
		void on_aRevertChanges_triggered();
		// SEPERATOR
		void on_aDatabase_triggered();
		void on_aResourceManager_triggered();
		void on_aImport_triggered();
		void on_aListenToMusic_triggered();
		void on_aFindEvent_triggered();
		// SEPERATOR
		void on_aTestPlay_triggered();
		// SEPERATOR
		void on_aGameOptions_FS_toggled(bool on);
		void on_aGameOptions_DisplayTitle_toggled(bool on);
		// SEPERATOR
		void on_aHelp_ShowIndex_triggered();
		void on_aAbout_ORPGM_triggered();
		// MapList Buttons (only visible from context menu)
		void on_aNewEmptyMap_triggered();
		void on_aNewRandomMap_triggered();
		void on_aDeleteMap_triggered();
		void on_aNewArea_triggered();
		void on_aDeleteArea_triggered();
		void on_aMapProperties_triggered();
		void on_aEditOnloadEvent_triggered();
		// Layers slots
		void on_bMoveLayerUp_clicked();
		void on_bMoveLayerDown_clicked();
		void on_bDeleteLayer_clicked();
		void on_bAddLayer_clicked();
		void on_twLayers_itemDoubleClicked(QTableWidgetItem *item);
		void on_twLayers_itemChanged(QTableWidgetItem *item);
		void on_twLayers_itemSelectionChanged();
		// Passability slots
		void on_bConfigurePhases_clicked();
		void on_cboxPhases_currentIndexChanged(int index);
		void on_rbSetPassability_toggled(bool on);
		void on_rbClearPassability_toggled(bool on);
		void on_rbDirectionPassability_toggled(bool on);
		// MapView Buttons
		void on_aGeneratePreview_triggered();
		void on_aToggleGrid_toggled(bool on);
		void on_toolMakeSelection_toggled(bool on);
		void on_toolDrawRectangle_toggled(bool on);
		void on_toolDrawCircle_toggled(bool on);
		void on_toolBucketFill_toggled(bool on);
		void on_toolPencil_toggled(bool on);
		void on_toolTiling_toggled(bool on);
		void on_aEditLayers_toggled(bool on);
		void on_aEditPassability_toggled(bool on);
		void on_aEdit_EventLayer_toggled(bool on);
		void on_aZoom_1to1_toggled(bool on);
		void on_aZoom_1to2_toggled(bool on);
		void on_aZoom_1to4_toggled(bool on);
		void on_aZoom_1to8_toggled(bool on);
		// Other Miscellaneous slots
		void on_gvTileSelection_scrollBarRangeChanged(int min, int max);
		void on_gvTileSelection_scrollBarValueChanged(int value);
		void on_twMapList_itemExpanded(QTreeWidgetItem *item);
		void on_twMapList_itemCollapsed(QTreeWidgetItem *item);
		void on_twMapList_itemDoubleClicked(QTreeWidgetItem *item);
		void on_twMapList_itemSelectionChanged();
		void on_twMapList_customContextMenuRequested(const QPoint &pos);
		void on_editSelectTileset_currentIndexChanged(int index);
	private:
		void closeEvent(QCloseEvent *event);
		void updateTilesetList();
		void updateLayerList();
		void enableDisableMainWindow(bool enable);
		ProjectData::DataReference<Map> *mapRef;
		Database *database;
		QTreeWidgetItem *contextMenuItem;
		QPoint rectStart;
		QPoint rectEnd;
		int currentTileset;
		int contextMenuMapID;
		int contextMenuAreaID;
		bool enableAudio;
		bool ignoreEvents;
};

#endif // MAINWINDOW_H
