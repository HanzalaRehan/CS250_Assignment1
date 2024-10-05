/*
Author(s): 1. Hanzala B. Rehan
Description: A simulated CPU Process Scheduling Algorithm using a Linked List.
Date created: October 4th, 2024.
Date last modified: October 4th, 2024.
*/
#include <iostream>
#include <string>
using namespace std;

class Process {
    // Defining a process class based on a linked list node.
public:
    // Declaring different variables
    string id;      // Process Id
    int exec_time;  // Total execution time
    int rem_time;   // Remaining execution time
    Process* next;  // Pointer to the next process

    Process(string process_id, int total_time) {
        // Constructor initializing all the variables.
        id = process_id;
        exec_time = total_time;
        rem_time = exec_time;
        next = nullptr;
    }

    void process(int cycle_time) {
        /*
        Desc: Simulates a process by decrementing the remaining time, by the cpu cycle time.
        Parameters:
            cycle_time (int): CPU cycle time.
        */
        rem_time -= cycle_time;
        if (rem_time < 0) rem_time = 0; // Remaining time can not be negative, hence it stops at 0.
    }

    bool has_ended() {
        /*
        Desc: Checks if the process has completed or not, based on the remaining time.
        returns:
        (bool): true if process has completed else false
        */
        return rem_time == 0;
    }
};

class Scheduler {
    // the scheduler algorithm based on circular linked list.
public:
    // Variables declaration.
    int cpu_time;   // fixed amount of CPU time to each process in each cycle
    int total;      // Total number of processes in the scheduler
    int rem;        // Remaining number of processes in the scheduler
    int cycles;     // Number of cycles, the scheduler has gone through
    Process* tail;  // pointer to last node

    Scheduler(int Cpu_time) {
        // Constructor to initialize all variables.
        cpu_time = Cpu_time;
        total = 0;
        rem = 0;
        cycles = 0;
        tail = nullptr;
    }

    void addProcess(int exec_time) {
        /*
        Desc: Adds a new process in to the scheduler, using the same logic as in circular linked list.
                Also increments total and rem.
        Parameters:
            exec_time (int): execution time required for the process.
        */
        total += 1;
        rem += 1;
        string id = 'P' + to_string(total);
        Process* new_node = new Process(id, exec_time);
        if (tail == nullptr) {
            tail = new_node;
            tail->next = tail;  // Circular link (points to itself)
        } else {
            new_node->next = tail->next;  // Insert after tail (at head)
            tail->next = new_node;        // Point tail to new node
            tail = new_node;              // Update tail
        }
    }

    void delProcess(string id) {
        /*
        Desc: deletes a process given its id, using the same logic as in a circular linked list.
        Parameters:
            id (string): id for process to be deleted.
        */

        if (tail == nullptr) return;  // No process to delete

        Process* current = tail->next;  // Start from head
        Process* prev = tail;

        // Traverse the list to find the process to delete
        do {
            if (current->id == id) {
                rem -= 1;
                if (current == tail && current == tail->next) {
                    // If the list has only one process
                    tail = nullptr;
                } else if (current == tail) {
                    // If we're deleting the tail
                    prev->next = tail->next;  // Bypass tail
                    tail = prev;              // Move tail back
                } else {
                    // Deleting a non-tail process
                    prev->next = current->next;
                }
                delete current;  // Free memory
                return;
            }
            else {
                prev = current;
                current = current->next;
            }
        } while (current != tail->next);  // Stop when we've circled back to the head
    }

    void cycle() {
        /*
        Desc: traverses the circular linked, calls the process function for each Process, outputs current working.
        */

        if (tail == nullptr) {
            // List's empty
            cout << "All processes completed!" << endl;
            return;
        }

        // Increments cycle count.
        cycles += 1;
        cout << "Cycle " << cycles << ": ";

        // Traversing.
        Process* current = tail->next;  // Start from head
        Process* start = current;       // To mark the head of the list

        do {
            cout << current->id << " ";
            current->process(cpu_time);  // Process for CPU time slice

            if (current->has_ended()) {
                cout << "(Completes), ";
                Process* to_delete = current;
                current = current->next;  // Move to the next process before deleting
                delProcess(to_delete->id);
                if (tail == nullptr) break;  // If the last process was deleted, exit
            } else {
                cout << "(Remaining: " << current->rem_time << "), ";
                current = current->next;
            }
        } while (current != start && tail != nullptr);  // Ensure a full cycle around the list

        cout << endl;
    }
};

int main() {
    Scheduler sc(3);

    sc.addProcess(10);
    sc.addProcess(5);
    sc.addProcess(8);

    cout << "Initial Processes: [(P1, 10), (P2, 5), (P3, 8)]" << endl;

    while (sc.tail != nullptr) {  // Continue cycling until all processes are completed
        sc.cycle();
    }

    return 0;
}
