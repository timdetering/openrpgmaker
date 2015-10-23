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

#ifndef EVENTVIEW_H
#define EVENTVIEW_H

class BaseEvent;

#include <QListWidget>

class EventView : public QListWidget
{
	Q_OBJECT
	public:
		EventView(QWidget *parent=0);
		~EventView();
		void setupView(int id, int type, BaseEvent *event);
	signals:
		void eventCopied();
	protected:
		void focusOutEvent();
		void keyPressEvent(QKeyEvent *event);
		void mouseDoubleClickEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void setCurrentRow(int row);
		int eventIDAtRow(int row);
		int eventsParentIDAtRow(int row);
		int childEventIDAtRow(int row);
		int childEventTypeIDAtRow(int row);
		int getIndentAtRow(int row);
		void updateView();
		QList<BaseEvent*> eventList;
		BaseEvent *currentEvent;
		int callerID;
		int callerType;
};

#endif // EVENTVIEW_H
