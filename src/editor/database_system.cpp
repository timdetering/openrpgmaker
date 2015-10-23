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

#include <QFileInfo>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "audiomanager.h"
#include "clipboard.h"
#include "configurefont.h"
#include "configuresystemgraphics.h"
#include "database_system.h"
#include "imagebrowser.h"
#include "musicbrowser.h"
#include "projectdata.h"
#include "projectdata_font.h"
#include "projectdata_vehicle.h"
#include "soundeffectbrowser.h"
#include "xmlParser.h"

Database_System::Database_System(QWidget *parent) : QWidget(parent)
{
	QGraphicsScene *scene;
	setupUi(this);
	scene = new QGraphicsScene(0.0, 0.0, 160.0, 120.0);
	titleScreenItem = new QGraphicsPixmapItem();
	titleScreenItem->setPos(0.0, 0.0);
	titleScreenItem->setFlags(0x0);
	titleScreenItem->setZValue(0.0);
	scene->addItem(titleScreenItem);
	gvTitleScreen->setScene(scene);
	scene = new QGraphicsScene(0.0, 0.0, 160.0, 120.0);
	gameOverScreenItem = new QGraphicsPixmapItem();
	gameOverScreenItem->setPos(0.0, 0.0);
	gameOverScreenItem->setFlags(0x0);
	gameOverScreenItem->setZValue(0.0);
	scene->addItem(gameOverScreenItem);
	gvGameOverScreen->setScene(scene);
	vehicleRef = NULL;
	titleAnimRef = NULL;
	gameOverAnimRef = NULL;
	ignoreEvents = true;
	animationPreviewWidget = new GlobalAnimationPreview(this);
	animationPreviewWidget->setGeometry((width() - 640) / 2, (height() - 480) / 2, 640, 480);
	animationPreviewWidget->setAutoFillBackground(true);
	animationPreviewWidget->raise();
	animationPreviewWidget->setVisible(false);
}

Database_System::~Database_System()
{
}

