#include "Player.hpp"
#include "ui_Player.h"
#include <qdatetime.h>
#include <qglobal.h>
#include <qmediaplayer.h>
#include <qnetworkreply.h>
#include <qurl.h>
#include <QStyle>
#include <QTimer>
#include <string>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <sys/socket.h>

Player::Player(QMediaPlayer *audioPlayer, QWidget *parent) :
    QWidget(parent),
    audioPlayer(audioPlayer),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    if (!audioPlayer) {
        qDebug() << "Svoi";
        audioPlayer = new QMediaPlayer(this);
    }
    ui->playButton->setEnabled(false);
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(audioPlayer, &QMediaPlayer::stateChanged, 
            this, &Player::onStateChanged);
    connect(audioPlayer, &QMediaPlayer::mediaStatusChanged, 
            this, &Player::onStatusChanged);
    ui->infoEdit->setEnabled(false);
    ui->infoEdit->setStyleSheet("color: black; background-color: #F0F0F0; border: 1px solid gray;");
}

Player::~Player()
{
    delete ui;
}

void Player::set(SourceWithAdditionalInfo audio, bool local) {
    QString info = QString::fromStdString(audio.info);
    local_ = local;
    if (local_) {
        url = QUrl::fromLocalFile(QString::fromStdString(audio.src));
        return;
    }
    url = QUrl(QString::fromStdString(audio.src));
    ui->infoEdit->setText(QString::fromStdString(audio.info));
    ui->playButton->setEnabled(true);
    ui->infoEdit->setEnabled(false);
    ui->infoEdit->setStyleSheet("color: black; background-color: #F0F0F0; border: 1px solid gray;");
}

void Player::onStateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::StoppedState) {
        ui->playButton->setEnabled(true);
    }
}

void Player::onStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::LoadedMedia) {
        ui->playButton->setEnabled(true);
    }
}

void Player::onPlayClicked() {
    audioPlayer->setMedia(url);
    audioPlayer->play();
    ui->playButton->setEnabled(false);
}

bool Player::isLocal() const {
    return local_;
}

QString Player::getSrc() const {
    return url.toString();
}

QString Player::getInfo() const {
    return info;
}
