// This file represents a route file which computes the routes from one airport/airline location to another.
#include "route.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

/**
 *This is a route constructor for the route class
 *
 * @param airlineIata the airline iata code
 * @param airlineId the airline id
 * @param sourceIata the iata code of the starting airport location
 * @param sourceId the id of the starting airport location
 * @param destIata the iata code of the destination airport location
 * @param destId the destination id of the destination airport location
 * */

route::route(string airlineIata, string airlineId, string sourceIata, string sourceId, string destIata, string destId) {
    this->airlineIata = airlineIata;
    this->airlineId = airlineId;
    this->sourceIata = sourceIata;
    this->sourceId = sourceId;
    this->destIata = destIata;
    this->destId = destId;
}
/**
 *   A function to read the airport file and store each airport in the file created in a map
 *
 **/

void route::config() {
    ifstream file;


    file.open("/Users/xander/Downloads/C++ Individual Project 2/routes.csv");

    string temp, airlineIATA, airlineID, sourceAirportIATA, sourceAirportID, destAirportIATA, destAirportID;

    while(getline(file, temp)) {
        stringstream filestream(temp);

        getline(filestream, airlineIATA, ',');
        getline(filestream, airlineID, ',');
        getline(filestream, sourceAirportIATA, ',');
        getline(filestream, sourceAirportID, ',');
        getline(filestream, destAirportIATA, ',');
        getline(filestream, destAirportID, ',');

        if (flightroute.find(sourceAirportIATA) != flightroute.end()){
            route myroute = route(airlineIATA,airlineID,sourceAirportIATA,sourceAirportID,destAirportIATA,destAirportID);
            vector<route> list = flightroute.at(sourceAirportIATA);
            list.emplace_back(myroute);
            flightroute.erase(sourceAirportIATA);
            flightroute.insert({sourceAirportIATA,list});
        }
        else{
            route myroute = route(airlineIATA,airlineID,sourceAirportIATA,sourceAirportID,destAirportIATA,destAirportID);
            vector<route> list;
            list.emplace_back(myroute);
            flightroute.insert({sourceAirportIATA,list});
        }
    }

    file.close();

}

/**
 *  this function is an implementation of the breadth first search algorithm to find the path
 between airport locations
 * */


vector<string> route::bfs(airports x, airports y) {
    deque<string> frontier;
    set<string> explored;
    frontier.push_back(x.iata);
    path.insert({x.iata, "null"});



    while(!frontier.empty()){
        string val = frontier.front();
        frontier.pop_front();
        explored.insert(val);

        if(flightroute.find(val) != flightroute.end()){
            for(int i = 0; i < flightroute[val].size(); i++){


                route child = flightroute[val].at(i);

                if (!containsDeque(frontier,child.destIata) and !containsSet(explored,child.destIata)){



                    if(path.find(child.destIata) == path.end()) {

                        path.insert({child.destIata, val});
                    }
                    if (child.destIata == y.iata) {


                        return reconstruct(child.destIata);
                    }


                    frontier.push_back(child.destIata);
                }

            }

        }

    }

}

/**
 * This functions checks whether it can find a value in a deque
 * @param deques the deque
 * @param x the value being checked
 * @return whether it has found a value in the deque
 */

bool route::containsDeque(deque<string> deques, string x) {
    deque<string>::iterator itr;
    itr = find(deques.begin(), deques.end(), x);
    if(itr != deques.end()) {
        return true;
    }
    else {
        return false;
    }

}

/**
 * This function checks whether a value is contained in a set
 * @param set The set
 * @param x The value being checked
 * @return whether the set contains the value or not
 */

bool route::containsSet(set<string> set, string x) {
    auto pos = set.find(x);

    if(pos != set.end())
        return true;
    else{
        return false;
    }


}

vector<string> route::reconstruct(string iata) {
    vector<string> list;
    list.emplace_back(iata);
    string now = iata;

    while(path.find(now) != path.end()){
        now = path.at(now);
        if (now == "null"){
            break;
        }
        else{
            list.emplace_back(now);
        }
    }
    reverse(list.begin(), list.end());
    return list;
}

