#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <IRequestable.h>
#include "deck_model.h"
#include "Card.h"
#include "AudioWidget.hpp"
#include "ImagesWidget.hpp"
#include "ExamplesWidget.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchReturned();
    void updateCardFields();
    void updateWord(const Card*);
    void updateDefinition(const Card*);
    void updateTags(const Card*);
    void updateExamples(const Card*);
    void updateAudio(const Card*);
    void updateImages(const Card*);
    void setCurrentIndex(QModelIndex);
    void onNextClicked();
    void onPrevClicked();
    void onAddClicked();
    void save();

private:
    Ui::MainWindow *ui;
    DeckModel* deckModel;
    std::vector<Card> savedDeck;
    std::shared_ptr<IRequestable> connection;
    ExamplesWidget* examplesWidget;
    AudioWidget* audioWidget;
    IMagesWidget* imagesWidget;
    QModelIndex current_index;
};
#endif // MAINWINDOW_H
