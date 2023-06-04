#include "ImagesWidget.hpp"
#include "Downloader.hpp"
#include "ui_ImagesWidget.h"
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtConcurrent>
#include <qicon.h>
#include <qimage.h>
#include <qlayoutitem.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>
#include <qurl.h>

ImagesWidget::ImagesWidget(std::unique_ptr<IImagePluginWrapper> imagePlugin, 
                            QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagesWidget)
{
    ui->setupUi(this);
    imagePlugin_ = std::move(imagePlugin);
    imagePlugin_->init("images");
    gridLayout = new QGridLayout;
    QWidget *imagesListWidget = new QWidget;
    imagesListWidget->setLayout(gridLayout);
    ui->scrollArea->setWidget(imagesListWidget);
}

ImagesWidget::~ImagesWidget()
{
    delete ui;
}

void ImagesWidget::addImage(SourceWithAdditionalInfo image, bool isLocal, bool isChosen) {
    QPushButton *pushButton = new QPushButton;

    pushButton->setCheckable(true);
    pushButton->setStyleSheet("QPushButton:checked { background-color: green; }");
    pushButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pushButton->setProperty("isLocal", isLocal);
    pushButton->setProperty("src", QString::fromStdString(image.src));
    pushButton->setProperty("info", QString::fromStdString(image.info));

    if (isChosen) {
        pushButton->setChecked(true);
    }

    int new_row = gridLayout->count() / picsInRow;
    int new_col = gridLayout->count() % picsInRow;

    gridLayout->addWidget(pushButton, new_row, new_col);

    if (isLocal) {
    auto pixmap = QPixmap(QString::fromStdString(image.src));
    pushButton->setIcon(pixmap);
    }
    else if (QUrl(QString::fromStdString(image.src)).isValid()) {
        downloader = new Downloader(this);
        connect(downloader, &Downloader::done, [=](const QUrl &url, const QByteArray &bytes) {
            QPixmap pixmap;
            pixmap.loadFromData(bytes);
            QIcon icon(pixmap);
            pushButton->setIcon(icon);
            pushButton->setIconSize(QSize(150, 150));
        });
        downloader->download(QUrl(QString::fromStdString(image.src)));
    }
}

void ImagesWidget::load(int batch_size) {
    currentWord = ui->loadImageLine->text().toStdString();
    if (currentWord.empty()) {
        return;
    }
    auto [images, error] = imagePlugin_->get(currentWord, batch_size, false);

    for (SourceWithAdditionalInfo localImage: images.local) {
        addImage(localImage, true);
    }
    for (SourceWithAdditionalInfo webImage: images.web) {
        addImage(webImage, false);
    }
}

void ImagesWidget::clear() {
    QLayoutItem *item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        QWidget *widget = item->widget();
        gridLayout->removeItem(item);
        widget->deleteLater();
        delete item;
    }
}

void ImagesWidget::set(std::string word, Media images, 
                    std::pair<std::vector<bool>, std::vector<bool>> chosen_mask) {
    clear();
    currentWord = word;
    ui->loadImageLine->setText(QString::fromStdString(currentWord));
    for (int i = 0; i < images.local.size(); ++i) {
        bool chosen = i < chosen_mask.first.size() ? chosen_mask.first.at(i) : false;
        addImage(images.local.at(i), true, chosen);
    }
    for (int i = 0; i < images.web.size(); ++i) {
        bool chosen = i < chosen_mask.second.size() ? chosen_mask.second.at(i) : false;
        addImage(images.web.at(i), false, chosen);
    }
}

Media ImagesWidget::getImages() {
    Media images;
    for (int i = 0; i < gridLayout->count(); ++i) {
        QLayoutItem *pushButtonItem = gridLayout->itemAt(i);
        if (!pushButtonItem) {
            continue;
        }
        QPushButton *pushButton = qobject_cast<QPushButton*>(pushButtonItem->widget());
        if (!pushButton) {
            continue;
        }
        SourceWithAdditionalInfo image;
        bool isLocal = pushButton->property("isLocal").toBool();
        image.src = pushButton->property("src").toString().toStdString();
        image.info = pushButton->property("info").toString().toStdString();

        if (isLocal) {
            images.local.push_back(image);
        }
        else {
            images.web.push_back(image);
        }
    }
    return images;
}

std::pair<std::vector<bool>, std::vector<bool>> ImagesWidget::getMask() {
    std::pair<std::vector<bool>, std::vector<bool>>  imagesMask;
    for (int i = 0; i < gridLayout->count(); ++i) {
        QLayoutItem *pushButtonItem = gridLayout->itemAt(i);
        if (!pushButtonItem) {
            continue;
        }
        QPushButton *pushButton = qobject_cast<QPushButton*>(pushButtonItem->widget());
        if (!pushButton) {
            continue;
        }
        if (pushButton->property("isLocal").toBool()) {
            imagesMask.first.push_back(pushButton->isChecked());
        }
        else {
            imagesMask.second.push_back(pushButton->isChecked());
        }
    }
    return imagesMask;
}

Media ImagesWidget::getChosenImages() {
    Media chosenImages;
    for (int i = 0; i < gridLayout->count(); ++i) {
        QLayoutItem *pushButtonItem = gridLayout->itemAt(i);
        if (!pushButtonItem) {
            continue;
        }
        QPushButton *pushButton = qobject_cast<QPushButton*>(pushButtonItem->widget());
        if (!pushButton) {
            continue;
        }
        if (!pushButton->isChecked()) {
            continue;
        }
        SourceWithAdditionalInfo image;
        bool isLocal = pushButton->property("isLocal").toBool();
        image.src = pushButton->property("src").toString().toStdString();
        image.info = pushButton->property("info").toString().toStdString();

        if (isLocal) {
            chosenImages.local.push_back(image);
        }
        else {
            chosenImages.web.push_back(image);
        }
    }
    return chosenImages;
}