void Database_System::setupSystem()
{
	AudioManager::MusicInfo bgmInfo;
	AudioManager::SoundEffectInfo sfxInfo;
	ignoreEvents = true;
	ProjectData::getSystemBGMInfo(ProjectData::BGM_TITLESCREEN, &bgmInfo);
	if (bgmInfo.location.isEmpty())
		leTitleScreenBGM->setText("(OFF)");
	else
		leTitleScreenBGM->setText(QFileInfo(bgmInfo.location).fileName());
	ProjectData::getSystemBGMInfo(ProjectData::BGM_BATTLE, &bgmInfo);
	if (bgmInfo.location.isEmpty())
		leBattleBGM->setText("(OFF)");
	else
		leBattleBGM->setText(QFileInfo(bgmInfo.location).fileName());
	ProjectData::getSystemBGMInfo(ProjectData::BGM_VICTORY, &bgmInfo);
	if (bgmInfo.location.isEmpty())
		leVictoryBGM->setText("(OFF)");
	else
		leVictoryBGM->setText(QFileInfo(bgmInfo.location).fileName());
	ProjectData::getSystemBGMInfo(ProjectData::BGM_INN, &bgmInfo);
	if (bgmInfo.location.isEmpty())
		leInnBGM->setText("(OFF)");
	else
		leInnBGM->setText(QFileInfo(bgmInfo.location).fileName());
	ProjectData::getSystemBGMInfo(ProjectData::BGM_GAMEOVER, &bgmInfo);
	if (bgmInfo.location.isEmpty())
		leGameOverBGM->setText("(OFF)");
	else
		leGameOverBGM->setText(QFileInfo(bgmInfo.location).fileName());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_CURSOR, &sfxInfo);
	if (sfxInfo.location.isEmpty())
		leCursorSFX->setText("(OFF)");
	else
		leCursorSFX->setText(QFileInfo(sfxInfo.location).baseName());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_SELECTCHOICE, &sfxInfo);
	if (sfxInfo.location.isEmpty())
		leSelectChoiceSFX->setText("(OFF)");
	else
		leSelectChoiceSFX->setText(QFileInfo(sfxInfo.location).baseName());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_BUZZER, &sfxInfo);
	if (sfxInfo.location.isEmpty())
		leBuzzerSFX->setText("(OFF)");
	else
		leBuzzerSFX->setText(QFileInfo(sfxInfo.location).baseName());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_CANCEL, &sfxInfo);
	if (sfxInfo.location.isEmpty())
		leCancelSFX->setText("(OFF)");
	else
		leCancelSFX->setText(QFileInfo(sfxInfo.location).baseName());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_BATTLETRANSITION, &sfxInfo);
	if (sfxInfo.location.isEmpty())
		leBattleTransitionSFX->setText("(OFF)");
	else
		leBattleTransitionSFX->setText(QFileInfo(sfxInfo.location).baseName());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_FLEEBATTLE, &sfxInfo);
	if (sfxInfo.location.isEmpty())
		leFleeBattleSFX->setText("(OFF)");
	else
		leFleeBattleSFX->setText(QFileInfo(sfxInfo.location).baseName());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_CHARACTERLEVELUP, &sfxInfo);
	if (sfxInfo.location.isEmpty())
		leCharacterLevelUpSFX->setText("(OFF)");
	else
		leCharacterLevelUpSFX->setText(QFileInfo(sfxInfo.location).baseName());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_EVADEATTACK, &sfxInfo);
	if (sfxInfo.location.isEmpty())
		leEvadeAttackSFX->setText("(OFF)");
	else
		leEvadeAttackSFX->setText(QFileInfo(sfxInfo.location).baseName());
	ProjectData::getSystemSFXInfo(ProjectData::SFX_ITEMUSED, &sfxInfo);
	if (sfxInfo.location.isEmpty())
		leItemUsedSFX->setText("(OFF)");
	else
		leItemUsedSFX->setText(QFileInfo(sfxInfo.location).baseName());
	cboxTeleportTransition_Erase->setCurrentIndex(ProjectData::screentransitions[ProjectData::SCREENTRANSITION_TELEPORT].erase);
	cboxTeleportTransition_Show->setCurrentIndex(ProjectData::screentransitions[ProjectData::SCREENTRANSITION_TELEPORT].show);
	cboxMenuTransition_Erase->setCurrentIndex(ProjectData::screentransitions[ProjectData::SCREENTRANSITION_MENU].erase);
	cboxMenuTransition_Show->setCurrentIndex(ProjectData::screentransitions[ProjectData::SCREENTRANSITION_MENU].show);
	cboxBattleEntryTransition_Erase->setCurrentIndex(ProjectData::screentransitions[ProjectData::SCREENTRANSITION_BATTLEENTER].erase);
	cboxBattleEntryTransition_Show->setCurrentIndex(ProjectData::screentransitions[ProjectData::SCREENTRANSITION_BATTLEENTER].show);
	cboxBattleExitTransition_Erase->setCurrentIndex(ProjectData::screentransitions[ProjectData::SCREENTRANSITION_BATTLEEXIT].erase);
	cboxBattleExitTransition_Show->setCurrentIndex(ProjectData::screentransitions[ProjectData::SCREENTRANSITION_BATTLEEXIT].show);
	firstStartingPartyMemberList.append(-1);
	secondStartingPartyMemberList.append(-1);
	thirdStartingPartyMemberList.append(-1);
	fourthStartingPartyMemberList.append(-1);
	cboxFirstPartyMember->addItem("(NONE)");
	cboxSecondPartyMember->addItem("(NONE)");
	cboxThirdPartyMember->addItem("(NONE)");
	cboxFourthPartyMember->addItem("(NONE)");
	updateFirstStartingPartyMemberList();
	updateSecondStartingPartyMemberList();
	updateThirdStartingPartyMemberList();
	updateFourthStartingPartyMemberList();
	cboxFirstPartyMember->setCurrentIndex(firstStartingPartyMemberList.indexOf(ProjectData::initialParty[0]));
	cboxSecondPartyMember->setCurrentIndex(secondStartingPartyMemberList.indexOf(ProjectData::initialParty[1]));
	cboxThirdPartyMember->setCurrentIndex(thirdStartingPartyMemberList.indexOf(ProjectData::initialParty[2]));
	cboxFourthPartyMember->setCurrentIndex(fourthStartingPartyMemberList.indexOf(ProjectData::initialParty[3]));
	updateFirstStartingPartyMemberList();
	updateSecondStartingPartyMemberList();
	updateThirdStartingPartyMemberList();
	updateFourthStartingPartyMemberList();
	if (ProjectData::titleLocation.isEmpty())
		gvTitleScreen->setEnabled(false);
	else
		titleScreenItem->setPixmap(QPixmap(ProjectData::titleLocation).scaled(160, 140));
	if (ProjectData::gameoverLocation.isEmpty())
		gvGameOverScreen->setEnabled(false);
	else
		gameOverScreenItem->setPixmap(QPixmap(ProjectData::gameoverLocation).scaled(160, 140));
	for (int i = 0; i < ProjectData::vehicleNames.size(); ++i)
		cboxSelectVehicle->addItem(ProjectData::vehicleNames[i]);
	for (int i = 0; i < ProjectData::characterSpriteNames.size(); ++i)
		cboxVehicleSprite->addItem(ProjectData::characterSpriteNames[i]);
	for (int i = 0; i < NUM_PHASES; ++i)
	{
		cboxVehiclePassingPhase->addItem(ProjectData::phaseNames[i]);
		cboxVehicleLandingPhase->addItem(ProjectData::phaseNames[i]);
	}
	ignoreEvents = false;
	if (ProjectData::vehicleNames.size() == 0)
	{
		cboxSelectVehicle->setEnabled(false);
		bDeleteVehicle->setEnabled(false);
		frmVehicleName->setEnabled(false);
		cbIsVehicleFlying->setEnabled(false);
		frmVehicleBGM->setEnabled(false);
		frmVehicleSprite->setEnabled(false);
		frmVehiclePhases->setEnabled(false);
	}
	else
		cboxSelectVehicle->setCurrentIndex(0);
	mainMenuLayoutList = ProjectData::mainMenuLayoutNames.keys();
	cboxMainMenu->clear();
	cboxMainMenu->addItems(ProjectData::mainMenuLayoutNames.values());
	cboxMainMenu->setCurrentIndex(ProjectData::mainMenuID);
	for (int i = 0; i < ProjectData::fontNames.size(); ++i)
	{
		QListWidgetItem *item = new QListWidgetItem(ProjectData::fontNames[i]);
		if (i == ProjectData::defaultFontID)
			item->setIcon(QIcon(":/icons/anchorselection.png"));
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		lwFontList->addItem(item);
	}
	if (ProjectData::fontNames.size() > 0)
		lwFontList->setCurrentRow(0);
	bAddFont->setEnabled(ProjectData::fontNames.size() < 256);
	bDeleteFont->setEnabled(ProjectData::fontNames.size() > 1);
	bEditFont->setEnabled(ProjectData::fontNames.size() > 0);
	bMakeDefaultFont->setEnabled(ProjectData::fontNames.size() > 0);
	updateSystemGraphicsPreview();
	updateGlobalAnimationLists();
}

