#include <iostream>
#include "Airport.h"
#include <iostream>
#include "Route.h"
#include <cmath>
using namespace std;

unordered_map<string, vector<Airport>> Airport::airports = *new unordered_map<string, vector<Airport>>;
unordered_map<string, vector<Route>> Route::routes = *new unordered_map<string, vector<Route>>;
unordered_map<string, vector<Route>> Route::flights = *new unordered_map<string, vector<Route>>;
unordered_map<string, string> Route::parents = *new unordered_map<string, string>;
unordered_map<string, Airport> Airport::codes = *new unordered_map<string, Airport>;

double haversine(double lat1, double lon1,
                 double lat2, double lon2);
int main() {


//    double lat1 = 5.6051898;
//    double lon1 = -0.166786;
//    double lat2 = 51.4706;
//    double lon2 = -0.461941;
//    double lat3 = 39.86169815;
//    double lon3 = -104.6729965;
//    double lat4 = 49.90999985;
//    double lon4 = -97.23989868;
//
//
//    cout << haversine(lat1, lon1,
//                      lat2, lon2) + haversine(lat2, lon2, lat3, lon3) + haversine(lat3, lon3, lat4,lon4)<< " K.M.";

    Route::findRoute("/Users/xander/Documents/OneDrive - Ashesi University/ Academia/2nd Year Sem 2/Intermediate Computer Programming /C++ Individual Project/test.txt");

}

double haversine(double lat1, double lon1,
                        double lat2, double lon2)
{
    // distance between latitudes
    // and longitudes
    double dLat = (lat2 - lat1) *
                  M_PI / 180.0;
    double dLon = (lon2 - lon1) *
                  M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}

