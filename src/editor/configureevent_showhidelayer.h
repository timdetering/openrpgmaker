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

#ifndef CONFIGUREEVENT_SHOWHIDELAYER_H
#define CONFIGUREEVENT_SHOWHIDELAYER_H

#include "ui_configureevent_showhidelayer.h"

class ConfigureEvent_ShowHideLayer : public QDialog, public Ui::ConfigureEvent_ShowHideLayer
{
	Q_OBJECT
	public:
		ConfigureEvent_ShowHideLayer(int mapID, bool show, QWidget *parent=0);
		~ConfigureEvent_ShowHideLayer();
		void setLayer(int layer);
		void setTransition(int transition);
		void setLayerReferencedInVariable(bool layerReferencedInVariable);
		int getLayer();
		int getTransition();
		bool isLayerReferencedInVariable();
	protected slots:
		void on_bBrowseLayerVariables_clicked();
		void on_rbSpecificLayer_toggled(bool checked);
	private:
		bool showLayer;
};

#endif // CONFIGUREEVENT_SHOWHIDELAYER_H
