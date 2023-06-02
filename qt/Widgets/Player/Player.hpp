#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QWidget>
#include <QMediaPlayer>
#include <qmediaplayer.h>
#include <qnetworkreply.h>
#include <qurl.h>
#include "Card.h"
#include "Media.h"

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QMediaPlayer *audioPlayer, QWidget *parent = nullptr);
    ~Player();
    void download(QUrl url);

public slots:
    void set(SourceWithAdditionalInfo audio, bool isLocal);
    void onStateChanged(QMediaPlayer::State state);
    void onStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlayClicked();

private:
    Ui::Player *ui;
    QMediaPlayer *audioPlayer;
    QUrl url;
};

#endif // PLAYER_HPP
