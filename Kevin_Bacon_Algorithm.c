#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBEROFLINES 750//14129
#define MOVIENAMEBUFFER 500
#define NAMEBUFFER 150
#define LINEBUFFER 3800

char allMovies[NUMBEROFLINES][MOVIENAMEBUFFER];
char **allActors;
int noneRepeatActor=0;

int createData();
int createFiles();
int readFiles();
struct graph *initilizationOfGraph(int vertex);
void addEdge(struct graph* aGraph, int vertex1,int edge1,int vertex2, int edge2);
struct node* creatingNode(int actorIDP,int movieIDP);
int detectActor(char *actorName);
void BFSALGORITHM(struct graph *aGraph, int actorIndex, int searchIndex);
struct queueEl *newQueue(struct node *aNode);
struct queue *initializationOfQueue();
int isEmpty(struct queue *aQueue);
void enqueue(struct queue *q, struct node *value);
struct queueEl *dequeue(struct queue *q);
int detectBacon();

//It keeps actorID as a node and movieID as a vertex.
struct node
{
	int actorID;
	int movieID;
	int baconNumber;
	struct node *next;
};
//It keeps which actors cast which movie.
typedef struct
{
	int *movies;
	int movieNumber;
	int actorID;
}ACTORStoMOVIE;
//It keeps which movie has role which actor.
typedef struct 
{
	int *actors;
	int actorNumber;
	int movieID;
}MOVIEtoACTORS;

ACTORStoMOVIE *actors_to_movie;
MOVIEtoACTORS movie_to_actors[NUMBEROFLINES];
//It keeps adjanceny list,vertex number and visited nodes.
struct graph
{
	struct node **adjacencyList;
	int numberOfVertex;	
	int *visited;
};
//Elements of queue
typedef struct queueEl
{
    struct node value;
    struct queueEl *next;
}queueEl;
//Basic queue
struct queue
{
    struct queueEl *front;
    struct queueEl *rear;
};

