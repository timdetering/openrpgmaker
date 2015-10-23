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

#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QKeyEvent>
#include <QMouseEvent>
//#include "browsebattleevents.h"
//#include "browsecommonevents.h"
#include "browsemapevents.h"
#include "browsemenuevents.h"
#include "clipboard.h"
#include "eventview.h"
#include "projectdata_baseevent.h"

EventView::EventView(QWidget *parent) : QListWidget(parent)
{
}

EventView::~EventView()
{
}

void EventView::setupView(int id, int type, BaseEvent *event)
{
	currentEvent = event;
	eventList = currentEvent->getEventPointers();
	callerID = id;
	callerType = type;
	updateView();
}

void EventView::focusOutEvent()
{
	for (int i = 0; i < count(); ++i)
		item(i)->setSelected(false);
	setCurrentItem(NULL);
}

void EventView::keyPressEvent(QKeyEvent *event)
{
	int row = currentRow();
	if (row != -1)
	{
		QString currentText = item(row)->text();
		if (event->key() == Qt::Key_Delete && !currentText.contains(QRegExp("^\\s*::")) && !currentText.contains(QRegExp("<>$")))
		{
			int parentID = eventsParentIDAtRow(row);
			int childTypeID = childEventTypeIDAtRow(row);
			int childID = childEventIDAtRow(row);
			eventList[parentID]->deleteChildEvent(childID, childTypeID);
			updateView();
			eventList.clear();
			eventList = currentEvent->getEventPointers();
		}
		else if (event->key() == Qt::Key_Space && !currentText.contains(QRegExp("^\\s*::")) && !currentText.contains(QRegExp("<>$")))
		{
			int eventID = eventIDAtRow(row);
			if (eventList[eventID]->configureEvent(callerID, callerType))
			{
				updateView();
				eventList.clear();
				eventList = currentEvent->getEventPointers();
			}
		}
		else if (event->key() == Qt::Key_Return && !currentText.contains(QRegExp("^\\s*::")))
		{
			BaseEvent *eventToAdd = BaseEvent::browseEvents(currentEvent->getBaseType(), callerID, callerType);
			if (eventToAdd != NULL)
			{
				int eventsID = childEventIDAtRow(row);
				int parentID = eventsParentIDAtRow(row);
				int childTypeID = childEventTypeIDAtRow(row);
				eventList[parentID]->addChildEvent(eventToAdd, eventsID, childTypeID);
				updateView();
				eventList.clear();
				eventList = currentEvent->getEventPointers();
				setCurrentRow(row);
			}
		}
		else if (event->matches(QKeySequence::Cut) && !currentText.contains(QRegExp("^\\s*::")) && !currentText.contains(QRegExp("<>$")))
		{
			int eventID = eventIDAtRow(row);
			int childTypeID = childEventTypeIDAtRow(row);
			int childID = childEventIDAtRow(row);
			int parentID = eventsParentIDAtRow(row);
			Clipboard::clear();
			Clipboard::dataType = Clipboard::TYPE_BASEEVENT;
			Clipboard::data.baseEvent = new BaseEvent(eventList[eventID]);
			eventList[parentID]->deleteChildEvent(childID, childTypeID);
			updateView();
			eventList.clear();
			eventList = currentEvent->getEventPointers();
			emit eventCopied();
		}
		else if (event->matches(QKeySequence::Copy) && !currentText.contains(QRegExp("^\\s*::")) && !currentText.contains(QRegExp("<>$")))
		{
			int eventID = eventIDAtRow(currentRow());
			Clipboard::clear();
			Clipboard::dataType = Clipboard::TYPE_BASEEVENT;
			Clipboard::data.baseEvent = new BaseEvent(eventList[eventID]);
			emit eventCopied();
		}
		else if (event->matches(QKeySequence::Paste) && !currentText.contains(QRegExp("^\\s*::")) && Clipboard::dataType == Clipboard::TYPE_BASEEVENT)
		{
			int eventID = childEventIDAtRow(row);
			int parentID = eventsParentIDAtRow(row);
			int childTypeID = childEventTypeIDAtRow(row);
			eventList[parentID]->addChildEvent(new BaseEvent(Clipboard::data.baseEvent), eventID, childTypeID);
			updateView();
			eventList.clear();
			eventList = currentEvent->getEventPointers();
			setCurrentRow(row);
		}
	}
}

void EventView::mouseDoubleClickEvent(QMouseEvent *event)
{
	QListWidgetItem *selectedItem = itemAt(event->pos());
	if (selectedItem != NULL)
	{
		int selectedRow = row(selectedItem);
		if (!item(selectedRow)->text().contains(QRegExp("^\\s*::")))
		{
			BaseEvent *eventToAdd = BaseEvent::browseEvents(currentEvent->getBaseType(), callerID, callerType);
			if (eventToAdd != NULL)
			{
				int row = currentRow();
				int eventsID = childEventIDAtRow(row);
				int parentID = eventsParentIDAtRow(row);
				int childTypeID = childEventTypeIDAtRow(row);
				eventList[parentID]->addChildEvent(eventToAdd, eventsID, childTypeID);
				updateView();
				eventList.clear();
				eventList = currentEvent->getEventPointers();
				setCurrentRow(row);
			}
		}
	}
}

