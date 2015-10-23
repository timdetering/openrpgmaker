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

#ifndef DATABASE_MONSTERANIMATIONS_H
#define DATABASE_MONSTERANIMATIONS_H

class MonsterAnimation;

#include "ui_database_monsteranimations.h"
#include "projectdata.h"

class Database_MonsterAnimations : public QWidget, public Ui::Database_MonsterAnimations
{
	Q_OBJECT
	public:
		Database_MonsterAnimations(QWidget *parent=0);
		~Database_MonsterAnimations();
		void setupMonsterAnimations();
		void shutdownMonsterAnimations();
	protected slots:
		void on_lwMonsterAnimationList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_sbFPS_valueChanged(int value);
		void on_lwPoseList_itemSelectionChanged();
		void on_lePoseName_textChanged(QString text);
		void on_bSetPoseAnimation_clicked();
		void on_cbPingPong_toggled(bool checked);
		void on_bAddPose_clicked();
		void on_bRemovePose_clicked();
		void on_bMovePoseUp_clicked();
		void on_bMovePoseDown_clicked();
		void on_bClearQueue_clicked();
		void on_bPlay_clicked();
		void animatePose();
		void animatePoseQueue();
	private:
		void loadPoseQueueImages();
		void renderPoseAnimation();
		void updateMonsterAnimationList();
		void updatePoseList();
		QGraphicsPixmapItem *poseAnimationItem;
		QGraphicsPixmapItem *poseQueueItem;
		QTimer *poseAnimationTimer;
		QTimer *poseQueueTimer;
		QPixmap poseAnimationPixmap;
		QString poseAnimationLocation;
		ProjectData::DataReference<MonsterAnimation> *animRef;
		QList<QPixmap> poseQueuePixmaps;
		QList<int> poseQueueTimerIntervals;
		int poseAnimationFrame;
		int poseAnimationDir;
		int poseQueueFrame;
		bool ignoreEvents;
};

#endif // DATABASE_MONSTERANIMATIONS_H