void Database_System::shutdownSystem()
{
	ignoreEvents = true;
	firstStartingPartyMemberList.clear();
	secondStartingPartyMemberList.clear();
	thirdStartingPartyMemberList.clear();
	fourthStartingPartyMemberList.clear();
	cboxFirstPartyMember->clear();
	cboxSecondPartyMember->clear();
	cboxThirdPartyMember->clear();
	cboxFourthPartyMember->clear();
	cboxSelectVehicle->clear();
	cboxVehicleSprite->clear();
	cboxVehiclePassingPhase->clear();
	cboxVehicleLandingPhase->clear();
	cboxMainMenu->clear();
	lwFontList->clear();
	ignoreEvents = false;
	if (vehicleRef != NULL)
	{
		delete vehicleRef;
		vehicleRef = NULL;
	}
	if (titleAnimRef != NULL)
	{
		delete titleAnimRef;
		titleAnimRef = NULL;
	}
	if (gameOverAnimRef != NULL)
	{
		delete gameOverAnimRef;
		gameOverAnimRef = NULL;
	}
}

void Database_System::on_cboxSelectVehicle_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		AudioManager::MusicInfo bgmInfo;
		if (vehicleRef != NULL)
		{
			delete vehicleRef;
			vehicleRef = NULL;
		}
		vehicleRef = new ProjectData::DataReference<Vehicle>(index, __FILE__, __LINE__);
		leVehicleName->setText((*vehicleRef)->getName());
		cbIsVehicleFlying->setChecked((*vehicleRef)->isFlying());
		cboxVehicleSprite->setCurrentIndex((*vehicleRef)->getSpriteID());
		cboxVehiclePassingPhase->setCurrentIndex((*vehicleRef)->getPassingPhase());
		cboxVehicleLandingPhase->setCurrentIndex((*vehicleRef)->getLandingPhase());
		(*vehicleRef)->getBGMInfo(&bgmInfo);
		if (bgmInfo.location.isEmpty())
			leVehicleBGM->setText("(OFF)");
		else
			leVehicleBGM->setText(QFileInfo(bgmInfo.location).fileName());
	}
}

void Database_System::on_bDeleteVehicle_clicked()
{
	int vehicleID = cboxSelectVehicle->currentIndex();
	ProjectData::deleteVehicle(vehicleID);
	cboxSelectVehicle->removeItem(vehicleID);
	if (ProjectData::vehicleNames.size() == 0)
	{
		cboxSelectVehicle->setEnabled(false);
		bDeleteVehicle->setEnabled(false);
		frmVehicleName->setEnabled(false);
		cbIsVehicleFlying->setEnabled(false);
		frmVehicleBGM->setEnabled(false);
		frmVehicleSprite->setEnabled(false);
	}
	bAddVehicle->setEnabled(true);
}

void Database_System::on_bAddVehicle_clicked()
{
	if (ProjectData::vehicleNames.size() < 8)
	{
		Vehicle *vehicle = new Vehicle;
		vehicle->setName(QString("Vehicle%1").arg(ProjectData::vehicleNames.size(), 2, 10, QChar('0')));
		ProjectData::addVehicle(vehicle);
		cboxSelectVehicle->addItem(vehicle->getName());
		cboxSelectVehicle->setCurrentIndex(ProjectData::vehicleNames.size() - 1);
		if (ProjectData::vehicleNames.size() == 8)
			bAddVehicle->setEnabled(false);
		delete vehicle;
	}
	cboxSelectVehicle->setEnabled(true);
	bDeleteVehicle->setEnabled(true);
	frmVehicleName->setEnabled(true);
	cbIsVehicleFlying->setEnabled(true);
	frmVehicleBGM->setEnabled(true);
	frmVehicleSprite->setEnabled(true);
}

