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
#include <QColor>
#include <QToolTip>
#include <QHeaderView>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "characterclassexperiencecurveeditor.h"
#include "characterclassstateditor.h"
#include "clipboard.h"
#include "configurebattlecommands.h"
#include "configureskilllearned.h"
#include "database_classes.h"
#include "projectdata.h"
#include "projectdata_charactersprite.h"
#include "projectdata_class.h"
#include "projectdata_item.h"
#include "xmlParser.h"

Database_Classes::Database_Classes(QWidget *parent) : QWidget(parent)
{
	ignoreEvents = true;
	setupUi(this);
	animationTimer = new QTimer(this);
	QObject::connect(animationTimer, SIGNAL(timeout()), this, SLOT(animateSprite()));
	twSkillList->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
	twSkillList->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	twSkillList->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
	bSpriteOpacitySideButton->setSpinBox(sbSpriteOpacity);
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
	gvSprite->setScene(new QGraphicsScene(0.0, 0.0, 48.0, 64.0));
	gvSprite->setBackgroundBrush(QBrush(Qt::magenta));
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
	classRef = NULL;
	tbDeleteSkill->setEnabled(false);
}

Database_Classes::~Database_Classes()
{
	if (classRef != NULL)
	{
		delete classRef;
		classRef = NULL;
	}
}

void Database_Classes::setupClasses()
{
	gvExperienceGraph->setNumValues(99);
	gvExperienceGraph->setForegroundColor(QColor(255, 0, 255));
	gvExperienceGraph->setMaxValue(9999999);
	gvExperienceGraph->setupGraph();
	gvExperienceGraph->setDisplayValue(-1);
	ignoreEvents = true;
	updateExperienceCurve();
	command1List.append(-1);
	command2List.append(-1);
	command3List.append(-1);
	command4List.append(-1);
	command5List.append(-1);
	command6List.append(-1);
	updateCommand1List();
	updateCommand2List();
	updateCommand3List();
	updateCommand4List();
	updateCommand5List();
	updateCommand6List();
	updateClassList();
	cboxSprite->addItems(ProjectData::characterSpriteNames);
	ignoreEvents = false;
	on_lwClassList_itemSelectionChanged();
}

void Database_Classes::shutdownClasses()
{
	ignoreEvents = true;
	animationTimer->stop();
	cboxSprite->clear();
	if (classRef != NULL)
	{
		delete classRef;
		classRef = NULL;
	}
}

