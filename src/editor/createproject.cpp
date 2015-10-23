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

#include <QFileDialog>
#include "createproject.h"
#include "projectdata.h"

CreateProject::CreateProject(bool renaming, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	gameValidator = new QRegExpValidator(QRegExp("^\\w(\\w|[ ]){0,}$"), 0);
	dirValidator = new QRegExpValidator(QRegExp("^\\w{1,}$"), 0);
	//leGameName->setValidator(gameValidator);
	leDirName->setValidator(dirValidator);
	leProjectPath->setText(ProjectData::projectsdir);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	frmDirName->setHidden(renaming);
	frmProjectPath->setHidden(renaming);
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

CreateProject::~CreateProject()
{
}

QString CreateProject::getGameName() const
{
	return leGameName->text();
}

QString CreateProject::getDirName() const
{
	return leDirName->text();
}

void CreateProject::on_leGameName_textChanged(QString text)
{
	QString autoDirName = text;
	autoDirName.remove(QRegExp("\\W"));
	if (text.isEmpty())
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	else if (!leDirName->text().isEmpty())
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	leDirName->setText(autoDirName);
}

void CreateProject::on_leDirName_textChanged(QString text)
{
	if (text.isEmpty())
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	else if (!leGameName->text().isEmpty())
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void CreateProject::on_tbBrowse_clicked()
{
	QString folderPath = QFileDialog::getExistingDirectory(0, "Open Directory", ProjectData::datadir, QFileDialog::ShowDirsOnly);
	if (!folderPath.isEmpty())
		ProjectData::projectsdir = folderPath;
}
