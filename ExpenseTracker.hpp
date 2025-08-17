#pragma once
#include "Transaction.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class ExpenseTracker {
public:
    explicit ExpenseTracker(const std::string& csvPath);

    bool load();
    bool save() const;

    // CRUD
    int add(const std::string& date, const std::string& desc, const std::string& category, double amount);
    bool edit(int id, const std::string* date, const std::string* desc, const std::string* category, const double* amount);
    bool remove(int id);

    // Queries
    std::vector<Transaction> listAll() const;
    std::vector<Transaction> listByMonth(const std::string& yearMonth) const; // YYYY-MM
    std::unordered_map<std::string,double> categoryTotals(const std::string& yearMonth) const;
    void monthlySummary(const std::string& yearMonth, double& income, double& expense, double& net) const;

    // helpers
    static std::string toYearMonth(const std::string& date); // YYYY-MM from YYYY-MM-DD
private:
    std::string path;
    std::vector<Transaction> items;
    int nextId{1};

    void updateNextId();
};
