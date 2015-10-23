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

#ifndef CONFIGUREEVENT_CHANGEMENUTEXT_H
#define CONFIGUREEVENT_CHANGEMENUTEXT_H

#include "ui_configureevent_changemenutext.h"
#include "menutext.h"

class ConfigureEvent_ChangeMenuText : public QDialog, public Ui::ConfigureEvent_ChangeMenuText
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeMenuText(int saveSlotID, int textType, MenuText::TextData *data, int menuID, int menuType, QWidget *parent=0);
		~ConfigureEvent_ChangeMenuText();
		void setTextObject(QString object);
		int getTextType();
		QString getTextObject();
		int getSaveSlotID();
	protected slots:
		void on_bBrowseSwitches_clicked();
		void on_bBrowseVariables_clicked();
		void on_bCharacterBrowseVariables_clicked();
		void on_bItemBrowseVariables_clicked();
		void on_bMonsterBrowseVariables_clicked();
		void on_bPartyMember_BrowseVariables_clicked();
		void on_bStringBrowseVariables_clicked();
		void on_rbPlainText_toggled(bool on);
		void on_rbPartyMember_toggled(bool on);
		void on_rbCharacter_toggled(bool on);
		void on_rbMonster_toggled(bool on);
		void on_rbItem_toggled(bool on);
		void on_rbString_toggled(bool on);
		void on_rbVariable_toggled(bool on);
		void on_rbSwitch_toggled(bool on);
		void on_rbCharacterEquippedAccessory_toggled(bool on);
		void on_rbCharacterEquippedArmor_toggled(bool on);
		void on_rbCharacterEquippedHelmet_toggled(bool on);
		void on_rbCharacterEquippedShield_toggled(bool on);
		void on_rbCharacterEquippedWeapon_toggled(bool on);
		void on_rbCharacterSpecific_toggled(bool on);
		void on_rbCharacterVariable_toggled(bool on);
		void on_rbItemSpecific_toggled(bool on);
		void on_rbItemVariable_toggled(bool on);
		void on_rbMonsterSpecific_toggled(bool on);
		void on_rbMonsterVariable_toggled(bool on);
		void on_rbPartyMemberEquippedArmor_toggled(bool on);
		void on_rbPartyMemberEquippedHelmet_toggled(bool on);
		void on_rbPartyMemberEquippedShield_toggled(bool on);
		void on_rbPartyMemberEquippedWeapon_toggled(bool on);
		void on_rbPartyMemberEquippedAccessory_toggled(bool on);
		void on_rbPartyMemberVar_toggled(bool on);
		void on_rbStringSystem_toggled(bool on);
		void on_rbStringVariable_toggled(bool on);
		void on_cbUseValueInSaveSlot_toggled(bool on);
		void on_buttonBox_accepted();
	private:
		MenuText::TextData *textData;
		int type;
};

#endif // CONFIGUREEVENT_CHANGEMENUTEXT_H
