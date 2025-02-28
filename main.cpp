#include <iostream>
#include "Restaurant.h"

int main(){
    Restaurant restaurant;
    restaurant.openRestaurant(5, 5);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    restaurant.closeRestaurant();
    return 0;
}