//Option process, reading, creating, files and making bfs.
int main()
{
	int i,j,k;
	FILE *fp1;
	int choice;
	struct graph *aGraph;
	int actorIndex,movieIndex;
	struct node *aNode;
	int control;
	char *inputName=malloc(NAMEBUFFER*sizeof(char));
	char *inputName2=malloc(NAMEBUFFER*sizeof(char));
	int nameIndex,nameIndex2;
	int deepChoise;
	
	
	createData();
	createFiles();
	readFiles();
	
	printf("Enter 1 for start.Enter 0 for stop,please.\n");
    scanf("%d",&choice);
    
    while(choice)
	{
        aGraph=initilizationOfGraph(noneRepeatActor);
        
        for(i=0; i<noneRepeatActor; i++)
		{
            for(j=0; j<actors_to_movie[i].movieNumber; j++)
			{
                movieIndex=actors_to_movie[i].movies[j]; 
                
                for(k=0; k<movie_to_actors[movieIndex].actorNumber; k++)
				{
                    actorIndex=movie_to_actors[movieIndex].actors[k];
                    if(actorIndex != i) 
					{
                        aNode = aGraph->adjacencyList[i];
                        control=0;
                        
                        while((aNode != NULL) && (!control))
						{
                            if(aNode->actorID == actorIndex)
							{
                                control=1;
                            }
                            aNode=aNode->next;
                        }
                        if(!control)
						{
                            addEdge(aGraph, i, movieIndex, actorIndex, movieIndex);
                        }
                    }
                }
            }
        }
        
        printf("[MENU]Enter 2 if you want to choose Kevin Bacon comparision.\n");
        printf("[MENU]Enter 3 if you want to choose different actors comparison.\n");
        scanf("%d", &deepChoise);
        
        if(deepChoise==2)
        {
       		fgets(inputName,NAMEBUFFER,stdin);
        	printf("Enter actor name,please. ");
        	scanf ("%[^\n]%*c", inputName);
        	nameIndex=detectActor(inputName);
        	printf("\nThe index of actor is: %d",nameIndex);
        	BFSALGORITHM(aGraph,detectBacon("Bacon, Kevin"),nameIndex); 	
		}
		else if(deepChoise==3)
		{
			fgets(inputName,NAMEBUFFER,stdin);
        	printf("Enter first actor name,please. ");
        	scanf ("%[^\n]%*c", inputName);
        	printf("Enter second actor name,please. ");
        	scanf ("%[^\n]%*c", inputName2);
        	nameIndex=detectActor(inputName);
        	nameIndex2=detectActor(inputName2);
        	printf("\nThe index of first actor is: %d",nameIndex);
        	printf("\nThe index of second actor is: %d",nameIndex2);
        	BFSALGORITHM(aGraph,detectBacon(inputName2),nameIndex);
		}
       
        printf("\nDevam icin 1 cikis icin 0 giriniz \n");
        scanf("%d",&choice);
    }
    
    printf("\nKBacon's index is %d\n",detectBacon());
    for (i = 0; i < noneRepeatActor; i++) 
	{
        free(allActors[i]);
    }
    free(allActors);
    	
	return 0;
}
//Taking movies, actors from main data input.
int createData()
{
	FILE *graphFile,*inputFile;
	int lineCounter;
	char *line;
	char *tempMovies;
	char *tempActors;
	int i=0,k=0;
	int sizeOfMovies=0;
	
	allActors=(char **)malloc(sizeof(char *));
	allActors[0]=(char *)malloc(NAMEBUFFER*sizeof(char));
	
	graphFile=fopen("graph.txt","w");
	if(graphFile==NULL)
	{
		printf("[ERROR-1!]File can not opened.\n");
		exit(-1);
	}
	inputFile=fopen("input-mpaa.txt","rb");
	if(inputFile==NULL)
	{
		printf("[ERROR-2!]File can not opened.\n");
		exit(-2);
	}
	
	line=(char *)calloc(LINEBUFFER,sizeof(char));
	for(lineCounter=0; lineCounter<NUMBEROFLINES; lineCounter++)
	{
		fgets(line,sizeof(char)*LINEBUFFER,inputFile);
		line[strlen(line)-1]='\0';
		tempMovies=strtok(line,"/");
        
        fprintf(graphFile, "%d", k++);
        strcpy(allMovies[sizeOfMovies++],tempMovies);
        tempActors=strtok(NULL,"/");
        
        while(tempActors != NULL)
        {
        	
        	i=0;
        	while((i<noneRepeatActor) && (strncmp(allActors[i],tempActors,NAMEBUFFER)) != 0)
        	{
        		i++;
			}
			if(noneRepeatActor==i)
			{
				noneRepeatActor++;
				allActors=realloc(allActors,sizeof(char *)*noneRepeatActor);
				allActors[noneRepeatActor-1]=(char *)malloc(NAMEBUFFER*sizeof(char));
				strcpy(allActors[noneRepeatActor-1],tempActors);
				fprintf(graphFile, " %d", i);
			}
			else
			{
				fprintf(graphFile, " %d", i);
			}
			tempActors=strtok(NULL,"/");        	
		}
		fputc('\n',graphFile);
	}
	fclose(inputFile);
	fclose(graphFile);
	
}
//Seperating actors and movies from main data file.
int createFiles()
{
	FILE *movieFile,*actorFile;
	int i;
	
	movieFile=fopen("movies.txt","w");
	if(movieFile==NULL)
	{
		printf("[ERROR-3!]File can not opened.\n");
		exit(-3);
	}
	
	for(i=0; i<NUMBEROFLINES; i++)
	{
		fprintf(movieFile,"%d-%s\n",i,allMovies[i]);
	}
	fclose(movieFile);
	
	actorFile=fopen("actors.txt","w");
	if(actorFile==NULL)
	{
		printf("[ERROR-4!]File can not opened.\n");
		exit(-4);
	}
	
	fprintf(actorFile,"%d\n",noneRepeatActor);
	for(i=0; i<noneRepeatActor; i++)
	{
        fprintf(actorFile,"%d-%s\n",i,allActors[i]);
    }
    fclose(actorFile);
    
    return 0;
}

