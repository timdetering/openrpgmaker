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
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QFileInfo>
#include "configurecustomgriddepth.h"
#include "configureevent_enemyencounter.h"
#include "imagebrowser.h"
#include "projectdata.h"
#include "projectdata_terrain.h"
#include "variableselector.h"

ConfigureEvent_EnemyEncounter::ConfigureEvent_EnemyEncounter(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	lblMonsterGroupVariable->setText(ProjectData::numberVariableIdToString(0));
	wMonsterGroupVariable->setEnabled(false);
	cboxMonsterGroups->addItems(ProjectData::monsterGroupNames);
	cboxTerrain->addItems(ProjectData::terrainNames);
	wSpecificBackground->setEnabled(false);
	cboxTerrain->setEnabled(false);
	bConfigureGridDepth->setEnabled(false);
	gridDepthValues.hTopLineLocation = Terrain::gridDepth_ShallowValues.hTopLineLocation;
	gridDepthValues.hBottomLineLocation = Terrain::gridDepth_ShallowValues.hBottomLineLocation;
	gridDepthValues.vMonsterLine_TopLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_TopLocation;
	gridDepthValues.vMonsterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_BottomLocation;
	gridDepthValues.vMonsterLine_Spacing = Terrain::gridDepth_ShallowValues.vMonsterLine_Spacing;
	gridDepthValues.vCharacterLine_TopLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_TopLocation;
	gridDepthValues.vCharacterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_BottomLocation;
	gridDepthValues.vCharacterLine_Spacing = Terrain::gridDepth_ShallowValues.vCharacterLine_Spacing;
}

ConfigureEvent_EnemyEncounter::~ConfigureEvent_EnemyEncounter()
{
}

void ConfigureEvent_EnemyEncounter::setSpecialConditions(int specialConditions)
{
	if (specialConditions == NONE)
		rbNoSpecialConditions->setChecked(true);
	else if (specialConditions == INITIATIVE)
		rbInitiative->setChecked(true);
	else if (specialConditions == BACK_ATTACK)
		rbBackAttack->setChecked(true);
	else if (specialConditions == SURROUND_ATTACK)
		rbSurroundAttack->setChecked(true);
	else if (specialConditions == PINCERS_ATTACK)
		rbPincersAttack->setChecked(true);
}

void ConfigureEvent_EnemyEncounter::setMonsterGroupID(int monsterGroupID)
{
	if (rbSpecificMonsterGroup->isChecked())
		cboxMonsterGroups->setCurrentIndex(monsterGroupID);
	else
		lblMonsterGroupVariable->setText(ProjectData::numberVariableIdToString(monsterGroupID));
}

void ConfigureEvent_EnemyEncounter::setIsReferencedMonsterGroup(bool referenced)
{
	if (referenced)
		rbReferencedMonsterGroup->setChecked(true);
	else
		rbSpecificMonsterGroup->setChecked(true);
}

void ConfigureEvent_EnemyEncounter::setBattleBackgroundType(int type)
{
	if (type == USE_MAP_OR_TERRAIN_SETTING)
		rbUseMapTerrainBackground->setChecked(true);
	else if (type == SPECIFIC_BACKGROUND)
		rbSpecificBackground->setChecked(true);
	else if (type == USE_TERRAIN_BACKGROUND)
		rbUseTerrainBackground->setChecked(true);
}

