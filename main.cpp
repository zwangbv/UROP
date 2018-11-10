#include <iostream>
#include "allocation.h"
#include <limits.h>
#include <string.h>
#include <queue>
#include "vector"
#include "algorithm"
#include "iterator"

using namespace std;

allocation generateAllocation(){
    allocation b = allocation(2);
    b.addPlayer(0);
    b.addPlayer(1);
    b.addPlayer(2);
    b.addPlayer(3);
    b.addResources(0,5);
    b.addResources(1,4);
    b.addResources(2,3);
    b.addResources(3,2);

    b.buildRequirement();

    b.addRequirement(0,0);
    b.addRequirement(0,2);
    b.addRequirement(1,0);
    b.addRequirement(2,2);
    b.addRequirement(2,0);
    b.addRequirement(2,1);
    b.addRequirement(3,3);

    b.buildMaxMatching();
    b.buildGm();
    return b;
}

/* Returns true if there is a path from source 's' to sink 't' in
  residual graph. Also fills parent[] to store the path */
bool bfs(int** rGraph, int s, int t, int parent[], int V)
{
    bool visited[V];
    for(int i=0;i<V;i++){
        visited[i]=false;
    }
    queue<int> q;
    q.push(s);
    visited[s]=true;
    parent[s] = -1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();//delete the first element
        for (int i=0; i<V; i++)
        {
            if (!visited[i] && rGraph[u][i] > 0)
            {
                q.push(i);
                parent[i] = u;//the previous nodes
                visited[i] = true;
            }
        }
    }
    return (visited[t]);
}
int fordFulkerson(int** graph, int s, int t, int V)
{
    int u, v;

    int** rGraph=new int*[V];
    for(int i = 0; i < V; i++) {
        rGraph[i] = new int[V];
    }

    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];

    int parent[V];  // This array is filled by BFS and to store path

    int max_flow = 0;  // There is no flow initially

    // Augment the flow while tere is path from source to sink
    while (bfs(rGraph, s, t, parent, V))
    {
        // the maximum flow through the path found.
        int path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    // Return the overall flow
    return max_flow;
}
int fM(allocation allo,vector<int> svector, vector<int> tvector){
    int **Gm=allo.buildGm();
    int num_players=allo.playerSize();
    int num_resources=allo.resourceSize();
    int __maxNodes=num_players+num_resources;
    int **maxFlow  =  new int* [__maxNodes+2];
    for(int i = 0; i < __maxNodes+2; i++) {
        maxFlow[i] = new int[__maxNodes+2];
    }
    for(int i = 0; i < __maxNodes+2; i++){
        for(int j = 0; j < __maxNodes+2; j++){
            maxFlow[i][j]=0;
        }
    }
    for(int i = 0; i < __maxNodes; i++){
        for(int j = 0; j < __maxNodes; j++){
            maxFlow[i][j]=Gm[i][j];
        }
    }
    for(int i = 0; i <svector .size() ; i++){
        maxFlow[svector[i]][__maxNodes]=1;
    }
    for(int i = 0; i <tvector .size() ; i++){
        maxFlow[__maxNodes][tvector[i]]=1;
    }
    return fordFulkerson(maxFlow, __maxNodes, __maxNodes+1, __maxNodes+2);
}

vector<int> vectors_set_union(vector<int> v1,vector<int> v2){
    vector<int> v;
    sort(v1.begin(),v1.end());
    sort(v2.begin(),v2.end());
    set_union(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v));//Union
    return v;
}

bool checkAddable(allocation allo,int p,vector<int> x,vector<int> y,vector<int> I){
    vector<int> tvector = vectors_set_union(x,I);
    int a = fM(allo,y, tvector);
    vector<int> tvector2;
    tvector2.push_back(p);
    tvector = vectors_set_union(tvector,tvector2);
    int b = fM(allo,y, tvector);
    return (a == b);
}


int main() {
    allocation b = generateAllocation();
    cout<<2;
    b.printMax();
    b.buildGm();
    return 0;
}