void Database_Classes::on_lwClassList_itemSelectionChanged()
{
	ProjectData::DataReference<CharacterSprite> *spriteRef;
	QString location;
	int classID = lwClassList->currentRow();
	int spriteNumLoops, numFrames;
	if (!ignoreEvents)
	{
		if (classRef != NULL)
		{
			delete classRef;
			classRef = NULL;
		}
		if (classID >= 0)
		{
			classRef = new ProjectData::DataReference<Class>(classID, __FILE__, __LINE__);
			ignoreEvents = true;
			updateExperienceCurve();
			updateCharacterSpriteList();
			updateSkillsList();
			updateConditionResistList();
			updateAttributeResistList();
			command1List.clear();
			command2List.clear();
			command3List.clear();
			command4List.clear();
			command5List.clear();
			command6List.clear();
			command1List.append(-1);
			command2List.append(-1);
			command3List.append(-1);
			command4List.append(-1);
			command5List.append(-1);
			command6List.append(-1);
			cboxCommand1->clear();
			cboxCommand2->clear();
			cboxCommand3->clear();
			cboxCommand4->clear();
			cboxCommand5->clear();
			cboxCommand6->clear();
			cboxCommand1->addItem("(NONE)");
			cboxCommand2->addItem("(NONE)");
			cboxCommand3->addItem("(NONE)");
			cboxCommand4->addItem("(NONE)");
			cboxCommand5->addItem("(NONE)");
			cboxCommand6->addItem("(NONE)");
			cboxCommand1->setCurrentIndex(0);
			cboxCommand2->setCurrentIndex(0);
			cboxCommand3->setCurrentIndex(0);
			cboxCommand4->setCurrentIndex(0);
			cboxCommand5->setCurrentIndex(0);
			cboxCommand6->setCurrentIndex(0);
			updateCommand1List();
			updateCommand2List();
			updateCommand3List();
			updateCommand4List();
			updateCommand5List();
			updateCommand6List();
			ignoreEvents = false;
			leName->setText((*classRef)->getName());
			sbSpriteOpacity->setValue((*classRef)->getSpriteOpacity());
			cbDuelWielding->setChecked((*classRef)->isDuelWielding());
			cbAIControlled->setChecked((*classRef)->isAIControlled());
			cbEquipmentLocked->setChecked((*classRef)->isEquipmentLocked());
			cbMightyGuard->setChecked((*classRef)->isMightyGuard());
			spriteFrame = 0;
			spriteFrameDirection = 1;
			cboxSprite->setCurrentIndex((*classRef)->getSpriteID());
			spriteRef = new ProjectData::DataReference<CharacterSprite>((*classRef)->getSpriteID(), __FILE__, __LINE__);
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
			for (int i = 0; i < 99; ++i)
			{
				gvMaxHP->changeValue(i, (*classRef)->getMaxHP(i + 1));
				gvMaxMP->changeValue(i, (*classRef)->getMaxMP(i + 1));
				gvAttack->changeValue(i, (*classRef)->getAttack(i + 1));
				gvDefense->changeValue(i, (*classRef)->getDefense(i + 1));
				gvWisdom->changeValue(i, (*classRef)->getWisdom(i + 1));
				gvAgility->changeValue(i, (*classRef)->getAgility(i + 1));
			}
			gvMaxHP->updateView();
			gvMaxMP->updateView();
			gvAttack->updateView();
			gvDefense->updateView();
			gvWisdom->updateView();
			gvAgility->updateView();
			lblExperienceCurve->setText(QString("Primary: %1; Secondary: %2").arg((*classRef)->getExperiencePrimary()).arg((*classRef)->getExperienceSecondary()));
			setCommand1((*classRef)->getBattleCommand(0));
			setCommand2((*classRef)->getBattleCommand(1));
			setCommand3((*classRef)->getBattleCommand(2));
			setCommand4((*classRef)->getBattleCommand(3));
			setCommand5((*classRef)->getBattleCommand(4));
			setCommand6((*classRef)->getBattleCommand(5));
		}
	}
}

void Database_Classes::on_bArraySize_clicked()
{
	int numClasses = ProjectData::classNames.size();
	ArraySize *arraysize = new ArraySize(numClasses, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numClasses)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (classRef != NULL)
				{
					if (newSize <= classRef->getDataID())
					{
						delete classRef;
						classRef = NULL;
					}
				}
				for (int i = numClasses - 1; i >= newSize; --i)
				{
					ProjectData::deleteClass(i);
					for (int j = 0; j < ProjectData::itemNames.size(); ++j)
					{
						ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>(i, __FILE__, __LINE__);
						(*itemRef)->classDeleted(i);
					}
				}
			}
		}
		else
		{
			for (int i = numClasses; i < newSize; ++i)
			{
				Class *characterClass = new Class;
				characterClass->setName(QString("Clas%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addClass(characterClass);
				for (int j = 0; j < ProjectData::itemNames.size(); ++j)
				{
					ProjectData::DataReference<Item> *itemRef = new ProjectData::DataReference<Item>(i, __FILE__, __LINE__);
					(*itemRef)->classAdded();
				}
				delete characterClass;
			}
		}
		ignoreEvents = true;
		updateClassList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_Classes::on_leName_textChanged(QString text)
{
	int currentRow = lwClassList->currentRow();
	ignoreEvents = true;
	lwClassList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 4, 10, QChar('0')).arg(text));
	ProjectData::classNames[currentRow] = text;
	(*classRef)->setName(text);
	ignoreEvents = false;
}

