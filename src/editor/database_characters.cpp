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

#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QFileInfo>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "characterclassexperiencecurveeditor.h"
#include "characterclassstateditor.h"
#include "clipboard.h"
#include "configureskilllearned.h"
#include "database_characters.h"
#include "imagebrowser.h"
#include "projectdata.h"
#include "projectdata_character.h"
#include "projectdata_charactersprite.h"
#include "projectdata_item.h"
#include "setbattlecommands.h"
#include "xmlParser.h"

Database_Characters::Database_Characters(QWidget *parent) : QWidget(parent)
{
	ignoreEvents = true;
	setupUi(this);
	animationTimer = new QTimer(this);
	QObject::connect(animationTimer, SIGNAL(timeout()), this, SLOT(animateSprite()));
	bMinLevelSpinButton->setSpinBox(sbMinLevel);
	bMaxLevelSpinButton->setSpinBox(sbMaxLevel);
	bSpriteOpacitySideButton->setSpinBox(sbSpriteOpacity);
	bCriticalHitRateSpinButton->setDoubleSpinBox(dsbCriticalHitRate);
	bCriticalHitMultiplierSpinButton->setDoubleSpinBox(dsbCriticalHitMultiplier);
	twSkillList->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
	twSkillList->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	twSkillList->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
	gvMaxHP->setNumValues(99);
	gvMaxHP->setForegroundColor(QColor(255, 0, 0));
	gvMaxHP->setMaxValue(9999);
	gvMaxHP->setupGraph();
	gvMaxMP->setNumValues(99);
	gvMaxMP->setForegroundColor(QColor(0, 0, 255));
	gvMaxMP->setMaxValue(999);
	gvMaxMP->setupGraph();
	gvAttack->setNumValues(99);
	gvAttack->setForegroundColor(QColor(255, 192, 0));
	gvAttack->setMaxValue(999);
	gvAttack->setupGraph();
	gvDefense->setNumValues(99);
	gvDefense->setForegroundColor(QColor(0, 255, 0));
	gvDefense->setMaxValue(999);
	gvDefense->setupGraph();
	gvWisdom->setNumValues(99);
	gvWisdom->setForegroundColor(QColor(255, 0, 255));
	gvWisdom->setMaxValue(999);
	gvWisdom->setupGraph();
	gvAgility->setNumValues(99);
	gvAgility->setForegroundColor(QColor(0, 255, 255));
	gvAgility->setMaxValue(999);
	gvAgility->setupGraph();
	gvFace->setScene(new QGraphicsScene(0.0, 0.0, 72.0, 72.0));
	gvFace->setBackgroundBrush(QBrush(Qt::magenta));
	gvSprite->setScene(new QGraphicsScene(0.0, 0.0, 48.0, 64.0));
	gvSprite->setBackgroundBrush(QBrush(Qt::magenta));
	faceItem = new QGraphicsPixmapItem(QPixmap());
	faceItem->setFlags(0x0);
	faceItem->setPos(0.0, 0.0);
	gvFace->scene()->addItem(faceItem);
	spriteItem = new QGraphicsPixmapItem(QPixmap());
	spriteItem->setFlags(0x0);
	spriteItem->setPos(0.0, 0.0);
	gvSprite->scene()->addItem(spriteItem);
	spriteFrame = 0;
	spriteFrameDirection = 1;
	spriteNumFrames = 1;
	spritePingPong = false;
	resistLevelA = QIcon(":/icons/resistLevelA.png");
	resistLevelB = QIcon(":/icons/resistLevelB.png");
	resistLevelC = QIcon(":/icons/resistLevelC.png");
	resistLevelD = QIcon(":/icons/resistLevelD.png");
	resistLevelE = QIcon(":/icons/resistLevelE.png");
	charRef = NULL;
}

Database_Characters::~Database_Characters()
{
	if (charRef != NULL)
	{
		delete charRef;
		charRef = NULL;
	}
}

void Database_Characters::setupCharacters()
{
	updateCharacterList();
	cboxSprite->addItems(ProjectData::characterSpriteNames);
	ignoreEvents = false;
}

void Database_Characters::shutdownCharacters()
{
	ignoreEvents = true;
	animationTimer->stop();
	cboxSprite->clear();
	if (charRef != NULL)
	{
		delete charRef;
		charRef = NULL;
	}
}

