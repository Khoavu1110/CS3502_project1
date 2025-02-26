#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

class Restaurant{
    private:
        std::queue<std::string> orderQueue;                 // Store pending orders
        std::mutex queueMutex;                              // Protect access to the order queue
        std::condition_variable orderCondition;             // Notifies waiting chefs threads when an order is added
        std::vector <std::thread> waiters;                  // Waiters Thread
        std::vector <std::thread> chefs;                    // Chef Thread
        bool open;
    
    public:
        /*
         * openRestaurant: open restaurant with numbers of waiters and chefs
         * @param   numberWaiters:  number of waiter thread
         *          numberChefs:    number of chef thread 
         */
        void openRestaurant(int numberWaiters, int numberChefs){
            open = true;
            // Creating x number of waiters
            for (int i = 0; i != numberWaiters; ++i){
                waiters.emplace_back(&Restaurant::runWaiter, this, i);
            }
            // Creating x number of chefs
            for (int i = 0; i != numberChefs; ++i){
                chefs.emplace_back(&Restaurant::runChef, this, i);
            }
        }

        /*
         * closeRestaurant: close restaurant and join all waiter and chef threads to ensure the 
         *                  main thread is waiting for all worker threads to finish before the program
         *                  exits.
         */

        void closeRestaurant(){
            open = false;                                   // Stop new operations.
            orderCondition.notify_all();                    // Wake up any waiting threads.

            // Join waiter threads
            for (auto &waiter : waiters){
                if (waiter.joinable()) {
                    waiter.join();
                }
            }

            // Join chefs threads
            for (auto &chef : chefs){
                if (chef.joinable()) {
                    chef.join();
                }
            }
        }

        /*
         * addOrder: Add order to the queue and notifies the chefs that they have an order
         * @param   order: A const reference to a string representing the order to be added
         */
        void addOrder(const std::string &order){
            std::lock_guard<std::mitex> lock(queueMutex);
            orderQueue.push(order);             // Add order to the queue
            orderCondition.notify_one();        // Notifify one waiting chef
        }
};