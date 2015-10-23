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
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QMessageBox>
#include <QHeaderView>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "clipboard.h"
#include "configureitemdrops.h"
#include "configuremonsterbehavioraction.h"
#include "database_monsters.h"
#include "projectdata.h"
#include "projectdata_monster.h"
#include "projectdata_monsteranimation.h"
#include "xmlParser.h"

Database_Monsters::Database_Monsters(QWidget *parent) : QWidget(parent)
{
	QGraphicsScene *scene;
	ignoreEvents = true;
	setupUi(this);
	animationTimer = new QTimer(this);
	QObject::connect(animationTimer, SIGNAL(timeout()), this, SLOT(animateMonster()));
	bMaxHPSideButton->setSpinBox(sbMaxHP);
	bMaxMPSideButton->setSpinBox(sbMaxMP);
	bAttackSideButton->setSpinBox(sbAttack);
	bDefenseSideButton->setSpinBox(sbDefense);
	bWisdomSideButton->setSpinBox(sbWisdom);
	bAgilitySideButton->setSpinBox(sbAgility);
	bExperienceSideButton->setSpinBox(sbExperience);
	bMoneySideButton->setSpinBox(sbMoney);
	bCriticalHitRateSideButton->setDoubleSpinBox(dsbCriticalHitRate);
	bCriticalHitMultiplierSideButton->setDoubleSpinBox(dsbCriticalHitMultiplier);
	scene = new QGraphicsScene(0.0, 0.0, 0.0, 0.0);
	scene->setBackgroundBrush(QBrush(Qt::magenta));
	gvMonsterGraphic->setBackgroundBrush(QBrush(QImage(":/icons/graphicsview_background.png")));
	gvMonsterGraphic->setScene(scene);
	monsterItem = new QGraphicsPixmapItem;
	monsterItem->setPos(0.0, 0.0);
	monsterItem->setPixmap(QPixmap());
	scene->addItem(monsterItem);
	gvHueShift->setScene(new QGraphicsScene(0.0, 0.0, 180.0, 8.0));
	hueShiftItem1 = new QGraphicsPixmapItem(QPixmap(":/images/hueShift.png"));
	hueShiftItem1->setFlags(0x0);
	hueShiftItem1->setPos(-180.0, 0.0);
	hueShiftItem2 = new QGraphicsPixmapItem(QPixmap(":/images/hueShift.png"));
	hueShiftItem2->setFlags(0x0);
	hueShiftItem2->setPos(0.0, 0.0);
	gvHueShift->scene()->addItem(hueShiftItem1);
	gvHueShift->scene()->addItem(hueShiftItem2);
	monsterRef = NULL;
	animRef = NULL;
	monsterFrame = 0;
	monsterFrameDirection = 1;
	resistLevelA = QIcon(":/icons/resistLevelA.png");
	resistLevelB = QIcon(":/icons/resistLevelB.png");
	resistLevelC = QIcon(":/icons/resistLevelC.png");
	resistLevelD = QIcon(":/icons/resistLevelD.png");
	resistLevelE = QIcon(":/icons/resistLevelE.png");
	twBehavior->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	twBehavior->horizontalHeader()->setResizeMode(1, QHeaderView::Interactive);
	twBehavior->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
	twBehavior->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

Database_Monsters::~Database_Monsters()
{
	if (monsterRef != NULL)
	{
		delete monsterRef;
		monsterRef = NULL;
	}
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

void Database_Monsters::setupMonsters()
{
	updateMonsterAnimationList();
	updateMonsterList();
	ignoreEvents = false;
}

void Database_Monsters::shutdownMonsters()
{
	ignoreEvents = true;
	animationTimer->stop();
	if (monsterRef != NULL)
	{
		delete monsterRef;
		monsterRef = NULL;
	}
	if (animRef != NULL)
	{
		delete animRef;
		animRef = NULL;
	}
}

void Database_Monsters::on_lwMonsterList_itemSelectionChanged()
{
	QString location;
	int monsterID = lwMonsterList->currentRow();
	animationTimer->stop();
	if (monsterRef != NULL)
	{
		delete monsterRef;
		monsterRef = NULL;
	}
	if (monsterID >= 0)
	{
		monsterRef = new ProjectData::DataReference<Monster>(monsterID, __FILE__, __LINE__);
		animRef = new ProjectData::DataReference<MonsterAnimation>((*monsterRef)->getMonsterAnimationID(), __FILE__, __LINE__);
		leName->setText((*monsterRef)->getName());
		sbMaxHP->setValue((*monsterRef)->getMaxHP());
		sbMaxMP->setValue((*monsterRef)->getMaxMP());
		sbAttack->setValue((*monsterRef)->getAttack());
		sbDefense->setValue((*monsterRef)->getDefense());
		sbWisdom->setValue((*monsterRef)->getWisdom());
		sbAgility->setValue((*monsterRef)->getAgility());
		cboxMonsterAnimation->setCurrentIndex((*monsterRef)->getMonsterAnimationID());
		cbTransparent->setChecked((*monsterRef)->isTransparent());
		cbFlying->setChecked((*monsterRef)->isFlying());
		sldrHueShift->setValue((*monsterRef)->getHueShift());
		sbExperience->setValue((*monsterRef)->getExperienceGain());
		sbMoney->setValue((*monsterRef)->getMoneyAquired());
		sbItemProbability->setValue((*monsterRef)->getItemDropProbability());
		cbCriticalHits->setChecked((*monsterRef)->isCriticalHits());
		dsbCriticalHitRate->setValue((*monsterRef)->getCriticalHitRate());
		dsbCriticalHitMultiplier->setValue((*monsterRef)->getCriticalHitMultiplier());
		cbAttacksOftenMiss->setChecked((*monsterRef)->getAttacksOftenMiss());
		originalMonsterImage = QImage((*animRef)->getPoseLocation(MonsterAnimation::POSE_IDLESTANCE)).convertToFormat(QImage::Format_ARGB32);
		hueShiftedMonsterImage = QImage(originalMonsterImage.width(), originalMonsterImage.height(), QImage::Format_ARGB32);
		updateMonsterImage();
		renderMonsterImage();
		updateBehaviorList();
		updateConditionResistList();
		updateAttributeResistList();
		monsterFrame = 0;
		monsterFrameDirection = 1;
		animationTimer->start(1000 / (*animRef)->getPoseFPS(MonsterAnimation::POSE_IDLESTANCE));
		pingPongAnimation = (*animRef)->isPosePingPong(MonsterAnimation::POSE_IDLESTANCE);
	}
}

void Database_Monsters::on_bArraySize_clicked()
{
	int numMonsters = ProjectData::monsterNames.size();
	ArraySize *arraysize = new ArraySize(numMonsters, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numMonsters)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (monsterRef != NULL)
				{
					if (newSize <= monsterRef->getDataID())
					{
						delete monsterRef;
						monsterRef = NULL;
					}
				}
				for (int i = numMonsters - 1; i >= newSize; --i)
					ProjectData::deleteMonster(i);
			}
		}
		else
		{
			for (int i = numMonsters; i < newSize; ++i)
			{
				Monster *monster = new Monster;
				monster->setName(QString("Mon%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addMonster(monster);
				delete monster;
			}
		}
		ignoreEvents = true;
		updateMonsterList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_Monsters::on_leName_textChanged(QString text)
{
	int currentRow = lwMonsterList->currentRow();
	ignoreEvents = true;
	lwMonsterList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 4, 10, QChar('0')).arg(text));
	ProjectData::monsterNames[currentRow] = text;
	(*monsterRef)->setName(text);
	ignoreEvents = false;
}

void Database_Monsters::on_sbMaxHP_valueChanged(int value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setMaxHP(value);
}

void Database_Monsters::on_sbMaxMP_valueChanged(int value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setMaxMP(value);
}

void Database_Monsters::on_sbAttack_valueChanged(int value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setAttack(value);
}

void Database_Monsters::on_sbDefense_valueChanged(int value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setDefense(value);
}

void Database_Monsters::on_sbWisdom_valueChanged(int value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setWisdom(value);
}

void Database_Monsters::on_sbAgility_valueChanged(int value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setAgility(value);
}

void Database_Monsters::on_cboxMonsterAnimation_currentIndexChanged(int value)
{
	if (monsterRef != NULL && !ignoreEvents)
	{
		(*monsterRef)->setMonsterAnimationID(value);
		monsterFrame = 0;
		monsterFrameDirection = 1;
		animationTimer->stop();
		if (animRef != NULL)
			delete animRef;
		animRef = new ProjectData::DataReference<MonsterAnimation>(value, __FILE__, __LINE__);
		originalMonsterImage = QImage((*animRef)->getPoseLocation(MonsterAnimation::POSE_IDLESTANCE)).convertToFormat(QImage::Format_ARGB32);
		hueShiftedMonsterImage = QImage(originalMonsterImage.width(), originalMonsterImage.height(), QImage::Format_ARGB32);
		updateMonsterImage();
		renderMonsterImage();
		animationTimer->start(1000 / (*animRef)->getPoseFPS(MonsterAnimation::POSE_IDLESTANCE));
		pingPongAnimation = (*animRef)->isPosePingPong(MonsterAnimation::POSE_IDLESTANCE);
	}
}

void Database_Monsters::on_cbTransparent_toggled(bool on)
{
	if (monsterRef != NULL)
	{
		(*monsterRef)->setIsTransparent(on);
		monsterItem->setOpacity((on) ? 0.5:1.0);
	}
}

void Database_Monsters::on_cbFlying_toggled(bool on)
{
	if (monsterRef != NULL)
		(*monsterRef)->setIsFlying(on);
}

void Database_Monsters::on_sldrHueShift_valueChanged(int value)
{
	hueShiftItem1->setPos((float)((value / 2) - 180), 0.0);
	hueShiftItem2->setPos((float)(value / 2), 0.0);
	if (monsterRef != NULL && !ignoreEvents)
	{
		(*monsterRef)->setHueShift(value);
		updateMonsterImage();
		renderMonsterImage();
	}
}

void Database_Monsters::on_sbExperience_valueChanged(int value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setExperienceGain(value);
}

void Database_Monsters::on_sbMoney_valueChanged(int value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setMoneyAquired(value);
}

void Database_Monsters::on_sbItemProbability_valueChanged(int value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setItemDropProbability(value);
}

void Database_Monsters::on_bConfigureItems_clicked()
{
	ConfigureItemDrops *configureItems = new ConfigureItemDrops;
	configureItems->setupItemDrops(monsterRef->getDataID());
	if (configureItems->exec())
		configureItems->getItemDrops(monsterRef->getDataID());
	delete configureItems;
}

void Database_Monsters::on_cbCriticalHits_toggled(bool on)
{
	if (monsterRef != NULL)
		(*monsterRef)->setCriticalHits(on);
	wCriticalHits->setEnabled(on);
}

void Database_Monsters::on_dsbCriticalHitRate_valueChanged(double value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setCriticalHitRate(value);
}

void Database_Monsters::on_dsbCriticalHitMultiplier_valueChanged(double value)
{
	if (monsterRef != NULL)
		(*monsterRef)->setCriticalHitMultiplier(value);
}

void Database_Monsters::on_cbAttacksOftenMiss_toggled(bool on)
{
	if (monsterRef != NULL)
		(*monsterRef)->setAttacksOftenMiss(on);
}

void Database_Monsters::on_bConfigureBattleEvent_clicked()
{
	QMessageBox::information(window(), "Not Implemented", "This feature is not yet implemented.");
	TODO("Implement function");
}

void Database_Monsters::on_bConfigureAI_clicked()
{
	QMessageBox::information(window(), "Not Implemented", "This feature is not yet implemented.");
	TODO("Implement function");
}

void Database_Monsters::on_lwConditionResist_itemClicked(QListWidgetItem *item)
{
	int row = lwConditionResist->row(item);
	int resistance = (*monsterRef)->getConditionResist(row);
	lwConditionResist->setCurrentRow(-1);
	resistance = (resistance + 1) % 5;
	(*monsterRef)->setConditionResist(row, resistance);
	if (resistance == Monster::RESISTLEVEL_A)
		item->setIcon(resistLevelA);
	else if (resistance == Monster::RESISTLEVEL_B)
		item->setIcon(resistLevelB);
	else if (resistance == Monster::RESISTLEVEL_C)
		item->setIcon(resistLevelC);
	else if (resistance == Monster::RESISTLEVEL_D)
		item->setIcon(resistLevelD);
	else
		item->setIcon(resistLevelE);
}

void Database_Monsters::on_lwAttributeResist_itemClicked(QListWidgetItem *item)
{
	int row = lwAttributeResist->row(item);
	int resistance = (*monsterRef)->getAttributeResist(row);
	lwAttributeResist->setCurrentRow(-1);
	resistance = (resistance + 1) % 5;
	(*monsterRef)->setAttributeResist(row, resistance);
	if (resistance == Monster::RESISTLEVEL_A)
		item->setIcon(resistLevelA);
	else if (resistance == Monster::RESISTLEVEL_B)
		item->setIcon(resistLevelB);
	else if (resistance == Monster::RESISTLEVEL_C)
		item->setIcon(resistLevelC);
	else if (resistance == Monster::RESISTLEVEL_D)
		item->setIcon(resistLevelD);
	else
		item->setIcon(resistLevelE);
}

void Database_Monsters::on_twBehavior_itemDoubleClicked()
{
	QTableWidgetItem *item;
	Monster::BehaviorAction action;
	ConfigureMonsterBehaviorAction *configureBehaviorAction = new ConfigureMonsterBehaviorAction(cboxMonsterAnimation->currentIndex(), sldrHueShift->value());
	int actionID = twBehavior->currentRow();
	(*monsterRef)->getBehaviorAction(actionID, &action);
	configureBehaviorAction->setupMonsterBehaviorAction(action);
	if (configureBehaviorAction->exec())
	{
		configureBehaviorAction->getMonsterBehaviorAction(&action);
		(*monsterRef)->setBehaviorAction(actionID, action);
		item = twBehavior->item(actionID, 0);
		if (action.actionType == Monster::TYPE_BASIC)
		{
			if (action.actionID == Monster::ACTION_ATTACK)
				item->setText("Attack");
			else if (action.actionID == Monster::ACTION_DOUBLEATTACK)
				item->setText("Double Attack");
			else if (action.actionID == Monster::ACTION_DEFEND)
				item->setText("Defend");
			else if (action.actionID == Monster::ACTION_OBSERVEBATTLE)
				item->setText("Observe Battle");
			else if (action.actionID == Monster::ACTION_CHARGEUP)
				item->setText("Charge Up");
			else if (action.actionID == Monster::ACTION_SELFDESTRUCT)
				item->setText("Self-Destruct");
			else if (action.actionID == Monster::ACTION_ESCAPE)
				item->setText("Escape");
			else if (action.actionID == Monster::ACTION_NOACTION)
				item->setText("No Action");
		}
		else if (action.actionType == Monster::TYPE_SKILL)
			item->setText(QString("Use Skill: %1").arg(ProjectData::skillNames[action.actionID]));
		else if (action.actionType == Monster::TYPE_TRANSFORM)
			item->setText(QString("Transform into %1").arg(ProjectData::monsterNames[action.actionID]));
		item = twBehavior->item(actionID, 1);
		if (action.preconditionType == Monster::PRECONDITION_NONE)
			item->setText("None");
		else if (action.preconditionType == Monster::PRECONDITION_SWITCH)
			item->setText(QString("Switch[%1] is on.").arg(ProjectData::switchVariableIdToString(action.preconditionValue1)));
		else if (action.preconditionType == Monster::PRECONDITION_TURNSELAPSED)
			item->setText(QString("Turns elapsed (first: %1, every: %2).").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_MONSTERSPRESENT)
			item->setText(QString("At least %1 to %2 monsters are present.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_MONSTERSHP)
			item->setText(QString("Monster's HP is between %1 and %2.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_MONSTERSMP)
			item->setText(QString("Monster's MP is between %1 and %2.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_PARTYSAVERAGELEVEL)
			item->setText(QString("Party's average level is between %1 and %2.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_PARTYSEXHAUSTION)
			item->setText(QString("Party's exhaustion is between %1% and %2%.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		item = twBehavior->item(actionID, 2);
		item->setText(QString::number(action.priority));
	}
	delete configureBehaviorAction;
}

void Database_Monsters::on_tbDeleteBehavior_clicked()
{
	int actionID = twBehavior->currentRow();
	(*monsterRef)->deleteBehaviorAction(actionID);
	twBehavior->removeRow(actionID);
}

void Database_Monsters::on_tbAddBehavior_clicked()
{
	QTableWidgetItem *item;
	Monster::BehaviorAction action;
	ConfigureMonsterBehaviorAction *configureBehaviorAction = new ConfigureMonsterBehaviorAction(cboxMonsterAnimation->currentIndex(), sldrHueShift->value());
	configureBehaviorAction->setupMonsterBehaviorAction();
	if (configureBehaviorAction->exec())
	{
		int actionID = (*monsterRef)->numBehaviorActions();
		configureBehaviorAction->getMonsterBehaviorAction(&action);
		(*monsterRef)->addBehaviorAction(action);
		twBehavior->setRowCount(twBehavior->rowCount() + 1);
		item = new QTableWidgetItem;
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		if (action.actionType == Monster::TYPE_BASIC)
		{
			if (action.actionID == Monster::ACTION_ATTACK)
				item->setText("Attack");
			else if (action.actionID == Monster::ACTION_DOUBLEATTACK)
				item->setText("Double Attack");
			else if (action.actionID == Monster::ACTION_DEFEND)
				item->setText("Defend");
			else if (action.actionID == Monster::ACTION_OBSERVEBATTLE)
				item->setText("Observe Battle");
			else if (action.actionID == Monster::ACTION_CHARGEUP)
				item->setText("Charge Up");
			else if (action.actionID == Monster::ACTION_SELFDESTRUCT)
				item->setText("Self-Destruct");
			else if (action.actionID == Monster::ACTION_ESCAPE)
				item->setText("Escape");
			else if (action.actionID == Monster::ACTION_NOACTION)
				item->setText("No Action");
		}
		else if (action.actionType == Monster::TYPE_SKILL)
			item->setText(QString("Use Skill: %1").arg(ProjectData::skillNames[action.actionID]));
		else if (action.actionType == Monster::TYPE_TRANSFORM)
			item->setText(QString("Transform into %1").arg(ProjectData::monsterNames[action.actionID]));
		twBehavior->setItem(actionID, 0, item);
		item = new QTableWidgetItem;
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		if (action.preconditionType == Monster::PRECONDITION_NONE)
			item->setText("None");
		else if (action.preconditionType == Monster::PRECONDITION_SWITCH)
			item->setText(QString("Switch[%1] is on.").arg(ProjectData::switchVariableIdToString(action.preconditionValue1)));
		else if (action.preconditionType == Monster::PRECONDITION_TURNSELAPSED)
			item->setText(QString("Turns elapsed (first: %1, every: %2).").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_MONSTERSPRESENT)
			item->setText(QString("At least %1 to %2 monsters are present.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_MONSTERSHP)
			item->setText(QString("Monster's HP is between %1 and %2.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_MONSTERSMP)
			item->setText(QString("Monster's MP is between %1 and %2.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_PARTYSAVERAGELEVEL)
			item->setText(QString("Party's average level is between %1 and %2.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_PARTYSEXHAUSTION)
			item->setText(QString("Party's exhaustion is between %1% and %2%.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		twBehavior->setItem(actionID, 1, item);
		item = new QTableWidgetItem(QString::number(action.priority));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		twBehavior->setItem(actionID, 2, item);
	}
	delete configureBehaviorAction;
}

void Database_Monsters::animateMonster()
{
	if (animRef != NULL)
	{
		int numFrames = (*animRef)->getPoseNumFrames(MonsterAnimation::POSE_IDLESTANCE);
		if (numFrames > 1)
		{
			monsterFrame += monsterFrameDirection;
			if (pingPongAnimation && (monsterFrame == 0 || monsterFrame == numFrames - 1))
				monsterFrameDirection *= -1;
			else
				monsterFrame %= numFrames;
			renderMonsterImage();
		}
	}
}

void Database_Monsters::updateMonsterImage()
{
	QColor pixelRGB, pixelHSV;
	int hueShift = sldrHueShift->value();
	if (animRef != NULL)
	{
		for (int y = 0; y < originalMonsterImage.height(); ++y)
		{
			for (int x = 0; x < originalMonsterImage.width(); ++x)
			{
				pixelRGB = QColor::fromRgba(originalMonsterImage.pixel(x, y));
				pixelHSV = pixelRGB.toHsv();
				pixelHSV.setHsv((pixelHSV.hue() + hueShift) % 360, pixelHSV.saturation(), pixelHSV.value(), pixelHSV.alpha());
				pixelRGB = pixelHSV.toRgb();
				hueShiftedMonsterImage.setPixel(x, y, pixelRGB.rgba());
			}
		}
	}
}

void Database_Monsters::renderMonsterImage()
{
	QPixmap scaledPixmap;
	if (animRef != NULL)
	{
		int numFrames = (*animRef)->getPoseNumFrames(MonsterAnimation::POSE_IDLESTANCE);
		int frameSize = hueShiftedMonsterImage.width() / numFrames;
		scaledPixmap = QPixmap::fromImage(hueShiftedMonsterImage.copy(monsterFrame * frameSize, 0, frameSize, hueShiftedMonsterImage.height())).scaled(200, 160, Qt::KeepAspectRatio);
		gvMonsterGraphic->setSceneRect(0.0, 0.0, scaledPixmap.width(), scaledPixmap.height());
		monsterItem->setPixmap(scaledPixmap);
	}
}

void Database_Monsters::updateMonsterList()
{
	QListWidgetItem *item;
	int i, numMonsters = ProjectData::monsterNames.size();
	int currentRow = lwMonsterList->currentRow();
	lwMonsterList->clear();
	for (i = 0; i < numMonsters; ++i)
	{
		item = new QListWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::monsterNames[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		lwMonsterList->addItem(item);
	}
	if (currentRow >= lwMonsterList->count())
		currentRow = lwMonsterList->count() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::monsterNames.size() > 0)
	{
		lwMonsterList->setCurrentRow(currentRow);
		lwMonsterList->item(currentRow)->setSelected(true);
	}
}

void Database_Monsters::updateMonsterAnimationList()
{
	int numMonsterAnimations = ProjectData::monsterAnimationNames.size();
	cboxMonsterAnimation->clear();
	for (int i = 0; i < numMonsterAnimations; ++i)
		cboxMonsterAnimation->addItem(ProjectData::monsterAnimationNames[i]);
}

void Database_Monsters::updateBehaviorList()
{
	QTableWidgetItem *item;
	Monster::BehaviorAction action;
	int numBehaviorActions = (*monsterRef)->numBehaviorActions();
	twBehavior->clearContents();
	twBehavior->setRowCount(numBehaviorActions);
	for (int i = 0; i < numBehaviorActions; ++i)
	{
		(*monsterRef)->getBehaviorAction(i, &action);
		item = new QTableWidgetItem;
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		if (action.actionType == Monster::TYPE_BASIC)
		{
			if (action.actionID == Monster::ACTION_ATTACK)
				item->setText("Attack");
			else if (action.actionID == Monster::ACTION_DOUBLEATTACK)
				item->setText("Double Attack");
			else if (action.actionID == Monster::ACTION_DEFEND)
				item->setText("Defend");
			else if (action.actionID == Monster::ACTION_OBSERVEBATTLE)
				item->setText("Observe Battle");
			else if (action.actionID == Monster::ACTION_CHARGEUP)
				item->setText("Charge Up");
			else if (action.actionID == Monster::ACTION_SELFDESTRUCT)
				item->setText("Self-Destruct");
			else if (action.actionID == Monster::ACTION_ESCAPE)
				item->setText("Escape");
			else if (action.actionID == Monster::ACTION_NOACTION)
				item->setText("No Action");
		}
		else if (action.actionType == Monster::TYPE_SKILL)
			item->setText(QString("Use Skill: %1").arg(ProjectData::skillNames[action.actionID]));
		else if (action.actionType == Monster::TYPE_TRANSFORM)
			item->setText(QString("Transform into %1").arg(ProjectData::monsterNames[action.actionID]));
		twBehavior->setItem(i, 0, item);
		item = new QTableWidgetItem;
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		if (action.preconditionType == Monster::PRECONDITION_NONE)
			item->setText("None");
		else if (action.preconditionType == Monster::PRECONDITION_SWITCH)
			item->setText(QString("Switch[%1] is on.").arg(ProjectData::switchVariableIdToString(action.preconditionValue1)));
		else if (action.preconditionType == Monster::PRECONDITION_TURNSELAPSED)
			item->setText(QString("Turns elapsed (first: %1, every: %2).").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_MONSTERSPRESENT)
			item->setText(QString("At least %1 to %2 monsters are present.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_MONSTERSHP)
			item->setText(QString("Monster's HP is between %1 and %2.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_MONSTERSMP)
			item->setText(QString("Monster's MP is between %1 and %2.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_PARTYSAVERAGELEVEL)
			item->setText(QString("Party's average level is between %1 and %2.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		else if (action.preconditionType == Monster::PRECONDITION_PARTYSEXHAUSTION)
			item->setText(QString("Party's exhaustion is between %1% and %2%.").arg(action.preconditionValue1).arg(action.preconditionValue2));
		twBehavior->setItem(i, 1, item);
		item = new QTableWidgetItem(QString::number(action.priority));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		twBehavior->setItem(i, 2, item);
	}
}

void Database_Monsters::updateConditionResistList()
{
	QListWidgetItem *item;
	int i, numConditions = ProjectData::conditionNames.size();
	int resistance;
	lwConditionResist->clear();
	for (i = 0; i < numConditions; ++i)
	{
		item = new QListWidgetItem(ProjectData::conditionNames[i]);
		resistance = (*monsterRef)->getConditionResist(i);
		if (resistance == Monster::RESISTLEVEL_A)
			item->setIcon(resistLevelA);
		else if (resistance == Monster::RESISTLEVEL_B)
			item->setIcon(resistLevelB);
		else if (resistance == Monster::RESISTLEVEL_C)
			item->setIcon(resistLevelC);
		else if (resistance == Monster::RESISTLEVEL_D)
			item->setIcon(resistLevelD);
		else
			item->setIcon(resistLevelE);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		item->setSizeHint(QSize(-1, 16));
		lwConditionResist->addItem(item);
	}
}

void Database_Monsters::updateAttributeResistList()
{
	QListWidgetItem *item;
	int i, numAttributes = ProjectData::attributeNames.size();
	int resistance;
	lwAttributeResist->clear();
	for (i = 0; i < numAttributes; ++i)
	{
		item = new QListWidgetItem(ProjectData::attributeNames[i]);
		resistance = (*monsterRef)->getAttributeResist(i);
		if (resistance == Monster::RESISTLEVEL_A)
			item->setIcon(resistLevelA);
		else if (resistance == Monster::RESISTLEVEL_B)
			item->setIcon(resistLevelB);
		else if (resistance == Monster::RESISTLEVEL_C)
			item->setIcon(resistLevelC);
		else if (resistance == Monster::RESISTLEVEL_D)
			item->setIcon(resistLevelD);
		else
			item->setIcon(resistLevelE);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		item->setSizeHint(QSize(-1, 16));
		lwAttributeResist->addItem(item);
	}
}
