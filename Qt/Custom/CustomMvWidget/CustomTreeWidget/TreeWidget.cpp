#include "TreeWidget.h"
#include <QPixmap>
TreeWidget::TreeWidget() {}

bool TreeItem::isRootItem() const
{
    return mIsRootItem;
}

void TreeItem::setIsRootItem(bool newIsRootItem)
{
    mIsRootItem = newIsRootItem;
}

void TreeItem::setChildren(const QList<TreeItem*>& newChildren)
{
    mChildren = newChildren;
}

TreeItem::ItemType TreeItem::type() const
{
    return mType;
}

void TreeItem::setType(ItemType newType)
{
    mType = newType;
}

QString TreeItem::name() const
{
    return mName;
}

void TreeItem::setName(const QString& newName)
{
    mName = newName;
}

QString TreeItem::parentName() const
{
    return mParentName;
}

void TreeItem::setParentName(const QString& newParentName)
{
    mParentName = newParentName;
}

QString TreeItem::nameStructure() const
{
    return mNameStructure;
}

void TreeItem::setNameStructure(const QString& newNameStructure)
{
    mNameStructure = newNameStructure;
}

QPixmap TreeItem::pixmap() const
{
    return mPixmap;
}

void TreeItem::setPixmap(const QPixmap& newPixmap)
{
    mPixmap = newPixmap;
}

bool TreeItem::expanded() const
{
    return mExpanded;
}

void TreeItem::setExpanded(bool newExpanded)
{
    mExpanded = newExpanded;
}

void TreeItem::insertChild(int position, TreeItem* pTreeItem)
{
    mChildren.insert(position, pTreeItem);
}

TreeItem* TreeItem::child(int row)
{
    return mChildren.value(row);
}

void TreeItem::moveChild(int from, int to) {}

QVariant TreeItem::data(int column, int role) const
{
    switch (column)
    {
    case 0:
        switch (role)
        {
        case Qt::DisplayRole:
            return mName;
        case Qt::DecorationRole:
        {

            return mPixmap.isNull() ? getTreeItemIcon() : mPixmap;
        }
        }
    }
}

int TreeItem::row() const
{
    if (mpParentTreeItem)
    {
        return mpParentTreeItem->mChildren.indexOf(const_cast<TreeItem*>(this));
    }
    return 0;
}

QIcon TreeItem::getTreeItemIcon() const
{
    switch (mType)
    {
    case TreeItem::TypeA:
        return QIcon(QPixmap(":/Resources/icons/model-icon.svg"));
        break;
    case TreeItem::TypeB:
        return QIcon(QPixmap(":/Resources/icons/class-icon.svg"));
        break;
    case TreeItem::TypeC:
        return QIcon(QPixmap(":/Resources/icons/connect-mode.svg"));
        break;
    default:
        break;
    }
}

int TreeItem::childrenSize() const
{
    return mChildren.size();
}

TreeItem::TreeItem(QAbstractItemModel* pParent)
    : QObject(pParent)
{
    mIsRootItem = true;
}

TreeItem::TreeItem(ItemType type, QString name, QString nameStructure, TreeItem* pParent)
    : QObject(pParent)
    , mType(type)
{
    mIsRootItem = false;
    mpParentTreeItem = pParent;
    setPixmap(QPixmap());
    setName(name);
    setNameStructure(nameStructure);
    setExpanded(true);
}

TreeItem::~TreeItem()
{
    mChildren.clear();
}

TreeProxyModel::TreeProxyModel(TreeWidget* pTreeWidget)
    : mpTreeWidget(pTreeWidget)
{
}
/**
 * @brief: 用于判断指定行是否应该在过滤后显示
 * @param: sourceRow: 行   sourceParent: 父索引
 */
bool TreeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    TreeItem*   pTreeItem = static_cast<TreeItem*>(index.internalPointer());
    if (pTreeItem)
    {
        bool hide = false;
        int  rows = sourceModel()->rowCount(index);
        for (int i = 0; i < rows; ++i)
        {
            if (filterAcceptsRow(i, index))
            {
                return !hide;
            }
        }
        if (hide)
        {
            return false;
        }
        else
        {
            return pTreeItem->nameStructure().contains(filterRegExp());
        }
    }
    else
    {
        return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    }
}

TreeModel::TreeModel(TreeWidget* pTreeWidget)
{
    mpTreeWidget = pTreeWidget;
    mpRootTreeItem = new TreeItem(this);
}

TreeModel::~TreeModel()
{
    if (mpRootTreeItem)
    {
        mpRootTreeItem->deleteLater();
        mpRootTreeItem = nullptr;
    }
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* pParentTreeItem;
    if (parent.column() > 0)
    {
        return 0;
    }
    if (!parent.isValid())
    {
        pParentTreeItem = mpRootTreeItem;
    }
    else
    {
        pParentTreeItem = static_cast<TreeItem*>(parent.internalPointer());
    }
    return pParentTreeItem->childrenSize();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return tr("TreeView");
    }
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    TreeItem* pParentTreeItem;
    if (!parent.isValid())
    {
        pParentTreeItem = mpRootTreeItem;
    }
    else
    {
        pParentTreeItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem* pChildTreeItem = pParentTreeItem->child(row);
    if (pChildTreeItem)
    {
        return createIndex(row, column, pChildTreeItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }
    TreeItem* pChildTreeItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* pParentTreeItem = pChildTreeItem->parent();
    if (pParentTreeItem == mpRootTreeItem)
    {
        return QModelIndex();
    }
    return createIndex(pParentTreeItem->row(), 0, pParentTreeItem);
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    TreeItem* pTreeItem = static_cast<TreeItem*>(index.internalPointer());
    return pTreeItem->data(index.column(), role);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }
    else
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    }
}

Qt::DropActions TreeModel::supportedDropActions() const
{
    return Qt::CopyAction;
}
