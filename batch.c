#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int num_processes;

struct batch_table {
    int id;
    int arrival;
    int total_cpu;
    int total_remaining;
    int done;
    int start_time;
    int end_time;
    int turnaround_time;
    int already_started;
}*table_ptr=NULL;


typedef struct batch_table table_type;

#define max(a,b)(a>b ? a : b)

void print_table(){
    
    printf("ID\tArrival\tTotal\tStart\tEnd\tTurnaround\n");
    
    for(int i =0; i < num_processes; i++){
        printf("%d\t%d\t%d\t", table_ptr[i].id, table_ptr[i].arrival, table_ptr[i].total_cpu);
        if(table_ptr[i].done == 1){
            printf("%d\t%d\t%d\t", table_ptr[i].start_time, table_ptr[i].end_time, table_ptr[i].turnaround_time);
        }
        printf("\n");
    }
    
    return;
    
}

void num_processes_init(){
    printf("Enter the total number of processes: ");
    scanf("%d", &num_processes);
    printf("\n");
    
    table_ptr = (table_type*)malloc(num_processes*sizeof(table_type));
    
    for(int i =0; i < num_processes; i++){
        printf("Enter process id: ");
        scanf("%d", &table_ptr[i].id);
        printf("Enter arrival cycle for Process %d: ", table_ptr[i].id);
        scanf("%d", &table_ptr[i].arrival);
        printf("Enter total CPU cycles for Process %d: ", table_ptr[i].id);
        scanf("%d", &table_ptr[i].total_cpu);
        
    }
    
    print_table();
    
    return;
    
}

void fifo(){
    int num_done = 0;
    int earliest_arrival;
    int earliest_index;
    int current_cycle = 0;
    
    for(int i = 0; i < num_processes; i++){
        table_ptr[i].done = 0;
    }
    
    while(num_done < num_processes){
        earliest_arrival = INT_MAX;
        
        for(int i = 0; i < num_processes; i++){
            if(table_ptr[i].done == 0){
                if(table_ptr[i].arrival < earliest_arrival){
                    earliest_arrival = table_ptr[i].arrival;
                    earliest_index = i;
                }
            }
        }
    
        table_ptr[earliest_index].start_time = max(table_ptr[earliest_index].arrival, current_cycle);
        table_ptr[earliest_index].end_time = table_ptr[earliest_index].start_time + table_ptr[earliest_index].total_cpu;
        table_ptr[earliest_index].turnaround_time = table_ptr[earliest_index].end_time - table_ptr[earliest_index].arrival;
        table_ptr[earliest_index].done = 1;
        current_cycle = table_ptr[earliest_index].end_time;
        num_done++;
        
    }
    print_table();
    return;
    
}

void sjf(){
    int num_done = 0;
    int shortest_job;
    int shortest_index;
    int current_cycle = 0;
    int at_least_one;
    
    for(int i = 0; i < num_processes; i++){
        table_ptr[i].done = 0;
    }
    
    while(num_done < num_processes){
        shortest_job = INT_MAX;
        at_least_one = 0;
        
        for(int i = 0; i < num_processes; i++){
            if(table_ptr[i].done == 0){
                if((table_ptr[i].total_cpu < shortest_job)&&(table_ptr[i].arrival <= current_cycle)){
                    shortest_job = table_ptr[i].total_cpu;
                    shortest_index = i;
                    at_least_one = 1;
                }
            }
        }
        
        if(at_least_one != 0){
            table_ptr[shortest_index].start_time = max(table_ptr[shortest_index].arrival, current_cycle);
            table_ptr[shortest_index].end_time = table_ptr[shortest_index].start_time + table_ptr[shortest_index].total_cpu;
            table_ptr[shortest_index].turnaround_time = table_ptr[shortest_index].end_time - table_ptr[shortest_index].arrival;
            table_ptr[shortest_index].done = 1;
            num_done++;
            current_cycle = table_ptr[shortest_index].end_time;
        }
        else{
            current_cycle++;   
        }

    }
    
    print_table();
    return;
    
}


void srt(){
    int num_done = 0;
    int shortest_remaining;
    int shortest_index;
    int current_cycle = 0;
    int at_least_one;
    
    for(int i = 0; i < num_processes; i++){
        table_ptr[i].done = 0;
        table_ptr[i].already_started = 0;
        table_ptr[i].total_remaining = table_ptr[i].total_cpu;
    }
    
    while(num_done < num_processes){
        shortest_remaining = INT_MAX;
        at_least_one = 0;
        
        for(int i = 0; i < num_processes; i++){
            if(table_ptr[i].done == 0){
                if((table_ptr[i].total_remaining < shortest_remaining)&&(table_ptr[i].arrival <= current_cycle)){
                    shortest_remaining = table_ptr[i].total_remaining;
                    shortest_index = i;
                    at_least_one = 1;
                }
            }
        }
        
        if(at_least_one != 0){
            if(table_ptr[shortest_index].already_started == 0){
                table_ptr[shortest_index].start_time = max(table_ptr[shortest_index].arrival, current_cycle);
                table_ptr[shortest_index].already_started = 1;
            }
            table_ptr[shortest_index].end_time = current_cycle + 1;
            table_ptr[shortest_index].turnaround_time = table_ptr[shortest_index].end_time - table_ptr[shortest_index].arrival;
            table_ptr[shortest_index].total_remaining--;
            if(table_ptr[shortest_index].total_remaining == 0){
                table_ptr[shortest_index].done = 1;
                num_done++;
            }
        }
        current_cycle++;
    }
    print_table();
    return;
    
}

void free_quit(){
    if(table_ptr != NULL){
        free(table_ptr);
        printf("Memory has been freed.\n");
    }
    printf("Quitting program...\n");
    return;
}

int main() {
    int choice = 0;
    while(choice != 5) {
        printf("\n\nBatch Scheduling \n ---------------- \n 1) Enter parameters \n 2) Schedule processes with FIFO algorithm \n 3) Schedule processes with SJF algorithm \n 4) Schedule processes with SRT algorithm  \n 5) Quit and free memory \n");
        printf("\n Enter Selection: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: printf("\nProcess Initialization (Option 1) selected. \n\n"),num_processes_init(); break;
            case 2: printf("\nFirst-In-First-Out Scheduling (Option 2) selected. \n\n"),fifo(); break;
            case 3: printf("\nShortest Job First Scheduling (Option 3) selected. \n\n"),sjf(); break;
            case 4: printf("\nShortest Remaining Time Scheduling (Option 4) selected.  \n\n"),srt(); break;
            case 5: printf("\nFree Memory and Quit (Option 5) selected.  \n\n"),free_quit(); break;
            default: printf("\nInvalid option!\n"); break;
        }
    }
    return 0;
}