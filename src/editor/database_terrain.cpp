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

#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QFileInfo>
#include <QBitmap>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "audiomanager.h"
#include "clipboard.h"
#include "configurecustomgriddepth.h"
#include "database_terrain.h"
#include "imagebrowser.h"
#include "projectdata.h"
#include "projectdata_terrain.h"
#include "soundeffectbrowser.h"
#include "xmlParser.h"

Database_Terrain::Database_Terrain(QWidget *parent) : QWidget(parent)
{
	QPen pen(QBrush(Qt::white), 1.0, Qt::SolidLine, Qt::FlatCap);
	setupUi(this);
	twTerrainList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	twTerrainList->horizontalHeader()->setHidden(true);
	twTerrainList->verticalHeader()->setHidden(true);
	bDamageSpinButton->setSpinBox(nbrDamage);
	bEncMulSpinButton->setSpinBox(nbrEncMul);
	bBGHScrollSpeedSpinButton->setSpinBox(nbrBGHScrollSpeed);
	bBGVScrollSpeedSpinButton->setSpinBox(nbrBGVScrollSpeed);
	bFGHScrollSpeedSpinButton->setSpinBox(nbrFGHScrollSpeed);
	bFGVScrollSpeedSpinButton->setSpinBox(nbrFGVScrollSpeed);
	bInitiativePercent_PartySpinButton->setSpinBox(nbrInitiativePercent_Party);
	bBackAttackPercent_EnemySpinButton->setSpinBox(nbrBackAttackPercent_Enemy);
	bSurroundAttackPercent_PartySpinButton->setSpinBox(nbrSurroundAttackPercent_Party);
	bSurroundAttackPercent_EnemySpinButton->setSpinBox(nbrSurroundAttackPercent_Enemy);
	bgPreviewScene = new QGraphicsScene(0.0, 0.0, 400.0, 300.0);
	gvBGPreview->setScene(bgPreviewScene);
	bgPreview_BGImageItem = new QGraphicsPixmapItem;
	bgPreview_BGImageItem->setFlags(0x0);
	bgPreview_BGImageItem->setPos(0.0, 0.0);
	bgPreview_BGImageItem->setZValue(0.0);
	bgPreview_FGImageItem = new QGraphicsPixmapItem;
	bgPreview_FGImageItem->setFlags(0x0);
	bgPreview_FGImageItem->setPos(0.0, 0.0);
	bgPreview_FGImageItem->setZValue(2.0);
	hLine_Top = new QGraphicsLineItem;
	hLine_Top->setFlags(0x0);
	hLine_Top->setZValue(1.0);
	hLine_Top->setPen(pen);
	hLine_CenterTop = new QGraphicsLineItem;
	hLine_CenterTop->setFlags(0x0);
	hLine_CenterTop->setZValue(1.0);
	hLine_CenterTop->setPen(pen);
	hLine_CenterBottom = new QGraphicsLineItem;
	hLine_CenterBottom->setFlags(0x0);
	hLine_CenterBottom->setZValue(1.0);
	hLine_CenterBottom->setPen(pen);
	hLine_Bottom = new QGraphicsLineItem;
	hLine_Bottom->setFlags(0x0);
	hLine_Bottom->setZValue(1.0);
	hLine_Bottom->setPen(pen);
	vLine_Monsters_Left = new QGraphicsLineItem;
	vLine_Monsters_Left->setFlags(0x0);
	vLine_Monsters_Left->setZValue(1.0);
	vLine_Monsters_Left->setPen(pen);
	vLine_Monsters_Right = new QGraphicsLineItem;
	vLine_Monsters_Right->setFlags(0x0);
	vLine_Monsters_Right->setZValue(1.0);
	vLine_Monsters_Right->setPen(pen);
	vLine_Center = new QGraphicsLineItem;
	vLine_Center->setFlags(0x0);
	vLine_Center->setZValue(1.0);
	vLine_Center->setPen(pen);
	vLine_Characters_Left = new QGraphicsLineItem;
	vLine_Characters_Left->setFlags(0x0);
	vLine_Characters_Left->setZValue(1.0);
	vLine_Characters_Left->setPen(pen);
	vLine_Characters_Right = new QGraphicsLineItem;
	vLine_Characters_Right->setFlags(0x0);
	vLine_Characters_Right->setZValue(1.0);
	vLine_Characters_Right->setPen(pen);
	bgPreviewScene->addItem(bgPreview_BGImageItem);
	bgPreviewScene->addItem(bgPreview_FGImageItem);
	bgPreviewScene->addItem(hLine_Top);
	bgPreviewScene->addItem(hLine_CenterTop);
	bgPreviewScene->addItem(hLine_CenterBottom);
	bgPreviewScene->addItem(hLine_Bottom);
	bgPreviewScene->addItem(vLine_Monsters_Left);
	bgPreviewScene->addItem(vLine_Monsters_Right);
	bgPreviewScene->addItem(vLine_Center);
	bgPreviewScene->addItem(vLine_Characters_Left);
	bgPreviewScene->addItem(vLine_Characters_Right);
	terrainRef = NULL;
	ignoreEvents = true;
	bConfigureCustomGD->setEnabled(false);
}

