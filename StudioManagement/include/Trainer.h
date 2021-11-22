#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer {
public:
    Trainer(int id,int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen() const;
    int getId() const;
    virtual ~Trainer(); //destructor
    Trainer(const Trainer& other); //copy constructor
    Trainer(Trainer&& other); //move constructor
    Trainer& operator=(const Trainer& other); //copy assignment
    Trainer& operator=(Trainer&& other); //move assignment


private:
    int id;
    int capacity;
    bool open;
    void copy(const Trainer& other);
    void clear();
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif