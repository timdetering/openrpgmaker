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

#ifndef MUSICBROWSER_H
#define MUSICBROWSER_H

#include "ui_musicbrowser.h"
#include "audiomanager.h"

class MusicBrowser : public QDialog, public Ui::MusicBrowser
{
	Q_OBJECT
	public:
		enum Filter
		{
			FILTER_NONE=0,
			FILTER_AIFF=1,
			FILTER_FLAC=2,
			FILTER_MIDI=3,
			FILTER_MP3=4,
			FILTER_OGG=5,
			FILTER_TRACKER=6,
			FILTER_WAV=7
		};
		MusicBrowser(QWidget *parent=0);
		~MusicBrowser();
		void setupBrowser();
		QString getMusicName();
		QString getMusicLocation();
		void getMusicInfo(AudioManager::MusicInfo *info);
		void setMusicInfo(AudioManager::MusicInfo info);
	protected slots:
		void on_twMusicFiles_itemCollapsed(QTreeWidgetItem *item);
		void on_twMusicFiles_itemExpanded(QTreeWidgetItem *item);
		void on_twMusicFiles_currentItemChanged(QTreeWidgetItem *item);
		void on_twMusicFiles_itemActivated(QTreeWidgetItem *item);
		void on_cboxFileFilter_currentIndexChanged(int index);
		void on_bPlay_clicked();
		void on_bStop_clicked();
		void on_buttonBox_helpRequested();
		void musicPositionUpdated(unsigned int position);
		void musicStopped();
	private:
		void setupDirectoryTree(QTreeWidgetItem *directoryItem);
		QList<QTreeWidgetItem*> musicItems;
		float musicPlaySpeed;
};

#endif // MUSICBROWSER_H
