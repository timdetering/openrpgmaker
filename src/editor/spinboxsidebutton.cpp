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

#include <QCursor>
#include <QApplication>
#include <QToolButton>
#include <QMouseEvent>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "spinboxsidebutton.h"

SpinBoxSideButton::SpinBoxSideButton(QWidget *parent) : QToolButton(parent)
{
	spinBox = NULL;
	doubleSpinBox = NULL;
	previousY = 0;
	processMovement = false;
}

SpinBoxSideButton::~SpinBoxSideButton()
{
	spinBox = NULL;
}

void SpinBoxSideButton::setSpinBox(QSpinBox *box)
{
	spinBox = box;
	doubleSpinBox = NULL;
}

void SpinBoxSideButton::setDoubleSpinBox(QDoubleSpinBox *box)
{
	spinBox = NULL;
	doubleSpinBox = box;
}

void SpinBoxSideButton::mouseMoveEvent(QMouseEvent *event)
{
	if (processMovement)
	{
		if (spinBox != NULL)
		{
			int amount = (previousY - event->y()) * spinBox->singleStep();
			spinBox->setValue(spinBox->value() + amount);
		}
		else if (doubleSpinBox != NULL)
		{
			double amount = ((double)(previousY - event->y())) * doubleSpinBox->singleStep();
			doubleSpinBox->setValue(doubleSpinBox->value() + amount);
		}
		previousY = event->y();
	}
}

void SpinBoxSideButton::mousePressEvent(QMouseEvent *event)
{
	QToolButton::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		QApplication::setOverrideCursor(QCursor(Qt::SplitVCursor));
		previousY = event->y();
		processMovement = true;
	}
}

void SpinBoxSideButton::mouseReleaseEvent(QMouseEvent *event)
{
	QToolButton::mouseReleaseEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		QApplication::restoreOverrideCursor();
		processMovement = false;
	}
}