Database_Terrain::~Database_Terrain()
{
	if (terrainRef != NULL)
	{
		delete terrainRef;
		terrainRef = NULL;
	}
}

void Database_Terrain::setupTerrain()
{
	AudioManager::SoundEffectInfo soundEffect;
	ignoreEvents = true;
	terrainRef = new ProjectData::DataReference<Terrain>(0, __FILE__, __LINE__);
	leName->setText(ProjectData::terrainNames[0]);
	nbrDamage->setValue((*terrainRef)->getDamage());
	nbrEncMul->setValue((*terrainRef)->getEncMultiplier());
	(*terrainRef)->getSoundEffectInfo(&soundEffect);
	if (soundEffect.location.isEmpty())
		leSoundEffect->setText("(OFF)");
	else
		leSoundEffect->setText(QFileInfo(soundEffect.location).baseName());
	cbSoundIfDamaged->setChecked((*terrainRef)->playSoundOnlyIfDamaged());
	if ((*terrainRef)->getBGImageLocation().isEmpty())
	{
		bgPreview_BGImageItem->setPixmap(QPixmap());
		gvBGPreview->setEnabled(false);
	}
	else
	{
		bgPreview_BGImageItem->setPixmap(QPixmap((*terrainRef)->getBGImageLocation()).scaled(400, 300));
		gvBGPreview->setEnabled(true);
	}
	cbSetBGAutoHScroll->setChecked((*terrainRef)->getBGHScrollSpeed() != 0);
	cbSetBGAutoVScroll->setChecked((*terrainRef)->getBGVScrollSpeed() != 0);
	nbrBGHScrollSpeed->setValue((*terrainRef)->getBGHScrollSpeed());
	nbrBGVScrollSpeed->setValue((*terrainRef)->getBGVScrollSpeed());
	if ((*terrainRef)->getBackgroundAssociation() == Terrain::BG_BACKGROUND)
	{
		rbBackground->setChecked(true);
		bgPreview_FGImageItem->setVisible(false);
	}
	else
	{
		rbFrame->setChecked(true);
		bgPreview_FGImageItem->setVisible(true);
		if ((*terrainRef)->getFGImageLocation().isEmpty())
			bgPreview_FGImageItem->setPixmap(QPixmap());
		else
		{
			bgPreview_FGImageItem->setPixmap(QPixmap((*terrainRef)->getFGImageLocation()).scaled(400, 300));
			gvBGPreview->setEnabled(true);
		}
		cbSetFGAutoHScroll->setChecked((*terrainRef)->getFGHScrollSpeed() != 0);
		cbSetFGAutoVScroll->setChecked((*terrainRef)->getFGVScrollSpeed() != 0);
		nbrFGHScrollSpeed->setValue((*terrainRef)->getFGHScrollSpeed());
		nbrFGVScrollSpeed->setValue((*terrainRef)->getFGVScrollSpeed());
	}
	if ((*terrainRef)->getGlobalAnimType() == Terrain::TYPE_BACKGROUND)
		rbGlobalAnimBG->setChecked(true);
	else
		rbGlobalAnimFG->setChecked(true);
	cboxGlobalAnimation->setCurrentIndex((*terrainRef)->getGlobalAnimID() + 1);
	cbInitiative_Party->setChecked((*terrainRef)->getInitPartyMod() != 0);
	cbBackAttack_Enemy->setChecked((*terrainRef)->getBackAttackEnemyMod() != 0);
	cbSurroundAttack_Party->setChecked((*terrainRef)->getSurroundAttackPartyMod() != 0);
	cbSurroundAttack_Enemy->setChecked((*terrainRef)->getSurroundAttackEnemyMod() != 0);
	nbrInitiativePercent_Party->setValue((*terrainRef)->getInitPartyMod());
	nbrBackAttackPercent_Enemy->setValue((*terrainRef)->getBackAttackEnemyMod());
	nbrSurroundAttackPercent_Party->setValue((*terrainRef)->getSurroundAttackPartyMod());
	nbrSurroundAttackPercent_Enemy->setValue((*terrainRef)->getSurroundAttackEnemyMod());
	if ((*terrainRef)->getGridDepth() == Terrain::DEPTH_SHALLOW)
		rbShallowGD->setChecked(true);
	else if ((*terrainRef)->getGridDepth() == Terrain::DEPTH_DEEP)
		rbDeepGD->setChecked(true);
	else
	{
		rbCustomGD->setChecked(true);
		bConfigureCustomGD->setEnabled(true);
	}
	if ((*terrainRef)->getSpriteDisplay() == Terrain::DISPLAY_NORMAL)
		rbNormal->setChecked(true);
	else if ((*terrainRef)->getSpriteDisplay() == Terrain::DISPLAY_OBSBOTHALF)
		rbObsBottHalf->setChecked(true);
	else if ((*terrainRef)->getSpriteDisplay() == Terrain::DISPLAY_OBSBOTTHIRD)
		rbObsBottThird->setChecked(true);
	else if ((*terrainRef)->getSpriteDisplay() == Terrain::DISPLAY_SEMITRANSPARENT)
		rbSemiTransparent->setChecked(true);
	updateTerrainList();
	updateGlobalAnimationList();
	updateGridDepthLines();
	ignoreEvents = false;
}

