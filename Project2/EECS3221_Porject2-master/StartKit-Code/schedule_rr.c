#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"
#include "cpu.h"


struct node *head = NULL;
struct node *current;
int id = 0;
int count = 0;
int size = 0;
int count2 = 0;

//variables for calculating the averages
int bursts[100];
int rem_bursts[100];
int wait[100] = {0};
int time = 0;


// adds the task to the list
void add(char *name, int priority, int burst){
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->tid = id;
    id++;
    insert(&head, newTask);
    bursts[count] = burst;
    rem_bursts[count] = burst;
    count++;
    count2++;
    size++;

}



void wait_sum(){
    int i = 0;
    
    while(count > 0){
            
            // run only if the remaing burst is > 0. i.e if the process is not yet finished
            if(rem_bursts[i] > 0){

                if(rem_bursts[i] > QUANTUM){
                    time = time + QUANTUM;
                    rem_bursts[i] -= QUANTUM;
                } 
                else{
                    time = time + rem_bursts[i];
                    wait[i] = time - bursts[i];
                    rem_bursts[i] = 0;
                    count--;

                }   
                i++;
                if(i == size) {
                    i = 0;
                }

            }  
            else{
                i++;
                if(i == size) {
                    i = 0;
                }
            }  

    }

}

//variables for calculating averages

double avg_wait = 0.0;
double avg_ta = 0.0;
double avg_response = 0.0;

averages(){
    int i;
    double previous_burst = 0.0;
    for(i = 0; i < size; i++ ){
        avg_wait = avg_wait + (double) wait[i];
        avg_ta = avg_ta + (wait[i] + bursts[i]);

        if(bursts[i] > QUANTUM){
            avg_response = avg_response + previous_burst;
            previous_burst = previous_burst + QUANTUM;
        }
        else{
            avg_response = avg_response + previous_burst;
            previous_burst = previous_burst + bursts[i];
        }
       // printf("bursts[%d]: %d\n", i, bursts[i]);


    }
    avg_wait = avg_wait / (double) size;
    avg_ta = avg_ta / (double) size;
    avg_response = avg_response / (double) size;
}



void rr_schedule(){

    current = head;
    reverse(current, head);

}

void schedule(){

//calculate the averages first
    wait_sum();
   // response_sum();
    averages();
// keep calling the rr_schedule until all the processes are executed    
     while(count2 > 0){
         rr_schedule();
     }
     printf("Average waiting time = %.2f\n", avg_wait);
     printf("Average turnaround time = %.2f\n", avg_ta);
     printf("Average response time = %.2f\n", avg_response);
      
}

reverse(struct node *current, struct node *head){
    if(current->next == NULL){
        if(current->task->burst > QUANTUM){
            run(current->task, QUANTUM);
            current->task->burst -= QUANTUM;

        }
        else{
            run(current->task, current->task->burst);
            delete(&head, current->task);
            count2--;

        }
        return;
    }
    else{
        reverse(current->next, head);
        if(current->task->burst > QUANTUM){
            run(current->task, QUANTUM);
            current->task->burst -= QUANTUM;

        }
        else{
            run(current->task, current->task->burst);
            delete(&head, current->task);
            count2--;

        }
    }

}




