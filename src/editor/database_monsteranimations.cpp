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
#include "database_monsteranimations.h"
#include "projectdata.h"
#include "projectdata_monsteranimation.h"
#include "xmlParser.h"

Database_MonsterAnimations::Database_MonsterAnimations(QWidget *parent) : QWidget(parent)
{
	QGraphicsScene *scene;
	setupUi(this);
	poseAnimationTimer = new QTimer(this);
	poseQueueTimer = new QTimer(this);
	poseQueueTimer->setSingleShot(true);
	QObject::connect(poseAnimationTimer, SIGNAL(timeout()), this, SLOT(animatePose()));
	QObject::connect(poseQueueTimer, SIGNAL(timeout()), this, SLOT(animatePoseQueue()));
	scene = new QGraphicsScene(0.0, 0.0, 0.0, 0.0);
	scene->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	gvPoseAnimation->setScene(scene);
	gvPoseAnimation->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	poseAnimationItem = new QGraphicsPixmapItem;
	poseAnimationItem->setPos(0.0, 0.0);
	scene->addItem(poseAnimationItem);
	scene = new QGraphicsScene(0.0, 0.0, 0.0, 0.0);
	scene->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	gvQueuePreview->setScene(scene);
	gvPoseAnimation->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	poseQueueItem = new QGraphicsPixmapItem;
	scene->addItem(poseQueueItem);
	bFPSSideButton->setSpinBox(sbFPS);
	poseAnimationLocation = "";
	animRef = NULL;
	poseAnimationFrame = 0;
	poseAnimationDir = 1;
	ignoreEvents = true;
}

