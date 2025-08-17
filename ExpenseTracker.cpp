#include "ExpenseTracker.hpp"
#include "Utils.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

ExpenseTracker::ExpenseTracker(const std::string& csvPath): path(csvPath) {}

bool ExpenseTracker::load(){
    items.clear();
    std::ifstream in(path);
    if(!in.good()){
        // create file with header
        std::ofstream out(path);
        out << "id,date,description,category,amount\n";
        out.close();
        nextId = 1;
        return true;
    }
    std::string line;
    bool first=true;
    while(std::getline(in,line)){
        if(first){ first=false; continue; } // skip header
        if(line.empty()) continue;
        auto cols = Utils::splitCSVLine(line);
        if(cols.size()<5) continue;
        Transaction t;
        try{
            t.id = std::stoi(cols[0]);
            t.date = Utils::unescapeCSV(cols[1]);
            t.description = Utils::unescapeCSV(cols[2]);
            t.category = Utils::unescapeCSV(cols[3]);
            t.amount = std::stod(cols[4]);
            items.push_back(t);
        }catch(...){
            // skip malformed row
        }
    }
    in.close();
    updateNextId();
    return true;
}

bool ExpenseTracker::save() const{
    std::ofstream out(path, std::ios::trunc);
    if(!out.good()) return false;
    out << "id,date,description,category,amount\n";
    for(const auto& t: items){
        out << t.id << ","
            << Utils::escapeCSV(t.date) << ","
            << Utils::escapeCSV(t.description) << ","
            << Utils::escapeCSV(t.category) << ","
            << std::fixed << std::setprecision(2) << t.amount << "\n";
    }
    return true;
}

int ExpenseTracker::add(const std::string& date, const std::string& desc, const std::string& category, double amount){
    Transaction t;
    t.id = nextId++;
    t.date = date;
    t.description = desc;
    t.category = category;
    t.amount = amount;
    items.push_back(t);
    save();
    return t.id;
}

bool ExpenseTracker::edit(int id, const std::string* date, const std::string* desc, const std::string* category, const double* amount){
    for(auto& t: items){
        if(t.id == id){
            if(date) t.date = *date;
            if(desc) t.description = *desc;
            if(category) t.category = *category;
            if(amount) t.amount = *amount;
            save();
            return true;
        }
    }
    return false;
}

bool ExpenseTracker::remove(int id){
    auto oldSize = items.size();
    items.erase(std::remove_if(items.begin(), items.end(), [&](const Transaction& t){ return t.id==id; }), items.end());
    bool changed = items.size()!=oldSize;
    if(changed) save();
    return changed;
}

std::vector<Transaction> ExpenseTracker::listAll() const{
    auto cp = items;
    std::sort(cp.begin(), cp.end(), [](const Transaction& a, const Transaction& b){
        if(a.date==b.date) return a.id < b.id;
        return a.date < b.date;
    });
    return cp;
}

std::string ExpenseTracker::toYearMonth(const std::string& date){
    if(date.size()>=7) return date.substr(0,7);
    return "";
}

std::vector<Transaction> ExpenseTracker::listByMonth(const std::string& yearMonth) const{
    std::vector<Transaction> out;
    for(const auto& t: items){
        if(toYearMonth(t.date)==yearMonth) out.push_back(t);
    }
    std::sort(out.begin(), out.end(), [](const Transaction& a, const Transaction& b){
        if(a.date==b.date) return a.id < b.id;
        return a.date < b.date;
    });
    return out;
}

std::unordered_map<std::string,double> ExpenseTracker::categoryTotals(const std::string& yearMonth) const{
    std::unordered_map<std::string,double> totals;
    for(const auto& t: items){
        if(toYearMonth(t.date)==yearMonth){
            totals[t.category] += t.amount;
        }
    }
    return totals;
}

void ExpenseTracker::monthlySummary(const std::string& yearMonth, double& income, double& expense, double& net) const{
    income = expense = 0.0;
    for(const auto& t: items){
        if(toYearMonth(t.date)==yearMonth){
            if(t.amount >= 0) income += t.amount;
            else expense += -t.amount;
        }
    }
    net = income - expense;
}

void ExpenseTracker::updateNextId(){
    int maxId = 0;
    for(const auto& t: items) maxId = std::max(maxId, t.id);
    nextId = maxId + 1;
}
