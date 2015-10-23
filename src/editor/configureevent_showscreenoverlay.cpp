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

#include "configureevent_showscreenoverlay.h"
#include "projectdata.h"

TODO("Uncomment code when screen overlays are done.");

ConfigureEvent_ShowScreenOverlay::ConfigureEvent_ShowScreenOverlay(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	//cboxOverlays->addItems(ProjectData::screenOverlayNames);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_ShowScreenOverlay::~ConfigureEvent_ShowScreenOverlay()
{
}

int ConfigureEvent_ShowScreenOverlay::getOverlayID()
{
	//return ProjectData::screenOverlayNames.keys()[cboxOverlays->currentIndex()];
	return -1;
}

void ConfigureEvent_ShowScreenOverlay::setOverlayID(int value)
{
	/*int index = ProjectData::screenOverlayNames.keys().indexOf(value);
	cboxOverlays->setCurrentIndex(index);*/
}

int ConfigureEvent_ShowScreenOverlay::getTransition()
{
	return cboxTransition->currentIndex();
}

void ConfigureEvent_ShowScreenOverlay::setTransition(int value)
{
	cboxTransition->setCurrentIndex(value);
}
