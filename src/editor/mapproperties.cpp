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
#include <QFileInfo>
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "audiomanager.h"
#include "imagebrowser.h"
#include "mapproperties.h"
#include "monsterencounterdelegate.h"
#include "musicbrowser.h"
#include "projectdata.h"
#include "projectdata_map.h"
#include "storagefile.h"
#include "tilechooser.h"

MapProperties::MapProperties(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	monsterEncounterDelegate = new MonsterEncounterDelegate(0, 1);
	twMonsterEncounters->setItemDelegate(monsterEncounterDelegate);
	twMonsterEncounters->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
	twMonsterEncounters->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
	twMonsterEncounters->horizontalHeader()->resizeSection(1, 54);
	twMonsterEncounters->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	QObject::connect(twMonsterEncounters, SIGNAL(clicked(const QModelIndex&)), twMonsterEncounters, SLOT(edit(const QModelIndex&)));
	tbSubEncounter->setEnabled(false);
	frmBGImage->setEnabled(false);
	frmBGScrolling->setEnabled(false);
	frmBGMSelector->setEnabled(false);
	frmBBGSelector->setEnabled(false);
	bgPreviewScene = new QGraphicsScene(0.0, 0.0, 160.0, 120.0);
	gvBGPreview->setScene(bgPreviewScene);
	bgPreviewItem = new QGraphicsPixmapItem;
	bgPreviewItem->setFlags(0x0);
	bgPreviewItem->setPos(0.0, 0.0);
	bgPreviewItem->setZValue(0.0);
	bgPreviewScene->addItem(bgPreviewItem);
	mapRef = NULL;
	bWidthSpinButton->setSpinBox(sbWidth);
	bHeightSpinButton->setSpinBox(sbHeight);
	bEncRateSpinButton->setSpinBox(sbEncRate);
	bMinStepsSpinButton->setSpinBox(sbMinSteps);
	bMaxStepsSpinButton->setSpinBox(sbMaxSteps);
	bHScrollSpeedSpinButton->setSpinBox(sbHScrollSpeed);
	bVScrollSpeedSpinButton->setSpinBox(sbVScrollSpeed);
}

MapProperties::~MapProperties()
{
	if (mapRef != NULL)
	{
		delete mapRef;
		mapRef = NULL;
	}
	delete monsterEncounterDelegate;
}

