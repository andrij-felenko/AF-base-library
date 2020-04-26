#ifndef LIB_QUICK_AF_MENU_MODEL_H
#define LIB_QUICK_AF_MENU_MODEL_H

#include <QtCore/QAbstractListModel>

namespace AFquick {
    class MenuModel;

    struct MenuSingle {
        QString name;
        QString iconPath;
    };
}

class AFquick::MenuModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MenuModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash <int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void updateData();

public slots:
    void beginResetModelSlot();
    void endResetModelSlot();

signals:
    QString getNameByIndex(int index) const;
    QString getIconByIndex(int index) const;
    bool  isCurrentByIndex(int index) const;
    int getCount() const;

private:
    enum FieldName {
        IconPath = Qt::DisplayRole,
        Name = Qt::UserRole,
        IsCurrent,
        Id,
    };

    int m_currentIndex;
    QHash <int, QByteArray> m_roleNames;
};

typedef AFquick::MenuModel AFquickMenuModel;

#endif
