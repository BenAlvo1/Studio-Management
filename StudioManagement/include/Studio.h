#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
class BaseAction;
class Studio {
public:
    Studio();
    Studio(const std::string& configFilePath);
    void start();
    std::vector<Trainer*> getTrainersList();
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void setCustomerNum(int size);
    int getCustomerNum() const;
    void CloseStudio();
    std::vector<std::string> getInputs() const;
    virtual ~Studio(); //destructor
    Studio(const Studio& other); //copy constructor
    Studio(Studio&& other); //move constructor
    Studio& operator=(const Studio& other); //copy assignment
    Studio& operator=(Studio&& other); //move assignment

private:
    bool open;
    int trainers_num;
    int customer_num;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    std::vector<std::string> splitter(std::string s);
    Customer* createCustomer(const std::string& c);
    std::vector<std::string> inputs;
    void clear();
};

#endif