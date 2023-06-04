#ifndef AUDIOSWIDGET_HPP
#define AUDIOSWIDGET_HPP

#include <QWidget>
#include <QGridLayout>
#include <memory>
#include <qgridlayout.h>
#include <QMediaPlayer>
#include <qmediaplayer.h>
#include "Card.h"
#include "IAudioPluginWrapper.h"
#include "Media.h"

namespace Ui {
class AudiosWidget;
}

class AudiosWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudiosWidget(std::unique_ptr<IAudioPluginWrapper> audioPlugin,
                        QWidget *parent = nullptr);
    ~AudiosWidget();

public slots:
    void addAudio(SourceWithAdditionalInfo audio, bool isLocal, bool isChosen = false);
    void load(int batch_size = 5);
    void clear();
    void set(std::string word, Media audios, 
            std::pair<std::vector<bool>, std::vector<bool>> chosen_mask = 
            std::pair<std::vector<bool>, std::vector<bool>>());
    Media getAudios();
    std::pair<std::vector<bool>, std::vector<bool>> getMask();
    Media getChosenAudio();

private:
    Ui::AudiosWidget *ui;
    QGridLayout *gridLayout;
    QMediaPlayer *mediaPlayer;
    std::string currentWord;
    std::unique_ptr<IAudioPluginWrapper> audioPlugin_;
};

#endif // AUDIOSWIDGET_HPP
