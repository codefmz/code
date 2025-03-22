#ifndef TILEDLISTVIEW_H
#define TILEDLISTVIEW_H

#include <QAbstractItemView>
#include <QHash>
#include <QRectF>

class TiledListView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit TiledListView(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model) override;
    QRect visualRect(const QModelIndex &index) const override;
    void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint) override;
    QModelIndex indexAt(const QPoint &point_) const override;

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;
    void rowsInserted(const QModelIndex &parent, int start, int end) override;
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end) override;
    void updateGeometries() override;

protected:
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
    bool isIndexHidden(const QModelIndex&) const override {
        return false;
    }
    int horizontalOffset() const override;
    int verticalOffset() const override;
    void scrollContentsBy(int dx, int dy) override;
    void setSelection(const QRect &rect, QFlags<QItemSelectionModel::SelectionFlag> flags) override;
    QRegion visualRegionForSelection(const QItemSelection &selection) const override;

    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    void calculateRectsIfNecessary() const;
    QRectF viewportRectForRow(int row) const;
    void paintOutline(QPainter *painter, const QRectF &rectangle);
private:
    mutable int idealWidth;
    mutable int idealHeight;
    mutable QHash<int, QRectF> rectForRow;
    mutable bool hashIsDirty;
};

#endif // TILEDLISTVIEW_H