void Database_System::on_leVehicleName_textChanged(QString text)
{
	(*vehicleRef)->setName(text);
	cboxSelectVehicle->setItemText(cboxSelectVehicle->currentIndex(), text);
}

void Database_System::on_cbIsVehicleFlying_toggled(bool on)
{
	(*vehicleRef)->setIsFlying(on);
}

void Database_System::on_bBrowseVehicleBGM_clicked()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	AudioManager::MusicInfo bgmInfo;
	musicBrowser->setupBrowser();
	(*vehicleRef)->getBGMInfo(&bgmInfo);
	musicBrowser->setMusicInfo(bgmInfo);
	if (musicBrowser->exec())
	{
		musicBrowser->getMusicInfo(&bgmInfo);
		(*vehicleRef)->setBGMInfo(bgmInfo);
		leVehicleBGM->setText(musicBrowser->getMusicName());
	}
	delete musicBrowser;
}

void Database_System::on_cboxVehicleSprite_currentIndexChanged(int index)
{
	if (vehicleRef != NULL)
		(*vehicleRef)->setSpriteID(index);
}

void Database_System::on_cboxVehiclePassingPhase_currentIndexChanged(int index)
{
	if (vehicleRef != NULL)
		(*vehicleRef)->setPassingPhase(index);
}

void Database_System::on_cboxVehicleLandingPhase_currentIndexChanged(int index)
{
	if (vehicleRef != NULL)
		(*vehicleRef)->setLandingPhase(index);
}

void Database_System::on_bSetTitleScreenImage_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		gvTitleScreen->setEnabled(true);
		titleScreenItem->setPixmap(QPixmap(fileLocation).scaled(160, 120));
		ProjectData::titleLocation = fileLocation;
		ProjectData::saved = false;
	}
	delete imageBrowser;
}

void Database_System::on_cboxTitleScreenAnimation_currentIndexChanged(int index)
{
	int animationID = index - 1;
	if (titleAnimRef != NULL)
	{
		delete titleAnimRef;
		titleAnimRef = NULL;
	}
	if (animationID >= 0 && animationID < ProjectData::globalAnimationNames.size())
		titleAnimRef = new ProjectData::DataReference<GlobalAnimation>(animationID, __FILE__, __LINE__);
}

void Database_System::on_bPreviewTitleScreen_clicked()
{
	QPixmap bgPixmap = QPixmap(ProjectData::titleLocation);
	animationPreviewWidget->setVisible(true);
	wBottomWidget->setEnabled(false);
	wTopWidget->setEnabled(false);
	animationPreviewWidget->playAnimation(titleAnimRef->getDataID(), bgPixmap);
	while (!animationPreviewWidget->done)
		QCoreApplication::processEvents();
	wBottomWidget->setEnabled(true);
	wTopWidget->setEnabled(true);
	animationPreviewWidget->setVisible(false);
}

void Database_System::on_bSetGameOverScreenImage_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		gvGameOverScreen->setEnabled(true);
		gameOverScreenItem->setPixmap(QPixmap(fileLocation).scaled(160, 120));
		ProjectData::gameoverLocation = fileLocation;
		ProjectData::saved = false;
	}
	delete imageBrowser;
}

void Database_System::on_cboxGameOverScreenAnimation_currentIndexChanged(int index)
{
	int animationID = index - 1;
	if (gameOverAnimRef != NULL)
	{
		delete gameOverAnimRef;
		gameOverAnimRef = NULL;
	}
	if (animationID >= 0 && animationID < ProjectData::globalAnimationNames.size())
		gameOverAnimRef = new ProjectData::DataReference<GlobalAnimation>(animationID, __FILE__, __LINE__);
}

void Database_System::on_bPreviewGameOverScreen_clicked()
{
	QPixmap bgPixmap = QPixmap(ProjectData::gameoverLocation);
	animationPreviewWidget->setVisible(true);
	wBottomWidget->setEnabled(false);
	wTopWidget->setEnabled(false);
	animationPreviewWidget->playAnimation(gameOverAnimRef->getDataID(), bgPixmap);
	while (!animationPreviewWidget->done)
		QCoreApplication::processEvents();
	wBottomWidget->setEnabled(true);
	wTopWidget->setEnabled(true);
	animationPreviewWidget->setVisible(false);
}

void Database_System::on_bSetSystemGraphics_clicked()
{
	ConfigureSystemGraphics *configSystemGraphics = new ConfigureSystemGraphics;
	configSystemGraphics->exec();
	updateSystemGraphicsPreview();
}

void Database_System::on_cboxFirstPartyMember_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateSecondStartingPartyMemberList();
		updateThirdStartingPartyMemberList();
		updateFourthStartingPartyMemberList();
		if (index == 0)
			updateFirstStartingPartyMemberList();
		ProjectData::initialParty[0] = firstStartingPartyMemberList[index];
		ProjectData::saved = false;
		ignoreEvents = false;
	}
}

