#include "IImagePluginWrapper.h"
#include "ISentencePluginWrapper.h"
#include "Media.h"
#include "SentencePluginWrapper.h"
#include "AudioPluginWrapper.h"
#include "mainwindow.h"
#include "ImagePluginWrapper.h"
#include <QApplication>
#include <iostream>
#include <memory>
#include "SentencesWidget.hpp"
#include "AudiosWidget.hpp"
#include "ImagesWidget.hpp"
#include "ServerConnection.h"
#include "Card.h"
#include "spdlog/spdlog.h"
#include "Player.hpp"
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <qmediacontent.h>
#include <qmediaplayer.h>
#include <thread>
#include <chrono>
#include <thread>
#include <QBuffer>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow wgt;
    wgt.show();
    app.exec();
    return 0;
}
