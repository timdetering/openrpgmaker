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

#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "battlespriteimagebrowser.h"
#include "clipboard.h"
#include "database_charactersprites.h"
#include "imagebrowser.h"
#include "projectdata.h"
#include "projectdata_charactersprite.h"
#include "xmlParser.h"

Database_CharacterSprites::Database_CharacterSprites(QWidget *parent) : QWidget(parent)
{
	QGraphicsScene *scene;
	setupUi(this);
	poseAnimationTimer = new QTimer(this);
	poseQueueTimer = new QTimer(this);
	poseQueueTimer->setSingleShot(true);
	QObject::connect(poseAnimationTimer, SIGNAL(timeout()), this, SLOT(animatePose()));
	QObject::connect(poseQueueTimer, SIGNAL(timeout()), this, SLOT(animatePoseQueue()));
	scene = new QGraphicsScene(0.0, 0.0, 64.0, 96.0);
	scene->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	mapSprite_gvPoseAnimation->setScene(scene);
	mapSprite_gvPoseAnimation->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	mapSprite_poseAnimationItem = new QGraphicsPixmapItem;
	mapSprite_poseAnimationItem->setPos(8.0, 16.0);
	mapSprite_poseAnimationItem->setZValue(1.0);
	scene->addItem(mapSprite_poseAnimationItem);
	mapSprite_tileGridItem = new QGraphicsPixmapItem(QPixmap(":/images/mapSpriteTileGrid.png"));
	mapSprite_tileGridItem->setPos(-16.0, -16.0);
	mapSprite_tileGridItem->setZValue(0.0);
	scene->addItem(mapSprite_tileGridItem);
	scene = new QGraphicsScene(0.0, 0.0, 48.0, 64.0);
	scene->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	mapSprite_gvQueuePreview->setScene(scene);
	mapSprite_gvQueuePreview->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	mapSprite_poseQueueItem = new QGraphicsPixmapItem;
	mapSprite_poseQueueItem->setPos(0.0, 0.0);
	scene->addItem(mapSprite_poseQueueItem);
	scene = new QGraphicsScene(0.0, 0.0, 0.0, 0.0);
	scene->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	battleSprite_gvPoseAnimation->setScene(scene);
	battleSprite_gvPoseAnimation->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	battleSprite_poseAnimationItem = new QGraphicsPixmapItem;
	battleSprite_poseAnimationItem->setPos(0.0, 0.0);
	scene->addItem(battleSprite_poseAnimationItem);
	scene = new QGraphicsScene(0.0, 0.0, 0.0, 0.0);
	scene->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	battleSprite_gvQueuePreview->setScene(scene);
	battleSprite_gvQueuePreview->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	battleSprite_poseQueueItem = new QGraphicsPixmapItem;
	scene->addItem(battleSprite_poseQueueItem);
	mapSprite_bNumLoopsSideButton->setSpinBox(mapSprite_sbNumLoops);
	battleSprite_bFPSSideButton->setSpinBox(battleSprite_sbFPS);
	mapSprite_poseAnimationLocation = "";
	battleSprite_poseAnimationLocation = "";
	spriteRef = NULL;
	poseAnimationFrame = 0;
	poseAnimationDir = 1;
	poseQueueFrame = 0;
	currentTab = TAB_MAPSPRITE;
	tileGridX = 16.0;
	tileGridY = 16.0;
	tileGridW = 96;
	tileGridH = 128;
	tileGridMoveX = 0;
	tileGridMoveY = 0;
	ignoreEvents = true;
	mapSprite_lwPoseList->setCurrentRow(0);
	battleSprite_lwPoseList->setCurrentRow(0);
}

Database_CharacterSprites::~Database_CharacterSprites()
{
	if (spriteRef != NULL)
	{
		delete spriteRef;
		spriteRef = NULL;
	}
}

void Database_CharacterSprites::setupCharacterSprites()
{
	updateCharacterSpriteList();
	mapSprite_lwPoseList->setCurrentRow(0);
	mapSprite_lwPoseList->item(0)->setSelected(true);
	battleSprite_lwPoseList->setCurrentRow(0);
	battleSprite_lwPoseList->item(0)->setSelected(true);
	ignoreEvents = false;
}

void Database_CharacterSprites::shutdownCharacterSprites()
{
	ignoreEvents = true;
	poseAnimationTimer->stop();
	poseQueueTimer->stop();
	if (spriteRef != NULL)
	{
		delete spriteRef;
		spriteRef = NULL;
	}
}

