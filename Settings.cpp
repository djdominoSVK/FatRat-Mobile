/*
FatRat download manager
http://fatrat.dolezel.info

Copyright (C) 2006-2008 Lubos Dolezel <lubos a dolezel.info>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

In addition, as a special exemption, Luboš Doležel gives permission
to link the code of FatRat with the OpenSSL project's
"OpenSSL" library (or with modified versions of it that use the; same
license as the "OpenSSL" library), and distribute the linked
executables. You must obey the GNU General Public License in all
respects for all of the code used other than "OpenSSL".
*/

//#include "fatrat.h"

#include "Settings.h"
#include <QApplication>
#include <QSettings>
#include <QVector>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <iostream>

//#include "SettingsGeneralForm.h"
//#include "SettingsDropBoxForm.h"
//#include "SettingsNetworkForm.h"
//#include "SettingsQueueForm.h"
//#include "SettingsSchedulerForm.h"
//#include "SettingsClipboardMonitorForm.h"
//#include "rss/SettingsRssForm.h"

QVector<SettingsItem> g_settingsPages;
QSettings* g_settings = 0;

static QSettings* m_settingsDefaults = 0;


QList<QMap<QString, QVariant> > getSettingsArray(QString id)
{
	QList<QMap<QString, QVariant> > rv;
	int count = g_settings->beginReadArray(id);
	for (int i = 0; i < count; i++)
	{
		//g_settings->setArrayIndex(i);
		g_settings->beginGroup(QString::number(i+1));

		QMap<QString, QVariant> map;
		foreach (QString key, g_settings->childKeys())
		{
			qDebug() << "Key:" << key;
			map[key] = g_settings->value(key);
		}
		rv << map;
		g_settings->endGroup();
	}
	g_settings->endArray();

	return rv;
}

void setSettingsArray(QString id, QList<QMap<QString, QVariant> >& value)
{
	qDebug() << "setSettingsArray()";

	g_settings->beginWriteArray(id, value.size());
	for (int i = 0; i < value.size(); i++)
	{
		g_settings->setArrayIndex(i);
		for (QMap<QString, QVariant>::iterator it = value[i].begin(); it != value[i].end(); it++)
			g_settings->setValue(it.key(), it.value());
	}
	g_settings->endArray();
}

QVariant getSettingsValue(QString id, QVariant def)
{
	QVariant mdef = getSettingsDefault(id);
	if (mdef.isNull())
		mdef = def;
	return g_settings->value(id, mdef);
}

void setSettingsValue(QString id, QVariant value)
{
	g_settings->setValue(id, value);
}

void initSettingsDefaults(QString manualPath)
{
	if (manualPath.isEmpty())
		g_settings = new QSettings;
	else
        g_settings = new QSettings(manualPath, QSettings::IniFormat, qApp);
	
	QLatin1String df("/defaults.conf");
    QString path = "/home";
//	QString path = getDataFileDir("/data", df) + df;

	if (!QFile::exists(path)) {
		const char* error = "Your installation is incomplete.\nIf you compiled FatRat from source, please learn how to install programs first!\n";
		std::cerr << error;

		if (getenv("DISPLAY"))
			QMessageBox::critical(0, "FatRat", error);
		exit(1);
	}

    m_settingsDefaults = new QSettings(path, QSettings::IniFormat, qApp);
}

void exitSettings()
{
	delete g_settings;
}

QVariant getSettingsDefault(QString id)
{
	return m_settingsDefaults->value(id);
}

void applyAllSettings()
{
	for (int i = 0; i < g_settingsPages.size(); i++)
	{
		if (g_settingsPages[i].pfnApply) {
			qDebug() << "Applying" << g_settingsPages[i].title;
			g_settingsPages[i].pfnApply();
		}
	}
}

