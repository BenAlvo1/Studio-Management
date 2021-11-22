#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id) :name(c_name), id(c_id) {}

std::string Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}

Customer::Customer(const Customer& other):name(other.name),id(other.id)
{
}

Customer::Customer(Customer&& other):name(other.name),id(other.id)
{
}

Customer::~Customer() {
}


SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name,id){}

std::vector<int> SweatyCustomer::order(const std::vector<Workout>& workout_options)
{
    std::vector<int> orders;
    for (Workout w : workout_options) {
        if (w.getType() == 2) {
            orders.push_back(w.getId());
        }
    }
    return orders;
}

std::string SweatyCustomer::toString() const
{
    return getName()+",swt";
}

CheapCustomer::CheapCustomer(std::string name, int id):Customer(name, id) {}

std::vector<int> CheapCustomer::order(const std::vector<Workout>& workout_options)
{
    std::vector<int> orders;
    int min = workout_options[0].getPrice();
    for (Workout w : workout_options) {
        if (w.getPrice() < min) {
            min = w.getPrice();
        }
    }
    for (Workout w : workout_options) {
        if (w.getPrice() == min) {
            orders.push_back(w.getId());
            break;
        }
    }
    return orders;
}

std::string CheapCustomer::toString() const
{
    return getName() + ",chp";
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout>& workout_options)
{
    std::vector<int> orders;
    for (Workout w : workout_options) {
        if (w.getType() == 0) {
            orders.push_back(w.getId());
        }
    }
    return orders;
}

std::string HeavyMuscleCustomer::toString() const
{
    return getName() + ",mcl";
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name, id){}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout>& workout_options)
{
    std::vector<int> orders;
    std::vector<Workout> cardio;
    std::vector<Workout> mix;
    std::vector<Workout> anaerobic;
    int minCardio;
    int maxMix;
    int minAnaerobic;
    for (Workout w : workout_options) {
        if (w.getType() == 0) {
            anaerobic.push_back(w);
        }
        else if (w.getType() == 1) {
            mix.push_back(w);
        }
        else
        {
            cardio.push_back(w);
        }
    }
    minCardio = cardio[0].getPrice();
    for (Workout w : cardio) {
        if (w.getPrice() < minCardio)
            minCardio = w.getPrice();
    }
    maxMix = mix[0].getPrice();
    for (Workout w : mix) {
        if (w.getPrice() > maxMix)
            maxMix = w.getPrice();
    }
    minAnaerobic = anaerobic[0].getPrice();
    for (Workout w : anaerobic) {
        if (w.getPrice() < minCardio)
            minCardio = w.getPrice();
    }
    for (Workout w : workout_options) {
        if (w.getPrice() == minCardio && w.getType()==2) {
            orders.push_back(w.getId());
            break;
        }
    }
    for (Workout w : workout_options) {
        if (w.getPrice() == maxMix && w.getType() == 1) {
            orders.push_back(w.getId());
            break;
        }
    }
    for (Workout w : workout_options) {
        if (w.getPrice() == minAnaerobic && w.getType() == 0) {
            orders.push_back(w.getId());
            break;
        }
    }
    return orders;
}

std::string FullBodyCustomer::toString() const
{
    return getName()+",fbd";
}