void Database_Characters::on_lwCharacterList_itemSelectionChanged()
{
	ProjectData::DataReference<CharacterSprite> *spriteRef;
	QString location;
	int charID = lwCharacterList->currentRow();
	int spriteNumLoops, numFrames;
	if (charRef != NULL)
	{
		delete charRef;
		charRef = NULL;
	}
	if (charID >= 0)
	{
		ignoreEvents = true;
		animationTimer->stop();
		charRef = new ProjectData::DataReference<Character>(charID, __FILE__, __LINE__);
		leName->setText((*charRef)->getName());
		sbMinLevel->setValue((*charRef)->getMinLevel());
		sbMaxLevel->setValue((*charRef)->getMaxLevel());
		cboxClass->setCurrentIndex((*charRef)->getClassID() + 1);
		cbDuelWielding->setChecked((*charRef)->isDuelWielding());
		cbAIControlled->setChecked((*charRef)->isAIControlled());
		cbEquipmentLocked->setChecked((*charRef)->isEquipmentLocked());
		cbMightyGuard->setChecked((*charRef)->isMightyGuard());
		cbCriticalHits->setChecked((*charRef)->isCriticalHits());
		dsbCriticalHitRate->setValue((*charRef)->getCriticalHitRate());
		dsbCriticalHitMultiplier->setValue((double)((*charRef)->getCriticalHitMultiplier()));
		location = (*charRef)->getFaceGraphicLocation();
		if (location.isEmpty())
			faceItem->setPixmap(QPixmap());
		else
			faceItem->setPixmap(QPixmap(location).scaled(72, 72));
		faceItem->setPos(0.0, 0.0);
		spriteFrame = 0;
		spriteFrameDirection = 1;
		cboxSprite->setCurrentIndex((*charRef)->getSpriteID());
		spriteRef = new ProjectData::DataReference<CharacterSprite>((*charRef)->getSpriteID(), __FILE__, __LINE__);
		spriteImage = QPixmap((*spriteRef)->getMapPoseLocation(CharacterSprite::MAPPOSE_IDLE));
		if (spriteImage.height() == 256)
			spriteImage = spriteImage.copy(0, 128, spriteImage.width(), 64);
		spriteNumFrames = spriteImage.width() / 48;
		spriteNumLoops = (*spriteRef)->getMapPoseNumLoops(CharacterSprite::MAPPOSE_IDLE);
		spritePingPong = (*spriteRef)->isMapPosePingPong(CharacterSprite::MAPPOSE_IDLE);
		numFrames = spriteNumFrames;
		if (spritePingPong)
			numFrames += spriteNumFrames - 1;
		spriteItem->setPixmap(spriteImage.copy(0, 0, 48, 64));
		if (spriteNumFrames > 1)
			animationTimer->start(1000 / (numFrames * spriteNumLoops));
		delete spriteRef;
		sbSpriteOpacity->setValue((*charRef)->getSpriteOpacity());
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*charRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*charRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*charRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*charRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*charRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*charRef)->getAgility(i + 1));
		}
		gvMaxHP->updateView();
		gvMaxMP->updateView();
		gvAttack->updateView();
		gvDefense->updateView();
		gvWisdom->updateView();
		gvAgility->updateView();
		lblExperienceCurve->setText(QString("Primary: %1; Secondary: %2").arg((*charRef)->getExperiencePrimary()).arg((*charRef)->getExperienceSecondary()));
		updateItemLists();
		if ((*charRef)->getWeaponID() == -1)
			cboxWeapon->setCurrentIndex(0);
		else
			cboxWeapon->setCurrentIndex(weaponIDs.indexOf((*charRef)->getWeaponID()) + 1);
		if ((*charRef)->getShieldID() == -1)
			cboxShield->setCurrentIndex(0);
		else
			cboxShield->setCurrentIndex(shieldIDs.indexOf((*charRef)->getShieldID()) + 1);
		if ((*charRef)->getArmorID() == -1)
			cboxArmor->setCurrentIndex(0);
		else
			cboxArmor->setCurrentIndex(armorIDs.indexOf((*charRef)->getArmorID()) + 1);
		if ((*charRef)->getHelmetID() == -1)
			cboxHelmet->setCurrentIndex(0);
		else
			cboxHelmet->setCurrentIndex(helmetIDs.indexOf((*charRef)->getHelmetID()) + 1);
		if ((*charRef)->getAccessoryID() == -1)
			cboxAccessory->setCurrentIndex(0);
		else
			cboxAccessory->setCurrentIndex(accessoryIDs.indexOf((*charRef)->getAccessoryID()) + 1);
		cboxUnarmedBattleAnimation->setCurrentIndex((*charRef)->getUnarmedBattleAnimationID());
		updateCharacterSpriteList();
		updateBattleAnimationList();
		updateSkillsList();
		updateConditionResistList();
		updateAttributeResistList();
		ignoreEvents = false;
	}
}

