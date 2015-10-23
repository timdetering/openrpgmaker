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

#ifndef CONFIGUREEVENT_WEATHEREFFECTS_H
#define CONFIGUREEVENT_WEATHEREFFECTS_H

#include "ui_configureevent_weathereffects.h"

class ConfigureEvent_WeatherEffects : public QDialog, public Ui::ConfigureEvent_WeatherEffects
{
	Q_OBJECT
	public:
		enum Effect {NONE=0, RAIN, SNOW, FOG, SANDSTORM};
		enum Strength {WEAK=0, MEDIUM, STRONG};
		ConfigureEvent_WeatherEffects(QWidget *parent=0);
		~ConfigureEvent_WeatherEffects();
		int getEffect();
		void setEffect(int effect);
		int getStrength();
		void setStrength(int strength);
	protected slots:
		void on_rbEffectNone_toggled(bool checked);
};

#endif // CONFIGUREEVENT_WEATHEREFFECTS_H