Database_MonsterAnimations::~Database_MonsterAnimations()
{
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

void Database_MonsterAnimations::setupMonsterAnimations()
{
	updateMonsterAnimationList();
	lwPoseList->setCurrentRow(0);
	lwPoseList->item(0)->setSelected(true);
	ignoreEvents = false;
}

void Database_MonsterAnimations::shutdownMonsterAnimations()
{
	ignoreEvents = true;
	poseAnimationTimer->stop();
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

void Database_MonsterAnimations::on_lwMonsterAnimationList_itemSelectionChanged()
{
	int animID = lwMonsterAnimationList->currentRow();
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
	if (animID >= 0)
	{
		int poseID = lwPoseList->currentRow();
		animRef = new ProjectData::DataReference<MonsterAnimation>(animID, __FILE__, __LINE__);
		ignoreEvents = true;
		leName->setText((*animRef)->getName());
		sbFPS->setValue((*animRef)->getPoseFPS(poseID));
		cbPingPong->setChecked((*animRef)->isPosePingPong(poseID));
		updatePoseList();
		ignoreEvents = false;
		poseAnimationLocation = (*animRef)->getPoseLocation(poseID);
		if (poseAnimationLocation.isEmpty())
		{
			gvPoseAnimation->setSceneRect(0.0, 0.0, 0.0, 0.0);
			poseAnimationItem->setPixmap(QPixmap());
		}
		else
		{
			poseAnimationPixmap = QPixmap(poseAnimationLocation);
			poseAnimationFrame = 0;
			poseAnimationDir = 1;
			poseAnimationTimer->start(1000 / sbFPS->value());
			renderPoseAnimation();
		}
	}
}

void Database_MonsterAnimations::on_bArraySize_clicked()
{
	int numMonsterAnimations = ProjectData::monsterAnimationNames.size();
	ArraySize *arraysize = new ArraySize(numMonsterAnimations, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numMonsterAnimations)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (animRef != NULL)
				{
					if (newSize <= animRef->getDataID())
					{
						delete animRef;
						animRef = NULL;
					}
				}
				for (int i = numMonsterAnimations - 1; i >= newSize; --i)
					ProjectData::deleteMonsterAnimation(i);
			}
		}
		else
		{
			for (int i = numMonsterAnimations; i < newSize; ++i)
			{
				MonsterAnimation *monsterAnimation = new MonsterAnimation;
				monsterAnimation->setName(QString("Mon%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addMonsterAnimation(monsterAnimation);
				delete monsterAnimation;
			}
		}
		ignoreEvents = true;
		updateMonsterAnimationList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_MonsterAnimations::on_leName_textChanged(QString text)
{
	if (!ignoreEvents && animRef != NULL)
	{
		int currentRow = lwMonsterAnimationList->currentRow();
		lwMonsterAnimationList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 4, 10, QChar('0')).arg(text));
		ProjectData::monsterAnimationNames[currentRow] = text;
		(*animRef)->setName(text);
	}
}

void Database_MonsterAnimations::on_sbFPS_valueChanged(int value)
{
	if (!ignoreEvents && animRef != NULL)
	{
		int currentPose = lwPoseList->currentRow();
		(*animRef)->setPoseFPS(currentPose, value);
		poseAnimationTimer->setInterval(1000 / value);
	}
}

void Database_MonsterAnimations::on_lwPoseList_itemSelectionChanged()
{
	int poseID = lwPoseList->currentRow();
	if (animRef != NULL)
	{
		poseAnimationTimer->stop();
		sbFPS->setValue((*animRef)->getPoseFPS(poseID));
		cbPingPong->setChecked((*animRef)->isPosePingPong(poseID));
		lePoseName->setText((*animRef)->getPoseName(poseID));
		lblPoseName->setText((*animRef)->getPoseName(poseID));
		if (poseID >= 0 && poseID <= 12)
		{
			lePoseName->setVisible(false);
			lblPoseName->setVisible(true);
		}
		else
		{
			lePoseName->setVisible(true);
			lblPoseName->setVisible(false);
		}
		poseAnimationLocation = (*animRef)->getPoseLocation(poseID);
		if (poseAnimationLocation.isEmpty())
		{
			gvPoseAnimation->setSceneRect(0.0, 0.0, 0.0, 0.0);
			poseAnimationItem->setPixmap(QPixmap());
		}
		else
		{
			poseAnimationPixmap = QPixmap(poseAnimationLocation);
			poseAnimationFrame = 0;
			poseAnimationDir = 1;
			poseAnimationTimer->start(1000 / sbFPS->value());
			renderPoseAnimation();
		}
	}
	else
	{
		lePoseName->setVisible(false);
		lblPoseName->setVisible(true);
		if (poseID == MonsterAnimation::POSE_IDLESTANCE)
			lblPoseName->setText("Idle Stance");
		else if (poseID == MonsterAnimation::POSE_ATTACK)
			lblPoseName->setText("Attack");
		else if (poseID == MonsterAnimation::POSE_SKILLUSED)
			lblPoseName->setText("Skill Used");
		else if (poseID == MonsterAnimation::POSE_DIEING)
			lblPoseName->setText("Dieing");
		else if (poseID == MonsterAnimation::POSE_DEAD)
			lblPoseName->setText("Dead");
		else if (poseID == MonsterAnimation::POSE_DAMAGED)
			lblPoseName->setText("Damaged");
		else if (poseID == MonsterAnimation::POSE_BADSTATUS)
			lblPoseName->setText("Bad Status");
		else if (poseID == MonsterAnimation::POSE_DEFENDING)
			lblPoseName->setText("Defending");
		else if (poseID == MonsterAnimation::POSE_RUNNINGLEFT)
			lblPoseName->setText("Running Left");
		else if (poseID == MonsterAnimation::POSE_RUNNINGRIGHT)
			lblPoseName->setText("Running Right");
		else if (poseID == MonsterAnimation::POSE_ITEMUSED)
			lblPoseName->setText("Item Used");
		else if (poseID == MonsterAnimation::POSE_WOUNDED)
			lblPoseName->setText("Wounded");
		else if (poseID == MonsterAnimation::POSE_VICTORY)
			lblPoseName->setText("Victory");
		else
			lblPoseName->setText("");
	}
}

void Database_MonsterAnimations::on_lePoseName_textChanged(QString text)
{
	if (!ignoreEvents && animRef != NULL)
	{
		int currentRow = lwPoseList->currentRow();
		lwPoseList->item(currentRow)->setText(QString("%1: %2").arg(currentRow + 1, 2, 10, QChar('0')).arg(text));
		(*animRef)->setPoseName(currentRow, text);
	}
}

void Database_MonsterAnimations::on_bSetPoseAnimation_clicked()
{
	if (animRef != NULL)
	{
		int poseID = lwPoseList->currentRow();
		BattleSpriteImageBrowser *imageBrowser = new BattleSpriteImageBrowser("monsters", poseAnimationLocation, (*animRef)->getPoseNumFrames(poseID));
		if (imageBrowser->exec())
		{
			poseAnimationTimer->stop();
			poseAnimationLocation = imageBrowser->getFileLocation();
			(*animRef)->setPoseLocation(poseID, poseAnimationLocation);
			(*animRef)->setPoseNumFrames(poseID, imageBrowser->getNumFrames());
			if (poseAnimationLocation.isEmpty())
			{
				gvPoseAnimation->setSceneRect(0.0, 0.0, 0.0, 0.0);
				poseAnimationItem->setPixmap(QPixmap());
			}
			else
			{
				poseAnimationPixmap = QPixmap(poseAnimationLocation);
				poseAnimationFrame = 0;
				poseAnimationDir = 1;
				poseAnimationTimer->start(1000 / sbFPS->value());
				renderPoseAnimation();
			}
		}
		delete imageBrowser;
	}
}

void Database_MonsterAnimations::on_cbPingPong_toggled(bool checked)
{
	if (!ignoreEvents && animRef != NULL)
	{
		(*animRef)->setPosePingPong(lwPoseList->currentRow(), checked);
		poseAnimationTimer->stop();
		poseAnimationFrame = 0;
		poseAnimationDir = 1;
		poseAnimationTimer->start(1000 / sbFPS->value());
		renderPoseAnimation();
	}
}

void Database_MonsterAnimations::on_bAddPose_clicked()
{
	if (!ignoreEvents && animRef != NULL)
	{
		QListWidgetItem *item;
		int poseID = lwPoseList->currentRow();
		item = new QListWidgetItem(QString("[%1]").arg(poseID + 1, 2, 10, QChar('0')));
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsDragEnabled);
		item->setTextAlignment(Qt::AlignHCenter);
		item->setData(Qt::UserRole, QVariant(poseID));
		lwPoseQueue->addItem(item);
	}
}

