#ifndef DOWNLOADER_HPP
#define DOWNLOADER_HPP

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <qimage.h>
#include <qnetworkaccessmanager.h>
#include <qobjectdefs.h>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);

public slots:
    void download(const QUrl &url);

signals:
    void done(const QUrl &url, const QByteArray&);
    void error();

private slots:
    void slotFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // DOWNLOADER_HPP
