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
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "audiomanager.h"
#include "clipboard.h"
#include "configureskillsubsets.h"
#include "database_skills.h"
#include "projectdata.h"
#include "projectdata_skill.h"
#include "soundeffectbrowser.h"
#include "variableselector.h"
#include "xmlParser.h"

Database_Skills::Database_Skills(QWidget *parent) : QWidget(parent)
{
	ignoreEvents = true;
	setupUi(this);
	bMPNumberSideButton->setSpinBox(sbMPNumber);
	bMPPercentSideButton->setSpinBox(sbMPPercent);
	normal_bEffectRatingSideButton->setSpinBox(normal_sbEffectRating);
	normal_bSuccessRateSideButton->setSpinBox(normal_sbSuccessRate);
	normal_bConditionEffectivenessSpinButton->setSpinBox(normal_sbConditionEffectiveness);
	normal_twAttributeList->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	normal_twConditionList->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	normal_cboxBattleAnimation->clear();
	normal_cboxBattleAnimation->addItem("(NONE)");
	normal_cboxBattleAnimation->addItems(ProjectData::battleAnimationNames);
	normal_cboxGlobalAnimation->clear();
	normal_cboxGlobalAnimation->addItem("(NONE)");
	normal_cboxGlobalAnimation->addItems(ProjectData::globalAnimationNames);
	skillRef = NULL;
	switch_lwSwitch->addItem(ProjectData::switchVariableIdToString(0));
	normal_updateAttributeList();
	normal_updateConditionList();
}

Database_Skills::~Database_Skills()
{
	if (skillRef != NULL)
	{
		applySkillData();
		delete skillRef;
		skillRef = NULL;
	}
}

void Database_Skills::setupSkills()
{
	updateTypeList();
	updateSkillList();
	ignoreEvents = false;
}

void Database_Skills::shutdownSkills()
{
	if (skillRef != NULL)
	{
		applySkillData();
		delete skillRef;
		skillRef = NULL;
	}
}

void Database_Skills::on_lwSkillList_itemSelectionChanged()
{
	int currentRow = lwSkillList->currentRow();
	int type;
	if (skillRef != NULL)
	{
		applySkillData();
		delete skillRef;
		skillRef = NULL;
	}
	if (currentRow >= 0)
	{
		skillRef = new ProjectData::DataReference<Skill>(currentRow, __FILE__, __LINE__);
		type = ProjectData::skillSubsets[(*skillRef)->getType()]->type;
		leName->setText((*skillRef)->getName());
		cboxType->setCurrentIndex((*skillRef)->getType());
		if ((*skillRef)->isMPNumber())
			rbNumber->setChecked(true);
		else
			rbPercent->setChecked(true);
		if (type == ProjectData::SKILLTYPE_NORMAL)
		{
			normal_leDescription->setText((*skillRef)->getDescription());
			(*skillRef)->getNormalData(&normalData);
			for (int i = 0; i < normalData.attributes.size(); ++i)
				((QCheckBox*)normal_twAttributeList->cellWidget(i, 0))->setChecked(normalData.attributes[i]);
			for (int i = 0; i < normalData.conditions.size(); ++i)
				((QCheckBox*)normal_twConditionList->cellWidget(i, 0))->setChecked(normalData.conditions[i]);
			normal_cboxTarget->setCurrentIndex(normalData.target);
			normal_sbEffectRating->setValue(normalData.effectRating);
			normal_sbSuccessRate->setValue(normalData.successRate);
			normal_cboxBattleAnimation->setCurrentIndex(normalData.battleAnimationID + 1);
			normal_cboxGlobalAnimation->setCurrentIndex(normalData.globalAnimationID + 1);
			normal_hsAttackInfluence->setValue(normalData.attackInfluence);
			normal_hsWisdomInfluence->setValue(normalData.wisdomInfluence);
			normal_hsVariance->setValue(normalData.variance);
			normal_sbConditionEffectiveness->setValue(normalData.percentConditionEffectivenessProbability);
			normal_cbHP->setChecked(normalData.changeHP);
			normal_cbMP->setChecked(normalData.changeMP);
			normal_cbAttack->setChecked(normalData.changeAttack);
			normal_cbDefense->setChecked(normalData.changeDefense);
			normal_cbWisdom->setChecked(normalData.changeWisdom);
			normal_cbAgility->setChecked(normalData.changeAgility);
			normal_cbAbsorb->setChecked(normalData.absorb);
			normal_cbIgnoreDefense->setChecked(normalData.ignoreDefense);
			if (normalData.inflictConditions)
				normal_rbInflictCondition->setChecked(true);
			else
				normal_rbRemoveCondition->setChecked(true);
			normal_cbReduceAttributeResistance->setChecked(normalData.reduceAttributeResistance);
		}
		else if (type == ProjectData::SKILLTYPE_TELEPORT || type == ProjectData::SKILLTYPE_ESCAPE)
		{
			escapeTeleport_leDescription->setText((*skillRef)->getDescription());
			(*skillRef)->getEscapeTeleportData(&escapeTeleportData);
			if (escapeTeleportData.soundEffect.location.isEmpty())
				escapeTeleport_leSoundEffect->setText("(OFF)");
			else
				escapeTeleport_leSoundEffect->setText(QFileInfo(escapeTeleportData.soundEffect.location).fileName());
		}
		else if (type == ProjectData::SKILLTYPE_SWITCH)
		{
			switch_leDescription->setText((*skillRef)->getDescription());
			(*skillRef)->getSwitchData(&switchData);
			switch_lwSwitch->item(0)->setText(ProjectData::switchVariableIdToString(switchData.switchID));
			switch_cbAllowField->setChecked(switchData.allowField);
			switch_cbAllowBattle->setChecked(switchData.allowBattle);
			if (switchData.soundEffect.location.isEmpty())
				switch_leSoundEffect->setText("(OFF)");
			else
				switch_leSoundEffect->setText(QFileInfo(switchData.soundEffect.location).fileName());
		}
	}
}