void Database_System::on_cboxSecondPartyMember_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateFirstStartingPartyMemberList();
		updateThirdStartingPartyMemberList();
		updateFourthStartingPartyMemberList();
		if (index == 0)
		{
			updateSecondStartingPartyMemberList();
			updateFirstStartingPartyMemberList();
		}
		ProjectData::initialParty[1] = secondStartingPartyMemberList[index];
		ProjectData::saved = false;
		ignoreEvents = false;
	}
}

void Database_System::on_cboxThirdPartyMember_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateFirstStartingPartyMemberList();
		updateSecondStartingPartyMemberList();
		updateFourthStartingPartyMemberList();
		if (index == 0)
		{
			updateThirdStartingPartyMemberList();
			updateSecondStartingPartyMemberList();
			updateFirstStartingPartyMemberList();
		}
		ProjectData::initialParty[2] = thirdStartingPartyMemberList[index];
		ProjectData::saved = false;
		ignoreEvents = false;
	}
}

void Database_System::on_cboxFourthPartyMember_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateFirstStartingPartyMemberList();
		updateSecondStartingPartyMemberList();
		updateThirdStartingPartyMemberList();
		if (index == 0)
		{
			updateFourthStartingPartyMemberList();
			updateThirdStartingPartyMemberList();
			updateSecondStartingPartyMemberList();
			updateFirstStartingPartyMemberList();
		}
		ProjectData::initialParty[3] = fourthStartingPartyMemberList[index];
		ProjectData::saved = false;
		ignoreEvents = false;
	}
}

void Database_System::on_bBrowseTitleScreenBGM_clicked()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	AudioManager::MusicInfo bgmInfo;
	musicBrowser->setupBrowser();
	ProjectData::getSystemBGMInfo(ProjectData::BGM_TITLESCREEN, &bgmInfo);
	musicBrowser->setMusicInfo(bgmInfo);
	if (musicBrowser->exec())
	{
		musicBrowser->getMusicInfo(&bgmInfo);
		ProjectData::setSystemBGMInfo(ProjectData::BGM_TITLESCREEN, bgmInfo);
		ProjectData::saved = false;
		leTitleScreenBGM->setText(musicBrowser->getMusicName());
	}
	delete musicBrowser;
}

void Database_System::on_bBrowseBattleBGM_clicked()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	AudioManager::MusicInfo bgmInfo;
	musicBrowser->setupBrowser();
	ProjectData::getSystemBGMInfo(ProjectData::BGM_BATTLE, &bgmInfo);
	musicBrowser->setMusicInfo(bgmInfo);
	if (musicBrowser->exec())
	{
		musicBrowser->getMusicInfo(&bgmInfo);
		ProjectData::setSystemBGMInfo(ProjectData::BGM_BATTLE, bgmInfo);
		ProjectData::saved = false;
		leBattleBGM->setText(musicBrowser->getMusicName());
	}
	delete musicBrowser;
}

void Database_System::on_bBrowseVictoryBGM_clicked()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	AudioManager::MusicInfo bgmInfo;
	musicBrowser->setupBrowser();
	ProjectData::getSystemBGMInfo(ProjectData::BGM_VICTORY, &bgmInfo);
	musicBrowser->setMusicInfo(bgmInfo);
	if (musicBrowser->exec())
	{
		musicBrowser->getMusicInfo(&bgmInfo);
		ProjectData::setSystemBGMInfo(ProjectData::BGM_VICTORY, bgmInfo);
		ProjectData::saved = false;
		leVictoryBGM->setText(musicBrowser->getMusicName());
	}
	delete musicBrowser;
}

void Database_System::on_bBrowseInnBGM_clicked()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	AudioManager::MusicInfo bgmInfo;
	musicBrowser->setupBrowser();
	ProjectData::getSystemBGMInfo(ProjectData::BGM_INN, &bgmInfo);
	musicBrowser->setMusicInfo(bgmInfo);
	if (musicBrowser->exec())
	{
		musicBrowser->getMusicInfo(&bgmInfo);
		ProjectData::setSystemBGMInfo(ProjectData::BGM_INN, bgmInfo);
		ProjectData::saved = false;
		leInnBGM->setText(musicBrowser->getMusicName());
	}
	delete musicBrowser;
}

void Database_System::on_bBrowseGameOverBGM_clicked()
{
	MusicBrowser *musicBrowser = new MusicBrowser;
	AudioManager::MusicInfo bgmInfo;
	musicBrowser->setupBrowser();
	ProjectData::getSystemBGMInfo(ProjectData::BGM_GAMEOVER, &bgmInfo);
	musicBrowser->setMusicInfo(bgmInfo);
	if (musicBrowser->exec())
	{
		musicBrowser->getMusicInfo(&bgmInfo);
		ProjectData::setSystemBGMInfo(ProjectData::BGM_GAMEOVER, bgmInfo);
		ProjectData::saved = false;
		leGameOverBGM->setText(musicBrowser->getMusicName());
	}
	delete musicBrowser;
}

