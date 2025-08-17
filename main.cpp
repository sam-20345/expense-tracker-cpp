#include "ExpenseTracker.hpp"
#include "Utils.hpp"
#include <iostream>
#include <iomanip>
#include <filesystem>

static void printMenu(){
    std::cout << "\n=== Expense Tracker ===\n"
              << "1) Add transaction\n"
              << "2) List all\n"
              << "3) List by month (YYYY-MM)\n"
              << "4) Monthly summary (YYYY-MM)\n"
              << "5) Category breakdown (YYYY-MM)\n"
              << "6) Edit by ID\n"
              << "7) Delete by ID\n"
              << "0) Exit\n"
              << "Choice: ";
}

static void printTransaction(const Transaction& t){
    std::cout << std::left << std::setw(4) << t.id << "  "
              << std::setw(10) << t.date << "  "
              << std::setw(15) << t.category << "  "
              << std::setw(30) << t.description << "  "
              << std::right << std::fixed << std::setprecision(2) << t.amount << "\n";
}

int main(int argc, char** argv){
    std::string dataDir = "data";
    std::filesystem::create_directories(dataDir);
    ExpenseTracker tracker(dataDir + std::string("/transactions.csv"));
    tracker.load();

    // CLI subcommands
    if(argc >= 2){
        std::string cmd = argv[1];
        auto flags = Utils::parseFlags(argc, argv, 2);

        if(cmd=="add"){
            std::string date = flags.count("date") ? flags["date"] : "";
            std::string desc = flags.count("desc") ? flags["desc"] : "";
            std::string cat  = flags.count("cat")  ? flags["cat"]  : "General";
            double amount = flags.count("amount") ? std::stod(flags["amount"]) : 0.0;
            if(!Utils::isValidDate(date) || desc.empty()){
                std::cerr << "Usage: add --date YYYY-MM-DD --desc \"text\" --cat Category --amount +/-value\n";
                return 1;
            }
            int id = tracker.add(date, desc, cat, amount);
            std::cout << "Added ID " << id << "\n";
            return 0;
        } else if(cmd=="list"){
            if(flags.count("month")){
                std::string ym = flags["month"];
                if(!Utils::isValidYearMonth(ym)){
                    std::cerr << "Invalid month. Use YYYY-MM\n"; return 1;
                }
                auto v = tracker.listByMonth(ym);
                for(const auto& t: v) printTransaction(t);
            }else{
                auto v = tracker.listAll();
                for(const auto& t: v) printTransaction(t);
            }
            return 0;
        } else if(cmd=="summary"){
            std::string ym = flags.count("month") ? flags["month"] : "";
            if(!Utils::isValidYearMonth(ym)){ std::cerr << "summary --month YYYY-MM\n"; return 1; }
            double inc, exp, net;
            tracker.monthlySummary(ym, inc, exp, net);
            std::cout << "Summary for " << ym << ":\n";
            std::cout << "  Income : " << std::fixed << std::setprecision(2) << inc << "\n";
            std::cout << "  Expense: " << exp << "\n";
            std::cout << "  Net    : " << net << "\n";
            return 0;
        } else if(cmd=="categories"){
            std::string ym = flags.count("month") ? flags["month"] : "";
            if(!Utils::isValidYearMonth(ym)){ std::cerr << "categories --month YYYY-MM\n"; return 1; }
            auto totals = tracker.categoryTotals(ym);
            std::cout << "Category totals for " << ym << ":\n";
            Utils::printBarChart(totals);
            return 0;
        } else if(cmd=="edit"){
            int id = flags.count("id") ? std::stoi(flags["id"]) : -1;
            if(id<0){ std::cerr << "edit --id N [--date YYYY-MM-DD] [--desc text] [--cat str] [--amount val]\n"; return 1; }
            std::string date, desc, cat;
            double amount; bool hasAmount=false;
            const std::string* dPtr=nullptr; const std::string* dsPtr=nullptr; const std::string* cPtr=nullptr; const double* aPtr=nullptr;
            if(flags.count("date")){ date = flags["date"]; if(!Utils::isValidDate(date)){ std::cerr<<"Invalid date\n"; return 1;} dPtr=&date; }
            if(flags.count("desc")){ desc = flags["desc"]; dsPtr=&desc; }
            if(flags.count("cat")) { cat  = flags["cat"]; cPtr=&cat; }
            if(flags.count("amount")){ amount = std::stod(flags["amount"]); hasAmount=true; aPtr=&amount; }
            if(tracker.edit(id, dPtr, dsPtr, cPtr, hasAmount? aPtr: nullptr)){
                std::cout << "Edited ID " << id << "\n";
            } else {
                std::cout << "ID not found\n";
            }
            return 0;
        } else if(cmd=="delete"){
            int id = flags.count("id") ? std::stoi(flags["id"]) : -1;
            if(id<0){ std::cerr << "delete --id N\n"; return 1; }
            if(tracker.remove(id)) std::cout << "Deleted ID " << id << "\n";
            else std::cout << "ID not found\n";
            return 0;
        }
        // fall through to menu on unknown command
    }

    // Interactive menu
    while(true){
        printMenu();
        int ch; 
        if(!(std::cin >> ch)){ std::cout << "\n"; break; }
        if(ch==0) break;
        if(ch==1){
            std::string date, desc, cat; double amount;
            std::cout << "Date (YYYY-MM-DD): "; std::cin >> date;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if(!Utils::isValidDate(date)){ std::cout << "Invalid date.\n"; continue; }
            std::cout << "Description: "; std::getline(std::cin, desc);
            std::cout << "Category: "; std::getline(std::cin, cat); if(cat.empty()) cat="General";
            std::cout << "Amount (+income / -expense): "; std::cin >> amount;
            int id = tracker.add(date, desc, cat, amount);
            std::cout << "Added with ID " << id << "\n";
        }else if(ch==2){
            auto v = tracker.listAll();
            for(const auto& t: v) printTransaction(t);
        }else if(ch==3){
            std::string ym; std::cout << "Month (YYYY-MM): "; std::cin >> ym;
            if(!Utils::isValidYearMonth(ym)){ std::cout << "Invalid.\n"; continue; }
            auto v = tracker.listByMonth(ym);
            for(const auto& t: v) printTransaction(t);
        }else if(ch==4){
            std::string ym; std::cout << "Month (YYYY-MM): "; std::cin >> ym;
            if(!Utils::isValidYearMonth(ym)){ std::cout << "Invalid.\n"; continue; }
            double inc, exp, net; tracker.monthlySummary(ym, inc, exp, net);
            std::cout << "Income: " << inc << "  Expense: " << exp << "  Net: " << net << "\n";
        }else if(ch==5){
            std::string ym; std::cout << "Month (YYYY-MM): "; std::cin >> ym;
            if(!Utils::isValidYearMonth(ym)){ std::cout << "Invalid.\n"; continue; }
            auto totals = tracker.categoryTotals(ym);
            Utils::printBarChart(totals);
        }else if(ch==6){
            int id; std::cout << "ID: "; std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string date, desc, cat; double amount; 
            std::cout << "New date (YYYY-MM-DD, blank to skip): "; std::getline(std::cin, date);
            std::cout << "New description (blank to skip): "; std::getline(std::cin, desc);
            std::cout << "New category (blank to skip): "; std::getline(std::cin, cat);
            std::string amountStr; std::cout << "New amount (blank to skip): "; std::getline(std::cin, amountStr);

            const std::string* dPtr=nullptr; const std::string* dsPtr=nullptr; const std::string* cPtr=nullptr; const double* aPtr=nullptr;
            if(!date.empty()){ if(!Utils::isValidDate(date)){ std::cout<<"Invalid date\n"; continue;} dPtr=&date; }
            if(!desc.empty()){ dsPtr=&desc; }
            if(!cat.empty()){ cPtr=&cat; }
            if(!amountStr.empty()){ try{ amount = std::stod(amountStr); aPtr=&amount; }catch(...){ std::cout<<"Invalid amount\n"; continue;} }
            if(tracker.edit(id, dPtr, dsPtr, cPtr, aPtr)) std::cout << "Edited.\n"; else std::cout << "ID not found.\n";
        }else if(ch==7){
            int id; std::cout << "ID: "; std::cin >> id;
            if(tracker.remove(id)) std::cout << "Deleted.\n"; else std::cout << "ID not found.\n";
        }else{
            std::cout << "Unknown option.\n";
        }
    }

    return 0;
}
