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

#ifndef CONFIGUREEVENT_CHANGEENCOUNTERRATE_H
#define CONFIGUREEVENT_CHANGEENCOUNTERRATE_H

#include "ui_configureevent_changeencounterrate.h"

class ConfigureEvent_ChangeEncounterRate : public QDialog, public Ui::ConfigureEvent_ChangeEncounterRate
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeEncounterRate(QWidget *parent=0);
		~ConfigureEvent_ChangeEncounterRate();
		void setEncounterRate(int rate);
		int getEncounterRate();
		void setMinSteps(int min);
		int getMinSteps();
		void setMaxSteps(int max);
		int getMaxSteps();
};

#endif // CONFIGUREEVENT_CHANGEENCOUNTERRATE_H