void Database_Characters::on_bArraySize_clicked()
{
	int numCharacters = ProjectData::characterNames.size();
	ArraySize *arraysize = new ArraySize(numCharacters, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numCharacters)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (charRef != NULL)
				{
					if (newSize <= charRef->getDataID())
					{
						delete charRef;
						charRef = NULL;
					}
				}
				for (int i = numCharacters - 1; i >= newSize; --i)
				{
					ProjectData::deleteCharacter(i);
					for (int j = 0; j < ProjectData::itemNames.size(); ++j)
					{
						ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>(j, __FILE__, __LINE__);
						(*itemRef)->characterDeleted(i);
						delete itemRef;
					}
				}
			}
		}
		else
		{
			for (int i = numCharacters; i < newSize; ++i)
			{
				Character *character = new Character;
				character->setName(QString("Char%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addCharacter(character);
				for (int j = 0; j < ProjectData::itemNames.size(); ++j)
				{
					ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>(j, __FILE__, __LINE__);
					(*itemRef)->characterAdded();
					delete itemRef;
				}
				delete character;
			}
		}
		ignoreEvents = true;
		updateCharacterList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_Characters::on_leName_textChanged(QString text)
{
	int currentRow = lwCharacterList->currentRow();
	ignoreEvents = true;
	lwCharacterList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 4, 10, QChar('0')).arg(text));
	ProjectData::characterNames[currentRow] = text;
	(*charRef)->setName(text);
	ignoreEvents = false;
}

void Database_Characters::on_sbMinLevel_valueChanged(int value)
{
	(*charRef)->setMinLevel(value);
	sbMaxLevel->setMinimum(value);
	gvMaxHP->setDisplayValue(value - 1);
	gvMaxMP->setDisplayValue(value - 1);
	gvAttack->setDisplayValue(value - 1);
	gvDefense->setDisplayValue(value - 1);
	gvWisdom->setDisplayValue(value - 1);
	gvAgility->setDisplayValue(value - 1);
}

void Database_Characters::on_sbMaxLevel_valueChanged(int value)
{
	(*charRef)->setMaxLevel(value);
	sbMinLevel->setMaximum(value);
}

void Database_Characters::on_cboxClass_currentIndexChanged(int index)
{
	if (charRef != NULL)
		(*charRef)->setClassID(index);
}

void Database_Characters::on_bApplyClass_clicked()
{
	TODO("Implement function");
}

void Database_Characters::on_cbDuelWielding_toggled(bool on)
{
	(*charRef)->setDuelWielding(on);
	if (on)
	{
		lblShield->setText("Weapon2:");
		updateItemLists();
	}
	else
	{
		lblShield->setText("Shield:");
		updateItemLists();
	}
}

void Database_Characters::on_cbAIControlled_toggled(bool on)
{
	(*charRef)->setAIControlled(on);
	frmAI->setEnabled(on);
}

void Database_Characters::on_cbEquipmentLocked_toggled(bool on)
{
	(*charRef)->setEquipmentLocked(on);
}

void Database_Characters::on_cbMightyGuard_toggled(bool on)
{
	(*charRef)->setMightyGuard(on);
}

void Database_Characters::on_cbCriticalHits_toggled(bool on)
{
	(*charRef)->setCriticalHits(on);
	wCriticalHits->setEnabled(on);
}

void Database_Characters::on_dsbCriticalHitRate_valueChanged(double value)
{
	(*charRef)->setCriticalHitRate(value);
}