void Database_MonsterAnimations::on_bRemovePose_clicked()
{
	int row = lwPoseQueue->currentRow();
	if (row >= 0 && row < lwPoseQueue->count())
		delete (lwPoseQueue->takeItem(row));
}

void Database_MonsterAnimations::on_bMovePoseUp_clicked()
{
	int row = lwPoseQueue->currentRow();
	if (row > 0 && row < lwPoseQueue->count())
	{
		QListWidgetItem *item = lwPoseQueue->takeItem(row);
		--row;
		lwPoseQueue->insertItem(row, item);
		item->setSelected(true);
		lwPoseQueue->setCurrentRow(row);
	}
}

void Database_MonsterAnimations::on_bMovePoseDown_clicked()
{
	int row = lwPoseQueue->currentRow();
	if (row >= 0 && row < lwPoseQueue->count() - 1)
	{
		QListWidgetItem *item = lwPoseQueue->takeItem(row);
		++row;
		lwPoseQueue->insertItem(row, item);
		item->setSelected(true);
		lwPoseQueue->setCurrentRow(row);
	}
}

void Database_MonsterAnimations::on_bClearQueue_clicked()
{
	lwPoseQueue->clear();
}

void Database_MonsterAnimations::on_bPlay_clicked()
{
	if (lwPoseQueue->count() > 0)
	{
		float scaleWidth, scaleHeight, scale = 1.0;
		int maxWidth = gvQueuePreview->width();
		int maxHeight = gvQueuePreview->height();
		int scaledWidth, scaledHeight;
		frmMonsterAnimations->setEnabled(false);
		frmName->setEnabled(false);
		frmPoseInfo->setEnabled(false);
		frmQueue->setEnabled(false);
		loadPoseQueueImages();
		scaleWidth = ((float)maxWidth) / ((float)poseQueuePixmaps[0].width());
		scaleHeight = ((float)maxHeight) / ((float)poseQueuePixmaps[0].height());
		scaledWidth = (int)(poseQueuePixmaps[0].width() * scaleWidth);
		scaledHeight = (int)(poseQueuePixmaps[0].height() * scaleWidth);
		if (scaledWidth > maxWidth || scaledHeight > maxHeight)
			scale = scaleHeight;
		else
			scale = scaleWidth;
		gvQueuePreview->setSceneRect(0, 0, poseQueuePixmaps[0].width(), poseQueuePixmaps[0].height());
		gvQueuePreview->resetMatrix();
		gvQueuePreview->scale(scale, scale);
		poseQueueFrame = 0;
		poseQueueItem->setPixmap(QPixmap(poseQueuePixmaps[poseQueueFrame]));
		poseQueueTimer->start(poseQueueTimerIntervals[poseQueueFrame]);
	}
}