void Database_Terrain::shutdownTerrain()
{
	if (terrainRef != NULL)
	{
		delete terrainRef;
		terrainRef = NULL;
	}
}

void Database_Terrain::on_twTerrainList_cellClicked(int row)
{
	AudioManager::SoundEffectInfo soundEffect;
	if (terrainRef != NULL)
	{
		delete terrainRef;
		terrainRef = NULL;
	}
	terrainRef = new ProjectData::DataReference<Terrain>(row, __FILE__, __LINE__);
	leName->setText(ProjectData::terrainNames[row]);
	nbrDamage->setValue((*terrainRef)->getDamage());
	nbrEncMul->setValue((*terrainRef)->getEncMultiplier());
	(*terrainRef)->getSoundEffectInfo(&soundEffect);
	if (soundEffect.location.isEmpty())
		leSoundEffect->setText("(OFF)");
	else
		leSoundEffect->setText(QFileInfo(soundEffect.location).baseName());
	cbSoundIfDamaged->setChecked((*terrainRef)->playSoundOnlyIfDamaged());
	if ((*terrainRef)->getBGImageLocation().isEmpty())
	{
		bgPreview_BGImageItem->setPixmap(QPixmap());
		gvBGPreview->setEnabled(false);
	}
	else
	{
		bgPreview_BGImageItem->setPixmap(QPixmap((*terrainRef)->getBGImageLocation()).scaled(400, 300));
		gvBGPreview->setEnabled(true);
	}
	bgPreview_FGImageItem->setPixmap(QPixmap());
	cbSetBGAutoHScroll->setChecked((*terrainRef)->getBGHScrollSpeed() != 0);
	cbSetBGAutoVScroll->setChecked((*terrainRef)->getBGVScrollSpeed() != 0);
	nbrBGHScrollSpeed->setValue((*terrainRef)->getBGHScrollSpeed());
	nbrBGVScrollSpeed->setValue((*terrainRef)->getBGVScrollSpeed());
	if ((*terrainRef)->getBackgroundAssociation() == Terrain::BG_BACKGROUND)
	{
		rbBackground->setChecked(true);
		bgPreview_FGImageItem->setVisible(false);
	}
	else
	{
		rbFrame->setChecked(true);
		bgPreview_FGImageItem->setVisible(true);
		if ((*terrainRef)->getFGImageLocation().isEmpty())
			bgPreview_FGImageItem->setPixmap(QPixmap());
		else
		{
			bgPreview_FGImageItem->setPixmap(QPixmap((*terrainRef)->getFGImageLocation()).scaled(400, 300));
			gvBGPreview->setEnabled(true);
		}
		cbSetFGAutoHScroll->setChecked((*terrainRef)->getFGHScrollSpeed() != 0);
		cbSetFGAutoVScroll->setChecked((*terrainRef)->getFGVScrollSpeed() != 0);
		nbrFGHScrollSpeed->setValue((*terrainRef)->getFGHScrollSpeed());
		nbrFGVScrollSpeed->setValue((*terrainRef)->getFGVScrollSpeed());
	}
	cboxGlobalAnimation->setCurrentIndex((*terrainRef)->getGlobalAnimID() + 1);
	if ((*terrainRef)->getGlobalAnimType() == Terrain::TYPE_BACKGROUND)
		rbGlobalAnimBG->setChecked(true);
	else
		rbGlobalAnimFG->setChecked(true);
	cbInitiative_Party->setChecked((*terrainRef)->getInitPartyMod() != 0);
	cbBackAttack_Enemy->setChecked((*terrainRef)->getBackAttackEnemyMod() != 0);
	cbSurroundAttack_Party->setChecked((*terrainRef)->getSurroundAttackPartyMod() != 0);
	cbSurroundAttack_Enemy->setChecked((*terrainRef)->getSurroundAttackEnemyMod() != 0);
	nbrInitiativePercent_Party->setValue((*terrainRef)->getInitPartyMod());
	nbrBackAttackPercent_Enemy->setValue((*terrainRef)->getBackAttackEnemyMod());
	nbrSurroundAttackPercent_Party->setValue((*terrainRef)->getSurroundAttackPartyMod());
	nbrSurroundAttackPercent_Enemy->setValue((*terrainRef)->getSurroundAttackEnemyMod());
	if ((*terrainRef)->getGridDepth() == Terrain::DEPTH_SHALLOW)
		rbShallowGD->setChecked(true);
	else
		rbDeepGD->setChecked(true);
	if ((*terrainRef)->getSpriteDisplay() == Terrain::DISPLAY_NORMAL)
		rbNormal->setChecked(true);
	else if ((*terrainRef)->getSpriteDisplay() == Terrain::DISPLAY_OBSBOTHALF)
		rbObsBottHalf->setChecked(true);
	else if ((*terrainRef)->getSpriteDisplay() == Terrain::DISPLAY_OBSBOTTHIRD)
		rbObsBottThird->setChecked(true);
	else if ((*terrainRef)->getSpriteDisplay() == Terrain::DISPLAY_SEMITRANSPARENT)
		rbSemiTransparent->setChecked(true);
}

