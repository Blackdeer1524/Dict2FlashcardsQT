#include "deck_model.h"
#include <QString>
#include <iostream>

DeckModel::DeckModel(std::unique_ptr<IDeck> deck, QObject *parent)
    : QAbstractListModel(parent)
{
    deck_ = std::move(deck);
}

int DeckModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return deck_->size();
}

QVariant DeckModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
        return QString::fromStdString(deck_->getWord(index.row()));
    case CardRole:
    {
        Card* card = const_cast<Card*>(deck_->getCard(index.row()));
        void* card_void = static_cast<void*>(card);
        return QVariant::fromValue(card_void);
    }
    default:
        return QVariant();
    }
    return QVariant();
}

int DeckModel::indexOfWord(const QString &word)
{
    return deck_->indexOfWord(word.toStdString());
}

QModelIndex DeckModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || row >= rowCount() || column != 0)
    {
        return QModelIndex();
    }
    return createIndex(row, 0);
}

void DeckModel::load(const QString &word, QString query)
{
    beginResetModel();
    deck_->load(word.toStdString(), query.toStdString());
    endResetModel();
}

void DeckModel::next(const QModelIndex &index)
{
    deck_->next(index.row());
}

void DeckModel::prev(const QModelIndex &index)
{
    deck_->prev(index.row());
}