void Database_CharacterSprites::on_lwCharacterSpriteList_itemSelectionChanged()
{
	int spriteID = lwCharacterSpriteList->currentRow();
	if (spriteRef != NULL)
	{
		delete spriteRef;
		spriteRef = NULL;
	}
	if (spriteID >= 0)
	{
		int mapPoseID = mapSprite_lwPoseList->currentRow();
		int battlePoseID = battleSprite_lwPoseList->currentRow();
		poseAnimationTimer->stop();
		mapSprite_tileGridItem->setVisible(false);
		mapSprite_poseQueueItem->setPixmap(QPixmap());
		mapSprite_poseAnimationItem->setPixmap(QPixmap());
		battleSprite_poseAnimationItem->setPixmap(QPixmap());
		battleSprite_poseQueueItem->setPixmap(QPixmap());
		if (mapPoseID < 0)
			mapPoseID = 0;
		if (battlePoseID < 0)
			battlePoseID = 0;
		mapSprite_lwPoseQueue->clear();
		battleSprite_lwPoseQueue->clear();
		spriteRef = new ProjectData::DataReference<CharacterSprite>(spriteID, __FILE__, __LINE__);
		ignoreEvents = true;
		leName->setText((*spriteRef)->getName());
		mapSprite_sbNumLoops->setValue((*spriteRef)->getMapPoseNumLoops(mapPoseID));
		mapSprite_cbPingPong->setChecked((*spriteRef)->isMapPosePingPong(mapPoseID));
		battleSprite_sbFPS->setValue((*spriteRef)->getBattlePoseFPS(battlePoseID));
		battleSprite_cbPingPong->setChecked((*spriteRef)->isBattlePosePingPong(battlePoseID));
		updatePoseLists();
		ignoreEvents = false;
		mapSprite_poseAnimationLocation = (*spriteRef)->getMapPoseLocation(mapPoseID);
		mapSprite_poseAnimationImage = QImage(mapSprite_poseAnimationLocation);
		battleSprite_poseAnimationLocation = (*spriteRef)->getBattlePoseLocation(battlePoseID);
		battleSprite_poseAnimationImage = QImage(battleSprite_poseAnimationLocation);
		poseAnimationFrame = 0;
		poseAnimationDir = 1;
		tileGridX = 16.0;
		tileGridY = 16.0;
		tileGridMoveX = 0;
		tileGridMoveY = 0;
		if (mapPoseID == CharacterSprite::MAPPOSE_WALKING || mapPoseID == CharacterSprite::MAPPOSE_JUMPING)
		{
			int direction = mapSprite_cboxPreviewDirection->currentIndex();
			mapSprite_tileGridItem->setVisible(true);
			if (direction == DIR_UP)
				tileGridMoveY = -1;
			else if (direction == DIR_DOWN)
				tileGridMoveY = 1;
			else if (direction == DIR_LEFT)
				tileGridMoveX = -1;
			else if (direction == DIR_RIGHT)
				tileGridMoveX = 1;
		}
		else
			mapSprite_tileGridItem->setVisible(false);
		if (currentTab == TAB_MAPSPRITE && !mapSprite_poseAnimationLocation.isEmpty())
		{
			int tilesPerSecondIndex = mapSprite_cboxPreviewMovementSpeed->currentIndex();
			int numFrames = mapSprite_poseAnimationImage.width() / 48;
			int numLoops = mapSprite_sbNumLoops->value();
			poseAnimationTimer->start((int)(1000 / (ProjectData::tilesPerSecond[tilesPerSecondIndex] * numFrames * numLoops)));
		}
		else if (currentTab == TAB_BATTLESPRITE && !battleSprite_poseAnimationLocation.isEmpty())
			poseAnimationTimer->start(1000 / battleSprite_sbFPS->value());
		renderPoseAnimation();
	}
}

void Database_CharacterSprites::on_bArraySize_clicked()
{
	int numCharacterSprites = ProjectData::characterSpriteNames.size();
	ArraySize *arraysize = new ArraySize(numCharacterSprites, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numCharacterSprites)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (spriteRef != NULL)
				{
					if (newSize <= spriteRef->getDataID())
					{
						delete spriteRef;
						spriteRef = NULL;
					}
				}
				for (int i = numCharacterSprites - 1; i >= newSize; --i)
					ProjectData::deleteCharacterSprite(i);
			}
		}
		else
		{
			for (int i = numCharacterSprites; i < newSize; ++i)
			{
				CharacterSprite *characterSprite = new CharacterSprite;
				characterSprite->setName(QString("Sprite%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addCharacterSprite(characterSprite);
				delete characterSprite;
			}
		}
		ignoreEvents = true;
		updateCharacterSpriteList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_CharacterSprites::on_leName_textChanged(QString text)
{
	if (!ignoreEvents && spriteRef != NULL)
	{
		int currentRow = lwCharacterSpriteList->currentRow();
		lwCharacterSpriteList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 4, 10, QChar('0')).arg(text));
		ProjectData::characterSpriteNames[currentRow] = text;
		(*spriteRef)->setName(text);
	}
}

void Database_CharacterSprites::on_tabWidget_currentChanged(int index)
{
	if (!ignoreEvents && spriteRef != NULL)
	{
		currentTab = index;
		poseAnimationFrame = 0;
		poseAnimationDir = 1;
		tileGridX = 16.0;
		tileGridY = 16.0;
		poseAnimationTimer->stop();
		if (currentTab == TAB_MAPSPRITE && !mapSprite_poseAnimationLocation.isEmpty())
		{
			int tilesPerSecondIndex = mapSprite_cboxPreviewMovementSpeed->currentIndex();
			int numFrames = mapSprite_poseAnimationImage.width() / 48;
			int numLoops = mapSprite_sbNumLoops->value();
			poseAnimationTimer->start((int)(1000 / (ProjectData::tilesPerSecond[tilesPerSecondIndex] * numFrames * numLoops)));
		}
		else if (currentTab == TAB_BATTLESPRITE && !battleSprite_poseAnimationLocation.isEmpty())
			poseAnimationTimer->start(1000 / battleSprite_sbFPS->value());
	}
}

