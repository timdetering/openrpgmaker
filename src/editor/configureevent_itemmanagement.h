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

#ifndef CONFIGUREEVENT_ITEMMANAGEMENT_H
#define CONFIGUREEVENT_ITEMMANAGEMENT_H

#include "ui_configureevent_itemmanagement.h"

class ConfigureEvent_ItemManagement : public QDialog, public Ui::ConfigureEvent_ItemManagement
{
	Q_OBJECT
	public:
		ConfigureEvent_ItemManagement(QWidget *parent=0);
		~ConfigureEvent_ItemManagement();
		void setItemID(int itemID);
		void setIsReferencedItem(bool referenced);
		void setOperation(bool addItem);
		void setAmount(int amount);
		void setIsStoredAmount(bool stored);
		int getItemID();
		bool isReferencedItem();
		bool isAddItemOperation();
		int getAmount();
		bool isStoredAmount();
	protected slots:
		void on_bBrowseAmountVariables_clicked();
		void on_bBrowseItemVariables_clicked();
		void on_rbSpecificAmount_toggled(bool checked);
		void on_rbSpecificItem_toggled(bool checked);
};

#endif // CONFIGUREEVENT_ITEMMANAGEMENT_H