void Database_Classes::on_cboxSprite_currentIndexChanged(int index)
{
	if (!ignoreEvents && classRef != NULL)
	{
		ProjectData::DataReference<CharacterSprite> *spriteRef = new ProjectData::DataReference<CharacterSprite>(index, __FILE__, __LINE__);
		int spriteNumLoops, numFrames;
		(*classRef)->setSpriteID(index);
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

void Database_Classes::on_cbDuelWielding_toggled(bool on)
{
	(*classRef)->setDuelWielding(on);
}

void Database_Classes::on_cbEquipmentLocked_toggled(bool on)
{
	(*classRef)->setEquipmentLocked(on);
}

void Database_Classes::on_cbAIControlled_toggled(bool on)
{
	(*classRef)->setAIControlled(on);
}

void Database_Classes::on_cbMightyGuard_toggled(bool on)
{
	(*classRef)->setMightyGuard(on);
}

void Database_Classes::on_gvMaxHP_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(classRef->getDataID(), false, CharacterClassStatEditor::STAT_MAXHP);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*classRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*classRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*classRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*classRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*classRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*classRef)->getAgility(i + 1));
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

void Database_Classes::on_gvMaxMP_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(classRef->getDataID(), false, CharacterClassStatEditor::STAT_MAXMP);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*classRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*classRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*classRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*classRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*classRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*classRef)->getAgility(i + 1));
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

void Database_Classes::on_gvAttack_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(classRef->getDataID(), false, CharacterClassStatEditor::STAT_ATTACK);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*classRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*classRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*classRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*classRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*classRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*classRef)->getAgility(i + 1));
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

void Database_Classes::on_gvDefense_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(classRef->getDataID(), false, CharacterClassStatEditor::STAT_DEFENSE);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*classRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*classRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*classRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*classRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*classRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*classRef)->getAgility(i + 1));
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

void Database_Classes::on_gvWisdom_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(classRef->getDataID(), false, CharacterClassStatEditor::STAT_WISDOM);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*classRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*classRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*classRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*classRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*classRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*classRef)->getAgility(i + 1));
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

void Database_Classes::on_gvAgility_mouseDoubleClicked()
{
	CharacterClassStatEditor *statEditor = new CharacterClassStatEditor(classRef->getDataID(), false, CharacterClassStatEditor::STAT_AGILITY);
	statEditor->setupEditor();
	if (statEditor->exec())
	{
		for (int i = 0; i < 99; ++i)
		{
			gvMaxHP->changeValue(i, (*classRef)->getMaxHP(i + 1));
			gvMaxMP->changeValue(i, (*classRef)->getMaxMP(i + 1));
			gvAttack->changeValue(i, (*classRef)->getAttack(i + 1));
			gvDefense->changeValue(i, (*classRef)->getDefense(i + 1));
			gvWisdom->changeValue(i, (*classRef)->getWisdom(i + 1));
			gvAgility->changeValue(i, (*classRef)->getAgility(i + 1));
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

void Database_Classes::on_tbConfigureExperienceCurve_clicked()
{
	CharacterClassExperienceCurveEditor *experienceEditor = new CharacterClassExperienceCurveEditor(classRef->getDataID(), false);
	int primary, secondary;
	experienceEditor->setupEditor();
	experienceEditor->exec();
	delete experienceEditor;
	primary = (*classRef)->getExperiencePrimary();
	secondary = (*classRef)->getExperienceSecondary();
	lblExperienceCurve->setText(QString("Primary: %1; Secondary: %2").arg(primary).arg(secondary));
	updateExperienceCurve();
}

void Database_Classes::on_gvExperienceGraph_mouseDoubleClicked()
{
	CharacterClassExperienceCurveEditor *experienceEditor = new CharacterClassExperienceCurveEditor(classRef->getDataID(), false);
	int primary, secondary;
	experienceEditor->setupEditor();
	experienceEditor->exec();
	delete experienceEditor;
	primary = (*classRef)->getExperiencePrimary();
	secondary = (*classRef)->getExperienceSecondary();
	lblExperienceCurve->setText(QString("Primary: %1; Secondary: %2").arg(primary).arg(secondary));
	updateExperienceCurve();
}

void Database_Classes::on_cboxCommand1_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand2List();
		updateCommand3List();
		updateCommand4List();
		updateCommand5List();
		updateCommand6List();
		if (index == 0)
			updateCommand1List();
		ignoreEvents = false;
		(*classRef)->setBattleCommand(0, getCommand1());
	}
}

void Database_Classes::on_cboxCommand2_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand3List();
		updateCommand4List();
		updateCommand5List();
		updateCommand6List();
		if (index == 0)
		{
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
		(*classRef)->setBattleCommand(1, getCommand2());
	}
}

