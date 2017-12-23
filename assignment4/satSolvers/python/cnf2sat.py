def visit(v, graph, visited, finished=[]):
	visited[v] = True
	for neighbor in graph[v]:
		if not visited[neighbor]:
			visit(neighbor, graph, visited, finished)
	finished.append(v)

def reverseTopo(graph):
	finished = []
        visited = {v: False for v in graph}
	for vertex in graph:
		if not visited[vertex]:
			visit(vertex, graph, visited, finished) 
	return finished

def kosaraju(graph,rGraph):
	rTopo = reverseTopo(graph)
	
	SCCs = []
        visited = {v: False for v in rTopo}

	while len(rTopo) > 0:
		current = rTopo.pop()
		if not visited[current]:
			scc = []
			visit(current, rGraph, visited, scc)
			SCCs.append(scc)

	return SCCs

def canSat(SCCs):
	for component in SCCs:
		verticesInSCC = set(component);
		for vertex in verticesInSCC:
			if NOT(vertex) in verticesInSCC:
                            return False
	return True

def NOT(var):
        return var*-1
	#return (var[0], int(not var[1]))

def createSATGraph(clauses):
	graph = {}
	rGraph = {}

	for clause in clauses:
		A = clause[0]
		B = clause[1]

		if not (NOT(A) in graph):
			graph[NOT(A)] = []
		if not (NOT(B) in graph):
			graph[NOT(B)] = []
		if not (A in rGraph):
			rGraph[A] = []
		if not (B in rGraph):
			rGraph[B] = []

		graph[NOT(A)].append(B)
		graph[NOT(B)].append(A)
		rGraph[B].append(NOT(A))
		rGraph[A].append(NOT(B))

	return graph, rGraph

def satisfiable(clauses):
	graph, rGraph = createSATGraph(clauses)
	SCCs = kosaraju(graph,rGraph)
	return canSat(SCCs)

