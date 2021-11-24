#include "../include/Action.h"
extern Studio* backup;
BaseAction::BaseAction():errorMsg("Error"),status(COMPLETED)
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

void BaseAction::error(std::string errorMsg)
{
    this->errorMsg = errorMsg;
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
    if (studio.getTrainer(this->trainerId) == nullptr)
    {
        error("Error: Workout session does not exist or is already open.");
    }
    else if (studio.getTrainer(this->trainerId)->isOpen())
    {
        error("Error: Workout session does not exist or is already open.");
    }
    else
    {
        int insertions = 0;
        Trainer *trainer = studio.getTrainer(this->trainerId);
        trainer->openTrainer();
        unsigned int cap = trainer->getCapacity();
        for (size_t i=0;i<customers.size();i++)
        {
            if (trainer->getCustomers().size() < cap)
            {
                trainer->addCustomer(customers[i]->Clone());
                insertions++;
            }
            else{
                break;
            }
        }
        for(size_t i=customers.size()-1; i>=(size_t)insertions;i--){
            delete customers[i];
            customers.erase(customers.begin()+i);
        }
        studio.setCustomerNum(studio.getCustomerNum()-customers.size()+insertions);
        complete();
    };
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
    for(unsigned int i =0;i<customers.size();i++){
        delete customers[i];
        customers[i]= nullptr;
    }
    customers.clear();
}

OpenTrainer *OpenTrainer::Clone() {
    return new OpenTrainer(*this);
}

OpenTrainer::OpenTrainer(const OpenTrainer& other): trainerId(other.trainerId),customers() {
    for (unsigned int i = 0; i < other.customers.size();i++ ) {
        customers.push_back(other.customers[i]->Clone());
    }
}

Order::Order(int id) : BaseAction(),trainerId(id),a_Names(),a_Workouts()
{
    error("Error: Trainer does not exist or is not open.");
}
Order::Order(int id, std::vector<std::string> names, std::vector<std::string> workouts):trainerId(id),a_Names(names),a_Workouts(workouts) {

}

void Order::act(Studio& studio)
{
    if (studio.getTrainer(this->trainerId) == nullptr)
    {
        error("Error: Trainer does not exist or is not open.");
    }
    else if (!studio.getTrainer(this->trainerId)->isOpen())
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
        for (OrderPair o: trainer->getOrders()) {
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
        for (size_t i = 0; i < a_Names.size(); i++) {
            con += a_Names[i] + " Is Doing " + a_Workouts[i] + "\n";
        }
        return "Order " + std::to_string(trainerId) + "\n" + con;
    } else return getErrorMsg();
}

Order *Order::Clone() {
    return new Order(this->trainerId, this->a_Names, this->a_Workouts);
}

Close::Close(int id) : BaseAction(),trainerId(id),Salary(0)
{
    error("Error: Trainer does not exist or is not open.");
}
Close::Close(int _id, int _salary):trainerId(_id),Salary(_salary) {
    error("Error: Trainer does not exist or is not open.");
}

void Close::act(Studio& studio)
{
    if (studio.getTrainer(this->trainerId) == nullptr)
    {
        error("Error: Trainer does not exist or is not open.");
    }
    else if (!studio.getTrainer(this->trainerId)->isOpen())
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

Close *Close::Clone() {
    return new Close(this->trainerId, this->Salary);
}


CloseAll::CloseAll(): BaseAction(), a_Salary(),a_Id()
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
    for (size_t i = 0; i < a_Id.size(); i++)
    {
        con += "Trainer " + std::to_string(a_Id[i]) + " Closed. Salary " + std::to_string(a_Salary[i])+"NIS"+"\n";
    }
    return con;
}

CloseAll::CloseAll(std::vector<int> salaries, std::vector<int> ids):a_Salary(salaries),a_Id(ids) {

}

CloseAll *CloseAll::Clone() {
    return new CloseAll(this->a_Salary,this->a_Id);
}

PrintWorkoutOptions::PrintWorkoutOptions(): BaseAction(), a_Wo()
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

PrintWorkoutOptions::PrintWorkoutOptions(std::vector<Workout> wo): a_Wo(wo){

}

PrintTrainerStatus::PrintTrainerStatus(int id) : BaseAction(),Salary(0),trainerId(id),ac_Id(),ac_Name(),ac_Workout()
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

PrintWorkoutOptions *PrintWorkoutOptions::Clone() {
    return new PrintWorkoutOptions(this->a_Wo);
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
        for (size_t i = 0; i < ac_Id.size(); i++)
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

PrintTrainerStatus::PrintTrainerStatus(int salary, const int t_id, std::vector<int> acId,std::vector<std::string> _ac_Name, std::vector<std::string> _ac_Workout):Salary(salary),trainerId(t_id),ac_Id(acId),ac_Name(_ac_Name),ac_Workout(_ac_Workout) {

}

PrintTrainerStatus *PrintTrainerStatus::Clone() {
    return new PrintTrainerStatus(this->Salary, this->trainerId, this->ac_Id, this->ac_Name, this->ac_Workout);
}

PrintActionsLog::PrintActionsLog():  BaseAction(),a_Actions()
{

}

void PrintActionsLog::act(Studio& studio)
{
    for(size_t i=0; i< studio.getActionsLog().size();i++) {
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

PrintActionsLog::PrintActionsLog(std::vector<std::string> actions):a_Actions(actions) {

}

PrintActionsLog *PrintActionsLog::Clone() {
    return new PrintActionsLog(this->a_Actions);
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

BackupStudio *BackupStudio::Clone() {
    return new BackupStudio();
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

RestoreStudio *RestoreStudio::Clone() {
    return new RestoreStudio();
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
        for(OrderPair o: moveFromTrainer->getOrders()){
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

MoveCustomer *MoveCustomer::Clone() {
    return new MoveCustomer(this->srcTrainer, this->dstTrainer, this->id);
}