void Database_Classes::on_cboxCommand3_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand4List();
		updateCommand5List();
		updateCommand6List();
		if (index == 0)
		{
			updateCommand3List();
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
		(*classRef)->setBattleCommand(2, getCommand3());
	}
}

void Database_Classes::on_cboxCommand4_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand3List();
		updateCommand5List();
		updateCommand6List();
		if (index == 0)
		{
			updateCommand4List();
			updateCommand3List();
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
		(*classRef)->setBattleCommand(3, getCommand4());
	}
}

void Database_Classes::on_cboxCommand5_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand3List();
		updateCommand4List();
		updateCommand6List();
		if (index == 0)
		{
			updateCommand5List();
			updateCommand4List();
			updateCommand3List();
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
		(*classRef)->setBattleCommand(4, getCommand5());
	}
}

void Database_Classes::on_cboxCommand6_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand3List();
		updateCommand4List();
		updateCommand5List();
		if (index == 0)
		{
			updateCommand6List();
			updateCommand5List();
			updateCommand4List();
			updateCommand3List();
			updateCommand2List();
			updateCommand1List();
		}
		ignoreEvents = false;
		(*classRef)->setBattleCommand(5, getCommand6());
	}
}

void Database_Classes::on_bConfigureCommands_clicked()
{
	ConfigureBattleCommands *configureCommands = new ConfigureBattleCommands;
	if (configureCommands->exec())
	{
		ignoreEvents = true;
		updateCommand1List();
		updateCommand2List();
		updateCommand3List();
		updateCommand4List();
		updateCommand5List();
		updateCommand6List();
		ignoreEvents = false;
	}
	delete configureCommands;
}

void Database_Classes::on_twSkillList_itemSelectionChanged()
{
	int row = twSkillList->currentRow();
	tbDeleteSkill->setEnabled((row >= 0));
}

void Database_Classes::on_twSkillList_itemDoubleClicked(QTableWidgetItem *item)
{
	int row = item->row();
	if (classRef != NULL)
	{
		ConfigureSkillLearned *configSkill = new ConfigureSkillLearned();
		configSkill->setLevelLearned((*classRef)->getSkillLearnedPointer(row)->levelLearned);
		configSkill->setSkillLearned((*classRef)->getSkillLearnedPointer(row)->skillID);
		if (configSkill->exec())
		{
			Class::SkillLearned skillLearned;
			(*classRef)->deleteSkillLearned(row);
			skillLearned.levelLearned = configSkill->getLevelLearned();
			skillLearned.skillID = configSkill->getSkillLearned();
			(*classRef)->addSkillLearned(skillLearned);
			updateSkillsList();
		}
		delete configSkill;
	}
}

void Database_Classes::on_tbDeleteSkill_clicked()
{
	if (classRef != NULL)
	{
		int currentRow = twSkillList->currentRow();
		(*classRef)->deleteSkillLearned(currentRow);
		twSkillList->removeRow(currentRow);
		tbDeleteSkill->setEnabled((twSkillList->rowCount() > 0));
	}
}

void Database_Classes::on_tbAddSkill_clicked()
{
	if (classRef != NULL)
	{
		ConfigureSkillLearned *configSkill = new ConfigureSkillLearned();
		if (configSkill->exec())
		{
			Class::SkillLearned skillLearned;
			skillLearned.levelLearned = configSkill->getLevelLearned();
			skillLearned.skillID = configSkill->getSkillLearned();
			(*classRef)->addSkillLearned(skillLearned);
			updateSkillsList();
		}
		delete configSkill;
	}
}