void Database_Skills::on_bArraySize_clicked()
{
	int numSkills = ProjectData::skillNames.size();
	ArraySize *arraysize = new ArraySize(numSkills, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numSkills)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (skillRef != NULL)
				{
					if (newSize >= skillRef->getDataID())
					{
						delete skillRef;
						skillRef = NULL;
					}
				}
				for (int i = numSkills - 1; i >= newSize; --i)
					ProjectData::deleteSkill(i);
			}
		}
		else
		{
			for (int i = numSkills; i < newSize; ++i)
			{
				Skill *skill = new Skill;
				skill->setName(QString("Skill%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addSkill(skill);
				delete skill;
			}
		}
		ignoreEvents = true;
		updateSkillList();
		ignoreEvents = false;
	}
	delete arraysize;
}

void Database_Skills::on_leName_textChanged(QString text)
{
	if (!ignoreEvents)
	{
		int row = lwSkillList->currentRow();
		lwSkillList->item(row)->setText(QString("%1: %2").arg(row, 4, 10, QChar('0')).arg(text));
		ProjectData::skillNames[row] = text;
		(*skillRef)->setName(text);
	}
}

void Database_Skills::on_cboxType_currentIndexChanged(int index)
{
	if (!ignoreEvents && index >= 0)
	{
		int type = ProjectData::skillSubsets[index]->type;
		if (type == ProjectData::SKILLTYPE_NORMAL)
			stackedWidget->setCurrentWidget(wNormal);
		else if (type == ProjectData::SKILLTYPE_TELEPORT || type == ProjectData::SKILLTYPE_ESCAPE)
			stackedWidget->setCurrentWidget(wEscapeTeleport);
		else if (type == ProjectData::SKILLTYPE_SWITCH)
			stackedWidget->setCurrentWidget(wSwitch);
	}
}

void Database_Skills::on_bConfigureTypes_clicked()
{
	ConfigureSkillSubsets *configTypes = new ConfigureSkillSubsets;
	configTypes->exec();
	delete configTypes;
	updateTypeList();
}

void Database_Skills::on_rbNumber_toggled(bool on)
{
	wMPNumber->setEnabled(on);
	wMPPercent->setEnabled(!on);
}

void Database_Skills::on_normal_leDescription_textChanged(QString text)
{
	escapeTeleport_leDescription->setText(text);
	switch_leDescription->setText(text);
}

