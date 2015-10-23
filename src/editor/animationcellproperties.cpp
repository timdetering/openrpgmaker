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

#include <QGraphicsColorizeEffect>
#include <QColor>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>
#include <QBitmap>
#include <cstdio>
#include "animationcellproperties.h"
#include "spinboxsidebutton.h"

AnimationCellProperties::AnimationCellProperties(QStringList images, int imageID, int scale, int rotation, int opacity, QColor tintColor, int tintAmount, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	imageLocations = images;
	bImageIDSideButton->setSpinBox(sbImageID);
	bScaleSideButton->setSpinBox(sbScale);
	bRotationSideButton->setSpinBox(sbRotation);
	bOpacitySideButton->setSpinBox(sbOpacity);
	bTintAmountSideButton->setSpinBox(sbTintAmount);
	previewScene = new QGraphicsScene(0, 0, 192, 192);
	gvPreview->setScene(previewScene);
	gvPreview->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	previewTintEffect = new MyGraphicsTintEffect();
	previewTintEffect->setColor(tintColor.red(), tintColor.green(), tintColor.blue(), tintAmount);
	previewItem = new QGraphicsPixmapItem;
	previewItem->setPos(0, 0);
	previewItem->setZValue(0);
	previewItem->setTransformOriginPoint(96, 96);
	previewItem->setGraphicsEffect(previewTintEffect);
	previewScene->addItem(previewItem);
	previewItem->setScale(scale / 100.0);
	previewItem->setRotation((float)rotation);
	previewItem->setOpacity(opacity / 100.0);
	sbImageID->setRange(0, images.size() - 1);
	sbImageID->setValue(imageID);
	sbScale->setValue(scale);
	sbRotation->setValue(rotation);
	sbOpacity->setValue(opacity);
	bSetTintColor->setColor(tintColor);
	sbTintAmount->setValue(tintAmount);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

AnimationCellProperties::~AnimationCellProperties()
{
}

int AnimationCellProperties::getImageID()
{
	return sbImageID->value();
}

void AnimationCellProperties::setImageID(int imageID)
{
	sbImageID->setValue(imageID);
}

int AnimationCellProperties::getScale()
{
	return sbScale->value();
}

void AnimationCellProperties::setScale(int scale)
{
	sbScale->setValue(scale);
}

int AnimationCellProperties::getRotation()
{
	return sbRotation->value();
}

void AnimationCellProperties::setRotation(int rotation)
{
	sbRotation->setValue(rotation);
}

int AnimationCellProperties::getOpacity()
{
	return sbOpacity->value();
}

void AnimationCellProperties::setOpacity(int opacity)
{
	sbOpacity->setValue(opacity);
}

QColor AnimationCellProperties::getTintColor()
{
	return bSetTintColor->getColor();
}

void AnimationCellProperties::setTintColor(QColor color)
{
	bSetTintColor->setColor(color);
}

int AnimationCellProperties::getTintAmount()
{
	return sbTintAmount->value();
}

void AnimationCellProperties::setTintAmount(int tintAmount)
{
	sbTintAmount->setValue(tintAmount);
}

void AnimationCellProperties::on_bSetTintColor_colorChanged(QColor color)
{
	previewTintEffect->setColor(color.red(), color.green(), color.blue(), sbTintAmount->value());
}

void AnimationCellProperties::on_sbImageID_valueChanged()
{
	previewItem->setPixmap(QPixmap(imageLocations[sbImageID->value()]));
}

void AnimationCellProperties::on_sbOpacity_valueChanged(int value)
{
	previewItem->setOpacity(value / 100.0);
}

void AnimationCellProperties::on_sbRotation_valueChanged(int value)
{
	previewItem->setRotation((float)value);
}

void AnimationCellProperties::on_sbScale_valueChanged(int value)
{
	previewItem->setScale(value / 100.0);
}

void AnimationCellProperties::on_sbTintAmount_valueChanged(int value)
{
	QColor color = bSetTintColor->getColor();
	previewTintEffect->setColor(color.red(), color.green(), color.blue(), value);
}
