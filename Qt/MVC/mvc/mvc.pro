TEMPLATE = subdirs

SUBDIRS += \
    base \
    tableview \
    test \
    tiledlistview \
    treeview

tableview.depends = base
treeview.depends = base
