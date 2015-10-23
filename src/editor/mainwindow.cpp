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
#include <QCloseEvent>
#include <QDir>
#include <QDesktopWidget>
#include <QCoreApplication>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "about.h"
#include "clipboard.h"
#include "configuremaponloadevent.h"
#include "configurephases.h"
#include "createproject.h"
#include "database.h"
#include "importer.h"
#include "loading.h"
#include "helpmanual.h"
#include "mainwindow.h"
#include "mappreview.h"
#include "mapproperties.h"
#include "mapview.h"
#include "musicbrowser.h"
#include "openproject.h"
#include "projectdata.h"
#include "projectdata_map.h"
#include "randommapgenerator.h"
#include "resourcemanager.h"
#include "xmlParser.h"

MainWindow::MainWindow() : QMainWindow()
{
	QHeaderView *header;
	setupUi(this);
	twMapList->header()->setResizeMode(QHeaderView::Stretch);
	twMapList->header()->setHidden(true);
	header = twLayers->horizontalHeader();
	header->setResizeMode(0, QHeaderView::Fixed);
	header->setResizeMode(1, QHeaderView::Stretch);
	header->resizeSections(QHeaderView::ResizeToContents);
	contextMenuItem = NULL;
	currentTileset = 0;
	contextMenuMapID = -1;
	contextMenuAreaID = -1;
	rectStart = QPoint(0, 0);
	rectEnd = QPoint(0, 0);
	for (int i = 0; i < NUM_PHASES; ++i)
		cboxPhases->addItem("");
	ignoreEvents = false;
	mapRef = NULL;
	QObject::connect(sbTileView, SIGNAL(rangeChanged(int, int)), gvTileSelection, SLOT(setScrollBarRange(int, int)));
	QObject::connect(sbTileView, SIGNAL(valueChanged(int)), gvTileSelection, SLOT(setScrollBarValue(int)));
	QObject::connect(gvTileSelection, SIGNAL(tileSelectionChanged(int, int, QList<Map::TileData>)), gvMapEditor, SLOT(changeTileSelection(int, int, QList<Map::TileData>)));
	QObject::connect(gvTileSelection, SIGNAL(tilesetChanged(int)), editSelectTileset, SLOT(setCurrentIndex(int)));
	QObject::connect(gvMapEditor, SIGNAL(tileSelectionChanged(int, int, QList<Map::TileData>)), gvTileSelection, SLOT(changeTileSelection(int, int, QList<Map::TileData>)));
	QObject::connect(tbUndo, SIGNAL(clicked()), gvMapEditor, SLOT(undo()));
	QObject::connect(tbRedo, SIGNAL(clicked()), gvMapEditor, SLOT(redo()));
	QObject::connect(gvMapEditor, SIGNAL(undoEnabled(bool)), tbUndo, SLOT(setEnabled(bool)));
	QObject::connect(gvMapEditor, SIGNAL(redoEnabled(bool)), tbRedo, SLOT(setEnabled(bool)));
	ProjectData::helpManual = new HelpManual(ProjectData::datadir + "/helpManual");
	aSaveChanges->setShortcut(QKeySequence(QKeySequence::Save));
}

