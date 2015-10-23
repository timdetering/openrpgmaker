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

#ifndef CONFIGUREEVENT_ENEMYENCOUNTER_H
#define CONFIGUREEVENT_ENEMYENCOUNTER_H

#include "ui_configureevent_enemyencounter.h"
#include "projectdata_terrain.h"

class ConfigureEvent_EnemyEncounter : public QDialog, public Ui::ConfigureEvent_EnemyEncounter
{
	Q_OBJECT
	public:
		enum SpecialConditions {NONE=0, INITIATIVE, BACK_ATTACK, SURROUND_ATTACK, PINCERS_ATTACK};
		enum BattleBackgroundType {USE_MAP_OR_TERRAIN_SETTING=0, SPECIFIC_BACKGROUND, USE_TERRAIN_BACKGROUND};
		enum EscapeHandler {DISALLOW_ESCAPE=0, END_EVENT_PROCESSING, CUSTOM_ESCAPE_HANDLER};
		enum DefeatHandler {GAMEOVER=0, CUSTOM_DEFEAT_HANDLER};
		ConfigureEvent_EnemyEncounter(QWidget *parent=0);
		~ConfigureEvent_EnemyEncounter();
		void setSpecialConditions(int specialConditions);
		void setMonsterGroupID(int monsterGroupID);
		void setIsReferencedMonsterGroup(bool referenced);
		void setBattleBackgroundType(int type);
		void setBackgroundLocation(QString location);
		void setGridDepth(int depth);
		void setGridDepthValues(Terrain::GridDepthValues *values);
		void setTerrainID(int terrainID);
		void setEscapeHandler(int handler);
		void setDefeatHandler(int handler);
		void setBeginBattleWithFirstStrike(bool beginBattleWithFirstStrike);
		int getSpecialConditions();
		int getMonsterGroupID();
		bool isReferencedMonsterGroup();
		int getBattleBackgroundType();
		QString getBackgroundLocation();
		int getGridDepth();
		void getGridDepthValues(Terrain::GridDepthValues *values);
		int getTerrainID();
		int getEscapeHandler();
		int getDefeatHandler();
		bool beginBattleWithFirstStrike();
	protected slots:
		void on_bBrowseBackgrounds_clicked();
		void on_bBrowseMonsterGroupVariables_clicked();
		void on_bConfigureGridDepth_clicked();
		void on_cboxGridDepth_currentIndexChanged(int index);
		void on_rbSpecificBackground_toggled(bool checked);
		void on_rbSpecificMonsterGroup_toggled(bool checked);
		void on_rbUseTerrainBackground_toggled(bool checked);
	private:
		Terrain::GridDepthValues gridDepthValues;
		QString backgroundLocation;
};

#endif // CONFIGUREEVENT_ENEMYENCOUNTER_H