void Database_CharacterSprites::on_mapSprite_sbNumLoops_valueChanged(int value)
{
	int tilesPerSecondIndex = mapSprite_cboxPreviewMovementSpeed->currentIndex();
	int numFrames = mapSprite_poseAnimationImage.width() / 48;
	(*spriteRef)->setMapPoseNumLoops(mapSprite_lwPoseList->currentRow(), value);
	if (!mapSprite_poseAnimationLocation.isEmpty())
		poseAnimationTimer->start((int)(1000 / (ProjectData::tilesPerSecond[tilesPerSecondIndex] * numFrames * value)));
}

void Database_CharacterSprites::on_mapSprite_lwPoseList_itemSelectionChanged()
{
	int poseID = mapSprite_lwPoseList->currentRow();
	if (spriteRef != NULL)
	{
		poseAnimationTimer->stop();
		mapSprite_sbNumLoops->setValue((*spriteRef)->getMapPoseNumLoops(poseID));
		mapSprite_cbPingPong->setChecked((*spriteRef)->isMapPosePingPong(poseID));
		mapSprite_lePoseName->setText((*spriteRef)->getMapPoseName(poseID));
		mapSprite_lblPoseName->setText((*spriteRef)->getMapPoseName(poseID));
		if (poseID >= 0 && poseID <= 3)
		{
			mapSprite_lePoseName->setVisible(false);
			mapSprite_lblPoseName->setVisible(true);
		}
		else
		{
			mapSprite_lePoseName->setVisible(true);
			mapSprite_lblPoseName->setVisible(false);
		}
		mapSprite_poseAnimationLocation = (*spriteRef)->getMapPoseLocation(poseID);
		if (mapSprite_poseAnimationLocation.isEmpty())
		{
			mapSprite_tileGridItem->setVisible(false);
			mapSprite_poseAnimationItem->setPixmap(QPixmap());
		}
		else
		{
			int tilesPerSecondIndex = mapSprite_cboxPreviewMovementSpeed->currentIndex();
			int numFrames, numLoops = mapSprite_sbNumLoops->value();
			mapSprite_poseAnimationImage = QImage(mapSprite_poseAnimationLocation);
			numFrames = mapSprite_poseAnimationImage.width() / 48;
			poseAnimationFrame = 0;
			poseAnimationDir = 1;
			tileGridX = 16.0;
			tileGridY = 16.0;
			tileGridMoveX = 0;
			tileGridMoveY = 0;
			if (poseID == CharacterSprite::MAPPOSE_WALKING || poseID == CharacterSprite::MAPPOSE_JUMPING)
			{
				int direction = mapSprite_cboxPreviewDirection->currentIndex();
				mapSprite_tileGridItem->setVisible(true);
				if (direction == DIR_UP)
					tileGridMoveY = -1;
				else if (direction == DIR_DOWN)
					tileGridMoveY = 1;
				else if (direction == DIR_LEFT)
					tileGridMoveX = -1;
				else if (direction == DIR_RIGHT)
					tileGridMoveX = 1;
			}
			else
				mapSprite_tileGridItem->setVisible(false);
			poseAnimationTimer->start((int)(1000 / (ProjectData::tilesPerSecond[tilesPerSecondIndex] * numFrames * numLoops)));
			renderPoseAnimation();
		}
	}
	else
	{
		mapSprite_lePoseName->setVisible(false);
		mapSprite_lblPoseName->setVisible(true);
		if (poseID == CharacterSprite::MAPPOSE_IDLE)
			mapSprite_lblPoseName->setText("Idle");
		else if (poseID == CharacterSprite::MAPPOSE_WALKING)
			mapSprite_lblPoseName->setText("Walking");
		else if (poseID == CharacterSprite::MAPPOSE_JUMPING)
			mapSprite_lblPoseName->setText("Jumping");
		else if (poseID == CharacterSprite::MAPPOSE_GRAPPLING)
			mapSprite_lblPoseName->setText("Grappling");
		else
			mapSprite_lblPoseName->setText("");
	}
}

void Database_CharacterSprites::on_mapSprite_lePoseName_textChanged(QString text)
{
	if (!ignoreEvents && spriteRef != NULL)
	{
		int currentRow = mapSprite_lwPoseList->currentRow();
		mapSprite_lwPoseList->item(currentRow)->setText(QString("%1: %2").arg(currentRow + 1, 2, 10, QChar('0')).arg(text));
		(*spriteRef)->setMapPoseName(currentRow, text);
	}
}

