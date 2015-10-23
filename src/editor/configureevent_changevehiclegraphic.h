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

#ifndef CONFIGUREEVENT_CHANGEVEHICLEGRAPHIC_H
#define CONFIGUREEVENT_CHANGEVEHICLEGRAPHIC_H

#include "ui_configureevent_changevehiclegraphic.h"

class ConfigureEvent_ChangeVehicleGraphic : public QDialog, public Ui::ConfigureEvent_ChangeVehicleGraphic
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeVehicleGraphic(QWidget *parent=0);
		~ConfigureEvent_ChangeVehicleGraphic();
		void setVehicleID(int vehicleID);
		void setSpriteID(int spriteID);
		void setIsReferencedVehicle(bool referenced);
		void setIsReferencedSprite(bool referenced);
		void setSpriteLocation(QString location);
		int getVehicleID();
		int getSpriteID();
		bool isReferencedVehicle();
		bool isReferencedSprite();
	protected slots:
		void on_bBrowseVehicleVariables_clicked();
		void on_rbSpecificVehicle_toggled(bool checked);
		void on_rbSpecificSprite_toggled(bool checked);
		void on_bBrowseSpriteVariables_clicked();
};

#endif // CONFIGUREEVENT_CHANGEVEHICLEGRAPHIC_H
