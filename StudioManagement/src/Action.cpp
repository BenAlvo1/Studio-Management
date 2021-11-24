#include "../include/Action.h"

#include <utility>
extern Studio* backup;
BaseAction::BaseAction():status(COMPLETED)
{

}
ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = COMPLETED;
}

void BaseAction::error(std::string errMsg)
{
    this->errorMsg = std::move(errMsg);
    status = ERROR;
}

std::string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

BaseAction::~BaseAction() = default;

OpenTrainer::OpenTrainer(int id, std::vector<Customer*>& customersList) : BaseAction() , trainerId(id) , customers(customersList)
{
    error("Error: Workout session does not exist or is already open.");
}

void OpenTrainer::act(Studio& studio)
{
    if (studio.getTrainer(this->trainerId) == nullptr||studio.getTrainer(this->trainerId)->isOpen())
        error("Error: Workout session does not exist or is already open.");
    else
    {
        int insertions = 0;
        Trainer *trainer = studio.getTrainer(this->trainerId);
        trainer->openTrainer();
        int cap = trainer->getCapacity();
        for (auto & customer : customers)
        {
            if (trainer->getCustomers().size() < cap)
            {
                trainer->addCustomer(customer);
                insertions++;
            }
            else{
                int size = int(studio.getCustomerNum()-customers.size()+insertions);
                studio.setCustomerNum(size);
                delete &customer;
            }
        }
        complete();
    }
    std::cout<< this->toString()<<std::endl;
}

std::string OpenTrainer::toString() const
{
    if(getStatus()==COMPLETED){
        std::string con;
        for (auto customer : customers)
        {
            con += customer->toString() + " ";
        }
        return std::string("Open " + std::to_string(trainerId)+" "+ con);
    }
    else
    {
        return getErrorMsg();
    }
}

OpenTrainer::~OpenTrainer() {
    for(auto & customer : customers){
        delete &customer;
    }
}

Order::Order(int id) : trainerId(id)
{
    error("Error: Trainer does not exist or is not open.");
}

void Order::act(Studio& studio)
{
    if (studio.getTrainer(this->trainerId) == nullptr||!studio.getTrainer(this->trainerId)->isOpen())
    {
        error("Error: Trainer does not exist or is not open.");
    }
    else {
        Trainer *trainer = studio.getTrainer(this->trainerId);
        std::vector<Customer *> ac = trainer->getCustomers();
        for (auto & i : ac) {
            trainer->order(i->getId(), i->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
        }
        std::vector<OrderPair> Op;
        for (const OrderPair& o: trainer->getOrders()) {
            OrderPair new_op(o.first, Workout(o.second));
            Op.push_back(new_op);
        }
        for (auto & j : Op) {
            a_Names.push_back(trainer->getCustomer(j.first)->getName());
            a_Workouts.push_back(j.second.getName());
        }
        complete();
    }
    std::cout<< this->toString()<<std::endl;
}

std::string Order::toString() const
{
    if(getStatus()==COMPLETED) {
        std::string con;
        for (int i = 0; i < a_Names.size(); i++) {
            con += a_Names[i] + " Is Doing " + a_Workouts[i] + "\n";
        }
        return "Order " + std::to_string(trainerId) + "\n" + con;
    } else return getErrorMsg();
}

Close::Close(int id) : BaseAction(),trainerId(id),Salary(0)
{
    error("Error: Trainer does not exist or is not open.");
}

void Close::act(Studio& studio)
{
    if (studio.getTrainer(this->trainerId) == nullptr||!studio.getTrainer(this->trainerId)->isOpen())
    {
        error("Error: Trainer does not exist or is not open.");
    }
    else {
        Trainer *trainer = studio.getTrainer(this->trainerId);
        Salary = trainer->getSalary();
        trainer->closeTrainer();
        complete();
    }
    std::cout<< this->toString()<<std::endl;
}

std::string Close::toString() const
{
    if(getStatus()==COMPLETED) {
        return std::string("Trainer " + std::to_string(trainerId) + " Closed. Salary " + std::to_string(Salary)+"NIS");
    } else return getErrorMsg();
}

CloseAll::CloseAll(): BaseAction()
{
}

void CloseAll::act(Studio& studio)
{
    std::vector<Trainer*> a_Trainers = studio.getTrainersList();
    for (auto & a_Trainer : a_Trainers)
    {
        if(a_Trainer->isOpen()) {
            a_Salary.push_back(a_Trainer->getSalary());
            a_Trainer->closeTrainer();
            a_Id.push_back(a_Trainer->getId());
        }
    }
    complete();
    std::cout<< this->toString()<<std::endl;
    studio.CloseStudio();
}

std::string CloseAll::toString() const
{
    std::string con;
    for (int i = 0; i < a_Id.size(); i++)
    {
        con += "Trainer " + std::to_string(a_Id[i]) + " Closed. Salary " + std::to_string(a_Salary[i])+"NIS"+"\n";
    }
    return con;
}

PrintWorkoutOptions::PrintWorkoutOptions(): BaseAction()
{
}

void PrintWorkoutOptions::act(Studio& studio)
{
    for (const Workout& w : studio.getWorkoutOptions()) {
        a_Wo.emplace_back(w);
    }
    complete();
    std::cout<< this->toString()<<std::endl;
}

std::string PrintWorkoutOptions::toString() const
{
    std::string con;
    for (const auto & i : a_Wo)
    {
        con += i.getDetails() + "\n";
    }
    return con;
}

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id),Salary(0)
{

}