void Database_CharacterSprites::on_mapSprite_bSetPoseAnimation_clicked()
{
	if (spriteRef != NULL)
	{
		int poseID = mapSprite_lwPoseList->currentRow();
		ImageBrowser *imageBrowser = new ImageBrowser;
		imageBrowser->setupBrowser("/sprites", ImageBrowser::BlockALL - ImageBrowser::BlockPNG);
		if (imageBrowser->exec())
		{
			poseAnimationTimer->stop();
			mapSprite_poseAnimationLocation = imageBrowser->getFileLocation();
			(*spriteRef)->setMapPoseLocation(poseID, mapSprite_poseAnimationLocation);
			if (mapSprite_poseAnimationLocation.isEmpty())
				mapSprite_poseAnimationItem->setPixmap(QPixmap());
			else
			{
				int tilesPerSecondIndex = mapSprite_cboxPreviewMovementSpeed->currentIndex();
				int numFrames, numLoops = mapSprite_sbNumLoops->value();
				mapSprite_poseAnimationImage = QImage(mapSprite_poseAnimationLocation);
				numFrames = mapSprite_poseAnimationImage.width() / 48;
				poseAnimationFrame = 0;
				poseAnimationDir = 1;
				tileGridX = 16.0;
				tileGridY = 16.0;
				tileGridMoveX = 0;
				tileGridMoveY = 0;
				if (poseID == CharacterSprite::MAPPOSE_WALKING || poseID == CharacterSprite::MAPPOSE_JUMPING)
				{
					int direction = mapSprite_cboxPreviewDirection->currentIndex();
					mapSprite_tileGridItem->setVisible(true);
					if (direction == DIR_UP)
						tileGridMoveY = -1;
					else if (direction == DIR_DOWN)
						tileGridMoveY = 1;
					else if (direction == DIR_LEFT)
						tileGridMoveX = -1;
					else if (direction == DIR_RIGHT)
						tileGridMoveX = 1;
				}
				else
					mapSprite_tileGridItem->setVisible(false);
				poseAnimationTimer->start((int)(1000 / (ProjectData::tilesPerSecond[tilesPerSecondIndex] * numFrames * numLoops)));
				renderPoseAnimation();
			}
		}
		delete imageBrowser;
	}
}

void Database_CharacterSprites::on_mapSprite_cbPingPong_toggled(bool checked)
{
	if (!ignoreEvents && spriteRef != NULL)
	{
		int tilesPerSecondIndex = mapSprite_cboxPreviewMovementSpeed->currentIndex();
		int numFrames = mapSprite_poseAnimationImage.width() / 48;
		int numLoops = mapSprite_sbNumLoops->value();
		(*spriteRef)->setMapPosePingPong(mapSprite_lwPoseList->currentRow(), checked);
		poseAnimationTimer->stop();
		poseAnimationFrame = 0;
		poseAnimationDir = 1;
		tileGridX = 16.0;
		tileGridY = 16.0;
		if (!mapSprite_poseAnimationLocation.isEmpty())
			poseAnimationTimer->start((int)(1000 / (ProjectData::tilesPerSecond[tilesPerSecondIndex] * numFrames * numLoops)));
		renderPoseAnimation();
	}
}

void Database_CharacterSprites::on_mapSprite_bAddPose_clicked()
{
	if (!ignoreEvents && spriteRef != NULL)
	{
		QListWidgetItem *item;
		int poseID = mapSprite_lwPoseList->currentRow();
		item = new QListWidgetItem(QString("[%1]").arg(poseID + 1, 2, 10, QChar('0')));
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsDragEnabled);
		item->setTextAlignment(Qt::AlignHCenter);
		item->setData(Qt::UserRole, QVariant(poseID));
		mapSprite_lwPoseQueue->addItem(item);
	}
}

void Database_CharacterSprites::on_mapSprite_bRemovePose_clicked()
{
	int row = mapSprite_lwPoseQueue->currentRow();
	if (row >= 0 && row < mapSprite_lwPoseQueue->count())
		delete (mapSprite_lwPoseQueue->takeItem(row));
}

void Database_CharacterSprites::on_mapSprite_bMovePoseUp_clicked()
{
	int row = mapSprite_lwPoseQueue->currentRow();
	if (row > 0 && row < mapSprite_lwPoseQueue->count())
	{
		QListWidgetItem *item = mapSprite_lwPoseQueue->takeItem(row);
		--row;
		mapSprite_lwPoseQueue->insertItem(row, item);
		item->setSelected(true);
		mapSprite_lwPoseQueue->setCurrentRow(row);
	}
}

void Database_CharacterSprites::on_mapSprite_bMovePoseDown_clicked()
{
	int row = mapSprite_lwPoseQueue->currentRow();
	if (row >= 0 && row < mapSprite_lwPoseQueue->count() - 1)
	{
		QListWidgetItem *item = mapSprite_lwPoseQueue->takeItem(row);
		++row;
		mapSprite_lwPoseQueue->insertItem(row, item);
		item->setSelected(true);
		mapSprite_lwPoseQueue->setCurrentRow(row);
	}
}

void Database_CharacterSprites::on_mapSprite_bClearQueue_clicked()
{
	mapSprite_lwPoseQueue->clear();
}

void Database_CharacterSprites::on_mapSprite_bPlay_clicked()
{
	if (mapSprite_lwPoseQueue->count() > 0)
	{
		mapSprite_frmPoseInfo->setEnabled(false);
		mapSprite_frmPoseQueue->setEnabled(false);
		battleSprite_frmPoseInfo->setEnabled(false);
		battleSprite_frmPoseQueue->setEnabled(false);
		//tabWidget->setTabEnabled(TAB_MAPSPRITE, false);
		//tabWidget->setTabEnabled(TAB_BATTLESPRITE, false);
		loadPoseQueueImages();
		poseQueueFrame = 0;
		mapSprite_poseQueueItem->setPixmap(poseQueuePixmaps[poseQueueFrame]);
		poseQueueTimer->start(poseQueueTimerIntervals[poseQueueFrame]);
	}
}

void Database_CharacterSprites::on_mapSprite_cboxPreviewMovementSpeed_currentIndexChanged(int index)
{
	if (!ignoreEvents && spriteRef != NULL && !mapSprite_poseAnimationLocation.isEmpty())
	{
		int numFrames = mapSprite_poseAnimationImage.width() / 48;
		int numLoops = mapSprite_sbNumLoops->value();
		poseAnimationTimer->start((int)(1000 / (ProjectData::tilesPerSecond[index] * numFrames * numLoops)));
	}
}

