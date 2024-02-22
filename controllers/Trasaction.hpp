#pragma once 
#include <time.h>
#include <string.h>
#include <ctime>

enum TrasactionType {
    Debit,
    Credit
};

std::string timeToString(time_t time) {
    char buffer[20]; // Tamanho suficiente para a data no formato ISO 8601
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&time)); // Formata o tempo como uma string ISO 8601
    return std::string(buffer);
}

class Trasaction {
    private:
    int64_t value;
    TrasactionType type;    
    std::string descrition;
    public:
    struct tm Create_date {0};
    Trasaction( int64_t value, TrasactionType type, std::string descrition ) {
        this->value = value;
        this->type = type;
        this->descrition = descrition;
        time_t now_time = time(0);
        tm *data = localtime(&now_time);
        this->Create_date = *data;
    }
    int64_t getValue();
    TrasactionType getTrasactionType();
    std::string getDescrition();
};

int64_t Trasaction::getValue() {
    return value;
}
TrasactionType Trasaction::getTrasactionType() {
    return type;
}
std::string Trasaction::getDescrition() {
    return descrition;
}
