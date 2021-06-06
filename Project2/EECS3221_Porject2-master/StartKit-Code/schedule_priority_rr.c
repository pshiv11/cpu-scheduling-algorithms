#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *head = NULL;
struct node *current = NULL;


// pointer to the struct containing the next task
struct node *tmp;

int same_priority = 1;//counts how many processes have the same priority
int id = 0, count;
int bursts[100] = {0};
int rem_bursts[100] = {0};
int wait[100] = {0};
int time = 0;
double avg_wait = 0.0;
double avg_ta = 0.0;

Task *pickNextTask();
averages();

// add a new task to the list of tasks
void add(char *name, int priority, int burst) {
    // create the new task
    Task *newTask = (Task *) malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->tid = id++;
    bursts[count] = burst;
    rem_bursts[count] = burst;
    count++;

    // add it to the list
    insert(&head, newTask);
}


 //Run the scheduler

void schedule() 
{
    Task *current;
    //Task *previous;

    current = head;

    while (head != NULL) {
        current = pickNextTask();
        if (same_priority > 1){
            //tuplicate priorities exists,therefore run as round robin
            
            if (current->burst > QUANTUM) {
                
                run(current, QUANTUM);
                time+= QUANTUM;
                current->burst -= QUANTUM;
                delete(&head, current);
                insert(&head, current);
            }
            else {
                run(current, current->burst);
                time+= current->burst;
                wait[current->tid] = time - bursts[current->tid];      
                delete(&head, current);
            }
        }
        else{
            //no duplicate priorities
            run(current, current->burst);
            time+= current->burst;
            wait[current->tid] = time - bursts[current->tid];  
            delete(&head, current);

        }
    }
    averages();
    printf("Average waiting time = %.2f\n", avg_wait);
    printf("Average turnaround time = %.2f\n", avg_ta);
    
}



averages(){
    int i;
    for(i = 0; i < count; i++){
        avg_wait += wait[i];
        avg_ta = avg_ta + (wait[i] + bursts[i]);
    }
    avg_wait /= (double) count;
    avg_ta /= (double) count;

}

 // Returns the next task selected to run.
Task *pickNextTask()
{
    
    struct node *current;
    current = head;
    Task *highest;
    highest = current->task;

    same_priority = 1;  // is same priority ?

    
    while(current->next != NULL){
        current = current->next;
        if(current->task->priority > highest->priority){
            highest = current->task;
            same_priority = 1; // reset the duplicate counter as another task with higher priority exist    
        }
        else if(current->task->priority == highest->priority){
            same_priority++;
            highest = current->task;

        }

    }
    return highest;

 
}