void Database_Terrain::on_bArraySize_clicked()
{
	int numTerrain = ProjectData::terrainNames.size();
	ArraySize *arraysize = new ArraySize(numTerrain, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numTerrain)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (terrainRef != NULL)
				{
					if (newSize >= terrainRef->getDataID())
					{
						delete terrainRef;
						terrainRef = NULL;
					}
				}
				for (int i = numTerrain - 1; i >= newSize; --i)
					ProjectData::deleteTerrain(i);
			}
		}
		else
		{
			for (int i = numTerrain; i < newSize; ++i)
			{
				Terrain *terrain = new Terrain;
				terrain->setName(QString("Terrain%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addTerrain(terrain);
				delete terrain;
			}
		}
		ignoreEvents = true;
		updateTerrainList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_Terrain::on_leName_textChanged(QString text)
{
	if (!ignoreEvents)
	{
		int currentRow = twTerrainList->currentRow();
		ignoreEvents = true;
		twTerrainList->item(currentRow, 0)->setText(QString("%1: %2").arg(currentRow, 4, 10, QChar('0')).arg(text));
		ProjectData::terrainNames[currentRow] = text;
		(*terrainRef)->setName(text);
		ignoreEvents = false;
	}
}

void Database_Terrain::on_nbrDamage_valueChanged(int value)
{
	if (!ignoreEvents)
		(*terrainRef)->setDamage(value);
}

void Database_Terrain::on_nbrEncMul_valueChanged(int value)
{
	if (!ignoreEvents)
		(*terrainRef)->setEncMultiplier(value);
}

void Database_Terrain::on_bBrowseSounds_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	(*terrainRef)->getSoundEffectInfo(&soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		(*terrainRef)->setSoundEffectInfo(soundEffect);
		leSoundEffect->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_Terrain::on_cbSoundIfDamaged_toggled(bool on)
{
	if (!ignoreEvents)
		(*terrainRef)->setPlaySoundOnlyIfDamaged(on);
}

void Database_Terrain::on_rbBackground_toggled(bool on)
{
	if (!ignoreEvents && on)
		(*terrainRef)->setBackgroundAssociation(Terrain::BG_BACKGROUND);
}

void Database_Terrain::on_rbFrame_toggled(bool on)
{
	if (!ignoreEvents && on)
		(*terrainRef)->setBackgroundAssociation(Terrain::BG_FRAME);
	frmForeground->setEnabled(on);
	bgPreview_FGImageItem->setVisible(on);
}

void Database_Terrain::on_cboxGlobalAnimation_currentIndexChanged(int index)
{
	if (!ignoreEvents)
		(*terrainRef)->setGlobalAnimID(index - 1);
}

void Database_Terrain::on_rbGlobalAnimBG_toggled(bool on)
{
	if (!ignoreEvents && on)
		(*terrainRef)->setGlobalAnimType(Terrain::TYPE_BACKGROUND);
}

void Database_Terrain::on_rbGlobalAnimFG_toggled(bool on)
{
	if (!ignoreEvents && on)
		(*terrainRef)->setGlobalAnimType(Terrain::TYPE_FOREGROUND);
}

void Database_Terrain::on_bSetBG_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		bgPreview_BGImageItem->setPixmap(QPixmap(fileLocation).scaled(400, 300));
		gvBGPreview->setEnabled(true);
		(*terrainRef)->setBGImageLocation(fileLocation);
	}
	delete imageBrowser;
}

void Database_Terrain::on_nbrBGHScrollSpeed_valueChanged(int value)
{
	if (!ignoreEvents)
		(*terrainRef)->setBGHScrollSpeed(value);
}

void Database_Terrain::on_nbrBGVScrollSpeed_valueChanged(int value)
{
	if (!ignoreEvents)
		(*terrainRef)->setBGVScrollSpeed(value);
}

void Database_Terrain::on_bSetFG_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockGIF|ImageBrowser::BlockJPEG|ImageBrowser::BlockPBM|ImageBrowser::BlockPCX|ImageBrowser::BlockPGM|ImageBrowser::BlockPNM|ImageBrowser::BlockTGA|ImageBrowser::BlockTIFF, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		QPixmap pixmap = QPixmap(fileLocation).scaled(400, 300);
		pixmap.setMask(pixmap.createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
		bgPreview_FGImageItem->setPixmap(pixmap);
		gvBGPreview->setEnabled(true);
		(*terrainRef)->setFGImageLocation(fileLocation);
	}
	delete imageBrowser;
}

