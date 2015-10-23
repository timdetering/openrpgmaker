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

#include <QImage>
#include <QPixmap>
#include "colorbutton.h"
#include "configuregradient.h"

ConfigureGradient::ConfigureGradient(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	QObject::connect(bTopColor, SIGNAL(colorChanged(QColor)), this, SLOT(updatePreviewGradient()));
	QObject::connect(bBottomColor, SIGNAL(colorChanged(QColor)), this, SLOT(updatePreviewGradient()));
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureGradient::~ConfigureGradient()
{
}

QColor ConfigureGradient::getTopColor()
{
	return bTopColor->getColor();
}

QColor ConfigureGradient::getBottomColor()
{
	return bBottomColor->getColor();
}

void ConfigureGradient::setColors(QColor topColor, QColor bottomColor)
{
	bTopColor->setColor(topColor);
	bBottomColor->setColor(bottomColor);
	updatePreviewGradient();
}

void ConfigureGradient::updatePreviewGradient()
{
	QImage gradient = QImage(lblPreview->width() - 4, lblPreview->height() - 4, QImage::Format_RGB32);
	QColor topColor = bTopColor->getColor();
	QColor bottomColor = bBottomColor->getColor();
	float tR = (float)topColor.red() / 255.0;
	float tG = (float)topColor.green() / 255.0;
	float tB = (float)topColor.blue() / 255.0;
	float bR = (float)bottomColor.red() / 255.0;
	float bG = (float)bottomColor.green() / 255.0;
	float bB = (float)bottomColor.blue() / 255.0;
	float rangeR = bR - tR;
	float rangeG = bG - tG;
	float rangeB = bB - tB;
	float fR, fG, fB;
	for (int y = 0; y < gradient.height(); ++y)
	{
		float loc = (float)y / (float)(gradient.height() - 1);
		fR = (loc * rangeR) + tR;
		fG = (loc * rangeG) + tG;
		fB = (loc * rangeB) + tB;
		for (int x = 0; x < gradient.width(); ++x)
			gradient.setPixel(x, y, qRgb((int)(fR * 255), (int)(fG * 255), (int)(fB * 255)));
	}
	lblPreview->setPixmap(QPixmap::fromImage(gradient));
}
