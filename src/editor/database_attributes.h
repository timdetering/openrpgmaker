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

#ifndef DATABASE_ATTRIBUTES_H
#define DATABASE_ATTRIBUTES_H

class Attribute;

#include "ui_database_attributes.h"
#include "projectdata.h"

class Database_Attributes : public QWidget, public Ui::Database_Attributes
{
	Q_OBJECT
	public:
		Database_Attributes(QWidget *parent=0);
		~Database_Attributes();
		void setupAttributes();
		void shutdownAttributes();
		void applyAttributes();
	protected slots:
		void on_lwAttributeList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_rbTypeWeapon_toggled(bool on);
		void on_rbTypeMagic_toggled(bool on);
		void on_sbMultiplierA_valueChanged(int value);
		void on_sbMultiplierB_valueChanged(int value);
		void on_sbMultiplierC_valueChanged(int value);
		void on_sbMultiplierD_valueChanged(int value);
		void on_sbMultiplierE_valueChanged(int value);
	private:
		void updateAttributeList();
		ProjectData::DataReference<Attribute> *attributeRef;
};

#endif // DATABASE_ATTRIBUTES
