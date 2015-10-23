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

#ifndef DATABASE_GLOBALANIMATIONS_H
#define DATABASE_GLOBALANIMATIONS_H

#include "ui_database_globalanimations.h"
#include "globalanimationpreview.h"
#include "mygraphicstinteffect.h"
#include "projectdata.h"
#include "projectdata_globalanimation.h"

class Database_GlobalAnimations : public QWidget, public Ui::Database_GlobalAnimations
{
	Q_OBJECT
	public:
		enum Zoom {ZOOM_25=0, ZOOM_50, ZOOM_75, ZOOM_100, ZOOM_FITTOWINDOW};
		enum GridSize {SIZE_2x2=0, SIZE_4x4, SIZE_8x8, SIZE_16x16, SIZE_32x32, SIZE_64x64};
		Database_GlobalAnimations(QWidget *parent=0);
		~Database_GlobalAnimations();
		void setupGlobalAnimations();
		void shutdownGlobalAnimations();
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
		void on_bBrowseBGImage_clicked();
		void on_cbShowBGImage_toggled(bool on);
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
		void updateEffectList();
		void updateFrame();
		int getItemUnderCursor(QPoint scenePos, bool cycleSelection);
		GlobalAnimationPreview *previewWidget;
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
		QGraphicsPixmapItem *animationBGItem;
		QGraphicsRectItem *screenQuadrant1Rect1;
		QGraphicsRectItem *screenQuadrant1Rect2;
		QGraphicsRectItem *screenQuadrant2Rect1;
		QGraphicsRectItem *screenQuadrant2Rect2;
		QGraphicsRectItem *screenQuadrant3Rect1;
		QGraphicsRectItem *screenQuadrant3Rect2;
		QGraphicsRectItem *screenQuadrant4Rect1;
		QGraphicsRectItem *screenQuadrant4Rect2;
		QGraphicsRectItem *blackBackground;
		QList<QGraphicsPixmapItem*> imageItems;
		QList<QGraphicsPixmapItem*> imageItemGrids;
		QList<int> effectIDs;
		QImage animationImageFrame_numbers;
		ProjectData::DataReference<GlobalAnimation> *animRef;
		GlobalAnimation::AnimationFrame *currentFrame;
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

#endif // DATABASE_GLOBALANIMATIONS_H
