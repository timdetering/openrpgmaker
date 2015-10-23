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
#include <QGraphicsScene>
#include <QTimer>
#include <QIcon>
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "clipboard.h"
#include "database_conditions.h"
#include "imagebrowser.h"
#include "projectdata.h"
#include "projectdata_character.h"
#include "projectdata_charactersprite.h"
#include "projectdata_condition.h"
#include "projectdata_monster.h"
#include "xmlParser.h"

Database_Conditions::Database_Conditions(QWidget *parent) : QWidget(parent)
{
	ignoreEvents = true;
	QGraphicsScene *scene;
	setupUi(this);
	animationTimer = new QTimer(this);
	QObject::connect(animationTimer, SIGNAL(timeout()), this, SLOT(animateSprite()));
	scene = new QGraphicsScene(0.0, 0.0, 96.0, 96.0);
	scene->setBackgroundBrush(QBrush(Qt::magenta));
	animationItem = new QGraphicsPixmapItem(QPixmap());
	animationItem->setPos(0.0, 0.0);
	animationItem->setFlags(0x0);
	scene->addItem(animationItem);
	gvAnimationPreview->setScene(scene);
	bGroupSideButton->setSpinBox(sbGroup);
	bPrioritySideButton->setSpinBox(sbPriority);
	bSusceptibilityASideButton->setSpinBox(sbSusceptibilityA);
	bSusceptibilityBSideButton->setSpinBox(sbSusceptibilityB);
	bSusceptibilityCSideButton->setSpinBox(sbSusceptibilityC);
	bSusceptibilityDSideButton->setSpinBox(sbSusceptibilityD);
	bSusceptibilityESideButton->setSpinBox(sbSusceptibilityE);
	bNumTurnsSideButton->setSpinBox(sbNumTurns);
	bPercentEachTurnSideButton->setSpinBox(sbPercentEachTurn);
	bPhysicalDamageRecoveryChanceSideButton->setSpinBox(sbPhysicalDamageRecoveryChance);
	bAttackAlterationSideButton->setSpinBox(sbAttackAlteration);
	bDefenseAlterationSideButton->setSpinBox(sbDefenseAlteration);
	bWisdomAlterationSideButton->setSpinBox(sbWisdomAlteration);
	bAgilityAlterationSideButton->setSpinBox(sbAgilityAlteration);
	bHitPercentageSideButton->setSpinBox(sbHitPercentage);
	bAttackInfluenceSideButton->setSpinBox(sbAttackInfluence);
	bWisdomInfluenceSideButton->setSpinBox(sbWisdomInfluence);
	bAnimationSideButton->setSpinBox(sbAnimation);
	hpEffect_bPercentPerTurnSideButton->setSpinBox(hpEffect_sbPercentPerTurn);
	hpEffect_bAmountPerTurnSideButton->setSpinBox(hpEffect_sbAmountPerTurn);
	hpEffect_bAmountPerNumStepsSideButton->setSpinBox(hpEffect_sbAmountPerNumSteps);
	hpEffect_bNumStepsSideButton->setSpinBox(hpEffect_sbNumSteps);
	mpEffect_bPercentPerTurnSideButton->setSpinBox(mpEffect_sbPercentPerTurn);
	mpEffect_bAmountPerTurnSideButton->setSpinBox(mpEffect_sbAmountPerTurn);
	mpEffect_bAmountPerNumStepsSideButton->setSpinBox(mpEffect_sbAmountPerNumSteps);
	mpEffect_bNumStepsSideButton->setSpinBox(mpEffect_sbNumSteps);
	wPreventSkillWithAttackInfluence->setEnabled(false);
	wPreventSkillWithWisdomInfluence->setEnabled(false);
	wHPEffect->setEnabled(false);
	hpEffect_frmFieldMap->setEnabled(false);
	wMPEffect->setEnabled(false);
	mpEffect_frmFieldMap->setEnabled(false);
	conditionRef = NULL;
	animationFrame = 0;
	animationFrameDirection = 1;
	animationCharacterSpriteID = -1;
	animationPingPong = false;
}

