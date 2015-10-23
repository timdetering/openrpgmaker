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

#ifndef BARGRAPH_H
#define BARGRAPH_H

class QGraphicsPixmapItem;
class QGraphicsTextItem;

#include <QGraphicsView>

class BarGraph : public QGraphicsView
{
	Q_OBJECT
	public:
		BarGraph(QWidget *parent=0);
		~BarGraph();
		void setupGraph();
		void setBackgroundColor(QColor color);
		void setForegroundColor(QColor color);
		void setTextColor(QColor color);
		void setNumValues(int num);
		int numValues();
		void changeValue(int valueID, int newValue);
		int getValue(int valueID);
		void setMaxValue(int value);
		void setDisplayValue(int valueID);
		void setModifiable(bool on);
		void updateView();
	signals:
		void valueChanged(int level, int value);
		void mouseMoved(QMouseEvent *event);
		void mousePressed(QMouseEvent *event);
		void mouseReleased(QMouseEvent *event);
		void mouseDoubleClicked(QMouseEvent *event);
		void resized(QResizeEvent *event);
	protected:
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseDoubleClickEvent(QMouseEvent *event);
		void resizeEvent(QResizeEvent *event);
	private:
		QGraphicsPixmapItem *graphItem;
		QGraphicsTextItem *valueDisplayItem;
		QImage graph;
		QColor backgroundColor;
		QColor foregroundColor;
		QColor textColor;
		QList<int> values;
		int maxValue;
		int displayValueID;
		bool modifiable;
		bool ignoreEvents;
};

#endif // BARGRAPH_H
