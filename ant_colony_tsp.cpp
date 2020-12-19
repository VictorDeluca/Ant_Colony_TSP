//Ant colony thingy to solve a large TSP instance
//Step 1: Construct TSP structure - ok
//Step 2: Construct ant colony structure;

#include <bits/stdc++.h>
#define INF 99999999
using namespace std;

//Undirected graph
int graph[1000][1000];
double phero[1000][1000];
int v,e;

double pick_random(double l, double r){
	double f = (double)rand() / RAND_MAX;
  double ans = l + f * (r - l);
	return ans;
}

int get_value(vector<int> path){
	int i,ans = 0;
	for(i=1;i<path.size();i++)
		ans += graph[path[i-1]][path[i]];
	return ans;
}

void spread_pheromones(double value, vector<int> path){
	int i;
	for(i=1;i<path.size();i++){
		int from = path[i-1], to = path[i];
		phero[from][to] = value;
	}
}


vector<int> ant_walk(){
	int visited[1000],i,j;
	vector<int> ans;
	double solution[1001],odds[1001];
	memset(visited,0,sizeof(visited));

	int cur = 0; //Current edge. Always start from edge[0] because it really doesn't matter where you start from (A TSP is a cycle, the size should always be the same no matter where you start from)
	visited[cur] = 1;
	//solution[0] = 0;

	//Constructing a solution
	ans.push_back(0);
	for(i=0;i<v-1;i++){
		//Choosing the next edge (Counting what are the odds of picking the j'th edge next)
		odds[0] = 0;
		//Some type of cummulative sum. Note that odds[v] is the total amount of pheromones among the edges to choose from.
		for(j=1;j<v;j++){
			if(visited[j]==1) odds[j] = odds[j-1]; //If the node 'j' was already chosen, make its odds 0%.
			else odds[j] = odds[j-1] + phero[cur][j]; 
		}
		//for(j=0;j<v;j++) odds[j] = 100*(phero[cur][j]/odds[v]); //Odds of an edge[cur][j] being chosen over the others edges from [cur]. Might be useless in this implementation
		double choice = pick_random(0,odds[v-1]); //Choose a random value in the range [0,total_sum]
		for(j=1;j<v;j++)
			if(choice >= odds[j-1] && choice <= odds[j]) break;
		
		ans.push_back(j);
		//solution[i] = j-1;
		cur = j;
		visited[cur] = 1;
	}
	ans.push_back(0); //Finishing the circular tour

	return ans;
}

vector<int> ant_colony_tsp(){
	int best = INF,i,j;
	vector<int> ans;
	int ants = 10, it = 100; //10 ants per iteration; 100 iterations;
	vector<vector<int>> ant_record;

	while(it--){
		//Ants walking
		for(i=0;i<ants;i++){
			vector<int> walk = ant_walk();
			int cur = get_value(walk);
			if(cur < best){
				best = cur;
				ans = walk;
			}
			ant_record.push_back(walk);
		}

		//Pheromones spreading (You know, that SHOULD happen during the walk, but this algorithm is weird so...)
		//Just updating the phero[][] table
		for(i=0;i<ants;i++){
			vector<int> walk = ant_record[i];
			double val = get_value(walk);
			double ant_phero = 1.0/val;
			spread_pheromones(ant_phero,walk);
		}
	}
	
	return ans;
}


int main() {
	scanf("%d%d",&v,&e);
	int i,j;
	for(i=0;i<v;i++){
		for(j=0;j<v;j++){
			graph[i][j] = INF;
			phero[i][j] = 1; //Might be a problem if we initialize with 0. As long as everyone is the same, it's fine
		}
	}

	for(i=0;i<e;i++){
		int a,b,cost; scanf("%d%d%d",&a,&b,&cost);
		graph[a][b] = cost;
	}

	vector<int> ans = ant_colony_tsp();
	printf("The quickest route found was:\n");
	for(j = 0; j<ans.size();j++) printf("%d ", ans[j]);
	printf("\n");
	printf("And its cost was %d\n",get_value(ans));
	
	return 0;
}