Database_Conditions::~Database_Conditions()
{
	if (conditionRef != NULL)
	{
		delete conditionRef;
		conditionRef = NULL;
	}
}

void Database_Conditions::setupConditions()
{
	updateConditionList();
	ignoreEvents = false;
}

void Database_Conditions::shutdownConditions()
{
	animationTimer->stop();
	if (conditionRef != NULL)
	{
		delete conditionRef;
		conditionRef = NULL;
	}
}

void Database_Conditions::applyConditions()
{
	for (int i = 0; i < ProjectData::characterNames.size(); ++i)
	{
		ProjectData::DataReference<Character> *charRef = new ProjectData::DataReference<Character>(i, __FILE__, __LINE__);
		(*charRef)->resizeConditionResists(ProjectData::conditionNames.size());
		delete charRef;
	}
	for (int i = 0; i < ProjectData::monsterNames.size(); ++i)
	{
		ProjectData::DataReference<Monster> *monsterRef = new ProjectData::DataReference<Monster>(i, __FILE__, __LINE__);
		(*monsterRef)->resizeConditionResists(ProjectData::conditionNames.size());
		delete monsterRef;
	}
}

void Database_Conditions::on_lwConditionList_itemSelectionChanged()
{
	int conditionID;
	if (conditionRef != NULL)
	{
		delete conditionRef;
		conditionRef = NULL;
	}
	conditionID = lwConditionList->currentRow();
	animationTimer->stop();
	animationFrame = 0;
	animationFrameDirection = 1;
	animationCharacterSpriteID = 0;
	if (conditionID >= 0)
		conditionRef = new ProjectData::DataReference<Condition>(conditionID, __FILE__, __LINE__);
	if (conditionID == 0)
	{
		leName->setText((*conditionRef)->getName());
		cboxExtent->setCurrentIndex((*conditionRef)->getExtent());
		tbIcon->setIcon(QIcon((*conditionRef)->getIconLocation()));
		sbGroup->setValue(0);
		sbPriority->setValue(100);
		cboxRestriction->setCurrentIndex(1);
		sbSusceptibilityA->setValue((*conditionRef)->getSusceptibilityA());
		sbSusceptibilityB->setValue((*conditionRef)->getSusceptibilityB());
		sbSusceptibilityC->setValue((*conditionRef)->getSusceptibilityC());
		sbSusceptibilityD->setValue((*conditionRef)->getSusceptibilityD());
		sbSusceptibilityE->setValue((*conditionRef)->getSusceptibilityE());
		sbNumTurns->setValue(0);
		sbPercentEachTurn->setValue(0);
		sbPhysicalDamageRecoveryChance->setValue(0);
		sbAttackAlteration->setValue(100);
		sbDefenseAlteration->setValue(100);
		sbWisdomAlteration->setValue(100);
		sbAgilityAlteration->setValue(100);
		sbHitPercentage->setValue(100);
		cbAttackInfluence->setChecked(false);
		sbAttackInfluence->setValue(0);
		cbWisdomInfluence->setChecked(false);
		sbWisdomInfluence->setValue(0);
		sbAnimation->setValue(CharacterSprite::BATTLEPOSE_DEAD);
		hpEffect_rbNone->setChecked(true);
		hpEffect_sbPercentPerTurn->setValue(0);
		hpEffect_sbAmountPerTurn->setValue(0);
		hpEffect_sbAmountPerNumSteps->setValue(0);
		hpEffect_sbNumSteps->setValue(0);
		mpEffect_rbNone->setChecked(true);
		mpEffect_sbPercentPerTurn->setValue(0);
		mpEffect_sbAmountPerTurn->setValue(0);
		mpEffect_sbAmountPerNumSteps->setValue(0);
		mpEffect_sbNumSteps->setValue(0);
		cbLockEquipment->setChecked(false);
		cbReflectSkills->setChecked(false);
		rbEvadingNoChange->setChecked(true);
		rbCriticalHitsNoChange->setChecked(true);
		wEverythingPart1->setEnabled(false);
		wEverythingPart2->setEnabled(false);
		wEverythingPart3->setEnabled(false);
		loadAnimationImage();
		animationItem->setPos((float)(animationFrame * -96), 0.0);
		animationTimer->start(150);
	}
	else if (conditionID > 0)
	{
		leName->setText((*conditionRef)->getName());
		cboxExtent->setCurrentIndex((*conditionRef)->getExtent());
		tbIcon->setIcon(QIcon((*conditionRef)->getIconLocation()));
		sbGroup->setValue((*conditionRef)->getGroup());
		sbPriority->setValue((*conditionRef)->getPriority());
		cboxRestriction->setCurrentIndex((*conditionRef)->getActionRestriction());
		sbSusceptibilityA->setValue((*conditionRef)->getSusceptibilityA());
		sbSusceptibilityB->setValue((*conditionRef)->getSusceptibilityB());
		sbSusceptibilityC->setValue((*conditionRef)->getSusceptibilityC());
		sbSusceptibilityD->setValue((*conditionRef)->getSusceptibilityD());
		sbSusceptibilityE->setValue((*conditionRef)->getSusceptibilityE());
		sbNumTurns->setValue((*conditionRef)->getAbatesAfterNumTurns());
		sbPercentEachTurn->setValue((*conditionRef)->getPerTurnRecoveryChance());
		sbPhysicalDamageRecoveryChance->setValue((*conditionRef)->getPhysicalDamageRecoveryChance());
		sbAttackAlteration->setValue((*conditionRef)->getAttackAlteration());
		sbDefenseAlteration->setValue((*conditionRef)->getDefenseAlteration());
		sbWisdomAlteration->setValue((*conditionRef)->getWisdomAlteration());
		sbAgilityAlteration->setValue((*conditionRef)->getAgilityAlteration());
		sbHitPercentage->setValue((*conditionRef)->getHitPercentage());
		cbAttackInfluence->setChecked((*conditionRef)->getPreventSkillsWithAnAttackInfluence());
		sbAttackInfluence->setValue((*conditionRef)->getAttackInfluence());
		cbWisdomInfluence->setChecked((*conditionRef)->getPreventSkillsWithAWisdomInfluence());
		sbWisdomInfluence->setValue((*conditionRef)->getWisdomInfluence());
		sbAnimation->setValue((*conditionRef)->getAnimationID());
		switch ((*conditionRef)->getHPEffect())
		{
			case Condition::EFFECT_NONE:
				hpEffect_rbNone->setChecked(true);
				break;
			case Condition::EFFECT_RECOVERY:
				hpEffect_rbRecovery->setChecked(true);
				break;
			case Condition::EFFECT_DAMAGE:
				hpEffect_rbDamage->setChecked(true);
				break;
		}
		hpEffect_sbPercentPerTurn->setValue((*conditionRef)->getHPPercentPerTurn());
		hpEffect_sbAmountPerTurn->setValue((*conditionRef)->getHPAmountPerTurn());
		hpEffect_sbAmountPerNumSteps->setValue((*conditionRef)->getHPAmountPerNumSteps());
		hpEffect_sbNumSteps->setValue((*conditionRef)->getHPNumSteps());
		switch ((*conditionRef)->getMPEffect())
		{
			case Condition::EFFECT_NONE:
				mpEffect_rbNone->setChecked(true);
				break;
			case Condition::EFFECT_RECOVERY:
				mpEffect_rbRecovery->setChecked(true);
				break;
			case Condition::EFFECT_DAMAGE:
				mpEffect_rbDamage->setChecked(true);
				break;
		}
		mpEffect_sbPercentPerTurn->setValue((*conditionRef)->getMPPercentPerTurn());
		mpEffect_sbAmountPerTurn->setValue((*conditionRef)->getMPAmountPerTurn());
		mpEffect_sbAmountPerNumSteps->setValue((*conditionRef)->getMPAmountPerNumSteps());
		mpEffect_sbNumSteps->setValue((*conditionRef)->getMPNumSteps());
		cbLockEquipment->setChecked((*conditionRef)->getLockEquipment());
		cbReflectSkills->setChecked((*conditionRef)->getReflectSkills());
		switch ((*conditionRef)->getEvadingChange())
		{
			case Condition::CHANGE_NOCHANGE:
				rbEvadingNoChange->setChecked(true);
				break;
			case Condition::CHANGE_ALWAYS:
				rbEvadingAlways->setChecked(true);
				break;
			case Condition::CHANGE_NEVER:
				rbEvadingNever->setChecked(true);
				break;
		}
		switch ((*conditionRef)->getCriticalHitsChange())
		{
			case Condition::CHANGE_NOCHANGE:
				rbCriticalHitsNoChange->setChecked(true);
				break;
			case Condition::CHANGE_ALWAYS:
				rbCriticalHitsAlways->setChecked(true);
				break;
			case Condition::CHANGE_NEVER:
				rbCriticalHitsNever->setChecked(true);
				break;
		}
		wEverythingPart1->setEnabled(true);
		wEverythingPart2->setEnabled(true);
		wEverythingPart3->setEnabled(true);
		loadAnimationImage();
		animationItem->setPos((float)(animationFrame * -96), 0.0);
		animationTimer->start(150);
	}
}

