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

#ifndef CONFIGUREINTROSCREENEFFECT_H
#define CONFIGUREINTROSCREENEFFECT_H

#include "ui_configureintroscreeneffect.h"
#include "audiomanager.h"

class ConfigureIntroScreenEffect : public QDialog, public Ui::ConfigureIntroScreenEffect
{
	Q_OBJECT
	public:
		ConfigureIntroScreenEffect(QWidget *parent=0);
		~ConfigureIntroScreenEffect();
	protected slots:
		void on_changeBGImage_bBrowseImages_clicked();
		void on_playBGM_bBrowseBGMs_clicked();
		void on_playMovie_bBrowseMovies_clicked();
		void on_playSFX_bBrowseSFX_clicked();
		void on_rbChangeBGImage_toggled(bool checked);
		void on_rbFadeBGM_toggled(bool checked);
		void on_rbFlashScreen_toggled(bool checked);
		void on_rbHideBGImage_toggled(bool checked);
		void on_rbPlayBGM_toggled(bool checked);
		void on_rbPlayGlobalAnimation_toggled(bool checked);
		void on_rbPlayMovie_toggled(bool checked);
		void on_rbPlaySoundEffect_toggled(bool checked);
		void on_rbShakeScreen_toggled(bool checked);
		void on_rbShowBGImage_toggled(bool checked);
		void on_rbTintScreen_toggled(bool checked);
		void on_rbWait_toggled(bool checked);
		void on_showBGImage_bBrowseImages_clicked();
		void on_dialogButtonBox_helpRequested();
	private:
		AudioManager::SoundEffectInfo soundEffectInfo;
		AudioManager::MusicInfo bgmInfo;
		QString bgImageLocation;
		QString movieLocation;
};

#endif // CONFIGUREINTROSCREENEFFECT_H
