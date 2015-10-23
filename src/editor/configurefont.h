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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef CONFIGUREFONT_H
#define CONFIGUREFONT_H

class Font;

#include "ui_configurefont.h"
#include "projectdata.h"

class ConfigureFont : public QDialog, public Ui::ConfigureFont
{
	Q_OBJECT
	public:
		ConfigureFont(int fontID, QWidget *parent=0);
		~ConfigureFont();
	protected slots:
		void on_bRenderPreviewText_clicked();
		void on_bSetImage_clicked();
		void on_buttonBox_accepted();
	private:
		ProjectData::DataReference<Font> *fontRef;
		QString imageLocation;
};

#endif // CONFIGUREFONT_H