void MainWindow::setupWindow(int argc, char *argv[])
{
	QFileInfo info(ProjectData::homePath + "/storage");
	XMLNode ConfigNode, TempNode;
	QList<int> sizes;
	QStringList stringList;
	QPoint windowLocation;
	QSize windowSize;
	QString projectLocation;
	bool maximized = false;
	enableAudio = true;
	if (info.exists())
	{
		int confirmation = QMessageBox::question(0, "Recover Project", "Open RPG Maker has crashed unexpectedly.\nWould you like to try and recover any unsaved changes?", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
		if (confirmation == QMessageBox::Yes)
		{
			Loading *loading = new Loading("Loading Project");
			loading->show();
			QCoreApplication::processEvents();
			ProjectData::recoverProject();
			if (ProjectData::open)
			{
				database = new Database;
				enableDisableMainWindow(true);
				currentTileset = 0;
				rectStart = QPoint(0, 0);
				rectEnd = QPoint(0, 0);
				updateTilesetList();
				for (int i = 0; i < NUM_PHASES; ++i)
					cboxPhases->setItemText(i, ProjectData::phaseNames[i]);
				gvMapEditor->setupView();
				gvTileSelection->changeTileset(ProjectData::getSelectedTilesetID(), true);
				gvTileSelection->setupView();
				sbTileView->setRange(gvTileSelection->verticalScrollBar()->minimum(), gvTileSelection->verticalScrollBar()->maximum());
				sbTileView->setValue(gvTileSelection->verticalScrollBar()->value());
			}
			loading->doneLoading();
			delete loading;
		}
		else
		{
			ProjectData::cleanStorageData();
			ProjectData::saved = true;
		}
	}
	else
	{
		info.setFile(ProjectData::homePath + "/config.xml");
		if (info.exists())
		{
			ConfigNode = XMLNode::openFileHelper(QString(QDir::homePath() + "/.OpenRPGMaker/config.xml").toLocal8Bit().data(), "openrpgmaker-config");
			windowLocation.setX(QString(ConfigNode.getAttribute("windowX")).toInt());
			windowLocation.setY(QString(ConfigNode.getAttribute("windowY")).toInt());
			windowSize.setWidth(QString(ConfigNode.getAttribute("windowWidth")).toInt());
			windowSize.setHeight(QString(ConfigNode.getAttribute("windowHeight")).toInt());
			move(windowLocation);
			resize(windowSize);
			stringList = QString(ConfigNode.getAttribute("mapSplitterSizes")).split(';');
			sizes.clear();
			for (int i = 0; i < stringList.size(); ++i)
				sizes.append(stringList[i].toInt());
			mapSplitter->setSizes(sizes);
			stringList = QString(ConfigNode.getAttribute("layersSplitterSizes")).split(';');
			sizes.clear();
			for (int i = 0; i < stringList.size(); ++i)
				sizes.append(stringList[i].toInt());
			layersSplitter->setSizes(sizes);
			ProjectData::projectsdir = ConfigNode.getAttribute("projectsdir");
			projectLocation = ConfigNode.getAttribute("projectLocation");
		}
		for (int i = 1; i < argc; ++i)
		{
			if (QString(argv[i]) == "--disableaudio")
				enableAudio = false;
			else if (QString(argv[i]) == "--enableaudio")
				enableAudio = true;
			else if (QString(argv[i]) == "--maximized")
				maximized = true;
		}
		if (!projectLocation.isEmpty())
		{
			Loading *loading = new Loading("Loading Project");
			loading->show();
			ProjectData::openProject(projectLocation);
			if (ProjectData::open)
			{
				database = new Database;
				enableDisableMainWindow(true);
				currentTileset = 0;
				rectStart = QPoint(0, 0);
				rectEnd = QPoint(0, 0);
				updateTilesetList();
				for (int i = 0; i < NUM_PHASES; ++i)
					cboxPhases->setItemText(i, ProjectData::phaseNames[i]);
				gvMapEditor->setupView();
				gvTileSelection->changeTileset(ProjectData::getSelectedTilesetID(), true);
				gvTileSelection->setupView();
				sbTileView->setRange(gvTileSelection->verticalScrollBar()->minimum(), gvTileSelection->verticalScrollBar()->maximum());
				sbTileView->setValue(gvTileSelection->verticalScrollBar()->value());
			}
			else
				ProjectData::cleanStorageData();
			loading->doneLoading();
			delete loading;
		}
	}
	if (enableAudio)
	{
		ProjectData::audioManager = new AudioManager;
		AudioManager::initializeAudioManager();
	}
	if (maximized)
		setGeometry(QApplication::desktop()->availableGeometry());
}

void MainWindow::setCurrentTileset(int tilesetID)
{
	editSelectTileset->setCurrentIndex(tilesetID);
}

void MainWindow::setCurrentTile(int tileID)
{
	gvTileSelection->changeSelection(tileID);
}

void MainWindow::on_aNewProject_triggered()
{
	CreateProject *createproject = new CreateProject;
	bool ignore = true;
	if (createproject->exec())
	{
		Loading *loading = new Loading("Creating Project");
		loading->show();
		if (ProjectData::open)
		{
			if (!ProjectData::saved)
			{
				int confirmsave = QMessageBox::warning(0, "Save Changes?", "If you close now all unsaved data will be lost.\nDo you wish to save project data?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
				if (confirmsave == QMessageBox::Save)
				{
					ProjectData::saveProject();
					ignore = false;
				}
				else if (confirmsave == QMessageBox::Cancel)
					ignore = true;
				else
					ignore = false;
			}
			if (!ignore)
			{
				if (database != NULL)
				{
					delete database;
					database = NULL;
				}
				if (mapRef != NULL)
				{
					delete mapRef;
					mapRef = NULL;
				}
				gvMapEditor->shutdownView();
				gvTileSelection->shutdownView();
				currentTileset = 0;
				rectStart = QPoint(0, 0);
				rectEnd = QPoint(0, 0);
				editSelectTileset->clear();
				twMapList->clear();
				ProjectData::closeProject();
			}
		}
		if (ignore)
		{
			ProjectData::createProject(createproject->getGameName(), createproject->getDirName());
			if (ProjectData::open)
			{
				database = new Database;
				enableDisableMainWindow(true);
				currentTileset = 0;
				rectStart = QPoint(0, 0);
				rectEnd = QPoint(0, 0);
				updateTilesetList();
				for (int i = 0; i < NUM_PHASES; ++i)
					cboxPhases->setItemText(i, ProjectData::phaseNames[i]);
				gvMapEditor->setupView();
				gvMapEditor->updateMapView();
				gvTileSelection->setupView();
				sbTileView->setRange(gvTileSelection->verticalScrollBar()->minimum(), gvTileSelection->verticalScrollBar()->maximum());
				sbTileView->setValue(gvTileSelection->verticalScrollBar()->value());
			}
		}
		loading->doneLoading();
		delete loading;
	}
	delete createproject;
}

void MainWindow::on_aOpenProject_triggered()
{
	OpenProject *openproject = new OpenProject;
	bool ignore = true;
	if (openproject->exec())
	{
		Loading *loading = new Loading("Loading Project");
		loading->show();
		if (ProjectData::open)
		{
			if (!ProjectData::saved)
			{
				int confirmsave = QMessageBox::warning(0, "Save Changes?", "If you close now all unsaved data will be lost.\nDo you wish to save project data?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
				if (confirmsave == QMessageBox::Save)
				{
					ProjectData::saveProject();
					ignore = false;
				}
				else if (confirmsave == QMessageBox::Cancel)
					ignore = true;
				else
					ignore = false;
			}
			if (!ignore)
			{
				if (database != NULL)
				{
					delete database;
					database = NULL;
				}
				if (mapRef != NULL)
				{
					delete mapRef;
					mapRef = NULL;
				}
				gvMapEditor->shutdownView();
				gvTileSelection->shutdownView();
				currentTileset = 0;
				rectStart = QPoint(0, 0);
				rectEnd = QPoint(0, 0);
				editSelectTileset->clear();
				twMapList->clear();
				ProjectData::closeProject();
			}
		}
		if (ignore)
		{
			ProjectData::openProject(openproject->getProjectDir());
			if (ProjectData::open)
			{
				database = new Database;
				enableDisableMainWindow(true);
				currentTileset = 0;
				rectStart = QPoint(0, 0);
				rectEnd = QPoint(0, 0);
				updateTilesetList();
				for (int i = 0; i < NUM_PHASES; ++i)
					cboxPhases->setItemText(i, ProjectData::phaseNames[i]);
				gvMapEditor->setupView();
				gvMapEditor->updateMapView();
				gvTileSelection->changeTileset(ProjectData::getSelectedTilesetID(), true);
				gvTileSelection->setupView();
				sbTileView->setRange(gvTileSelection->verticalScrollBar()->minimum(), gvTileSelection->verticalScrollBar()->maximum());
				sbTileView->setValue(gvTileSelection->verticalScrollBar()->value());
			}
			else
				ProjectData::cleanStorageData();
		}
		loading->doneLoading();
		delete loading;
	}
}

void MainWindow::on_aCloseProject_triggered()
{
	bool ignore = false;
	if (!ProjectData::saved)
	{
		int confirmsave = QMessageBox::warning(0, "Save Changes?", "If you close now all unsaved data will be lost.\nDo you wish to save project data?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
		if (confirmsave == QMessageBox::Save)
		{
			ProjectData::saveProject();
		}
		else if (confirmsave == QMessageBox::Cancel)
			ignore = true;
	}
	if (!ignore)
	{
		ignoreEvents = true;
		if (database != NULL)
		{
			delete database;
			database = NULL;
		}
		if (mapRef != NULL)
		{
			delete mapRef;
			mapRef = NULL;
		}
		gvMapEditor->shutdownView();
		gvTileSelection->shutdownView();
		currentTileset = 0;
		rectStart = QPoint(0, 0);
		rectEnd = QPoint(0, 0);
		editSelectTileset->clear();
		twMapList->clear();
		enableDisableMainWindow(false);
		twLayers->clearContents();
		for (int i = 0; i < NUM_PHASES; ++i)
			cboxPhases->setItemText(i, "");
		ProjectData::closeProject();
		ignoreEvents = false;
	}
}

void MainWindow::on_aChangeProjectName_triggered()
{
	if (ProjectData::open)
	{
		CreateProject *createproject = new CreateProject(true);
		createproject->leGameName->setText(ProjectData::gameName);
		createproject->leDirName->setText(ProjectData::directoryName);
		if (createproject->exec())
			ProjectData::changeName(createproject->getGameName());
		delete createproject;
	}
}

void MainWindow::on_aCreateGameDisk_triggered()
{
	TODO("Implement function");
}

void MainWindow::on_aSaveChanges_triggered()
{
	ProjectData::saveProject();
}

void MainWindow::on_aRevertChanges_triggered()
{
	TODO("Implement function");
}

void MainWindow::on_aDatabase_triggered()
{
	database->setupDatabase();
	database->exec();
	database->shutdownDatabase();
	updateTilesetList();
	gvTileSelection->changeTileset(editSelectTileset->currentIndex(), true);
	gvTileSelection->updateTileView();
	gvMapEditor->updateTilesetList();
	gvMapEditor->updateMapView();
}

void MainWindow::on_aResourceManager_triggered()
{
	ResourceManager *manager = new ResourceManager;
	manager->exec();
	delete manager;
	gvMapEditor->updateTilesetList();
	updateTilesetList();
}

void MainWindow::on_aImport_triggered()
{
	Importer *importer = new Importer;
	importer->exec();
	delete importer;
	gvMapEditor->updateTilesetList();
	updateTilesetList();
}

void MainWindow::on_aListenToMusic_triggered()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	musicBrowser->setupBrowser();
	musicBrowser->exec();
	delete musicBrowser;
}

void MainWindow::on_aFindEvent_triggered()
{
	TODO("Implement function");
}

void MainWindow::on_aTestPlay_triggered()
{
	TODO("Implement Function");
	QMessageBox::information(this, "Feature Not Implemented", "The engine is not yet fully implemented, but is coming soon. Until then please continue to review Open RPG Maker and submit any feature requests or bug reports to the Open RPG Maker forum.");
}

void MainWindow::on_aGameOptions_FS_toggled(bool on)
{
	ProjectData::fullscreen = on;
	ProjectData::saved = false;
}

void MainWindow::on_aGameOptions_DisplayTitle_toggled(bool on)
{
	ProjectData::showtitle = on;
	ProjectData::saved = false;
}

void MainWindow::on_aHelp_ShowIndex_triggered()
{
	if (!ProjectData::helpManual->isVisible())
	{
		ProjectData::helpManual->loadPage("/index.html");
		ProjectData::helpManual->show();
	}
	else
	{
		ProjectData::helpManual->raise();
		ProjectData::helpManual->activateWindow();
	}
}

void MainWindow::on_aAbout_ORPGM_triggered()
{
	About *about = new About;
	about->exec();
	delete about;
}

void MainWindow::on_aNewEmptyMap_triggered()
{
	int mapID;
	ignoreEvents = true;
	mapID = ProjectData::addMap(contextMenuItem);
	if (mapRef != NULL)
	{
		delete mapRef;
		mapRef = NULL;
	}
	ProjectData::changeCurrentMap(mapID);
	gvMapEditor->changeLayer(0);
	gvMapEditor->changeMap(mapID, true);
	mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	updateLayerList();
	frmMapEditor->setEnabled(true);
	gvMapEditor->setEnabled(true);
	aEditLayers->setEnabled(true);
	aEditPassability->setEnabled(true);
	aEdit_EventLayer->setEnabled(true);
	aZoom_1to1->setEnabled(true);
	aZoom_1to2->setEnabled(true);
	aZoom_1to4->setEnabled(true);
	aZoom_1to8->setEnabled(true);
	toolRenderPreview->setEnabled(true);
	toolMakeSelection->setEnabled(true);
	toolDrawRectangle->setEnabled(true);
	toolDrawCircle->setEnabled(true);
	toolBucketFill->setEnabled(true);
	toolPencil->setEnabled(true);
	toolTiling->setEnabled(true);
	bEditLayers->setEnabled(true);
	bEditPassability->setEnabled(true);
	editEventLayer->setEnabled(true);
	zoom1to1->setEnabled(true);
	zoom1to2->setEnabled(true);
	zoom1to4->setEnabled(true);
	zoom1to8->setEnabled(true);
	ignoreEvents = false;
	contextMenuItem = NULL;
	contextMenuMapID = -1;
}

void MainWindow::on_aNewRandomMap_triggered()
{
	RandomMapGenerator *generator = new RandomMapGenerator;
	QTreeWidgetItem *topLevelMapItem = NULL;
	Map *map;
	StorageFile storageFile;
	int numMaps, topLevelMapID = 0;
	if (generator->exec())
	{
		numMaps = generator->numGeneratedMaps();
		ignoreEvents = true;
		for (int i = 0; i < numMaps; ++i)
		{
			if (i == 0)
			{
				map = generator->getGeneratedMap(i);
				topLevelMapID = ProjectData::addMap(contextMenuItem, map);
				delete map;
				topLevelMapItem = contextMenuItem->child(contextMenuItem->childCount() - 1);
			}
			else
			{
				map = generator->getGeneratedMap(i);
				ProjectData::addMap(topLevelMapItem, map);
				delete map;
			}
		}
		twMapList->setCurrentItem(topLevelMapItem);
		gvMapEditor->changeMap(topLevelMapID, true);
		frmMapEditor->setEnabled(true);
		gvMapEditor->setEnabled(true);
		aEditLayers->setEnabled(true);
		aEditPassability->setEnabled(true);
		aEdit_EventLayer->setEnabled(true);
		aZoom_1to1->setEnabled(true);
		aZoom_1to2->setEnabled(true);
		aZoom_1to4->setEnabled(true);
		aZoom_1to8->setEnabled(true);
		toolRenderPreview->setEnabled(true);
		toolMakeSelection->setEnabled(true);
		toolDrawRectangle->setEnabled(true);
		toolDrawCircle->setEnabled(true);
		toolBucketFill->setEnabled(true);
		toolPencil->setEnabled(true);
		toolTiling->setEnabled(true);
		bEditLayers->setEnabled(true);
		bEditPassability->setEnabled(true);
		editEventLayer->setEnabled(true);
		zoom1to1->setEnabled(true);
		zoom1to2->setEnabled(true);
		zoom1to4->setEnabled(true);
		zoom1to8->setEnabled(true);
		ignoreEvents = false;
	}
	delete generator;
	contextMenuItem = NULL;
	contextMenuMapID = -1;
}

void MainWindow::on_aDeleteMap_triggered()
{
	int confirmation = QMessageBox::question(NULL, "Confirm Delete", "Are you sure you want to delete the selected map?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
	if (confirmation == QMessageBox::Yes)
		ProjectData::deleteMap(contextMenuItem);
	contextMenuItem = NULL;
	contextMenuMapID = -1;
}

void MainWindow::on_aNewArea_triggered()
{
	TODO("Implement function");
	contextMenuItem = NULL;
	contextMenuMapID = -1;
}

void MainWindow::on_aDeleteArea_triggered()
{
	int confirmation = QMessageBox::question(NULL, "Confirm Delete", "Are you sure you want to delete the selected map area?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
	if (confirmation == QMessageBox::Yes)
		ProjectData::deleteMapArea(contextMenuItem);
	contextMenuItem = NULL;
	contextMenuMapID = -1;
	contextMenuAreaID = -1;
}

void MainWindow::on_aMapProperties_triggered()
{
	MapProperties *mapproperties = new MapProperties;
	if (twMapList->currentItem()->parent()->text(1) == "top")
		(*mapRef)->setParent(-1);
	else
		(*mapRef)->setParent(twMapList->currentItem()->parent()->text(1).toInt(NULL, 16));
	mapproperties->setupMapProperties(contextMenuMapID);
	mapproperties->exec();
	delete mapproperties;
	contextMenuItem->setText(0, (*mapRef)->getName());
	contextMenuItem = NULL;
	contextMenuMapID = -1;
	gvMapEditor->changeMap(twMapList->currentItem()->text(1).toInt(NULL, 16), true);
}

void MainWindow::on_aEditOnloadEvent_triggered()
{
	if (contextMenuMapID != -1)
	{
		ConfigureMapOnLoadEvent *configEvent = new ConfigureMapOnLoadEvent(contextMenuMapID, this);
		configEvent->exec();
		delete configEvent;
	}
	contextMenuItem = NULL;
	contextMenuMapID = -1;
}

void MainWindow::on_bMoveLayerUp_clicked()
{
	if (mapRef != NULL)
	{
		int row = twLayers->currentRow();
		if (row > 0)
		{
			(*mapRef)->moveLayerUp(row);
			updateLayerList();
			twLayers->item(row - 1, 1)->setSelected(true);
			twLayers->setCurrentCell(row - 1, 1);
			gvMapEditor->updateMapView();
			ProjectData::saved = false;
		}
	}
}

void MainWindow::on_bMoveLayerDown_clicked()
{
	if (mapRef != NULL)
	{
		int row = twLayers->currentRow();
		if (row < (*mapRef)->numLayers() - 1)
		{
			(*mapRef)->moveLayerDown(row);
			updateLayerList();
			twLayers->item(row + 1, 1)->setSelected(true);
			twLayers->setCurrentCell(row + 1, 1);
			gvMapEditor->updateMapView();
			ProjectData::saved = false;
		}
	}
}

void MainWindow::on_bDeleteLayer_clicked()
{
	if (mapRef != NULL)
	{
		int row = twLayers->currentRow();
		if (row >= 0 && row < (*mapRef)->numLayers())
		{
			(*mapRef)->deleteLayer(row);
			updateLayerList();
			ProjectData::saved = false;
		}
	}
}

void MainWindow::on_bAddLayer_clicked()
{
	if (mapRef != NULL)
	{
		int numLayers = (*mapRef)->numLayers();
		(*mapRef)->addLayer(QString("Layer %1").arg(numLayers + 1));
		updateLayerList();
		twLayers->item(numLayers, 1)->setSelected(true);
		twLayers->setCurrentCell(numLayers, 1);
		ProjectData::saved = false;
	}
}

void MainWindow::on_twLayers_itemDoubleClicked(QTableWidgetItem *item)
{
	if (item->column() == 1 && mapRef != NULL && !ignoreEvents)
	{
		ignoreEvents = true;
		item->setText((*mapRef)->getLayerName(item->row()));
		ignoreEvents = false;
	}
}

void MainWindow::on_twLayers_itemChanged(QTableWidgetItem *item)
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
		gvMapEditor->updateMapView();
		ignoreEvents = false;
	}
	if (column == 1 && mapRef != NULL && !ignoreEvents)
	{
		ignoreEvents = true;
		(*mapRef)->setLayerName(row, item->text());
		item->setText(QString("%1: %2").arg(row).arg(item->text()));
		ignoreEvents = false;
	}
}

void MainWindow::on_twLayers_itemSelectionChanged()
{
	if (!ignoreEvents && mapRef != NULL)
	{
		int row = twLayers->currentRow();
		if (row >= 0 && row < (*mapRef)->numLayers())
			gvMapEditor->changeLayer(row);
		bMoveLayerUp->setEnabled(row > 0);
		bMoveLayerDown->setEnabled(row < (*mapRef)->numLayers() - 1);
	}
}

void MainWindow::on_bConfigurePhases_clicked()
{
	ConfigurePhases *configurePhases = new ConfigurePhases(this);
	if (configurePhases->exec())
	{
		for (int i = 0; i < NUM_PHASES; ++i)
			cboxPhases->setItemText(i, ProjectData::phaseNames[i]);
	}
	delete configurePhases;
}

void MainWindow::on_cboxPhases_currentIndexChanged(int index)
{
	gvMapEditor->changePhase(index);
}

void MainWindow::on_rbSetPassability_toggled(bool on)
{
	if (on)
		gvMapEditor->setPassabilityMode(MapView::PASSMODE_SET);
}

void MainWindow::on_rbClearPassability_toggled(bool on)
{
	if (on)
		gvMapEditor->setPassabilityMode(MapView::PASSMODE_CLEAR);
}

void MainWindow::on_rbDirectionPassability_toggled(bool on)
{
	if (on)
	{
		if (aEditPassability->isChecked())
		{
			toolPencil->setChecked(true);
			toolDrawRectangle->setEnabled(false);
			toolDrawCircle->setEnabled(false);
			toolBucketFill->setEnabled(false);
		}
		gvMapEditor->setPassabilityMode(MapView::PASSMODE_DIRECTIONS);
	}
	else if (aEditPassability->isChecked())
	{
		toolDrawRectangle->setEnabled(true);
		toolDrawCircle->setEnabled(true);
		toolBucketFill->setEnabled(true);
	}
}

void MainWindow::on_aGeneratePreview_triggered()
{
	MapPreview *mappreview = new MapPreview;
	int row = twLayers->currentRow();
	mappreview->setupMapPreview(twMapList->currentItem()->text(1).toInt(NULL, 16));
	mappreview->exec();
	ignoreEvents = true;
	for (int i = 0; i < (*mapRef)->numLayers(); ++i)
		twLayers->item(i, 0)->setCheckState(((*mapRef)->isLayerVisible(i)) ? Qt::Checked:Qt::Unchecked);
	twLayers->item(row, 1)->setSelected(true);
	twLayers->setCurrentCell(row, 1);
	ignoreEvents = false;
	gvMapEditor->updateMapView();
	delete mappreview;
}

void MainWindow::on_aToggleGrid_toggled(bool on)
{
	gvMapEditor->setShowGrid(on);
}

void MainWindow::on_toolMakeSelection_toggled(bool on)
{
	if (on)
	{
		toolDrawRectangle->setChecked(false);
		toolDrawCircle->setChecked(false);
		toolBucketFill->setChecked(false);
		toolPencil->setChecked(false);
		aEditPassability->setEnabled(false);
		aEdit_EventLayer->setEnabled(false);
		bEditPassability->setEnabled(false);
		editEventLayer->setEnabled(false);
		gvMapEditor->changeTool(MapView::TOOL_SELECTION);
		if (toolTiling->isChecked())
		{
			toolTiling->setChecked(false);
			if (aZoom_1to1->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO1);
			if (aZoom_1to2->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO2);
			if (aZoom_1to4->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO4);
			if (aZoom_1to8->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO8);
		}
	}
	else if (!toolDrawRectangle->isChecked() and !toolDrawCircle->isChecked() and !toolBucketFill->isChecked() and !toolPencil->isChecked() and !toolTiling->isChecked())
		toolMakeSelection->setChecked(true);
	else if (!toolTiling->isChecked())
	{
		aEditPassability->setEnabled(true);
		aEdit_EventLayer->setEnabled(true);
		bEditPassability->setEnabled(true);
		editEventLayer->setEnabled(true);
	}
}

void MainWindow::on_toolDrawRectangle_toggled(bool on)
{
	if (on)
	{
		toolMakeSelection->setChecked(false);
		toolDrawCircle->setChecked(false);
		toolBucketFill->setChecked(false);
		toolPencil->setChecked(false);
		gvMapEditor->changeTool(MapView::TOOL_RECTANGLE);
		if (toolTiling->isChecked())
		{
			toolTiling->setChecked(false);
			if (aZoom_1to1->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO1);
			if (aZoom_1to2->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO2);
			if (aZoom_1to4->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO4);
			if (aZoom_1to8->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO8);
		}
	}
	else if (!toolMakeSelection->isChecked() and !toolDrawCircle->isChecked() and !toolBucketFill->isChecked() and !toolPencil->isChecked() and !toolTiling->isChecked())
		toolDrawRectangle->setChecked(true);
}

void MainWindow::on_toolDrawCircle_toggled(bool on)
{
	if (on)
	{
		toolMakeSelection->setChecked(false);
		toolDrawRectangle->setChecked(false);
		toolBucketFill->setChecked(false);
		toolPencil->setChecked(false);
		gvMapEditor->changeTool(MapView::TOOL_CIRCLE);
		if (toolTiling->isChecked())
		{
			toolTiling->setChecked(false);
			if (aZoom_1to1->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO1);
			if (aZoom_1to2->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO2);
			if (aZoom_1to4->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO4);
			if (aZoom_1to8->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO8);
		}
	}
	else if (!toolMakeSelection->isChecked() and !toolDrawRectangle->isChecked() and !toolBucketFill->isChecked() and !toolPencil->isChecked() and !toolTiling->isChecked())
		toolDrawCircle->setChecked(true);
}

void MainWindow::on_toolBucketFill_toggled(bool on)
{
	if (on)
	{
		toolMakeSelection->setChecked(false);
		toolDrawRectangle->setChecked(false);
		toolDrawCircle->setChecked(false);
		toolPencil->setChecked(false);
		gvMapEditor->changeTool(MapView::TOOL_FILL);
		if (toolTiling->isChecked())
		{
			toolTiling->setChecked(false);
			if (aZoom_1to1->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO1);
			if (aZoom_1to2->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO2);
			if (aZoom_1to4->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO4);
			if (aZoom_1to8->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO8);
		}
	}
	else if (!toolMakeSelection->isChecked() and !toolDrawRectangle->isChecked() and !toolDrawCircle->isChecked() and !toolPencil->isChecked() and !toolTiling->isChecked())
		toolBucketFill->setChecked(true);
}

void MainWindow::on_toolPencil_toggled(bool on)
{
	if (on)
	{
		toolMakeSelection->setChecked(false);
		toolDrawRectangle->setChecked(false);
		toolDrawCircle->setChecked(false);
		toolBucketFill->setChecked(false);
		gvMapEditor->changeTool(MapView::TOOL_PENCIL);
		if (toolTiling->isChecked())
		{
			toolTiling->setChecked(false);
			if (aZoom_1to1->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO1);
			if (aZoom_1to2->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO2);
			if (aZoom_1to4->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO4);
			if (aZoom_1to8->isChecked())
				gvMapEditor->changeZoom(MapView::ZOOM_1TO8);
		}
	}
	else if (!toolMakeSelection->isChecked() and !toolDrawRectangle->isChecked() and !toolDrawCircle->isChecked() and !toolBucketFill->isChecked() and !toolTiling->isChecked())
		toolPencil->setChecked(true);
}

void MainWindow::on_toolTiling_toggled(bool on)
{
	if (on)
	{
		toolMakeSelection->setChecked(false);
		toolDrawRectangle->setChecked(false);
		toolDrawCircle->setChecked(false);
		toolBucketFill->setChecked(false);
		toolPencil->setChecked(false);
		zoom1to1->setEnabled(false);
		zoom1to2->setEnabled(false);
		zoom1to4->setEnabled(false);
		zoom1to8->setEnabled(false);
		aZoom_1to1->setEnabled(false);
		aZoom_1to2->setEnabled(false);
		aZoom_1to4->setEnabled(false);
		aZoom_1to8->setEnabled(false);
		aEditPassability->setEnabled(false);
		aEdit_EventLayer->setEnabled(false);
		bEditPassability->setEnabled(false);
		editEventLayer->setEnabled(false);
		gvMapEditor->changeTool(MapView::TOOL_TILING);
		gvMapEditor->changeZoom(MapView::ZOOM_3TO2);
	}
	else if (!toolMakeSelection->isChecked() and !toolDrawRectangle->isChecked() and !toolDrawCircle->isChecked() and !toolBucketFill->isChecked() and !toolPencil->isChecked())
		toolTiling->setChecked(true);
	else
	{
		zoom1to1->setEnabled(true);
		zoom1to2->setEnabled(true);
		zoom1to4->setEnabled(true);
		zoom1to8->setEnabled(true);
		aZoom_1to1->setEnabled(true);
		aZoom_1to2->setEnabled(true);
		aZoom_1to4->setEnabled(true);
		aZoom_1to8->setEnabled(true);
		if (!toolMakeSelection->isChecked())
		{
			aEditPassability->setEnabled(true);
			aEdit_EventLayer->setEnabled(true);
			bEditPassability->setEnabled(true);
			editEventLayer->setEnabled(true);
		}
	}
}

void MainWindow::on_aEditLayers_toggled(bool on)
{
	if (on)
	{
		aEditPassability->setChecked(false);
		aEdit_EventLayer->setChecked(false);
		gvMapEditor->changeEditingMode(MapView::EDITMODE_LAYERS);
		toolTiling->setEnabled(true);
		toolMakeSelection->setEnabled(true);
		toolDrawRectangle->setEnabled(true);
		toolDrawCircle->setEnabled(true);
		toolBucketFill->setEnabled(true);
	}
	else if (!aEditPassability->isChecked() && !aEdit_EventLayer->isChecked())
		aEditLayers->setChecked(true);
	else
	{
		toolTiling->setEnabled(false);
		toolMakeSelection->setEnabled(false);
	}
}

void MainWindow::on_aEditPassability_toggled(bool on)
{
	if (on)
	{
		if (rbDirectionPassability->isChecked())
		{
			toolPencil->setChecked(true);
			toolDrawRectangle->setEnabled(false);
			toolDrawCircle->setEnabled(false);
			toolBucketFill->setEnabled(false);
		}
		else
		{
			toolDrawRectangle->setEnabled(true);
			toolDrawCircle->setEnabled(true);
			toolBucketFill->setEnabled(true);
		}
		aEditLayers->setChecked(false);
		aEdit_EventLayer->setChecked(false);
		gvMapEditor->changeEditingMode(MapView::EDITMODE_PASSABILITY);
	}
	else if (!aEditLayers->isChecked() && !aEdit_EventLayer->isChecked())
		aEditPassability->setChecked(true);
}

void MainWindow::on_aEdit_EventLayer_toggled(bool on)
{
	if (on)
	{
		aEditLayers->setChecked(false);
		aEditPassability->setChecked(false);
		gvMapEditor->changeEditingMode(MapView::EDITMODE_EVENTS);
		toolDrawRectangle->setEnabled(false);
		toolDrawCircle->setEnabled(false);
		toolBucketFill->setEnabled(false);
	}
	else if (!aEditLayers->isChecked() && !aEditPassability->isChecked())
		aEdit_EventLayer->setChecked(true);
}

void MainWindow::on_aZoom_1to1_toggled(bool on)
{
	if (on)
	{
		aZoom_1to2->setChecked(false);
		aZoom_1to4->setChecked(false);
		aZoom_1to8->setChecked(false);
		gvMapEditor->changeZoom(MapView::ZOOM_1TO1);
	}
	else if (!aZoom_1to2->isChecked() and !aZoom_1to4->isChecked() and !aZoom_1to8->isChecked())
		aZoom_1to1->setChecked(true);
}

void MainWindow::on_aZoom_1to2_toggled(bool on)
{
	if (on)
	{
		aZoom_1to1->setChecked(false);
		aZoom_1to4->setChecked(false);
		aZoom_1to8->setChecked(false);
		gvMapEditor->changeZoom(MapView::ZOOM_1TO2);
	}
	else if (!aZoom_1to1->isChecked() and !aZoom_1to4->isChecked() and !aZoom_1to8->isChecked())
		aZoom_1to2->setChecked(true);
}

void MainWindow::on_aZoom_1to4_toggled(bool on)
{
	if (on)
	{
		aZoom_1to1->setChecked(false);
		aZoom_1to2->setChecked(false);
		aZoom_1to8->setChecked(false);
		gvMapEditor->changeZoom(MapView::ZOOM_1TO4);
	}
	else if (!aZoom_1to1->isChecked() and !aZoom_1to2->isChecked() and !aZoom_1to8->isChecked())
		aZoom_1to4->setChecked(true);
}

void MainWindow::on_aZoom_1to8_toggled(bool on)
{
	if (on)
	{
		aZoom_1to1->setChecked(false);
		aZoom_1to2->setChecked(false);
		aZoom_1to4->setChecked(false);
		gvMapEditor->changeZoom(MapView::ZOOM_1TO8);
	}
	else if (!aZoom_1to1->isChecked() and !aZoom_1to2->isChecked() and !aZoom_1to4->isChecked())
		aZoom_1to8->setChecked(true);
}

void MainWindow::on_gvTileSelection_scrollBarRangeChanged(int min, int max)
{
	if (min != sbTileView->minimum() || max != sbTileView->maximum())
		sbTileView->setRange(min, max);
}

void MainWindow::on_gvTileSelection_scrollBarValueChanged(int value)
{
	if (value != sbTileView->value())
		sbTileView->setValue(value);
}

void MainWindow::on_twMapList_itemExpanded(QTreeWidgetItem *item)
{
	if (item->text(1) != "top")
	{
		if (item->text(3) == "map")
			item->setText(2, "true");
	}
}

void MainWindow::on_twMapList_itemCollapsed(QTreeWidgetItem *item)
{
	if (item->text(1) == "top")
		item->setExpanded(true);
	else if (item->text(3) == "map")
		item->setText(2, "false");
}

void MainWindow::on_twMapList_itemDoubleClicked(QTreeWidgetItem *item)
{
	if (item->text(1) != "top" && item->text(3) == "map" && mapRef != NULL)
	{
		MapProperties *mapproperties = new MapProperties;
		int mapID = item->text(1).toInt(NULL, 16);
		if (item->parent()->text(1) == "top")
			(*mapRef)->setParent(-1);
		else
			(*mapRef)->setParent(item->parent()->text(1).toInt(NULL, 16));
		mapproperties->setupMapProperties(mapID);
		mapproperties->exec();
		delete mapproperties;
		item->setText(0, (*mapRef)->getName());
		gvMapEditor->changeMap(mapID, true);
	}
}

void MainWindow::on_twMapList_itemSelectionChanged()
{
	QTreeWidgetItem *currentItem = twMapList->currentItem();
	if (!ignoreEvents)
	{
		if (currentItem->text(1) != "top")
		{
			if (mapRef != NULL)
			{
				delete mapRef;
				mapRef = NULL;
			}
			if (currentItem->text(3) == "map")
			{
				ProjectData::changeCurrentMap(currentItem->text(1).toInt(NULL, 16));
				gvMapEditor->changeLayer(0);
				gvMapEditor->changeMap(currentItem->text(1).toInt(NULL, 16));
				mapRef = new ProjectData::DataReference<Map>(currentItem->text(1).toInt(NULL, 16), __FILE__, __LINE__);
			}
			else if (currentItem->text(3) == "area")
			{
				gvMapEditor->changeLayer(0);
				gvMapEditor->changeMap(currentItem->parent()->text(1).toInt(NULL, 16));
				mapRef = new ProjectData::DataReference<Map>(currentItem->parent()->text(1).toInt(NULL, 16), __FILE__, __LINE__);
				TODO("Code to edit map area");
			}
			frmMapEditor->setEnabled(true);
			gvMapEditor->setEnabled(true);
			aEditLayers->setEnabled(true);
			aEditPassability->setEnabled(true);
			aEdit_EventLayer->setEnabled(true);
			aZoom_1to1->setEnabled(true);
			aZoom_1to2->setEnabled(true);
			aZoom_1to4->setEnabled(true);
			aZoom_1to8->setEnabled(true);
			toolRenderPreview->setEnabled(true);
			toolMakeSelection->setEnabled(true);
			toolDrawRectangle->setEnabled(true);
			toolDrawCircle->setEnabled(true);
			toolBucketFill->setEnabled(true);
			toolPencil->setEnabled(true);
			toolTiling->setEnabled(true);
			bEditLayers->setEnabled(true);
			bEditPassability->setEnabled(true);
			editEventLayer->setEnabled(true);
			zoom1to1->setEnabled(true);
			zoom1to2->setEnabled(true);
			zoom1to4->setEnabled(true);
			zoom1to8->setEnabled(true);
			updateLayerList();
		}
		else
		{
			gvMapEditor->setEnabled(false);
			aGeneratePreview->setEnabled(false);
			aEditLayers->setEnabled(false);
			aEditPassability->setEnabled(false);
			aEdit_EventLayer->setEnabled(false);
			aZoom_1to1->setEnabled(false);
			aZoom_1to2->setEnabled(false);
			aZoom_1to4->setEnabled(false);
			aZoom_1to8->setEnabled(false);
			toolRenderPreview->setEnabled(false);
			toolMakeSelection->setEnabled(false);
			toolDrawRectangle->setEnabled(false);
			toolDrawCircle->setEnabled(false);
			toolBucketFill->setEnabled(false);
			toolPencil->setEnabled(false);
			toolTiling->setEnabled(false);
			bEditLayers->setEnabled(false);
			bEditPassability->setEnabled(false);
			editEventLayer->setEnabled(false);
			zoom1to1->setEnabled(false);
			zoom1to2->setEnabled(false);
			zoom1to4->setEnabled(false);
			zoom1to8->setEnabled(false);
		}
	}
}

void MainWindow::on_twMapList_customContextMenuRequested(const QPoint &pos)
{
	contextMenuItem = twMapList->itemAt(pos);
	if (contextMenuItem != NULL)
	{
		QMenu *popupMenu = new QMenu(this);
		if (contextMenuItem->text(1) == "top")
		{
			contextMenuMapID = -1;
			contextMenuAreaID = -1;
			popupMenu->addAction(aNewEmptyMap);
			popupMenu->addAction(aNewRandomMap);
		}
		else if (contextMenuItem->text(3) == "map")
		{
			contextMenuMapID = contextMenuItem->text(1).toInt(0, 16);
			contextMenuAreaID = -1;
			popupMenu->addAction(aNewEmptyMap);
			popupMenu->addAction(aNewRandomMap);
			popupMenu->addAction(aNewArea);
			popupMenu->addAction(aDeleteMap);
			popupMenu->addAction(aMapProperties);
			popupMenu->addAction(aEditOnloadEvent);
		}
		else if (contextMenuItem->text(3) == "area")
		{
			contextMenuMapID = contextMenuItem->parent()->text(1).toInt();
			contextMenuAreaID = contextMenuItem->text(1).toInt();
			popupMenu->addAction(aDeleteArea);
		}
		popupMenu->exec(twMapList->mapToGlobal(pos));
	}
}

void MainWindow::on_editSelectTileset_currentIndexChanged(int index)
{
	if (index >= 0)
	{
		ProjectData::changeCurrentTileset(index);
		gvTileSelection->changeTileset(index);
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (ProjectData::open && !ProjectData::saved)
	{
		int confirmsave = QMessageBox::warning(0, "Save Changes?", "If you close now all unsaved data will be lost.\nDo you wish to save project data?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
		if (confirmsave == QMessageBox::Save)
		{
			ProjectData::saveProject();
			event->accept();
		}
		else if (confirmsave == QMessageBox::Cancel)
			event->ignore();
		else
			event->accept();
	}
	else
		event->accept();
	if (event->isAccepted())
	{
		gvMapEditor->shutdownView();
		gvTileSelection->shutdownView();
		ProjectData::helpManual->close();
		delete ProjectData::helpManual;
		ProjectData::helpManual = NULL;
		Clipboard::clear();
		ProjectData::saveConfiguration();
		if (ProjectData::open)
			ProjectData::closeProject();
		if (enableAudio)
		{
			AudioManager::shutdownAudioManager();
			delete ProjectData::audioManager;
		}
	}
}

void MainWindow::updateTilesetList()
{
	currentTileset = editSelectTileset->currentIndex();
	editSelectTileset->clear();
	if (ProjectData::open)
	{
		for (int i = 0; i < ProjectData::tilesetNames.size(); ++i)
			editSelectTileset->addItem(ProjectData::tilesetNames[i]);
		if (editSelectTileset->count() < currentTileset)
			currentTileset = editSelectTileset->count() - 1;
		else if (currentTileset < 0)
			currentTileset = 0;
		editSelectTileset->setCurrentIndex(currentTileset);
	}
}

void MainWindow::updateLayerList()
{
	QTableWidgetItem *item;
	if (mapRef != NULL)
	{
		int numLayers = (*mapRef)->numLayers();
		twLayers->clearContents();
		twLayers->setRowCount(numLayers);
		for (int i = 0; i < numLayers; ++i)
		{
			Map::LayerData *layerData = (*mapRef)->getLayerData(i);
			item = new QTableWidgetItem();
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable);
			item->setCheckState((layerData->visible) ? Qt::Checked:Qt::Unchecked);
			twLayers->setItem(i, 0, item);
			item = new QTableWidgetItem(layerData->name);
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
			twLayers->setItem(i, 1, item);
			twLayers->setRowHeight(i, 16);
		}
		twLayers->item(0, 0)->setSelected(true);
		twLayers->item(0, 1)->setSelected(true);
		twLayers->setCurrentCell(0, 1);
		bAddLayer->setEnabled((*mapRef)->numLayers() < 99);
		bDeleteLayer->setEnabled((*mapRef)->numLayers() > 1);
	}
}

void MainWindow::enableDisableMainWindow(bool enable)
{
	frmMapSelector->setEnabled(enable);
	frmMapEditor->setEnabled(enable);
	frmLayers->setEnabled(enable);
	frmPassability->setEnabled(enable);
	gvMapEditor->setEnabled(enable);
	frmTileSelector->setEnabled(enable);
	gvTileSelection->setEnabled(enable);
	sbTileView->setEnabled(enable);
	aCloseProject->setEnabled(enable);
	aChangeProjectName->setEnabled(enable);
	aCreateGameDisk->setEnabled(enable);
	aSaveChanges->setEnabled(enable);
	aRevertChanges->setEnabled(enable);
	aDatabase->setEnabled(enable);
	aResourceManager->setEnabled(enable);
	aImport->setEnabled(enable);
	aListenToMusic->setEnabled(enable);
	aFindEvent->setEnabled(enable);
	aTestPlay->setEnabled(enable);
	aGameOptions_FS->setEnabled(enable);
	aGameOptions_DisplayTitle->setEnabled(enable);
	aGeneratePreview->setEnabled(enable);
	aToggleGrid->setEnabled(enable);
	aEditLayers->setEnabled(enable);
	aEditPassability->setEnabled(enable);
	aEdit_EventLayer->setEnabled(enable);
	aZoom_1to1->setEnabled(enable);
	aZoom_1to2->setEnabled(enable);
	aZoom_1to4->setEnabled(enable);
	aZoom_1to8->setEnabled(enable);
	toolRenderPreview->setEnabled(enable);
	toolMakeSelection->setEnabled(enable);
	toolDrawRectangle->setEnabled(enable);
	toolDrawCircle->setEnabled(enable);
	toolBucketFill->setEnabled(enable);
	toolPencil->setEnabled(enable);
	toolTiling->setEnabled(enable);
	bEditLayers->setEnabled(enable);
	bEditPassability->setEnabled(enable);
	editEventLayer->setEnabled(enable);
	zoom1to1->setEnabled(enable);
	zoom1to2->setEnabled(enable);
	zoom1to4->setEnabled(enable);
	zoom1to8->setEnabled(enable);
	if (enable && (toolMakeSelection->isChecked() || toolTiling->isChecked()))
	{
		aEditPassability->setEnabled(false);
		aEdit_EventLayer->setEnabled(false);
		bEditPassability->setEnabled(false);
		editEventLayer->setEnabled(false);
		if (toolTiling->isChecked())
		{
			aZoom_1to1->setEnabled(false);
			aZoom_1to2->setEnabled(false);
			aZoom_1to4->setEnabled(false);
			aZoom_1to8->setEnabled(false);
			zoom1to1->setEnabled(false);
			zoom1to2->setEnabled(false);
			zoom1to4->setEnabled(false);
			zoom1to8->setEnabled(false);
		}
	}
	else if (enable && (aEditPassability->isChecked() || aEdit_EventLayer->isChecked()))
	{
		toolMakeSelection->setEnabled(false);
		toolTiling->setEnabled(false);
	}
}