void Database_Terrain::on_nbrFGHScrollSpeed_valueChanged(int value)
{
	if (!ignoreEvents)
		(*terrainRef)->setFGHScrollSpeed(value);
}

void Database_Terrain::on_nbrFGVScrollSpeed_valueChanged(int value)
{
	if (!ignoreEvents)
		(*terrainRef)->setFGVScrollSpeed(value);
}

void Database_Terrain::on_cbInitiative_Party_toggled()
{
	if (!ignoreEvents)
		nbrInitiativePercent_Party->setValue(0);
}

void Database_Terrain::on_nbrInitiativePercent_Party_valueChanged(int value)
{
	if (!ignoreEvents)
	{
		int InitiativeAmount = nbrInitiativePercent_Party->value();
		int BackAttackAmount = nbrBackAttackPercent_Enemy->value();
		int SurroundAttackAmount_Party = nbrSurroundAttackPercent_Party->value();
		int SurroundAttackAmount_Enemy = nbrSurroundAttackPercent_Enemy->value();
		int percentLeft = 100 - (InitiativeAmount + BackAttackAmount + SurroundAttackAmount_Party + SurroundAttackAmount_Enemy);
		nbrBackAttackPercent_Enemy->setMaximum(BackAttackAmount + percentLeft);
		nbrSurroundAttackPercent_Party->setMaximum(SurroundAttackAmount_Party + percentLeft);
		nbrSurroundAttackPercent_Enemy->setMaximum(SurroundAttackAmount_Enemy + percentLeft);
		(*terrainRef)->setInitPartyMod(value);
	}
}

