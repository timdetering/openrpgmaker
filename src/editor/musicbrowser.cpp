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

#include <QString>
#include <QSpinBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
#include <QHeaderView>
#include "audiomanager.h"
#include "loading.h"
#include "musicbrowser.h"
#include "projectdata.h"

MusicBrowser::MusicBrowser(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	twMusicFiles->header()->setResizeMode(0, QHeaderView::Stretch);
	twMusicFiles->header()->setSectionHidden(1, true);
	twMusicFiles->header()->setSectionHidden(2, true);
	QObject::connect(ProjectData::audioManager, SIGNAL(musicPositionUpdated(unsigned int)), this, SLOT(musicPositionUpdated(unsigned int)));
	QObject::connect(ProjectData::audioManager, SIGNAL(musicStopped()), this, SLOT(musicStopped()));
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

MusicBrowser::~MusicBrowser()
{
	for (int i = 0; i < musicItems.size(); ++i)
		musicItems[i] = NULL;
	musicItems.clear();
	QObject::disconnect(ProjectData::audioManager, SIGNAL(musicPositionUpdated(unsigned int)), this, SLOT(musicPositionUpdated(unsigned int)));
	QObject::disconnect(ProjectData::audioManager, SIGNAL(musicStopped()), this, SLOT(musicStopped()));
}

void MusicBrowser::setupBrowser()
{
	QTreeWidgetItem *item;
	Loading *loading = new Loading;
	loading->show();
	item = new QTreeWidgetItem;
	item->setText(0, "(OFF)");
	item->setText(1, "null");
	item->setText(2, "");
	item->setIcon(0, QIcon(":/icons/music.png"));
	item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	twMusicFiles->addTopLevelItem(item);
	twMusicFiles->setCurrentItem(item);
	item->setSelected(true);
	item = new QTreeWidgetItem;
	item->setText(0, "Global Data");
	item->setText(1, "dir");
	item->setText(2, ProjectData::datadir + "/music");
	item->setIcon(0, QIcon(":/icons/folder.png"));
	item->setFlags(Qt::ItemIsEnabled);
	twMusicFiles->addTopLevelItem(item);
	QCoreApplication::processEvents();
	setupDirectoryTree(item);
	QCoreApplication::processEvents();
	item = new QTreeWidgetItem;
	item->setText(0, "Project Data");
	item->setText(1, "dir");
	item->setText(2, ProjectData::projectLocation + "/music");
	item->setIcon(0, QIcon(":/icons/folder.png"));
	item->setFlags(Qt::ItemIsEnabled);
	twMusicFiles->addTopLevelItem(item);
	QCoreApplication::processEvents();
	setupDirectoryTree(item);
	QCoreApplication::processEvents();
	if (AudioManager::isMusicPlaying())
	{
		unsigned int length, position, minutes, seconds, deciSeconds;
		musicPlaySpeed = AudioManager::getMusicPlaySpeed();
		length = (unsigned int)(AudioManager::getMusicLength() / musicPlaySpeed);
		minutes = length / 60000;
		seconds = (length / 1000) % 60;
		deciSeconds = (length / 100) % 10;
		lblTotalLength->setText(QString("%1:%2.%3").arg(minutes).arg(seconds, 2, 10, QChar('0')).arg(deciSeconds));
		position = (unsigned int)(AudioManager::getMusicPosition() / musicPlaySpeed);
		minutes = ((unsigned int)(position / musicPlaySpeed)) / 60000;
		seconds = (((unsigned int)(position / musicPlaySpeed)) / 1000) % 60;
		deciSeconds = (((unsigned int)(position / musicPlaySpeed)) / 100) % 10;
		lblCurrentPosition->setText(QString("%1:%2.%3").arg(minutes).arg(seconds, 2, 10, QChar('0')).arg(deciSeconds));
		bStop->setEnabled(true);
	}
	else
		bStop->setEnabled(false);
	loading->doneLoading();
	delete loading;
}

QString MusicBrowser::getMusicName()
{
	return twMusicFiles->currentItem()->text(0);
}

QString MusicBrowser::getMusicLocation()
{
	QTreeWidgetItem *item = twMusicFiles->currentItem();
	if (item->text(1) == "dir" || item->text(1) == "null")
		return "";
	else
		return item->text(2);
}

void MusicBrowser::getMusicInfo(AudioManager::MusicInfo *info)
{
	info->location = getMusicLocation();
	info->fadeInTime = sldrFadeInTime->value();
	info->volume = sldrVolume->value() / 10.0;
	info->panning = sldrBalance->value() / 5.0;
	info->speed = sldrSpeed->value() / 10.0;
	info->retainPitch = cbRetainPitch->isChecked();
}

void MusicBrowser::setMusicInfo(AudioManager::MusicInfo info)
{
	if (!info.location.isEmpty())
	{
		bool foundOne = false;
		cboxFileFilter->setCurrentIndex(FILTER_NONE);
		for (int i = 0; i < musicItems.size() && !foundOne; ++i)
		{
			if (musicItems[i]->text(2) == info.location)
			{
				twMusicFiles->setCurrentItem(musicItems[i]);
				musicItems[i]->setSelected(true);
			}
		}
		if (foundOne)
		{
			sldrFadeInTime->setValue(info.fadeInTime);
			sldrVolume->setValue((int)(info.volume * 10.0));
			sldrBalance->setValue((int)(info.panning * 5.0));
			sldrSpeed->setValue((int)(info.speed * 10.0));
			cbRetainPitch->setChecked(info.retainPitch);
		}
	}
}

void MusicBrowser::on_twMusicFiles_itemCollapsed(QTreeWidgetItem *item)
{
	if (item->text(1) == "dir")
		item->setIcon(0, QIcon(":/icons/folder.png"));
}

void MusicBrowser::on_twMusicFiles_itemExpanded(QTreeWidgetItem *item)
{
	if (item->text(1) == "dir")
		item->setIcon(0, QIcon(":/icons/folder_open.png"));
}

void MusicBrowser::on_twMusicFiles_currentItemChanged(QTreeWidgetItem *item)
{
	if (item->text(1) == "dir")
	{
		bPlay->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	else if (item->text(1) == "null")
	{
		bPlay->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
	else
	{
		bPlay->setEnabled(AudioManager::isAudioEnabled());
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
}

void MusicBrowser::on_twMusicFiles_itemActivated(QTreeWidgetItem *item)
{
	if (item->text(1) != "dir")
		bPlay->click();
}

void MusicBrowser::on_cboxFileFilter_currentIndexChanged(int index)
{
	for (int i = 0; i < musicItems.size(); ++i)
	{
		if (index == FILTER_NONE)
			musicItems[i]->setHidden(false);
		else if (index != musicItems[i]->text(1).toInt())
			musicItems[i]->setHidden(true);
	}
}

void MusicBrowser::on_bPlay_clicked()
{
	QString musicLocation = getMusicLocation();
	if (!musicLocation.isEmpty())
	{
		float volume = sldrVolume->value() / 10.0;
		float panning = sldrBalance->value() / 5.0;
		float speed = sldrSpeed->value() / 10.0;
		unsigned int length, minutes, seconds, deciSeconds;
		int fadeInTime = sldrFadeInTime->value();
		bool retainPitch = cbRetainPitch->isChecked();
		AudioManager::stopMusic();
		AudioManager::loadMusicFile(musicLocation);
		AudioManager::playMusic(fadeInTime, volume, panning, speed, retainPitch, true);
		bStop->setEnabled(true);
		musicPlaySpeed = speed;
		length = (unsigned int)(AudioManager::getMusicLength() / musicPlaySpeed);
		minutes = length / 60000;
		seconds = (length / 1000) % 60;
		deciSeconds = (length / 100) % 10;
		lblTotalLength->setText(QString("%1:%2.%3").arg(minutes).arg(seconds, 2, 10, QChar('0')).arg(deciSeconds));
	}
}

void MusicBrowser::on_bStop_clicked()
{
	AudioManager::stopMusic();
}

void MusicBrowser::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Music");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void MusicBrowser::musicPositionUpdated(unsigned int position)
{
	unsigned int minutes = ((unsigned int)(position / musicPlaySpeed)) / 60000;
	unsigned int seconds = (((unsigned int)(position / musicPlaySpeed)) / 1000) % 60;
	unsigned int deciSeconds = (((unsigned int)(position / musicPlaySpeed)) / 100) % 10;
	lblCurrentPosition->setText(QString("%1:%2.%3").arg(minutes).arg(seconds, 2, 10, QChar('0')).arg(deciSeconds));
}

void MusicBrowser::musicStopped()
{
	lblCurrentPosition->setText("0:00.0");
	lblTotalLength->setText("0:00.0");
	bStop->setEnabled(false);
}

void MusicBrowser::setupDirectoryTree(QTreeWidgetItem *directoryItem)
{
	QDir directory(directoryItem->text(2));
	QFileInfoList files;
	QStringList nameFilters = QStringList();
	QString temp_string;
	nameFilters << "*.aiff" << "*.AIFF";
	nameFilters << "*.flac" << "*.FLAC";
	nameFilters << "*.mid" << "*.MID" << "*.midi" << "*.MIDI";
	nameFilters << "*.mp3" << "*.MP3";
	nameFilters << "*.ogg" << "*.OGG";
	nameFilters << "*.it" << "*.IT";
	nameFilters << "*.mod" << "*.MOD";
	nameFilters << "*.s3m" << "*.S3M";
	nameFilters << "*.wav" << "*.WAV";
	directory.setFilter(QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable);
	directory.setNameFilters(nameFilters);
	directory.setSorting(QDir::Name|QDir::DirsFirst|QDir::IgnoreCase);
	files = directory.entryInfoList();
	for (int i = 0, numfiles = files.size(); i < numfiles; ++i)
	{
		QCoreApplication::processEvents();
		if (files[i].isDir())
		{
			QTreeWidgetItem *newDirItem = new QTreeWidgetItem;
			newDirItem->setIcon(0, QIcon(":/icons/folder.png"));
			newDirItem->setText(0, files[i].fileName());
			newDirItem->setText(1, "dir");
			newDirItem->setText(2, directoryItem->text(2) + QString("/") + files[i].fileName());
			newDirItem->setFlags(Qt::ItemIsEnabled);
			setupDirectoryTree(newDirItem);
			if (newDirItem->childCount() > 0)
				directoryItem->addChild(newDirItem);
		}
		else
		{
			QTreeWidgetItem *newFileItem = new QTreeWidgetItem;
			newFileItem->setIcon(0, QIcon(":/icons/music.png"));
			newFileItem->setText(0, files[i].fileName());
			temp_string = files[i].suffix().toLower();
			if (temp_string == "aiff")
				newFileItem->setText(1, "1");
			else if (temp_string == "flac")
				newFileItem->setText(1, "2");
			else if (temp_string == "mid" || temp_string == "midi")
				newFileItem->setText(1, "3");
			else if (temp_string == "mp3")
				newFileItem->setText(1, "4");
			else if (temp_string == "ogg")
				newFileItem->setText(1, "5");
			else if (temp_string == "it" || temp_string == "mod" || temp_string == "s3m")
				newFileItem->setText(1, "6");
			else if (temp_string == "wav")
				newFileItem->setText(1, "7");
			newFileItem->setText(2, directoryItem->text(2) + QString("/") + files[i].fileName());
			directoryItem->addChild(newFileItem);
			musicItems.append(newFileItem);
		}
	}
}