void Database_Classes::on_lwConditionResist_itemClicked(QListWidgetItem *item)
{
	int row = lwConditionResist->row(item);
	int resistance = (*classRef)->getConditionResist(row);
	lwConditionResist->setCurrentRow(-1);
	resistance = (resistance + 1) % 5;
	(*classRef)->setConditionResist(row, resistance);
	if (resistance == Class::RESISTLEVEL_A)
		item->setIcon(resistLevelA);
	else if (resistance == Class::RESISTLEVEL_B)
		item->setIcon(resistLevelB);
	else if (resistance == Class::RESISTLEVEL_C)
		item->setIcon(resistLevelC);
	else if (resistance == Class::RESISTLEVEL_D)
		item->setIcon(resistLevelD);
	else
		item->setIcon(resistLevelE);
}

void Database_Classes::on_lwAttributeResist_itemClicked(QListWidgetItem *item)
{
	int row = lwAttributeResist->row(item);
	int resistance = (*classRef)->getAttributeResist(row);
	lwAttributeResist->setCurrentRow(-1);
	resistance = (resistance + 1) % 5;
	(*classRef)->setAttributeResist(row, resistance);
	if (resistance == Class::RESISTLEVEL_A)
		item->setIcon(resistLevelA);
	else if (resistance == Class::RESISTLEVEL_B)
		item->setIcon(resistLevelB);
	else if (resistance == Class::RESISTLEVEL_C)
		item->setIcon(resistLevelC);
	else if (resistance == Class::RESISTLEVEL_D)
		item->setIcon(resistLevelD);
	else
		item->setIcon(resistLevelE);
}

void Database_Classes::on_sbSpriteOpacity_valueChanged(int value)
{
	if (classRef != NULL)
		(*classRef)->setSpriteOpacity(value);
	spriteItem->setOpacity(value / 100.0);
}

void Database_Classes::animateSprite()
{
	spriteFrame += spriteFrameDirection;
	if ((spriteFrame == 0 || spriteFrame == spriteNumFrames - 1) && spritePingPong)
		spriteFrameDirection *= -1;
	if (!spritePingPong)
		spriteFrame %= spriteNumFrames;
	spriteItem->setPixmap(spriteImage.copy(spriteFrame * 48, 0, 48, 64));
}

int Database_Classes::getCommand1()
{
	return command1List[cboxCommand1->currentIndex()];
}

void Database_Classes::setCommand1(int value)
{
	ignoreEvents = true;
	cboxCommand1->setCurrentIndex(command1List.indexOf(value));
	updateCommand2List();
	updateCommand3List();
	updateCommand4List();
	updateCommand5List();
	updateCommand6List();
	if (value == 0)
		updateCommand1List();
	ignoreEvents = false;
}

int Database_Classes::getCommand2()
{
	return command2List[cboxCommand2->currentIndex()];
}

