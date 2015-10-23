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

#include <QPushButton>
#include "animationautocomplete.h"
#include "projectdata_battleanimation.h"
#include "projectdata_globalanimation.h"

AnimationAutoComplete::AnimationAutoComplete(int animType, int animID, QWidget *parent) : QDialog(parent)
{
	ignoreEvents = true;
	battleAnimation = NULL;
	globalAnimation = NULL;
	setupUi(this);
	bStartFrameSideButton->setSpinBox(sbStartFrame);
	bEndFrameSideButton->setSpinBox(sbEndFrame);
	bStartCellSideButton->setSpinBox(sbStartCell);
	bEndCellSideButton->setSpinBox(sbEndCell);
	if (animType == TYPE_BATTLE)
	{
		battleAnimation = new ProjectData::DataReference<BattleAnimation>(animID, __FILE__, __LINE__);
		setupBattleAnimation();
	}
	else
	{
		globalAnimation = new ProjectData::DataReference<GlobalAnimation>(animID, __FILE__, __LINE__);
		setupGlobalAnimation();
	}
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	ignoreEvents = false;
	on_sbStartFrame_valueChanged(sbStartFrame->value());
}

AnimationAutoComplete::~AnimationAutoComplete()
{
	if (battleAnimation != NULL)
	{
		delete battleAnimation;
		battleAnimation = NULL;
	}
	if (globalAnimation != NULL)
	{
		delete globalAnimation;
		globalAnimation = NULL;
	}
}

int AnimationAutoComplete::getStartFrame()
{
	return sbStartFrame->value() - 1;
}

int AnimationAutoComplete::getEndFrame()
{
	return sbEndFrame->value() - 1;
}

int AnimationAutoComplete::getStartCell()
{
	return sbStartCell->value() - 1;
}

int AnimationAutoComplete::getEndCell()
{
	return sbEndCell->value() - 1;
}

bool AnimationAutoComplete::carryImageID()
{
	return cbImageID->isChecked();
}

bool AnimationAutoComplete::carryScale()
{
	return cbScale->isChecked();
}

bool AnimationAutoComplete::carryRotation()
{
	return cbRotation->isChecked();
}

bool AnimationAutoComplete::carryOpacity()
{
	return cbOpacity->isChecked();
}

bool AnimationAutoComplete::carryTintColor()
{
	return cbTintColor->isChecked();
}

bool AnimationAutoComplete::carryTintAmount()
{
	return cbTintAmount->isChecked();
}

void AnimationAutoComplete::on_sbStartFrame_valueChanged(int value)
{
	if (!ignoreEvents)
	{
		if (battleAnimation != NULL)
		{
			BattleAnimation::AnimationFrame *startFrame = (*battleAnimation)->getFrame(value - 1);
			BattleAnimation::AnimationFrame *endFrame = (*battleAnimation)->getFrame(sbEndFrame->value() - 1);
			sbEndFrame->setMinimum(value);
			if (startFrame->imageList.size() <= endFrame->imageList.size())
			{
				if (startFrame->imageList.size() == 0)
				{
					sbStartCell->setRange(0, 0);
					sbEndCell->setRange(0, 0);
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				}
				else
				{
					sbStartCell->setRange(1, startFrame->imageList.size());
					sbEndCell->setRange(1, startFrame->imageList.size());
					sbStartCell->setValue(1);
					sbEndCell->setValue(startFrame->imageList.size());
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
				}
			}
			else
			{
				if (endFrame->imageList.size() == 0)
				{
					sbStartCell->setRange(0, 0);
					sbEndCell->setRange(0, 0);
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				}
				else
				{
					sbStartCell->setRange(1, endFrame->imageList.size());
					sbEndCell->setRange(1, endFrame->imageList.size());
					sbStartCell->setValue(1);
					sbEndCell->setValue(endFrame->imageList.size());
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
				}
			}
		}
		else
		{
			GlobalAnimation::AnimationFrame *startFrame = (*globalAnimation)->getFrame(value - 1);
			GlobalAnimation::AnimationFrame *endFrame = (*globalAnimation)->getFrame(sbEndFrame->value() - 1);
			sbEndFrame->setMinimum(value);
			if (startFrame->imageList.size() <= endFrame->imageList.size())
			{
				if (startFrame->imageList.size() == 0)
				{
					sbStartCell->setRange(0, 0);
					sbEndCell->setRange(0, 0);
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				}
				else
				{
					sbStartCell->setRange(1, startFrame->imageList.size());
					sbEndCell->setRange(1, startFrame->imageList.size());
					sbStartCell->setValue(1);
					sbEndCell->setValue(startFrame->imageList.size());
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
				}
			}
			else
			{
				if (endFrame->imageList.size() == 0)
				{
					sbStartCell->setRange(0, 0);
					sbEndCell->setRange(0, 0);
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				}
				else
				{
					sbStartCell->setRange(1, endFrame->imageList.size());
					sbEndCell->setRange(1, endFrame->imageList.size());
					sbStartCell->setValue(1);
					sbEndCell->setValue(endFrame->imageList.size());
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
				}
			}
		}
	}
}

