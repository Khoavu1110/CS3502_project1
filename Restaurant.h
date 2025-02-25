/*
 * Manages orders, chefs, and waiters.
 * ✅ Attributes:
 *  std::queue<std::string> orderQueue; → Stores pending orders.
 *  std::mutex queueMutex; → Protects orderQueue from race conditions.
 *  std::condition_variable orderCondition; → Notifies chefs when orders are ready.
 *  std::vector<std::thread> waiters; → Threads handling customers' orders.
 *  std::vector<std::thread> chefs; → Threads preparing food.
 *  bool open; → Controls when the restaurant is open/closed.

 * ✅ Functions:
 *  void openRestaurant(int numWaiters, int numChefs); → Starts the restaurant with given staff.
 *  void closeRestaurant(); → Stops all operations.
 *  void addOrder(const std::string& order); → Adds an order to the queue.
 *  std::string getOrder(); → Waits for an order & returns it to a chef.
 *  void runWaiter(int waiterID); → Simulates a waiter handling customers.
 *  void runChef(int chefID); → Simulates a chef preparing food.
 */

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

class Restaurant{
    private:
        std::queue<std::string> orderQueue;
        std::mutex queueMutex;
        std::condition_variable orderCondition;
        std::vector <std::thread> waiters;
        std::vector <std::thread> chefs;
        bool open;
    
    public:
        void openRestaurant(int numberWaiters, int numberChefs){
            
        }
};