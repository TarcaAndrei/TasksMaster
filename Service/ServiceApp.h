//
// Created by Andrei on 27.08.2023.
//

#ifndef APP1_SERVICEAPP_H
#define APP1_SERVICEAPP_H


#include "../Repository/RepositoryApp.h"
#define TASK_DUE "Task Due Now"
#include <QTimer>

class ServiceApp : public ObserverApp, public ObservableApp{
private:
    RepositoryApp& repositoryApp;
    QTimer* timer;
//    int generate_id(const string& table_name);
public:
    explicit ServiceApp(RepositoryApp &repositoryApp);
    std::vector<std::pair<int, string>> get_all_lists();
    vector<Task> get_tasks_from_list(int id_list);
    void add_new_task(int id_list, const string &name_t, const string &details_t, const QDateTime& time_due_t, const string &priority_t, bool is_done=false);
    void check_if_there_is_a_task_due();
    void update(const std::string &option, const std::string &option2, const Task &task) override;
    Task get_task_from_id(int id_task);
    void modify_task(int id_task, int id_list, const string &name_t, const string &details_t, const QDateTime& time_due_t, const string &priority_t, bool is_done=false);
    void delete_task(int id_task);
    void check_task_due();
    void add_new_list(const std::string &listname);
    void modifiy_list(int id_l, const std::string&newname);
    void delete_list(int id_l);
    bool is_task_done(int id_t);
    void set_task_done(int id_task, int id_list, bool is_done);
    ServiceApp()=default;
    ~ServiceApp()=default;
};


#endif //APP1_SERVICEAPP_H