void Database_Terrain::on_cbBackAttack_Enemy_toggled()
{
	if (!ignoreEvents)
		nbrBackAttackPercent_Enemy->setValue(0);
}

void Database_Terrain::on_nbrBackAttackPercent_Enemy_valueChanged(int value)
{
	if (!ignoreEvents)
	{
		int InitiativeAmount = nbrInitiativePercent_Party->value();
		int BackAttackAmount = nbrBackAttackPercent_Enemy->value();
		int SurroundAttackAmount_Party = nbrSurroundAttackPercent_Party->value();
		int SurroundAttackAmount_Enemy = nbrSurroundAttackPercent_Enemy->value();
		int percentLeft = 100 - (InitiativeAmount + BackAttackAmount + SurroundAttackAmount_Party + SurroundAttackAmount_Enemy);
		nbrInitiativePercent_Party->setMaximum(InitiativeAmount + percentLeft);
		nbrSurroundAttackPercent_Party->setMaximum(SurroundAttackAmount_Party + percentLeft);
		nbrSurroundAttackPercent_Enemy->setMaximum(SurroundAttackAmount_Enemy + percentLeft);
		(*terrainRef)->setBackAttackEnemyMode(value);
	}
}

void Database_Terrain::on_cbSurroundAttack_Party_toggled()
{
	if (!ignoreEvents)
		nbrSurroundAttackPercent_Party->setValue(0);
}

void Database_Terrain::on_nbrSurroundAttackPercent_Party_valueChanged(int value)
{
	if (!ignoreEvents)
	{
		int InitiativeAmount = nbrInitiativePercent_Party->value();
		int BackAttackAmount = nbrBackAttackPercent_Enemy->value();
		int SurroundAttackAmount_Party = nbrSurroundAttackPercent_Party->value();
		int SurroundAttackAmount_Enemy = nbrSurroundAttackPercent_Enemy->value();
		int percentLeft = 100 - (InitiativeAmount + BackAttackAmount + SurroundAttackAmount_Party + SurroundAttackAmount_Enemy);
		nbrInitiativePercent_Party->setMaximum(InitiativeAmount + percentLeft);
		nbrBackAttackPercent_Enemy->setMaximum(BackAttackAmount + percentLeft);
		nbrSurroundAttackPercent_Enemy->setMaximum(SurroundAttackAmount_Enemy + percentLeft);
		(*terrainRef)->setSurroundAttackPartyMod(value);
	}
}

void Database_Terrain::on_cbSurroundAttack_Enemy_toggled()
{
	if (!ignoreEvents)
		nbrSurroundAttackPercent_Enemy->setValue(0);
}

