//
// Created by noki on 2018/11/10.
//

#ifndef UROP_ALLOCATION_H
#define UROP_ALLOCATION_H

#include <vector>

using namespace std;


class allocation {
public:
    allocation(double fat);
    ~allocation();
    void addPlayer(int player); //player should be 0-n
    void addResources(int resource, double value);//resource should be 0-n
    void addRequirement(int player, int resource);
    int** buildGm();
    int** buildMaxMatching();
    int** buildRequirement();
    int playerSize(){return num_players;};
    int resourceSize(){return num_resources;};
    int getRequirement(int player,int resource){return requirement[player][resource];};
    double getValue(int player,int resource){return values[resource]*requirement[player][resource];}
    int getGm(int player,int resource){return Gm[player][resource];};
    void printMax();
    int** getMaxMatching(){return maxMatching;};
    void setMaxMatching(int **matching){maxMatching = matching;};
   // bool find(int x);
private:
vector<int> players;
vector<int> resources;
vector<double> values;
int **requirement;
int num_players;
int num_resources;
int **maxMatching;
int **Gm;
double fatvalue;

};


#endif //UROP_ALLOCATION_H