void Database_Conditions::on_bArraySize_clicked()
{
	int numConditions = ProjectData::conditionNames.size();
	ArraySize *arraysize = new ArraySize(numConditions, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numConditions)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (conditionRef != NULL)
				{
					if (newSize <= conditionRef->getDataID())
					{
						delete conditionRef;
						conditionRef = NULL;
					}
				}
				for (int i = numConditions - 1; i >= newSize; --i)
					ProjectData::deleteCondition(i);
			}
		}
		else
		{
			for (int i = numConditions; i < newSize; ++i)
			{
				Condition *condition = new Condition;
				condition->setName(QString("Cond%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addCondition(condition);
				delete condition;
			}
		}
		updateConditionList();
	}
	delete arraysize;
}

void Database_Conditions::on_leName_textChanged(QString text)
{
	int currentRow = lwConditionList->currentRow();
	lwConditionList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 4, 10, QChar('0')).arg(text));
	ProjectData::conditionNames[currentRow] = text;
	(*conditionRef)->setName(text);
}

void Database_Conditions::on_cboxExtent_currentIndexChanged(int index)
{
	(*conditionRef)->setExtent(index);
	if (index == Condition::EXTENT_ENDSAFTERBATTLE)
	{
		hpEffect_frmFieldMap->setEnabled(false);
		mpEffect_frmFieldMap->setEnabled(false);
	}
	else
	{
		hpEffect_frmFieldMap->setEnabled(true);
		mpEffect_frmFieldMap->setEnabled(true);
	}
}

