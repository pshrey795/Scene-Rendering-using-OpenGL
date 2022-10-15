#include<bits/stdc++.h>
using namespace std;

//Getting the direction of sunlight based on the time of day
void getSunDir(){
    //Getting current time of day using chrono
    auto currentTime = chrono::system_clock::now();
    //Extract time from currentTime
    time_t time = chrono::system_clock::to_time_t(currentTime);
    //Extract the current hour from time
    int hour = localtime(&time)->tm_hour;
    cout << hour << endl; 
}

int main(){
    getSunDir();
}