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

#ifndef CONFIGUREMENUITEMLIST_H
#define CONFIGUREMENUITEMLIST_H

#include "ui_configuremenuitemlist.h"
#include "menuitemtable.h"

class ConfigureMenuItemList : public QDialog, public Ui::ConfigureMenuItemList
{
	Q_OBJECT
	public:
		ConfigureMenuItemList(MenuItemTable *listToConfigure, QWidget *parent=0);
		~ConfigureMenuItemList();
	protected slots:
		void on_buttonBox_accepted();
		void on_cbUseVariablesToSetState_toggled(bool checked);
		void on_rbAvailableEquipment_toggled(bool checked);
		void on_rbCharacters_toggled(bool checked);
		void on_rbClasses_toggled(bool checked);
		void on_rbMonsters_toggled(bool checked);
		void on_rbPartysItems_toggled(bool checked);
		void on_rbSkills_toggled(bool checked);
		void on_rbVehicles_toggled(bool checked);
		void on_skills_rbPartyMemberVar_toggled(bool checked);
		void on_equip_rbPartyMemberVar_toggled(bool checked);
		void on_skills_bPartyMember_BrowseVariables_clicked();
		void on_equip_bPartyMember_BrowseVariables_clicked();
		void on_other_bBrowseVariables_clicked();
	private:
		void setupSkillTypeList();
		MenuItemTable *list;
};

#endif // CONFIGUREMENUITEMLIST_H
