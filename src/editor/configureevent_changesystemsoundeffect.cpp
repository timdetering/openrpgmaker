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

#include <QFileInfo>
#include "audiomanager.h"
#include "configureevent_changesystemsoundeffect.h"
#include "projectdata.h"
#include "soundeffectbrowser.h"

ConfigureEvent_ChangeSystemSoundEffect::ConfigureEvent_ChangeSystemSoundEffect(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_CURSOR, &soundEffect);
	if (soundEffect.location.isEmpty())
		leSoundEffect->setText("(OFF)");
	else
		leSoundEffect->setText(QFileInfo(soundEffect.location).baseName());
}

ConfigureEvent_ChangeSystemSoundEffect::~ConfigureEvent_ChangeSystemSoundEffect()
{
}

void ConfigureEvent_ChangeSystemSoundEffect::setSoundEffectToChange(int soundEffectToChange)
{
	cboxSystemSoundEffect->setCurrentIndex(soundEffectToChange);
	ProjectData::getSystemSFXInfo(soundEffectToChange, &soundEffect);
	if (soundEffect.location.isEmpty())
		leSoundEffect->setText("(OFF)");
	else
		leSoundEffect->setText(QFileInfo(soundEffect.location).baseName());
}

void ConfigureEvent_ChangeSystemSoundEffect::setSoundEffect(AudioManager::SoundEffectInfo *info)
{
	soundEffect.location = info->location;
	soundEffect.volume = info->volume;
	soundEffect.panning = info->panning;
	soundEffect.speed = info->speed;
	soundEffect.numLoops = info->numLoops;
	soundEffect.retainPitch = info->retainPitch;
	if (soundEffect.location.isEmpty())
		leSoundEffect->setText("(OFF)");
	else
		leSoundEffect->setText(QFileInfo(soundEffect.location).baseName());
}

int ConfigureEvent_ChangeSystemSoundEffect::getSoundEffectToChange()
{
	return cboxSystemSoundEffect->currentIndex();
}

void ConfigureEvent_ChangeSystemSoundEffect::getSoundEffect(AudioManager::SoundEffectInfo *info)
{
	info->location = soundEffect.location;
	info->volume = soundEffect.volume;
	info->panning = soundEffect.panning;
	info->speed = soundEffect.speed;
	info->numLoops = soundEffect.numLoops;
	info->retainPitch = soundEffect.retainPitch;
}

void ConfigureEvent_ChangeSystemSoundEffect::on_bBrowseSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	soundBrowser->setupBrowser();
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		leSoundEffect->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void ConfigureEvent_ChangeSystemSoundEffect::on_cboxSystemSoundEffect_currentIndexChanged(int index)
{
	ProjectData::getSystemSFXInfo(index, &soundEffect);
	if (soundEffect.location.isEmpty())
		leSoundEffect->setText("(OFF)");
	else
		leSoundEffect->setText(QFileInfo(soundEffect.location).baseName());
}