void Database_Characters::on_dsbCriticalHitMultiplier_valueChanged(double value)
{
	(*charRef)->setCriticalHitMultiplier((float)value);
}

void Database_Characters::on_bSetFace_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/faces", ImageBrowser::BlockNone, QList<QSize>() << QSize(96, 96));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		faceItem->setPixmap(QPixmap(fileLocation).scaled(72, 72));
		(*charRef)->setFaceGraphicLocation(fileLocation);
	}
	delete imageBrowser;
}

void Database_Characters::on_sbSpriteOpacity_valueChanged(int value)
{
	if (charRef != NULL)
		(*charRef)->setSpriteOpacity(value);
	spriteItem->setOpacity(value / 100.0);
}

void Database_Characters::on_cboxSprite_currentIndexChanged(int index)
{
	if (!ignoreEvents && charRef != NULL)
	{
		ProjectData::DataReference<CharacterSprite> *spriteRef = new ProjectData::DataReference<CharacterSprite>(index, __FILE__, __LINE__);
		int spriteNumLoops, numFrames;
		(*charRef)->setSpriteID(index);
		animationTimer->stop();
		spriteImage = QPixmap((*spriteRef)->getMapPoseLocation(CharacterSprite::MAPPOSE_IDLE));
		if (spriteImage.height() == 256)
			spriteImage = spriteImage.copy(0, 128, spriteImage.width(), 64);
		spriteNumFrames = spriteImage.width() / 48;
		spriteNumLoops = (*spriteRef)->getMapPoseNumLoops(CharacterSprite::MAPPOSE_IDLE);
		spritePingPong = (*spriteRef)->isMapPosePingPong(CharacterSprite::MAPPOSE_IDLE);
		numFrames = spriteNumFrames;
		if (spritePingPong)
			numFrames += spriteNumFrames - 1;
		spriteItem->setPixmap(spriteImage.copy(0, 0, 48, 64));
		if (spriteNumFrames > 1)
			animationTimer->start(1000 / (numFrames * spriteNumLoops));
		delete spriteRef;
	}
}

void Database_Characters::on_gvMaxHP_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(charRef->getDataID(), true, CharacterClassStatEditor::STAT_MAXHP);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*charRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*charRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*charRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*charRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*charRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*charRef)->getAgility(i + 1));
		}
		gvMaxHP->updateView();
		gvMaxMP->updateView();
		gvAttack->updateView();
		gvDefense->updateView();
		gvWisdom->updateView();
		gvAgility->updateView();
	}
	delete statEditor;
}

void Database_Characters::on_gvMaxMP_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(charRef->getDataID(), true, CharacterClassStatEditor::STAT_MAXMP);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*charRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*charRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*charRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*charRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*charRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*charRef)->getAgility(i + 1));
		}
		gvMaxHP->updateView();
		gvMaxMP->updateView();
		gvAttack->updateView();
		gvDefense->updateView();
		gvWisdom->updateView();
		gvAgility->updateView();
	}
	delete statEditor;
}

void Database_Characters::on_gvAttack_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(charRef->getDataID(), true, CharacterClassStatEditor::STAT_ATTACK);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*charRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*charRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*charRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*charRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*charRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*charRef)->getAgility(i + 1));
		}
		gvMaxHP->updateView();
		gvMaxMP->updateView();
		gvAttack->updateView();
		gvDefense->updateView();
		gvWisdom->updateView();
		gvAgility->updateView();
	}
	delete statEditor;
}

void Database_Characters::on_gvDefense_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(charRef->getDataID(), true, CharacterClassStatEditor::STAT_DEFENSE);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*charRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*charRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*charRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*charRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*charRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*charRef)->getAgility(i + 1));
		}
		gvMaxHP->updateView();
		gvMaxMP->updateView();
		gvAttack->updateView();
		gvDefense->updateView();
		gvWisdom->updateView();
		gvAgility->updateView();
	}
	delete statEditor;
}

void Database_Characters::on_gvWisdom_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(charRef->getDataID(), true, CharacterClassStatEditor::STAT_WISDOM);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*charRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*charRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*charRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*charRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*charRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*charRef)->getAgility(i + 1));
		}
		gvMaxHP->updateView();
		gvMaxMP->updateView();
		gvAttack->updateView();
		gvDefense->updateView();
		gvWisdom->updateView();
		gvAgility->updateView();
	}
	delete statEditor;
}

