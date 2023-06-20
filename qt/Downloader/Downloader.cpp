#include "Downloader.hpp"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

Downloader::Downloader(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager;
    connect(manager, SIGNAL(finished(QNetworkReply*)), 
            this, SLOT(slotFinished(QNetworkReply*)));
}

void Downloader::download(const QUrl &url) {
    QNetworkRequest request(url);
    manager->get(request);
}

void Downloader::slotFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        emit error();
    }
    else {
        emit done(reply->url(), reply->readAll());
    }
    reply->deleteLater();
}
