#include "../include/Studio.h"
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>

using namespace std;


Studio::Studio():open(false),trainers_num(0),customer_num(0)
{
}

Studio::Studio(const std::string& configFilePath) : open(false), trainers_num(0), customer_num(0)
{
    int workout_num = 0;
    std::string input;
    int index = 1;
    std::ifstream reader;
    reader.open(configFilePath);
    if(!reader){
        cout<<"Unable to open file"<<endl;
        exit(1);
    }
    std::vector<int> capacities;
    while (std::getline(reader,input)) {
        if(input.empty()) {break;}
        if (index == 1) {
            stringstream num(input);
            num >> trainers_num;
        }
        else if (index == 2) {
            std::string delimiter = ",";
            size_t pos;
            int capacity = 0;
            pos = input.find(delimiter);
            std::string name = input.substr(0, pos);
            while ((pos = input.find(delimiter)) != std::string::npos) {
                stringstream cap(input.substr(0, pos));
                cap >> capacity;
                capacities.push_back(capacity);
                input.erase(0, pos + delimiter.length());
            }
            stringstream cap(input);
            cap >> capacity;
            capacities.push_back(capacity);
        }
        else{
            string checkType;
            std::string name;
            WorkoutType type;
            int price=0;
            std::string delimiter = ", ";
            size_t pos;
            pos = input.find(delimiter);
            name = input.substr(0, pos);
            input.erase(0, pos + delimiter.length());
            pos = input.find(delimiter);
            checkType = input.substr(0, pos);
            if (checkType == "Anaerobic") { type = ANAEROBIC; }
            else if (checkType == "Cardio") { type = CARDIO; }
            else { type = MIXED; }
            input.erase(0, pos + delimiter.length());
            stringstream p(input);
            p >> price;
            Workout w(workout_num, name, price, type);
            workout_options.push_back(Workout(workout_num, name, price, type));
            workout_num++;
        }
        index++;
    }
    reader.close();
    for (int i = 0;i < trainers_num;i++) {
        Trainer* t = new Trainer(i, capacities[i]);
        trainers.push_back(t);
    }
}

void Studio::start()
{
    open = true;
    std::string input;
    while (open) {
        cout<<"Enter your command:"<<endl;
        std::getline(cin,input);
        inputs.push_back(input);
        vector<string> splits = splitter(input);
        if (splits[0] == "open") {
            std::vector<Customer*> customers;
            stringstream id(splits[1]);
            int t_id = 0;
            id >> t_id;
            for (int i = 2;i < splits.size();i++) {
                customers.push_back(createCustomer(splits[i]));
            }
            OpenTrainer* o_t = new OpenTrainer(t_id, customers);
            o_t->act(*this);
            actionsLog.push_back(o_t);
        }
        else if (splits[0] == "order") {
            int t_id = 0;
            stringstream id(splits[1]);
            id >> t_id;
            Order* o = new Order(t_id);
            o->act(*this);
            actionsLog.push_back(o);
        }
        else if (splits[0] == "move") {
            stringstream t1(splits[1]);
            stringstream t2(splits[2]);
            stringstream c(splits[3]);
            int t1_id = 0;
            int t2_id = 0;
            int c_id = 0;
            t1 >> t1_id;
            t2 >> t2_id;
            c >> c_id;
            MoveCustomer* move = new MoveCustomer(t1_id, t2_id, c_id);
            move->act(*this);
            actionsLog.push_back(move);
        }
        else if (splits[0] == "close") {
            stringstream t(splits[1]);
            int t_id = 0;
            t >> t_id;
            Close* c = new Close(t_id);
            c->act(*this);
            actionsLog.push_back(c);
        }
        else if (splits[0] == "closeall") {
            CloseAll* close = new CloseAll();
            close->act(*this);
            actionsLog.push_back(close);
            open = false;
        }
        else if (splits[0] == "workout_options") {
            PrintWorkoutOptions* w = new PrintWorkoutOptions();
            w->act(*this);
            actionsLog.push_back(w);
        }
        else if (splits[0] == "status") {
            stringstream t(splits[1]);
            int t_id = 0;
            t >> t_id;
            PrintTrainerStatus* print = new PrintTrainerStatus(t_id);
            print->act(*this);
            actionsLog.push_back(print);
        }
        else if (splits[0] == "log") {
            PrintActionsLog* print = new PrintActionsLog();
            print->act(*this);
            actionsLog.push_back(print);
        }
        else if (splits[0] == "backup") {
            BackupStudio* b = new BackupStudio();
            b->act(*this);
            actionsLog.push_back(b);
        }
        else if (splits[0] == "restore") {
            RestoreStudio* b = new RestoreStudio();
            b->act(*this);
            actionsLog.push_back(b);
        }
        else cout<<"Command does not exist, try again";
    }
}

