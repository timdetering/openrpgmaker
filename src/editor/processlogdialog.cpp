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

#include <QScrollBar>
#include <QListWidgetItem>
#include <QFont>
#include <QCloseEvent>
#include <cstdio>
#include "processlogdialog.h"

ProcessLogDialog::ProcessLogDialog(QString fileLocation, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	scrollBar = lwTaskLog->verticalScrollBar();
	file = fopen(fileLocation.toUtf8().data(), "w");
	lblLogFile->setText(fileLocation);
	allowClose = false;
}

ProcessLogDialog::~ProcessLogDialog()
{
	fclose(file);
}

void ProcessLogDialog::setCurrentTask(QString currentTask)
{
	QListWidgetItem *item = new QListWidgetItem(currentTask);
	QFont font = lwTaskLog->font();
	bool scroll = (scrollBar->sliderPosition() == scrollBar->maximum()) ? true:false;
	lblCurrentTask->setText(currentTask);
	font.setBold(true);
	item->setFont(font);
	lwTaskLog->addItem(item);
	if (scroll)
		lwTaskLog->scrollToItem(item, QAbstractItemView::PositionAtBottom);
	fprintf(file, "%s\n", currentTask.toUtf8().data());
	QCoreApplication::processEvents();
}

void ProcessLogDialog::addLogMessage(QString message)
{
	bool scroll = (scrollBar->sliderPosition() == scrollBar->maximum()) ? true:false;
	lwTaskLog->addItem(message);
	if (scroll)
		lwTaskLog->scrollToItem(lwTaskLog->item(lwTaskLog->count() - 1), QAbstractItemView::PositionAtBottom);
	fprintf(file, "  %s\n", message.toUtf8().data());
	QCoreApplication::processEvents();
}

void ProcessLogDialog::clearLog()
{
	lwTaskLog->clear();
	QCoreApplication::processEvents();
}


void ProcessLogDialog::done()
{
	allowClose = true;
	close();
}

void ProcessLogDialog::closeEvent(QCloseEvent *event)
{
	if (allowClose)
		event->accept();
	else
		event->ignore();
}