//Reading movie and actors from my own create files.
int readFiles()
{
	FILE *actorFile,*movieFile,*graphFile;
	char *line,*temp;
	int i,j=0,k;
	
	actorFile=fopen("actors.txt", "r");
	if(actorFile==NULL)
	{
		printf("[ERROR-5!]File can not opened.\n");
		exit(-5);
	}
	line=calloc(LINEBUFFER,sizeof(char));
	fscanf(actorFile,"%d",&noneRepeatActor);
	fgets(line,LINEBUFFER*sizeof(char),actorFile);
	allActors=(char**)(char**)malloc(sizeof(char*)*noneRepeatActor);
		
	for(i=0; i<noneRepeatActor; i++)
	{
        fgets(line,LINEBUFFER*sizeof(char),actorFile);
        line[strlen(line)-1]='\0';
        
        temp=strtok(line,"-");
        temp=strtok(NULL,"-");
        
        while(temp != NULL)
		{
            allActors[i] = (char*)malloc(NAMEBUFFER*sizeof(char));
            strcpy(allActors[i],temp);
            temp=strtok(NULL,"-");
        }
    }
    fclose(actorFile);
    
    movieFile=fopen("movies.txt","r");
    if(movieFile==NULL)
	{
		printf("[ERROR-6!]File can not opened.\n");
		exit(-6);
	}
	
	for(i=0; i<NUMBEROFLINES; i++)
	{
        fgets(line,LINEBUFFER*sizeof(char),movieFile);
        line[strlen(line)-1]='\0';
        
        temp=strtok(line,"-");
        temp=strtok(NULL,"-");
        
        while(temp != NULL)
		{
            strcpy(allMovies[i],temp);
            temp=strtok(NULL,"-");
        }
    }
    fclose(movieFile);
    
    graphFile=fopen("graph.txt","r");
    if(graphFile==NULL)
	{
        printf("[ERROR-7!]File can not opened.\n");
		exit(-7);
    }
    
    for(i=0; i<NUMBEROFLINES; i++)
	{
        fgets(line,LINEBUFFER*sizeof(char),graphFile);
        line[strlen(line)-1]='\0';
        temp=strtok(line," ");
        
        movie_to_actors[i].movieID=atoi(temp);
        temp=strtok(NULL," ");
        movie_to_actors[i].actors=(int *)calloc(1,sizeof(int));
        j=0;
        while(temp != NULL)
		{
            movie_to_actors[i].actors[j]=atoi(temp);
            j++;
            movie_to_actors[i].actors=(int *)realloc(movie_to_actors[i].actors,sizeof(int )*(j+1));
            temp=strtok(NULL," ");
        }

        movie_to_actors[i].actors=realloc(movie_to_actors[i].actors,sizeof(int)*j);
        movie_to_actors[i].actorNumber=j;
    }
    fclose(graphFile);
	
	actors_to_movie=(ACTORStoMOVIE *)malloc(sizeof(ACTORStoMOVIE)*noneRepeatActor);
	for(i=0; i<noneRepeatActor; i++)
	{
        actors_to_movie[i].movieNumber=0;
        actors_to_movie[i].actorID=i;
        actors_to_movie[i].movies=(int *)malloc(sizeof(int));
    }
    
     for(i=0; i<noneRepeatActor; i++)
	 {
        for(j=0; j<NUMBEROFLINES; j++)
		{
            for(k=0; k<movie_to_actors[j].actorNumber; k++)
			{
                if(movie_to_actors[j].actors[k] == i)
				{
                    actors_to_movie[i].movies=realloc(actors_to_movie[i].movies,sizeof(int)*(actors_to_movie[i].movieNumber + 1));
                    actors_to_movie[i].movies[actors_to_movie[i].movieNumber]=j;
                    actors_to_movie[i].movieNumber++;
                }
            }
        }
    }
    
    return 0;
}
//Initializing graph for the first time.
struct graph *initilizationOfGraph(int vertex)
{
    int i;
    struct graph *aGraph;
    
    aGraph=malloc(sizeof(struct graph));
    aGraph->numberOfVertex=vertex;
    aGraph->adjacencyList=malloc(sizeof(struct node *)*vertex);
    aGraph->visited=malloc(vertex*sizeof(int));
    
    for(i=0; i<vertex; i++)
	{
        aGraph->adjacencyList[i]=NULL;
        aGraph->visited[i]=0;
    }
    
    return aGraph;
}
//Adding an edge to nodes.
void addEdge(struct graph* aGraph, int vertex1,int edge1,int vertex2, int edge2)
{
    struct node *aNode = creatingNode(vertex2,edge2);
    aNode->next=aGraph->adjacencyList[vertex1];
    aGraph->adjacencyList[vertex1]=aNode;

    aNode = creatingNode(vertex1,edge1);
    aNode->next=aGraph->adjacencyList[vertex2];
    aGraph->adjacencyList[vertex2]=aNode;
}
//Creating a new node first time.
struct node* creatingNode(int actorIDP,int movieIDP)
{
    struct node *aNode = malloc(sizeof(struct node));
    aNode->actorID=actorIDP;
    aNode->movieID=movieIDP;
    aNode->next=NULL;
    aNode->baconNumber=0;
    