void Database_Terrain::on_nbrSurroundAttackPercent_Enemy_valueChanged(int value)
{
	if (!ignoreEvents)
	{
		int InitiativeAmount = nbrInitiativePercent_Party->value();
		int BackAttackAmount = nbrBackAttackPercent_Enemy->value();
		int SurroundAttackAmount_Party = nbrSurroundAttackPercent_Party->value();
		int SurroundAttackAmount_Enemy = nbrSurroundAttackPercent_Enemy->value();
		int percentLeft = 100 - (InitiativeAmount + BackAttackAmount + SurroundAttackAmount_Party + SurroundAttackAmount_Enemy);
		nbrInitiativePercent_Party->setMaximum(InitiativeAmount + percentLeft);
		nbrBackAttackPercent_Enemy->setMaximum(BackAttackAmount + percentLeft);
		nbrSurroundAttackPercent_Party->setMaximum(SurroundAttackAmount_Party + percentLeft);
		(*terrainRef)->setSurroundAttackEnemyMod(value);
	}
}

void Database_Terrain::on_rbShallowGD_toggled(bool on)
{
	if (!ignoreEvents && on)
	{
		(*terrainRef)->setGridDepth(Terrain::DEPTH_SHALLOW);
		updateGridDepthLines();
	}
}

void Database_Terrain::on_rbDeepGD_toggled(bool on)
{
	if (!ignoreEvents && on)
	{
		(*terrainRef)->setGridDepth(Terrain::DEPTH_DEEP);
		updateGridDepthLines();
	}
}

void Database_Terrain::on_rbCustomGD_toggled(bool on)
{
	if (!ignoreEvents && on)
		(*terrainRef)->setGridDepth(Terrain::DEPTH_CUSTOM);
	bConfigureCustomGD->setEnabled(on);
}

void Database_Terrain::on_bConfigureCustomGD_clicked()
{
	ConfigureCustomGridDepth *configureGD = new ConfigureCustomGridDepth((*terrainRef)->getGridDepthValues(), (*terrainRef)->getBGImageLocation());
	if (configureGD->exec())
	{
		configureGD->applyValues((*terrainRef)->getGridDepthValues());
		updateGridDepthLines();
	}
	delete configureGD;
}

void Database_Terrain::on_rbNormal_toggled(bool on)
{
	if (!ignoreEvents && on)
		(*terrainRef)->setSpriteDisplay(Terrain::DISPLAY_NORMAL);
}

void Database_Terrain::on_rbObsBottHalf_toggled(bool on)
{
	if (!ignoreEvents && on)
		(*terrainRef)->setSpriteDisplay(Terrain::DISPLAY_OBSBOTHALF);
}

void Database_Terrain::on_rbObsBottThird_toggled(bool on)
{
	if (!ignoreEvents && on)
		(*terrainRef)->setSpriteDisplay(Terrain::DISPLAY_OBSBOTTHIRD);
}

void Database_Terrain::on_rbSemiTransparent_toggled(bool on)
{
	if (!ignoreEvents && on)
		(*terrainRef)->setSpriteDisplay(Terrain::DISPLAY_SEMITRANSPARENT);
}

void Database_Terrain::updateTerrainList()
{
	QTableWidgetItem *item;
	int i, numterrain = ProjectData::terrainNames.size();
	int currentRow = twTerrainList->currentRow();
	twTerrainList->setRowCount(numterrain);
	for (i = 0; i < numterrain; ++i)
	{
		item = twTerrainList->item(i, 0);
		if (item)
		{
			item->setText(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::terrainNames[i]));
			item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
		}
		else
		{
			item = new QTableWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::terrainNames[i]));
			item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
			twTerrainList->setItem(i, 0, item);
		}
		twTerrainList->setRowHeight(i, 15);
	}
	if (currentRow >= twTerrainList->rowCount())
		currentRow = twTerrainList->rowCount() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::terrainNames.size() > 0)
	{
		twTerrainList->setCurrentCell(currentRow, 0);
		twTerrainList->item(currentRow, 0)->setSelected(true);
	}
}

void Database_Terrain::updateGlobalAnimationList()
{
	int numGlobalAnimations = ProjectData::globalAnimationNames.size();
	cboxGlobalAnimation->clear();
	cboxGlobalAnimation->addItem("(NONE)");
	for (int i = 0; i < numGlobalAnimations; ++i)
		cboxGlobalAnimation->addItem(ProjectData::globalAnimationNames[i]);
}

