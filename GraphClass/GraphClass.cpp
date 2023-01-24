#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>

using namespace std;


//bfs, dfs, topo sort
//prim, kruskall 
//dijkstra, bellman-ford, floyd-warsh
//eulerian, hamiltonian
//maxflux
class Graph{
	
private:
	class DisjointSet {
	private:
		const int _size;
		int* _father;
		int* _rank;

		int getRoot(int el) {
			while (_father[el] != el) {
				el = _father[el];
			}
			return el;
		}
	public:
		DisjointSet(const int p_size) : _size(p_size) {
			_father = new int[_size];
			_rank = new int[_size];
			for (int i = 0; i < p_size; ++i) {
				_father[i] = i;
				_rank[i] = 1;
			}
		}
		void merge(const int x, const int y) {
			const int root_y = getRoot(y);
			const int root_x = getRoot(x);
			if (_rank[root_x] > _rank[root_y]) {
				_father[root_y] = root_x;
			}
			else {
				_father[root_x] = root_y;
				if (_rank[root_x] == _rank[root_y]) {
					_rank[root_y] = _rank[root_x] + 1;
				}
			}
		}

		bool check(const int x, const int y) {
			return getRoot(x) == getRoot(y);
		}
		~DisjointSet() {
			delete[] _father;
			delete[] _rank;
		}
	};

	vector<tuple<int, int, int>> _edge_list;
	vector<vector<pair<int, int>>> _adj_list;
	vector<unordered_map<int, int>> _residual;

	bool _undirected;
	int _nodes;
	static bool _pairGreater(pair<int,int> edge1, pair<int,int> edge2){
		return edge1.second > edge2.second;
	}
	static bool _tupleLesser(tuple<int, int, int> edge1, tuple<int, int, int> edge2) {
		return get<2>(edge1) < get<2>(edge2);
	}

	void rec_dfs(int start, vector<bool>& visited, vector<int>& traversal) {

		for(const auto& edge : _adj_list[start]){
			const auto node = edge.first;
			if(!visited[node]) {
				visited[node] = true;
				traversal[node] = start;
				rec_dfs(node,visited,traversal);
			}
		}
	}
	bool flow_bfs(const int start, const int dest, vector<int>& bfs_tree, vector<bool>& visited) {

		queue<int> node_queue;
		visited.assign(_nodes, false);
		bfs_tree.assign(_nodes,-1);

		bfs_tree[start] = -1;
		visited[start] = true;
		node_queue.push(start);

		while (!node_queue.empty()) {
			const int node = node_queue.front();
			node_queue.pop();

			for (auto& edge : _residual[node]) {

				if (!visited[edge.first] && edge.second > 0) {
					visited[edge.first] = true;
					bfs_tree[edge.first] = node;
					node_queue.push(edge.first);
				}
			}
		}
		return visited[dest];
	}
	vector<int> compute_degrees(){

		vector<int> in_deg(_nodes);

		for (int node = 0; node < _nodes; ++node) {

			if(in_deg[node] == 0){
				in_deg[node] = 0;
			}
			for(const auto& edge : _adj_list[node]){
				const auto next_node = edge.first;
				++in_deg[next_node];
			}
		}
		return in_deg;
	}
	void rec_euler(int start,vector<int>& path, vector<vector<bool>>& visited){

		for(auto& edge : _adj_list[start]) {
			int node = edge.first;
			if (visited[start][node])
				continue;

			visited[start][node] = true;

			if(_undirected && visited[node][start])
				continue;

			rec_euler(node,path, visited);
			path.push_back(node);
		}
	}
	bool solution(int size, vector<bool> state) {

		if (size < _nodes - 1)
			return false;

		for (int node = 0; node < _nodes; ++node) {
			if (state[node] == false)
				return false;
		}
		return true;
	}
public:
	static constexpr int _infinity = numeric_limits<int>::max();

	Graph(vector<vector<pair<int, int>>>& p_adj_list, bool undirected = false) :
		_adj_list(move(p_adj_list)),
		_undirected(undirected),
		_nodes(static_cast<int>(_adj_list.size()))
		
		{}
	Graph(vector<tuple<int, int, int>>& p_edge_list, int nodes, bool undirected = false) :
		_edge_list(move(p_edge_list)),
		_undirected(undirected),
		_nodes(nodes)
		{}
	Graph(vector<unordered_map<int,int>>& p_residual) :
		_residual(move(p_residual)),
		_undirected(false),
		_nodes(static_cast<int>(_residual.size()))
	{
		for (int i = 0; i < _nodes; ++i) {
			for (const auto& edge : _residual[i]) {
				if (_residual[edge.first].find(i) == _residual[edge.first].end())
					_residual[edge.first][i] = 0;
			}
		}
	}

	void add_edge_list(vector<tuple<int, int, int>>& p_edge_list)
	{
		_edge_list = move(p_edge_list);
	}
	void add_adj_list(vector<vector<pair<int, int>>>& p_adj_list)
	{
		_adj_list = move(p_adj_list);
	}
	void add_residual(vector<unordered_map<int, int>>& p_residual)
	{
		_residual = move(p_residual);
		for (int i = 0; i < _nodes; ++i) {
			for (const auto& edge : _residual[i]) {
				if (_residual[edge.first].find(i) == _residual[edge.first].end())
					_residual[edge.first][i] = 0;
			}
		}
	}

