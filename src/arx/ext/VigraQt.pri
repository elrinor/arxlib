# This file is part of ArXLib, a C++ ArX Primitives Library.
#
# Copyright (C) 2008-2010 Alexander Fokin <apfokin@gmail.com>
#
# ArXLib is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# ArXLib is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
# for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with ArXLib. If not, see <http://www.gnu.org/licenses/>. 
# 
# $Id: FormPrinter.pri 112 2010-08-22 12:52:58Z ru.elric $
SOURCES += $$PWD/qt/QtStaticPlugins.cpp

contains(CONFIG, static) {
  QTPLUGIN += qjpeg qgif qmng qico qtiff
  DEFINES  += ARX_QT_STATIC_PLUGINS
}

DEFINES     += ARX_VIGRA_QT_PRI_INCLUDED

INCLUDEPATH += $$PWD/..
DEPENDPATH  += $$PWD/..

