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
*  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
****************************************************************************/

#ifndef SCREENLAYOUTS_TITLEMENU_H
#define SCREENLAYOUTS_TITLEMENU_H

class TitleMenuLayout;

#include "ui_screenlayouts_titlemenu.h"
#include "projectdata.h"

class ScreenLayouts_TitleMenu : public QWidget, public Ui::ScreenLayouts_TitleMenu
{
	Q_OBJECT
	public:
		enum Zoom {ZOOM_25=0, ZOOM_50, ZOOM_75, ZOOM_100, ZOOM_150, ZOOM_200, ZOOM_FITTOWINDOW};
		enum EditEvent {EVENT_ONLOAD=0, EVENT_ONLEAVE, EVENT_ACCEPT, EVENT_CANCEL, EVENT_UPARROW, EVENT_DOWNARROW, EVENT_LEFTARROW, EVENT_RIGHTARROW};
		enum ResizeHandle
		{
			RESIZE_TOPLEFT=0,
			RESIZE_TOP,
			RESIZE_TOPRIGHT,
			RESIZE_LEFT,
			RESIZE_RIGHT,
			RESIZE_BOTTOMLEFT,
			RESIZE_BOTTOM,
			RESIZE_BOTTOMRIGHT
		};
		enum GridSize {SIZE_2x2=0, SIZE_4x4, SIZE_8x8, SIZE_16x16, SIZE_32x32, SIZE_64x64};
		ScreenLayouts_TitleMenu(QWidget *parent=0);
		~ScreenLayouts_TitleMenu();
		void setupLayout();
		void shutdownLayout();
	protected slots:
		void on_aCopy_triggered();
		void on_aCut_triggered();
		void on_aDeleteItem_triggered();
		void on_aPaste_triggered();
		void on_bPreview_clicked();
		void on_cboxZoom_currentIndexChanged(int index);
		void on_gvTitleMenuLayout_customContextMenuRequested(const QPoint &pos);
		void on_gvTitleMenuLayout_dragEnterEvent(QDragEnterEvent *event);
		void on_gvTitleMenuLayout_dragLeaveEvent(QDragLeaveEvent *event);
		void on_gvTitleMenuLayout_dragMoveEvent(QDragMoveEvent *event);
		void on_gvTitleMenuLayout_dropEvent(QDropEvent *event);
		void on_gvTitleMenuLayout_leaveEvent(QEvent *event);
		void on_gvTitleMenuLayout_mouseDoubleClickEvent(QMouseEvent *event);
		void on_gvTitleMenuLayout_mouseMoveEvent(QMouseEvent *event);
		void on_gvTitleMenuLayout_mousePressEvent(QMouseEvent *event);
		void on_gvTitleMenuLayout_mouseReleaseEvent(QMouseEvent *event);
		void on_twObjectList_customContextMenuRequested(const QPoint &pos);
		void on_twObjectList_itemSelectionChanged();
		void on_twObjectList_itemDoubleClicked(QTreeWidgetItem *item, int column);
		void on_twObjectList_itemChanged(QTreeWidgetItem *item, int column);
		void on_twProperties_itemChanged(QTableWidgetItem *item);
		void on_bEasyNavigationCreator_clicked();
		void on_bEditMenusEvents_clicked();
		void on_cbSnapToGrid_toggled(bool checked);
		void on_cboxGridSize_currentIndexChanged(int index);
		void applyMenuNavigation();
	private:
		void selectedObjectChanged();
		MenuObject *getObjectUnderCursor(QPointF scenePos);
		void buildObjectTree(MenuObject *parentObject, QTreeWidgetItem *parentItem);
		void deleteChildItems(QTreeWidgetItem *item);
		void addObjectToLayout(MenuObject *object);
		QGraphicsScene *titleMenuScene;
		QGraphicsPixmapItem *menuBackgroundItem;
		QGraphicsRectItem *backupMenuBackgroundItem;
		QGraphicsRectItem *menuGridItem;
		QGraphicsRectItem *resizeItem_topLeft;
		QGraphicsRectItem *resizeItem_top;
		QGraphicsRectItem *resizeItem_topRight;
		QGraphicsRectItem *resizeItem_left;
		QGraphicsRectItem *resizeItem_right;
		QGraphicsRectItem *resizeItem_bottomLeft;
		QGraphicsRectItem *resizeItem_bottom;
		QGraphicsRectItem *resizeItem_bottomRight;
		QGraphicsRectItem *selectionRect;
		QGraphicsRectItem *highlightedObject;
		TitleMenuLayout *layout;
		MenuObject *selectedObject;
		QMap<QString, QTreeWidgetItem*> objectItemList;
		QPointF hotspot;
		int activeResizeHandle;
		bool resizeInProgress;
		bool repositionInProgress;
		bool ignoreEvents;
		bool objectMoved;
};

#endif // SCREENLAYOUTS_TITLEMENU_H
