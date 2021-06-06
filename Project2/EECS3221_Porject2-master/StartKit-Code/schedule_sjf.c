#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"
#include "cpu.h"

//declaration of functions
sum(double current_burst);
average();
print_averages();



// attributes 
struct node *head = NULL;
struct node *current;
int id = 1;
int count = 0;
struct node *shortest;


// adds the task to the list

void add(char *name, int priority, int burst){
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->tid = id;
    id++;
    insert(&head, newTask);
    count++;

}


//pickNext function will select the shortest job first
pickNext(){
   
        current = head;
        shortest = current;
        while(current->next != NULL){
            current = current->next;
            if(current->task->burst <= shortest->task->burst){
                shortest = current;
            }
            
        }
        
}


//schedule() loops over the list and repeatedly calls pickNext() until the list is empty
void schedule(){

    while(head != NULL){
        pickNext();
        run(shortest->task, shortest->task->burst);
        sum(shortest->task->burst);
        delete(&head, shortest->task);
        
    }  
    average();
    print_averages(); 
    
}


double wait_sum = 0; double turnaround_sum = 0; double response_sum = 0;
double previous_ta = 0; double current_ta;
double previous_wait = 0; double current_wait;
double response_time;
double previous_burst;


sum(double current_burst){

    current_ta = previous_ta + current_burst;
    previous_ta = current_ta;
    turnaround_sum = turnaround_sum + current_ta;
    response_sum = response_sum + previous_burst;
    previous_burst = previous_burst + current_burst;
    current_wait = current_ta - current_burst;
    wait_sum = wait_sum + current_wait;
    
}

average(){
    wait_sum = wait_sum / (double) count;
    turnaround_sum = turnaround_sum / (double) count;
    response_sum = response_sum / (double) count;
}

print_averages(){
    printf("Avg wait: %f\n", wait_sum);
    printf("Avg ta: %f\n", turnaround_sum);
    printf("Avg response: %f\n", response_sum);
    //printf("Count:%d\n", count);
}

