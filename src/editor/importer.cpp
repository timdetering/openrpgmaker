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

#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "importer.h"
#include "importerplugininterface.h"
#include "projectdata.h"

Importer::Importer(QWidget *parent) : QDialog(parent)
{
	QDir pluginsDir(ProjectData::datadir + "/importers" + ARCH_TYPE);
	QFileInfoList fileLocations = pluginsDir.entryInfoList(QDir::Files, QDir::Name);
	setupUi(this);
	for (int i = 0; i < fileLocations.size(); ++i)
	{
		if (QLibrary::isLibrary(fileLocations[i].absoluteFilePath()))
		{
			QPluginLoader pluginLoader(fileLocations[i].absoluteFilePath());
			pluginLoader.setLoadHints(QLibrary::ExportExternalSymbolsHint);
			ImporterPluginInterface *plugin = qobject_cast<ImporterPluginInterface*>(pluginLoader.instance());
			if (plugin != NULL)
			{
				pluginLocations.append(fileLocations[i].absoluteFilePath());
				pluginNames.append(plugin->getImporterName());
				pluginDescriptions.append(plugin->getImporterDescription());
			}
			else
				printf("%s\n", pluginLoader.errorString().toUtf8().data());
			pluginLoader.unload();
		}
	}
	for (int i = 0; i < pluginNames.size(); ++i)
		lwPluginList->addItem(pluginNames[i]);
	if (lwPluginList->count() > 0)
		lwPluginList->setCurrentRow(0);
	else
	{
		QMessageBox::warning(0, "No Importer Plugins", "There are no importer plugins available.\nYou can add importer plugins by placing them in the \"data/importers\" folder.");
		reject();
	}
}

Importer::~Importer()
{
}

void Importer::on_lwPluginList_currentRowChanged(int row)
{
	lblDescription->setText(pluginDescriptions[row]);
}

void Importer::on_bRunPlugin_clicked()
{
	int currentRow = lwPluginList->currentRow();
	QPluginLoader pluginLoader(pluginLocations[currentRow]);
	pluginLoader.setLoadHints(QLibrary::ExportExternalSymbolsHint);
	ImporterPluginInterface *plugin = qobject_cast<ImporterPluginInterface*>(pluginLoader.instance());
	if (plugin != NULL)
	{
		ProjectData::saveToStorage();
		ProjectData::saveAllDataReferencesToStorage();
		plugin->initializeImporter(ProjectData::homePath);
		plugin->runImporter();
		plugin->shutdownImporter();
		ProjectData::loadImportedData();
		ProjectData::loadFromStorage();
	}
	else
		QMessageBox::critical(0, "Error", pluginLoader.errorString());
	pluginLoader.unload();
}
