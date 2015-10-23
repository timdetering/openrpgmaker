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

#include "animationmasscopyclear.h"

AnimationMassCopyClear::AnimationMassCopyClear(int numFrames, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	bStartFrameSideButton->setSpinBox(sbStartFrame);
	bEndFrameSideButton->setSpinBox(sbEndFrame);
	bSourceFrameSideButton->setSpinBox(sbSourceFrame);
	sbStartFrame->setRange(1, numFrames);
	sbEndFrame->setRange(1, numFrames);
	sbSourceFrame->setRange(1, numFrames);
	sbStartFrame->setValue(1);
	sbEndFrame->setValue(numFrames);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

AnimationMassCopyClear::~AnimationMassCopyClear()
{
}

int AnimationMassCopyClear::getStartFrame()
{
	return sbStartFrame->value() - 1;
}

int AnimationMassCopyClear::getEndFrame()
{
	return sbEndFrame->value() - 1;
}

int AnimationMassCopyClear::getSourceFrame()
{
	return sbSourceFrame->value() - 1;
}

bool AnimationMassCopyClear::massCopy()
{
	return rbCopy->isChecked();
}

void AnimationMassCopyClear::on_sbStartFrame_valueChanged(int value)
{
	sbEndFrame->setMinimum(value);
}

void AnimationMassCopyClear::on_sbEndFrame_valueChanged(int value)
{
	sbStartFrame->setMaximum(value);
}