void ConfigureEvent_EnemyEncounter::setBackgroundLocation(QString location)
{
	backgroundLocation = location;
	if (!location.isEmpty())
		lblBackground->setText(QFileInfo(location).fileName());
	else
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void ConfigureEvent_EnemyEncounter::setGridDepth(int depth)
{
	cboxGridDepth->setCurrentIndex(depth);
	if (depth == Terrain::DEPTH_SHALLOW)
	{
		gridDepthValues.hTopLineLocation = Terrain::gridDepth_ShallowValues.hTopLineLocation;
		gridDepthValues.hBottomLineLocation = Terrain::gridDepth_ShallowValues.hBottomLineLocation;
		gridDepthValues.vMonsterLine_TopLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_TopLocation;
		gridDepthValues.vMonsterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_BottomLocation;
		gridDepthValues.vMonsterLine_Spacing = Terrain::gridDepth_ShallowValues.vMonsterLine_Spacing;
		gridDepthValues.vCharacterLine_TopLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_TopLocation;
		gridDepthValues.vCharacterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_BottomLocation;
		gridDepthValues.vCharacterLine_Spacing = Terrain::gridDepth_ShallowValues.vCharacterLine_Spacing;
	}
	else if (depth == Terrain::DEPTH_DEEP)
	{
		gridDepthValues.hTopLineLocation = Terrain::gridDepth_DeepValues.hTopLineLocation;
		gridDepthValues.hBottomLineLocation = Terrain::gridDepth_DeepValues.hBottomLineLocation;
		gridDepthValues.vMonsterLine_TopLocation = Terrain::gridDepth_DeepValues.vMonsterLine_TopLocation;
		gridDepthValues.vMonsterLine_BottomLocation = Terrain::gridDepth_DeepValues.vMonsterLine_BottomLocation;
		gridDepthValues.vMonsterLine_Spacing = Terrain::gridDepth_DeepValues.vMonsterLine_Spacing;
		gridDepthValues.vCharacterLine_TopLocation = Terrain::gridDepth_DeepValues.vCharacterLine_TopLocation;
		gridDepthValues.vCharacterLine_BottomLocation = Terrain::gridDepth_DeepValues.vCharacterLine_BottomLocation;
		gridDepthValues.vCharacterLine_Spacing = Terrain::gridDepth_DeepValues.vCharacterLine_Spacing;
	}
}

void ConfigureEvent_EnemyEncounter::setGridDepthValues(Terrain::GridDepthValues *values)
{
	gridDepthValues.hTopLineLocation = values->hTopLineLocation;
	gridDepthValues.hBottomLineLocation = values->hBottomLineLocation;
	gridDepthValues.vMonsterLine_TopLocation = values->vMonsterLine_TopLocation;
	gridDepthValues.vMonsterLine_BottomLocation = values->vMonsterLine_BottomLocation;
	gridDepthValues.vMonsterLine_Spacing = values->vMonsterLine_Spacing;
	gridDepthValues.vCharacterLine_TopLocation = values->vCharacterLine_TopLocation;
	gridDepthValues.vCharacterLine_BottomLocation = values->vCharacterLine_BottomLocation;
	gridDepthValues.vCharacterLine_Spacing = values->vCharacterLine_Spacing;
}

void ConfigureEvent_EnemyEncounter::setTerrainID(int terrainID)
{
	cboxTerrain->setCurrentIndex(terrainID);
}

void ConfigureEvent_EnemyEncounter::setEscapeHandler(int handler)
{
	if (handler == DISALLOW_ESCAPE)
		escape_rbDisallowEscape->setChecked(true);
	else if (handler == END_EVENT_PROCESSING)
		escape_rbEndEventProcessing->setChecked(true);
	else if (handler == CUSTOM_ESCAPE_HANDLER)
		escape_rbExecuteCustomHandler->setChecked(true);
}

void ConfigureEvent_EnemyEncounter::setDefeatHandler(int handler)
{
	if (handler == GAMEOVER)
		defeat_rbGameOver->setChecked(true);
	else
		defeat_rbExecuteCustomHandler->setChecked(true);
}

void ConfigureEvent_EnemyEncounter::setBeginBattleWithFirstStrike(bool beginBattleWithFirstStrike)
{
	cbBeginBattleWithFirstStrike->setChecked(beginBattleWithFirstStrike);
}

int ConfigureEvent_EnemyEncounter::getSpecialConditions()
{
	if (rbNoSpecialConditions->isChecked())
		return NONE;
	else if (rbInitiative->isChecked())
		return INITIATIVE;
	else if (rbBackAttack->isChecked())
		return BACK_ATTACK;
	else if (rbSurroundAttack->isChecked())
		return SURROUND_ATTACK;
	return PINCERS_ATTACK;
}

int ConfigureEvent_EnemyEncounter::getMonsterGroupID()
{
	if (rbSpecificMonsterGroup->isChecked())
		return cboxMonsterGroups->currentIndex();
	return ProjectData::numberVariableIdFromString(lblMonsterGroupVariable->text());
}

bool ConfigureEvent_EnemyEncounter::isReferencedMonsterGroup()
{
	return rbReferencedMonsterGroup->isChecked();
}

int ConfigureEvent_EnemyEncounter::getBattleBackgroundType()
{
	if (rbUseMapTerrainBackground->isChecked())
		return USE_MAP_OR_TERRAIN_SETTING;
	else if (rbSpecificBackground->isChecked())
		return SPECIFIC_BACKGROUND;
	return USE_TERRAIN_BACKGROUND;
}

QString ConfigureEvent_EnemyEncounter::getBackgroundLocation()
{
	return backgroundLocation;
}

int ConfigureEvent_EnemyEncounter::getGridDepth()
{
	return cboxGridDepth->currentIndex();
}

void ConfigureEvent_EnemyEncounter::getGridDepthValues(Terrain::GridDepthValues *values)
{
	values->hTopLineLocation = gridDepthValues.hTopLineLocation;
	values->hBottomLineLocation = gridDepthValues.hBottomLineLocation;
	values->vMonsterLine_TopLocation = gridDepthValues.vMonsterLine_TopLocation;
	values->vMonsterLine_BottomLocation = gridDepthValues.vMonsterLine_BottomLocation;
	values->vMonsterLine_Spacing = gridDepthValues.vMonsterLine_Spacing;
	values->vCharacterLine_TopLocation = gridDepthValues.vCharacterLine_TopLocation;
	values->vCharacterLine_BottomLocation = gridDepthValues.vCharacterLine_BottomLocation;
	values->vCharacterLine_Spacing = gridDepthValues.vCharacterLine_Spacing;
}

int ConfigureEvent_EnemyEncounter::getTerrainID()
{
	return cboxTerrain->currentIndex();
}

int ConfigureEvent_EnemyEncounter::getEscapeHandler()
{
	if (escape_rbDisallowEscape->isChecked())
		return DISALLOW_ESCAPE;
	else if (escape_rbEndEventProcessing->isChecked())
		return END_EVENT_PROCESSING;
	return CUSTOM_ESCAPE_HANDLER;
}

int ConfigureEvent_EnemyEncounter::getDefeatHandler()
{
	if (defeat_rbGameOver->isChecked())
		return GAMEOVER;
	return CUSTOM_DEFEAT_HANDLER;
}

bool ConfigureEvent_EnemyEncounter::beginBattleWithFirstStrike()
{
	return cbBeginBattleWithFirstStrike->isChecked();
}

void ConfigureEvent_EnemyEncounter::on_bBrowseBackgrounds_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		backgroundLocation = imageBrowser->getFileLocation();
		if (!backgroundLocation.isEmpty())
		{
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
			lblBackground->setText(QFileInfo(backgroundLocation).fileName());
		}
		else
		{
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
			lblBackground->setText("");
		}
	}
	delete imageBrowser;
}