void MapProperties::setupMapProperties(int mapID)
{
	QPixmap pixmap;
	int tempInt;
	mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	setWindowTitle(QString("Map Properties - %1 (ID %2)").arg((*mapRef)->getName()).arg(mapID, 4, 10, QChar('0')));
	leName->setText((*mapRef)->getName());
	sbWidth->setValue((*mapRef)->getWidth());
	sbHeight->setValue((*mapRef)->getHeight());
	cboxHorizontalMode->setCurrentIndex((*mapRef)->getHorizontalMode());
	cboxVerticalMode->setCurrentIndex((*mapRef)->getVerticalMode());
	bBorderTile->setTile((*mapRef)->getBorderTile());
	twMonsterEncounters->setRowCount((*mapRef)->numMonsterEncounters());
	for (int i = 0, count = (*mapRef)->numMonsterEncounters(); i < count; ++i)
	{
		QTableWidgetItem *item;
		unsigned long encounter = (*mapRef)->getMonsterEncounter(i);
		int groupID = int((encounter >> 16) % 65536);
		int priority = int(encounter % 65536);
		item = new QTableWidgetItem;
		item->setData(Qt::DisplayRole, ProjectData::monsterGroupNames[groupID]);
		item->setData(Qt::UserRole, groupID);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
		item->setTextAlignment(Qt::AlignLeft);
		twMonsterEncounters->setItem(i, 0, item);
		item = new QTableWidgetItem(QString::number(priority));
		item->setData(Qt::DisplayRole, QString::number(priority));
		item->setData(Qt::UserRole, priority);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
		item->setTextAlignment(Qt::AlignRight);
		twMonsterEncounters->setItem(i, 1, item);
	}
	sbEncRate->setValue((*mapRef)->getEncounterRate());
	sbMinSteps->setValue((*mapRef)->getMinPerBattle());
	sbMaxSteps->setValue((*mapRef)->getMaxPerBattle());
	bgLocation = (*mapRef)->getBGLocation();
	cbUseBG->setChecked(!bgLocation.isEmpty());
	pixmap = QPixmap(bgLocation);
	if (pixmap.width() > 160 || pixmap.height() > 120)
		pixmap = QPixmap(bgLocation).scaled(160, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	bgPreviewItem->setPixmap(pixmap);
	bgPreviewScene->setSceneRect(0.0, 0.0, (qreal)pixmap.width(), (qreal)pixmap.height());
	sbHScrollSpeed->setValue((*mapRef)->getBGHScrollSpeed());
	cbAutoHScroll->setChecked((*mapRef)->getBGAutoHScroll());
	sbVScrollSpeed->setValue((*mapRef)->getBGVScrollSpeed());
	cbAutoVScroll->setChecked((*mapRef)->getBGAutoVScroll());
	tempInt = (*mapRef)->getBGMType();
	leBGMName->setText("(OFF)");
	if (tempInt == Map::BGM_PARENT)
		rbBGM_Parent->setChecked(true);
	else if (tempInt == Map::BGM_EVENT)
		rbBGM_Event->setChecked(true);
	else
	{
		(*mapRef)->getBGMInfo(&bgmInfo);
		leBGMName->setText(QFileInfo(bgmInfo.location).fileName());
		rbBGM_Specify->setChecked(true);
	}
	tempInt = (*mapRef)->getBBGType();
	if (tempInt == Map::BBG_PARENT)
		rbBBG_Parent->setChecked(true);
	else if (tempInt == Map::BBG_TERRAIN)
		rbBBG_Terrain->setChecked(true);
	else
	{
		bbgLocation = (*mapRef)->getBBGLocation();
		leBBGName->setText(QFileInfo(bbgLocation).fileName());
		rbBBG_Specify->setChecked(true);
	}
	tempInt = (*mapRef)->getTeleport();
	if (tempInt == Map::ASINPARENT)
		rbTeleport_Parent->setChecked(true);
	else if (tempInt == Map::ALLOW)
		rbTeleport_Allow->setChecked(true);
	else
		rbTeleport_Forbid->setChecked(true);
	tempInt = (*mapRef)->getEscape();
	if (tempInt == Map::ASINPARENT)
		rbEscape_Parent->setChecked(true);
	else if (tempInt == Map::ALLOW)
		rbEscape_Allow->setChecked(true);
	else
		rbEscape_Forbid->setChecked(true);
	tempInt = (*mapRef)->getSave();
	if (tempInt == Map::ASINPARENT)
		rbSave_Parent->setChecked(true);
	else if (tempInt == Map::ALLOW)
		rbSave_Allow->setChecked(true);
	else
		rbSave_Forbid->setChecked(true);
	if ((*mapRef)->getParent() == -1)
	{
		if (rbBGM_Parent->isChecked())
			rbBGM_Event->setChecked(true);
		if (rbBBG_Parent->isChecked())
			rbBBG_Terrain->setChecked(true);
		if (rbTeleport_Parent->isChecked())
			rbTeleport_Allow->setChecked(true);
		if (rbEscape_Parent->isChecked())
			rbEscape_Allow->setChecked(true);
		if (rbSave_Parent->isChecked())
			rbSave_Allow->setChecked(true);
		rbBGM_Parent->setEnabled(false);
		rbBBG_Parent->setEnabled(false);
		rbTeleport_Parent->setEnabled(false);
		rbEscape_Parent->setEnabled(false);
		rbSave_Parent->setEnabled(false);
	}
}

void MapProperties::on_tbAddEncounter_clicked()
{
	TODO("Implement function");
}

void MapProperties::on_tbSubEncounter_clicked()
{
	TODO("Implement function");
}

void MapProperties::on_cbUseBG_toggled(bool on)
{
	frmBGImage->setEnabled(on);
	frmBGScrolling->setEnabled(on);
}

void MapProperties::on_gvBGPreview_mouseDoubleClickEvent()
{
	bSetBG->click();
}

void MapProperties::on_bSetBG_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser;
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		if (!fileLocation.isEmpty())
		{
			QPixmap pixmap = QPixmap(fileLocation);
			bgLocation = fileLocation;
			if (pixmap.width() > 160 || pixmap.height() > 120)
				bgPreviewItem->setPixmap(pixmap.scaled(160, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			else
				bgPreviewItem->setPixmap(pixmap);
			bgPreviewScene->setSceneRect(0.0, 0.0, (qreal)pixmap.width(), (qreal)pixmap.height());
		}
	}
	delete imageBrowser;
}

void MapProperties::on_rbBGM_Specify_toggled(bool on)
{
	frmBGMSelector->setEnabled(on);
}

void MapProperties::on_tbBGM_Browse_clicked()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	musicBrowser->setupBrowser();
	musicBrowser->setMusicInfo(bgmInfo);
	if (musicBrowser->exec())
	{
		musicBrowser->getMusicInfo(&bgmInfo);
		leBGMName->setText(musicBrowser->getMusicName());
	}
	delete musicBrowser;
}