void PrintTrainerStatus::act(Studio& studio)
{
    Trainer *trainer = studio.getTrainer(this->trainerId);
    Salary = trainer->getSalary();
    std::vector<Customer*> ac = trainer->getCustomers();

    for (auto & i : ac)
    {
        ac_Id.push_back(i->getId());
        ac_Name.push_back(i->getName());
    }
    for(const OrderPair& order: trainer->getOrders()){
        ac_Workout.push_back(order.second.getName() + " " + std::to_string(order.second.getPrice())+"NIS "+std::to_string(order.first));
    }
    complete();
    std::cout<< this->toString()<<std::endl;
}

std::string PrintTrainerStatus::toString() const
{
    if (ac_Id.empty())
    {
        return std::string("Trainer " + std::to_string(trainerId) + " closed.");
    }
    else
    {
        std::string con;
        for (int i = 0; i < ac_Id.size(); i++)
        {
            con += ac_Name[i] + " " + std::to_string(ac_Id[i]) + "\n";
        }
        con += "Order:\n";
        for (const auto & i : ac_Workout)
        {
            con += i + "\n";
        }
        con += "Current Trainer's Salary: " + std::to_string(Salary)+"NIS";

        return std::string("Trainer " + std::to_string(trainerId) + " Status: open\nCustomers:\n" + con);
    }

}

PrintActionsLog::PrintActionsLog():  BaseAction()
{

}

void PrintActionsLog::act(Studio& studio)
{
    for(int i=0; i< studio.getActionsLog().size();i++) {
        if(studio.getActionsLog()[i]->getStatus()==ERROR)
            a_Actions.push_back(studio.getInputs()[i] +" "+ studio.getActionsLog()[i]->toString());
        else
            a_Actions.push_back(studio.getInputs()[i]+" Completed");
    }
    complete();
    std::cout<< this->toString()<<std::endl;
}

std::string PrintActionsLog::toString() const
{
    std::string con;
    for (const auto & a_Action : a_Actions)
    {
        con += a_Action + "\n";
    }
    return con;
}

BackupStudio::BackupStudio():BaseAction()
{
}

void BackupStudio::act(Studio& studio)
{
    if(backup== nullptr)
        backup = new Studio(studio);
    else{
        delete backup;
        backup = nullptr;
        backup = new Studio(studio);
    }
    complete();
    std::cout<< this->toString()<<std::endl;
}

std::string BackupStudio::toString() const
{
    return "Studio backed-up";
}

RestoreStudio::RestoreStudio():BaseAction()
{

}

void RestoreStudio::act(Studio& studio)
{
    studio = *backup;
    complete();
    std::cout<< this->toString()<<std::endl;
}

std::string RestoreStudio::toString() const
{
    return "Studio restored";
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src) , dstTrainer(dst) , id(customerId)
{
    error("Error: Cannot move customer.");
}

void MoveCustomer::act(Studio& studio)
{
    Trainer* moveFromTrainer = studio.getTrainer(srcTrainer);
    Trainer* moveToTrainer = studio.getTrainer(dstTrainer);
    bool checkOrderExists = false;
    if (moveFromTrainer == nullptr || moveToTrainer==nullptr ||
            !moveFromTrainer->isOpen()||!moveToTrainer->isOpen()||
            moveFromTrainer->getCustomer(id)==nullptr||moveToTrainer->getCapacity()==0)
        error("Error: Cannot move customer.");
    else
    {
        for(const OrderPair& o: moveFromTrainer->getOrders()){
            if(o.first==id) {
                checkOrderExists = true;
                break;
            }
        }
        Customer* customer = moveFromTrainer->getCustomer(id);
        moveFromTrainer->removeCustomer(id);
        if (moveFromTrainer->getCustomers().empty())
        {
            moveFromTrainer->closeTrainer();
        }
        moveToTrainer->addCustomer(customer);
        if(checkOrderExists)
            moveToTrainer->order(customer->getId(),customer->order(studio.getWorkoutOptions()),studio.getWorkoutOptions());
        complete();
    }
    std::cout<< this->toString()<<std::endl;
}

std::string MoveCustomer::toString() const
{
    if(getStatus()== COMPLETED)
        return std::string("move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) +" "+ std::to_string(id));
    else
        return getErrorMsg();
}
