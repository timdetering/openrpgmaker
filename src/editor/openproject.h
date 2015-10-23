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

#ifndef OPENPROJECT_H
#define OPENPROJECT_H

class QDir;

#include "ui_openproject.h"

class OpenProject : public QDialog, public Ui::OpenProject
{
	Q_OBJECT
	public:
		OpenProject(QWidget *parent=0);
		~OpenProject();
		QString getProjectDir();
	protected slots:
		void on_bRename_clicked();
		void on_bDelete_clicked();
		void on_tbBrowse_clicked();
		void on_twProjectList_cellClicked();
	private:
		void updateProjectList();
		bool cleanDirectory(QDir dir);
};

#endif // OPENPROJECT_H
