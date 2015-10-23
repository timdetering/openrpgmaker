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

#ifndef DATABASE_CHARACTERSPRITES_H
#define DATABASE_CHARACTERSPRITES_H

class CharacterSprite;

#include "ui_database_charactersprites.h"
#include "projectdata.h"

class Database_CharacterSprites : public QWidget, public Ui::Database_CharacterSprites
{
	Q_OBJECT
	public:
		enum CurrentTab {TAB_MAPSPRITE=0, TAB_BATTLESPRITE};
		enum Direction {DIR_UP=0, DIR_RIGHT, DIR_DOWN, DIR_LEFT};
		Database_CharacterSprites(QWidget *parent=0);
		~Database_CharacterSprites();
		void setupCharacterSprites();
		void shutdownCharacterSprites();
	protected slots:
		void on_lwCharacterSpriteList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_tabWidget_currentChanged(int index);
		void on_mapSprite_sbNumLoops_valueChanged(int value);
		void on_mapSprite_lwPoseList_itemSelectionChanged();
		void on_mapSprite_lePoseName_textChanged(QString text);
		void on_mapSprite_bSetPoseAnimation_clicked();
		void on_mapSprite_cbPingPong_toggled(bool checked);
		void on_mapSprite_bAddPose_clicked();
		void on_mapSprite_bRemovePose_clicked();
		void on_mapSprite_bMovePoseUp_clicked();
		void on_mapSprite_bMovePoseDown_clicked();
		void on_mapSprite_bClearQueue_clicked();
		void on_mapSprite_bPlay_clicked();
		void on_mapSprite_cboxPreviewMovementSpeed_currentIndexChanged(int index);
		void on_mapSprite_cboxPreviewDirection_currentIndexChanged(int index);
		void on_battleSprite_sbFPS_valueChanged(int value);
		void on_battleSprite_lwPoseList_itemSelectionChanged();
		void on_battleSprite_lePoseName_textChanged(QString text);
		void on_battleSprite_bSetPoseAnimation_clicked();
		void on_battleSprite_cbPingPong_toggled(bool checked);
		void on_battleSprite_bAddPose_clicked();
		void on_battleSprite_bRemovePose_clicked();
		void on_battleSprite_bMovePoseUp_clicked();
		void on_battleSprite_bMovePoseDown_clicked();
		void on_battleSprite_bClearQueue_clicked();
		void on_battleSprite_bPlay_clicked();
		void animatePose();
		void animatePoseQueue();
	private:
		void loadPoseQueueImages();
		void renderPoseAnimation();
		void updateCharacterSpriteList();
		void updatePoseLists();
		QGraphicsPixmapItem *mapSprite_poseAnimationItem;
		QGraphicsPixmapItem *mapSprite_poseQueueItem;
		QGraphicsPixmapItem *mapSprite_tileGridItem;
		QGraphicsPixmapItem *battleSprite_poseAnimationItem;
		QGraphicsPixmapItem *battleSprite_poseQueueItem;
		QTimer *poseAnimationTimer;
		QTimer *poseQueueTimer;
		QImage mapSprite_poseAnimationImage;
		QImage battleSprite_poseAnimationImage;
		QString mapSprite_poseAnimationLocation;
		QString battleSprite_poseAnimationLocation;
		ProjectData::DataReference<CharacterSprite> *spriteRef;
		QList<QPixmap> poseQueuePixmaps;
		QList<int> poseQueueTimerIntervals;
		int poseAnimationFrame;
		int poseAnimationDir;
		int poseQueueFrame;
		int currentTab;
		float tileGridX, tileGridY;
		int tileGridW, tileGridH;
		int tileGridMoveX, tileGridMoveY;
		bool ignoreEvents;
		TODO("Add tile grid behind character sprite for better animation preview.");
};

#endif // DATABASE_CHARACTERSPRITES_H