void Database_CharacterSprites::on_mapSprite_cboxPreviewDirection_currentIndexChanged(int index)
{
	int tilesPerSecondIndex = mapSprite_cboxPreviewMovementSpeed->currentIndex();
	int numFrames = mapSprite_poseAnimationImage.width() / 48;
	int numLoops = mapSprite_sbNumLoops->value();
	poseAnimationTimer->stop();
	poseAnimationFrame = 0;
	poseAnimationDir = 1;
	tileGridX = 16.0;
	tileGridY = 16.0;
	tileGridMoveX = 0;
	tileGridMoveY = 0;
	if (index == DIR_UP)
		tileGridMoveY = -1;
	else if (index == DIR_DOWN)
		tileGridMoveY = 1;
	else if (index == DIR_LEFT)
		tileGridMoveX = -1;
	else if (index == DIR_RIGHT)
		tileGridMoveX = 1;
	if (!mapSprite_poseAnimationLocation.isEmpty())
		poseAnimationTimer->start((int)(1000 / (ProjectData::tilesPerSecond[tilesPerSecondIndex] * numFrames * numLoops)));
	renderPoseAnimation();
}

void Database_CharacterSprites::on_battleSprite_sbFPS_valueChanged(int value)
{
	if (!ignoreEvents && spriteRef != NULL)
	{
		(*spriteRef)->setBattlePoseFPS(battleSprite_lwPoseList->currentRow(), value);
		if (!battleSprite_poseAnimationLocation.isEmpty())
			poseAnimationTimer->setInterval(1000 / value);
	}
}

void Database_CharacterSprites::on_battleSprite_lwPoseList_itemSelectionChanged()
{
	int poseID = battleSprite_lwPoseList->currentRow();
	if (spriteRef != NULL)
	{
		poseAnimationTimer->stop();
		battleSprite_sbFPS->setValue((*spriteRef)->getBattlePoseFPS(poseID));
		battleSprite_cbPingPong->setChecked((*spriteRef)->isBattlePosePingPong(poseID));
		battleSprite_lePoseName->setText((*spriteRef)->getBattlePoseName(poseID));
		battleSprite_lblPoseName->setText((*spriteRef)->getBattlePoseName(poseID));
		if (poseID >= 0 && poseID <= 12)
		{
			battleSprite_lePoseName->setVisible(false);
			battleSprite_lblPoseName->setVisible(true);
		}
		else
		{
			battleSprite_lePoseName->setVisible(true);
			battleSprite_lblPoseName->setVisible(false);
		}
		battleSprite_poseAnimationLocation = (*spriteRef)->getBattlePoseLocation(poseID);
		if (battleSprite_poseAnimationLocation.isEmpty())
		{
			battleSprite_gvPoseAnimation->setSceneRect(0.0, 0.0, 0.0, 0.0);
			battleSprite_poseAnimationItem->setPixmap(QPixmap());
		}
		else
		{
			battleSprite_poseAnimationImage = QImage(battleSprite_poseAnimationLocation);
			poseAnimationFrame = 0;
			poseAnimationDir = 1;
			poseAnimationTimer->start(1000 / battleSprite_sbFPS->value());
			renderPoseAnimation();
		}
	}
	else
	{
		battleSprite_lePoseName->setVisible(false);
		battleSprite_lblPoseName->setVisible(true);
		if (poseID == CharacterSprite::BATTLEPOSE_IDLESTANCE)
			battleSprite_lblPoseName->setText("Idle Stance");
		else if (poseID == CharacterSprite::BATTLEPOSE_ATTACK)
			battleSprite_lblPoseName->setText("Attack");
		else if (poseID == CharacterSprite::BATTLEPOSE_SKILLUSED)
			battleSprite_lblPoseName->setText("Skill Used");
		else if (poseID == CharacterSprite::BATTLEPOSE_DIEING)
			battleSprite_lblPoseName->setText("Dieing");
		else if (poseID == CharacterSprite::BATTLEPOSE_DEAD)
			battleSprite_lblPoseName->setText("Dead");
		else if (poseID == CharacterSprite::BATTLEPOSE_DAMAGED)
			battleSprite_lblPoseName->setText("Damaged");
		else if (poseID == CharacterSprite::BATTLEPOSE_BADSTATUS)
			battleSprite_lblPoseName->setText("Bad Status");
		else if (poseID == CharacterSprite::BATTLEPOSE_DEFENDING)
			battleSprite_lblPoseName->setText("Defending");
		else if (poseID == CharacterSprite::BATTLEPOSE_RUNNINGLEFT)
			battleSprite_lblPoseName->setText("Running Left");
		else if (poseID == CharacterSprite::BATTLEPOSE_RUNNINGRIGHT)
			battleSprite_lblPoseName->setText("Running Right");
		else if (poseID == CharacterSprite::BATTLEPOSE_ITEMUSED)
			battleSprite_lblPoseName->setText("Item Used");
		else if (poseID == CharacterSprite::BATTLEPOSE_WOUNDED)
			battleSprite_lblPoseName->setText("Wounded");
		else if (poseID == CharacterSprite::BATTLEPOSE_VICTORY)
			battleSprite_lblPoseName->setText("Victory");
		else
			battleSprite_lblPoseName->setText("");
	}
}

