#ifndef TABLEDELEGATES_H
#define TABLEDELEGATES_H
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QBuffer>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QEvent>
#include <QMouseEvent>
#include <QFileDialog>
class ComboBoxDelegate: public QStyledItemDelegate
{
public:
    ComboBoxDelegate(QObject* parent =nullptr):  QStyledItemDelegate(parent) {}
    void setItems(const QStringList& items){
        m_items=items;
    }
    QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem& option,const QModelIndex& index)const override{
        Q_UNUSED(option);
        Q_UNUSED(index);

        QComboBox* editor =new QComboBox(parent);
        editor->addItems(m_items);
        return editor;
    }

    void setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex& index)const override{
        QComboBox* comboBox =static_cast<QComboBox*>(editor);
        QString value=comboBox->currentText();
        model->setData(index,value,Qt::EditRole);
    }

private:
    QStringList m_items;
};


class DateEditDelegate: public QStyledItemDelegate
{
public:
    DateEditDelegate(QObject* parent =nullptr):  QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem& option,const QModelIndex& index)const override{
        QDateEdit* editor=new QDateEdit(parent);
        editor->setDisplayFormat("yyyy-MM-dd");
        editor->setCalendarPopup(true);
        return editor;
    }

    void setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex& index)const override{
        QDateEdit* dateEditor =static_cast<QDateEdit*>(editor);
        if(dateEditor)
            model->setData(index,dateEditor->date().toString("yyyy-MM-dd"),Qt::EditRole);
    }
};

class ImageDelegate: public QStyledItemDelegate
{
public:
    ImageDelegate(QObject* parent =nullptr):  QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem& option,const QModelIndex& index)const override{
        Q_UNUSED(option);Q_UNUSED(index);
        QLabel* editor=new QLabel(parent);
        return editor;
    }

    void setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex& index)const override{
        QLabel* label =qobject_cast<QLabel*>(editor);
        if(label){
            QByteArray imageData;
            QPixmap pixmap=label->pixmap();
            if(!pixmap.isNull()){
                QBuffer buffer(&imageData);
                buffer.open(QIODevice::WriteOnly);
                pixmap.save(&buffer,"PNG");
            }
            model->setData(index,imageData,Qt::UserRole);
        }
    }
    void paint(QPainter* painter,const QStyleOptionViewItem& option,const QModelIndex& index)const override{
        QByteArray imageData =index.data(Qt::UserRole).toByteArray();
        if(imageData.isEmpty()){
            QStyledItemDelegate::paint(painter,option,index);
            return;
        }
        QPixmap pixmap;
        pixmap.loadFromData(imageData);
        if(pixmap.isNull()){
            QStyledItemDelegate::paint(painter,option,index);
            return;
        }

        QRect rect=option.rect;
        QPixmap scaledPixmap=pixmap.scaled(100,100,Qt::KeepAspectRatio);
        painter->drawPixmap(rect,scaledPixmap);
    }

    bool editorEvent(QEvent* event,QAbstractItemModel* model,const QStyleOptionViewItem& option,const QModelIndex& index) override{

        if(event->type()== QEvent::MouseButtonDblClick){
            QMouseEvent* mouseEvent =static_cast<QMouseEvent*>(event);
            if(mouseEvent->button()==Qt::LeftButton){
                QString imagePath =QFileDialog::getOpenFileName(nullptr,"选择图片","","图片文件(*.png *.jpg *.bmp)");
                if(!imagePath.isEmpty()){
                    QFile file(imagePath);
                    if(file.open(QIODevice::ReadOnly)){
                        QByteArray imageData=file.readAll();
                        file.close();
                        model->setData(index,imageData,Qt::UserRole);
                    }
                }
                return true;
            }


        }
        return QStyledItemDelegate::editorEvent(event,model,option,index);
    }

};

#endif // TABLEDELEGATES_H