void Database_Skills::on_normal_cboxTarget_currentIndexChanged(int index)
{
	if (!ignoreEvents)
	{
		if (index == Skill::TARGET_1ENEMY || index == Skill::TARGET_ALLENEMIES)
		{
			normal_lblIncreaseDecrease->setText("Decrease");
			normal_cbReduceAttributeResistance->setText("Reduce Resis.");
		}
		else
		{
			normal_lblIncreaseDecrease->setText("Increase");
			normal_cbReduceAttributeResistance->setText("Increase Resis.");
		}
	}
}

void Database_Skills::on_normal_bPlayBattleAnimationPreview_clicked()
{
	QMessageBox::information(window(), "Not Implemented", "This feature is not yet implemented.");
	TODO("Implement function.");
}

void Database_Skills::on_normal_bConfigureBattleAnimationPreview_clicked()
{
	QMessageBox::information(window(), "Not Implemented", "This feature is not yet implemented.");
	TODO("Implement function.");
}

void Database_Skills::on_normal_bSetAnimation_clicked()
{
	QMessageBox::information(window(), "Not Implemented", "This feature is not yet implemented.");
	TODO("Implement function.");
}

void Database_Skills::on_normal_hsAttackInfluence_valueChanged(int value)
{
	normal_lblAttackInfluence->setText(QString("%1").arg(value, 2, 10, QChar(' ')));
}

void Database_Skills::on_normal_hsWisdomInfluence_valueChanged(int value)
{
	normal_lblWisdomInfluence->setText(QString("%1").arg(value, 2, 10, QChar(' ')));
}

void Database_Skills::on_normal_hsVariance_valueChanged(int value)
{
	normal_lblVariance->setText(QString("%1").arg(value, 2, 10, QChar(' ')));
}

void Database_Skills::on_switch_leDescription_textChanged(QString text)
{
	normal_leDescription->setText(text);
	escapeTeleport_leDescription->setText(text);
}

void Database_Skills::on_switch_bBrowseSoundEffect_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	soundBrowser->setupBrowser();
	soundBrowser->setSoundEffectInfo(switchData.soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&switchData.soundEffect);
		switch_leSoundEffect->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_Skills::on_switch_lwSwitch_itemDoubleClicked()
{
	switch_bBrowseSwitches->click();
}

void Database_Skills::on_switch_bBrowseSwitches_clicked()
{
	VariableSelector *variableSelector;
	int switchID = ProjectData::switchVariableIdFromString(switch_lwSwitch->item(0)->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_SWITCHES, switchID);
	if (variableSelector->exec())
	{
		switchID = variableSelector->getVariableID();
		switch_lwSwitch->item(0)->setText(ProjectData::switchVariableIdToString(switchID));
	}
	delete variableSelector;
}

void Database_Skills::on_escapeTeleport_leDescription_textChanged(QString text)
{
	normal_leDescription->setText(text);
	switch_leDescription->setText(text);
}

void Database_Skills::on_escapeTeleport_bBrowseSoundEffect_clicked()
{
	SoundEffectBrowser *soundBrowser = new SoundEffectBrowser;
	soundBrowser->setupBrowser();
	soundBrowser->setSoundEffectInfo(escapeTeleportData.soundEffect);
	if (soundBrowser->exec())
	{
		soundBrowser->getSoundEffectInfo(&escapeTeleportData.soundEffect);
		escapeTeleport_leSoundEffect->setText(soundBrowser->getSoundEffectName());
	}
	delete soundBrowser;
}

void Database_Skills::updateSkillList()
{
	int currentRow = lwSkillList->currentRow();
	lwSkillList->clear();
	for (int i = 0; i < ProjectData::skillNames.size(); ++i)
	{
		QListWidgetItem *item = new QListWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::skillNames[i]));
		item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		lwSkillList->addItem(item);
	}
	if (currentRow >= lwSkillList->count())
		currentRow = lwSkillList->count() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::skillNames.size() > 0)
	{
		lwSkillList->setCurrentRow(currentRow);
		lwSkillList->item(currentRow)->setSelected(true);
	}
}

