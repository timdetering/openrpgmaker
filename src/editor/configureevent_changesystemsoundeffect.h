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

#ifndef CONFIGUREEVENT_CHANGESYSTEMSOUNDEFFECT_H
#define CONFIGUREEVENT_CHANGESYSTEMSOUNDEFFECT_H

#include "ui_configureevent_changesystemsoundeffect.h"
#include "audiomanager.h"

class ConfigureEvent_ChangeSystemSoundEffect : public QDialog, public Ui::ConfigureEvent_ChangeSystemSoundEffect
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeSystemSoundEffect(QWidget *parent=0);
		~ConfigureEvent_ChangeSystemSoundEffect();
		void setSoundEffectToChange(int soundEffectToChange);
		void setSoundEffect(AudioManager::SoundEffectInfo *info);
		int getSoundEffectToChange();
		void getSoundEffect(AudioManager::SoundEffectInfo *info);
	protected slots:
		void on_bBrowseSFX_clicked();
		void on_cboxSystemSoundEffect_currentIndexChanged(int index);
	private:
		AudioManager::SoundEffectInfo soundEffect;
};

#endif // CONFIGUREEVENT_CHANGESYSTEMSOUNDEFFECT_H
