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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "animationmassmodify.h"

AnimationMassModify::AnimationMassModify(int numFrames, int numImages, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	bImageIDSideButton->setSpinBox(sbImageID);
	bScaleSideButton->setSpinBox(sbScale);
	bRotationSideButton->setSpinBox(sbRotation);
	bOpacitySideButton->setSpinBox(sbOpacity);
	bTintAmountSideButton->setSpinBox(sbTintAmount);
	sbImageID->setRange(0, numImages - 1);
	for (int i = 0; i < numFrames; ++i)
	{
		QListWidgetItem *item = new QListWidgetItem;
		QCheckBox *checkBox = new QCheckBox(QString("< %1 >").arg(i, 3));
		lwFrames->addItem(item);
		lwFrames->setItemWidget(item, checkBox);
		frameCheckBoxes.append(checkBox);
	}
	cellCheckBoxes.append(cbCell1);
	cellCheckBoxes.append(cbCell2);
	cellCheckBoxes.append(cbCell3);
	cellCheckBoxes.append(cbCell4);
	cellCheckBoxes.append(cbCell5);
	cellCheckBoxes.append(cbCell6);
	cellCheckBoxes.append(cbCell7);
	cellCheckBoxes.append(cbCell8);
	cellCheckBoxes.append(cbCell9);
	cellCheckBoxes.append(cbCell10);
	cellCheckBoxes.append(cbCell11);
	cellCheckBoxes.append(cbCell12);
	cellCheckBoxes.append(cbCell13);
	cellCheckBoxes.append(cbCell14);
	cellCheckBoxes.append(cbCell15);
	cellCheckBoxes.append(cbCell16);
	wImageID->setEnabled(false);
	wScale->setEnabled(false);
	wRotation->setEnabled(false);
	wOpacity->setEnabled(false);
	wTintColor->setEnabled(false);
	wTintAmount->setEnabled(false);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

AnimationMassModify::~AnimationMassModify()
{
}

QList<bool> AnimationMassModify::getAffectedFrames()
{
	QList<bool> list;
	for (int i = 0; i < frameCheckBoxes.size(); ++i)
		list.append(frameCheckBoxes[i]->isChecked());
	return list;
}

QList<bool> AnimationMassModify::getAffectedCells()
{
	QList<bool> list;
	for (int i = 0; i < cellCheckBoxes.size(); ++i)
		list.append(cellCheckBoxes[i]->isChecked());
	return list;
}

bool AnimationMassModify::modifyImageID()
{
	return cbImageID->isChecked();
}

int AnimationMassModify::getImageID()
{
	return sbImageID->value();
}

bool AnimationMassModify::modifyScale()
{
	return cbScale->isChecked();
}

int AnimationMassModify::getScale()
{
	return sbScale->value();
}

bool AnimationMassModify::modifyRotation()
{
	return cbRotation->isChecked();
}

int AnimationMassModify::getRotation()
{
	return sbRotation->value();
}

bool AnimationMassModify::modifyOpacity()
{
	return cbOpacity->isChecked();
}

int AnimationMassModify::getOpacity()
{
	return sbOpacity->value();
}

bool AnimationMassModify::modifyTintColor()
{
	return cbTintColor->isChecked();
}

QColor AnimationMassModify::getTintColor()
{
	return bSetTintColor->getColor();
}

bool AnimationMassModify::modifyTintAmount()
{
	return cbTintAmount->isChecked();
}

int AnimationMassModify::getTintAmount()
{
	return sbTintAmount->value();
}
