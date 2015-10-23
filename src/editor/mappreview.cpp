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

#include <QFileDialog>
#include <QDir>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mappreview.h"
#include "projectdata.h"
#include "projectdata_map.h"

MapPreview::MapPreview(QWidget *parent) : QDialog(parent)
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

MapPreview::~MapPreview()
{
	gvMapPreview->shutdownView();
}

void MapPreview::setupMapPreview(int mapID)
{
	QTableWidgetItem *item;
	int numLayers;
	ignoreEvents = true;
	gvMapPreview->changeMap(mapID);
	gvMapPreview->changeLayer(0);
	gvMapPreview->changeEditingMode(MapView::EDITMODE_PREVIEW);
	gvMapPreview->setupView();
	mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	numLayers = (*mapRef)->numLayers();
	twLayers->setRowCount(numLayers);
	for (int i = 0; i < numLayers; ++i)
	{
		Map::LayerData *layerData = (*mapRef)->getLayerData(i);
		item = new QTableWidgetItem();
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		item->setCheckState((layerData->visible) ? Qt::Checked:Qt::Unchecked);
		twLayers->setItem(i, 0, item);
		item = new QTableWidgetItem(layerData->name);
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
		twLayers->setItem(i, 1, item);
		twLayers->setRowHeight(i, 16);
	}
	ignoreEvents = false;
}

void MapPreview::on_twLayers_itemChanged(QTableWidgetItem *item)
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
		gvMapPreview->updateMapView();
		ignoreEvents = false;
	}
}

void MapPreview::on_buttonBox_clicked(QAbstractButton *abstractButton)
{
	int button = buttonBox->standardButton(abstractButton);
	if (button == QDialogButtonBox::Save)
	{
		QString filelocation = QFileDialog::getSaveFileName(0, "Save Location", QDir::homePath(), "PNG Images (*.png)");
		if (!filelocation.isEmpty())
			gvMapPreview->savePreview(filelocation);
	}
	else if (button == QDialogButtonBox::Help)
	{
		ProjectData::helpManual->loadIndex("Map Preview");
		if (!ProjectData::helpManual->isVisible())
			ProjectData::helpManual->show();
		else
			ProjectData::helpManual->raise();
		ProjectData::helpManual->activateWindow();
	}
	else if (button == QDialogButtonBox::Ok)
		accept();
}
