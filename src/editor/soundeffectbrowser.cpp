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
#include "projectdata.h"
#include "soundeffectbrowser.h"
#include "spinboxsidebutton.h"

SoundEffectBrowser::SoundEffectBrowser(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	bLoopsSpinButton->setSpinBox(sbLoops);
	twSoundEffects->header()->setResizeMode(0, QHeaderView::Stretch);
	twSoundEffects->header()->setSectionHidden(1, true);
	twSoundEffects->header()->setSectionHidden(2, true);
	sfxID = 0;
	QObject::connect(ProjectData::audioManager, SIGNAL(soundEffectPositionUpdated(unsigned int, unsigned int)), this, SLOT(soundEffectPositionUpdated(unsigned int, unsigned int)));
	QObject::connect(ProjectData::audioManager, SIGNAL(soundEffectStopped(unsigned int)), this, SLOT(soundEffectStopped(unsigned int)));
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

SoundEffectBrowser::~SoundEffectBrowser()
{
	QObject::disconnect(ProjectData::audioManager, SIGNAL(soundEffectPositionUpdated(unsigned int, unsigned int)), this, SLOT(soundEffectPositionUpdated(unsigned int, unsigned int)));
	QObject::disconnect(ProjectData::audioManager, SIGNAL(soundEffectStopped(unsigned int)), this, SLOT(soundEffectStopped(unsigned int)));
}

void SoundEffectBrowser::setupBrowser()
{
	QTreeWidgetItem *item;
	Loading *loading = new Loading;
	loading->show();
	item = new QTreeWidgetItem;
	item->setText(0, "(OFF)");
	item->setText(1, "null");
	item->setText(2, "");
	item->setIcon(0, QIcon(":/icons/soundeffect.png"));
	item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
	twSoundEffects->addTopLevelItem(item);
	twSoundEffects->setCurrentItem(item);
	item->setSelected(true);
	item = new QTreeWidgetItem;
	item->setText(0, "Global Data");
	item->setText(1, "dir");
	item->setText(2, ProjectData::datadir + "/sounds");
	item->setIcon(0, QIcon(":/icons/folder.png"));
	item->setFlags(Qt::ItemIsEnabled);
	twSoundEffects->addTopLevelItem(item);
	QCoreApplication::processEvents();
	setupDirectoryTree(item);
	QCoreApplication::processEvents();
	item = new QTreeWidgetItem;
	item->setText(0, "Project Data");
	item->setText(1, "dir");
	item->setText(2, ProjectData::projectLocation + "/sounds");
	item->setIcon(0, QIcon(":/icons/folder.png"));
	item->setFlags(Qt::ItemIsEnabled);
	twSoundEffects->addTopLevelItem(item);
	QCoreApplication::processEvents();
	setupDirectoryTree(item);
	QCoreApplication::processEvents();
	AudioManager::stopAllSoundEffects();
	loading->doneLoading();
	delete loading;
}

QString SoundEffectBrowser::getSoundEffectName()
{
	return twSoundEffects->currentItem()->text(0);
}

QString SoundEffectBrowser::getSoundEffectLocation()
{
	QTreeWidgetItem *item = twSoundEffects->currentItem();
	if (item->text(1) == "sfx")
		return item->text(2);
	else
		return "";
}

void SoundEffectBrowser::getSoundEffectInfo(AudioManager::SoundEffectInfo *info)
{
	info->location = getSoundEffectLocation();
	info->volume = sldrVolume->value() / 10.0;
	info->panning = sldrBalance->value() / 5.0;
	info->speed = sldrSpeed->value() / 10.0;
	info->numLoops = sbLoops->value();
	info->retainPitch = cbRetainPitch->isChecked();
}

void SoundEffectBrowser::setSoundEffectInfo(AudioManager::SoundEffectInfo info)
{
	if (!info.location.isEmpty())
	{
		QList<QTreeWidgetItem*> itemList = twSoundEffects->findItems(info.location, Qt::MatchFixedString|Qt::MatchRecursive, 2);
		if (itemList.size() > 0)
		{
			twSoundEffects->setCurrentItem(itemList[0]);
			itemList[0]->setSelected(true);
			sldrVolume->setValue((int)(info.volume * 10.0));
			sldrBalance->setValue((int)(info.panning * 5.0));
			sldrSpeed->setValue((int)(info.speed * 10.0));
			sbLoops->setValue(info.numLoops);
			cbRetainPitch->setChecked(info.retainPitch);
		}
	}
}

void SoundEffectBrowser::on_twSoundEffects_itemCollapsed(QTreeWidgetItem *item)
{
	if (item->text(1) == "dir")
		item->setIcon(0, QIcon(":/icons/folder.png"));
}

void SoundEffectBrowser::on_twSoundEffects_itemExpanded(QTreeWidgetItem *item)
{
	if (item->text(1) == "dir")
		item->setIcon(0, QIcon(":/icons/folder_open.png"));
}

void SoundEffectBrowser::on_twSoundEffects_currentItemChanged(QTreeWidgetItem *item)
{
	if (item->text(1) == "sfx")
	{
		bPlay->setEnabled(AudioManager::isAudioEnabled());
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
	else if (item->text(1) == "null")
	{
		bPlay->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
	else
	{
		bPlay->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

void SoundEffectBrowser::on_twSoundEffects_itemActivated(QTreeWidgetItem *item)
{
	if (item->text(1) == "sfx" || item->text(1) == "null")
		bPlay->click();
}

void SoundEffectBrowser::on_bPlay_clicked()
{
	QString soundLocation = getSoundEffectLocation();
	if (!soundLocation.isEmpty())
	{
		float volume = sldrVolume->value() / 10.0;
		float panning = sldrBalance->value() / 5.0;
		float speed = sldrSpeed->value() / 10.0;
		unsigned int length, seconds, deciSeconds;
		int numLoops = sbLoops->value();
		bool retainPitch = cbRetainPitch->isChecked();
		sfxPlaySpeed = speed;
		AudioManager::stopSoundEffect(sfxID);
		sfxID = AudioManager::loadSoundEffect(soundLocation);
		AudioManager::playSoundEffect(sfxID, volume, panning, speed, retainPitch, numLoops, true);
		bStop->setEnabled(true);
		length = AudioManager::getSoundEffectLength(sfxID);
		seconds = ((unsigned int)(length / sfxPlaySpeed)) / 1000;
		deciSeconds = (((unsigned int)(length / sfxPlaySpeed)) / 100) % 10;
		lblTotalLength->setText(QString("%1.%2").arg(seconds).arg(deciSeconds));
	}
}

void SoundEffectBrowser::on_bStop_clicked()
{
	AudioManager::stopSoundEffect(sfxID);
}

void SoundEffectBrowser::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("Sound Effects");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}

void SoundEffectBrowser::soundEffectPositionUpdated(unsigned int soundID, unsigned int position)
{
	if (soundID == sfxID)
	{
		unsigned int seconds = ((unsigned int)(position / sfxPlaySpeed)) / 1000;
		unsigned int deciSeconds = (((unsigned int)(position / sfxPlaySpeed)) / 100) % 10;
		lblCurrentPosition->setText(QString("%1.%2").arg(seconds).arg(deciSeconds));
	}
}

void SoundEffectBrowser::soundEffectStopped(unsigned int soundID)
{
	if (soundID == sfxID)
	{
		lblCurrentPosition->setText("0.0");
		lblTotalLength->setText("0.0");
		bStop->setEnabled(false);
	}
}

void SoundEffectBrowser::setupDirectoryTree(QTreeWidgetItem *directoryItem)
{
	QDir directory(directoryItem->text(2));
	QFileInfoList files;
	directory.setFilter(QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable);
	directory.setNameFilters(QStringList() << "*.wav" << "*.WAV");
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
			newFileItem->setIcon(0, QIcon(":/icons/soundeffect.png"));
			newFileItem->setText(0, files[i].baseName());
			newFileItem->setText(1, "sfx");
			newFileItem->setText(2, directoryItem->text(2) + QString("/") + files[i].fileName());
			directoryItem->addChild(newFileItem);
		}
	}
}
