
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"
#include "cpu.h"



sum(double current_burst);
average();
print_averages();


struct node *head = NULL;
struct node *current;
int id = 1;
int count = 0;

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

void schedule(){
 
    current = head;
    reverse(current, head); 
    average();
    print_averages();     
}

reverse(struct node *current, struct node *head){
    if(current->next == NULL){
        run(current->task, current->task->burst);
        sum(current->task->burst);
        delete(&head, current->task);
        return;
    }
    else{
        reverse(current->next, head);
        run(current->task, current->task->burst);
        sum(current->task->burst);
        //printf("this is a test\n");
        delete(&head, current->task);

    }

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
    printf("Average waiting time = %.2f\n", wait_sum);
    printf("Average turnaround time = %.2f\n", turnaround_sum);
    printf("Average response time = %.2f\n", response_sum);
   // printf("Count:%d\n", count);
}



