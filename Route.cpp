/**
 * This is a solution to the route finding problem. This class represents a route object.
 * @author Obed Yiadom Boakye & Nana Kofi Djan
 */

#include "Route.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <cmath>
#include <set>

using namespace std;

/**
 * Constructor for a route object
 * @param airlineIata unique airline iata code
 * @param airlineId unique airline id number
 * @param sourceIata iata code of the source airport
 * @param sourceId id of the source airport
 * @param destIata iata code of the destination airport
 * @param destId id of the destination airport
 * @param stops number of stops
 */
Route::Route( string airlineCode, string airlineID,string sourceAirportCode, string sourceAirportID,
             string destinationAirportCode, string destinationAirportID,  string stops){
            this->airlineCode = airlineCode;
            this->airlineID = airlineID;
            this->sourceAirportCode = sourceAirportCode;
            this->sourceAirportID = sourceAirportID;
            this->destinationAirportCode = destinationAirportCode;
            this->destinationAirportID = destinationAirportID;
            this->stops = stops;
}


/**
 * This function reads a csv file, creates route objects from the values and stores them in unordered maps.
 *
 * @param csvFile The routes.csv file being read.
 */
void Route::getFlights(string csvFile) {
    ifstream fileStream;

      if (!fileStream.is_open()) {
        cout << "File not opened";
    }else{

        string line, airlineIata, airlineId, sourceAirportIata, sourceAirportId, destAirportIata, destAirportId, codeshare, stops;

        while(getline(fileStream, line)) {
            //Try and skip the first line of file
            stringstream stream(line);

            getline(stream, airlineIata, ',');
            getline(stream, airlineId, ',');
            getline(stream, sourceAirportIata, ',');
            getline(stream, sourceAirportId, ',');
            getline(stream, destAirportIata, ',');
            getline(stream, destAirportId, ',');
            getline(stream, codeshare, ',');
            getline(stream, stops, ',');



            string compKey = sourceAirportIata + "," + destAirportIata;
            string flight = airlineIata;
            Route tempRoute = Route(airlineIata,airlineId,sourceAirportIata,sourceAirportId,destAirportIata,destAirportId,stops);

            if (Route::flights.find(compKey) != Route::flights.end()){
                vector<Route> flightList = Route::flights[compKey];
                flightList.emplace_back(tempRoute);
                Route::flights.erase(compKey);
                Route::flights.insert({compKey,flightList});
            }
            else{
                vector<Route> flightList;
                flightList.emplace_back(tempRoute);
                Route::flights.insert({compKey,flightList});
            }

            string key = sourceAirportIata;
            if (Route::routes.find(key) != Route::routes.end()){
                vector<Route> routeList = Route::routes[key];
                routeList.emplace_back(tempRoute);
                Route::routes.erase(key);
                Route::routes.insert({key,routeList});
            }
            else{
                vector<Route> routeList;
                routeList.emplace_back(tempRoute);
                Route::routes.insert({key,routeList});
            }
        }

    }



/**
 * This function reads an input file with a start city and destination city and finds an optimal route between the two cities.
 * @param csvFile The input file being read.
 */
void Route::findRoute(std::string csvFile) {
    Airport::readFile("/Users/xander/Documents/OneDrive - Ashesi University/ Academia/2nd Year Sem 2/Intermediate Computer Programming /C++ Individual Project/airports.csv");
    ifstream fileStream;
    vector<string> list;

       if (!airportFile.is_open()) {
        cout << "File not opened";
    } else{
        string line,city, country, destinationCity, destinationCountry;

        fileStream.open(csvFile);
        while (getline(fileStream,line)){
            stringstream stream(line);
            getline(stream,city,',');
            getline(stream, country,',');
            list.emplace_back(city);
            list.emplace_back(country);
        }

    }


    fileStream.close();
    string sourceCity = list.at(0);
    string sourceCountry = list.at(1);
    string destCity = list.at(2);
    string destCountry = list.at(3);
    string source = sourceCity + "," + sourceCountry;
    string dest = destCity + "," + destCountry;


    getFlights("/Users/xander/Documents/OneDrive - Ashesi University/ Academia/2nd Year Sem 2/Intermediate Computer Programming /C++ Individual Project/routes.csv");


    vector<Airport> sourceAirports = Airport::getAirport(source);
    vector<Airport> destinationAirports = Airport::getAirport(dest);

    //Check for null values
    vector<string> path;
    vector<string> flightPath;
    vector<double> distances;
    unordered_map<double, vector<string>> pathDistance;
    for (int i = 0; i < destinationAirports.size(); i++){

        if (destinationAirports.at(i).getIataCode() == "\\N")
            continue;
        for (int j = 0; j < sourceAirports.size(); j++){
            if (sourceAirports.at(j).getIataCode() == "\\N")
                continue;
            path = search(sourceAirports.at(j),destinationAirports.at(i));
            double distance = 0;
            for (int k = 0; k < path.size()-1; k++) {
                Airport start = Airport::getObject(path.at(k));
                Airport destination = Airport::getObject(path.at(k+1)); //Airport::codes.at(path.at(k + 1));
                distance += haversine(start,destination);

            }
            distances.emplace_back(distance);
            pathDistance.insert({distance, path});
            for (string p: path)
                cout << p << " ";
            cout << endl;

        }
    }

    double minDistance = *min_element(distances.begin(), distances.end());
    path = pathDistance[minDistance];

    for (int k = 0; k < path.size()-1; k++){
        string key = path.at(k) + "," + path.at(k+1);

        if(Route::flights.find(key) != Route::flights.end()){
            string current = Route::flights[key].at(0).getAirlineIata();  //Picking the first available flight from one location to another
            flightPath.emplace_back(current);
        }
    }
    Route::writeToFile(sourceCity, destCity, path, flightPath, minDistance);

}



/**
 * This function uses a breadth first search to all available routes from one city to another.
 * @param start The airport of the start city
 * @param destination The airport of the destination city
 * @return A list of IATA codes that indicate the path taken, i.e. the various airports that were passed through.
 */
vector<string> Route::search(Airport start, Airport destination) {
    deque<string> frontier;
    set<string> explored;
    frontier.push_back(start.getIataCode());
    parents.insert({start.getIataCode(), "None"});



    while(!frontier.empty()){
        string poppedValue = frontier.front();
        frontier.pop_front();
        explored.insert(poppedValue);

        if(Route::routes.find(poppedValue) != Route::routes.end()){
            vector<Route> temp = Route::routes[poppedValue];
            for(int i = 0; i < temp.size(); i++){
                Route child = temp.at(i);

                if (!Route::contains(frontier,child.getdestIata()) and !Route::set_contains(explored,child.getdestIata())){
                    if(Route::parents.find(child.getdestIata()) == Route::parents.end()) {
                        Route::parents.insert({child.getdestIata(), poppedValue});
                    }
                    if (child.getdestIata() == destination.getIataCode()) {
                        return solutionPath(child.destIata);
                    }
                    frontier.push_back(child.getdestIata());
                }

            }

        }

    }

}




/**
 * This function gets the IATA code of an airline
 * @return airline IATA code
 */
string Route::getAirlineIata() {
    return this->airlineIata;
}


/**
 * This function gets the ID of an airline
 * @return airline ID
 */
string Route::getAirlineId() {
    return this->airlineId;
}


/**
 * This function gets the IATA code of the source airport
 * @return source airport IATA code
 */
string Route::getSourceIata() {
    return this->sourceIata;
}

/**
 * This function gets the ID of the source airport
 * @return source airport ID
 */
string Route::getSourceId() {
    return this->sourceId;
}

/**
 * This function gets the IATA code of the destination airport
 * @return destination airport IATA code
 */
string Route::getdestIata() {
    return this->destIata;
}

/**
 * This function gets the ID of the destination airport
 * @return destination airport ID
 */
string Route::getdestId() {
    return this->destId;
}

/**
 * This functions checks whether a value is found in a deque
 * @param myDeque the deque
 * @param value the value being checked for
 * @return whether the value is in the deque or not
 */
bool Route::contains(deque<string> myDeque, string value) {
        deque<string>::iterator itr;
        itr = find(myDeque.begin(), myDeque.end(), value);
        if(itr != myDeque.end()) {
            return true;
        }
        else
            return false;

}



/**
 * This function checks whether a set contains a particular value
 * @param s The set
 * @param value The value being checked for
 * @return whether the set contains the value or not
 */
bool Route::set_contains(set<string> s, string value) {
    auto pos = s.find(value);

    if(pos != s.end())
        return true;
    else
        return false;

}

/**
 * This function reconstructs the path took from the start to the destination
 * @param destinationIata the IATA code of the destination airport
 * @return A vector containing the path taken from start to destination
 */
vector<string> Route::solutionPath(string destinationIata) {
    vector<string> path;
    path.emplace_back(destinationIata);
    string current = destinationIata;

    while(Route::parents.find(current) != Route::parents.end()){
        current = parents[current];
        if (current == "None"){
            break;
        }
        else{
            path.emplace_back(current);
        }
    }
    reverse(path.begin(), path.end());
    return path;
}


/**
 * This function writes the path found by the program to a file
 * @param start The starting city
 * @param destination The destination city
 * @param path The IATA codes of path found
 * @param flightPath The airlines taken to get to these paths
 * @param distance The total distance covered by the path
 */
void Route::writeToFile(string start, string destination, vector<string> path, vector<string> flightPath, double distance) {
    ofstream fileStream;
    start[0] = tolower(start[0]);
    destination[0] = tolower(destination[0]);
    if (!airportFile.is_open()) {
        cout << "File not opened";
    } else {
        string filename = start + "-" + destination + "_output.txt";
        fileStream.open(filename);

        int count = 0;
        int numStops = 0;
        while (count < path.size() - 1) {
            string key = path.at(count) + "," + path.at(count + 1);
            vector<Route> route = Route::flights[key];
            string stops = route.at(
                    0).getNumStops();
            fileStream << count + 1 << ". "
                       << flightPath.at(count) + " from " + path.at(count) + " to " + path.at(count + 1) + stops +
                          " stops" << endl;
            numStops += stoi(stops);
            count++;

        }

        int numFlights = path.size() - 1;
        fileStream << "Total flights = " << numFlights << endl;
        fileStream << "Total additional stops = " << numStops << endl;
        fileStream << "Total distance: " << distance << "km" << endl;


    }


/**
 * This function computes the distance between two points with given latitude and longitude
 * @param start The start airport
 * @param destination The destination airport
 * @return The distance between the two airports
 */

    string Route::getNumStops() {
        return this->stops;
    }


}
}

