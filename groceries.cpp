#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "split.h"
#include <iomanip>

using namespace std;

class Customer {
public:
    int id;
    string name;
    string street;
    string city;
    string state;
    string zip;
    string phone;
    string email;
};

class Item {
public:
    int id;
    string description;
    double price;
};

vector<Customer> customers;
vector<Item> items;

void read_customers(string filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: could not open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        vector<string> fields = split(line, ',');
        if (fields.size() != 8) {
            cerr << "Error: invalid customer record: " << line << endl;
            exit(1);
        }

        Customer customer;
        customer.id = stoi(fields[0]);
        customer.name = fields[1];
        customer.street = fields[2];
        customer.city = fields[3];
        customer.state = fields[4];
        customer.zip = fields[5];
        customer.phone = fields[6];
        customer.email = fields[7];

        customers.push_back(customer);
    }

    file.close();
}

void read_items(string filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: could not open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        vector<string> fields = split(line, ',');
        if (fields.size() != 3) {
            cerr << "Error: invalid item record: " << line << endl;
            exit(1);
        }

        Item item;
        item.id = stoi(fields[0]);
        item.description = fields[1];
        item.price = stod(fields[2]);

        items.push_back(item);
    }

    file.close();
}

int find_cust_idx(int cust_id) {
    for (int i = 0; i < customers.size(); i++) {
        if (customers[i].id == cust_id) {
            return i;
        }
    }

    return -1;
}

int find_item_idx(int item_id) {
    for (int i = 0; i < items.size(); i++) {
        if (items[i].id == item_id) {
            return i;
        }
    }

    return -1;
}

void one_customer_order() {
    int cust_id;
    cout << "Enter customer number: ";
    cin >> cust_id;

    int cust_idx = find_cust_idx(cust_id);
    if (cust_idx == -1) {
        cerr << "Error: customer not found" << endl;
        exit(1);
    }

    double total_cost = 0.0;
    int num_items = 0;

    while (true) {
        int item_id;
        cout << "Enter item number (0 to exit): ";
        cin >> item_id;

        if (item_id == 0) {
            break;
        }

        int item_idx = find_item_idx(item_id);
        if (item_idx == -1) {
            cerr << "Error: item not found" << endl;
            continue;
        }

        Item item = items[item_idx];
        cout << item.description << " - $" << item.price << endl;

        total_cost += item.price;
        num_items++;
    }

    cout << "Number of items purchased: " << num_items << endl;
    cout << "Total cost: $" << fixed << setprecision(2) << total_cost << endl;
}

int main() {
    read_customers("customers.txt");
    read_items("items.txt");

    cout << "Number of customers: " << customers.size() << endl;
    cout << "Number of items: " << items.size() << endl;

    one_customer_order();

    return 0;
}
