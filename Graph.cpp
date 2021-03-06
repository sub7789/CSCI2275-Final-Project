#include "Graph.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
#include <stack>
#include <list>
#include "BST.h"

using namespace std;

Graph::Graph()
{
}

Graph::~Graph()
{
}

// Function that adds an edge to a vertex (used in the driver file when building the graph)
void Graph::addEdge(string v1, string v2, int weight)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].name == v1)
		{
			for (int j = 0; j < vertices.size(); j++)
			{
				if (vertices[j].name == v2 && i != j)
				{
					adjVertex av;
					av.v = &vertices[j];
					av.weight = weight;
					vertices[i].adj.push_back(av);
					//another vertex for edge in other direction
					adjVertex av2;
					av2.v = &vertices[i];
					av2.weight = weight;
					vertices[j].adj.push_back(av2);
				}
			}
		}
	}
}

// Function that adds a vertex to the list of vertices (also used in the driver main file to build the graph)
void Graph::addVertex(string n)
{
	bool found = false;
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].name == n)
		{
			found = true;
			cout << vertices[i].name << " found." << endl;
		}
	}
	if (found == false)
	{
		vertex v;
		v.name = n;
		vertices.push_back(v);
	}
}

// The function that runs at the start of every execution, that prints out the graph, with the vertex's name first, then --> and the adjacent vertices separated by ***
void Graph::displayEdges()
{
	for (int i = 0; i < vertices.size(); i++)
	{
		cout << vertices[i].name << "-->";
		for (int j = 0; j < vertices[i].adj.size(); j++)
		{
			cout << vertices[i].adj[j].v->name;
			if (j != vertices[i].adj.size() - 1)
				cout << "***";
		}
		cout << endl;
	}
}

// Function that takes in a string and searches to return the vertex that has the same string name
vertex *Graph::search(string v)
{
	for (int x = 0; x < vertices.size(); x++)
	{
		if (vertices[x].name == v)
		{
			return &vertices[x];
		}
	}
	return nullptr;
}

// Function that runs the Depth First Search recursive algorithm
void Graph::DFS(string name)
{
	vertex *v = search(name);
	cout << v->name << endl;
	DFSHelper(v);
}

// The runner for Depth First Search recursive algorithm
void Graph::DFSHelper(vertex *v)
{ // done
	v->visited = true;
	for (int i = 0; i < v->adj.size(); i++)
	{
		if (!v->adj[i].v->visited)
		{
			// cout << v->adj[i].v->visited << endl;
			DFSHelper(v->adj[i].v);
		}
	}
}

// The Breadth First Search algorithm, that takes in the names of two nodes, then gets the path to them
vertex *Graph::BFS(string startVal, string endVal)
{
	for (int x = 0; x < vertices.size(); x++)
	{
		vertices[x].visited = false;
		vertices[x].distance = 0;
		vertices[x].parent = nullptr;
	}
	//search for startVal
	vertex *v = search(startVal);
	v->visited = true;
	v->distance = 0;
	v->parent = nullptr;
	//add v to a queue
	queue<vertex *> q;
	q.push(v);
	while (!q.empty())
	{
		vertex *n = q.front(); //front returns a value
		cout << n->name << endl;
		q.pop();			   //pop removes from queue
		for (int x = 0; x < n->adj.size(); x++)
		{
			if (n->adj[x].v->visited == false)
			{
				n->adj[x].v->distance = n->distance + 1; //parent distance + 1
				n->adj[x].v->parent = n;
				//check if it's what we're looking for
				if (n->adj[x].v->name == endVal)
				{
					return n->adj[x].v;
				}
				else
				{
					n->adj[x].v->visited = true;
					q.push(n->adj[x].v);
				}
			}
		}
	}
	return nullptr;
}

// Function to do the Depth First Search iteratively, takes in a string name to find and it goes through and searches for it iteratively - fixed
void Graph::DFSIterative(string name)
{ // done
	vertex *searchV = search(name);
	searchV->visited = true;
	searchV->distance = 0;
	stack<vertex *> myStack;
	myStack.push(searchV);
	while (!myStack.empty())
	{
		vertex *myVertex = myStack.top(); // this way we can pop this (setting it equal doesn't work)
		cout << myVertex->name << endl;
		myStack.pop();
		// if (myVertex->visited = false)
		// {
		// 	myVertex->visited = true;
		// 	cout << myVertex->name << " was set to visited" << endl;
		// }
		for (adjVertex vIterator : myVertex->adj)
		{
			if (!vIterator.v->visited)
			{
				myStack.push(vIterator.v);
			}
		}
	}
}

