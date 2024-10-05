/*
Author(s): 1. Hanzala B. Rehan
Description: Storing a 1024 bit number in a linked list structure.
Date created: October 5th, 2024.
Date last modified: October 5th, 2024.
*/
#include <iostream>
#include <string>
#include <vector>
using namespace std;


class Node {
    // Node class for each chunk of the large number
    public:
    uint64_t value;  // 64-bit chunk value (up to 19 digits)
    Node* next;      // Pointer to the next node

    Node(uint64_t val) {
        //Constructor to initialize the node's value and set the next pointer to nullptr.
        value = val;
        next = nullptr;
    }
};


class LargeNumber {
    // LargeNumber class representing a large number using a linked list.
    public:
    Node* head;  // Head pointer to the first node

    LargeNumber() {
        //Constructor to initialize the head pointer to nullptr (empty list).
        head = nullptr;
    }

    ~LargeNumber() {
        //Destructor to free the allocated memory for the linked list.
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addChunk(uint64_t chunk) {
        /*
        Desc: Adds a chunk (64-bit value) to the end of the linked list.
        Parameters:
            chunk (uint64_t): Chunk value to be added to the list.
        */
        Node* newNode = new Node(chunk);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void print() const {
        /*
        Desc: Print the large number stored in the linked list in reverse order to reconstruct the full number.
        */
        printRecursive(head);
    }

    LargeNumber operator+(const LargeNumber& other) const {
        /*
        Desc: Overloads the + operator to add two LargeNumber objects.
        Parameters:
            other (const LargeNumber&): The other LargeNumber object to be added.
        returns:
            (LargeNumber): The result of the addition of the two large numbers.
        */
        LargeNumber result;  // Resultant large number
        Node* currentA = head;   // Pointer for traversing the current large number
        Node* currentB = other.head;  // Pointer for traversing the other large number
        uint64_t carry = 0;  // To handle overflow between chunk additions

        // Traverse both linked lists and add corresponding chunks, including the carry
        while (currentA != nullptr || currentB != nullptr || carry != 0) {
            uint64_t valueA = (currentA != nullptr) ? currentA->value : 0;  // Value from this number
            uint64_t valueB = (currentB != nullptr) ? currentB->value : 0;  // Value from the other number

            // Sum the chunks and the carry
            uint64_t sum = valueA + valueB + carry;

            // Calculate the new carry (for 19 digits in each chunk)
            carry = sum / 1000000000000000000ULL;
            
            // Store the last 19 digits of the sum in the result linked list
            result.addChunk(sum % 1000000000000000000ULL);

            // Move to the next chunk in each list (if available)
            if (currentA != nullptr) currentA = currentA->next;
            if (currentB != nullptr) currentB = currentB->next;
        }

        return result;  // Return the resulting large number
    }

    LargeNumber operator-(const LargeNumber& other) const {
        /*
        Desc: Overloads the - operator to subtract two LargeNumber objects.
        Parameters:
            other (const LargeNumber&): The other LargeNumber object to be subtracted.
        returns:
            (LargeNumber): The result of the subtraction of the two large numbers.
        Note: The function assumes that the first number is greater than or equal to the second number.
        */
        LargeNumber result;  // Resultant large number
        Node* currentA = head;   // Pointer for traversing the current large number
        Node* currentB = other.head;  // Pointer for traversing the other large number
        int64_t borrow = 0;  // To handle borrowing between chunk subtractions

        // Traverse both linked lists and subtract corresponding chunks, considering borrowing
        while (currentA != nullptr || currentB != nullptr || borrow != 0) {
            uint64_t valueA = (currentA != nullptr) ? currentA->value : 0;  // Value from this number
            uint64_t valueB = (currentB != nullptr) ? currentB->value : 0;  // Value from the other number

            // Subtract the chunks along with any borrowing
            int64_t diff = static_cast<int64_t>(valueA) - valueB - borrow;

            // If the result is negative, borrow from the next chunk
            if (diff < 0) {
                diff += 1000000000000000000ULL;  // Borrow by adding the maximum chunk size (19 digits)
                borrow = 1;  // Set borrow to 1 for the next iteration
            } else {
                borrow = 0;  // Reset borrow if no borrowing is needed
            }

            result.addChunk(diff);  // Add the result chunk to the linked list

            // Move to the next chunk in each list (if available)
            if (currentA != nullptr) currentA = currentA->next;
            if (currentB != nullptr) currentB = currentB->next;
        }

        return result;  // Return the resulting large number
    }

    private:
    void printRecursive(Node* node) const {
        /*
        Desc: Recursively print the linked list starting from the last node.
        Parameters:
            node (Node*): Current node to process.
        */
        if (node == nullptr) return;
        printRecursive(node->next);  // Recurse to the end of the list
        cout << node->value << " ";  // Print the value in reverse order
    }
};


// Helper function to split the large number into chunks of 19 digits
vector<uint64_t> splitNumberIntoChunks(const string& number, size_t maxSize) {
    /*
    Desc: Splits the input number string into chunks of up to 19 digits.
    Parameters:
        number (string): The large number as a string.
        maxSize (size_t): Maximum number of digits per chunk (19 for uint64_t).
    returns:
        (vector<uint64_t>): Vector of chunks as uint64_t.
    */
    vector<uint64_t> chunks;
    size_t length = number.size();

    for (int i = length; i > 0; i -= maxSize) {
        size_t start = (i >= maxSize) ? i - maxSize : 0;
        size_t size = (i >= maxSize) ? maxSize : i;
        string chunkStr = number.substr(start, size);

        // Convert chunk string to an unsigned long long integer
        uint64_t chunkValue = stoull(chunkStr);
        chunks.push_back(chunkValue);
    }

    return chunks;
}


int main() {
    // Input two large numbers as strings
    string largeNumber1, largeNumber2;
    cout << "Enter the first large number: ";
    cin >> largeNumber1;
    cout << "Enter the second large number: ";
    cin >> largeNumber2;

    // Split both numbers into chunks of 19 digits each
    vector<uint64_t> chunks1 = splitNumberIntoChunks(largeNumber1, 19);
    vector<uint64_t> chunks2 = splitNumberIntoChunks(largeNumber2, 19);

    // Create LargeNumber linked lists for both numbers
    LargeNumber ln1, ln2;
    for (const uint64_t& chunk : chunks1) {
        ln1.addChunk(chunk);
    }
    for (const uint64_t& chunk : chunks2) {
        ln2.addChunk(chunk);
    }

    // Display the original numbers (reconstructed from chunks)
    cout << "Stored number 1 chunks (in reverse): " << endl;
    ln1.print();
    cout << endl << "Stored number 2 chunks (in reverse): " << endl;
    ln2.print();
    cout << endl;

    // Perform addition of the two large numbers
    LargeNumber sum = ln1 + ln2;
    cout << "Sum of the two large numbers (in reverse): " << endl;
    sum.print();
    cout << endl;

    // Perform subtraction of the two large numbers
    if (largeNumber1 >= largeNumber2) {
        LargeNumber difference = ln1 - ln2;
        cout << "Difference (ln1 - ln2) (in reverse): " << endl;
        difference.print();
    } else {
        LargeNumber difference = ln2 - ln1;
        cout << "Difference (ln2 - ln1) (in reverse): " << endl;
        difference.print();
    }
    cout << endl;

    return 0;
}
