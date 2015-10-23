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

#include <QGraphicsEffect>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <cstdio>
#include "mygraphicstinteffect.h"

MyGraphicsTintEffect::MyGraphicsTintEffect(QObject *parent) : QGraphicsEffect(parent)
{
	tR = 0.0;
	tG = 0.0;
	tB = 0.0;
	tA = 0.0;
}

MyGraphicsTintEffect::~MyGraphicsTintEffect()
{
}

void MyGraphicsTintEffect::setColor(int r, int g, int b, int a)
{
	tR = r / 255.0;
	tG = g / 255.0;
	tB = b / 255.0;
	tA = a / 100.0;
	update();
}

void MyGraphicsTintEffect::draw(QPainter *painter)
{
	QPoint offset;
	const QPixmap pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);
	QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
	QRgb pixel;
	float sR, sG, sB;
	float fR, fG, fB;
	if (tA > 0.0)
	{
		for (int y = 0; y < image.height(); ++y)
		{
			for (int x = 0; x < image.width(); ++x)
			{
				pixel = image.pixel(x, y);
				if (tA == 1.0)
					image.setPixel(x, y, qRgba((int)(tR * 255), (int)(tG * 255), (int)(tB * 255), qAlpha(pixel)));
				else
				{
					sR = qRed(pixel) / 255.0;
					sG = qGreen(pixel) / 255.0;
					sB = qBlue(pixel) / 255.0;
					fR = (tR * tA) + (sR * (1.0 - tA));
					fG = (tG * tA) + (sG * (1.0 - tA));
					fB = (tB * tA) + (sB * (1.0 - tA));
					image.setPixel(x, y, qRgba((int)(fR * 255), (int)(fG * 255), (int)(fB * 255), qAlpha(pixel)));
				}
			}
		}
	}
	painter->drawImage(offset, image);
}