void Database_Terrain::updateGridDepthLines()
{
	QPointF p1, p2;
	int hTopLineLocation = (*terrainRef)->getGridDepth_hTopLineLocation();
	int hBottomLineLocation = (*terrainRef)->getGridDepth_hBottomLineLocation();
	int vMonsterLine_TopLocation = (*terrainRef)->getGridDepth_vMonsterLine_TopLocation();
	int vMonsterLine_BottomLocation = (*terrainRef)->getGridDepth_vMonsterLine_BottomLocation();
	int vMonsterLine_Spacing = (*terrainRef)->getGridDepth_vMonsterLine_Spacing();
	int vCharacterLine_TopLocation = (*terrainRef)->getGridDepth_vCharacterLine_TopLocation();
	int vCharacterLine_BottomLocation = (*terrainRef)->getGridDepth_vCharacterLine_BottomLocation();
	int vCharacterLine_Spacing = (*terrainRef)->getGridDepth_vCharacterLine_Spacing();
	p1.setX(0.0);
	p1.setY((float)hTopLineLocation);
	p2.setX(640.0);
	p2.setY((float)hTopLineLocation);
	p1 *= .625;
	p2 *= .625;
	hLine_Top->setLine(QLineF(p1, p2));
	p1.setX(0.0);
	p1.setY((((float)hBottomLineLocation - (float)hTopLineLocation) / 3.0) + (float)hTopLineLocation);
	p2.setX(640.0);
	p2.setY((((float)hBottomLineLocation - (float)hTopLineLocation) / 3.0) + (float)hTopLineLocation);
	p1 *= .625;
	p2 *= .625;
	hLine_CenterTop->setLine(QLineF(p1, p2));
	p1.setX(0.0);
	p1.setY(((((float)hBottomLineLocation - (float)hTopLineLocation) * 2.0) / 3.0) + (float)hTopLineLocation);
	p2.setX(640.0);
	p2.setY(((((float)hBottomLineLocation - (float)hTopLineLocation) * 2.0) / 3.0) + (float)hTopLineLocation);
	p1 *= .625;
	p2 *= .625;
	hLine_CenterBottom->setLine(QLineF(p1, p2));
	p1.setX(0.0);
	p1.setY((float)hBottomLineLocation);
	p2.setX(640.0);
	p2.setY((float)hBottomLineLocation);
	p1 *= .625;
	p2 *= .625;
	hLine_Bottom->setLine(QLineF(p1, p2));
	p1.setX((float)vMonsterLine_TopLocation - (float)vMonsterLine_Spacing);
	p1.setY((float)hTopLineLocation);
	p2.setX((float)vMonsterLine_BottomLocation - (float)vMonsterLine_Spacing);
	p2.setY((float)hBottomLineLocation);
	p1 *= .625;
	p2 *= .625;
	vLine_Monsters_Left->setLine(QLineF(p1, p2));
	p1.setX((float)vMonsterLine_TopLocation);
	p1.setY((float)hTopLineLocation);
	p2.setX((float)vMonsterLine_BottomLocation);
	p2.setY((float)hBottomLineLocation);
	p1 *= .625;
	p2 *= .625;
	vLine_Monsters_Right->setLine(QLineF(p1, p2));
	p1.setX(320.0);
	p1.setY((float)hTopLineLocation);
	p2.setX(320.0);
	p2.setY((float)hBottomLineLocation);
	p1 *= .625;
	p2 *= .625;
	vLine_Center->setLine(QLineF(p1, p2));
	p1.setX((float)vCharacterLine_TopLocation);
	p1.setY((float)hTopLineLocation);
	p2.setX((float)vCharacterLine_BottomLocation);
	p2.setY((float)hBottomLineLocation);
	p1 *= .625;
	p2 *= .625;
	vLine_Characters_Left->setLine(QLineF(p1, p2));
	p1.setX((float)vCharacterLine_TopLocation + (float)vCharacterLine_Spacing);
	p1.setY((float)hTopLineLocation);
	p2.setX((float)vCharacterLine_BottomLocation + (float)vCharacterLine_Spacing);
	p2.setY((float)hBottomLineLocation);
	p1 *= .625;
	p2 *= .625;
	vLine_Characters_Right->setLine(QLineF(p1, p2));
}
