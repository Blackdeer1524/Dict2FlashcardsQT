#ifndef SENTENCESWIDGET_HPP
#define SENTENCESWIDGET_HPP

#include <QWidget>
#include <QGridLayout>
#include <memory>
#include "ISentencePluginWrapper.h"

namespace Ui {
class SentencesWidget;
}

class SentencesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SentencesWidget(std::unique_ptr<ISentencePluginWrapper> sentencePlugin,
                            QWidget *parent = nullptr);
    ~SentencesWidget();

public slots:
    void addSentence(QString sentence = QString(), bool is_chosen = false);
    void load(int batch_size = 5);
    void clear();
    void set(std::string word, std::vector<std::string> sentences, 
            std::vector<bool> chosen_mask = std::vector<bool>()
            );
    std::vector<std::string> getSentences();
    std::vector<bool> getMask();
    std::vector<std::string> getChosenSentences();

private:
    Ui::SentencesWidget *ui;
    QGridLayout *gridLayout;
    std::string currentWord;
    std::unique_ptr<ISentencePluginWrapper> sentencePlugin_;
};

#endif // SENTENCESWIDGET_HPP