void Database_CharacterSprites::on_battleSprite_lePoseName_textChanged(QString text)
{
	if (!ignoreEvents && spriteRef != NULL)
	{
		int currentRow = battleSprite_lwPoseList->currentRow();
		battleSprite_lwPoseList->item(currentRow)->setText(QString("%1: %2").arg(currentRow + 1, 2, 10, QChar('0')).arg(text));
		(*spriteRef)->setBattlePoseName(currentRow, text);
	}
}

void Database_CharacterSprites::on_battleSprite_bSetPoseAnimation_clicked()
{
	if (spriteRef != NULL)
	{
		int poseID = battleSprite_lwPoseList->currentRow();
		BattleSpriteImageBrowser *imageBrowser = new BattleSpriteImageBrowser("battlecharacters", battleSprite_poseAnimationLocation, (*spriteRef)->getBattlePoseNumFrames(poseID));
		if (imageBrowser->exec())
		{
			poseAnimationTimer->stop();
			battleSprite_poseAnimationLocation = imageBrowser->getFileLocation();
			(*spriteRef)->setBattlePoseLocation(poseID, battleSprite_poseAnimationLocation);
			(*spriteRef)->setBattlePoseNumFrames(poseID, imageBrowser->getNumFrames());
			if (battleSprite_poseAnimationLocation.isEmpty())
			{
				battleSprite_gvPoseAnimation->setSceneRect(0.0, 0.0, 0.0, 0.0);
				battleSprite_poseAnimationItem->setPixmap(QPixmap());
			}
			else
			{
				battleSprite_poseAnimationImage = QImage(battleSprite_poseAnimationLocation);
				poseAnimationFrame = 0;
				poseAnimationDir = 1;
				poseAnimationTimer->start(1000 / battleSprite_sbFPS->value());
				renderPoseAnimation();
			}
		}
		delete imageBrowser;
	}
}

void Database_CharacterSprites::on_battleSprite_cbPingPong_toggled(bool checked)
{
	if (!ignoreEvents && spriteRef != NULL)
	{
		(*spriteRef)->setBattlePosePingPong(battleSprite_lwPoseList->currentRow(), checked);
		poseAnimationTimer->stop();
		poseAnimationFrame = 0;
		poseAnimationDir = 1;
		if (!battleSprite_poseAnimationLocation.isEmpty())
			poseAnimationTimer->start(1000 / battleSprite_sbFPS->value());
		renderPoseAnimation();
	}
}

void Database_CharacterSprites::on_battleSprite_bAddPose_clicked()
{
	if (!ignoreEvents && spriteRef != NULL)
	{
		QListWidgetItem *item;
		int poseID = battleSprite_lwPoseList->currentRow();
		item = new QListWidgetItem(QString("[%1]").arg(poseID + 1, 2, 10, QChar('0')));
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsDragEnabled);
		item->setTextAlignment(Qt::AlignHCenter);
		item->setData(Qt::UserRole, QVariant(poseID));
		battleSprite_lwPoseQueue->addItem(item);
	}
}

void Database_CharacterSprites::on_battleSprite_bRemovePose_clicked()
{
	int row = battleSprite_lwPoseQueue->currentRow();
	if (row >= 0 && row < battleSprite_lwPoseQueue->count())
		delete (battleSprite_lwPoseQueue->takeItem(row));
}

void Database_CharacterSprites::on_battleSprite_bMovePoseUp_clicked()
{
	int row = battleSprite_lwPoseQueue->currentRow();
	if (row > 0 && row < battleSprite_lwPoseQueue->count())
	{
		QListWidgetItem *item = battleSprite_lwPoseQueue->takeItem(row);
		--row;
		battleSprite_lwPoseQueue->insertItem(row, item);
		item->setSelected(true);
		battleSprite_lwPoseQueue->setCurrentRow(row);
	}
}

void Database_CharacterSprites::on_battleSprite_bMovePoseDown_clicked()
{
	int row = battleSprite_lwPoseQueue->currentRow();
	if (row >= 0 && row < battleSprite_lwPoseQueue->count() - 1)
	{
		QListWidgetItem *item = battleSprite_lwPoseQueue->takeItem(row);
		++row;
		battleSprite_lwPoseQueue->insertItem(row, item);
		item->setSelected(true);
		battleSprite_lwPoseQueue->setCurrentRow(row);
	}
}

void Database_CharacterSprites::on_battleSprite_bClearQueue_clicked()
{
	battleSprite_lwPoseQueue->clear();
}

