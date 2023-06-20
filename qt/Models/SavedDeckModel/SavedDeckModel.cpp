#include "SavedDeckModel.hpp"
#include <qabstractitemmodel.h>
#include <qnamespace.h>
#include <qvariant.h>

SavedDeckModel::SavedDeckModel(std::vector<Card> cards, QObject *parent)
    : QAbstractTableModel(parent)
{
    for (const Card& card: cards) {
        SavedCard savedCard;
        savedCard.card = card;
        savedCard.sentencesMask = std::vector<bool>(card.examples.size(), true);

        std::vector<bool> localAudiosMask = std::vector<bool>(card.audios.local.size(), true);
        std::vector<bool> webAudiosMask = std::vector<bool>(card.audios.web.size(), true);
        savedCard.audiosMask = std::make_pair(localAudiosMask, webAudiosMask);

        std::vector<bool> localImagesMask = std::vector<bool>(card.images.local.size(), true);
        std::vector<bool> webImagesMask = std::vector<bool>(card.images.web.size(), true);
        savedCard.imagesMask= std::make_pair(localImagesMask, webImagesMask);

        cards_.push_back(savedCard);
    }
}

QVariant SavedDeckModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return "word";
            case 1:
                return "definition";
            default:
            return QVariant();
        }
    }
    return QVariant();
}

int SavedDeckModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return cards_.size();
}

int SavedDeckModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant SavedDeckModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (!hasIndex(index.row(), index.column())) {
        return QVariant();
    }
    switch (role) {
        case Qt::DisplayRole:
            if (index.column() == 0) {
                return QString::fromStdString(cards_.at(index.row()).card.word);
            }
            if (index.column() == 1) {
                return QString::fromStdString(cards_.at(index.row()).card.definition);
            }
            return QVariant();
        default:
        return QVariant();
    }
    return QVariant();
}

bool SavedDeckModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    cards_.erase(cards_.begin() + row, cards_.begin() + row + count);
    endRemoveRows();
    return true;
}

QModelIndex SavedDeckModel::next(const QModelIndex& index) {
    if (!hasIndex(index.row() + 1, index.column())) {
        return index;
    }
    return createIndex(index.row() + 1, index.column());
}

QModelIndex SavedDeckModel::prev(const QModelIndex& index) {
    if (!hasIndex(index.row() - 1, index.column())) {
        return index;
    }
    return createIndex(index.row() - 1, index.column());
}
