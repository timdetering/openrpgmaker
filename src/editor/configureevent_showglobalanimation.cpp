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
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configureevent_showglobalanimation.h"
#include "projectdata.h"
#include "projectdata_globalanimation.h"

ConfigureEvent_ShowGlobalAnimation::ConfigureEvent_ShowGlobalAnimation(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	animRef = NULL;
	wGlobalAnimationPreview->setScale(0.25);
	cboxGlobalAnimation->addItems(ProjectData::globalAnimationNames);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_ShowGlobalAnimation::~ConfigureEvent_ShowGlobalAnimation()
{
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

int ConfigureEvent_ShowGlobalAnimation::getAction()
{
	if (rbPlayOnce->isChecked())
		return PLAY_ONCE;
	else if (rbBeginPlaying->isChecked())
		return BEGIN_PLAYING;
	else if (rbStopPlaying->isChecked())
		return STOP_PLAYING;
	return -1;
}

void ConfigureEvent_ShowGlobalAnimation::setAction(int value)
{
	if (value == PLAY_ONCE)
		rbPlayOnce->setChecked(true);
	else if (value == BEGIN_PLAYING)
		rbBeginPlaying->setChecked(true);
	else if (value == STOP_PLAYING)
		rbStopPlaying->setChecked(true);
}

int ConfigureEvent_ShowGlobalAnimation::getAnimationID()
{
	return cboxGlobalAnimation->currentIndex();
}

void ConfigureEvent_ShowGlobalAnimation::setAnimationID(int value)
{
	cboxGlobalAnimation->setCurrentIndex(value);
}

bool ConfigureEvent_ShowGlobalAnimation::getHaltOtherProcesses()
{
	return cbHaltOtherProcesses->isChecked();
}

void ConfigureEvent_ShowGlobalAnimation::setHaltOtherProcesses(bool value)
{
	cbHaltOtherProcesses->setChecked(value);
}

void ConfigureEvent_ShowGlobalAnimation::on_cboxGlobalAnimation_currentIndexChanged(int index)
{
	if (index >= 0 && index < ProjectData::globalAnimationNames.size())
	{
		wGlobalAnimationPreview->stopAnimation();
		if (animRef != NULL)
		{
			delete animRef;
			animRef = NULL;
		}
		animRef = new ProjectData::DataReference<GlobalAnimation>(index, __FILE__, __LINE__);
		wGlobalAnimationPreview->cycleAnimation(animRef->getDataID());
	}
}

void ConfigureEvent_ShowGlobalAnimation::on_rbBeginPlaying_toggled(bool checked)
{
	if (checked)
	{
		frmOption->setEnabled(false);
		frmGlobalAnimation->setEnabled(true);
	}
}

void ConfigureEvent_ShowGlobalAnimation::on_rbPlayOnce_toggled(bool checked)
{
	if (checked)
	{
		frmOption->setEnabled(true);
		frmGlobalAnimation->setEnabled(true);
	}
}

void ConfigureEvent_ShowGlobalAnimation::on_rbStopPlaying_toggled(bool checked)
{
	if (checked)
	{
		frmOption->setEnabled(false);
		frmGlobalAnimation->setEnabled(false);
	}
}