void Database_MonsterAnimations::animatePose()
{
	if (animRef != NULL)
	{
		int poseID = lwPoseList->currentRow();
		int numFrames = (*animRef)->getPoseNumFrames(poseID);
		if (numFrames > 1)
		{
			if (cbPingPong->isChecked())
			{
				poseAnimationFrame += poseAnimationDir;
				if (poseAnimationFrame == 0 || poseAnimationFrame == numFrames - 1)
					poseAnimationDir *= -1;
			}
			else
				poseAnimationFrame = (poseAnimationFrame + 1) % numFrames;
			renderPoseAnimation();
		}
	}
}

void Database_MonsterAnimations::animatePoseQueue()
{
	++poseQueueFrame;
	if (poseQueueFrame < poseQueuePixmaps.size())
	{
		poseQueueItem->setPixmap(QPixmap(poseQueuePixmaps[poseQueueFrame]));
		poseQueueTimer->start(poseQueueTimerIntervals[poseQueueFrame]);
	}
	else
	{
		frmMonsterAnimations->setEnabled(true);
		frmName->setEnabled(true);
		frmPoseInfo->setEnabled(true);
		frmQueue->setEnabled(true);
		poseQueueItem->setPixmap(QPixmap());
		poseQueuePixmaps.clear();
		poseQueueTimerIntervals.clear();
	}
}

void Database_MonsterAnimations::loadPoseQueueImages()
{
	QPixmap pixmap;
	for (int i = 0; i < lwPoseQueue->count(); ++i)
	{
		int poseID = lwPoseQueue->item(i)->data(Qt::UserRole).toInt();
		int poseFPS = (*animRef)->getPoseFPS(poseID);
		int numFrames = (*animRef)->getPoseNumFrames(poseID);
		int frameWidth;
		pixmap = QPixmap((*animRef)->getPoseLocation(poseID));
		frameWidth = pixmap.width() / numFrames;
		for (int frame = 0; frame < numFrames; ++frame)
		{
			poseQueuePixmaps.append(pixmap.copy(frame * frameWidth, 0, frameWidth, pixmap.height()));
			poseQueueTimerIntervals.append(1000 / poseFPS);
		}
		if ((*animRef)->isPosePingPong(poseID))
		{
			for (int frame = numFrames - 2; frame >= 0; --frame)
			{
				poseQueuePixmaps.append(pixmap.copy(frame * frameWidth, 0, frameWidth, pixmap.height()));
				poseQueueTimerIntervals.append(1000 / poseFPS);
			}
		}
	}
}

void Database_MonsterAnimations::renderPoseAnimation()
{
	if (animRef != NULL)
	{
		QPixmap scaledPixmap;
		int poseID = lwPoseList->currentRow();
		int numFrames = (*animRef)->getPoseNumFrames(poseID);
		int frameSize = poseAnimationPixmap.width() / numFrames;
		scaledPixmap = poseAnimationPixmap.copy(poseAnimationFrame * frameSize, 0, frameSize, poseAnimationPixmap.height()).scaled(200, 200, Qt::KeepAspectRatio);
		gvPoseAnimation->setSceneRect(0.0, 0.0, scaledPixmap.width(), scaledPixmap.height());
		poseAnimationItem->setPixmap(scaledPixmap);
	}
}

void Database_MonsterAnimations::updateMonsterAnimationList()
{
	QListWidgetItem *item;
	int i, numMonsterAnimations = ProjectData::monsterAnimationNames.size();
	int currentRow = lwMonsterAnimationList->currentRow();
	lwMonsterAnimationList->clear();
	for (i = 0; i < numMonsterAnimations; ++i)
	{
		item = new QListWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::monsterAnimationNames[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		lwMonsterAnimationList->addItem(item);
	}
	if (currentRow >= lwMonsterAnimationList->count())
		currentRow = lwMonsterAnimationList->count() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::monsterAnimationNames.size() > 0)
	{
		lwMonsterAnimationList->setCurrentRow(currentRow);
		lwMonsterAnimationList->item(currentRow)->setSelected(true);
	}
}

void Database_MonsterAnimations::updatePoseList()
{
	for (int i = 11; i < 32; ++i)
		lwPoseList->item(i)->setText(QString("%1: %2").arg(i + 1, 2, 10, QChar('0')).arg((*animRef)->getPoseName(i)));
}
