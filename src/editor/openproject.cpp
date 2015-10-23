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

#include <QMessageBox>
#include <QFileDialog>
#include "createproject.h"
#include "openproject.h"
#include "projectdata.h"
#include "xmlParser.h"

OpenProject::OpenProject(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	twProjectList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	twProjectList->verticalHeader()->setHidden(true);
	leProjectPath->setText(ProjectData::projectsdir);
	updateProjectList();
}

OpenProject::~OpenProject()
{
}

QString OpenProject::getProjectDir()
{
	return twProjectList->item(twProjectList->currentRow(), 1)->text();
}

void OpenProject::on_bRename_clicked()
{
	CreateProject *createproject = new CreateProject();
	createproject->leGameName->setText(twProjectList->item(twProjectList->currentRow(), 0)->text());
	createproject->leDirName->setText(twProjectList->item(twProjectList->currentRow(), 1)->text());
	if (createproject->exec())
	{
		XMLNode ProjectNode = XMLNode::openFileHelper((ProjectData::projectsdir + QString("/") + twProjectList->item(twProjectList->currentRow(), 1)->text() + QString("/projectdata.xml")).toLocal8Bit().data(), "openrpgmaker-projectdata");
		QDir dir(ProjectData::projectsdir + QString("/") + twProjectList->item(twProjectList->currentRow(), 1)->text());
		if (dir.exists() && ProjectNode.isEmpty() == FALSE)
		{
			if (ProjectNode.isAttributeSet("name"), TRUE)
			{
				if (dir.dirName() != createproject->getDirName())
				{
					dir.cdUp();
					if (dir.rename(ProjectData::projectsdir + QString("/") + dir.dirName(), ProjectData::projectsdir + QString("/") + createproject->getDirName()))
					{
						ProjectNode.updateAttribute(createproject->getGameName().toUtf8().data(), NULL, "name");
						ProjectNode.writeToFile((ProjectData::projectsdir + QString("/") + createproject->getDirName() + "/projectdata.xml").toLocal8Bit().data());
						twProjectList->item(twProjectList->currentRow(), 0)->setText(createproject->getGameName());
						twProjectList->item(twProjectList->currentRow(), 1)->setText(createproject->getDirName());
					}
				}
				else
				{
					ProjectNode.updateAttribute(createproject->getGameName().toUtf8().data(), NULL, "name");
					ProjectNode.writeToFile((ProjectData::projectsdir + QString("/") + createproject->getDirName() + "/projectdata.xml").toLocal8Bit().data());
					twProjectList->item(twProjectList->currentRow(), 0)->setText(createproject->getGameName());
					twProjectList->item(twProjectList->currentRow(), 1)->setText(createproject->getDirName());
				}
			}
		}
	}
	updateProjectList();
}

void OpenProject::on_bDelete_clicked()
{
	QString directoryName = twProjectList->item(twProjectList->currentRow(), 1)->text();
	int confirm = QMessageBox::question(NULL, "Confirm Delete", "Are you sure you want to delete the selected project.\n", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
	if (confirm == QMessageBox::Yes)
	{
		QDir dir(ProjectData::projectsdir);
		if (cleanDirectory(QDir(ProjectData::projectsdir + QString("/") + directoryName)))
		{
			if (!dir.rmdir(directoryName))
				QMessageBox::critical(NULL, "Error: Delete Failed", "Failed to delete the project directory.\n");
			updateProjectList();
		}
	}
}

void OpenProject::on_tbBrowse_clicked()
{
	QString folderPath = QFileDialog::getExistingDirectory(0, "Open Directory", ProjectData::datadir, QFileDialog::ShowDirsOnly);
	if (!folderPath.isEmpty())
	{
		ProjectData::projectsdir = folderPath;
		updateProjectList();
	}
}

void OpenProject::on_twProjectList_cellClicked()
{
	bOpen->setEnabled(true);
	bRename->setEnabled(true);
	bDelete->setEnabled(true);
}

void OpenProject::updateProjectList()
{
	QTableWidgetItem *item;
	QStringList projectList = QDir(ProjectData::projectsdir).entryList(QDir::Hidden|QDir::AllDirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
	QStringList projectGameNames;
	QStringList projectDirNames;
	for (int i = 0; i < projectList.size(); ++i)
	{
		QFileInfo info(ProjectData::projectsdir + QString("/") + projectList[i] + QString("/projectdata.xml"));
		if (info.exists())
		{
			XMLNode ProjectData = XMLNode::openFileHelper(info.absoluteFilePath().toLocal8Bit().data(), "openrpgmaker-projectdata");
			if (ProjectData.isEmpty() == FALSE)
			{
				if (ProjectData.isAttributeSet("name") == TRUE)
				{
					projectDirNames.append(info.dir().dirName());
					projectGameNames.append(QString(ProjectData.getAttribute("name")));
				}
			}
			else
				QMessageBox::critical(0, "XML Error", XMLNode::parseError);
		}
	}
	twProjectList->clearContents();
	twProjectList->setRowCount(projectGameNames.size());
	for (int i = 0; i < projectGameNames.size(); ++i)
	{
		item = new QTableWidgetItem(projectGameNames[i]);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		twProjectList->setItem(i, 0, item);
		item = new QTableWidgetItem(projectDirNames[i]);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		twProjectList->setItem(i, 1, item);
		twProjectList->setRowHeight(i, 15);
	}
	if (twProjectList->rowCount() >= 0 && twProjectList->currentRow() >= 0 && twProjectList->currentRow() < twProjectList->rowCount())
	{
		bOpen->setEnabled(true);
		bRename->setEnabled(true);
		bDelete->setEnabled(true);
	}
	else
	{
		bOpen->setEnabled(false);
		bRename->setEnabled(false);
		bDelete->setEnabled(false);
	}
}

bool OpenProject::cleanDirectory(QDir dir)
{
	QFileInfoList infoList = dir.entryInfoList(QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot|QDir::Hidden);
	for (int i = 0; i < infoList.size(); ++i)
	{
		QFileInfo info = infoList[i];
		if (!info.isWritable())
		{
			QMessageBox::critical(NULL, "Error: Delete Failed", QString("Failed to delete the project directory, as you don't have write permision to the file \"%1\"").arg(info.absoluteFilePath()));
			return false;
		}
		if (info.isDir())
		{
			if (!cleanDirectory(QDir(info.absoluteFilePath())))
				return false;
			if (!dir.rmdir(info.fileName()))
			{
				QMessageBox::critical(NULL, "Error: Delete Failed", QString("Failed to delete the directory \"%1\".\n").arg(info.absoluteFilePath()));
				return false;
			}
		}
		else if (!dir.remove(info.fileName()))
		{
			QMessageBox::critical(NULL, "Error: Delete Failed", QString("Failed to delete the file \"%1\".\n").arg(info.absoluteFilePath()));
			return false;
		}
	}
	return true;
}