void Database_CharacterSprites::on_battleSprite_bPlay_clicked()
{
	if (battleSprite_lwPoseQueue->count() > 0)
	{
		float scaleWidth, scaleHeight, scale = 1.0;
		int maxWidth, maxHeight;
		mapSprite_frmPoseInfo->setEnabled(false);
		mapSprite_frmPoseQueue->setEnabled(false);
		battleSprite_frmPoseInfo->setEnabled(false);
		battleSprite_frmPoseQueue->setEnabled(false);
		tabWidget->setTabEnabled(TAB_MAPSPRITE, false);
		tabWidget->setTabEnabled(TAB_BATTLESPRITE, false);
		loadPoseQueueImages();
		maxWidth = poseQueuePixmaps[0].width();
		maxHeight = poseQueuePixmaps[0].height();
		for (int i = 1; i < poseQueuePixmaps.size(); ++i)
		{
			if (poseQueuePixmaps[i].width() > maxWidth)
				maxWidth = poseQueuePixmaps[i].width();
			if (poseQueuePixmaps[i].height() > maxHeight)
				maxHeight = poseQueuePixmaps[i].height();
		}
		if (maxWidth > 200 || maxHeight > 200)
		{
			scaleWidth = ((float)(battleSprite_gvQueuePreview->width())) / ((float)maxWidth);
			scaleHeight = ((float)(battleSprite_gvQueuePreview->height())) / ((float)maxHeight);
			if (scaleWidth < scaleHeight)
				scale = scaleWidth;
			else
				scale = scaleHeight;
		}
		battleSprite_gvQueuePreview->resetTransform();
		battleSprite_gvQueuePreview->scale(scale, scale);
		poseQueueFrame = 0;
		battleSprite_poseQueueItem->setPixmap(poseQueuePixmaps[poseQueueFrame]);
		poseQueueTimer->start(poseQueueTimerIntervals[poseQueueFrame]);
	}
}

void Database_CharacterSprites::animatePose()
{
	if (spriteRef != NULL)
	{
		int numFrames;
		bool pingPong;
		if (currentTab == TAB_MAPSPRITE)
		{
			int tilesPerSecondIndex = mapSprite_cboxPreviewMovementSpeed->currentIndex();
			float tileGridPixelsPerSecond = ProjectData::tilesPerSecond[tilesPerSecondIndex] * 32.0;
			float framesPerSecond = 1000.0 / poseAnimationTimer->interval();
			float tileGridPixelsPerFrame = tileGridPixelsPerSecond / framesPerSecond;
			numFrames = mapSprite_poseAnimationImage.width() / 48;
			pingPong = mapSprite_cbPingPong->isChecked();
			tileGridX += tileGridPixelsPerFrame * tileGridMoveX;
			tileGridY += tileGridPixelsPerFrame * tileGridMoveY;
			while (tileGridX < 0.0)
				tileGridX += 32.0;
			while (tileGridX + 64.0 > tileGridW)
				tileGridX -= 32.0;
			while (tileGridY < 0.0)
				tileGridY += 32.0;
			while (tileGridY + 96.0 > tileGridH)
				tileGridY -= 32.0;
			mapSprite_tileGridItem->setPos(-tileGridX, -tileGridY);
		}
		else
		{
			numFrames = (*spriteRef)->getBattlePoseNumFrames(battleSprite_lwPoseList->currentRow());
			pingPong = battleSprite_cbPingPong->isChecked();
		}
		if (numFrames > 1)
		{
			if (pingPong)
			{
				poseAnimationFrame += poseAnimationDir;
				if (poseAnimationFrame == 0 || poseAnimationFrame == numFrames - 1)
					poseAnimationDir *= -1;
			}
			else
				poseAnimationFrame = (poseAnimationFrame + 1) % numFrames;
		}
		renderPoseAnimation();
	}
}

void Database_CharacterSprites::animatePoseQueue()
{
	QGraphicsPixmapItem *poseQueueItem;
	if (currentTab == TAB_MAPSPRITE)
		poseQueueItem = mapSprite_poseQueueItem;
	else
	{
		battleSprite_gvQueuePreview->setSceneRect(0.0, 0.0, poseQueuePixmaps[poseQueueFrame].width(), poseQueuePixmaps[poseQueueFrame].height());
		poseQueueItem = battleSprite_poseQueueItem;
	}
	++poseQueueFrame;
	if (poseQueueFrame < poseQueuePixmaps.size())
	{
		poseQueueItem->setPixmap(QPixmap(poseQueuePixmaps[poseQueueFrame]));
		poseQueueTimer->start(poseQueueTimerIntervals[poseQueueFrame]);
	}
	else
	{
		mapSprite_frmPoseInfo->setEnabled(true);
		mapSprite_frmPoseQueue->setEnabled(true);
		battleSprite_frmPoseInfo->setEnabled(true);
		battleSprite_frmPoseQueue->setEnabled(true);
		tabWidget->setTabEnabled(TAB_MAPSPRITE, true);
		tabWidget->setTabEnabled(TAB_BATTLESPRITE, true);
		poseQueueItem->setPixmap(QPixmap());
		poseQueuePixmaps.clear();
		poseQueueTimerIntervals.clear();
	}
}

