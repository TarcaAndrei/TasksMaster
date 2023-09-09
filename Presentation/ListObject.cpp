//
// Created by Andrei on 08.09.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListObject.h" resolved

#include "ListObject.h"
#include "ui_ListObject.h"


ListObject::ListObject(QWidget *parent) :
        QWidget(parent), ui(new Ui::ListObject) {
    ui->setupUi(this);
    id_lista = -1;
    this->ui->editButton->setFlat(true);
    this->ui->deleteBtton->setFlat(true);
    this->ui->horizontalLayout->setAlignment(Qt::AlignRight);
    this->ui->editButton->setIcon(QIcon(":/Icons/edit.png"));
    this->ui->deleteBtton->setIcon(QIcon(":/Icons/trash.png"));
    this->ui->lineEdit->setVisible(false);
    this->setVisible(false);
//    this->ui->horizontalLayout->setContentsMargins(0, 0, this->height(), this->width());
//    this->ui->pushButton->setContentsMargins(0, 0, this->height(), this->width());
}

ListObject::~ListObject() {
    delete ui;
}

QPushButton *ListObject::get_del_btn() {
    return this->ui->deleteBtton;
}

QPushButton *ListObject::get_rename_btn() {
    return this->ui->editButton;
}



void ListObject::set_id(int id, const std::string& name) {
    this->id_lista = id;
    this->list_name = name;
    this->ui->lineEdit->setText(QString::fromStdString(name));
}

QLineEdit *ListObject::get_line_edit() {
    return ui->lineEdit;
}

int ListObject::get_id() {
    return this->id_lista;
}

