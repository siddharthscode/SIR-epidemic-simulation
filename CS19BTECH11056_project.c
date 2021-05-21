//NAME: Siddharth Singh Ahluwalia
//ROLL NO.:CS19BTECH11056
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#define MAX 99999
#define MAX_EDGE 3000
#define MAX_VERTICE 10000

/*-------graph functions------*/
int edge;  //random value of MAX_EDGE
int vertice;  //random value of MAX_VERTICE
int random_edge()  //function gives random value of edge
{
	srand(time(NULL));
	return rand()%MAX_EDGE;
}
int random_vertice()  //function gives random value of vertice
{
	srand(time(NULL));
	return rand()%MAX_VERTICE;
}
struct graph_node  //structure represents graph node
{
	int key;
	char *status;
	int pred_infection_time;
	int recover_time;
	struct graph_node** neighbour;
};
struct graph_node* graph;  //represents graph

void add_edge(int source, int e, int destination)  //links between source and destination
{
	graph[source].neighbour[e] = &graph[ destination ];  //link from source to destination

	int j = 0;
	while(graph[destination].neighbour[j])  //link from destination to source
	{
		j++;
	}

	if(j < edge)
	{
		graph[destination].neighbour[j] = &graph[source];
	}
	else  //if destination node already exceeds its degree then no link
	{
		graph[source].neighbour[e] = NULL;
	}
}

struct graph_node* make_graph(int vertice, int edge)  
{
	struct graph_node* temp = (struct graph_node*)malloc(vertice * sizeof(struct graph_node));  //allocates space for node

	for(int i=0; i<vertice; i++)
	{
		temp[i].key = i;
		temp[i].neighbour = NULL;
	}

	for(int i=0; i<vertice; i++)
	{
		temp[i].neighbour = (struct graph_node**)malloc(edge * sizeof(struct graph_node*));  //allocates space for containing edges
	}
	for(int i=0; i<vertice; i++)  //putting null pointer in it
	{
		for(int j=0; j<edge; j++)
		{
			temp[i].neighbour[j] = NULL;
		}
	}

	return temp;
}

void make_link()  //links the graph
{
	for(int i=0; i<vertice; i++)
	{
		for(int j=0; j<edge; j++)
		{
			if(graph[i].neighbour[j] == NULL)  
			{
				if(rand()%2 == 1)  //make link 
				{
					int dest = rand() % vertice;
					int k;
					for(k=0; k<j; k++)  //checks that no node repeats
					{
						if(graph[i].neighbour[k] != NULL)
						{
							if(graph[i].neighbour[k]->key == dest || graph[i].neighbour[k]->key == i)
								break;
						}
					}
					if(k == j)
					{
						add_edge(i, j, dest);
					}
				}
			}
		}
	}
}

bool graph_neighbour(int src, int dest)  //checks whether source and destination are neighbours or not
{
	for(int j=0; j<edge; j++)
	{
		if(graph[src].neighbour[j] != NULL)
		{
			if(graph[src].neighbour[j]->key == dest)
			{
				return true;
				break;
			}
		}
	}
	return false;
}

/*------end graph functions--------*/

/*----------lists functions-----------*/
struct list  //represents list node
{
	int size;
	struct list* next;
};

struct list *S, *R, *I, *times;

void insert(struct list** x, int node)  //inserts the node at last in list x
{
	if((*x) == NULL)  //if list is empty
	{
		(*x) = (struct list*)malloc(sizeof(struct list));
		(*x) -> size = node;
		(*x) -> next = NULL;
	}
	else  //if list is not empty
	{
		struct list* temp = (*x);

		while(temp->next)
		{
			temp = temp->next;
		}
		temp->next = (struct list*)malloc(sizeof(struct list));
		(temp->next)->size = node;
		(temp->next)->next = NULL;
	}
}

int last(struct list* x)  //gives value of last node in list x
{
	struct list* temp = x;
	while(temp->next)
	{
		temp = temp->next;
	}
	return temp->size;
}

struct initial  //structure that contain initial infected nodes
{
	int* node;  //array
	int size;  //size of array
};
struct initial* initialptr;  //pointer to initial infected list

void insert_initial(int node)  //adds node in list
{
	initialptr->node[initialptr->size] = node;
	initialptr->size = initialptr->size + 1;
}

bool is_initial_infected(int node)  //checks whether node is initial infected or not
{
	for(int i=0; i<(initialptr->size); i++)
	{
		if(initialptr->node[i] == node)
		{
			return true;
			break;
		}
	}
	return false;
}
/*-----end lists functions----------*/

/*--------event functons-----------*/
struct event  //structure represents event
{
	int time;  //time that event occurs
	char *action;  //what event do
	struct graph_node* node;  //pointer to graph node
};

/*--------end event functions------------*/

/*----------priority queue functions-------*/

struct queue  //structure represents queue node
{
	struct event* eventptr;  //pointer to event
	struct queue* next;  //pointer to next node
};
struct queue* Queue;  //pointer to queue

struct event* make_event(int node, int time, char* action)  //make a new event
{
	struct event* eventptr = (struct event*)malloc(sizeof(struct event));
	eventptr->node = &graph[node];
	eventptr->action = action;
	eventptr->time = time;

	return eventptr;
}

