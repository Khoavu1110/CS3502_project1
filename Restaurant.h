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
            std::lock_guard<std::mutex> lock(queueMutex);
            orderQueue.push(order);             // Add order to the queue
            orderCondition.notify_one();        // Notifify one waiting chef
        }


        /*
         * getOrder:    Return the first order in the order queue if exist
                        Else return an empty string
         */
        std::string getOrder(){
            /*
             * Note:    std::unique_lock<std::mutex> lock(queueMutex);
             *  This will creates a unique_lock object named lock that immediately acquires (lock) the queueMutex
             *  This ensures that while the lock object is in scope, the mutex is held, preventing other threads from 
             *  Entering the critical section that accesses the shared resource (order queue)
             *  When lock goes out of scope, its destructor automatically release the mutex
             */
            std::unique_lock<std::mutex> lock(queueMutex);

            // Wait until there is an order in the queue or the restaurant is closed
            /*
             * Note: The thread remains blocked until either there is an order to process or
             * the restaurant has been closed. When either condition is true, the wait is over and the 
             * thread continues --> Preventing the thread from continuously checking
             */
            orderCondition.wait(lock, [this]() {return !orderQueue.empty() || !open;});

            // If ther is an order, retrieve it; else, return an empty string
            if (!orderQueue.empty()){
                std::string order = orderQueue.front();
                orderQueue.pop();
                return order;
            }
            else{
                return "";
            }
        }
};