void Database_System::on_bBrowseCursorSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	ProjectData::getSystemSFXInfo(ProjectData::SFX_CURSOR, &soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		ProjectData::setSystemSFXInfo(ProjectData::SFX_CURSOR, soundEffect);
		ProjectData::saved = false;
		leCursorSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_System::on_bBrowseSelectChoiceSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	ProjectData::getSystemSFXInfo(ProjectData::SFX_SELECTCHOICE, &soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		ProjectData::setSystemSFXInfo(ProjectData::SFX_SELECTCHOICE, soundEffect);
		ProjectData::saved = false;
		leSelectChoiceSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_System::on_bBrowseBuzzerSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	ProjectData::getSystemSFXInfo(ProjectData::SFX_BUZZER, &soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		ProjectData::setSystemSFXInfo(ProjectData::SFX_BUZZER, soundEffect);
		ProjectData::saved = false;
		leBuzzerSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_System::on_bBrowseCancelSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	ProjectData::getSystemSFXInfo(ProjectData::SFX_CANCEL, &soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		ProjectData::setSystemSFXInfo(ProjectData::SFX_CANCEL, soundEffect);
		ProjectData::saved = false;
		leCancelSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_System::on_bBrowseBattleTransitionSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	ProjectData::getSystemSFXInfo(ProjectData::SFX_BATTLETRANSITION, &soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		ProjectData::setSystemSFXInfo(ProjectData::SFX_BATTLETRANSITION, soundEffect);
		ProjectData::saved = false;
		leBattleTransitionSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_System::on_bBrowseFleeBattleSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	ProjectData::getSystemSFXInfo(ProjectData::SFX_FLEEBATTLE, &soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		ProjectData::setSystemSFXInfo(ProjectData::SFX_FLEEBATTLE, soundEffect);
		ProjectData::saved = false;
		leFleeBattleSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_System::on_bBrowseCharacterLevelUpSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	ProjectData::getSystemSFXInfo(ProjectData::SFX_CHARACTERLEVELUP, &soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		ProjectData::setSystemSFXInfo(ProjectData::SFX_CHARACTERLEVELUP, soundEffect);
		ProjectData::saved = false;
		leCharacterLevelUpSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_System::on_bBrowseEvadeAttackSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	ProjectData::getSystemSFXInfo(ProjectData::SFX_EVADEATTACK, &soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		ProjectData::setSystemSFXInfo(ProjectData::SFX_EVADEATTACK, soundEffect);
		ProjectData::saved = false;
		leEvadeAttackSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_System::on_bBrowseItemUsedSFX_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	AudioManager::SoundEffectInfo soundEffect;
	soundBrowser->setupBrowser();
	ProjectData::getSystemSFXInfo(ProjectData::SFX_ITEMUSED, &soundEffect);
	soundBrowser->setSoundEffectInfo(soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&soundEffect);
		ProjectData::setSystemSFXInfo(ProjectData::SFX_ITEMUSED, soundEffect);
		ProjectData::saved = false;
		leItemUsedSFX->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_System::on_lwFontList_itemSelectionChanged()
{
}

void Database_System::on_bAddFont_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	int blockedTypes = 0;
	blockedTypes += ImageBrowser::BlockBMP;
	blockedTypes += ImageBrowser::BlockGIF;
	blockedTypes += ImageBrowser::BlockJPEG;
	blockedTypes += ImageBrowser::BlockPBM;
	blockedTypes += ImageBrowser::BlockPCX;
	blockedTypes += ImageBrowser::BlockPGM;
	blockedTypes += ImageBrowser::BlockPNM;
	blockedTypes += ImageBrowser::BlockPPM;
	blockedTypes += ImageBrowser::BlockTGA;
	blockedTypes += ImageBrowser::BlockTIFF;
	blockedTypes += ImageBrowser::BlockXPM;
	imageBrowser->setupBrowser("/fonts", blockedTypes);
	if (imageBrowser->exec())
	{
		Font *font = new Font;
		ConfigureFont *configureFont;
		QString imageLocation = imageBrowser->getFileLocation();
		QImage image = QImage(imageLocation);
		int fontID = ProjectData::fontNames.size();
		bool configured = false;
		font->setName(QString("Font%1").arg(fontID + 1));
		font->setData(imageLocation, "", image.width() / 26, 32, 8, 24, 2, 2);
		ProjectData::addFont(font);
		delete font;
		configureFont = new ConfigureFont(fontID, this);
		if (configureFont->exec())
		{
			QListWidgetItem *item;
			item = new QListWidgetItem(ProjectData::fontNames[fontID]);
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			lwFontList->addItem(item);
			configured = true;
		}
		delete configureFont;
		if (!configured)
			ProjectData::deleteFont(fontID);
		if (ProjectData::fontNames.size() == 256)
			bAddFont->setEnabled(false);
	}
	delete imageBrowser;
}

void Database_System::on_bDeleteFont_clicked()
{
	ProjectData::deleteFont(lwFontList->currentRow());
	delete lwFontList->takeItem(lwFontList->currentRow());
	lwFontList->item(ProjectData::defaultFontID)->setIcon(QIcon(":/icons/anchorselection.png"));
	if (ProjectData::fontNames.size() == 1)
		bDeleteFont->setEnabled(false);
	bAddFont->setEnabled(true);
}

void Database_System::on_bEditFont_clicked()
{
	ConfigureFont *configureFont = new ConfigureFont(lwFontList->currentRow(), this);
	if (configureFont->exec())
		lwFontList->currentItem()->setText(ProjectData::fontNames[lwFontList->currentRow()]);
	delete configureFont;
}

void Database_System::on_bMakeDefaultFont_clicked()
{
	if (ProjectData::defaultFontID >= 0 && ProjectData::defaultFontID < ProjectData::fontNames.size())
	{
		lwFontList->item(ProjectData::defaultFontID)->setIcon(QIcon());
		ProjectData::defaultFontID = lwFontList->currentRow();
		lwFontList->currentItem()->setIcon(QIcon(":/icons/anchorselection.png"));
	}
}

void Database_System::on_bConfigureButtonMapping_clicked()
{
	QMessageBox::information(0, "Coming Soon", "This feature is coming soon, once the engine is underway.");
}

void Database_System::on_bConfigureIntroScreens_clicked()
{
}

void Database_System::on_cboxTeleportTransition_Erase_currentIndexChanged(int index)
{
	ProjectData::screentransitions[ProjectData::SCREENTRANSITION_TELEPORT].erase = index;
	ProjectData::saved = false;
}

void Database_System::on_cboxTeleportTransition_Show_currentIndexChanged(int index)
{
	ProjectData::screentransitions[ProjectData::SCREENTRANSITION_TELEPORT].show = index;
	ProjectData::saved = false;
}

void Database_System::on_cboxMenuTransition_Erase_currentIndexChanged(int index)
{
	ProjectData::screentransitions[ProjectData::SCREENTRANSITION_MENU].erase = index;
	ProjectData::saved = false;
}

void Database_System::on_cboxMenuTransition_Show_currentIndexChanged(int index)
{
	ProjectData::screentransitions[ProjectData::SCREENTRANSITION_MENU].show = index;
	ProjectData::saved = false;
}

void Database_System::on_cboxBattleEntryTransition_Erase_currentIndexChanged(int index)
{
	ProjectData::screentransitions[ProjectData::SCREENTRANSITION_BATTLEENTER].erase = index;
	ProjectData::saved = false;
}

void Database_System::on_cboxBattleEntryTransition_Show_currentIndexChanged(int index)
{
	ProjectData::screentransitions[ProjectData::SCREENTRANSITION_BATTLEENTER].show = index;
	ProjectData::saved = false;
}

void Database_System::on_cboxBattleExitTransition_Erase_currentIndexChanged(int index)
{
	ProjectData::screentransitions[ProjectData::SCREENTRANSITION_BATTLEEXIT].erase = index;
	ProjectData::saved = false;
}

void Database_System::on_cboxBattleExitTransition_Show_currentIndexChanged(int index)
{
	ProjectData::screentransitions[ProjectData::SCREENTRANSITION_BATTLEEXIT].show = index;
	ProjectData::saved = false;
}

void Database_System::on_cboxMainMenu_currentIndexChanged(int index)
{
	if (index >= 0 && index < mainMenuLayoutList.size())
		ProjectData::mainMenuID = mainMenuLayoutList[index];
}

void Database_System::updateFirstStartingPartyMemberList()
{
	int currentChar = firstStartingPartyMemberList[cboxFirstPartyMember->currentIndex()];
	int char2 = secondStartingPartyMemberList[cboxSecondPartyMember->currentIndex()];
	int char3 = thirdStartingPartyMemberList[cboxThirdPartyMember->currentIndex()];
	int char4 = fourthStartingPartyMemberList[cboxFourthPartyMember->currentIndex()];
	firstStartingPartyMemberList.clear();
	cboxFirstPartyMember->clear();
	firstStartingPartyMemberList.append(-1);
	cboxFirstPartyMember->addItem("(NONE)");
	for (int i = 0; i < ProjectData::characterNames.size(); ++i)
	{
		if (i != char2 && i != char3 && i != char4)
		{
			firstStartingPartyMemberList.append(i);
			cboxFirstPartyMember->addItem(ProjectData::characterNames[i]);
		}
	}
	cboxFirstPartyMember->setCurrentIndex(firstStartingPartyMemberList.indexOf(currentChar));
}

void Database_System::updateSecondStartingPartyMemberList()
{
	int currentChar = secondStartingPartyMemberList[cboxSecondPartyMember->currentIndex()];
	int char1 = firstStartingPartyMemberList[cboxFirstPartyMember->currentIndex()];
	int char3 = thirdStartingPartyMemberList[cboxThirdPartyMember->currentIndex()];
	int char4 = fourthStartingPartyMemberList[cboxFourthPartyMember->currentIndex()];
	secondStartingPartyMemberList.clear();
	cboxSecondPartyMember->clear();
	secondStartingPartyMemberList.append(-1);
	cboxSecondPartyMember->addItem("(NONE)");
	for (int i = 0; i < ProjectData::characterNames.size(); ++i)
	{
		if (i != char1 && i != char3 && i != char4)
		{
			secondStartingPartyMemberList.append(i);
			cboxSecondPartyMember->addItem(ProjectData::characterNames[i]);
		}
	}
	if (char1 == -1)
	{
		currentChar = -1;
		cboxSecondPartyMember->setEnabled(false);
	}
	else
		cboxSecondPartyMember->setEnabled(true);
	cboxSecondPartyMember->setCurrentIndex(secondStartingPartyMemberList.indexOf(currentChar));
}

void Database_System::updateThirdStartingPartyMemberList()
{
	int currentChar = thirdStartingPartyMemberList[cboxThirdPartyMember->currentIndex()];
	int char1 = firstStartingPartyMemberList[cboxFirstPartyMember->currentIndex()];
	int char2 = secondStartingPartyMemberList[cboxSecondPartyMember->currentIndex()];
	int char4 = fourthStartingPartyMemberList[cboxFourthPartyMember->currentIndex()];
	thirdStartingPartyMemberList.clear();
	cboxThirdPartyMember->clear();
	thirdStartingPartyMemberList.append(-1);
	cboxThirdPartyMember->addItem("(NONE)");
	for (int i = 0; i < ProjectData::characterNames.size(); ++i)
	{
		if (i != char1 && i != char2 && i != char4)
		{
			thirdStartingPartyMemberList.append(i);
			cboxThirdPartyMember->addItem(ProjectData::characterNames[i]);
		}
	}
	if (char2 == -1)
	{
		currentChar = -1;
		cboxThirdPartyMember->setEnabled(false);
	}
	else
		cboxThirdPartyMember->setEnabled(true);
	cboxThirdPartyMember->setCurrentIndex(thirdStartingPartyMemberList.indexOf(currentChar));
}

void Database_System::updateFourthStartingPartyMemberList()
{
	int currentChar = fourthStartingPartyMemberList[cboxFourthPartyMember->currentIndex()];
	int char1 = firstStartingPartyMemberList[cboxFirstPartyMember->currentIndex()];
	int char2 = secondStartingPartyMemberList[cboxSecondPartyMember->currentIndex()];
	int char3 = thirdStartingPartyMemberList[cboxThirdPartyMember->currentIndex()];
	fourthStartingPartyMemberList.clear();
	cboxFourthPartyMember->clear();
	fourthStartingPartyMemberList.append(-1);
	cboxFourthPartyMember->addItem("(NONE)");
	for (int i = 0; i < ProjectData::characterNames.size(); ++i)
	{
		if (i != char1 && i != char2 && i != char3)
		{
			fourthStartingPartyMemberList.append(i);
			cboxFourthPartyMember->addItem(ProjectData::characterNames[i]);
		}
	}
	if (char3 == -1)
	{
		currentChar = -1;
		cboxFourthPartyMember->setEnabled(false);
	}
	else
		cboxFourthPartyMember->setEnabled(true);
	cboxFourthPartyMember->setCurrentIndex(fourthStartingPartyMemberList.indexOf(currentChar));
}

void Database_System::updateSystemGraphicsPreview()
{
	ProjectData::SystemGraphics *systemGraphics = &ProjectData::systemGraphics;
	QPixmap pixmap(320, 96);
	QImage image;
	QPainter painter;
	painter.begin(&pixmap);
	painter.fillRect(0, 0, 320, 96, QColor(255, 0, 255));
	image = QImage(systemGraphics->frameBGLocation);
	painter.drawImage(0, 0, image);
	image = QImage(systemGraphics->frameBorderLocation);
	painter.drawImage(64, 0, image);
	image = QImage(systemGraphics->scrollArrowsLocation);
	painter.drawImage(80, 16, image);
	image = QImage(systemGraphics->selectionBorderLocation);
	painter.drawImage(128, 0, image);
	image = QImage(systemGraphics->equipmentIndicatorsLocation);
	painter.drawImage(256, 0, image);
	image = QImage(systemGraphics->menuBGLocation);
	painter.drawImage(0, 64, image);
	image = QImage(systemGraphics->fontShadowLocation);
	painter.drawImage(32, 64, image);
	image = QImage(systemGraphics->timerNumbersLocation);
	painter.drawImage(64, 64, image);
	image = QImage(systemGraphics->grappleArmLocation).copy(0, 96, 32, 32);
	painter.drawImage(256, 64, image);
	image = QImage(systemGraphics->grappleHookLocation).copy(0, 96, 32, 32);
	painter.drawImage(288, 64, image);
	painter.end();
	lblSystemGraphicsPreview->setPixmap(pixmap);
}

void Database_System::updateGlobalAnimationLists()
{
	cboxTitleScreenAnimation->clear();
	cboxGameOverScreenAnimation->clear();
	cboxTitleScreenAnimation->addItem("(NONE)");
	cboxGameOverScreenAnimation->addItem("(NONE)");
	cboxTitleScreenAnimation->addItems(ProjectData::globalAnimationNames);
	cboxGameOverScreenAnimation->addItems(ProjectData::globalAnimationNames);
}
