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

#ifndef DATABASE_BATTLEANIMATIONS_H
#define DATABASE_BATTLEANIMATIONS_H

#include "ui_database_battleanimations.h"
#include "battleanimationpreview.h"
#include "mygraphicstinteffect.h"
#include "projectdata.h"
#include "projectdata_battleanimation.h"

class Database_BattleAnimations : public QWidget, public Ui::Database_BattleAnimations
{
	Q_OBJECT
	public:
		enum Zoom {ZOOM_25=0, ZOOM_50, ZOOM_75, ZOOM_100, ZOOM_FITTOWINDOW};
		enum GridSize {SIZE_2x2=0, SIZE_4x4, SIZE_8x8, SIZE_16x16, SIZE_32x32, SIZE_64x64};
		Database_BattleAnimations(QWidget *parent=0);
		~Database_BattleAnimations();
		void setupBattleAnimations();
		void shutdownBattleAnimations();
	protected slots:
		void on_lwAnimationList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_gvAnimation_mousePressEvent(QMouseEvent *event);
		void on_gvAnimation_mouseDoubleClickEvent(QMouseEvent *event);
		void on_gvAnimation_mouseMoveEvent(QMouseEvent *event);
		void on_gvAnimation_mouseReleaseEvent(QMouseEvent *event);
		void on_gvAnimation_dragEnterEvent(QDragEnterEvent *event);
		void on_gvAnimation_dropEvent(QDropEvent *event);
		void on_gvAnimation_customContextMenuRequested(QPoint pos);
		void on_gvAnimationImages_mousePressEvent(QMouseEvent *event);
		void on_gvAnimationImages_mouseMoveEvent(QMouseEvent *event);
		void on_gvAnimationImages_hScrollBarRangeChanged(int min, int max);
		void on_gvAnimationImages_hScrollBarValueChanged(int value);
		void on_bRemoveImage_clicked();
		void on_bAddImage_clicked();
		void on_leNumFrames_textChanged(QString text);
		void on_bSetNumFrames_clicked();
		void on_lwFrameList_itemSelectionChanged();
		void on_bClonePrevious_clicked();
		void on_bMassCopyClear_clicked();
		void on_bAutocomplete_clicked();
		void on_bMassModify_clicked();
		void on_bPlay_clicked();
		void on_leName_textChanged(QString text);
		void on_cboxTarget_currentIndexChanged(int index);
		void on_cboxXAxisLine_currentIndexChanged();
		void on_cboxYAxisLine_currentIndexChanged();
		void on_cbShowGrid_toggled(bool on);
		void on_cbSnapToGrid_toggled(bool on);
		void on_bBack_clicked();
		void on_bNext_clicked();
		void on_bDeleteEffect_clicked();
		void on_bAddEffect_clicked();
		void on_bConfigureEffect_clicked();
		void on_cboxZoom_currentIndexChanged(int index);
		void on_twAdditionalEffects_itemSelectionChanged();
		void on_aCellProperties_triggered();
		void on_aCopy_triggered();
		void on_aCut_triggered();
		void on_aPaste_triggered();
		void on_aDelete_triggered();
		void on_aMoveBack_triggered();
		void on_aMoveForward_triggered();
		void on_aMoveToBack_triggered();
		void on_aMoveToFront_triggered();
		void on_sbFrameRate_valueChanged(int value);
	private:
		void updateAnimationList();
		void updateTargetList();
		void updateEffectList();
		void updateFrame();
		int getItemUnderCursor(QPoint scenePos, bool cycleSelection);
		BattleAnimationPreview *previewWidget;
		QGraphicsScene *animationScene;
		QGraphicsScene *imagesScene;
		QGraphicsPixmapItem *animationImageItems[16];
		MyGraphicsTintEffect *animationImageTintEffects[16];
		QGraphicsRectItem *animationImageItemFrames[16];
		QGraphicsEllipseItem *animationImageItemFrameNumberBackground[16];
		QGraphicsPixmapItem *animationImageItemFrameNumber[16];
		QGraphicsRectItem *animationPreviousFrames[16];
		QGraphicsEllipseItem *animationPreviousFrameNumberBackground[16];
		QGraphicsPixmapItem *animationPreviousFrameNumber[16];
		QGraphicsPixmapItem *animationTargetItem;
		MyGraphicsTintEffect *animationTargetTintEffect;
		QGraphicsLineItem *xAxisLine1;
		QGraphicsLineItem *xAxisLine2;
		QGraphicsLineItem *yAxisLine1;
		QGraphicsLineItem *yAxisLine2;
		QList<QGraphicsPixmapItem*> imageItems;
		QList<QGraphicsPixmapItem*> imageItemGrids;
		QList<int> effectIDs;
		QPixmap targetPixmap;
		QImage animationImageFrame_numbers;
		ProjectData::DataReference<BattleAnimation> *animRef;
		BattleAnimation::AnimationFrame *currentFrame;
		QPoint dragStartPos;
		QPoint hotSpot;
		QPoint contextMenuScenePos;
		QPen animationFramePen;
		QPen animationFramePen_selected;
		QPen animationFramePen_old;
		QBrush animationFrameBrush;
		QBrush animationFrameBrush_selected;
		QBrush animationFrameBrush_old;
		float currentViewScale;
		int currentAnimationImage;
		int currentImageItem;
		int numImagesInFrame;
		bool ignoreEvents;
};

#endif // DATABASE_BATTLEANIMATIONS_H
