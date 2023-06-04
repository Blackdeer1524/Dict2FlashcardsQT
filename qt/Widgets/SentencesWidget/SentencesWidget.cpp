#include "SentencesWidget.hpp"
#include "spdlog/fmt/bundled/format.h"
#include "spdlog/spdlog.h"
#include "ui_SentencesWidget.h"
#include <QTextEdit>
#include <QPushButton>
#include <QLayoutItem>
#include <qdebug.h>
#include <qlayoutitem.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>
#include <qtextedit.h>
#include <qwidget.h>
#include <qwindowdefs.h>
#include <QDebug>
#include <string>

SentencesWidget::SentencesWidget(std::unique_ptr<ISentencePluginWrapper> sentencePlugin,
                            QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SentencesWidget)
{
    ui->setupUi(this);
    sentencePlugin_ = std::move(sentencePlugin);
    sentencePlugin_->init("sentences");
    gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignTop);
    QWidget *sentencesListWidget = new QWidget;
    sentencesListWidget->setObjectName("ListWidget");
    sentencesListWidget->setLayout(gridLayout);
    ui->scrollArea->setWidget(sentencesListWidget);
}

SentencesWidget::~SentencesWidget()
{
    delete ui;
}

void SentencesWidget::addSentence(QString sentence, bool is_chosen)
{
    SPDLOG_INFO("Adding sentence with text: {}", sentence.toStdString());
    
    QTextEdit *textEdit = new QTextEdit;
    textEdit->setPlaceholderText("Sentence");
    textEdit->setText(sentence);

    QPushButton *pushButton = new QPushButton;
    pushButton->setCheckable(true);
    pushButton->setStyleSheet("QPushButton:checked { background-color: green; }");
    pushButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    if (is_chosen) {
        pushButton->setChecked(true);
    }
    
    textEdit->setMaximumHeight(100);
    pushButton->setMaximumHeight(100);

    int new_row = gridLayout->count() / gridLayout->columnCount();

    gridLayout->addWidget(textEdit, new_row, 0);
    gridLayout->addWidget(pushButton, new_row, 1);

    SPDLOG_INFO("Sentence added. Row count =  {}", gridLayout->count() / gridLayout->columnCount());
}

void SentencesWidget::load(int batch_size) {
    SPDLOG_INFO("Start loading sentences");
    currentWord = ui->loadSentenceLine->text().toStdString();
    if (currentWord.empty()) {
        return;
    }
    auto [sentences, error] = sentencePlugin_->get(currentWord, batch_size, false);

    for (const std::string& sentence: sentences) {
        addSentence(QString::fromStdString(sentence));
    }

    SPDLOG_INFO("Loaded {} sentences", sentences.size());
    SPDLOG_INFO("Message from sentencePlugin: {}", error);
}

void SentencesWidget::clear() {
    SPDLOG_INFO("Clear start. Row count = {}", gridLayout->count() / gridLayout->columnCount());
    QLayoutItem *item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        QWidget *widget = item->widget();
        gridLayout->removeItem(item);
        widget->deleteLater();
        delete item;
    }
    SPDLOG_INFO("Clear end. Row count = {}", gridLayout->count() / gridLayout->columnCount());
}

void SentencesWidget::set(std::string word, std::vector<std::string> sentences, 
                            std::vector<bool> chosen_mask) {
    clear();
    currentWord = word;
    ui->loadSentenceLine->setText(QString::fromStdString(currentWord));
    for (int i = 0; i < sentences.size(); ++i) {
        bool chosen = i < chosen_mask.size() ? chosen_mask.at(i) : false;
        addSentence(QString::fromStdString(sentences.at(i)), chosen);
    }
}

std::vector<std::string> SentencesWidget::getSentences() {
    std::vector<std::string> sentences;
    for (int row = 0; row < gridLayout->count() / gridLayout->columnCount(); ++row) {
        QLayoutItem* textEditItem = gridLayout->itemAtPosition(row, 0);
        if (!textEditItem) {
            continue;
        }
        QTextEdit* textEdit = qobject_cast<QTextEdit*>(textEditItem->widget());
        if (!textEdit) {
            continue;
        }
        QString text = textEdit->toPlainText();
        if (!text.isEmpty()) {
            sentences.push_back(text.toStdString());
        }
    }
    return sentences;
}

std::vector<bool> SentencesWidget::getMask() {
    std::vector<bool> sentencesMask;
    for (int row = 0; row < gridLayout->count() / gridLayout->columnCount(); ++row) {

        QLayoutItem* textEditItem = gridLayout->itemAtPosition(row, 0);
        if (!textEditItem) {
            continue;
        }
        QTextEdit* textEdit = qobject_cast<QTextEdit*>(textEditItem->widget());
        if (!textEdit) {
            continue;
        }

        QString text = textEdit->toPlainText();
        if (text.isEmpty()) {
            continue;
        }

        QLayoutItem* pushButtonItem = gridLayout->itemAtPosition(row, 1);
        if (!pushButtonItem) {
            continue;
        }

        QPushButton* pushButton = qobject_cast<QPushButton*>(pushButtonItem->widget());
        if (!pushButton) {
            continue;
        }
        sentencesMask.push_back(pushButton->isChecked());
    }
    return sentencesMask;
}

std::vector<std::string> SentencesWidget::getChosenSentences() {
    std::vector<std::string> chosenSentences;
    for (int row = 0; row < gridLayout->count() / gridLayout->columnCount(); ++row) {

        QLayoutItem* textEditItem = gridLayout->itemAtPosition(row, 0);
        if (!textEditItem) {
            continue;
        }
        QTextEdit* textEdit = qobject_cast<QTextEdit*>(textEditItem->widget());
        if (!textEdit) {
            continue;
        }

        QString text = textEdit->toPlainText();
        if (text.isEmpty()) {
            continue;
        }

        QLayoutItem* pushButtonItem = gridLayout->itemAtPosition(row, 1);
        if (!pushButtonItem) {
            continue;
        }

        QPushButton* pushButton = qobject_cast<QPushButton*>(pushButtonItem->widget());
        if (!pushButton) {
            continue;
        }
        if (pushButton->isChecked()) {
            chosenSentences.push_back(text.toStdString());
        }
    }
    return chosenSentences;
}