void Database_Conditions::on_tbIcon_clicked()
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
	imageBrowser->setupBrowser("/icons", blockedTypes, QList<QSize>() << QSize(16, 16));
	if (imageBrowser->exec())
	{
		QString fileLocation = imageBrowser->getFileLocation();
		(*conditionRef)->setIconLocation(fileLocation);
		tbIcon->setIcon(QIcon(fileLocation));
	}
	delete imageBrowser;
}

void Database_Conditions::on_sbGroup_valueChanged(int value)
{
	(*conditionRef)->setGroup(value);
}

void Database_Conditions::on_sbPriority_valueChanged(int value)
{
	(*conditionRef)->setPriority(value);
}

void Database_Conditions::on_cboxRestriction_currentIndexChanged(int index)
{
	(*conditionRef)->setActionRestriction(index);
}

void Database_Conditions::on_sbSusceptibilityA_valueChanged(int value)
{
	(*conditionRef)->setSusceptibilityA(value);
}

void Database_Conditions::on_sbSusceptibilityB_valueChanged(int value)
{
	(*conditionRef)->setSusceptibilityB(value);
}

void Database_Conditions::on_sbSusceptibilityC_valueChanged(int value)
{
	(*conditionRef)->setSusceptibilityC(value);
}

void Database_Conditions::on_sbSusceptibilityD_valueChanged(int value)
{
	(*conditionRef)->setSusceptibilityD(value);
}

