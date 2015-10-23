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

#ifndef SOUNDEFFECTBROWSER_H
#define SOUNDEFFECTBROWSER_H

#include "ui_soundeffectbrowser.h"
#include "audiomanager.h"

class SoundEffectBrowser : public QDialog, public Ui::SoundEffectBrowser
{
	Q_OBJECT
	public:
		SoundEffectBrowser(QWidget *parent=0);
		~SoundEffectBrowser();
		void setupBrowser();
		QString getSoundEffectName();
		QString getSoundEffectLocation();
		void getSoundEffectInfo(AudioManager::SoundEffectInfo *info);
		void setSoundEffectInfo(AudioManager::SoundEffectInfo info);
	protected slots:
		void on_twSoundEffects_itemCollapsed(QTreeWidgetItem *item);
		void on_twSoundEffects_itemExpanded(QTreeWidgetItem *item);
		void on_twSoundEffects_currentItemChanged(QTreeWidgetItem *item);
		void on_twSoundEffects_itemActivated(QTreeWidgetItem *item);
		void on_bPlay_clicked();
		void on_bStop_clicked();
		void on_buttonBox_helpRequested();
		void soundEffectPositionUpdated(unsigned int soundID, unsigned int position);
		void soundEffectStopped(unsigned int soundID);
	private:
		void setupDirectoryTree(QTreeWidgetItem *directoryItem);
		float sfxPlaySpeed;
		unsigned int sfxID;
};

#endif // SOUNDEFFECTBROWSER_H