	vector<int> dfs(int start){

		vector<int> traversal(_nodes,-1);
		vector<bool> visited(_nodes);
		visited[start] = true;

		rec_dfs(start, visited, traversal);

		return traversal;
	}
	vector<int> bfs(int start){

		vector<int> traversal(_nodes,-1);
		vector<bool> visited(_nodes);
		queue<int> node_queue;

		node_queue.push(start);
		visited[start] = true;

		while(!node_queue.empty()){

			const auto crt_node = node_queue.front();
			node_queue.pop();

			for(const auto& edge : _adj_list[crt_node]){

				auto node = edge.first;
				if (!visited[node]) {

					visited[node] = true;
					traversal[node] = crt_node;
					node_queue.push(node);
				}
			}
		}

		return traversal;
	}
	vector<int> topo_sort(){

		if (_undirected)
			return {};
		auto in_deg = compute_degrees();

		vector<int> order;
		queue<int> node_queue;

		for (int node = 0; node < _nodes; ++ node) {
			const auto degree = in_deg[node];

			if(degree == 0){
				node_queue.push(node);
			}
		}

		while(!node_queue.empty()){

			const auto crt_node = node_queue.front();
			order.push_back(crt_node);
			node_queue.pop();

			for (const auto& edge : _adj_list[crt_node]) {
				const auto node = edge.first;

				--in_deg[node];
				if(in_deg[node] == 0){
					node_queue.push(node);
				}
			}
		}

		return order;
	}
	pair<vector<int>,vector<pair<int,int>>> critical(){
		
	};
	pair<vector<int>,vector<int>> prim(){

		priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(&_pairGreater)> edge_heap(&_pairGreater);
		vector<int> father(_nodes, -1);
		vector<int> costs(_nodes, _infinity);
		vector<bool> in_apm(_nodes);

		int first_node = 0;
		costs[first_node] = 0;
		edge_heap.emplace(first_node, 0);

		while (!edge_heap.empty()) {

			const auto crt_edge = edge_heap.top();
			edge_heap.pop();
			const auto crt_node = crt_edge.first;
			const auto crt_cost = crt_edge.second;

			if(in_apm[crt_node])
				continue;

			in_apm[crt_node] = true;

			for (const auto& edge : _adj_list[crt_node]) {

				const auto node = edge.first;
				const auto cost = edge.second;
				if(in_apm[node])
					continue;

				if (cost < costs[node]) {

					edge_heap.emplace(edge);
					costs[node] = cost;
					father[node] = crt_node;
				}
			}
		}
		return { costs,father };
	}
	vector<tuple<int, int, int>> kruskall(){

		sort(_edge_list.begin(), _edge_list.end(), _tupleLesser);

		vector<tuple<int, int, int>> apm_edges;
		DisjointSet component(_nodes);

		for (const auto& edge : _edge_list)
		{
			const auto node1 = get<0>(edge);
			const auto node2 = get<1>(edge);
			const auto cost = get<2>(edge);

			if (!component.check(node1, node2)) {
				component.merge(node1, node2);
				apm_edges.push_back(edge);
			}
		}
		return apm_edges;
	}
	pair<vector<int>,vector<int>> dijkstra(int start){

		priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(&_pairGreater)> edge_heap(&_pairGreater);
		vector<int> father(_nodes,-1);
		vector<int> costs(_nodes,_infinity);

		int first_node = start;
		costs[first_node] = 0;

		edge_heap.emplace(first_node, 0);

		while (!edge_heap.empty()) {

			const auto crt_edge = edge_heap.top();
			edge_heap.pop();
			const auto crt_node = crt_edge.first;
			const auto crt_cost = crt_edge.second;

			if (crt_cost > costs[crt_node])
				continue;

			for (const auto& edge : _adj_list[crt_node]) {

				const auto node = edge.first;
				const auto cost = edge.second;

				if (crt_cost + cost < costs[node]) {

					edge_heap.emplace(node,crt_cost+cost);
					costs[node] = crt_cost + cost;
					father[node] = crt_node;
				}
			}
		}
		return {costs,father};
	}
	pair<vector<int>,vector<int>> bellman_ford(int start){

		vector<int> father(_nodes, -1);
		vector<int> costs(_nodes, _infinity);
		vector<bool> in_queue(_nodes);

		queue<int> node_queue;

		node_queue.push(start);
		costs[start] = 0;
		in_queue[start] = true;

		while (!node_queue.empty()) {

			const auto crt_node = node_queue.front();
			node_queue.pop();
			in_queue[crt_node] = false;

			for (const auto& edge : _adj_list[crt_node]) {
				const auto node = edge.first;
				const auto cost = edge.second;

				if (costs[node] > costs[crt_node] + cost) {
					costs[node] = costs[crt_node] + cost;
					father[node] = crt_node;

					if (!in_queue[node]) {
						node_queue.push(node);
						in_queue[node] = true;
					}
				}
			}

		}

		return { costs,father };
	}
	pair<vector<vector<int>>,vector<vector<int>>> floyd_warshall(){

		vector<vector<int>> next(_nodes, vector<int>(_nodes,-1));
		vector<vector<int>> costs(_nodes, vector<int>(_nodes, _infinity));

		for(int crt_node = 0; crt_node < _nodes; ++crt_node){
			costs[crt_node][crt_node] = 0;
			next[crt_node][crt_node] = crt_node;

			for(const auto& edge : _adj_list[crt_node]){

				const auto node = edge.first;
				const auto cost = edge.second;

				costs[crt_node][node] = cost;
				next[crt_node][node] = node;
			}
		}

		for(int inter = 0; inter < _nodes; ++inter){
			for(int src = 0; src < _nodes; ++src){
				for(int dest = 0; src < _nodes; ++dest){
					if(costs[src][inter] + costs[inter][dest] < costs[src][dest]){

						costs[src][dest] = costs[src][inter] + costs[inter][dest];
						next[src][dest] = next[src][inter];
					}
				}
			}
			
		}
		return { costs,next };
	}
	int maxflow(int source, int dest){

		if (_undirected)
			return 0;
		int max_flux = 0;
		vector<int> bfs_tree(_nodes);
		vector<bool> visited(_nodes);

		while (flow_bfs(source, dest,bfs_tree,visited)) {
			for (const auto& edge : _residual[dest]) {

				int node = edge.first;
				if (_residual[node][dest] == 0 || !visited[node])
					continue;

				int flux = numeric_limits<int>::max();
				flux = min(flux, _residual[node][dest]);

				while (bfs_tree[node] != -1) {
					const int father = bfs_tree[node];
					flux = min(flux, _residual[father][node]);
					if (flux == 0)
						break;
					node = father;
				}

				if (flux == 0)
					continue;
				max_flux += flux;

				node = edge.first;
				_residual[node][dest] -= flux;
				_residual[dest][node] += flux;

				while (bfs_tree[node] != -1) {
					const int father = bfs_tree[node];
					_residual[father][node] -= flux;
					_residual[node][father] += flux;
					node = father;
				}
			}
		}
		return max_flux;
	}
	vector<int> eulerian(){

		auto in_deg = compute_degrees();

		int bad_count = 0;
		int start = 0;
		if(_undirected){
			for(int node = 0; node < _nodes; ++ node){

				if(in_deg[node]% 2 == 1){
					++bad_count;
					start = node;
					if (bad_count > 2)
						return { };
				}
			}
		}
		else{
			for (int node = 0; node < _nodes; ++node) {

				if (in_deg[node] < _adj_list[node].size()) {
					++bad_count;
					start = node;
					if (bad_count >= 2)
						return { };
				}
			}
		}

		vector<int> path;
		vector<vector<bool>> visited(_nodes, vector<bool>(_nodes));

		rec_euler(start, path, visited);
		path.push_back(start);

		reverse(path.begin(), path.end());
		return path;
	}
	int hamiltonian(){

		vector<unordered_set<vector<bool>>> states(_nodes);
		queue<tuple<int, int, vector<bool>>> path_q;

		for (int node = 0; node < _nodes; ++node) {

			vector<bool> visited(_nodes);
			visited[node] = true;
			path_q.emplace(node, 0, visited);
			states[node].insert(visited);
		}

		while (!path_q.empty()) {

			auto crt_path = path_q.front();
			const auto crt_node = get<0>(crt_path);
			const auto size = get<1>(crt_path);
			const auto state = get<2>(crt_path);

			path_q.pop();
			if (solution(size, state)) {
				return size;
			}
			for (const auto& edge : _adj_list[crt_node]) {

				const int node = edge.first;
				vector<bool> new_state = state;
				new_state[node] = true;

				if (states[node].find(new_state) == states[node].end()) {
					states[node].insert(new_state);
					path_q.emplace(node, size + 1, new_state);
				}
			}
		}
		return -1;
	}
};


int main()
{
	ifstream in("input.in");
	int n, m;
	in >> n >> m;
	vector<vector<pair<int,int>>> repr(n);
	//vector<tuple<int, int, int>> edges;
	//vector<unordered_map<int, int>> residual(n);
	for(int i=0; i<m;++i){
		int node1, node2, cost;
		in >> node1 >> node2>>cost;
		repr[node1].emplace_back(node2, cost);
		repr[node2].emplace_back(node1, cost);
		//edges.emplace_back(node1, node2, cost);
	}
	Graph test(repr,true);
	//test.add_edge_list(edges);

	//auto dfs = test.dfs(0);
	//auto bfs = test.bfs(0);
	//auto sol3 = test.topo_sort();
	//auto apm_prim = test.prim();
	//auto apm_krusk = test.kruskall();
	//auto dijk = test.dijkstra(0);
	//auto bmf = test.bellman_ford(0);
	//auto flux = test.maxflow(0, 3);
	//auto euler = test.eulerian();
	int len = test.hamiltonian();

	return 0;
}