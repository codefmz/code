#ifndef TREEWIDGET_H
#define TREEWIDGET_H
#include <QDrag>
#include <QFileInfo>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QTreeView>

class TreeItem : public QObject
{
    Q_OBJECT
public:
    enum ItemType
    {
        TypeA,
        TypeB,
        TypeC
    };

    //===============get and set===============
    bool             isRootItem() const;
    void             setIsRootItem(bool newIsRootItem);
    QList<TreeItem*> children() const;
    void             setChildren(const QList<TreeItem*>& newChildren);
    ItemType         type() const;
    void             setType(ItemType newType);
    QString          name() const;
    void             setName(const QString& newName);
    QString          parentName() const;
    void             setParentName(const QString& newParentName);
    QString          nameStructure() const;
    void             setNameStructure(const QString& newNameStructure);
    QPixmap          pixmap() const;
    void             setPixmap(const QPixmap& newPixmap);
    bool             expanded() const;
    void             setExpanded(bool newExpanded);
    //==============================
    void setParent(TreeItem* pParent)
    {
        mpParentTreeItem = pParent;
    }
    TreeItem* parent() const
    {
        return mpParentTreeItem;
    }
    void      insertChild(int position, TreeItem* pTreeItem);
    TreeItem* child(int row);
    void      moveChild(int from, int to);
    QVariant  data(int column, int role = Qt::DisplayRole) const;
    int       row() const;
    QIcon     getTreeItemIcon() const;
    int      childrenSize() const;
    //===========================
    TreeItem(QAbstractItemModel* pParent);
    TreeItem(ItemType type, QString name, QString nameStructure, TreeItem* pParent = 0);
    ~TreeItem();

private:
    bool             mIsRootItem;
    TreeItem*        mpParentTreeItem = 0;
    QList<TreeItem*> mChildren;
    ItemType         mType = ItemType::TypeA;
    QString          mName;
    QString          mParentName;
    QString          mNameStructure;
    QPixmap          mPixmap;
    bool             mExpanded = false;
};

class TreeWidget;
class TreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TreeProxyModel(TreeWidget* pTreeWidget);

private:
    TreeWidget* mpTreeWidget;

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
};

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(TreeWidget* pTreeWidget);
    ~TreeModel();

    // model function
    int           columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int           rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex   index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex   parent(const QModelIndex& index) const override;
    QVariant      data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

protected:
    Qt::DropActions supportedDropActions() const override;

private:
    TreeWidget* mpTreeWidget;
    TreeItem*   mpRootTreeItem;
};

class TreeWidget
{
public:
    TreeWidget();
};

#endif // TREEWIDGET_H
