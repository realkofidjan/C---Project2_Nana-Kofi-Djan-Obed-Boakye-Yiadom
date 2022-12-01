
#include "route.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;



route::route(string airlineIata, string airlineId, string sourceIata, string sourceId, string destIata, string destId) {
    this->airlineIata = airlineIata;
    this->airlineId = airlineId;
    this->sourceIata = sourceIata;
    this->sourceId = sourceId;
    this->destIata = destIata;
    this->destId = destId;
}

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


void route::getPath(string file) {
    ifstream fileStream;
    vector<string> list;
    string temp, city, country;

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

