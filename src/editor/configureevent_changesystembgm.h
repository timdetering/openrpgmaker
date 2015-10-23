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

#ifndef CONFIGUREEVENT_CHANGESYSTEMBGM_H
#define CONFIGUREEVENT_CHANGESYSTEMBGM_H

#include "ui_configureevent_changesystembgm.h"
#include "audiomanager.h"

class ConfigureEvent_ChangeSystemBGM : public QDialog, public Ui::ConfigureEvent_ChangeSystemBGM
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeSystemBGM(QWidget *parent=0);
		~ConfigureEvent_ChangeSystemBGM();
		void setBGMToChange(int bgm);
		int getBGMToChange();
		void setBGM(const AudioManager::MusicInfo *bgm);
		void getBGM(AudioManager::MusicInfo *bgm);
	protected slots:
		void on_bBrowseBGM_clicked();
		void on_cboxSystemBGM_currentIndexChanged(int index);
	private:
		AudioManager::MusicInfo currentBGM;
};

#endif // CONFIGUREEVENT_CHANGESYSTEMBGM_H