void Database_Skills::updateTypeList()
{
	cboxType->clear();
	for (int i = 0; i < ProjectData::skillSubsets.size(); ++i)
		cboxType->addItem(ProjectData::skillSubsets[i]->name);
}

void Database_Skills::normal_updateAttributeList()
{
	int numAttributes = ProjectData::attributeNames.size();
	normal_twAttributeList->clear();
	normal_twAttributeList->setRowCount(numAttributes);
	for (int i = 0; i < numAttributes; ++i)
		normal_twAttributeList->setCellWidget(i, 0, new QCheckBox(ProjectData::attributeNames[i]));
	normal_twAttributeList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Skills::normal_updateConditionList()
{
	int numConditions = ProjectData::conditionNames.size();
	normal_twConditionList->clear();
	normal_twConditionList->setRowCount(numConditions);
	for (int i = 0; i < numConditions; ++i)
		normal_twConditionList->setCellWidget(i, 0, new QCheckBox(ProjectData::conditionNames[i]));
	normal_twConditionList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void Database_Skills::applySkillData()
{
	int type = ProjectData::skillSubsets[cboxType->currentIndex()]->type;
	(*skillRef)->setType(cboxType->currentIndex());
	(*skillRef)->setMPIsNumber(rbNumber->isChecked());
	(*skillRef)->setMPConsumed((rbNumber->isChecked()) ? sbMPNumber->value():sbMPPercent->value());
	if (type == ProjectData::SKILLTYPE_NORMAL)
	{
		(*skillRef)->setDescription(normal_leDescription->text());
		normalData.attributes.clear();
		normalData.conditions.clear();
		for (int i = 0; i < normal_twAttributeList->rowCount(); ++i)
			normalData.attributes.append(((QCheckBox*)normal_twAttributeList->cellWidget(i, 0))->isChecked());
		for (int i = 0; i < normal_twConditionList->rowCount(); ++i)
			normalData.conditions.append(((QCheckBox*)normal_twConditionList->cellWidget(i, 0))->isChecked());
		normalData.target = normal_cboxTarget->currentIndex();
		normalData.effectRating = normal_sbEffectRating->value();
		normalData.successRate = normal_sbSuccessRate->value();
		normalData.battleAnimationID = normal_cboxBattleAnimation->currentIndex() - 1;
		normalData.globalAnimationID = normal_cboxGlobalAnimation->currentIndex() - 1;
		normalData.attackInfluence = normal_hsAttackInfluence->value();
		normalData.wisdomInfluence = normal_hsWisdomInfluence->value();
		normalData.variance = normal_hsVariance->value();
		normalData.percentConditionEffectivenessProbability = normal_sbConditionEffectiveness->value();
		normalData.changeHP = normal_cbHP->isChecked();
		normalData.changeMP = normal_cbMP->isChecked();
		normalData.changeAttack = normal_cbAttack->isChecked();
		normalData.changeDefense = normal_cbDefense->isChecked();
		normalData.changeWisdom = normal_cbWisdom->isChecked();
		normalData.changeAgility = normal_cbAgility->isChecked();
		normalData.absorb = normal_cbAbsorb->isChecked();
		normalData.ignoreDefense = normal_cbIgnoreDefense->isChecked();
		normalData.inflictConditions = normal_rbInflictCondition->isChecked();
		normalData.reduceAttributeResistance = normal_cbReduceAttributeResistance->isChecked();
		(*skillRef)->setNormalData(&normalData);
	}
	else if (type == ProjectData::SKILLTYPE_TELEPORT || type == ProjectData::SKILLTYPE_ESCAPE)
	{
		(*skillRef)->setDescription(escapeTeleport_leDescription->text());
		(*skillRef)->setEscapeTeleportData(&escapeTeleportData);
	}
	else if (type == ProjectData::SKILLTYPE_SWITCH)
	{
		(*skillRef)->setDescription(switch_leDescription->text());
		switchData.switchID = ProjectData::switchVariableIdFromString(switch_lwSwitch->item(0)->text());
		switchData.allowField = switch_cbAllowField->isChecked();
		switchData.allowBattle = switch_cbAllowBattle->isChecked();
		(*skillRef)->setSwitchData(&switchData);
	}
}
