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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef MENUNAVIGATIONCREATOR_H
#define MENUNAVIGATIONCREATOR_H

class Font;
class MenuNavigation;

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QBitmap>
#include "ui_menunavigationcreator.h"

class MenuNavigationCreator : public QDialog, public Ui::MenuNavigationCreator
{
	Q_OBJECT
	public:
		MenuNavigationCreator(QStringList objects, MenuNavigation *nav, QWidget *parent=0);
		~MenuNavigationCreator();
	signals:
		void applyMenuNavigation();
	protected slots:
		void on_gvLayout_dragEnterEvent(QDragEnterEvent *event);
		void on_gvLayout_dragMoveEvent(QDragMoveEvent *event);
		void on_gvLayout_dropEvent(QDropEvent *event);
		void on_gvLayout_mousePressEvent(QMouseEvent *event);
		void on_gvLayout_mouseReleaseEvent(QMouseEvent *event);
		void on_gvLayout_mouseMoveEvent(QMouseEvent *event);
		void on_buttonBox_clicked(QAbstractButton *button);
	private:
		void updateNavigationLayout();
		Font *font;
		MenuNavigation *navigation;
		QGraphicsScene *scene;
		QList<QGraphicsPixmapItem*> objectBGItems;
		QList<QGraphicsPixmapItem*> objectTextItems;
		QList<QGraphicsPixmapItem*> emptyBGItems;
		QList<QGraphicsPixmapItem*> emptyTextItems;
		QList<QGraphicsPixmapItem*> xButtons;
		QList<QPoint> xButtonLocations;
		QPixmap bgPixmap;
		QPixmap bgPixmapMouseOver;
		QPixmap menuNavX;
		QPixmap emptyText;
		QPoint lastPos;
		int imageWidth;
		bool dragging;
};

#endif // MENUNAVIGATIONCREATOR_H
