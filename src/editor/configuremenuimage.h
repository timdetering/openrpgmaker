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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef CONFIGUREMENUIMAGE_H
#define CONFIGUREMENUIMAGE_H

#include "ui_configuremenuimage.h"
#include "menuimage.h"

class ConfigureMenuImage : public QDialog, public Ui::ConfigureMenuImage
{
	Q_OBJECT
	public:
		ConfigureMenuImage(MenuImage *imageToConfigure, QWidget *parent=0, bool enableSelectSaveSlot=true);
		~ConfigureMenuImage();
	protected slots:
		void on_bBrowseFiles_clicked();
		void on_bCharacter_BrowseVariables_clicked();
		void on_bClass_BrowseVariables_clicked();
		void on_bMonster_BrowseVariables_clicked();
		void on_bPartyMember_BrowseVariables_clicked();
		void on_bTile_BrowseVariables_clicked();
		void on_bTileset_BrowseVariables_clicked();
		void on_bVehicle_BrowseVariables_clicked();
		void on_rbFile_toggled(bool checked);
		void on_rbPartyMember_toggled(bool checked);
		void on_rbCharacter_toggled(bool checked);
		void on_rbClass_toggled(bool checked);
		void on_rbMonster_toggled(bool checked);
		void on_rbVehicle_toggled(bool checked);
		void on_rbTile_toggled(bool checked);
		void on_rbCharacter_BattleSprite_toggled(bool checked);
		void on_rbCharacter_FaceGraphic_toggled(bool checked);
		void on_rbCharacter_MapSprite_toggled(bool checked);
		void on_rbCharacter_Specific_toggled(bool checked);
		void on_rbCharacter_Variable_toggled(bool checked);
		void on_rbClass_BattleSprite_toggled(bool checked);
		void on_rbClass_MapSprite_toggleD(bool checked);
		void on_rbClass_Specific_toggled(bool checked);
		void on_rbClass_Variable_toggled(bool checked);
		void on_rbMonster_Specific_toggled(bool checked);
		void on_rbMonster_Variable_toggled(bool checked);
		void on_rbPartyMemberVar_toggled(bool checked);
		void on_rbPartyMember_BattleSprite_toggled(bool checked);
		void on_rbPartyMember_FaceGraphic_toggled(bool checked);
		void on_rbPartyMember_MapSprite_toggled(bool checked);
		void on_rbTile_Specific_toggled(bool checked);
		void on_rbTile_Variable_toggled(bool checked);
		void on_rbVehicle_Specific_toggled(bool checked);
		void on_rbVehicle_Variable_toggled(bool checked);
		void on_cbUseValueInSaveSlot_toggled(bool checked);
		void on_buttonBox_accepted();
	private:
		MenuImage *image;
};

#endif // CONFIGUREMENUIMAGE_H
