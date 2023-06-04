#include "mainwindow.h"
#include "AudioPluginWrapper.h"
#include "AudiosWidget.hpp"
#include "Card.h"
#include "IAudioPluginWrapper.h"
#include "IImagePluginWrapper.h"
#include "IRequestable.h"
#include "ISentencePluginWrapper.h"
#include "IWordPluginWrapper.h"
#include "ImagePluginWrapper.h"
#include "ImagesWidget.hpp"
#include "ISentencePluginWrapper.h"
#include "SentencePluginWrapper.h"
#include "SentencesWidget.hpp"
#include "ui_mainwindow.h"
#include "deck_model.h"
#include "Deck.hpp"
#include "WordPluginWrapper.h"
#include "FormatProcessorPluginWrapper.h"
#include "ServerConnection.h"
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <QBoxLayout>
#include <iostream>
#include <iostream>
#include <qcoreevent.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connection = std::make_shared<ServerConnection>(8888);
    std::unique_ptr<IWordPluginWrapper> wordPlugin = std::make_unique<WordPluginWrapper>(connection);
    std::unique_ptr<IDeck> deck = std::make_unique<Deck>(std::move(wordPlugin));
    deckModel = new DeckModel(std::move(deck), this);
    ui->deckView->setModel(deckModel);

    std::unique_ptr<ISentencePluginWrapper> sentencePlugin = std::make_unique<SentencePluginWrapper>(connection);
    sentencesWidget = new SentencesWidget(std::move(sentencePlugin));
    ui->examplesTab->layout()->addWidget(sentencesWidget);

    std::unique_ptr<IAudioPluginWrapper> audioPlugin = std::make_unique<AudioPluginWrapper>(connection);
    audiosWidget = new AudiosWidget(std::move(audioPlugin));
    ui->audioTab->layout()->addWidget(audiosWidget);

    std::unique_ptr<IImagePluginWrapper> imagePlugin = std::make_unique<ImagePluginWrapper>(connection);
    imagesWidget = new ImagesWidget(std::move(imagePlugin));
    ui->imagesTab->layout()->addWidget(imagesWidget);
    
    connect(ui->searchLine, SIGNAL(returnPressed()), this, SLOT(onSearchReturned()));
    connect(ui->deckView, SIGNAL(clicked(QModelIndex)), this, SLOT(setCurrentIndex(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSearchReturned()
{
    QString word = ui->searchLine->text();
    if (word == "")
    {
        return;
    }
    int int_idx = deckModel->indexOfWord(word);
    if (int_idx == -1)
    {
        deckModel->load(ui->searchLine->text(), ui->filterEdit->toPlainText());
        int rows = deckModel->rowCount() - 1;
        QModelIndex qm_idx = deckModel->index(deckModel->rowCount() - 1);
        ui->deckView->setCurrentIndex(qm_idx);
        ui->deckView->clicked(qm_idx);
        return;
    }
    QModelIndex qm_idx = deckModel->index(int_idx);
    ui->deckView->setCurrentIndex(qm_idx);
    ui->deckView->clicked(qm_idx);
}

void MainWindow::updateCardFields()
{
    ui->tabWidget->setCurrentIndex(0);
    QVariant qvar = deckModel->data(current_index, DeckModel::CardRole);
    void* void_card = qvar.value<void*>();
    const Card* card = static_cast<const Card*>(void_card);
    updateWord(card);
    updateDefinition(card);
    updateExamples(card);
    updateAudio(card);
    updateImages(card);
    updateTags(card);
}

void MainWindow::updateWord(const Card *card)
{
    if (!card)
    {
        return;
    }
    ui->wordLine->setText(QString::fromStdString(card->word));
}

void MainWindow::updateDefinition(const Card *card)
{
    if (!card)
    {
        return;
    }
    ui->definitionEdit->setText(QString::fromStdString(card->definition));
}

void MainWindow::updateTags(const Card *card)
{
    if (!card) {
        return;
    }
    currentTags = card->tags;
    ui->tagsLine->setText(QString::fromStdString(parse_tags(card->tags)));
}

void MainWindow::updateExamples(const Card *card)
{
    if (!card) {
        return;
    }
    sentencesWidget->set(card->word, card->examples);
}

void MainWindow::updateAudio(const Card *card)
{
    if (!card) {
        return;
    }
    audiosWidget->set(card->word, card->audios);
}

void MainWindow::updateImages(const Card *card)
{
    if (!card) {
        return;
    }
    imagesWidget->set(card->word, card->images);
}

void MainWindow::setCurrentIndex(QModelIndex index)
{
    current_index = index;
    updateCardFields();
}

void MainWindow::onNextClicked()
{
    if (!current_index.isValid()) {
        return;
    }
    deckModel->next(current_index);
    updateCardFields();
}

void MainWindow::onPrevClicked()
{
    if (!current_index.isValid()) {
        return;
    }
    deckModel->prev(current_index);
    updateCardFields();
}

void MainWindow::onAddClicked() {
    Card card;
    card.word = ui->wordLine->text().toStdString();
    card.tags = currentTags;
    card.definition = ui->definitionEdit->toPlainText().toStdString();
    card.examples = sentencesWidget->getChosenSentences();
    card.audios = audiosWidget->getChosenAudio();
    card.images = imagesWidget->getChosenImages();
    savedDeck.push_back(card);
    onNextClicked();
}

void MainWindow::save() {
    std::string relative_path = "./savedDeck.json";
    auto absolute_path = std::filesystem::absolute(relative_path).string();
    save_cards(savedDeck, absolute_path);
    FormatProcessorPluginWrapper savingPlugin(connection);
    savingPlugin.init("processor");
    savingPlugin.save(absolute_path);
    exit(0);
}
