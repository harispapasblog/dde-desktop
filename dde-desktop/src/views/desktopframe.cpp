#include "views/desktopframe.h"
#include "widgets/util.h"

DesktopFrame::DesktopFrame(QWidget *parent)
    : TranslucentFrame(parent)
{

    setAcceptDrops(true);

    gridItemWidth = 110;
    xGridSpacing = 10;
    yGridSpacing = 10;
    xGridStartPos = 10;
    yGridStartPos = 10;
    columnCount = 0;
    rowCount = 0;

    initGrid();
    initDesktopItems();

    qApp->setStyleSheet(getQssFromFile(":/skin/qss/DesktopItem.qss"));
}

void DesktopFrame::initGrid(){
    const QRect availableGeometry = QApplication::desktop()->availableGeometry();
    int _desktopWidth = availableGeometry.width();
    int _desktopHeight = availableGeometry.height();

    columnCount = _desktopWidth / (gridItemWidth + xGridSpacing);
    rowCount = _desktopHeight / (gridItemWidth + yGridSpacing);

    int x=xGridStartPos;
    int y=yGridStartPos;
    for (int i=0; i< columnCount; i++){
        GridListPointer _gridlist =GridListPointer::create();
        m_gridItems.append(_gridlist);
        for (int j=0; j< rowCount; j++){
            QRect rect = QRect(x, y, gridItemWidth, gridItemWidth);
            GridItemPointer item = GridItemPointer::create(j, i, rect);
            _gridlist->append(item);
            y = (gridItemWidth + yGridSpacing) * (j + 1) + yGridStartPos;
        }
        x = (gridItemWidth + xGridSpacing) * (i + 1) + xGridStartPos;
        y = yGridStartPos;
    }
}

void DesktopFrame::initDesktopItems(){
    for(int i=0; i<12; i++){
        for (int j=0; j<7; j++){
            DesktopItemPointer  desktopItem = DesktopItemPointer::create(":/skin/images/QFramer.png", "11145454545544554144545", this);
            desktopItem->resize(110, 110);
            m_desktopItems.append(desktopItem);

            GridItemPointer gridItem = m_gridItems.at(i)->at(j);
            if (!gridItem.isNull()){
                QRect rect = gridItem->getRect();
                desktopItem->move(rect.topLeft());
            }
        }
    }
}


DesktopItemPointer DesktopFrame::getTopDesktopItemByPos(QPoint pos){
    for (int i=m_desktopItems.count() - 1; i >= 0; i--){
        if (m_desktopItems.at(i)->geometry().contains(pos)){
            m_TopDeskItem = m_desktopItems.at(i);
            return m_TopDeskItem;
        }
    }
    return DesktopItemPointer();
}

void DesktopFrame::checkDesktopItemsByRect(QRect rect){
    foreach (DesktopItemPointer pItem, m_desktopItems) {
        if (rect.intersects(pItem->geometry())){
            emit pItem->checkedChanged(true);
        }else{
            emit pItem->checkedChanged(false);
        }
    }
}

void DesktopFrame::unCheckAllItems(){
    foreach (DesktopItemPointer pItem, m_desktopItems) {
        emit pItem->checkedChanged(false);
    }
}

void DesktopFrame::unCheckCheckedItems(){
    foreach (DesktopItemPointer pItem, m_checkedDesktopItems) {
        emit pItem->checkedChanged(false);
    }
    m_checkedDesktopItems.clear();
}


void DesktopFrame::checkRaiseItem(DesktopItemPointer& item){
    if (!item.isNull()){
        emit item->checkedChanged(true);
        item->raise();

        bool flag = m_desktopItems.removeOne(item);
        if (flag){
            m_desktopItems.append(item);
        }
    }
}

QList<DesktopItemPointer> DesktopFrame::getCheckedDesktopItems(){
    m_checkedDesktopItems.clear();
    foreach (DesktopItemPointer pItem, m_desktopItems) {
        if (pItem->isChecked()){
            m_checkedDesktopItems.append(pItem);
        }
    }
    return m_checkedDesktopItems;
}

void DesktopFrame::focusInEvent(QFocusEvent *event){
    m_isSelectable = true;
    m_selectRect = QRect(0, 0, 0, 0);
    TranslucentFrame::focusInEvent(event);
}


void DesktopFrame::focusOutEvent(QFocusEvent *event){
    m_isSelectable = false;
    m_selectRect = QRect(0, 0, 0, 0);
    TranslucentFrame::focusOutEvent(event);
}


void DesktopFrame::dragEnterEvent(QDragEnterEvent *event){
    if (event->mimeData()->hasFormat("application/x-dnditemdata")){
        if (event->source() == this){
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }else{
            event->ignore();
        }
    }else{
        event->ignore();
    }
}