void EventView::mouseMoveEvent(QMouseEvent *event)
{
	QListWidgetItem *selectedItem = itemAt(event->pos());
	if (selectedItem != NULL)
		setCurrentRow(row(selectedItem));
	else
	{
		for (int i = 0; i < count(); ++i)
			item(i)->setSelected(false);
		setCurrentItem(NULL);
	}
}

void EventView::mousePressEvent(QMouseEvent *event)
{
	QListWidgetItem *selectedItem = itemAt(event->pos());
	if (selectedItem != NULL)
		setCurrentRow(row(selectedItem));
	else
	{
		for (int i = 0; i < count(); ++i)
			item(i)->setSelected(false);
		setCurrentItem(NULL);
	}
}

void EventView::setCurrentRow(int row)
{
	QListWidgetItem *currentItem = item(row);
	QString selectedText = currentItem->text();
	int i, indentAmount = selectedText.indexOf(QRegExp("[^ ]"));
	bool isChildOf = false;
	bool endAtBranch = selectedText.contains(QRegExp("^\\s*::"));
	setCurrentItem(currentItem);
	for (i = 0; i < count(); ++i)
	{
		if (i < row)
			item(i)->setSelected(false);
		else if (i == row)
		{
			item(i)->setSelected(true);
			isChildOf = true;
		}
		else if (i > row && item(i)->text().indexOf(QRegExp("[^ ]")) < indentAmount)
		{
			item(i)->setSelected(false);
			isChildOf = false;
		}
		else if (i > row && item(i)->text().indexOf(QRegExp("[^ ]")) == indentAmount)
		{
			if (item(i)->text().contains(QRegExp("^\\s*::")) && endAtBranch)
			{
				isChildOf = false;
				item(i)->setSelected(false);
			}
			else if (item(i)->text().contains(QRegExp("^\\s*::")) && !endAtBranch && isChildOf)
				item(i)->setSelected(true);
			else
			{
				item(i)->setSelected(false);
				isChildOf = false;
			}
		}
		else if (i > row && isChildOf)
			item(i)->setSelected(true);
		else
			item(i)->setSelected(false);
	}
}

int EventView::eventIDAtRow(int row)
{
	int eventID = 1;
	for (int i = 0; i < row; ++i)
	{
		if (!item(i)->text().contains(QRegExp("^\\s*::")))
			++eventID;
	}
	return eventID;
}

int EventView::eventsParentIDAtRow(int row)
{
	int indentAmount = item(row)->text().indexOf(QRegExp("[^ ]"));
	if (indentAmount > 0)
	{
		for (int i = row - 1; i >= 0; --i)
		{
			QString text = item(i)->text();
			if (text.indexOf(QRegExp("[^ ]")) < indentAmount && !text.contains(QRegExp("^\\s*::")))
				return eventIDAtRow(i);
		}
	}
	return 0;
}

int EventView::childEventIDAtRow(int row)
{
	int eventID = 0;
	int indentAmount = item(row)->text().indexOf(QRegExp("[^ ]"));
	bool found = false;
	for (int i = row - 1; i >= 0 && !found; --i)
	{
		QString text = item(i)->text();
		int currentIndent = text.indexOf(QRegExp("[^ ]"));
		if (currentIndent < indentAmount)
			found = true;
		else if (currentIndent == indentAmount && !text.contains(QRegExp("^\\s*::")))
			++eventID;
	}
	return eventID;
}

int EventView::childEventTypeIDAtRow(int row)
{
	QString itemText = item(row)->text();
	int id = -1;
	int indentAtRow = getIndentAtRow(row);
	bool done = false;
	for (int i = row - 1; i >= 0 && !done; --i)
	{
		if (getIndentAtRow(i) == indentAtRow - 1)
		{
			if (item(i)->text().contains(QRegExp("^\\s*::")))
				++id;
			else
				done = true;
		}
	}
	return id;
}

int EventView::getIndentAtRow(int row)
{
	QString itemText = item(row)->text();
	int indentAtRow = 0;
	for (int i = 0; itemText[i].isSpace() && i < itemText.size(); ++i)
		++indentAtRow;
	indentAtRow /= 2;
	return indentAtRow;
}

void EventView::updateView()
{
	QStringList viewableText = currentEvent->getViewableText(0);
	clear();
	for (int i = 0; i < viewableText.size(); ++i)
	{
		QListWidgetItem *item = new QListWidgetItem(viewableText[i]);
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		if (viewableText[i].contains("/*"))
			item->setForeground(QBrush(Qt::darkGreen));
		addItem(item);
	}
}
