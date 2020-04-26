#include "menuModel.h"

AFquick::MenuModel::MenuModel(QObject *parent) : QAbstractListModel(parent)
{
    m_roleNames[FieldName::Id]        = "id";
    m_roleNames[FieldName::Name]      = "name";
    m_roleNames[FieldName::IconPath]  = "icon_path";
    m_roleNames[FieldName::IsCurrent] = "is_current";
}

int AFquick::MenuModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return emit getCount();
}

QHash<int, QByteArray> AFquick::MenuModel::roleNames() const
{
    return m_roleNames;
}

QVariant AFquick::MenuModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= rowCount() || index.row() < 0)
        return QVariant();

    switch (static_cast <FieldName> (role)){
    case FieldName::Id: return index;
    case FieldName::Name: return getNameByIndex(index.row());
    case FieldName::IconPath: return getIconByIndex(index.row());
    case FieldName::IsCurrent: return isCurrentByIndex(index.row());
    }
    return QVariant();
}

void AFquick::MenuModel::updateData()
{
    emit dataChanged(this->index(0), this->index(rowCount() - 1));
}

void AFquick::MenuModel::beginResetModelSlot()
{
    beginResetModel();
}

void AFquick::MenuModel::endResetModelSlot()
{
    endResetModel();
}
