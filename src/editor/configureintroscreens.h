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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef CONFIGUREINTROSCREENS_H
#define CONFIGUREINTROSCREENS_H

#include "ui_configureintroscreens.h"

class ConfigureIntroScreens : public QDialog, public Ui::ConfigureIntroScreens
{
	Q_OBJECT
	public:
		ConfigureIntroScreens(QWidget *parent=0);
		~ConfigureIntroScreens();
	protected slots:
		void on_bAddEffect_clicked();
		void on_bAddScreen_clicked();
		void on_bBottomBGColor_colorChanged(QColor color);
		void on_bConfigureEffect_clicked();
		void on_bDeleteEffect_clicked();
		void on_bDeleteScreen_clicked();
		void on_bMoveEffectDown_clicked();
		void on_bMoveEffectUp_clicked();
		void on_bMoveScreenDown_clicked();
		void on_bMoveScreenUp_clicked();
		void on_bPreviewScreen_clicked();
		void on_bTopBGColor_colorChanged(QColor color);
		void on_buttonBox_accepted();
		void on_leScreenName_textEdited(const QString &text);
		void on_lwEffects_itemSelectionChanged();
		void on_lwScreens_itemSelectionChanged();
};

#endif // CONFIGUREINTROSCREENS_H