void AnimationAutoComplete::on_sbEndFrame_valueChanged(int value)
{
	if (!ignoreEvents)
	{
		if (battleAnimation != NULL)
		{
			BattleAnimation::AnimationFrame *startFrame = (*battleAnimation)->getFrame(sbStartFrame->value() - 1);
			BattleAnimation::AnimationFrame *endFrame = (*battleAnimation)->getFrame(value - 1);
			sbStartFrame->setMaximum(value);
			if (startFrame->imageList.size() <= endFrame->imageList.size())
			{
				if (startFrame->imageList.size() == 0)
				{
					sbStartCell->setRange(0, 0);
					sbEndCell->setRange(0, 0);
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				}
				else
				{
					sbStartCell->setRange(1, startFrame->imageList.size());
					sbEndCell->setRange(1, startFrame->imageList.size());
					sbStartCell->setValue(1);
					sbEndCell->setValue(startFrame->imageList.size());
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
				}
			}
			else
			{
				if (endFrame->imageList.size() == 0)
				{
					sbStartCell->setRange(0, 0);
					sbEndCell->setRange(0, 0);
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				}
				else
				{
					sbStartCell->setRange(1, endFrame->imageList.size());
					sbEndCell->setRange(1, endFrame->imageList.size());
					sbStartCell->setValue(1);
					sbEndCell->setValue(endFrame->imageList.size());
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
				}
			}
		}
		else
		{
			GlobalAnimation::AnimationFrame *startFrame = (*globalAnimation)->getFrame(sbStartFrame->value() - 1);
			GlobalAnimation::AnimationFrame *endFrame = (*globalAnimation)->getFrame(value - 1);
			sbStartFrame->setMaximum(value);
			if (startFrame->imageList.size() <= endFrame->imageList.size())
			{
				if (startFrame->imageList.size() == 0)
				{
					sbStartCell->setRange(0, 0);
					sbEndCell->setRange(0, 0);
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				}
				else
				{
					sbStartCell->setRange(1, startFrame->imageList.size());
					sbEndCell->setRange(1, startFrame->imageList.size());
					sbStartCell->setValue(1);
					sbEndCell->setValue(startFrame->imageList.size());
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
				}
			}
			else
			{
				if (endFrame->imageList.size() == 0)
				{
					sbStartCell->setRange(0, 0);
					sbEndCell->setRange(0, 0);
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				}
				else
				{
					sbStartCell->setRange(1, endFrame->imageList.size());
					sbEndCell->setRange(1, endFrame->imageList.size());
					sbStartCell->setValue(1);
					sbEndCell->setValue(endFrame->imageList.size());
					buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
				}
			}
		}
	}
}

void AnimationAutoComplete::on_sbStartCell_valueChanged(int value)
{
	sbEndCell->setMinimum(value);
}

void AnimationAutoComplete::on_sbEndCell_valueChanged(int value)
{
	sbStartCell->setMaximum(value);
}

void AnimationAutoComplete::setupBattleAnimation()
{
	BattleAnimation::AnimationFrame *startFrame = (*battleAnimation)->getFrame(0);
	BattleAnimation::AnimationFrame *endFrame = (*battleAnimation)->getFrame((*battleAnimation)->numFrames() - 1);
	int numFrames = (*battleAnimation)->numFrames();
	sbStartFrame->setRange(1, numFrames);
	sbStartFrame->setValue(1);
	sbEndFrame->setRange(1, numFrames);
	sbEndFrame->setValue(numFrames);
	if (startFrame->imageList.size() <= endFrame->imageList.size())
	{
		sbStartCell->setRange(1, startFrame->imageList.size());
		sbEndCell->setRange(1, startFrame->imageList.size());
		sbStartCell->setValue(1);
		sbEndCell->setValue(startFrame->imageList.size());
	}
	else
	{
		sbStartCell->setRange(1, endFrame->imageList.size());
		sbEndCell->setRange(1, endFrame->imageList.size());
		sbStartCell->setValue(1);
		sbEndCell->setValue(endFrame->imageList.size());
	}
}

void AnimationAutoComplete::setupGlobalAnimation()
{
	GlobalAnimation::AnimationFrame *startFrame = (*globalAnimation)->getFrame(0);
	GlobalAnimation::AnimationFrame *endFrame = (*globalAnimation)->getFrame((*globalAnimation)->numFrames() - 1);
	int numFrames = (*globalAnimation)->numFrames();
	sbStartFrame->setRange(1, numFrames);
	sbStartFrame->setValue(1);
	sbEndFrame->setRange(1, numFrames);
	sbEndFrame->setValue(numFrames);
	if (startFrame->imageList.size() <= endFrame->imageList.size())
	{
		sbStartCell->setRange(1, startFrame->imageList.size());
		sbEndCell->setRange(1, startFrame->imageList.size());
		sbStartCell->setValue(1);
		sbEndCell->setValue(startFrame->imageList.size());
	}
	else
	{
		sbStartCell->setRange(1, endFrame->imageList.size());
		sbEndCell->setRange(1, endFrame->imageList.size());
		sbStartCell->setValue(1);
		sbEndCell->setValue(endFrame->imageList.size());
	}
}