void Database_Characters::on_gvAgility_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(charRef->getDataID(), true, CharacterClassStatEditor::STAT_AGILITY);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*charRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*charRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*charRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*charRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*charRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*charRef)->getAgility(i + 1));
		}
		gvMaxHP->updateView();
		gvMaxMP->updateView();
		gvAttack->updateView();
		gvDefense->updateView();
		gvWisdom->updateView();
		gvAgility->updateView();
	}
	delete statEditor;
}

void Database_Characters::on_tbConfigureExperienceCurve_clicked()
{
	CharacterClassExperienceCurveEditor *experienceEditor = new CharacterClassExperienceCurveEditor(charRef->getDataID(), true);
	int primary, secondary;
	experienceEditor->setupEditor();
	experienceEditor->exec();
	delete experienceEditor;
	primary = (*charRef)->getExperiencePrimary();
	secondary = (*charRef)->getExperienceSecondary();
	lblExperienceCurve->setText(QString("Primary: %1; Secondary: %2").arg(primary).arg(secondary));
}

void Database_Characters::on_bConfigureBattleCommands_clicked()
{
	SetBattleCommands *setBattleCommands = new SetBattleCommands;
	setBattleCommands->setCommand1((*charRef)->getBattleCommand(0));
	setBattleCommands->setCommand2((*charRef)->getBattleCommand(1));
	setBattleCommands->setCommand3((*charRef)->getBattleCommand(2));
	setBattleCommands->setCommand4((*charRef)->getBattleCommand(3));
	setBattleCommands->setCommand5((*charRef)->getBattleCommand(4));
	setBattleCommands->setCommand6((*charRef)->getBattleCommand(5));
	if (setBattleCommands->exec())
	{
		(*charRef)->setBattleCommand(0, setBattleCommands->getCommand1());
		(*charRef)->setBattleCommand(1, setBattleCommands->getCommand2());
		(*charRef)->setBattleCommand(2, setBattleCommands->getCommand3());
		(*charRef)->setBattleCommand(3, setBattleCommands->getCommand4());
		(*charRef)->setBattleCommand(4, setBattleCommands->getCommand5());
		(*charRef)->setBattleCommand(5, setBattleCommands->getCommand6());
	}
	delete setBattleCommands;
}

void Database_Characters::on_bConfigureBattleEvent_clicked()
{
	QMessageBox::information(window(), "Not Implemented", "This feature is not yet implemented.");
	TODO("Implement function");
}

void Database_Characters::on_cboxWeapon_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		if (index > 0)
		{
			ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>(weaponIDs[index - 1], __FILE__, __LINE__);
			(*charRef)->setWeaponID(weaponIDs[index - 1]);
			if ((*itemRef)->getWeaponData()->twoHanded)
			{
				cboxShield->setCurrentIndex(0);
				cboxShield->setEnabled(false);
			}
			else
				cboxShield->setEnabled(true);
			delete itemRef;
		}
		else
		{
			(*charRef)->setWeaponID(-1);
			cboxShield->setEnabled(true);
		}
	}
}

void Database_Characters::on_cboxShield_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		if (index > 0)
			(*charRef)->setShieldID(shieldIDs[index - 1]);
		else
			(*charRef)->setShieldID(-1);
	}
}

void Database_Characters::on_cboxArmor_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		if (index > 0)
			(*charRef)->setArmorID(armorIDs[index - 1]);
		else
			(*charRef)->setArmorID(-1);
	}
}

void Database_Characters::on_cboxHelmet_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		if (index > 0)
			(*charRef)->setHelmetID(helmetIDs[index - 1]);
		else
			(*charRef)->setHelmetID(-1);
	}
}

void Database_Characters::on_cboxAccessory_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		if (index > 0)
			(*charRef)->setAccessoryID(accessoryIDs[index - 1]);
		else
			(*charRef)->setAccessoryID(-1);
	}
}

void Database_Characters::on_cboxUnarmedBattleAnimation_currentIndexChanged(int index)
{
	(*charRef)->setUnarmedBattleAnimationID(index);
}

