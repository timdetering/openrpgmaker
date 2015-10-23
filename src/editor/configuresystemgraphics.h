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

#ifndef CONFIGURESYSTEMGRAPHICS_H
#define CONFIGURESYSTEMGRAPHICS_H

#include "ui_configuresystemgraphics.h"

class ConfigureSystemGraphics : public QDialog, public Ui::ConfigureSystemGraphics
{
	Q_OBJECT
	public:
		ConfigureSystemGraphics(QWidget *parent=0);
		~ConfigureSystemGraphics();
	protected slots:
		void on_bSetFrameBG_clicked();
		void on_bSetFrameBorder_clicked();
		void on_bSetSelectionBorder_clicked();
		void on_bSetEquipmentIndicators_clicked();
		void on_bSetMenuBG_clicked();
		void on_bSetScrollArrows_clicked();
		void on_bSetGrappleArm_clicked();
		void on_bSetGrappleHook_clicked();
		void on_bSetTimerNumbers_clicked();
		void on_bDefaultEnabledTopColor_colorChanged(QColor color);
		void on_bDefaultEnabledBottomColor_colorChanged(QColor color);
		void on_bDefaultDisabledTopColor_colorChanged(QColor color);
		void on_bDefaultDisabledBottomColor_colorChanged(QColor color);
		void on_buttonBox_accepted();
	private:
		QImage createGradientImage(QColor top, QColor bottom, int width, int height);
		QString frameBGLocation;
		QString frameBorderLocation;
		QString selectionBorderLocation;
		QString equipmentIndicatorsLocation;
		QString menuBGLocation;
		QString scrollArrowsLocation;
		QString grappleArmLocation;
		QString grappleHookLocation;
		QString timerNumbersLocation;
};

#endif // CONFIGURESYSTEMGRAPHICS_H
