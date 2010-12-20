/***************************************************************************
 *   Copyright (C) 2010-2011 by Francesco Cecconi                          *
 *   francesco.cecconi@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "../mainwin.h"


void nmapClass::saveUiSettings() {
    QSettings settings("nmapsi4", "nmapsi4");
    if(savePos) {
        settings.setValue("window/pos", pos());
    }
    if(saveSize) {
        settings.setValue("window/size", size());
    }
    settings.setValue("NSSsupport", NSSsupport);
    settings.setValue("ADVSupport", ADVSupport);
    settings.setValue("HostEnabled", HostDetEnabled);
    settings.setValue("MonitorEnabled", MonitorEnabled);
    settings.setValue("LookupEnabled", LookupEnabled);
    settings.setValue("TraceEnabled", TraceEnabled);
    settings.setValue("splitterSizes", cW->saveState());
}