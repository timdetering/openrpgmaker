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
#include "configureevent_changesystembgm.h"
#include "musicbrowser.h"
#include "projectdata.h"

ConfigureEvent_ChangeSystemBGM::ConfigureEvent_ChangeSystemBGM(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	ProjectData::getSystemBGMInfo(ProjectData::BGM_TITLESCREEN, &currentBGM);
	if (currentBGM.location.isEmpty())
		leBGM->setText("(OFF)");
	else
		leBGM->setText(QFileInfo(currentBGM.location).fileName());
}

ConfigureEvent_ChangeSystemBGM::~ConfigureEvent_ChangeSystemBGM()
{
}

void ConfigureEvent_ChangeSystemBGM::setBGMToChange(int bgm)
{
	cboxSystemBGM->setCurrentIndex(bgm);
	ProjectData::getSystemBGMInfo(bgm, &currentBGM);
	if (currentBGM.location.isEmpty())
		leBGM->setText("(OFF)");
	else
		leBGM->setText(QFileInfo(currentBGM.location).fileName());
}

int ConfigureEvent_ChangeSystemBGM::getBGMToChange()
{
	return cboxSystemBGM->currentIndex();
}

void ConfigureEvent_ChangeSystemBGM::setBGM(const AudioManager::MusicInfo *bgm)
{
	currentBGM.location = bgm->location;
	currentBGM.volume = bgm->volume;
	currentBGM.panning = bgm->panning;
	currentBGM.speed = bgm->speed;
	currentBGM.fadeInTime = bgm->fadeInTime;
	currentBGM.retainPitch = bgm->retainPitch;
	if (currentBGM.location.isEmpty())
		leBGM->setText("(OFF)");
	else
		leBGM->setText(QFileInfo(currentBGM.location).fileName());
}

void ConfigureEvent_ChangeSystemBGM::getBGM(AudioManager::MusicInfo *bgm)
{
	bgm->location = currentBGM.location;
	bgm->volume = currentBGM.volume;
	bgm->panning = currentBGM.panning;
	bgm->speed = currentBGM.speed;
	bgm->fadeInTime = currentBGM.fadeInTime;
	bgm->retainPitch = currentBGM.retainPitch;
}

void ConfigureEvent_ChangeSystemBGM::on_bBrowseBGM_clicked()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	musicBrowser->setupBrowser();
	musicBrowser->setMusicInfo(currentBGM);
	if (musicBrowser->exec())
	{
		musicBrowser->getMusicInfo(&currentBGM);
		leBGM->setText(musicBrowser->getMusicName());
	}
	delete musicBrowser;
}

void ConfigureEvent_ChangeSystemBGM::on_cboxSystemBGM_currentIndexChanged(int index)
{
	setBGMToChange(index);
}
