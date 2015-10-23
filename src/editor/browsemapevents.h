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

#ifndef BROWSEMAPEVENTS_H
#define BROWSEMAPEVENTS_H

#include "ui_browsemapevents.h"

class BrowseMapEvents : public QDialog, public Ui::BrowseMapEvents
{
	Q_OBJECT
	public:
		enum Tab
		{
			TAB_AUDIOVIDEO=0,
			TAB_EVENTMANAGEMENT,
			TAB_MAPMANAGEMENT,
			TAB_MOVEMENT,
			TAB_PARTYCHARACTERMANAGEMENT,
			TAB_SCREENEFFECTS,
			TAB_SYSTEMMANAGEMENT,
			TAB_VARIABLES
		};
		BrowseMapEvents(QWidget *parent=0);
		~BrowseMapEvents();
		int getSelectedEvent();
	protected slots:
		void on_bAudioVideo_toggled(bool checked);
		void on_bEventManagement_toggled(bool checked);
		void on_bMapManagement_toggled(bool checked);
		void on_bMovement_toggled(bool checked);
		void on_bPartyCharacterManagement_toggled(bool checked);
		void on_bScreenEffects_toggled(bool checked);
		void on_bSystemManagement_toggled(bool checked);
		void on_bVariables_toggled(bool checked);
		void on_bAllowDisallowEscape_clicked();
		void on_bAllowDisallowMainMenu_clicked();
		void on_bAllowDisallowSaving_clicked();
		void on_bAllowDisallowTeleport_clicked();
		void on_bBreakOutOfLoop_clicked();
		void on_bCallCommonEvent_clicked();
		void on_bCallMapEvent_clicked();
		void on_bChangeBattleCommands_clicked();
		void on_bChangeBorderTile_clicked();
		void on_bChangeCharacterBaseStats_clicked();
		void on_bChangeCharacterCondition_clicked();
		void on_bChangeCharacterHP_clicked();
		void on_bChangeCharacterLevel_clicked();
		void on_bChangeCharacterMP_clicked();
		void on_bChangeCharacterSprite_clicked();
		void on_bChangeClass_clicked();
		void on_bChangeEncounterRate_clicked();
		void on_bChangeEquippedItems_clicked();
		void on_bChangeEventLocation_clicked();
		void on_bChangeExperiencePoints_clicked();
		void on_bChangeFaceGraphic_clicked();
		void on_bChangeHeroName_clicked();
		void on_bChangeMoney_clicked();
		void on_bChangeParallaxBackground_clicked();
		void on_bChangeParty_clicked();
		void on_bChangeSkills_clicked();
		void on_bChangeSystemBGM_clicked();
		void on_bChangeSystemGraphic_clicked();
		void on_bChangeSystemSoundEffect_clicked();
		void on_bChangeVehicleGraphic_clicked();
		void on_bCompleteHealing_clicked();
		void on_bConditionalBranch_clicked();
		void on_bDisplayMessage_clicked();
		void on_bEndEventProcessing_clicked();
		void on_bEnemyEncounter_clicked();
		void on_bEraseEvent_clicked();
		void on_bFadeBGM_clicked();
		void on_bFlashScreen_clicked();
		void on_bFlashSprite_clicked();
		void on_bGameOver_clicked();
		void on_bHaltAllMovement_clicked();
		void on_bHeroSpriteTransparency_clicked();
		void on_bHideLayer_clicked();
		void on_bHideScreenOverlay_clicked();
		void on_bHideScreen_clicked();
		void on_bInsertComment_clicked();
		void on_bItemManagement_clicked();
		void on_bJumpToLabel_clicked();
		void on_bKeyInputProcessing_clicked();
		void on_bLabel_clicked();
		void on_bLoadGame_clicked();
		void on_bLoop_clicked();
		void on_bMemorizeBGM_clicked();
		void on_bMemorizePosition_clicked();
		void on_bMoveEvent_clicked();
		void on_bOpenMenu_clicked();
		void on_bPanScreen_clicked();
		void on_bPlayBGM_clicked();
		void on_bPlayMemorizedBGM_clicked();
		void on_bPlayMovie_clicked();
		void on_bPlaySoundEffect_clicked();
		void on_bPlaySystemBGM_clicked();
		void on_bPlaySystemSoundEffect_clicked();
		void on_bProceedWithMovement_clicked();
		void on_bQuitGame_clicked();
		void on_bSaveGame_clicked();
		void on_bSetScreenTransition_clicked();
		void on_bShakeScreen_clicked();
		void on_bShowBattleAnimation_clicked();
		void on_bShowGlobalAnimation_clicked();
		void on_bShowLayer_clicked();
		void on_bShowPopupMenu_clicked();
		void on_bShowScreenOverlay_clicked();
		void on_bShowScreen_clicked();
		void on_bStoreEventID_clicked();
		void on_bStoreTerrainID_clicked();
		void on_bStringOperation_clicked();
		void on_bSwapTile_clicked();
		void on_bSwitchOperation_clicked();
		void on_bTeleport_clicked();
		void on_bTileSubstitution_clicked();
		void on_bTimerOperation_clicked();
		void on_bTintScreen_clicked();
		void on_bTradeTwoEventLocations_clicked();
		void on_bVariableMultiValueBranch_clicked();
		void on_bVariableOperation_clicked();
		void on_bWait_clicked();
		void on_bWeatherEffects_clicked();
	private:
		int selectedEvent;
		static int lastUsedTab;
};

#endif // BROWSEMAPEVENTS_H