void Database_CharacterSprites::loadPoseQueueImages()
{
	QPixmap pixmap;
	if (currentTab == TAB_MAPSPRITE)
	{
		for (int i = 0; i < mapSprite_lwPoseQueue->count(); ++i)
		{
			int direction = mapSprite_cboxPreviewDirection->currentIndex();
			int poseID = mapSprite_lwPoseQueue->item(i)->data(Qt::UserRole).toInt();
			int tilesPerSecondIndex = mapSprite_cboxPreviewMovementSpeed->currentIndex();
			int numFrames, poseInterval;
			int numLoops = (*spriteRef)->getMapPoseNumLoops(poseID);
			pixmap = QPixmap((*spriteRef)->getMapPoseLocation(poseID));
			numFrames = pixmap.width() / 48;
			poseInterval = (int)(1000 / (ProjectData::tilesPerSecond[tilesPerSecondIndex] * numFrames * numLoops));
			for (int i = 0; i < numLoops; ++i)
			{
				for (int frame = 0; frame < numFrames; ++frame)
				{
					if (pixmap.height() == 64)
						poseQueuePixmaps.append(pixmap.copy(frame * 48, 0, 48, 64));
					else
						poseQueuePixmaps.append(pixmap.copy(frame * 48, 64 * direction, 48, 64));
					poseQueueTimerIntervals.append(poseInterval);
				}
				if ((*spriteRef)->isMapPosePingPong(poseID))
				{
					for (int frame = numFrames - 2; frame >= 0; --frame)
					{
						if (pixmap.height() == 64)
							poseQueuePixmaps.append(pixmap.copy(frame * 48, 0, 48, 64));
						else
							poseQueuePixmaps.append(pixmap.copy(frame * 48, 64 * direction, 48, 64));
						poseQueueTimerIntervals.append(poseInterval);
					}
				}
			}
		}
	}
	else if (currentTab == TAB_BATTLESPRITE)
	{
		for (int i = 0; i < battleSprite_lwPoseQueue->count(); ++i)
		{
			int poseID = battleSprite_lwPoseQueue->item(i)->data(Qt::UserRole).toInt();
			int poseFPS = (*spriteRef)->getBattlePoseFPS(poseID);
			int numFrames = (*spriteRef)->getBattlePoseNumFrames(poseID);
			int frameWidth;
			pixmap = QPixmap((*spriteRef)->getBattlePoseLocation(poseID));
			frameWidth = pixmap.width() / numFrames;
			for (int frame = 0; frame < numFrames; ++frame)
			{
				poseQueuePixmaps.append(pixmap.copy(frame * frameWidth, 0, frameWidth, pixmap.height()));
				poseQueueTimerIntervals.append(1000 / poseFPS);
			}
			if ((*spriteRef)->isBattlePosePingPong(poseID))
			{
				for (int frame = numFrames - 2; frame >= 0; --frame)
				{
					poseQueuePixmaps.append(pixmap.copy(frame * frameWidth, 0, frameWidth, pixmap.height()));
					poseQueueTimerIntervals.append(1000 / poseFPS);
				}
			}
		}
	}
}

void Database_CharacterSprites::renderPoseAnimation()
{
	if (spriteRef != NULL)
	{
		if (currentTab == TAB_MAPSPRITE && !mapSprite_poseAnimationLocation.isEmpty())
		{
			int direction = mapSprite_cboxPreviewDirection->currentIndex();
			mapSprite_tileGridItem->setPos(-tileGridX, -tileGridY);
			if (mapSprite_poseAnimationImage.height() == 64)
				mapSprite_poseAnimationItem->setPixmap(QPixmap::fromImage(mapSprite_poseAnimationImage.copy(poseAnimationFrame * 48, 0, 48, 64)));
			else
				mapSprite_poseAnimationItem->setPixmap(QPixmap::fromImage(mapSprite_poseAnimationImage.copy(poseAnimationFrame * 48, 64 * direction, 48, 64)));
		}
		else if (currentTab == TAB_BATTLESPRITE && !battleSprite_poseAnimationLocation.isEmpty())
		{
			int poseID = battleSprite_lwPoseList->currentRow();
			int numFrames = (*spriteRef)->getBattlePoseNumFrames(poseID);
			int frameWidth = battleSprite_poseAnimationImage.width() / numFrames;
			int frameHeight = battleSprite_poseAnimationImage.height();
			battleSprite_gvPoseAnimation->setSceneRect(0.0, 0.0, frameWidth, frameHeight);
			battleSprite_gvPoseAnimation->resetTransform();
			if (frameWidth > 200 || frameHeight > 200)
				battleSprite_gvPoseAnimation->fitInView(0.0, 0.0, frameWidth, frameHeight, Qt::KeepAspectRatio);
			battleSprite_poseAnimationItem->setPixmap(QPixmap::fromImage(battleSprite_poseAnimationImage.copy(poseAnimationFrame * frameWidth, 0, frameWidth, frameHeight)));
		}
	}
}

void Database_CharacterSprites::updateCharacterSpriteList()
{
	QListWidgetItem *item;
	int i, numCharacterSprites = ProjectData::characterSpriteNames.size();
	int currentRow = lwCharacterSpriteList->currentRow();
	lwCharacterSpriteList->clear();
	for (i = 0; i < numCharacterSprites; ++i)
	{
		item = new QListWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::characterSpriteNames[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		lwCharacterSpriteList->addItem(item);
	}
	if (currentRow >= lwCharacterSpriteList->count())
		currentRow = lwCharacterSpriteList->count() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::characterSpriteNames.size() > 0)
	{
		lwCharacterSpriteList->setCurrentRow(currentRow);
		lwCharacterSpriteList->item(currentRow)->setSelected(true);
	}
}

void Database_CharacterSprites::updatePoseLists()
{
	for (int i = 0; i < 32; ++i)
	{
		mapSprite_lwPoseList->item(i)->setText(QString("%1: %2").arg(i + 1, 2, 10, QChar('0')).arg((*spriteRef)->getMapPoseName(i)));
		battleSprite_lwPoseList->item(i)->setText(QString("%1: %2").arg(i + 1, 2, 10, QChar('0')).arg((*spriteRef)->getBattlePoseName(i)));
	}
}
