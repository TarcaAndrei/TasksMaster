//
// Created by Andrei on 05.09.2023.
//

#ifndef APP1_OBSERVERAPP_H
#define APP1_OBSERVERAPP_H

#include <string>
#include "../Domain/Task.h"

class ObserverApp {
public:
    virtual void update(const std::string &option, const std::string &option2, const Task &task) =0;
};


#endif //APP1_OBSERVERAPP_H
