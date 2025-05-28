# 👟 Sneaker Shop – C Console Application

This is a console-based sneaker shop management system built in pure C. The project simulates essential operations of a retail sneaker store using a modular structure across multiple source and header files. It includes management functionality for customers, employees, and items, all accessible through a clean menu-driven interface.

The system allows viewing and editing of sneaker inventory, handling customer purchases, managing employee records, and more. It does not rely on any external libraries and is designed for educational and practice purposes in C programming.

## 🧾 Features

- Manage sneaker items (add, remove, list, purchase)
- Maintain customer data
- Manage employee records
- Interactive menu system
- Clear modular code across `.c` and `.h` files
- Terminal/command-line based UI

## 📁 Files Included

- `main.c`: Program entry point and main loop
- `menu.c`, `menu.h`: Displays main menu and handles user navigation
- `items.c`, `items.h`: Logic for sneaker items (inventory operations)
- `customers.c`, `customers.h`: Customer management (registration, lookup, etc.)
- `employees.c`, `employees.h`: Employee records and admin logic
- `logo`, `logs`, `customers`, `employees`, `items`: Text-based storage or placeholders for data persistence (likely via `fopen`/`fprintf`)
- `FinalProject`: Possibly a project file for your IDE

## 🛠 Compilation

You can compile all source files together using GCC:

```bash
gcc main.c menu.c items.c customers.c employees.c -o sneaker_shop
./sneaker_shop