void Database_Conditions::on_sbSusceptibilityE_valueChanged(int value)
{
	(*conditionRef)->setSusceptibilityE(value);
}

void Database_Conditions::on_sbNumTurns_valueChanged(int value)
{
	(*conditionRef)->setAbatesAfterNumTurns(value);
}

void Database_Conditions::on_sbPercentEachTurn_valueChanged(int value)
{
	(*conditionRef)->setPerTurnRecoveryChance(value);
}

void Database_Conditions::on_sbPhysicalDamageRecoveryChance_valueChanged(int value)
{
	(*conditionRef)->setPhysicalDamageRecoveryChance(value);
}

void Database_Conditions::on_sbAttackAlteration_valueChanged(int value)
{
	(*conditionRef)->setAttackAlteration(value);
}

void Database_Conditions::on_sbDefenseAlteration_valueChanged(int value)
{
	(*conditionRef)->setDefenseAlteration(value);
}

void Database_Conditions::on_sbWisdomAlteration_valueChanged(int value)
{
	(*conditionRef)->setWisdomAlteration(value);
}

void Database_Conditions::on_sbAgilityAlteration_valueChanged(int value)
{
	(*conditionRef)->setAgilityAlteration(value);
}

void Database_Conditions::on_sbHitPercentage_valueChanged(int value)
{
	(*conditionRef)->setHitPercentage(value);
}

void Database_Conditions::on_cbAttackInfluence_toggled(bool on)
{
	(*conditionRef)->setPreventSkillsWithAnAttackInfluence(on);
	wPreventSkillWithAttackInfluence->setEnabled(on);
}

void Database_Conditions::on_sbAttackInfluence_valueChanged(int value)
{
	(*conditionRef)->setAttackInfluence(value);
}

void Database_Conditions::on_cbWisdomInfluence_toggled(bool on)
{
	(*conditionRef)->setPreventSkillsWithAWisdomInfluence(on);
	wPreventSkillWithWisdomInfluence->setEnabled(on);
}

void Database_Conditions::on_sbWisdomInfluence_valueChanged(int value)
{
	(*conditionRef)->setWisdomInfluence(value);
}

void Database_Conditions::on_sbAnimation_valueChanged(int value)
{
	(*conditionRef)->setAnimationID(value);
	animationTimer->stop();
	animationFrame = 0;
	animationFrameDirection = 1;
	loadAnimationImage();
	animationItem->setPos((float)(animationFrame * -96), 0.0);
	animationTimer->start(150);
}

void Database_Conditions::on_hpEffect_rbNone_toggled(bool on)
{
	if (on)
	{
		(*conditionRef)->setHPEffect(Condition::EFFECT_NONE);
		wHPEffect->setEnabled(false);
	}
}

void Database_Conditions::on_hpEffect_rbRecovery_toggled(bool on)
{
	if (on)
	{
		(*conditionRef)->setHPEffect(Condition::EFFECT_RECOVERY);
		wHPEffect->setEnabled(true);
		hpEffect_lblPerTurn->setText("Recovery Per Turn in Battle");
	}
}

void Database_Conditions::on_hpEffect_rbDamage_toggled(bool on)
{
	if (on)
	{
		(*conditionRef)->setHPEffect(Condition::EFFECT_DAMAGE);
		wHPEffect->setEnabled(true);
		hpEffect_lblPerTurn->setText("Damage Per Turn in Battle");
	}
}

void Database_Conditions::on_hpEffect_sbPercentPerTurn_valueChanged(int value)
{
	(*conditionRef)->setHPPercentPerTurn(value);
}

void Database_Conditions::on_hpEffect_sbAmountPerTurn_valueChanged(int value)
{
	(*conditionRef)->setHPAmountPerTurn(value);
}

void Database_Conditions::on_hpEffect_sbAmountPerNumSteps_valueChanged(int value)
{
	(*conditionRef)->setHPAmountPerNumSteps(value);
}