// Function to check if every vertex is visited
bool Graph::visited()
{ // done
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].visited == false)
		{ // only return false if in the vector one of the values is false, meaning it hasn't been visited
			return false;
		}
	}
	return true; // if they are all true, that means they have all been visited so return true
}

// Function to get the minimum distance node - used in the dijkstra's algorithm
vertex *Graph::minimumDistanceNotVisited()
{ // done
	int lowestInt = INT32_MAX;
	vertex *iterator;
	for (int i = 0; i < vertices.size(); i++)
	{
		if (!vertices[i].visited)
		{
			if (vertices[i].distance < lowestInt)
			{
				lowestInt = vertices[i].distance;
				iterator = &vertices[i];
			}
		}
	}
	return iterator;
}

// The Dijkstra's algorithm - uses the minimumDistanceNotVisited() to get the vertex with the shortest path
void Graph::dijkstras(string v1)
{ // done - learned how to with recitation 10
	vertex *iterator = search(v1);
	iterator->distance = 0;
	while (!visited())
	{
		vertex *minimum = minimumDistanceNotVisited();
		minimum->visited = true;
		for (int i = 0; i < minimum->adj.size(); i++)
		{
			if (minimum->adj[i].v->distance > minimum->distance + minimum->adj[i].weight)
			{
				minimum->adj[i].v->distance = minimum->distance + minimum->adj[i].weight;
			}
		}
	}
	for (int j = 0; j < vertices.size(); j++)
	{
		cout << vertices[j].name << " is " << vertices[j].distance << " away." << endl;
		vertices[j].distance = INT32_MAX;
		vertices[j].visited = false;
	}
}

// Function that simply does a breadth first search without trying to get the shortest path
void Graph::BFSTraversal(string v)
{ // simply works layer by layer - done
	vertex *iterator = search(v);
	iterator->visited = true;
	queue<vertex *> q;
	q.push(iterator);
	while (!q.empty())
	{
		vertex *dequeued = q.front();
		q.pop();
		for (int i = 0; i < dequeued->adj.size(); i++)
		{
			if (!dequeued->adj[i].v->visited)
			{
				cout << dequeued->adj[i].v->distance << endl;
				dequeued->adj[i].v->visited = true;
			}
		}
	}
}

// Runner function that lets me build the Graph from the BST by adding edges connecting adjacent vertices
void Graph::buildEdgesFromGraphHelper(BST *node)
{
	if (!node->getRoot())
	{
		return;
	}
	buildEdgesFromGraph(node->getRoot());
}

// Recursive function that lets me build the Graph from the BST by adding edges connecting adjacent vertices
void Graph::buildEdgesFromGraph(BSTNode *node)
{
	if (node)
	{
		if (node->rightChild != nullptr)
		{
			addEdge(node->word, node->rightChild->word, 1);
			buildEdgesFromGraph(node->rightChild);
		}
		if (node->leftChild != nullptr)
		{
			addEdge(node->word, node->leftChild->word, 1);
			buildEdgesFromGraph(node->leftChild);
		}
	}
}

// Runner function that lets me build the Graph from the BST by adding the vertices
void Graph::buildVerticesFromGraphHelper(BST *node)
{
	if (!node->getRoot())
	{
		return;
	}
	buildVerticesFromGraph(node->getRoot());
}

// Recursive function that lets me build the Graph from the BST by adding the vertices
void Graph::buildVerticesFromGraph(BSTNode *node)
{
	if (node)
	{
		addVertex(node->word); // then add a vertex
		if (node->rightChild)
		{
			buildVerticesFromGraph(node->rightChild); // then recursively start over and make a vertice for the right child
		}
		if (node->leftChild)
		{
			buildVerticesFromGraph(node->leftChild); // then recursively start over and make a vertice for the left child
		}
	}
}