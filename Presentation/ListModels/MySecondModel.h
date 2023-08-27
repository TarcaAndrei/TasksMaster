//
// Created by Andrei on 27.08.2023.
//

#ifndef APP1_MYSECONDMODEL_H
#define APP1_MYSECONDMODEL_H
#include <QAbstractListModel>
#include "../../Service/ServiceApp.h"

class MySecondModel : public QAbstractListModel{
private:
    ServiceApp& serviceApp;
    string list_name;
    //asta tre sa aiba dinala->ai de capu meu....
public:
    explicit MySecondModel(ServiceApp &serviceApp);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void change();
    void set_list_name(const string& list_name_r="");
};


#endif //APP1_MYSECONDMODEL_H