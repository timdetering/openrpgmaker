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

#include "convertprojectdialog.h"
#include "helpmanual.h"
#include "projectdata.h"

ConvertProjectDialog::ConvertProjectDialog(int fileFormatVersion, QWidget *parent) : QDialog(parent)
{
	QString message;
	setupUi(this);
	message = lblMessage->text();
	message.replace("$(PROJECT_LOCATION)", ProjectData::projectLocation);
	message.replace("$(PROJECTS_DIR)", ProjectData::projectsdir);
	message.replace("$(DIRECTORY_NAME)", ProjectData::directoryName);
	message.replace("$(PROJECT_FILE_FORMAT_VERSION)", QString::number(fileFormatVersion));
	lblMessage->setText(message);
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConvertProjectDialog::~ConvertProjectDialog()
{
}

void ConvertProjectDialog::on_buttonBox_helpRequested()
{
	ProjectData::helpManual->loadIndex("File Format Changes");
	if (!ProjectData::helpManual->isVisible())
		ProjectData::helpManual->show();
	else
		ProjectData::helpManual->raise();
	ProjectData::helpManual->activateWindow();
}
