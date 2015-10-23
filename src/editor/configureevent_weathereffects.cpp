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

#include "configureevent_weathereffects.h"

ConfigureEvent_WeatherEffects::ConfigureEvent_WeatherEffects(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	frmStrength->setEnabled(false);
}

ConfigureEvent_WeatherEffects::~ConfigureEvent_WeatherEffects()
{
}

int ConfigureEvent_WeatherEffects::getEffect()
{
	if (rbEffectNone->isChecked())
		return NONE;
	else if (rbEffectRain->isChecked())
		return RAIN;
	else if (rbEffectSnow->isChecked())
		return SNOW;
	else if (rbEffectFog->isChecked())
		return FOG;
	return SANDSTORM;
}

void ConfigureEvent_WeatherEffects::setEffect(int effect)
{
	if (effect == NONE)
		rbEffectNone->setChecked(true);
	else if (effect == RAIN)
		rbEffectRain->setChecked(true);
	else if (effect == SNOW)
		rbEffectSnow->setChecked(true);
	else if (effect == FOG)
		rbEffectFog->setChecked(true);
	else if (effect == SANDSTORM)
		rbEffectSandstorm->setChecked(true);
}

int ConfigureEvent_WeatherEffects::getStrength()
{
	if (rbStrengthWeak->isChecked())
		return WEAK;
	else if (rbStrengthMedium->isChecked())
		return MEDIUM;
	return STRONG;
}

void ConfigureEvent_WeatherEffects::setStrength(int strength)
{
	if (strength == WEAK)
		rbStrengthWeak->setChecked(true);
	else if (strength == MEDIUM)
		rbStrengthMedium->setChecked(true);
	else if (strength == STRONG)
		rbStrengthStrong->setChecked(true);
}

void ConfigureEvent_WeatherEffects::on_rbEffectNone_toggled(bool checked)
{
	frmStrength->setEnabled(!checked);
}