/**
 * This method reads the route file and creates a hashmap of all airports and their corresponding start and end destinations
 * The key is the source iata and destination iata, the value is the airports
 * */

void route::config2() {
    ifstream file;
    string temp, airlineIATA, airlineID, sourceAirportIATA, sourceAirportID, destAirportIATA, destAirportID;
    file.open("/Users/xander/Downloads/C++ Individual Project 2/routes.csv");
    while(getline(file, temp)) {
        stringstream filestream(temp);
        getline(filestream, airlineIATA, ',');
        getline(filestream, airlineID, ',');
        getline(filestream, sourceAirportIATA, ',');
        getline(filestream, sourceAirportID, ',');
        getline(filestream, destAirportIATA, ',');
        getline(filestream, destAirportID, ',');
        string key = sourceAirportIATA + "," + destAirportIATA;
        if (codes.find(key) != codes.end()){
            vector<route> list = codes.at(key);
            route p = route(airlineIATA,airlineID,sourceAirportIATA,sourceAirportID,destAirportIATA,destAirportID);
            list.emplace_back(p);
            codes.erase(key);
            codes.insert({key,list});
        }
        else{
            vector<route> list;
            route p = route(airlineIATA,airlineID,sourceAirportIATA,sourceAirportID,destAirportIATA,destAirportID);
            list.emplace_back(p);
            codes.insert({key,list});
        }

    }
    file.close();

}


/**
 * This method generates the path and writes the outcome of the path to an output file called the Source - Destination
 * of the path being found
 * @param file The input file containing the start and destination location of the airport, for which the route is to be found
 * */


void route::getPath(string file) {
    ifstream fileStream;
    vector<string> list;
    string temp, city, country;

    // gets the start city, country and destination city and country from the input file
    fileStream.open(file);
    while (getline(fileStream,temp)){
        stringstream filestream(temp);
        getline(filestream,city,',');
        getline(filestream, country,',');
        list.emplace_back(city);
        list.emplace_back(country);
    }
    fileStream.close();
    string cityOne, countryOne, cityTwo, countryTwo;
    cityOne = list.at(0);
    countryOne = list.at(1);
    cityTwo = list.at(2);
    countryTwo = list.at(3);

    airports::reader();
    route::config();
    route::config2();



    string start = cityOne + "," + countryOne;
    string destination = cityTwo+ "," + countryTwo;


    vector<string> flights;
    vector<string> path;

    for (int i = 0; i < airports::portMap.at(destination).size(); i++){

        if (airports::portMap.at(destination).at(i).iata == "\\N")
            continue;
        for (int j = 0; j < airports::portMap.at(start).size(); j++){
            if (airports::portMap.at(start).at(j).iata == "\\N")
                continue;
            path = bfs(airports::portMap.at(start).at(j),airports::portMap.at(destination).at(i));
        }
    }



    for (int i = 0; i < path.size()-1; i++){

        if(codes.find(path.at(i) + "," +path.at(i+1)) != codes.end()){
            string current = codes.at(path.at(i) + "," +path.at(i+1)).at(0).airlineIata;
            flights.emplace_back(current);
        }
    }
    write(cityOne, cityTwo, flights, path);

}

/**
 * This function is creates an output file of the path found from the start location to the end aiport location
 * @param start The start location of the airport
 * @param destination The destination or end location of the airport
 * @param flights
 * @param pathFound
 *
 * */
void route::write(string start, string destination, vector<string> flights, vector<string> pathFound) {
    ofstream file;

    string filename = start + "-" + destination + "_output.txt";
    file.open(filename);

    int num  =  0;
    while (num < pathFound.size()-1) {
        string key = pathFound.at(num) + "," + pathFound.at(num+1);
        file << flights.at(num) + " from " + pathFound.at(num) + " to " + pathFound.at(num+1) + " 0 stops" << endl;
        num+=1;
    }

    file << "Total flights = " << flights.size() << endl;
    file << "Total additional stops = 0" << endl;

    file.close();


}