void Database_Classes::setCommand2(int value)
{
	ignoreEvents = true;
	cboxCommand2->setCurrentIndex(command2List.indexOf(value));
	updateCommand1List();
	updateCommand3List();
	updateCommand4List();
	updateCommand5List();
	updateCommand6List();
	if (value == 0)
	{
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

int Database_Classes::getCommand3()
{
	return command3List[cboxCommand3->currentIndex()];
}

void Database_Classes::setCommand3(int value)
{
	ignoreEvents = true;
	cboxCommand3->setCurrentIndex(command3List.indexOf(value));
	updateCommand1List();
	updateCommand2List();
	updateCommand4List();
	updateCommand5List();
	updateCommand6List();
	if (value == 0)
	{
		updateCommand3List();
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

int Database_Classes::getCommand4()
{
	return command4List[cboxCommand4->currentIndex()];
}

void Database_Classes::setCommand4(int value)
{
	ignoreEvents = true;
	cboxCommand4->setCurrentIndex(command4List.indexOf(value));
	updateCommand1List();
	updateCommand2List();
	updateCommand3List();
	updateCommand5List();
	updateCommand6List();
	if (value == 0)
	{
		updateCommand4List();
		updateCommand3List();
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

int Database_Classes::getCommand5()
{
	return command5List[cboxCommand5->currentIndex()];
}

void Database_Classes::setCommand5(int value)
{
	ignoreEvents = true;
	cboxCommand5->setCurrentIndex(command5List.indexOf(value));
	updateCommand1List();
	updateCommand2List();
	updateCommand3List();
	updateCommand4List();
	updateCommand6List();
	if (value == 0)
	{
		updateCommand5List();
		updateCommand4List();
		updateCommand3List();
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

int Database_Classes::getCommand6()
{
	return command6List[cboxCommand6->currentIndex()];
}

void Database_Classes::setCommand6(int value)
{
	ignoreEvents = true;
	cboxCommand6->setCurrentIndex(command6List.indexOf(value));
	updateCommand1List();
	updateCommand2List();
	updateCommand3List();
	updateCommand4List();
	updateCommand5List();
	if (value == 0)
	{
		updateCommand6List();
		updateCommand5List();
		updateCommand4List();
		updateCommand3List();
		updateCommand2List();
		updateCommand1List();
	}
	ignoreEvents = false;
}

void Database_Classes::updateClassList()
{
	QListWidgetItem *item;
	int i, numClasses = ProjectData::classNames.size();
	int currentRow = lwClassList->currentRow();
	lwClassList->clear();
	for (i = 0; i < numClasses; ++i)
	{
		item = new QListWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::classNames[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		lwClassList->addItem(item);
	}
	if (currentRow >= lwClassList->count())
		currentRow = lwClassList->count() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::classNames.size() > 0)
	{
		lwClassList->setCurrentRow(currentRow);
		lwClassList->item(currentRow)->setSelected(true);
	}
}

void Database_Classes::updateCharacterSpriteList()
{
	int numCharacterSprites = ProjectData::characterSpriteNames.size();
	cboxSprite->clear();
	for (int i = 0; i < numCharacterSprites; ++i)
		cboxSprite->addItem(ProjectData::characterSpriteNames[i]);
}

void Database_Classes::updateExperienceCurve()
{
	if (classRef != NULL)
	{
		int primary = (*classRef)->getExperiencePrimary();
		int secondary = (*classRef)->getExperienceSecondary();
		int totalExperience = 0;
		int nextLevelExperience = 0;
		for (int i = 0; i < 99; ++i)
		{
			totalExperience += nextLevelExperience;
			nextLevelExperience = (primary * (i + 1)) + secondary;
			gvExperienceGraph->changeValue(i, totalExperience);
		}
		gvExperienceGraph->updateView();
	}
	else if (!ignoreEvents && classRef == NULL)
	{
		for (int i = 0; i < 99; ++i)
			gvExperienceGraph->changeValue(i, 0);
		gvExperienceGraph->updateView();
	}
}

void Database_Classes::updateSkillsList()
{
	QTableWidgetItem *item;
	if (classRef != NULL)
	{
		twSkillList->clearContents();
		twSkillList->setRowCount((*classRef)->numSkillsLearned());
		for (int i = 0; i < (*classRef)->numSkillsLearned(); ++i)
		{
			Class::SkillLearned *skillLearned = (*classRef)->getSkillLearnedPointer(i);
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

void Database_Classes::updateConditionResistList()
{
	QListWidgetItem *item;
	int i, numConditions = ProjectData::conditionNames.size();
	int resistance;
	lwConditionResist->clear();
	for (i = 0; i < numConditions; ++i)
	{
		item = new QListWidgetItem(ProjectData::conditionNames[i]);
		resistance = (*classRef)->getConditionResist(i);
		if (resistance == Class::RESISTLEVEL_A)
			item->setIcon(resistLevelA);
		else if (resistance == Class::RESISTLEVEL_B)
			item->setIcon(resistLevelB);
		else if (resistance == Class::RESISTLEVEL_C)
			item->setIcon(resistLevelC);
		else if (resistance == Class::RESISTLEVEL_D)
			item->setIcon(resistLevelD);
		else
			item->setIcon(resistLevelE);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		item->setSizeHint(QSize(-1, 16));
		lwConditionResist->addItem(item);
	}
}

void Database_Classes::updateAttributeResistList()
{
	QListWidgetItem *item;
	int i, numAttributes = ProjectData::attributeNames.size();
	int resistance;
	lwAttributeResist->clear();
	for (i = 0; i < numAttributes; ++i)
	{
		item = new QListWidgetItem(ProjectData::attributeNames[i]);
		resistance = (*classRef)->getAttributeResist(i);
		if (resistance == Class::RESISTLEVEL_A)
			item->setIcon(resistLevelA);
		else if (resistance == Class::RESISTLEVEL_B)
			item->setIcon(resistLevelB);
		else if (resistance == Class::RESISTLEVEL_C)
			item->setIcon(resistLevelC);
		else if (resistance == Class::RESISTLEVEL_D)
			item->setIcon(resistLevelD);
		else
			item->setIcon(resistLevelE);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		item->setSizeHint(QSize(-1, 16));
		lwAttributeResist->addItem(item);
	}
}

void Database_Classes::updateCommand1List()
{
	int currentCMD = command1List[cboxCommand1->currentIndex()];
	command1List.clear();
	cboxCommand1->clear();
	command1List.append(-1);
	cboxCommand1->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command2List[cboxCommand2->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command1List.append(i);
			cboxCommand1->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	cboxCommand1->setCurrentIndex(command1List.indexOf(currentCMD));
}

void Database_Classes::updateCommand2List()
{
	int currentCMD = command2List[cboxCommand2->currentIndex()];
	command2List.clear();
	cboxCommand2->clear();
	command2List.append(-1);
	cboxCommand2->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command2List.append(i);
			cboxCommand2->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand1->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand2->setEnabled(false);
	}
	else
		cboxCommand2->setEnabled(true);
	cboxCommand2->setCurrentIndex(command2List.indexOf(currentCMD));
}

void Database_Classes::updateCommand3List()
{
	int currentCMD = command3List[cboxCommand3->currentIndex()];
	command3List.clear();
	cboxCommand3->clear();
	command3List.append(-1);
	cboxCommand3->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command2List[cboxCommand2->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command3List.append(i);
			cboxCommand3->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand2->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand3->setEnabled(false);
	}
	else
		cboxCommand3->setEnabled(true);
	cboxCommand3->setCurrentIndex(command3List.indexOf(currentCMD));
}

void Database_Classes::updateCommand4List()
{
	int currentCMD = command4List[cboxCommand4->currentIndex()];
	command4List.clear();
	cboxCommand4->clear();
	command4List.append(-1);
	cboxCommand4->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command2List[cboxCommand2->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command4List.append(i);
			cboxCommand4->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand3->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand4->setEnabled(false);
	}
	else
		cboxCommand4->setEnabled(true);
	cboxCommand4->setCurrentIndex(command4List.indexOf(currentCMD));
}

void Database_Classes::updateCommand5List()
{
	int currentCMD = command5List[cboxCommand5->currentIndex()];
	command5List.clear();
	cboxCommand5->clear();
	command5List.append(-1);
	cboxCommand5->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command2List[cboxCommand2->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command6List[cboxCommand6->currentIndex()] != i)
		{
			command5List.append(i);
			cboxCommand5->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand4->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand5->setEnabled(false);
	}
	else
		cboxCommand5->setEnabled(true);
	cboxCommand5->setCurrentIndex(command5List.indexOf(currentCMD));
}

void Database_Classes::updateCommand6List()
{
	int currentCMD = command6List[cboxCommand6->currentIndex()];
	command6List.clear();
	cboxCommand6->clear();
	command6List.append(-1);
	cboxCommand6->addItem("(NONE)");
	for (int i = 0; i < ProjectData::battleCommands.size(); ++i)
	{
		if (command1List[cboxCommand1->currentIndex()] != i && command2List[cboxCommand2->currentIndex()] != i && command3List[cboxCommand3->currentIndex()] != i && command4List[cboxCommand4->currentIndex()] != i && command5List[cboxCommand5->currentIndex()] != i)
		{
			command6List.append(i);
			cboxCommand6->addItem(ProjectData::battleCommands[i]->name);
		}
	}
	if (cboxCommand5->currentIndex() == 0)
	{
		currentCMD = -1;
		cboxCommand6->setEnabled(false);
	}
	else
		cboxCommand6->setEnabled(true);
	cboxCommand6->setCurrentIndex(command6List.indexOf(currentCMD));
}
