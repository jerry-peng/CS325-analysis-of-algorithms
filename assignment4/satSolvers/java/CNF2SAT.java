import java.util.*;

public class CNF2SAT {
	private int[] a;
	private int[] b;
	private int numClauses;
	private int numUniqueVars;
	private ArrayList<Integer>[] adj;
	private ArrayList<Integer>[] adjInv;
	private int[] visited;
	private int[] visitedInv;
	private Stack<Integer> s;
	private int sccCounter;
	private int[] scc;

	public CNF2SAT(OrClause[] Clauses) {
		numClauses = Clauses.length;
		a = new int[numClauses];
		b = new int[numClauses];
		Set<Integer> uniqueVars = new HashSet<Integer>();

		for(int i = 0; i < numClauses; i++) {
			uniqueVars.add(Math.abs(Clauses[i].first));
			uniqueVars.add(Math.abs(Clauses[i].second));
		}

		numUniqueVars = uniqueVars.size();

		List<Integer> remap = new ArrayList<Integer>(uniqueVars);
		Collections.sort(remap);
		for(int i = 0; i < numClauses; i++) {
			int adjustedFirst = 0;
			int adjustedSecond = 0;
			for(int j = 0; j < remap.size(); j++) 
			{

				if(remap.get(j) == Math.abs(Clauses[i].first)) {
					adjustedFirst = (j+1) * Math.abs(Clauses[i].first) / Clauses[i].first;
				} 

				if(remap.get(j) == Math.abs(Clauses[i].second)) {
					adjustedSecond = (j+1) * Math.abs(Clauses[i].second) / Clauses[i].second;
				} 

				if(adjustedFirst != 0 && adjustedSecond != 0) {
				   break;
				} 
			} 
			a[i] = adjustedFirst;
			b[i] = adjustedSecond;
		}

		adj = new ArrayList[numUniqueVars*2+1];	
		adjInv = new ArrayList[numUniqueVars*2+1];	
		for(int i = 0; i <= numUniqueVars*2; i++) {
			adj[i] = new ArrayList<Integer>();
			adjInv[i] = new ArrayList<Integer>();
		}
		visited = new int[numUniqueVars*2+1];
		visitedInv = new int[numUniqueVars*2+1];
		s = new Stack<Integer>();
		sccCounter = 1;
		scc = new int[numUniqueVars*2+1];
	}

    public boolean satisfiable() {

		//make sat graph
		for(int i = 0; i < numClauses; i++) {
			if(a[i] > 0 && b[i] > 0) {
				addEdges(a[i]+numUniqueVars, b[i]);
				addEdgesInverse(a[i]+numUniqueVars, b[i]);
				addEdges(b[i]+numUniqueVars, a[i]);
				addEdgesInverse(b[i]+numUniqueVars, a[i]);
			} else if(a[i] > 0 && b[i] < 0) {
				addEdges(a[i]+numUniqueVars, numUniqueVars-b[i]);
				addEdgesInverse(a[i]+numUniqueVars, numUniqueVars-b[i]);
				addEdges(-b[i], a[i]);
				addEdgesInverse(-b[i], a[i]);
			} else if (a[i] < 0 && b[i] > 0) {
				addEdges(-a[i], b[i]);
				addEdgesInverse(-a[i], b[i]);
				addEdges(b[i]+numUniqueVars, numUniqueVars-a[i]);
				addEdgesInverse(b[i]+numUniqueVars, numUniqueVars-a[i]);
			} else {
				addEdges(-a[i], numUniqueVars-b[i]);
				addEdgesInverse(-a[i], numUniqueVars-b[i]);
				addEdges(-b[i], numUniqueVars-a[i]);
				addEdgesInverse(-b[i], numUniqueVars-a[i]);
			}
		}

		for(int i = 1; i <= 2*numUniqueVars; i++) {
			if(visited[i] == 0) {
				dfsFirst(i);
			}
		}

		while(!s.empty()) {
			int v = s.pop();

			if(visitedInv[v] == 0) {
				dfsSecond(v);
				sccCounter+=1;
			}
		}

		for(int i = 1; i <= numUniqueVars; i++) {
			if(scc[i] == scc[i+numUniqueVars]) {
				return false;
			}
		}

		return true;
    }

	private void addEdges(int v, int u) {
		adj[v].add(u);
	}

	private void addEdgesInverse(int v, int u) {
		adjInv[v].add(u);
	}

	private void dfsFirst(int v) {
		if(visited[v] == 1) {
			return;
		}

		visited[v] = 1;

		for(int i = 0; i < adj[v].size(); i++) {
			dfsFirst((Integer)adj[v].get(i));
		}

		s.push(v);	
	}

	private void dfsSecond(int v) {
		if(visitedInv[v] == 1) {
			return;
		}

		visitedInv[v] = 1;

		for(int i = 0; i < adjInv[v].size(); i++) {
			dfsSecond((Integer)adjInv[v].get(i));
		}

		scc[v] = sccCounter;	
	}

}
