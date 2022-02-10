#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* * Process: Each process has a name and has an execution time.
*          We keep track of how long the process still needs
*          to run on the processor. "remaining_time" is the
*          remaining execution time of the process. */
typedef struct
{
    char *name;
    int starting_time;
    int remaining_time;
} Process;

/* * */

typedef struct Node
{
    Process data;
    struct Node* next;
} Node;

/* * */

typedef struct
{
    Node *head;
} Queue;

/* * */

Queue* init()
{
    Queue *q=malloc(sizeof(Queue));
    q->head=NULL;
    return q;
}

/* * */

int isEmpty(Queue *q)
{
    if(q->head==NULL)
        return 1;
    else
        return 0;
}

/* * */

Process dequeue(Queue *q)
{
    Process val;
    if(q->head==NULL)
    {
        printf("Error");
        exit(-1);
    }
    else
    {
        Node *n=q->head;
        val=n->data;
        q->head=n->next;
        free(n);
    }
    return val;
}

/* * */

void enqueue(Queue *q, Process val)
{
    Node* n=malloc(sizeof(Node));
    n->data=val;
    n->next=NULL;
    if(q->head==NULL)
    {
        q->head=n;
    }
    else
    {
        Node *temp;
        temp=q->head;
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next=n;
    }
}
/* * */
void destroy(Queue *q)
{
    Node *n=q->head;
    while(q->head!=NULL)
    {
        q->head=n->next;
        free(n);
    }
    free(q);
}
/* * RoundRobin Scheduling */
void RoundRobin(char* filename)
{
    int time_slots;
    int lines=0;
    int i=-1;
    char line[50];
    FILE *fp;
    fp=fopen(filename,"r");
    while(fgets(line,50, fp))
    {
        if(i==-1)
        {
            char *token=strtok(line," ");
            while(token)
            {
                time_slots=atoi(token);
                token=strtok(NULL," ");
            }
        }
        lines++;
        i++;
    }
    fseek(fp,0L,SEEK_SET);
    Process p[lines-1];
    i=-1;
    while (fgets(line,50,fp))
    {
        if(i > -1 )
        {
            char *token=strtok(line," ");
            p[i].name=malloc(strlen(token)+1);
            strcpy(p[i].name,token);
            token=strtok(NULL," ");
            p[i].starting_time=atoi(token);
            token=strtok(NULL," ");
            p[i].remaining_time=atoi(token);
        }
        i++;
    }
    fclose(fp);
    int counter=0;
    int processes=lines-1;
    Queue *q=init();
    for(i=0; i<processes-1; i++) //Sorting processes according to starting time
    {
        for(int j=0; j<processes-1-i; j++)
        {
            if(p[j].starting_time>p[j+1].starting_time)
            {
                Process temp=p[j];
                p[j]=p[j+1];
                p[j+1]=temp;
            }
        }
    }
    int m,flag;
    while(counter<=time_slots)
    {
        flag=0;
        for(m=0; m<processes; m++)
        {
            if(p[m].starting_time<=counter && p[m].remaining_time>0)
            {
                flag=1;//flag to prevent adding again to counter
                p[m].starting_time=counter++;
                enqueue(q,p[m]);
                p[m].remaining_time-=1;
            }
            else if(p[m].starting_time>counter && p[m].remaining_time>0)
            {
                break;//when there is an element that has starting time less than counter, break the for loop
            }
            if(counter==time_slots)
                break;
        }
        if(flag==0)
            counter++;
    }
    counter=0;
    printf("\n");
    while(!isEmpty(q))
    {
        Process temp=dequeue(q);
        while(temp.starting_time>counter)//printing idle
        {
            printf("idle    (%d-->%d)\n",counter,counter+1);
            counter++;
        }
        if(temp.starting_time<=counter)//printing processes
        {
            printf("%s      (%d-->%d)",temp.name,temp.starting_time,temp.starting_time+1);
            if(temp.remaining_time==1)
                printf(" %s aborts",temp.name);
            printf("\n");
            counter++;
        }
    }
    while(counter<time_slots)
    {
        printf("idle    (%d-->%d)\n",counter,counter+1);
        counter++;
    }
    for(int i=0; i<processes; i++)
    {
        free(p[i].name);
    }
    printf("stop\n\n");
    destroy(q);
}

/* * */

int main()
{
    char filename[261];
    puts("Enter file name or Ctrl+Z to exit:");
    puts("----------------------------------");
    while(fgets(filename, 260, stdin) != NULL)
    {
        filename[strlen(filename)-1]='\0';
        if(fopen(filename,"r"))
            RoundRobin(filename);
        else
        {
            puts("File Not Found!");
            puts("----------------------------------");
        }
        puts("Enter file name or Ctrl+Z to exit:");
        puts("----------------------------------");
    }
    return 0;
}
