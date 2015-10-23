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

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QGraphicsView>

class ImageView : public QGraphicsView
{
	Q_OBJECT
	public:
		ImageView(QWidget * parent=0);
		ImageView(QGraphicsScene *scene, QWidget * parent=0);
	signals:
		void dragEnterEvent(QDragEnterEvent *event);
		void dragLeaveEvent(QDragLeaveEvent *event);
		void dragMoveEvent(QDragMoveEvent *event);
		void dropEvent(QDropEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void leaveEvent(QEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseDoubleClickEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void hScrollBarRangeChanged(int min, int max);
		void hScrollBarValueChanged(int value);
		void vScrollBarRangeChanged(int min, int max);
		void vScrollBarValueChanged(int value);
	public slots:
		void setHScrollBarRange(int min, int max);
		void setHScrollBarValue(int value);
		void setVScrollBarRange(int min, int max);
		void setVScrollBarValue(int value);
	protected:
		void changeEvent(QEvent *event);
};

#endif // IMAGEWIDGET_H
