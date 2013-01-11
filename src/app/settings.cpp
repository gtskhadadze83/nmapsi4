/*
Copyright 2010-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

void MainWindow::syncSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");

    // load saved profile index
    m_savedProfileIndex = settings.value("savedProfileIndex", 0).toInt();
    m_hostCache = settings.value("hostCache", 10).toInt();

#if defined(Q_OS_WIN32)
    // disable lookup in MS windows
    m_lookupType = 0;
#else
    m_lookupType = settings.value("lookupType", 1).toInt();
#endif

    // restore actionMenuBar
    m_collections->m_collectionsScanSection.value("showmenubar-action")->setChecked(settings.value("showMenuBar", false).toBool());
    // update max parallel scan option
    m_monitor->updateMaxParallelScan();
}

void MainWindow::saveSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");

    settings.setValue("window/pos", pos());
    settings.setValue("window/geometry", saveGeometry());
    settings.setValue("hostCache", m_hostCache);
    settings.setValue("splitterSizes", m_mainHorizontalSplitter->saveState());
    settings.setValue("splitterSizesRight", m_mainVerticalSplitter->saveState());
    settings.setValue("showMenuBar", m_collections->m_collectionsScanSection.value("showmenubar-action")->isChecked());
    settings.setValue("savedProfileIndex", m_scanWidget->comboParametersProfiles->currentIndex());

    m_bookmark->syncSettings();
    m_discoverManager->syncSettings();
    m_parser->syncSettings();
    m_vulnerability->syncSettings();

// check and reset for settings file permission
#if !defined(Q_OS_WIN32)
    if (!m_userId) {
        QFileInfo fileInfo;

        fileInfo.setFile(settings.fileName());
        if ((!fileInfo.permissions().testFlag(QFile::WriteOther) && !fileInfo.ownerId())) {
            QFile::setPermissions(settings.fileName(), QFile::ReadOwner | QFile::ReadUser | QFile::ReadOther |
                                  QFile::WriteOwner | QFile::WriteUser | QFile::WriteOther);
        }

        QSettings settingsBookmark("nmapsi4", "nmapsi4_bookmark");
        fileInfo.setFile(settingsBookmark.fileName());
        if ((!fileInfo.permissions().testFlag(QFile::WriteOther) && !fileInfo.ownerId())) {
            QFile::setPermissions(settingsBookmark.fileName(), QFile::ReadOwner | QFile::ReadUser | QFile::ReadOther |
                                  QFile::WriteOwner | QFile::WriteUser | QFile::WriteOther);
        }
    }
#endif
}

void MainWindow::updateCompleter()
{
    if (!m_bookmark->isBookmarkHostListEmpty()) {
        if (!m_completer.isNull()) {
            QStringListModel *newModel = qobject_cast<QStringListModel*>(m_completer.data()->model());
            newModel->setStringList(m_bookmark->getHostListFromBookmark());
        } else if (m_hostModel.isNull()) {
            m_hostModel = new QStringListModel(m_bookmark->getHostListFromBookmark(), this);
        }
    }
}

void MainWindow::restoreSettings()
{
    // restore window position
    QSettings settings("nmapsi4", "nmapsi4");
    QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
    move(pos);

    if (settings.contains("window/geometry")) {
        restoreGeometry(settings.value("window/geometry").toByteArray());
    } else {
        resize(QSize(700, 500));
    }

    // restore state of the QAction's connected to splitter widget
    if (!settings.value("splitterSizes").toByteArray().isEmpty()) {
        m_mainHorizontalSplitter->restoreState(settings.value("splitterSizes").toByteArray());

        if (m_mainHorizontalSplitter->sizes()[0]) {
            m_collections->m_collectionsButton.value("scan-list")->setChecked(true);
        } else {
            m_collections->m_collectionsButton.value("scan-list")->setChecked(false);
        }
    } else {
        m_collections->m_collectionsButton.value("scan-list")->setChecked(true);
    }

    if (!settings.value("splitterSizesRight").toByteArray().isEmpty()) {
        m_mainVerticalSplitter->restoreState(settings.value("splitterSizesRight").toByteArray());

        if (m_mainVerticalSplitter->sizes()[1]) {
            m_collections->m_collectionsButton.value("details-list")->setChecked(true);
        } else {
            m_collections->m_collectionsButton.value("details-list")->setChecked(false);
        }
    } else {
        m_collections->m_collectionsButton.value("details-list")->setChecked(true);
    }
}

void MainWindow::setDefaultSplitter()
{
    // define default Ui splitter
    m_mainHorizontalSplitter = new QSplitter(this);
    m_mainVerticalSplitter = new QSplitter(this);

    connect(m_mainVerticalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(resizeVerticalSplitterEvent()));

    connect(m_mainHorizontalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(resizeHorizontalSplitterEvent()));

    m_mainHorizontalSplitter->setOrientation(Qt::Horizontal);
    m_mainHorizontalSplitter->addWidget(m_scanWidget->frameLeft);
    m_mainHorizontalSplitter->addWidget(m_scanWidget->frameCenter);
    m_mainVerticalSplitter->setOrientation(Qt::Vertical);
    m_mainVerticalSplitter->addWidget(m_scanWidget->tabWidget);
    m_mainVerticalSplitter->addWidget(m_scanWidget->frameRight);
    // insert splitter
    m_scanWidget->layout()->addWidget(m_mainHorizontalSplitter);
    m_scanWidget->frameCenter->layout()->addWidget(m_mainVerticalSplitter);
}