void Database_Conditions::on_hpEffect_sbNumSteps_valueChanged(int value)
{
	(*conditionRef)->setHPNumSteps(value);
}

void Database_Conditions::on_mpEffect_rbNone_toggled(bool on)
{
	if (on)
	{
		(*conditionRef)->setMPEffect(Condition::EFFECT_NONE);
		wMPEffect->setEnabled(false);
	}
}

void Database_Conditions::on_mpEffect_rbRecovery_toggled(bool on)
{
	if (on)
	{
		(*conditionRef)->setMPEffect(Condition::EFFECT_RECOVERY);
		wMPEffect->setEnabled(true);
		mpEffect_lblPerTurn->setText("Recovery Per Turn in Battle");
	}
}

void Database_Conditions::on_mpEffect_rbDamage_toggled(bool on)
{
	if (on)
	{
		(*conditionRef)->setMPEffect(Condition::EFFECT_DAMAGE);
		wMPEffect->setEnabled(true);
		mpEffect_lblPerTurn->setText("Damage Per Turn in Battle");
	}
}

void Database_Conditions::on_mpEffect_sbPercentPerTurn_valueChanged(int value)
{
	(*conditionRef)->setMPPercentPerTurn(value);
}

void Database_Conditions::on_mpEffect_sbAmountPerTurn_valueChanged(int value)
{
	(*conditionRef)->setMPAmountPerTurn(value);
}

void Database_Conditions::on_mpEffect_sbAmountPerNumSteps_valueChanged(int value)
{
	(*conditionRef)->setMPAmountPerNumSteps(value);
}

void Database_Conditions::on_mpEffect_sbNumSteps_valueChanged(int value)
{
	(*conditionRef)->setMPNumSteps(value);
}

void Database_Conditions::on_cbReflectSkills_toggled(bool on)
{
	(*conditionRef)->setReflectSkills(on);
}

void Database_Conditions::on_cbLockEquipment_toggled(bool on)
{
	(*conditionRef)->setLockEquipment(on);
}

void Database_Conditions::on_rbEvadingNoChange_toggled(bool on)
{
	if (on)
		(*conditionRef)->setEvadingChange(Condition::CHANGE_NOCHANGE);
}

void Database_Conditions::on_rbEvadingAlways_toggled(bool on)
{
	if (on)
		(*conditionRef)->setEvadingChange(Condition::CHANGE_ALWAYS);
}

void Database_Conditions::on_rbEvadingNever_toggled(bool on)
{
	if (on)
		(*conditionRef)->setEvadingChange(Condition::CHANGE_NEVER);
}

void Database_Conditions::on_rbCriticalHitsNoChange_toggled(bool on)
{
	if (on)
		(*conditionRef)->setCriticalHitsChange(Condition::CHANGE_NOCHANGE);
}

void Database_Conditions::on_rbCriticalHitsAlways_toggled(bool on)
{
	if (on)
		(*conditionRef)->setCriticalHitsChange(Condition::CHANGE_ALWAYS);
}

void Database_Conditions::on_rbCriticalHitsNever_toggled(bool on)
{
	if (on)
		(*conditionRef)->setCriticalHitsChange(Condition::CHANGE_NEVER);
}

void Database_Conditions::animateSprite()
{
	TODO("Implement Function.");
}

void Database_Conditions::updateConditionList()
{
	QListWidgetItem *item;
	int numConditions = ProjectData::conditionNames.size();
	int currentRow = lwConditionList->currentRow();
	lwConditionList->clear();
	for (int i = 0; i < numConditions; ++i)
	{
		item = new QListWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::conditionNames[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		lwConditionList->addItem(item);
	}
	if (currentRow >= lwConditionList->count())
		currentRow = lwConditionList->count() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::conditionNames.size() > 0)
	{
		lwConditionList->setCurrentRow(currentRow);
		lwConditionList->item(currentRow)->setSelected(true);
	}
}

void Database_Conditions::loadAnimationImage()
{
	TODO("Implement Function.");
}
