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

#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include "configuremonsterbehavioraction.h"
#include "projectdata.h"
#include "projectdata_monster.h"
#include "projectdata_monsteranimation.h"
#include "variableselector.h"

ConfigureMonsterBehaviorAction::ConfigureMonsterBehaviorAction(int monsterAnimationID, int hueShift, QWidget *parent) : QDialog(parent)
{
	QGraphicsScene *scene;
	setupUi(this);
	animRef = new ProjectData::DataReference<MonsterAnimation>(monsterAnimationID, __FILE__, __LINE__);
	monsterHueShift = hueShift;
	scene = new QGraphicsScene(0.0, 0.0, 0.0, 0.0);
	scene->setBackgroundBrush(QBrush(Qt::magenta));
	gvMonsterAnimation->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	gvMonsterAnimation->setScene(scene);
	monsterItem = new QGraphicsPixmapItem;
	monsterItem->setPos(0.0, 0.0);
	monsterItem->setPixmap(QPixmap());
	scene->addItem(monsterItem);
	wTurnSwitchOn->setEnabled(false);
	wTurnSwitchOff->setEnabled(false);
	cboxSkills->setEnabled(false);
	cboxMonsters->setEnabled(false);
	bPrioritySideButton->setSpinBox(sbPriority);
	bMinMonstersHPSideButton->setSpinBox(sbMinMonstersHP);
	bMaxMonstersHPSideButton->setSpinBox(sbMaxMonstersHP);
	bMinMonstersMPSideButton->setSpinBox(sbMinMonstersMP);
	bMaxMonstersMPSideButton->setSpinBox(sbMaxMonstersMP);
	bMinMonstersSideButton->setSpinBox(sbMinMonsters);
	bMaxMonstersSideButton->setSpinBox(sbMaxMonsters);
	bMinPartysAverageLevelSideButton->setSpinBox(sbMinPartysAverageLevel);
	bMaxPartysAverageLevelSideButton->setSpinBox(sbMaxPartysAverageLevel);
	bMinPartysExhaustionSideButton->setSpinBox(sbMinPartysExhaustion);
	bMaxPartysExhaustionSideButton->setSpinBox(sbMaxPartysExhaustion);
	bFirstTurnSideButton->setSpinBox(sbFirstTurn);
	bEveryTurnSideButton->setSpinBox(sbEveryTurn);
	animationTimer = new QTimer(this);
	monsterFrame = 0;
	monsterFrameDirection = 1;
	QObject::connect(animationTimer, SIGNAL(timeout()), this, SLOT(animateMonster()));
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureMonsterBehaviorAction::~ConfigureMonsterBehaviorAction()
{
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

void ConfigureMonsterBehaviorAction::setupMonsterBehaviorAction()
{
	lwPreconditionSwitchID->addItem(ProjectData::switchVariableIdToString(0));
	lwTurnSwitchOn->addItem(ProjectData::switchVariableIdToString(0));
	lwTurnSwitchOff->addItem(ProjectData::switchVariableIdToString(0));
	updateMonsterAnimationList();
	updateSkillList();
	updateMonsterList();
	loadMonsterAnimation();
	renderMonsterAnimation();
	animationTimer->start(150);
}

void ConfigureMonsterBehaviorAction::setupMonsterBehaviorAction(const Monster::BehaviorAction &action)
{
	updateMonsterAnimationList();
	updateSkillList();
	updateMonsterList();
	cboxPrecondition->setCurrentIndex(action.preconditionType);
	sbPriority->setValue(action.priority);
	if (action.preconditionType == Monster::PRECONDITION_SWITCH)
	{
		lwPreconditionSwitchID->clear();
		lwPreconditionSwitchID->addItem(ProjectData::switchVariableIdToString(action.preconditionValue1));
	}
	else if (action.preconditionType == Monster::PRECONDITION_TURNSELAPSED)
	{
		sbFirstTurn->setValue(action.preconditionValue1);
		sbEveryTurn->setValue(action.preconditionValue2);
	}
	else if (action.preconditionType == Monster::PRECONDITION_MONSTERSPRESENT)
	{
		sbMinMonsters->setValue(action.preconditionValue1);
		sbMaxMonsters->setValue(action.preconditionValue2);
	}
	else if (action.preconditionType == Monster::PRECONDITION_MONSTERSHP)
	{
		sbMinMonstersHP->setValue(action.preconditionValue1);
		sbMaxMonstersHP->setValue(action.preconditionValue2);
	}
	else if (action.preconditionType == Monster::PRECONDITION_MONSTERSMP)
	{
		sbMinMonstersMP->setValue(action.preconditionValue1);
		sbMaxMonstersMP->setValue(action.preconditionValue2);
	}
	else if (action.preconditionType == Monster::PRECONDITION_PARTYSAVERAGELEVEL)
	{
		sbMinPartysAverageLevel->setValue(action.preconditionValue1);
		sbMaxPartysAverageLevel->setValue(action.preconditionValue2);
	}
	else if (action.preconditionType == Monster::PRECONDITION_PARTYSEXHAUSTION)
	{
		sbMinPartysExhaustion->setValue(action.preconditionValue1);
		sbMaxPartysExhaustion->setValue(action.preconditionValue2);
	}
	if (action.turnSwitchOn)
	{
		cbTurnSwitchOn->setChecked(true);
		lwTurnSwitchOn->clear();
		lwTurnSwitchOn->addItem(ProjectData::switchVariableIdToString(action.turnSwitchOnID));
	}
	else
	{
		cbTurnSwitchOn->setChecked(false);
		lwTurnSwitchOn->clear();
		lwTurnSwitchOn->addItem(ProjectData::switchVariableIdToString(action.turnSwitchOnID));
	}
	if (action.turnSwitchOff)
	{
		cbTurnSwitchOff->setChecked(true);
		lwTurnSwitchOff->clear();
		lwTurnSwitchOff->addItem(ProjectData::switchVariableIdToString(action.turnSwitchOffID));
	}
	else
	{
		cbTurnSwitchOff->setChecked(false);
		lwTurnSwitchOff->clear();
		lwTurnSwitchOff->addItem(ProjectData::switchVariableIdToString(action.turnSwitchOffID));
	}
	if (action.actionType == Monster::TYPE_BASIC)
	{
		rbBasic->setChecked(true);
		cboxBasicActions->setCurrentIndex(action.actionID);
	}
	else if (action.actionType == Monster::TYPE_SKILL)
	{
		rbSkill->setChecked(true);
		cboxSkills->setCurrentIndex(action.actionID);
	}
	else
	{
		rbTransform->setChecked(true);
		cboxMonsters->setCurrentIndex(action.actionID);
	}
	cboxMonsterAnimation->setCurrentIndex(action.monsterAnimationID);
	loadMonsterAnimation();
	renderMonsterAnimation();
	animationTimer->start(150);
}

void ConfigureMonsterBehaviorAction::getMonsterBehaviorAction(Monster::BehaviorAction *action)
{
	action->preconditionType = cboxPrecondition->currentIndex();
	action->priority = sbPriority->value();
	if (action->preconditionType == Monster::PRECONDITION_NONE)
	{
		action->preconditionValue1 = 0;
		action->preconditionValue2 = 0;
	}
	else if (action->preconditionType == Monster::PRECONDITION_SWITCH)
	{
		action->preconditionValue1 = ProjectData::switchVariableIdFromString(lwPreconditionSwitchID->item(0)->text());
		action->preconditionValue2 = -1;
	}
	else if (action->preconditionType == Monster::PRECONDITION_TURNSELAPSED)
	{
		action->preconditionValue1 = sbFirstTurn->value();
		action->preconditionValue2 = sbEveryTurn->value();
	}
	else if (action->preconditionType == Monster::PRECONDITION_MONSTERSPRESENT)
	{
		action->preconditionValue1 = sbMinMonsters->value();
		action->preconditionValue2 = sbMaxMonsters->value();
	}
	else if (action->preconditionType == Monster::PRECONDITION_MONSTERSHP)
	{
		action->preconditionValue1 = sbMinMonstersHP->value();
		action->preconditionValue2 = sbMaxMonstersHP->value();
	}
	else if (action->preconditionType == Monster::PRECONDITION_MONSTERSMP)
	{
		action->preconditionValue1 = sbMinMonstersMP->value();
		action->preconditionValue2 = sbMaxMonstersMP->value();
	}
	else if (action->preconditionType == Monster::PRECONDITION_PARTYSAVERAGELEVEL)
	{
		action->preconditionValue1 = sbMinPartysAverageLevel->value();
		action->preconditionValue2 = sbMaxPartysAverageLevel->value();
	}
	else if (action->preconditionType == Monster::PRECONDITION_PARTYSEXHAUSTION)
	{
		action->preconditionValue1 = sbMinPartysExhaustion->value();
		action->preconditionValue2 = sbMaxPartysExhaustion->value();
	}
	action->turnSwitchOn = cbTurnSwitchOn->isChecked();
	action->turnSwitchOnID = ProjectData::switchVariableIdFromString(lwTurnSwitchOn->item(0)->text());
	action->turnSwitchOff = cbTurnSwitchOff->isChecked();
	action->turnSwitchOffID = ProjectData::switchVariableIdFromString(lwTurnSwitchOff->item(0)->text());
	if (rbBasic->isChecked())
	{
		action->actionType = Monster::TYPE_BASIC;
		action->actionID = cboxBasicActions->currentIndex();
	}
	else if (rbSkill->isChecked())
	{
		action->actionType = Monster::TYPE_SKILL;
		action->actionID = cboxSkills->currentIndex();
	}
	else
	{
		action->actionType = Monster::TYPE_TRANSFORM;
		action->actionID = cboxMonsters->currentIndex();
	}
	action->monsterAnimationID = cboxMonsterAnimation->currentIndex();
}

void ConfigureMonsterBehaviorAction::on_cboxPrecondition_currentIndexChanged(int index)
{
	if (index == Monster::PRECONDITION_NONE)
		stackedWidget->setCurrentWidget(wNone);
	else if (index == Monster::PRECONDITION_SWITCH)
		stackedWidget->setCurrentWidget(wSwitch);
	else if (index == Monster::PRECONDITION_TURNSELAPSED)
		stackedWidget->setCurrentWidget(wTurnsElapsed);
	else if (index == Monster::PRECONDITION_MONSTERSPRESENT)
		stackedWidget->setCurrentWidget(wMonstersPresent);
	else if (index == Monster::PRECONDITION_MONSTERSHP)
		stackedWidget->setCurrentWidget(wMonstersHP);
	else if (index == Monster::PRECONDITION_MONSTERSMP)
		stackedWidget->setCurrentWidget(wMonstersMP);
	else if (index == Monster::PRECONDITION_PARTYSAVERAGELEVEL)
		stackedWidget->setCurrentWidget(wPartysAverageLevel);
	else if (index == Monster::PRECONDITION_PARTYSEXHAUSTION)
		stackedWidget->setCurrentWidget(wPartysExhaustion);
}

void ConfigureMonsterBehaviorAction::on_lwPreconditionSwitchID_itemDoubleClicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lwPreconditionSwitchID->item(0)->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		int switchID = variableSelector->getVariableID();
		lwPreconditionSwitchID->clear();
		lwPreconditionSwitchID->addItem(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureMonsterBehaviorAction::on_tbBrowsePreconditionSwitch_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lwPreconditionSwitchID->item(0)->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		int switchID = variableSelector->getVariableID();
		lwPreconditionSwitchID->clear();
		lwPreconditionSwitchID->addItem(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureMonsterBehaviorAction::on_cbTurnSwitchOn_toggled(bool on)
{
	wTurnSwitchOn->setEnabled(on);
}

void ConfigureMonsterBehaviorAction::on_lwTurnSwitchOn_itemDoubleClicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lwTurnSwitchOn->item(0)->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		int switchID = variableSelector->getVariableID();
		lwTurnSwitchOn->clear();
		lwTurnSwitchOn->addItem(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureMonsterBehaviorAction::on_tbBrowseTurnSwitchOn_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lwTurnSwitchOn->item(0)->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		int switchID = variableSelector->getVariableID();
		lwTurnSwitchOn->clear();
		lwTurnSwitchOn->addItem(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureMonsterBehaviorAction::on_cbTurnSwitchOff_toggled(bool on)
{
	wTurnSwitchOff->setEnabled(on);
}

void ConfigureMonsterBehaviorAction::on_lwTurnSwitchOff_itemDoubleClicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lwTurnSwitchOff->item(0)->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		int switchID = variableSelector->getVariableID();
		lwTurnSwitchOff->clear();
		lwTurnSwitchOff->addItem(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureMonsterBehaviorAction::on_tbBrowseTurnSwitchOff_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(lwTurnSwitchOff->item(0)->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		int switchID = variableSelector->getVariableID();
		lwTurnSwitchOff->clear();
		lwTurnSwitchOff->addItem(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void ConfigureMonsterBehaviorAction::on_rbBasic_toggled(bool on)
{
	cboxBasicActions->setEnabled(on);
}

void ConfigureMonsterBehaviorAction::on_rbSkill_toggled(bool on)
{
	cboxSkills->setEnabled(on);
}

void ConfigureMonsterBehaviorAction::on_rbTransform_toggled(bool on)
{
	cboxMonsters->setEnabled(on);
}

void ConfigureMonsterBehaviorAction::on_cboxMonsterAnimation_currentIndexChanged()
{
	animationTimer->stop();
	monsterFrame = 0;
	monsterFrameDirection = 1;
	loadMonsterAnimation();
	renderMonsterAnimation();
	animationTimer->start(150);
}

void ConfigureMonsterBehaviorAction::animateMonster()
{
	if (!monsterImage.isNull())
	{
		int animationID = cboxMonsterAnimation->currentIndex();
		if ((*animRef)->getPoseNumFrames(animationID) > 1)
		{
			monsterFrame += monsterFrameDirection;
			if (monsterFrame == (*animRef)->getPoseNumFrames(animationID) - 1)
				monsterFrameDirection *= -1;
			renderMonsterAnimation();
		}
	}
}

void ConfigureMonsterBehaviorAction::renderMonsterAnimation()
{
	if (!monsterImage.isNull())
	{
		int animationID = cboxMonsterAnimation->currentIndex();
		int numFrames = (*animRef)->getPoseNumFrames(animationID);
		int frameSize = monsterImage.width() / numFrames;
		QPixmap scaledPixmap = QPixmap::fromImage(monsterImage.copy(monsterFrame *frameSize, 0, frameSize, monsterImage.height())).scaled(231, 160, Qt::KeepAspectRatio);
		gvMonsterAnimation->setSceneRect(0.0, 0.0, scaledPixmap.width(), scaledPixmap.height());
		monsterItem->setPixmap(scaledPixmap);
	}
}

void ConfigureMonsterBehaviorAction::loadMonsterAnimation()
{
	QColor pixelRGB, pixelHSV, tcolor = qRgb(255, 0, 255);
	int animationID = cboxMonsterAnimation->currentIndex();
	monsterImage = QImage((*animRef)->getPoseLocation(animationID)).convertToFormat(QImage::Format_ARGB32);
	if (monsterImage.isNull())
		monsterItem->setPixmap(QPixmap());
	else
	{
		for (int y = 0; y < monsterImage.height(); ++y)
		{
			for (int x = 0; x < monsterImage.width(); ++x)
			{
				pixelRGB = QColor::fromRgba(monsterImage.pixel(x, y));
				if (pixelRGB == tcolor)
					monsterImage.setPixel(x, y, qRgba(pixelRGB.red(), pixelRGB.green(), pixelRGB.blue(), 0));
				else
				{
					pixelHSV = pixelRGB.toHsv();
					pixelHSV.setHsv((pixelHSV.hue() + monsterHueShift) % 360, pixelHSV.saturation(), pixelHSV.value(), pixelHSV.alpha());
					pixelRGB = pixelHSV.toRgb();
					monsterImage.setPixel(x, y, pixelRGB.rgba());
				}
			}
		}
	}
}

void ConfigureMonsterBehaviorAction::updateMonsterAnimationList()
{
	cboxMonsterAnimation->clear();
	for (int i = 0; i < 32; ++i)
		cboxMonsterAnimation->addItem((*animRef)->getPoseName(i));
}

void ConfigureMonsterBehaviorAction::updateSkillList()
{
	int numSkills = ProjectData::skillNames.size();
	cboxSkills->clear();
	for (int i = 0; i < numSkills; ++i)
		cboxSkills->addItem(ProjectData::skillNames[i]);
}

void ConfigureMonsterBehaviorAction::updateMonsterList()
{
	int numMonsters = ProjectData::monsterNames.size();
	cboxMonsters->clear();
	for (int i = 0; i < numMonsters; ++i)
		cboxMonsters->addItem(ProjectData::monsterNames[i]);
}
