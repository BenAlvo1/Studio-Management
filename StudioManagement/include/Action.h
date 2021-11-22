#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Studio.h"

enum ActionStatus {
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction {
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio) = 0;
    virtual std::string toString() const = 0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer*>& customersList);
    void act(Studio& studio);
    std::string toString() const;
private:
    const int trainerId;
    std::vector<Customer*> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio& studio);
    std::string toString() const;
private:
    const int trainerId;
    std::vector<std::string> a_Names;
    std::vector<std::string> a_Workouts;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio& studio);
    std::string toString() const;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio& studio);
    std::string toString() const;
private:
    const int trainerId;
    int Salary;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio& studio);
    std::string toString() const;
private:
    std::vector<int> a_Salary;
    std::vector<int> a_Id;
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio& studio);
    std::string toString() const;
private:
    std::vector<Workout> a_Wo;
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio& studio);
    std::string toString() const;
private:
    int Salary;
    const int trainerId;
    std::vector<int> ac_Id;
    std::vector<std::string> ac_Name;
    std::vector<std::string> ac_Workout;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio& studio);
    std::string toString() const;
private:
    std::vector<std::string> a_Actions;
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio& studio);
    std::string toString() const;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio& studio);
    std::string toString() const;

};


#endif