void MapProperties::on_rbBBG_Specify_toggled(bool on)
{
	frmBBGSelector->setEnabled(on);
}

void MapProperties::on_tbBBG_Browse_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		if (!fileLocation.isEmpty())
		{
			bbgLocation = fileLocation;
			leBBGName->setText(QFileInfo(bbgLocation).fileName());
		}
	}
	delete imageBrowser;
}

void MapProperties::on_buttonBox_accepted()
{
	ProjectData::mapNames[mapRef->getDataID()] = leName->text();
	(*mapRef)->setName(leName->text());
	(*mapRef)->resizeMap(sbWidth->value(), sbHeight->value());
	(*mapRef)->setHorizontalMode(cboxHorizontalMode->currentIndex());
	(*mapRef)->setVerticalMode(cboxVerticalMode->currentIndex());
	(*mapRef)->setBorderTile(bBorderTile->getTile());
	for (int i = 0, count = (*mapRef)->numMonsterEncounters(); i < count; ++i)
		(*mapRef)->deleteMonsterEncounter(0);
	for (int i = 0, count = twMonsterEncounters->rowCount(); i < count; ++i)
	{
		unsigned long monsterID = (unsigned long)(twMonsterEncounters->item(i, 0)->data(Qt::UserRole).toUInt());
		unsigned long priority = (unsigned long)(twMonsterEncounters->item(i, 1)->data(Qt::UserRole).toUInt());
		(*mapRef)->addMonsterEncounter((monsterID << 8) + priority);
	}
	(*mapRef)->setEncounterRate(sbEncRate->value());
	(*mapRef)->setMinPerBattle(sbMinSteps->value());
	(*mapRef)->setMaxPerBattle(sbMaxSteps->value());
	if (cbUseBG->isChecked())
	{
		(*mapRef)->setBGLocation(bgLocation);
		(*mapRef)->setBGHScrollSpeed(sbHScrollSpeed->value());
		(*mapRef)->setBGAutoHScroll(cbAutoHScroll->isChecked());
		(*mapRef)->setBGVScrollSpeed(sbVScrollSpeed->value());
		(*mapRef)->setBGAutoVScroll(cbAutoVScroll->isChecked());
	}
	else
	{
		(*mapRef)->setBGLocation("");
		(*mapRef)->setBGHScrollSpeed(0);
		(*mapRef)->setBGAutoHScroll(false);
		(*mapRef)->setBGVScrollSpeed(0);
		(*mapRef)->setBGAutoVScroll(false);
	}
	if (rbBGM_Parent->isChecked())
		(*mapRef)->setBGMType(Map::BGM_PARENT);
	else if (rbBGM_Event->isChecked())
		(*mapRef)->setBGMType(Map::BGM_EVENT);
	else
	{
		(*mapRef)->setBGMInfo(bgmInfo);
		(*mapRef)->setBGMType(Map::BGM_SPECIFY);
	}
	if (rbBBG_Parent->isChecked())
	{
		(*mapRef)->setBBGLocation("");
		(*mapRef)->setBBGType(Map::BBG_PARENT);
	}
	else if (rbBBG_Terrain->isChecked())
	{
		(*mapRef)->setBBGLocation("");
		(*mapRef)->setBBGType(Map::BBG_TERRAIN);
	}
	else
	{
		(*mapRef)->setBBGLocation(bbgLocation);
		(*mapRef)->setBBGType(Map::BBG_SPECIFY);
	}
	if (rbTeleport_Parent->isChecked())
		(*mapRef)->setTeleport(Map::ASINPARENT);
	else if (rbTeleport_Allow->isChecked())
		(*mapRef)->setTeleport(Map::ALLOW);
	else
		(*mapRef)->setTeleport(Map::FORBID);
	if (rbEscape_Parent->isChecked())
		(*mapRef)->setEscape(Map::ASINPARENT);
	else if (rbEscape_Allow->isChecked())
		(*mapRef)->setEscape(Map::ALLOW);
	else
		(*mapRef)->setEscape(Map::FORBID);
	if (rbSave_Parent->isChecked())
		(*mapRef)->setSave(Map::ASINPARENT);
	else if (rbSave_Allow->isChecked())
		(*mapRef)->setSave(Map::ALLOW);
	else
		(*mapRef)->setSave(Map::FORBID);
	mapRef->saveToStorage();
	delete mapRef;
	mapRef = NULL;
	ProjectData::saved = false;
}

void MapProperties::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
	if (mapRef != NULL)
	{
		delete mapRef;
		mapRef = NULL;
	}
}

void MapProperties::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Map Properties");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}
