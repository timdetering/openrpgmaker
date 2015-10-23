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

#ifndef DATABASE_SYSTEM_H
#define DATABASE_SYSTEM_H

class Vehicle;

#include "ui_database_system.h"
#include "globalanimationpreview.h"
#include "projectdata.h"

class Database_System : public QWidget, public Ui::Database_System
{
	Q_OBJECT
	public:
		Database_System(QWidget *parent=0);
		~Database_System();
		void setupSystem();
		void shutdownSystem();
	protected slots:
		void on_cboxSelectVehicle_currentIndexChanged(int index);
		void on_bDeleteVehicle_clicked();
		void on_bAddVehicle_clicked();
		void on_leVehicleName_textChanged(QString text);
		void on_cbIsVehicleFlying_toggled(bool on);
		void on_bBrowseVehicleBGM_clicked();
		void on_cboxVehicleSprite_currentIndexChanged(int index);
		void on_cboxVehiclePassingPhase_currentIndexChanged(int index);
		void on_cboxVehicleLandingPhase_currentIndexChanged(int index);
		void on_bSetTitleScreenImage_clicked();
		void on_cboxTitleScreenAnimation_currentIndexChanged(int index);
		void on_bPreviewTitleScreen_clicked();
		void on_bSetGameOverScreenImage_clicked();
		void on_cboxGameOverScreenAnimation_currentIndexChanged(int index);
		void on_bPreviewGameOverScreen_clicked();
		void on_bSetSystemGraphics_clicked();
		void on_cboxFirstPartyMember_currentIndexChanged(int index);
		void on_cboxSecondPartyMember_currentIndexChanged(int index);
		void on_cboxThirdPartyMember_currentIndexChanged(int index);
		void on_cboxFourthPartyMember_currentIndexChanged(int index);
		void on_bBrowseTitleScreenBGM_clicked();
		void on_bBrowseBattleBGM_clicked();
		void on_bBrowseVictoryBGM_clicked();
		void on_bBrowseInnBGM_clicked();
		void on_bBrowseGameOverBGM_clicked();
		void on_bBrowseCursorSFX_clicked();
		void on_bBrowseSelectChoiceSFX_clicked();
		void on_bBrowseBuzzerSFX_clicked();
		void on_bBrowseCancelSFX_clicked();
		void on_bBrowseBattleTransitionSFX_clicked();
		void on_bBrowseFleeBattleSFX_clicked();
		void on_bBrowseCharacterLevelUpSFX_clicked();
		void on_bBrowseEvadeAttackSFX_clicked();
		void on_bBrowseItemUsedSFX_clicked();
		void on_lwFontList_itemSelectionChanged();
		void on_bAddFont_clicked();
		void on_bDeleteFont_clicked();
		void on_bEditFont_clicked();
		void on_bMakeDefaultFont_clicked();
		void on_bConfigureButtonMapping_clicked();
		void on_bConfigureIntroScreens_clicked();
		void on_cboxTeleportTransition_Erase_currentIndexChanged(int index);
		void on_cboxTeleportTransition_Show_currentIndexChanged(int index);
		void on_cboxMenuTransition_Erase_currentIndexChanged(int index);
		void on_cboxMenuTransition_Show_currentIndexChanged(int index);
		void on_cboxBattleEntryTransition_Erase_currentIndexChanged(int index);
		void on_cboxBattleEntryTransition_Show_currentIndexChanged(int index);
		void on_cboxBattleExitTransition_Erase_currentIndexChanged(int index);
		void on_cboxBattleExitTransition_Show_currentIndexChanged(int index);
		void on_cboxMainMenu_currentIndexChanged(int index);
	private:
		void updateFirstStartingPartyMemberList();
		void updateSecondStartingPartyMemberList();
		void updateThirdStartingPartyMemberList();
		void updateFourthStartingPartyMemberList();
		void updateSystemGraphicsPreview();
		void updateGlobalAnimationLists();
		GlobalAnimationPreview *animationPreviewWidget;
		QGraphicsPixmapItem *titleScreenItem;
		QGraphicsPixmapItem *gameOverScreenItem;
		QList<int> firstStartingPartyMemberList;
		QList<int> secondStartingPartyMemberList;
		QList<int> thirdStartingPartyMemberList;
		QList<int> fourthStartingPartyMemberList;
		QList<int> mainMenuLayoutList;
		ProjectData::DataReference<Vehicle> *vehicleRef;
		ProjectData::DataReference<GlobalAnimation> *titleAnimRef;
		ProjectData::DataReference<GlobalAnimation> *gameOverAnimRef;
		bool ignoreEvents;
};

#endif // DATABASE_SYSTEM_H
