#ifndef HELPER_H
#define HELPER_H

#include <QRegExp>

namespace Helper {
    inline float convertCoordinateInDec(QString coordStr)
    {
        QRegExp regExp("\\D?(1\\d{2}|0\\d{2}|\\d{2})\\s?(\\d{2})\\s?(\\d{2}(\\,|\\.\\d{1,2})?)");
        float dec = 0;

        if (regExp.indexIn(coordStr) != -1) {
            dec = regExp.cap(1).toInt() + regExp.cap(2).toFloat() / 60 + regExp.cap(3).replace(",", ".").toFloat() / 3600;
        }
        return dec;
    }
}

#endif // HELPER_H
