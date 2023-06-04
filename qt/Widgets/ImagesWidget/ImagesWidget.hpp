#ifndef IMAGESWIDGET_HPP
#define IMAGESWIDGET_HPP

#include <QWidget>
#include <QGridLayout>
#include <qgridlayout.h>
#include "Card.h"
#include "IImagePluginWrapper.h"
#include "ImagePluginWrapper.h"
#include "Downloader.hpp"

namespace Ui {
class ImagesWidget;
}

class ImagesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImagesWidget(std::unique_ptr<IImagePluginWrapper> imagePlugin,
                        QWidget *parent = nullptr);
    ~ImagesWidget();

public slots:
    void addImage(SourceWithAdditionalInfo audio, bool isLocal, bool isChosen = false);
    void load(int batch_size = 10);
    void clear();
    void set(std::string word, Media audios, 
            std::pair<std::vector<bool>, std::vector<bool>> chosen_mask = 
            std::pair<std::vector<bool>, std::vector<bool>>());
    Media getImages();
    std::pair<std::vector<bool>, std::vector<bool>> getMask();
    Media getChosenImages();

private:
    Ui::ImagesWidget *ui;
    QGridLayout *gridLayout;
    Downloader *downloader;
    const int picsInRow = 3;
    std::string currentWord;
    std::unique_ptr<IImagePluginWrapper> imagePlugin_;
};

#endif // IMAGESWIDGET_HPP
