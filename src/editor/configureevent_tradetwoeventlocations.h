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

#ifndef CONFIGUREEVENT_TRADETWOEVENTLOCATIONS_H
#define CONFIGUREEVENT_TRADETWOEVENTLOCATIONS_H

#include "ui_configureevent_tradetwoeventlocations.h"

class ConfigureEvent_TradeTwoEventLocations : public QDialog, public Ui::ConfigureEvent_TradeTwoEventLocations
{
	Q_OBJECT
	public:
		enum EventType {HERO=0, THIS_EVENT, VEHICLE, EVENT};
		ConfigureEvent_TradeTwoEventLocations(int mapID, QWidget *parent=0);
		~ConfigureEvent_TradeTwoEventLocations();
		int getEvent1Type();
		void setEvent1Type(int value);
		int getEvent1ID();
		void setEvent1ID(int value);
		int getEvent2Type();
		void setEvent2Type(int value);
		int getEvent2ID();
		void setEvent2ID(int value);
	private:
		QList<int> typeList;
		QList<int> idList;
		QList<int> vehicleIndexes;
		QList<int> eventIndexes;
};

#endif // CONFIGUREEVENT_TRADETWOEVENTLOCATIONS_H
