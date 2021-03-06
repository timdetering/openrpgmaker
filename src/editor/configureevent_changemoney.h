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

#ifndef CONFIGUREEVENT_CHANGEMONEY_H
#define CONFIGUREEVENT_CHANGEMONEY_H

#include "ui_configureevent_changemoney.h"

class ConfigureEvent_ChangeMoney : public QDialog, public Ui::ConfigureEvent_ChangeMoney
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeMoney(QWidget *parent=0);
		~ConfigureEvent_ChangeMoney();
		void setOperation(bool increase);
		void setAmount(int amount);
		void setIsStoredAmount(bool storedAmount);
		bool isIncreaseOperation();
		int getAmount();
		bool isStoredAmount();
	protected slots:
		void on_bBrowseAmountVariables_clicked();
		void on_rbSpecificAmount_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGEMONEY_H
