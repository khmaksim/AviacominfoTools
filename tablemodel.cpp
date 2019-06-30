#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
{

}

TableModel::~TableModel()
{

}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role == Qt::UserRole)
    {
        if ((Qt::CheckState)value.toInt() == Qt::Checked)
            return QStandardItemModel::setData(index, value, role);
        else
            return QStandardItemModel::setData(index, value, role);
    }
    return QStandardItemModel::setData(index, value, role);
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::CheckStateRole && index.column() == 0)
    {
        if (QStandardItemModel::data(index, role).toInt() == Qt::Checked)
            return Qt::Checked;
        return Qt::Unchecked;
    }
    return QStandardItemModel::data(index, role);
}
