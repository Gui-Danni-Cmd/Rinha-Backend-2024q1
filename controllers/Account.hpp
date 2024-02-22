#pragma once 
#include <stdint.h>
#include "./Trasaction.hpp"
#include <vector>


class Account {
    private:
        int64_t limit;
        int64_t balance;
    public:
        std::vector< Trasaction > *trasaction = new std::vector< Trasaction >();
        Account() {
            this->balance = 0;
            this->limit = 0;
        }
        Account( int64_t balance ) {
            this->balance = balance;
            this->limit = 0;
        }
        Account( int64_t balance, int64_t limit ) {
            this->balance = balance;
            this->limit = limit;
        }
        Account( int64_t balance, TrasactionType type, std::string descrition ) {
            this->balance = balance;
            this->limit = limit;
            this->CreateTrasaction(balance, type, descrition);
        }
        int64_t getValueBalance();
        int64_t getValueLimit();
        void AddTrasaction( Trasaction trasaction );
        void CreateTrasaction( int64_t value, TrasactionType type, std::string descrition );
        void DecrementBalance(int64_t value );
};

int64_t Account::getValueBalance() {
    return this->balance;
}

int64_t Account::getValueLimit() {
    return this->limit;
}

void Account::AddTrasaction( Trasaction trasaction ) {
    this->trasaction->push_back(trasaction);
}

void Account::CreateTrasaction( int64_t value, TrasactionType type, std::string descrition ) {
    Trasaction trasaction = Trasaction(value, type, descrition ); 
    this->trasaction->push_back(trasaction);
}

void Account::DecrementBalance(int64_t value ) {
    this->balance -= value;
}