std::vector<Trainer*> Studio::getTrainersList()
{
    return trainers;
}

Trainer* Studio::getTrainer(int tid)
{
    if(tid<trainers.size())
        return trainers[tid];
    else return nullptr;
}

const std::vector<BaseAction*>& Studio::getActionsLog() const
{
    return actionsLog;
}

std::vector<Workout>& Studio::getWorkoutOptions()
{
    return workout_options;
}

std::vector<std::string> Studio::splitter(std::string s)
{
    vector<string> splits;
    std::string delimiter = " ";
    size_t pos;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        splits.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    token = s;
    splits.push_back(token);
    return splits;
}

Customer* Studio::createCustomer(std::string c)
{
    vector<string> splits;
    std::string delimiter = ",";
    size_t pos;
    pos = c.find(delimiter);
    std::string name=c.substr(0, pos);
    std::string type = c.substr(pos+delimiter.length(), c.length());
    if (type == "swt") {
        SweatyCustomer* customer = new SweatyCustomer(name, customer_num);
        customer_num++;
        return customer;
    }
    else if (type == "chp") {
        CheapCustomer* customer = new CheapCustomer(name, customer_num);
        customer_num++;
        return customer;
    }
    else if (type == "mcl") {
        HeavyMuscleCustomer* customer = new HeavyMuscleCustomer(name, customer_num);
        customer_num++;
        return customer;
    }
    else
    {
        FullBodyCustomer* customer = new FullBodyCustomer(name, customer_num);
        customer_num++;
        return customer;
    }
}

void Studio::setCustomerNum(int size) {
    customer_num = size;
}

int Studio::getCustomerNum() const {
    return customer_num;
}

void Studio::CloseStudio() {
    open = false;
}

std::vector<std::string> Studio::getInputs() const {
    return inputs;
}

Studio::~Studio() {
    clear();
}

Studio::Studio(const Studio &other) : open(other.open), trainers_num(other.trainers_num),customer_num(other.customer_num)
{
    for(Trainer* trainer:other.trainers){
        Trainer* t = new Trainer(*trainer);
        this->trainers.push_back(t);
    }
    for(Workout w:other.workout_options){
        this->workout_options.push_back(w);
    }
    for(const std::string& input: other.inputs){
        this->inputs.push_back(input);
    }
    for(BaseAction* a: other.actionsLog){
        this->actionsLog.push_back(a);
    }

}

Studio::Studio(Studio &&other):open(other.open),trainers_num(other.trainers_num),customer_num(other.customer_num) {

}

Studio &Studio::operator=(const Studio &other) {
    clear();
    open = other.open;
    trainers_num = other.trainers_num;
    customer_num = other.customer_num;
    for(Trainer* trainer:other.trainers){
        Trainer* t = new Trainer(*trainer);
        this->trainers.push_back(t);
    }
    for(Workout w:other.workout_options){
        this->workout_options.push_back(w);
    }
    for(const std::string& input: other.inputs){
        this->inputs.push_back(input);
    }
    for(BaseAction* a: other.actionsLog){
        this->actionsLog.push_back(a);
    }
    return *this;
}

Studio &Studio::operator=(Studio &&other) {
    clear();
    return *this;
}

void Studio::clear() {
    trainers_num=0;
    open = false;
    customer_num = 0;
    for(Trainer* t: trainers){
        delete[] t;
    }
    trainers.clear();
    workout_options.clear();
    inputs.clear();
    actionsLog.clear();
}