    return aNode;
}
//Detecting actor.
int detectActor(char *actorName)
{
    int i=0;
    
    while(i<noneRepeatActor && strcmp(actorName,allActors[i]) != 0)
    {
    	i++;
	}
    if(i==noneRepeatActor)
	{
        printf("[ERROR-8!]The actor could not find.\n");
		exit(-8);
    }
    else
	{
        return i;
    }
}
//Bfs algroithm, keeping track of path.
void BFSALGORITHM(struct graph *aGraph, int actorIndex, int searchIndex)
{
    int i,j,count=0,found=1;
    int *track;
    
    track=malloc(sizeof(int));
    struct queue *aQueue=initializationOfQueue();
    
    aGraph->visited[actorIndex]=-1;
    struct node first;
    first.actorID=actorIndex;
    first.baconNumber=0;
    first.next=NULL;
    
    enqueue(aQueue,&first);
    
    while(!isEmpty(aQueue))
	{
        struct queueEl *temp1 = dequeue(aQueue);
        track=realloc(track,(count+1)*sizeof(int));
        track[count++]=temp1->value.actorID;

        struct node *aNode = aGraph->adjacencyList[temp1->value.actorID];
                
        while(aNode)
		{
			
			
            if(aGraph->visited[aNode->actorID]==0)
			{
				
                aNode->baconNumber = temp1->value.baconNumber+1;
                aGraph->visited[aNode->actorID]=aNode->baconNumber;
                enqueue(aQueue,aNode);
            }
            
            if(searchIndex==aNode->actorID && aGraph->visited[aNode->actorID] == aNode->baconNumber)
			{
                found=0;
                printf("\n%s 's Kevin Bacon number is %d\n",allActors[aNode->actorID],aNode->baconNumber);
                int actorID1 = aNode->actorID;
                int actorBN1 =aNode->baconNumber;
                int actorID2;
                int actorBN2;
                
                struct node *temp;
                
                for(i=0; i<count; i++)
				{
                    int found=0;
                    int tmpID;
                    actorID2=track[count-i-1];
                    actorBN2=aGraph->visited[actorID2];
                    struct node *temp = aGraph->adjacencyList[actorID2];
                    if(actorBN2==(actorBN1-1) || (actorBN2==-1))
					{
                        while(temp && !found)
						{
                            tmpID = temp->actorID;
                            if(tmpID == actorID1)
							{
                                printf("%s-%s : %s \n",allActors[actorID1],allActors[actorID2],allMovies[temp->movieID]);
                                found=1;
                                actorID1=actorID2;
                                actorBN1=actorBN2;
                            }
                            temp=temp->next;
                        }
                    }
                }
                system("PAUSE");
            }
            aNode=aNode->next;
        }
    }
    if(found)
	{
        printf("\nInfinite\n");
        system("PAUSE");
    }
}
//Creating queue first time.
struct queueEl *newQueue(struct node *aNode)
{
    struct queueEl *aQueue;
    aQueue=(struct queueEl *)malloc(sizeof(struct queueEl));
    aQueue->value=*aNode;
    aQueue->next=NULL;
    
    return aQueue;
}
//Initializaing queue.
struct queue* initializationOfQueue()
{
    struct queue *q=(struct queue *)malloc(sizeof(struct queue));
    q->front=NULL;
	q->rear=NULL;
	
    return q;
}
//Check if queue is empty.
int isEmpty(struct queue *aQueue)
{
    if(aQueue->front==NULL)
	{
        return 1;
    }
    else
	{
        return 0;
    }
}
//Enqueue process
void enqueue(struct queue *q, struct node *value)
{
    struct queueEl *aElement=newQueue(value);
    aElement->value=*value;
    aElement->next=NULL;
    if (q->rear==NULL)
	{
	   q->front=aElement;
	   q->rear=aElement;
    }
    else
	{
        q->rear->next=aElement;
        q->rear=aElement;
    }
}
//Dequeue process
struct queueEl *dequeue(struct queue *q)
{
    if(q->front==NULL)
	{
        return NULL;
    }
    
    struct queueEl *temp = q->front;
    q->front=q->front->next;
    if(q->front== NULL)
    {
    	q->rear=NULL;
	}
        
    return temp;
}
//Detecting Kevin Bacon function
int detectBacon(char *name)
{
    int i=0;
    
    while(i<noneRepeatActor && strcmp(name, allActors[i]) != 0)
    {
    	i++;
	}
    if(i==noneRepeatActor)
    {
    	return -1;
	}
    else
    {
    	return i;
	}
        
}
