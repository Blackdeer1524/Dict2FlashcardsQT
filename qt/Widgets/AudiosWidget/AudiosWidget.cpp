#include "AudiosWidget.hpp"
#include "Media.h"
#include "ui_AudiosWidget.h"
#include <QPushButton>
#include <qmediaplayer.h>
#include "Player.hpp"


AudiosWidget::AudiosWidget(std::unique_ptr<IAudioPluginWrapper> audioPlugin,
                        QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudiosWidget)
{
    ui->setupUi(this);
    audioPlugin_ = std::move(audioPlugin);
    audioPlugin_->init("audios");
    gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignTop);
    QWidget *audiosListWidget = new QWidget;
    audiosListWidget->setLayout(gridLayout);
    ui->scrollArea->setWidget(audiosListWidget);
    mediaPlayer = new QMediaPlayer(this);
}

AudiosWidget::~AudiosWidget()
{
    delete ui;
}

void AudiosWidget::addAudio(SourceWithAdditionalInfo audio, bool isLocal, bool isChosen) {
    QPushButton *pushButton = new QPushButton;
    pushButton->setCheckable(true);
    pushButton->setStyleSheet("QPushButton:checked { background-color: green; }");
    pushButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    Player *audioPlayer = new Player(mediaPlayer);
    audioPlayer->set(audio, isLocal);

    if (isChosen) {
        pushButton->setChecked(true);
    }
    
    audioPlayer->setMaximumHeight(100);
    pushButton->setMaximumHeight(40);
    pushButton->setMaximumWidth(40);

    int new_row = gridLayout->count() / gridLayout->columnCount();

    gridLayout->addWidget(pushButton, new_row, 0);
    gridLayout->addWidget(audioPlayer, new_row, 1);
}

void AudiosWidget::load(int batch_size) {
    currentWord = ui->loadAudioLine->text().toStdString();
    if (currentWord.empty()) {
        return;
    }
    auto [audios, error] = audioPlugin_->get(currentWord, batch_size, false);

    for (SourceWithAdditionalInfo localAudio: audios.local) {
        addAudio(localAudio, true);
    }
    for (SourceWithAdditionalInfo webAudio: audios.web) {
        addAudio(webAudio, false);
    }
}

void AudiosWidget::clear() {
    QLayoutItem *item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        QWidget *widget = item->widget();
        gridLayout->removeItem(item);
        widget->deleteLater();
        delete item;
    }
}

void AudiosWidget::set(std::string word, Media audios, 
                        std::pair<std::vector<bool>, std::vector<bool>> chosen_mask) {
    clear();
    currentWord = word;
    ui->loadAudioLine->setText(QString::fromStdString(currentWord));
    for (int i = 0; i < audios.local.size(); ++i) {
        bool chosen = i < chosen_mask.first.size() ? chosen_mask.first.at(i) : false;
        addAudio(audios.local.at(i), true, chosen);
    }
    for (int i = 0; i < audios.web.size(); ++i) {
        bool chosen = i < chosen_mask.second.size() ? chosen_mask.second.at(i) : false;
        addAudio(audios.web.at(i), false, chosen);
    }
}

Media AudiosWidget::getAudios() {
    Media audios;
    for (int row = 0; row < gridLayout->count() / gridLayout->columnCount(); ++row) {
        QLayoutItem* audioPlayerItem = gridLayout->itemAtPosition(row, 1);
        if (!audioPlayerItem) {
            continue;
        }
        Player *audioPlayer = qobject_cast<Player*>(audioPlayerItem->widget());
        if (!audioPlayer) {
            continue;
        }
        SourceWithAdditionalInfo audio;
        audio.info = audioPlayer->getInfo().toStdString();
        audio.src = audioPlayer->getSrc().toStdString();
        if (audioPlayer->isLocal()) {
            audios.local.push_back(audio);
        }
        else {
            audios.web.push_back(audio);
        }
    }
    return audios;
}

std::pair<std::vector<bool>, std::vector<bool>> AudiosWidget::getMask() {
    std::pair<std::vector<bool>, std::vector<bool>>  audiosMask;
    for (int row = 0; row < gridLayout->count() / gridLayout->columnCount(); ++row) {
        QLayoutItem* pushButtonItem = gridLayout->itemAtPosition(row, 0);
        if (!pushButtonItem) {
            continue;
        }

        QPushButton* pushButton = qobject_cast<QPushButton*>(pushButtonItem->widget());
        if (!pushButton) {
            continue;
        }

        QLayoutItem* audioPlayerItem = gridLayout->itemAtPosition(row, 1);
        if (!audioPlayerItem) {
            continue;
        }
        Player *audioPlayer = qobject_cast<Player*>(audioPlayerItem->widget());
        if (!audioPlayer) {
            continue;
        }

        if (audioPlayer->isLocal()) {
            audiosMask.first.push_back(pushButton->isChecked());
        }
        else {
            audiosMask.second.push_back(pushButton->isChecked());
        }
    }
    return audiosMask;
}

Media AudiosWidget::getChosenAudio() {
    Media chosenAudios;
    for (int row = 0; row < gridLayout->count() / gridLayout->columnCount(); ++row) {
        QLayoutItem* pushButtonItem = gridLayout->itemAtPosition(row, 0);
        if (!pushButtonItem) {
            continue;
        }

        QPushButton* pushButton = qobject_cast<QPushButton*>(pushButtonItem->widget());
        if (!pushButton) {
            continue;
        }

        if (!pushButton->isChecked()) {
            continue;
        }

        QLayoutItem* audioPlayerItem = gridLayout->itemAtPosition(row, 1);
        if (!audioPlayerItem) {
            continue;
        }

        Player *audioPlayer = qobject_cast<Player*>(audioPlayerItem->widget());
        if (!audioPlayer) {
            continue;
        }

        SourceWithAdditionalInfo audio;
        audio.info = audioPlayer->getInfo().toStdString();
        audio.src = audioPlayer->getSrc().toStdString();
        if (audioPlayer->isLocal()) {
            chosenAudios.local.push_back(audio);
        }
        else {
            chosenAudios.web.push_back(audio);
        }
    }
    return chosenAudios;
}
