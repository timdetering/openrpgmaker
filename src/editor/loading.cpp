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

#include <QCloseEvent>
#include <QCoreApplication>
#include <QTimer>
#include "loading.h"

Loading::Loading(QString title, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setWindowTitle(title);
	lblTitle->setText(title);
	timer = new QTimer(this);
	timer->setSingleShot(false);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateLoadingText()));
	timer->start(250);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	lblLoading->setText("Loading");
	allowClose = false;
	numberOfDots = 0;
}

Loading::~Loading()
{
}

void Loading::doneLoading()
{
	allowClose = true;
	close();
}

void Loading::updateLoadingText()
{
	QString loadingText = "Loading";
	numberOfDots = (numberOfDots + 1) % 5;
	for (int i = 0; i < numberOfDots; ++i)
		loadingText += ".";
	lblLoading->setText(loadingText);
	QCoreApplication::processEvents();
}

void Loading::closeEvent(QCloseEvent *event)
{
	if (allowClose)
		event->accept();
	else
		event->ignore();
}
