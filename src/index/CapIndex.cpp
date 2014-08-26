/********************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2013 Sascha Ludwig Häusler
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy of
 ** this software and associated documentation files (the "Software"), to deal in
 ** the Software without restriction, including without limitation the rights to
 ** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 ** the Software, and to permit persons to whom the Software is furnished to do so,
 ** subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 ** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 ** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 ** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 ** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *********************************************************************************/

#include "CapIndex.h"
#include "private/AbstractIndex_p.h"

#include <QtCore/QEventLoop>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <QtNetwork/QNetworkReply>

namespace arangodb
{
namespace index
{

class CapIndexPrivate : public AbstractIndexPrivate
{
    public:
        int size;
};

const QString CAP_INDEX_NAME = QStringLiteral("cap");

CapIndex::CapIndex(Collection * collection, QObject *parent) :
    AbstractIndex(collection, new CapIndexPrivate, parent)
{
}

void CapIndex::setSize(int size)
{
    Q_D(CapIndex);
    d->size = size;
}

int CapIndex::size() const
{
    Q_D(const CapIndex);
    return d->size;
}

QString CapIndex::name() const
{
    return CAP_INDEX_NAME;
}

QByteArray CapIndex::toJson() const
{
    QJsonDocument doc;
    QJsonObject obj;

    obj.insert(QLatin1String("type"), name());
    obj.insert(QLatin1String("size"), QJsonValue(size()));

    doc.setObject(obj);
    return doc.toJson();
}

}
}