void Database_Characters::on_bConfigureAI_clicked()
{
	QMessageBox::information(window(), "Not Implemented", "This feature is not yet implemented.");
	TODO("Implement function");
}

void Database_Characters::on_twSkillList_itemSelectionChanged()
{
	int row = twSkillList->currentRow();
	tbDeleteSkill->setEnabled((row >= 0));
}

void Database_Characters::on_twSkillList_itemDoubleClicked(QTableWidgetItem *item)
{
	int row = item->row();
	if (charRef != NULL)
	{
		ConfigureSkillLearned *configSkill = new ConfigureSkillLearned();
		configSkill->setLevelLearned((*charRef)->getSkillLearnedPointer(row)->levelLearned);
		configSkill->setSkillLearned((*charRef)->getSkillLearnedPointer(row)->skillID);
		if (configSkill->exec())
		{
			Character::SkillLearned skillLearned;
			(*charRef)->deleteSkillLearned(row);
			skillLearned.levelLearned = configSkill->getLevelLearned();
			skillLearned.skillID = configSkill->getSkillLearned();
			(*charRef)->addSkillLearned(skillLearned);
			updateSkillsList();
		}
		delete configSkill;
	}
}

void Database_Characters::on_tbDeleteSkill_clicked()
{
	if (charRef != NULL)
	{
		int currentRow = twSkillList->currentRow();
		(*charRef)->deleteSkillLearned(currentRow);
		twSkillList->removeRow(currentRow);
		tbDeleteSkill->setEnabled((twSkillList->rowCount() > 0));
	}
}

void Database_Characters::on_tbAddSkill_clicked()
{
	if (charRef != NULL)
	{
		ConfigureSkillLearned *configSkill = new ConfigureSkillLearned();
		if (configSkill->exec())
		{
			Character::SkillLearned skillLearned;
			skillLearned.levelLearned = configSkill->getLevelLearned();
			skillLearned.skillID = configSkill->getSkillLearned();
			(*charRef)->addSkillLearned(skillLearned);
			updateSkillsList();
		}
		delete configSkill;
	}
}

void Database_Characters::on_lwConditionResist_itemClicked(QListWidgetItem *item)
{
	int row = lwConditionResist->row(item);
	int resistance = (*charRef)->getConditionResist(row);
	lwConditionResist->setCurrentRow(-1);
	resistance = (resistance + 1) % 5;
	(*charRef)->setConditionResist(row, resistance);
	if (resistance == Character::RESISTLEVEL_A)
		item->setIcon(resistLevelA);
	else if (resistance == Character::RESISTLEVEL_B)
		item->setIcon(resistLevelB);
	else if (resistance == Character::RESISTLEVEL_C)
		item->setIcon(resistLevelC);
	else if (resistance == Character::RESISTLEVEL_D)
		item->setIcon(resistLevelD);
	else
		item->setIcon(resistLevelE);
}

void Database_Characters::on_lwAttributeResist_itemClicked(QListWidgetItem *item)
{
	int row = lwAttributeResist->row(item);
	int resistance = (*charRef)->getAttributeResist(row);
	lwAttributeResist->setCurrentRow(-1);
	resistance = (resistance + 1) % 5;
	(*charRef)->setAttributeResist(row, resistance);
	if (resistance == Character::RESISTLEVEL_A)
		item->setIcon(resistLevelA);
	else if (resistance == Character::RESISTLEVEL_B)
		item->setIcon(resistLevelB);
	else if (resistance == Character::RESISTLEVEL_C)
		item->setIcon(resistLevelC);
	else if (resistance == Character::RESISTLEVEL_D)
		item->setIcon(resistLevelD);
	else
		item->setIcon(resistLevelE);
}

void Database_Characters::animateSprite()
{
	spriteFrame += spriteFrameDirection;
	if ((spriteFrame == 0 || spriteFrame == spriteNumFrames - 1) && spritePingPong)
		spriteFrameDirection *= -1;
	if (!spritePingPong)
		spriteFrame %= spriteNumFrames;
	spriteItem->setPixmap(spriteImage.copy(spriteFrame * 48, 0, 48, 64));
}

