#include "../include/Trainer.h"

Trainer::Trainer(int t_id,int t_capacity):id(t_id),capacity(t_capacity),open(false),customersList(),orderList()
{
}

int Trainer::getCapacity() const
{
    return capacity;
}

void Trainer::addCustomer(Customer* customer)
{
    if (customer != nullptr && capacity>0) {
        customersList.push_back(customer);
        capacity--;
    }
}

void Trainer::removeCustomer(int c_id)
{
    int index = 0;
    for(Customer * i:customersList) {
        if (i->getId()==c_id)
        {
            customersList.erase(customersList.begin()+index);
            capacity++;
            break;
        }
        index++;
    }
    std::vector<OrderPair> backup;
    for(const OrderPair& order : orderList){
        if(order.first==c_id)
            continue;
        else backup.emplace_back(order.first,Workout(order.second));
    }
    orderList.clear();
    for(const OrderPair& order: backup){
        orderList.emplace_back(order.first,Workout(order.second));
    }
}

Customer* Trainer::getCustomer(int c_id)
{
    for (Customer* i : customersList) {
        if (i->getId() == c_id)
        {
            return i;
        }
    }
    return nullptr;
}

std::vector<Customer*>& Trainer::getCustomers()
{
    return customersList;
}

std::vector<OrderPair>& Trainer::getOrders()
{
    return orderList;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
    for(const OrderPair& i: orderList){
        if(i.first==customer_id)
            return;
    }
    for (int i : workout_ids) {
        for (const Workout& workout : workout_options) {
            if (i == workout.getId()) {
                orderList.emplace_back(customer_id, workout);
            }
        }
    }
}

void Trainer::openTrainer()
{
    open = true;
}

void Trainer::closeTrainer() {
    open = false;
    orderList.clear();
    for(Customer* customer:customersList){
        delete customer;
        capacity++;
    }
    customersList.clear();
}

int Trainer::getSalary()
{
    int salary = 0;
    for (const OrderPair& order : orderList) {
        salary += order.second.getPrice();
    }
    return salary;
}

bool Trainer::isOpen() const
{
    return open;
}

int Trainer::getId() const
{
    return id;
}

Trainer::~Trainer()
{
    clear();
}

Trainer::Trainer(const Trainer& other): id(other.id),capacity(other.capacity),open(other.open),customersList(),orderList()
{
    copy(other);
}

Trainer::Trainer(Trainer&& other):id(other.id),capacity(other.capacity),open(other.open),customersList(),orderList()
{
    other.capacity = 0;
    other.clear();
}

Trainer& Trainer::operator=(const Trainer& other)
{
    if (this == &other) {
        return *this;
    }
    clear();
    copy(other);
    return *this;
}

Trainer& Trainer::operator=(Trainer &&other)
{
    if (this != &other) {
        clear();
        capacity = other.capacity;
        orderList.clear();
        customersList = other.customersList;
        id = other.id;
        open = other.open;
        other.capacity = 0;
    }
    return *this;
}

void Trainer::copy(const Trainer& other)
{
    id = other.id;
    capacity = other.getCapacity();
    open = other.open;
    for (Customer* c : other.customersList) {
        customersList.push_back(c->Clone());
    }
    for (const OrderPair& op : other.orderList) {
        orderList.emplace_back(op.first, op.second);
    }
}

void Trainer::clear()
{
    for (size_t i=0;i<customersList.size();i++) {
        delete customersList[i];
        customersList[i]= nullptr;
    }
    orderList.clear();
    open = false;
    capacity=0;
}