void DesktopFrame::dragMoveEvent(QDragMoveEvent *event){
    if (event->mimeData()->hasFormat("application/x-dnditemdata")){
        if (event->source() == this){
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }else{
            event->acceptProposedAction();
        }
    }else{
        event->ignore();
    }
}

void DesktopFrame::dropEvent(QDropEvent *event){
    if (event->mimeData()->hasFormat("application/x-dnditemdata")){
        if (event->source() == this){
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }else{
            event->acceptProposedAction();
        }
    }else{
        event->ignore();
    }
}


void DesktopFrame::mousePressEvent(QMouseEvent *event){
    m_pressedEventPos = event->pos();
    DesktopItemPointer pTopDesktopItem  = getTopDesktopItemByPos(m_pressedEventPos);
    m_checkedDesktopItems = getCheckedDesktopItems();

    if (event->button() == Qt::LeftButton){

        if (pTopDesktopItem.isNull()){
            setFocus();
            unCheckCheckedItems();
        }else{
            if (!pTopDesktopItem->isChecked()){
                checkRaiseItem(pTopDesktopItem);
            }

            if (m_checkedDesktopItems.indexOf(pTopDesktopItem) == -1){
                unCheckCheckedItems();
                m_checkedDesktopItems.append(pTopDesktopItem);
            }

            startDrag();

        }

    }else if (event->button() == Qt::RightButton){
        unCheckCheckedItems();
        if (!pTopDesktopItem.isNull()){
            checkRaiseItem(pTopDesktopItem);
        }
    }

    TranslucentFrame::mousePressEvent(event);
}

void DesktopFrame::startDrag(){
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QMimeData* mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QPixmap dragPixmap = getCheckedPixmap();

    if (m_checkedDesktopItems.length() > 0){
        QDrag* drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(dragPixmap);
        drag->setHotSpot(mapFromGlobal(QCursor::pos()));
        Qt::DropAction action = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::MoveAction);
        if (action == Qt::MoveAction){
            foreach (DesktopItemPointer pItem, m_checkedDesktopItems) {
                QPoint newPos = pItem->pos() + QCursor::pos() - m_pressedEventPos;
                pItem->move(newPos);
                m_desktopItems.removeOne(pItem);
            }
            m_desktopItems.append(m_checkedDesktopItems);
            foreach (DesktopItemPointer pItem, m_desktopItems) {
                pItem->raise();
            }
        }else{
            unCheckCheckedItems();
            checkRaiseItem(m_TopDeskItem);
        }
    }
}


QPixmap DesktopFrame::getCheckedPixmap(){
    TranslucentFrame* F = new TranslucentFrame(this);
    F->resize(size());
    foreach (DesktopItemPointer pItem, m_checkedDesktopItems) {
        DesktopItem* item = new DesktopItem(pItem->getDesktopIcon(),
                                       pItem->getDesktopName(), F);
        item->resize(pItem->size());
        item->move(pItem->pos());
        item->setObjectName("DragChecked");
    }
    F->setStyleSheet(qApp->styleSheet());
    QPixmap ret = F->grab();
    F->close();
    return ret;
}

void DesktopFrame::mouseReleaseEvent(QMouseEvent *event){
    clearFocus();
    update();

    TranslucentFrame::mouseReleaseEvent(event);
}

void DesktopFrame::mouseMoveEvent(QMouseEvent *event){

    int x = m_pressedEventPos.x();
    int y = m_pressedEventPos.y();
    int width = event->pos().x() - x;
    int height = event->pos().y() -y;
    m_selectRect = QRect(x, y , width, height);
    update();
    checkDesktopItemsByRect(m_selectRect);
    TranslucentFrame::mouseMoveEvent(event);
}

void DesktopFrame::paintEvent(QPaintEvent *event){
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    foreach (GridListPointer gridlist, m_gridItems) {
        foreach (GridItemPointer pGridItem, *gridlist) {
            int row = pGridItem->getRow();
            int column = pGridItem->getColumn();
            int _c = 255 * (row + column * rowCount) / (columnCount * rowCount);
            QColor color(_c, _c, _c, 100);
            painter.fillRect(pGridItem->getRect(), color);
        }
    }
    if (m_isSelectable){
        QColor color(0, 0, 0, 90);
        painter.fillRect(m_selectRect, color);
    }
    TranslucentFrame::paintEvent(event);
}


void DesktopFrame::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Escape){
        close();
    }
    TranslucentFrame::keyPressEvent(event);
}

DesktopFrame::~DesktopFrame()
{

}