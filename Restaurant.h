#ifndef RESTAURANT_H
#define RESTAURANT_H

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
        std::mutex foodTrayMutex;                           // Food tray for the chef to put the food on and waiter to bring it out
        std::condition_variable orderCondition;             // Notifies waiting chefs threads when an order is added
        std::vector <std::thread> waiters;                  // Waiters Thread
        std::vector <std::thread> chefs;                    // Chef Thread
        std::mutex printMutex;
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
            /*
             * construct that locks the mutex immediately when the lock_guard is created
             * and automatically releases it when the lock_guard goes out of scope.
             */
            std::lock_guard<std::mutex> lock1(queueMutex);   // Lock mutex to safely access the queue
            orderQueue.push(order);                         // Add order to the queue
            std::cout << "Order added: " << order << std::endl;
            orderCondition.notify_one();                    // Notifify one waiting chef
        }


        /*
         * getOrder: Grab an order from the queue
         */
        std::string getOrder(){
            std::unique_lock<std::mutex> lock1(queueMutex); // Lock the queue

            // Wait until there is an order or the restaurant is close
            if (!orderCondition.wait_for(lock1, std::chrono::seconds(5), [this]() { return !orderQueue.empty() || !open; })) {
                std::cerr << "Chef has been waiting too long for an order!" << std::endl;
                return "";  // Return empty to prevent infinite waiting
            }

            // Grab an order if the queue is not empty
            if (!orderQueue.empty()){
                std::string order = orderQueue.front();
                orderQueue.pop();
                return order;
            }
            // Return an empty string if the queue is empty or the restaurant is closed
            else {
                return "";
            }
        }

        /*
         * runWaiter: while the restaurant is open, the waiter need to take order and add to the restaurant order queue
         * @param: waiterID:    identify which waiter took order
         */
        void runWaiter(int waiterID){
            std::vector<std::string> menu = {"Burger", "Pizza","Sushi","Caesar Salad","Boiled Egg","Frozen Water"};
            while(open){
                std::this_thread::sleep_for(std::chrono::seconds(1+rand()%3));          // Waiter wait for 1-3 sec before taking another order

                std::cout << "Waiter " << waiterID << " is trying to take an order..." << std::endl;
                // Lock queueMutex first to simmulate the adding an order
                {
                    std::lock_guard<std::mutex> lock1(queueMutex);
                    std::this_thread::sleep_for(std::chrono::seconds(1));                   // Processing an order
                    
                }
                {
                // Locking the foodTrayMutex
                std::lock_guard<std::mutex> lock2(foodTrayMutex);
                }
                // Generating a random food item
                std::string foodItem = menu[rand() %menu.size()];
                std::string order = foodItem + " from waiter " + std::to_string(waiterID);   // Generate a unique order based on the waiter's ID
                addOrder(order);                                                        // Add to the restaurant queue

                std::lock_guard<std::mutex> printLock(printMutex);
                std::cout << "Waiter " << waiterID << " took an order " << order << std::endl;
            }
        }

        /*
         * runChef: chef need to grab an order from the queue and cook it while the restaurant is open or there is an order in the queue
         * @param:  chefID: identify which chef cook which order
         */
        void runChef(int chefID){
            std::this_thread::sleep_for(std::chrono::seconds(2));   // Delay the chef so the waiter have time to take order first

            while (open){
                std::string order = getOrder();
                // If the order is empty, break since there are no order or the restaurant is closed
                if(order.empty()){
                    std::cerr << "Chef " << chefID << " exiting: No orders available." << std::endl;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::seconds(2+rand()%4));  // it's cooking time (got this from chat)

                // Locking queueMutex first (sync with the runWaiter)
                std::lock_guard<std::mutex> lock1(queueMutex);
                std::this_thread::sleep_for(std::chrono::seconds(1));

                // Then lock the foodTrayMutex
                std::lock_guard<std::mutex> lock2(foodTrayMutex);

                // Printing out status
                std::lock_guard<std::mutex> printLock(printMutex);                
                std::cout<< "Chef " << chefID << " is prepping " << order << std::endl;
                std::cout<< "Chef " << chefID << " cooked  " << order << std::endl;
            }
        }
};

#endif