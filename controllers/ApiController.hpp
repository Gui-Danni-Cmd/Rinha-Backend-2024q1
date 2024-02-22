#pragma once 
#include <iostream>
#include <chrono>
#include <drogon/HttpController.h>
#include <string.h>
#include "../lib/json.hpp"
#include "./Account.hpp"
#include "./Trasaction.hpp"
#include <stdint.h>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <unordered_map> 


using namespace std::chrono_literals;
using json = nlohmann::json;

class ApiController : public drogon::HttpController<ApiController> {
        public:
        METHOD_LIST_BEGIN   
        ADD_METHOD_TO(ApiController::postTrasaction, "/clientes/{id}/trasaction", drogon::Post);
        ADD_METHOD_TO(ApiController::account, "/cliente/{id}/extrato", drogon::Get);
        METHOD_LIST_END
        std::unordered_map < int8_t, Account > _Account;
        //void asyncHandleHttpRequest(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
        ApiController() {
            _Account.insert({1, Account( 100'000 )});
            _Account.insert({2, Account( 80'000 )});
            _Account.insert({3, Account( 1'000'000 )});
            _Account.insert({4, Account( 10'000'000 )});
            _Account.insert({5, Account( 500'000 )});
            
        }
        Json::Value createJson( int8_t userId );
        void postTrasaction( const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int userId );
        void account( const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int userId );
};

/*
id	limite	                saldo inicial
1	100000	                0
2	80000	                0
3	1000000	                0
4	10000000	            0
5	500000	                0
*/

Json::Value ApiController::createJson( int8_t userId ) {
    Json::Value jsonData;
    Json::Reader jsonReader;
    std::stringstream ss;
    time_t now_time = time(0);
    tm *data = localtime(&now_time);

    ss << "{";
    ss << "\"saldo\": {";
    ss << "\"total\": " << _Account[userId].getValueBalance() << ",";
    ss << "\"data_extrato\": \"" << std::put_time(data , "%Y-%m-%dT%H:%M:%SZ")<< "\",";
    ss << "\"limite\": " << _Account[userId].getValueLimit();
    ss << "},";
    ss << "\"ultimas_transacoes\": [";
    for ( auto i = 0; i < _Account[userId].trasaction->size(); i++ ){
        ss << "{";
        ss << "\"valor\": " << _Account[userId].trasaction->at(i).getValue() << ",";
        ss << "\"tipo\": \"" << ((_Account[userId].trasaction->at(i).getTrasactionType() == 0) ? "d":"c") << "\",";
        ss << "\"descricao\": \"" << _Account[userId].trasaction->at(i).getDescrition() << "\",";
        ss << "\"realizada_em\": \"" <<  std::put_time(&_Account[userId].trasaction->at(i).Create_date, "%Y-%m-%dT%H:%M:%SZ") << "\"";
        (_Account[userId].trasaction->size()-1 == i) ? ss << "}" : ss << "},";
    }
    ss << "]";
    ss << "}";
    jsonReader.parse(ss.str(), jsonData);
    return jsonData;
}

void ApiController::postTrasaction( const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int userId) {
    auto jsonReq = req->getJsonObject();
    if( jsonReq ) {
        if( ( *jsonReq )["tipo"].isString() && ( *jsonReq )["tipo"].asString() == "c" ) {
            _Account[userId].CreateTrasaction(stoi( ( *jsonReq )["valor"].asString()),Credit ,( *jsonReq )["descricao"].asString() );
            _Account[userId].DecrementBalance(stoi(( *jsonReq )["valor"].asString()));
        } else if( ( *jsonReq )["tipo"].isString() && ( *jsonReq )["tipo"].asString() == "d" ) {
            _Account[userId].CreateTrasaction(stoi( ( *jsonReq )["valor"].asString()),Debit ,( *jsonReq )["descricao"].asString() );
            _Account[userId].DecrementBalance(stoi(( *jsonReq )["valor"].asString()));
        }
    } 
    Json::Value json = createJson(userId);
    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
    

    std::cout << _Account[1].getValueBalance() << " " << userId << '\n';
    resp->setBody("Return Trasação! \n");
    callback(resp);
}

void ApiController::account( const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int userId ) {
    Json::Value json = createJson(userId);
    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
    time_t now = time(0);
    tm *ltm = localtime(&now);
    auto it = _Account.find(userId);
    std::cout << " " << userId << " " << ltm->tm_min << '\n';
    if ( it != _Account.end()) {
        resp->setStatusCode(drogon::HttpStatusCode::k200OK);
        resp->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
        resp->setBody("Valor encontrado! \n");
    }else {
        resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        resp->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
        resp->setBody("Valor nao encontrado!\n");
    }
    callback(resp);
}

