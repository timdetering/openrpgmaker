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

#include <QDialog>
#include <QMessageBox>
#include "configureintroscreeneffect.h"
#include "imagebrowser.h"
#include "musicbrowser.h"
#include "projectdata.h"
#include "soundeffectbrowser.h"

ConfigureIntroScreenEffect::ConfigureIntroScreenEffect(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	playGlobalAnimation_cboxAnimation->addItems(ProjectData::globalAnimationNames);
	tintScreen_bOpacitySideButton->setSpinBox(tintScreen_sbOpacity);
	tintScreen_bTimeSideButton->setSpinBox(tintScreen_sbTime);
	flashScreen_bOpacitySideButton->setSpinBox(flashScreen_sbOpacity);
	flashScreen_bDurationSideButton->setSpinBox(flashScreen_sbDuration);
	flashScreen_bIntervalSideButton->setSpinBox(flashScreen_sbInterval);
	shakeScreen_bTimeSideButton->setSpinBox(shakeScreen_sbTime);
	wait_bTimeSideButton->setSpinBox(wait_sbTime);
}

ConfigureIntroScreenEffect::~ConfigureIntroScreenEffect()
{
}

void ConfigureIntroScreenEffect::on_changeBGImage_bBrowseImages_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		bgImageLocation = imageBrowser->getFileLocation();
		changeBGImage_leImage->setText(QFileInfo(bgImageLocation).fileName());
		showBGImage_leImage->setText(QFileInfo(bgImageLocation).fileName());
	}
	delete imageBrowser;
}

void ConfigureIntroScreenEffect::on_playBGM_bBrowseBGMs_clicked()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	musicBrowser->setupBrowser();
	musicBrowser->setMusicInfo(bgmInfo);
	if (musicBrowser->exec())
	{
		musicBrowser->getMusicInfo(&bgmInfo);
		playBGM_leBGM->setText(musicBrowser->getMusicName());
	}
	delete musicBrowser;
}

void ConfigureIntroScreenEffect::on_playMovie_bBrowseMovies_clicked()
{
	TODO("Implement Function.");
}

void ConfigureIntroScreenEffect::on_playSFX_bBrowseSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	soundBrowser->setupBrowser();
	soundBrowser->setSoundEffectInfo(soundEffectInfo);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffectInfo);
		playSFX_leSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void ConfigureIntroScreenEffect::on_rbChangeBGImage_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wChangeBGImage);
}

void ConfigureIntroScreenEffect::on_rbFadeBGM_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wFadeBGM);
}

void ConfigureIntroScreenEffect::on_rbFlashScreen_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wFlashScreen);
}

void ConfigureIntroScreenEffect::on_rbHideBGImage_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wHideBGImage);
}

void ConfigureIntroScreenEffect::on_rbPlayBGM_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wPlayBGM);
}

void ConfigureIntroScreenEffect::on_rbPlayGlobalAnimation_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wPlayGlobalAnimation);
}

void ConfigureIntroScreenEffect::on_rbPlayMovie_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wPlayMovie);
}

void ConfigureIntroScreenEffect::on_rbPlaySoundEffect_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wPlaySoundEffect);
}

void ConfigureIntroScreenEffect::on_rbShakeScreen_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wShakeScreen);
}

void ConfigureIntroScreenEffect::on_rbShowBGImage_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wShowBGImage);
}

void ConfigureIntroScreenEffect::on_rbTintScreen_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wTintScreen);
}

void ConfigureIntroScreenEffect::on_rbWait_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wWait);
}

void ConfigureIntroScreenEffect::on_showBGImage_bBrowseImages_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		bgImageLocation = imageBrowser->getFileLocation();
		changeBGImage_leImage->setText(QFileInfo(bgImageLocation).fileName());
		showBGImage_leImage->setText(QFileInfo(bgImageLocation).fileName());
	}
	delete imageBrowser;
}

void ConfigureIntroScreenEffect::on_dialogButtonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Intro Screen Effect");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}