void Database_Characters::updateCharacterList()
{
	QListWidgetItem *item;
	int i, numCharacters = ProjectData::characterNames.size();
	int currentRow = lwCharacterList->currentRow();
	lwCharacterList->clear();
	for (i = 0; i < numCharacters; ++i)
	{
		item = new QListWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::characterNames[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		lwCharacterList->addItem(item);
	}
	if (currentRow >= lwCharacterList->count())
		currentRow = lwCharacterList->count() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::characterNames.size() > 0)
	{
		lwCharacterList->setCurrentRow(currentRow);
		lwCharacterList->item(currentRow)->setSelected(true);
	}
}

void Database_Characters::updateCharacterSpriteList()
{
	int numCharacterSprites = ProjectData::characterSpriteNames.size();
	ignoreEvents = true;
	cboxSprite->clear();
	for (int i = 0; i < numCharacterSprites; ++i)
		cboxSprite->addItem(ProjectData::characterSpriteNames[i]);
	ignoreEvents = false;
}

void Database_Characters::updateItemLists()
{
	int numItems = ProjectData::itemNames.size();
	ignoreEvents = true;
	cboxWeapon->clear();
	cboxShield->clear();
	cboxArmor->clear();
	cboxHelmet->clear();
	cboxAccessory->clear();
	weaponIDs.clear();
	shieldIDs.clear();
	armorIDs.clear();
	helmetIDs.clear();
	accessoryIDs.clear();
	cboxWeapon->addItem("(NONE)");
	cboxShield->addItem("(NONE)");
	cboxArmor->addItem("(NONE)");
	cboxHelmet->addItem("(NONE)");
	cboxAccessory->addItem("(NONE)");
	for (int i = 0; i < numItems; ++i)
	{
		ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>(i, __FILE__, __LINE__);
		int itemType = (*itemRef)->getType();
		if (itemType == Item::TYPE_WEAPON)
		{
			Item::WeaponItem *weaponData = (*itemRef)->getWeaponData();
			if (weaponData->usableByCharacters)
			{
				if (weaponData->usableBy[charRef->getDataID()])
				{
					cboxWeapon->addItem((*itemRef)->getName());
					weaponIDs.append(i);
					if (cbDuelWielding->isChecked() && !weaponData->twoHanded)
					{
						cboxShield->addItem((*itemRef)->getName());
						shieldIDs.append(i);
					}
				}
			}
			else
			{
				if (weaponData->usableBy[cboxClass->currentIndex()])
				{
					cboxWeapon->addItem((*itemRef)->getName());
					weaponIDs.append(i);
					if (cbDuelWielding->isChecked() && !weaponData->twoHanded)
					{
						cboxShield->addItem((*itemRef)->getName());
						shieldIDs.append(i);
					}
				}
			}
		}
		else if (itemType == Item::TYPE_SHIELD && !cbDuelWielding->isChecked())
		{
			Item::EquipmentItem *equipmentData = (*itemRef)->getEquipmentData();
			if (equipmentData->usableByCharacters)
			{
				if (equipmentData->usableBy[charRef->getDataID()])
				{
					cboxShield->addItem((*itemRef)->getName());
					shieldIDs.append(i);
				}
			}
			else
			{
				if (equipmentData->usableBy[cboxClass->currentIndex()])
				{
					cboxShield->addItem((*itemRef)->getName());
					shieldIDs.append(i);
				}
			}
		}
		else if (itemType == Item::TYPE_ARMOR)
		{
			Item::EquipmentItem *equipmentData = (*itemRef)->getEquipmentData();
			if (equipmentData->usableByCharacters)
			{
				if (equipmentData->usableBy[charRef->getDataID()])
				{
					cboxArmor->addItem((*itemRef)->getName());
					armorIDs.append(i);
				}
			}
			else
			{
				if (equipmentData->usableBy[cboxClass->currentIndex()])
				{
					cboxArmor->addItem((*itemRef)->getName());
					armorIDs.append(i);
				}
			}
		}
		else if (itemType == Item::TYPE_HELMET)
		{
			Item::EquipmentItem *equipmentData = (*itemRef)->getEquipmentData();
			if (equipmentData->usableByCharacters)
			{
				if (equipmentData->usableBy[charRef->getDataID()])
				{
					cboxHelmet->addItem((*itemRef)->getName());
					helmetIDs.append(i);
				}
			}
			else
			{
				if (equipmentData->usableBy[cboxClass->currentIndex()])
				{
					cboxHelmet->addItem((*itemRef)->getName());
					helmetIDs.append(i);
				}
			}
		}
		else if (itemType == Item::TYPE_ACCESSORY)
		{
			Item::EquipmentItem *equipmentData = (*itemRef)->getEquipmentData();
			if (equipmentData->usableByCharacters)
			{
				if (equipmentData->usableBy[charRef->getDataID()])
				{
					cboxAccessory->addItem((*itemRef)->getName());
					accessoryIDs.append(i);
				}
			}
			else
			{
				if (equipmentData->usableBy[cboxClass->currentIndex()])
				{
					cboxAccessory->addItem((*itemRef)->getName());
					accessoryIDs.append(i);
				}
			}
		}
		delete itemRef;
	}
	ignoreEvents = false;
}

