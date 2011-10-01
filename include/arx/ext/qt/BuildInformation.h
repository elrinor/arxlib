/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2011 Alexander Fokin <apfokin@gmail.com>
 *
 * ArXLib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * ArXLib is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ArXLib. If not, see <http://www.gnu.org/licenses/>. 
 * 
 * $Id$ */
#ifndef ARX_EXT_QT_BUILD_INFORMATION_H
#define ARX_EXT_QT_BUILD_INFORMATION_H

#include <arx/config.h>
#include <cassert>
#include <QDateTime>
#include <QString>
#include <arx/utility/Fatal.h>

namespace arx {
// -------------------------------------------------------------------------- //
// BuildInformation
// -------------------------------------------------------------------------- //
  class BuildInformation {
  public:
    /**
     * @returns                        Build time of this executable.
     */
    QDateTime dateTime() const {
      const char buildTime[] = __TIME__;
      const char buildDate[] = __DATE__;

      static_assert(sizeof(buildTime) == 9,  "__TIME__ was expected to expand into a \"hh:mm:ss\" string.");
      static_assert(sizeof(buildDate) == 12, "__DATE__ was expected to expand into a \"MMM dd yyyy\" string");

      QString timeString = QLatin1String(buildTime);
      QString dateString = QLatin1String(buildDate);

      QDate date(
        dateString.mid(7, 4).toInt(), 
        monthNumber(dateString.mid(0, 3)), 
        dateString.mid(4, 2).toInt()
      );
        
      return QDateTime(date, QTime::fromString(timeString, QLatin1String("hh:mm:ss")));
    }


  private:
    /**
     * @param monthString              Three-character English month string.
     * @returns                        Month number.
     */
    int monthNumber(const QString &monthString) const {
      assert(monthString.size() == 3);
      assert(monthString[0].isUpper());

      const char *monthStrings[] = {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec"
      };

      for(int i = 0; i < 12; i++)
        if(monthString == QLatin1String(monthStrings[i]))
          return i + 1;

      fatal();
      return 0;
    }
  };

} // namespace arx

#endif // ARX_EXT_QT_BUILD_INFORMATION_H
