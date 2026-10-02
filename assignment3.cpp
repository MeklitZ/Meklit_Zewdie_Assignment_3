/*
 * Program Name: EECS 348 Assignment 3
 * Description: This program uses a list-based MaxHeap to implement a priority
queue for a CEO's emails. The emails are prioritized by sender
category and then by date too.
 * Inputs: Email information and commands such as EMAIL, NEXT, READ, and COUNT.
 * Output: The program displays email information and the number of unread emails.
 * Collaborators: Google Gemini (OpenAI)
 * Other Sources: None
 * Author: Meklit Zewdie
 * Creation Date: 10/01/2026
 * Revision Date: 10/01/2026
 * Revisions: I first compared two GenAI-generated C++ programs and then I improved the
genmini program for correctness, maintainability, and assignment requirements.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// Structure representing an Email object
struct Email {
    std::string senderCategory;
    std::string subject;
    std::string date;

    // Helper to assign a numerical priority to each category
    int getCategoryPriority() const {
        if (senderCategory == "Boss") return 5;
        if (senderCategory == "Subordinate") return 4;
        if (senderCategory == "Peer") return 3;
        if (senderCategory == "ImportantPerson") return 2;
        return 1; // "OtherPerson" or any unknown category
    }

    // Helper to convert date "MM-DD-YYYY" to "YYYYMMDD" integer for easy comparison
    int getDateKey() const {
        if (date.length() != 10) return 0;
        int month = std::stoi(date.substr(0, 2));
        int day   = std::stoi(date.substr(3, 2));
        int year  = std::stoi(date.substr(6, 4));
        return year * 10000 + month * 100 + day;
    }

    // Greater-than operator overloaded to define priority ordering
    bool operator>(const Email& other) const {
        int p1 = getCategoryPriority();
        int p2 = other.getCategoryPriority();

        if (p1 != p2) {
            return p1 > p2;
        }
        // Tie-breaker: Newer date gets higher priority
        return getDateKey() > other.getDateKey();
    }
};

// MaxHeap implemented from scratch using dynamic array/list storage (std::vector)
class MaxHeap {
private:
    std::vector<Email> heap;

    // Percolate UP: Restore heap property after inserting a new element
    void heapifyUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (heap[index] > heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Percolate DOWN: Restore heap property after extracting the root element
    void heapifyDown(size_t index) {
        size_t size = heap.size();
        while (true) {
            size_t largest = index;
            size_t leftChild = 2 * index + 1;
            size_t rightChild = 2 * index + 2;

            if (leftChild < size && heap[leftChild] > heap[largest]) {
                largest = leftChild;
            }
            if (rightChild < size && heap[rightChild] > heap[largest]) {
                largest = rightChild;
            }

            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    // Insert an email into the priority queue
    void insert(const Email& email) {
        heap.push_back(email);
        heapifyUp(heap.size() - 1);
    }

    // Peak at the highest-priority email without removing it
    bool peek(Email& result) const {
        if (heap.empty()) {
            return false;
        }
        result = heap[0];
        return true;
    }

    // Remove the highest-priority email from the heap
    bool extractMax() {
        if (heap.empty()) {
            return false;
        }
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return true;
    }

    // Return current count of items in the heap
    size_t size() const {
        return heap.size();
    }

    bool empty() const {
        return heap.empty();
    }
};

// Class to manage CEO Email Operations
class EmailManager {
private:
    MaxHeap priorityQueue;

public:
    // Parse input string: EMAIL Sender,Subject,Date
    void processEmailCommand(const std::string& line) {
        // Skip "EMAIL " prefix
        std::string data = line.substr(6);
        std::stringstream ss(data);

        Email email;
        std::getline(ss, email.senderCategory, ',');
        std::getline(ss, email.subject, ',');
        std::getline(ss, email.date);

        priorityQueue.insert(email);
    }

    // Display the next highest priority email without deleting it
    void showNext() const {
        Email topEmail;
        if (priorityQueue.peek(topEmail)) {
            std::cout << "Next email:\n";
            std::cout << "Sender: " << topEmail.senderCategory << "\n";
            std::cout << "Subject: " << topEmail.subject << "\n";
            std::cout << "Date: " << topEmail.date << "\n";
        } else {
            std::cout << "No emails to read.\n";
        }
    }

    // Remove the top email without displaying
    void readEmail() {
        priorityQueue.extractMax();
    }

    // Print total count of unread emails
    void showCount() const {
        std::cout << "There are " << priorityQueue.size() << " emails to read.\n";
    }
};

int main() {
    EmailManager manager;
    std::string line;

    // Read commands line by line from standard input
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        if (line.rfind("EMAIL ", 0) == 0) {
            manager.processEmailCommand(line);
        } else if (line == "NEXT") {
            manager.showNext();
        } else if (line == "READ") {
            manager.readEmail();
        } else if (line == "COUNT") {
            manager.showCount();
        }
    }

    return 0;
}