void Database_Characters::updateBattleAnimationList()
{
	int numBattleAnimations = ProjectData::battleAnimationNames.size();
	cboxUnarmedBattleAnimation->clear();
	for (int i = 0; i < numBattleAnimations; ++i)
		cboxUnarmedBattleAnimation->addItem(ProjectData::battleAnimationNames[i]);
}

void Database_Characters::updateSkillsList()
{
	QTableWidgetItem *item;
	if (!ignoreEvents && charRef != NULL)
	{
		twSkillList->clearContents();
		twSkillList->setRowCount((*charRef)->numSkillsLearned());
		for (int i = 0; i < (*charRef)->numSkillsLearned(); ++i)
		{
			Character::SkillLearned *skillLearned = (*charRef)->getSkillLearnedPointer(i);
			item = new QTableWidgetItem(QString("L%1").arg(skillLearned->levelLearned));
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			twSkillList->setItem(i, 0, item);
			item = new QTableWidgetItem(ProjectData::skillNames[skillLearned->skillID]);
			item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
			twSkillList->setItem(i, 1, item);
			twSkillList->setRowHeight(i, 15);
		}
		if (twSkillList->rowCount() > 0)
		{
			twSkillList->setCurrentCell(0, 0);
			twSkillList->item(0, 0)->setSelected(true);
			twSkillList->item(0, 1)->setSelected(true);
		}
	}
}

void Database_Characters::updateConditionResistList()
{
	QListWidgetItem *item;
	int i, numConditions = ProjectData::conditionNames.size();
	int resistance;
	lwConditionResist->clear();
	for (i = 0; i < numConditions; ++i)
	{
		item = new QListWidgetItem(ProjectData::conditionNames[i]);
		resistance = (*charRef)->getConditionResist(i);
		if (resistance == Character::RESISTLEVEL_A)
			item->setIcon(resistLevelA);
		else if (resistance == Character::RESISTLEVEL_B)
			item->setIcon(resistLevelB);
		else if (resistance == Character::RESISTLEVEL_C)
			item->setIcon(resistLevelC);
		else if (resistance == Character::RESISTLEVEL_D)
			item->setIcon(resistLevelD);
		else
			item->setIcon(resistLevelE);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		item->setSizeHint(QSize(-1, 16));
		lwConditionResist->addItem(item);
	}
}

void Database_Characters::updateAttributeResistList()
{
	QListWidgetItem *item;
	int i, numAttributes = ProjectData::attributeNames.size();
	int resistance;
	lwAttributeResist->clear();
	for (i = 0; i < numAttributes; ++i)
	{
		item = new QListWidgetItem(ProjectData::attributeNames[i]);
		resistance = (*charRef)->getAttributeResist(i);
		if (resistance == Character::RESISTLEVEL_A)
			item->setIcon(resistLevelA);
		else if (resistance == Character::RESISTLEVEL_B)
			item->setIcon(resistLevelB);
		else if (resistance == Character::RESISTLEVEL_C)
			item->setIcon(resistLevelC);
		else if (resistance == Character::RESISTLEVEL_D)
			item->setIcon(resistLevelD);
		else
			item->setIcon(resistLevelE);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		item->setSizeHint(QSize(-1, 16));
		lwAttributeResist->addItem(item);
	}
}