void add_event(struct queue** Q, struct event* Event)  //adds event in queue
{
	struct queue* new = (struct queue*)malloc(sizeof(struct queue));  //allocates space for new node
	new -> eventptr = Event;  
	new -> next = NULL;
	struct queue* temp = (*Q);
	if((*Q) != NULL)  //if queue is not empty
	{
		if((*Q)->eventptr->time > Event->time)
		{
			(*Q) = new;
			new->next = temp;
		}
		else
		{
			bool condition()
			{
				if(temp->next != NULL)
				{
					if((temp->next)->eventptr->time < Event->time)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			while(condition())
			{
				temp = temp->next;
			}
			new->next = temp->next;
			temp->next = new;
		}
	}  //if queue is empty
	else
	{
		(*Q) = new;
		new->next = NULL;
	}
}

struct event earliest_event(struct queue** Q)  //extracts event that has lowest time to occur
{
	struct queue* temp = (*Q);
	(*Q) = (*Q)->next;
	struct event early_event  = *(temp->eventptr);
	return early_event;
}

/*---------end priority queue functions-------*/

/*----------important functions---------*/

int recovery_time()  //gives recover time
{
	int days = 1;
	while(1)
	{
		if(rand() % 100 < 20) //probability 0.2
			break;
		else
			days++;
	}
	return days;
}

int infection_time()  //gives infected time
{
	int days = 1;
	while(1)
	{
		if(rand() % 100 < 50)  //probability 0.5
			break;
		else
			days++;
	}
	return days;
}

int min(int x, int y, int z)  //gives minimum value
{
	int minimum = x;
	if(minimum > y)
		minimum = y;
	if(minimum > z)
		minimum = z;
	return minimum;
}


void find_trans_SIR(struct event* Event, int source, int target, int max_time)  //function that transmits the infection from source node to target node
{
	if(graph[target].status == "susceptible")  //if targeted node is not infected
	{
		int infect_time = Event->time + infection_time();  //gives infected time

		if( infect_time < min( graph[source].recover_time, graph[target].pred_infection_time, max_time ) )  //so target gets infection before the recover of source node and time max
		{
			struct event* new_eventptr = make_event(target, infect_time, "transmit");  //create new event

			add_event(&Queue, new_eventptr);  //adds new event in queue
			new_eventptr->node->pred_infection_time = infect_time;  //updates predected infection time for target node
		}
	}
}

void process_trans_event(struct event* Event, int max_time)  //function that process the transmission event
{
	
	Event->node->status = "infected";  //node is infected

	insert(&times, Event->time);  //adds time of event
	insert(&S, last(S)-1);  //node is deleted from suspectible list
	insert(&I, last(I)+1);  //node is add in infection list
	insert(&R, last(R));  //no change in recover list

	Event->node->recover_time = Event->time + recovery_time();  //finds recover time

	if(Event->node->recover_time < max_time)  //if recover time is less than time max
	{
		struct event* new_eventptr = make_event(Event->node->key, Event->node->recover_time, "recover");  //creates new event
		add_event(&Queue, new_eventptr);  //adds event in queue
	}
	for(int i=0; i<vertice ; i++)  //finds neighbour nodes
	{
		if(graph_neighbour(Event->node->key, i))  //if node 'i' is neighbour of infected node
		{
			find_trans_SIR(Event, Event->node->key, i, max_time);  //transmit infection to that node
		}
	}
}


void  process_rec_SIR(struct event* Event, int max_time)  //process recover event
{
	Event->node->status = "recovered";  //node is recovered
	insert(&times, Event->time);  //adds event itme
	insert(&S, last(S));  //no change in suspectible list
	insert(&I, last(I)-1);  //delete node from infection list
	insert(&R, last(R)+1);  //adds node in recover list
}

void fast_SIR(int max_time)  //functon that analysis infection spread by S, I, R list
{
	S->size = vertice;
	I->size = 0;
	R->size = 0;
	times->size = 0;

	for(int i=0; i<vertice; i++)  //making all graph nodes susceptible
	{
		graph[i].status = "susceptible";
		graph[i].pred_infection_time = MAX;
		
	}
	for(int i=0; i<vertice; i++)  
	{
		if(is_initial_infected(i))  //if initial infected
		{			
			struct event* Eventptr = make_event(i, 0, "transmit");  //make transmission event
			graph[i].pred_infection_time = 0;  
			add_event(&Queue, Eventptr);  //adds event in queue
		}
	}

	while(Queue != NULL)  //if queue is not empty
	{
		struct event early_event = earliest_event(&Queue);  //gives event

		if(early_event.action == "transmit")  //if event if transmission
		{
			if(early_event.node->status == "susceptible")  //if node for that event is not already infected
			{
				process_trans_event(&early_event, max_time);  
			}
		}
		else  //if event is recovery
		{
			process_rec_SIR(&early_event, max_time);
		}
	}

}

/*------end functions-----------*/

int main()
{
	srand(time(NULL));

	int max_time = 300;  

	edge = random_edge();
	vertice = random_vertice();

	S = (struct list*)malloc(sizeof(struct list));
	I = (struct list*)malloc(sizeof(struct list));
	R = (struct list*)malloc(sizeof(struct list));
	times = (struct list*)malloc(sizeof(struct list));

	graph = make_graph(vertice, edge);  
	make_link();

	initialptr = (struct initial*)malloc(sizeof(struct initial));
	initialptr->size = 0;

	int initial_infected_people = rand() % vertice;

	initialptr->node = (int*)malloc((initial_infected_people) * sizeof(int));

	for(int i=0; i < (initial_infected_people); i++)
	{
		insert_initial( rand() % vertice );
	}

	fast_SIR(max_time);

	
	struct list* tempt = times;
	struct list* temps = S;
	struct list* tempi = I;
	struct list* tempr = R;

	while(tempt)
	{
		printf("time: %d,   Suspected: %d,   Infected: %d,   Recovered: %d\n", tempt->size, temps->size, tempi->size, tempr->size);
		tempt = tempt->next;
		temps = temps->next;
		tempi = tempi->next;
		tempr = tempr->next;

	}
	
	free(graph);
	free(Queue);
	free(initialptr);
	free(S);
	free(I);
	free(R);
	free(times);

	return 0;
}