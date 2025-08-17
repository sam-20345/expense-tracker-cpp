# Expense Tracker (C++17, CLI)

A simple, resume-friendly **Expense Tracker** written in **modern C++ (C++17)**.  
No external dependencies; data is stored in a CSV file. Includes a clean CLI, monthly summaries, category breakdowns, and ASCII bar charts.

## ✨ Features
- Add income and expense transactions
- List transactions (optionally filter by month)
- Monthly summary (income, expense, net)
- Category breakdown with ASCII bar chart
- Edit or delete transactions by ID
- Persistent CSV storage (`data/transactions.csv`)
- Cross-platform (Linux/macOS/Windows via MinGW)
- Clean OOP design and modular code

## 📦 Project Structure
```
expense-tracker-cpp/
├─ include/
│  ├─ Transaction.hpp
│  ├─ ExpenseTracker.hpp
│  └─ Utils.hpp
├─ src/
│  ├─ main.cpp
│  ├─ ExpenseTracker.cpp
│  └─ Utils.cpp
├─ data/
│  └─ transactions.csv   (auto-created on first run)
├─ tests/
│  └─ sample_input.txt
├─ .gitignore
├─ LICENSE
├─ Makefile
└─ README.md
```

## 🚀 Build
Requires a C++17 compiler.

```bash
# Linux / macOS
make

# Windows (MSYS2/MinGW):
# Ensure g++ is available, then run:
make
```

The binary will be created at `bin/expense_tracker`.

## ▶️ Run
```bash
./bin/expense_tracker
```

You will see a menu-driven interface.

### Optional CLI flags
```bash
# Add a quick expense (negative amount) or income (positive)
./bin/expense_tracker add --date 2025-08-17 --desc "Coffee" --cat Food --amount -120.50

# List all transactions (or filter by month)
./bin/expense_tracker list
./bin/expense_tracker list --month 2025-08

# Summary for a month (income, expense, net)
./bin/expense_tracker summary --month 2025-08

# Category breakdown for a month with ASCII bars
./bin/expense_tracker categories --month 2025-08

# Edit / delete by ID
./bin/expense_tracker edit --id 3 --amount -250.00
./bin/expense_tracker delete --id 3
```

## 🧪 Quick Demo Data
You can paste the following into the app (or directly into `data/transactions.csv` after the header):

```
id,date,description,category,amount
1,2025-08-01,Salary,Income,55000
2,2025-08-02,Coffee,Food,-120.5
3,2025-08-03,Groceries,Food,-1324.0
4,2025-08-04,Metro Pass,Transport,-400.0
5,2025-08-05,Freelance,Income,3500
6,2025-08-05,Movie,Entertainment,-300
```

## 🧱 Design Notes (for your resume)
- **OOP:** `Transaction` (value type) + `ExpenseTracker` (stateful manager class).
- **Persistence:** CSV read/write with robust parsing and escaping.
- **CLI:** Lightweight hand-rolled parser supporting subcommands/flags.
- **Algorithmic bits:** Monthly filters, category aggregation, ASCII visualization.
- **Reliability:** Input validation, ID management, safe file writes.

## 📚 Future Extensions
- Export reports to JSON
- Unit tests (Catch2/GoogleTest)
- Category budgets and alerts
- Multi-user profiles
- Simple TUI (ncurses) or GUI (Qt/SFML)

## 📝 License
MIT — do whatever you want, just keep the copyright notice.

---

**Author:** Samarth Rai  
**GitHub:** https://github.com/sam-20345

Feel free to star the repo if you use it 💙
