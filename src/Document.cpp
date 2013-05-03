#include "Document.h"
#include <QtDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkReply>

using namespace arangodb;

namespace internal {

class DocumentPrivate
{
    public:
        const QString ID  = QStringLiteral("_id");
        const QString KEY = QStringLiteral("_key");
        const QString REV = QStringLiteral("_rev");

        bool isReady = false;
        bool isDirty = false;
        bool isCreated = false;
        QJsonObject data;
        QString collectionName;

        QString errorMessage;
        quint32 errorCode = 0;
        quint32 errorNumber = 0;

        QStringList dirtyAttributes;
};

}

Document::Document(QObject *parent) :
    QObject(parent),
    d(new internal::DocumentPrivate)
{
}

Document::Document(QString collection, QObject *parent) :
    Document(parent)
{
    d->collectionName = collection;
}

Document::~Document()
{
    delete d;
}

bool Document::isReady()
{
    return d->isReady;
}

bool Document::isCreated()
{
    return d->isCreated;
}

QByteArray Document::toJsonString() const
{
    QJsonDocument doc;

    if ( isEveryAttributeDirty() ) {
            QJsonObject obj;

            obj.insert(d->ID, obj.value(d->ID));
            obj.insert(d->KEY, obj.value(d->KEY));
            obj.insert(d->REV, obj.value(d->REV));

            for( QString attribute : d->dirtyAttributes ) {
                    obj.insert(attribute, d->data[attribute]);
                }

            doc.setObject(obj);
        }
    else {
            doc.setObject(d->data);
        }

    return doc.toJson();
}

QString Document::docID() const
{
    return d->data.value(d->ID).toString();
}

QString Document::key() const
{
    return d->data.value(d->KEY).toString();
}

QString Document::rev() const
{
    return d->data.value(d->REV).toString();
}

QString Document::collection() const
{
    return d->collectionName;
}

QString Document::errorMessage() const
{
    return d->errorMessage;
}

quint32 Document::errorCode()
{
    return d->errorCode;
}

quint32 Document::errorNumber()
{
    return d->errorNumber;
}

bool Document::hasErrorOccurred()
{
    return d->errorCode != 0;
}

void Document::set(const QString &key, QVariant data)
{
    d->dirtyAttributes.append(key);
    d->data.insert(key, QJsonValue::fromVariant(data));
    d->isDirty = true;
}

QVariant Document::get(const QString &key) const
{
    return d->data.value(key).toVariant();
}

QStringList Document::dirtyAttributes() const
{
    return d->dirtyAttributes;
}

bool Document::isEveryAttributeDirty() const
{
    int attributes = d->data.keys().size()-3;

    if ( d->data.contains("error") ) {
            attributes--;
        }

    return d->dirtyAttributes.size() >= attributes;
}

void Document::_ar_dataIsAvailable()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    d->data = QJsonDocument::fromJson(reply->readAll()).object();

    bool hasError = d->data.value("error").toBool();
    if ( hasError ) {
            d->errorMessage = d->data.value("errorMessage").toString();
            d->errorNumber  = d->data.value("errorNum").toVariant().toInt();
            d->errorCode    = d->data.value("code").toVariant().toInt();
            emit error();
        }
    else {
            d->isReady = true;
            d->isCreated = true;
            emit ready();
        }
}

void Document::_ar_dataDeleted()
{
    emit dataDeleted();
}

void Document::save()
{
    if ( !d->isCreated || d->isDirty ) {
            d->isDirty = false;
            d->dirtyAttributes.clear();

            emit saveData(this);
        }
}

void Document::drop()
{
    if ( d->isCreated ) {
            d->isDirty = false;
            d->isCreated = false;

            emit deleteData(this);
        }
}
