/*
 * ✅ Attributes (private)
 *  int id; → Unique ID for the waiter.
 *  Restaurant* restaurant; → A pointer to the Restaurant object.
 *  bool running; → Controls when the waiter should stop working.

 * ✅ Functions (public)
 *  Constructor → Initializes the waiter with an ID and a reference to the Restaurant.
 *  void takeOrder(); → Simulates taking an order from a customer.
 *  void deliverOrder(const std::string& order); → Simulates delivering food to a customer.
 *  void run(); → The main loop where the waiter continuously takes and delivers orders.
 */
#include "Restaurant.h"

class Waiter{
    private:
        int id;
        Restaurant* restaurant;
        bool running;
};

