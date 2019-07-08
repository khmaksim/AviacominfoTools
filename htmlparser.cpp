#include "htmlparser.h"
#include "databaseaccess.h"
#include <QDebug>

HtmlParser::HtmlParser(TypeData type) : QObject()
{
    this->type = type;
}

HtmlParser::~HtmlParser()
{

}

void HtmlParser::process()
{
    int pos = 0;

    if (type == TypeData::Airfields) {
        QString startMenu = "<div class=\"menu\">";
        QRegExp regExp("<a target=\"view_frame\" href=\"([^\"]+)\">([^<]+)<br /><b>([^<]+)</b></a>");

        if ((pos = htmlData.indexOf(startMenu, pos)) > 0) {
            while ((pos = regExp.indexIn(htmlData, pos)) != -1) {
                pos += regExp.matchedLength();
                QString href = regExp.cap(1);
                Airfield airfield;
                airfield.name = regExp.cap(2);
                airfield.icao = regExp.cap(3);

                DatabaseAccess::getInstance()->addAirfield(airfield);
                emit parseAirfieldCompleted(href);
                break;
            }
        }
    }
    else {
        QRegExp tableBeginRegExp("<tbody");
        QRegExp rowBeginRegExp("<tr[^>]*>");
        QRegExp rowEndRegExp("</tr>");
        QRegExp colRegExp("<td>([^<]*)</td>");
        QVector<QVector<QString>> obstracles;
        QVector<QString> obstracle;
        int posEndRow = 0;

        htmlData.replace("<td />", "<td></td>");        // fixed syntax error
        pos = tableBeginRegExp.indexIn(htmlData, pos);      // position begin table body

        if (pos != -1) {
            while ((pos = rowBeginRegExp.indexIn(htmlData, pos)) != -1) {
                posEndRow = rowEndRegExp.indexIn(htmlData, pos);

                if (posEndRow == -1)
                    continue;

                while ((pos = colRegExp.indexIn(htmlData, pos)) != -1) {
                    pos += colRegExp.matchedLength();

                    if (pos > posEndRow) {
                        pos = posEndRow;
                        obstracles << obstracle;
                        obstracle.clear();
                        break;
                    }
                    obstracle.append(colRegExp.cap(1));
                }
                //  add table last row
                if (!obstracle.isEmpty())
                    obstracles << obstracle;
            }
            emit parseObstraclesCompleted(obstracles);
        }
    }
    emit finished();
    return;
}

void HtmlParser::setData(QByteArray &data)
{
    htmlData = data;
}

void HtmlParser::stop()
{
    return;
}
