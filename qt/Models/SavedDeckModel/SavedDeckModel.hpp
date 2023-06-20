#ifndef SAVEDDECKMODEL_H
#define SAVEDDECKMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "Card.h"

class SavedDeckModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    struct SavedCard {
        Card card;
        std::vector<bool> sentencesMask;
        std::pair<std::vector<bool>, std::vector<bool>> audiosMask;
        std::pair<std::vector<bool>, std::vector<bool>> imagesMask;
    };

    explicit SavedDeckModel(std::vector<Card> cards, QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

public slots:
    QModelIndex next(const QModelIndex& index);
    QModelIndex prev(const QModelIndex& index);

private:
    std::vector<SavedCard> cards_;
};

#endif // SAVEDDECKMODEL_H
