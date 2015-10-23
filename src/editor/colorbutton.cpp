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

#include <QApplication>
#include <QColorDialog>
#include "colorbutton.h"

ColorButton::ColorButton(QWidget *parent) : QPushButton(parent)
{
	color = QColor(Qt::white);
	setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
	QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
}

ColorButton::~ColorButton()
{
}

QColor ColorButton::getColor()
{
	return color;
}

void ColorButton::setColor(QColor value)
{
	color = value;
	setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
	emit colorChanged(color);
}

void ColorButton::buttonClicked()
{
	QColor value = QColorDialog::getColor(color);
	if (value.isValid())
		setColor(value);
}
