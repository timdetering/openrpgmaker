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
#include "dualprogressindicator.h"

DualProgressIndicator::DualProgressIndicator(QString title, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setWindowTitle(title);
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SIGNAL(processCanceled()));
	allowClose = false;
}

DualProgressIndicator::~DualProgressIndicator()
{
}

void DualProgressIndicator::setOverallProgressBarRange(int min, int max)
{
	pbOverall->setRange(min, max);
	QCoreApplication::processEvents();
}

void DualProgressIndicator::setOverallProgressBarValue(int value)
{
	pbOverall->setValue(value);
	QCoreApplication::processEvents();
}

void DualProgressIndicator::setCurrentProcessProgressBarRange(int min, int max)
{
	pbCurrentProcess->setRange(min, max);
	QCoreApplication::processEvents();
}

void DualProgressIndicator::setCurrentProcessProgressBarValue(int value)
{
	pbCurrentProcess->setValue(value);
	QCoreApplication::processEvents();
}

void DualProgressIndicator::setCurrentProcessTitle(QString value)
{
	lblCurrentProcess->setText(value);
	QCoreApplication::processEvents();
}

void DualProgressIndicator::on_buttonBox_rejected()
{
	allowClose = true;
	close();
}

void DualProgressIndicator::doneLoading()
{
	allowClose = true;
	close();
}

void DualProgressIndicator::closeEvent(QCloseEvent *event)
{
	if (allowClose)
		event->accept();
	else
		event->ignore();
}