void ConfigureEvent_EnemyEncounter::on_bBrowseMonsterGroupVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblMonsterGroupVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblMonsterGroupVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_EnemyEncounter::on_bConfigureGridDepth_clicked()
{
	ConfigureCustomGridDepth *configGridDepth = new ConfigureCustomGridDepth(&gridDepthValues, backgroundLocation);
	if (configGridDepth->exec())
		configGridDepth->applyValues(&gridDepthValues);
	delete configGridDepth;
}

void ConfigureEvent_EnemyEncounter::on_cboxGridDepth_currentIndexChanged(int index)
{
	bConfigureGridDepth->setEnabled((index == Terrain::DEPTH_CUSTOM));
	if (index == Terrain::DEPTH_SHALLOW)
	{
		gridDepthValues.hTopLineLocation = Terrain::gridDepth_ShallowValues.hTopLineLocation;
		gridDepthValues.hBottomLineLocation = Terrain::gridDepth_ShallowValues.hBottomLineLocation;
		gridDepthValues.vMonsterLine_TopLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_TopLocation;
		gridDepthValues.vMonsterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vMonsterLine_BottomLocation;
		gridDepthValues.vMonsterLine_Spacing = Terrain::gridDepth_ShallowValues.vMonsterLine_Spacing;
		gridDepthValues.vCharacterLine_TopLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_TopLocation;
		gridDepthValues.vCharacterLine_BottomLocation = Terrain::gridDepth_ShallowValues.vCharacterLine_BottomLocation;
		gridDepthValues.vCharacterLine_Spacing = Terrain::gridDepth_ShallowValues.vCharacterLine_Spacing;
	}
	else if (index == Terrain::DEPTH_DEEP)
	{
		gridDepthValues.hTopLineLocation = Terrain::gridDepth_DeepValues.hTopLineLocation;
		gridDepthValues.hBottomLineLocation = Terrain::gridDepth_DeepValues.hBottomLineLocation;
		gridDepthValues.vMonsterLine_TopLocation = Terrain::gridDepth_DeepValues.vMonsterLine_TopLocation;
		gridDepthValues.vMonsterLine_BottomLocation = Terrain::gridDepth_DeepValues.vMonsterLine_BottomLocation;
		gridDepthValues.vMonsterLine_Spacing = Terrain::gridDepth_DeepValues.vMonsterLine_Spacing;
		gridDepthValues.vCharacterLine_TopLocation = Terrain::gridDepth_DeepValues.vCharacterLine_TopLocation;
		gridDepthValues.vCharacterLine_BottomLocation = Terrain::gridDepth_DeepValues.vCharacterLine_BottomLocation;
		gridDepthValues.vCharacterLine_Spacing = Terrain::gridDepth_DeepValues.vCharacterLine_Spacing;
	}
}

void ConfigureEvent_EnemyEncounter::on_rbSpecificBackground_toggled(bool checked)
{
	wSpecificBackground->setEnabled(checked);
}

void ConfigureEvent_EnemyEncounter::on_rbSpecificMonsterGroup_toggled(bool checked)
{
	cboxMonsterGroups->setEnabled(checked);
	wMonsterGroupVariable->setEnabled(!checked);
}

void ConfigureEvent_EnemyEncounter::on_rbUseTerrainBackground_toggled(bool checked)
{
	cboxTerrain->setEnabled(checked);
}
