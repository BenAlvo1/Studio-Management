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
    virtual ~BaseAction();
    virtual BaseAction* Clone()=0;
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
    virtual ~OpenTrainer();
    virtual OpenTrainer* Clone();
    OpenTrainer(const OpenTrainer& other);
private:
    const int trainerId;
    std::vector<Customer*> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    Order(int id,std::vector<std::string> names,std::vector<std::string> workouts);
    void act(Studio& studio);
    std::string toString() const;
    virtual Order* Clone();
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
    virtual MoveCustomer* Clone();
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(int _id,int _salary);
    void act(Studio& studio);
    std::string toString() const;
    virtual Close* Clone();
private:
    const int trainerId;
    int Salary;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    CloseAll(std::vector<int> salaries,std::vector<int> ids);
    void act(Studio& studio);
    std::string toString() const;
    virtual CloseAll* Clone();
private:
    std::vector<int> a_Salary;
    std::vector<int> a_Id;
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    PrintWorkoutOptions(std::vector<Workout> wo);
    void act(Studio& studio);
    std::string toString() const;
    virtual PrintWorkoutOptions* Clone();
private:
    std::vector<Workout> a_Wo;
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    PrintTrainerStatus(int salary,const int t_id,std::vector<int> acId,std::vector<std::string> ac_Name,std::vector<std::string> _ac_Workout);
    void act(Studio& studio);
    std::string toString() const;
    virtual PrintTrainerStatus* Clone();
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
    PrintActionsLog(std::vector<std::string> actions);
    void act(Studio& studio);
    std::string toString() const;
    virtual PrintActionsLog* Clone();
private:
    std::vector<std::string> a_Actions;
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio& studio);
    std::string toString() const;
    virtual BackupStudio* Clone();
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio& studio);
    std::string toString() const;
    virtual RestoreStudio* Clone();

};


#endif