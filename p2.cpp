/*
Author(s): 1. Hanzala B. Rehan
Description: Checking a 309 digit if it's prime or not, using Rabin-Miller Algorithm.
Date created: October 5th, 2024.
Date last modified: October 5th, 2024.
*/
#include <iostream>
#include <string>
#include <vector>
#include <random>
using namespace std;

// Node class to represent each chunk of the large number
class Node {
public:
    uint64_t value; // 64-bit chunk value (up to 19 digits)
    Node* next; // Pointer to the next node

    // Constructor to initialize a node with a value
    Node(uint64_t val) {
        value = val;
        next = nullptr;
    }
};

// LargeNumber class to handle large numbers using a linked list
class LargeNumber {
public:
    Node* head; // Head pointer to the first node

    // Constructor to initialize an empty LargeNumber
    LargeNumber() {
        head = nullptr;
    }

    // Destructor to free memory of the linked list
    ~LargeNumber() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp; // Free each node
        }
    }

    // Add a chunk (19-digit piece) to the LargeNumber
    void addChunk(uint64_t chunk) {
        Node* newNode = new Node(chunk); // Create a new node
        if (head == nullptr) {
            head = newNode; // If empty, set head to new node
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next; // Traverse to the last node
            }
            current->next = newNode; // Add new node at the end
        }
    }

    // Print the LargeNumber (from most significant to least)
    void print() const {
        printRecursive(head);
    }

    // Implementation of the Miller-Rabin primality test
    bool millerRabin(int k) const {
        /*
        Desc: Performs the Miller-Rabin primality test on the number stored in the linked list.
        Parameters:
            k (int): Number of iterations to perform for accuracy.
        Returns:
            bool: True if the number is probably prime, False if composite.
        */
        if (head == nullptr) return false; // Return false if the number is empty

        // Convert the linked list chunks into a single 64-bit integer (n)
        Node* current = head;
        uint64_t n = 0;
        while (current != nullptr) {
            n = n * 1000000000000000000ULL + current->value; // Combine chunks to form the number
            current = current->next;
        }

        // Base cases for small numbers
        if (n == 2 || n == 3) return true;  // 2 and 3 are prime numbers
        if (n <= 1 || n % 2 == 0) return false;  // Exclude numbers <= 1 and even numbers

        // Write n-1 as 2^s * d (factoring out powers of 2)
        uint64_t d = n - 1;
        int s = 0;
        while (d % 2 == 0) {
            d /= 2;  // Keep dividing by 2 to find d
            s++;     // Count the powers of 2
        }

        // Random number generator setup
        random_device rd;      // Seed for randomness
        mt19937 gen(rd());     // Mersenne Twister RNG
        uniform_int_distribution<uint64_t> dis(2, n - 2);  // Generate random base in [2, n-2]

        // Perform Miller-Rabin test for k iterations
        for (int i = 0; i < k; i++) {
            uint64_t a = dis(gen);  // Random base (a)
            uint64_t x = power(a, d, n);  // Compute a^d % n

            // If x == 1 or x == n-1, this round passes
            if (x == 1 || x == n - 1) continue;

            bool found = false;
            // Perform up to s-1 squaring rounds (check for x^2, x^4, ..., until x == n-1)
            for (int r = 1; r < s; r++) {
                x = (x * x) % n;  // Compute x^2 % n
                if (x == n - 1) {  // If x reaches n-1, the round passes
                    found = true;
                    break;
                }
            }

            // If no break, n is composite
            if (!found) return false;
        }

        // If all rounds pass, n is probably prime
        return true;
    }

    // Power function used in Miller-Rabin (Exponentiation by Squaring)
    static uint64_t power(uint64_t base, uint64_t exp, uint64_t mod) {
        /*
        Desc: Computes (base^exp) % mod using Exponentiation by Squaring.
        Parameters:
            base (uint64_t): Base of the exponentiation.
            exp (uint64_t): Exponent.
            mod (uint64_t): Modulus.
        Returns:
            uint64_t: Result of (base^exp) % mod.
        */
        uint64_t result = 1;  // Initialize result to 1
        base = base % mod;    // Mod the base to reduce its size

        while (exp > 0) {
            // If the exponent is odd, multiply base with result
            if (exp % 2 == 1) {
                result = (result * base) % mod;
            }
            exp = exp >> 1;  // Divide exponent by 2 (right shift)
            base = (base * base) % mod;  // Square the base and mod it
        }

        return result;  // Return the final result
    }

private:
    // Recursive function to print the number from most to least significant
    void printRecursive(Node* node) const {
        if (node == nullptr) return; // Base case: nothing to print
        printRecursive(node->next); // Recursively print the next node
        cout << node->value << " "; // Print the current node value
    }
};

// Helper function to split the large number into chunks of 19 digits
vector<uint64_t> splitNumberIntoChunks(const string& number, size_t maxSize) {
    /*
    Desc: Splits a large string number into chunks of up to 19 digits.
    Parameters:
        number (const string&): The number as a string.
        maxSize (size_t): Maximum size of each chunk (19 digits).
    Returns:
        vector<uint64_t>: Vector of chunks as 64-bit unsigned integers.
    */
    vector<uint64_t> chunks;
    size_t length = number.size();

    // Split the number into 19-digit chunks starting from the least significant part
    for (int i = length; i > 0; i -= maxSize) {
        size_t start = (i >= maxSize) ? i - maxSize : 0;  // Determine the starting point of the chunk
        size_t size = (i >= maxSize) ? maxSize : i;       // Determine the size of the chunk
        string chunkStr = number.substr(start, size);     // Extract the chunk substring

        uint64_t chunkValue = stoull(chunkStr);           // Convert the chunk to an unsigned 64-bit integer
        chunks.push_back(chunkValue);                     // Add the chunk to the vector
    }

    return chunks; // Return the vector of chunks
}

int main() {
    // Input number as a string
    cout << "Enter a number to check for primality: ";
    string numberStr;
    cin >> numberStr;

    // Create LargeNumber instance and populate it with chunks
    vector<uint64_t> numberChunks = splitNumberIntoChunks(numberStr, 19); // Split the input number into 19-digit chunks
    LargeNumber number;
    for (const uint64_t& chunk : numberChunks) {
        number.addChunk(chunk); // Add each chunk to the LargeNumber
    }

    // Perform Miller-Rabin primality test
    int k = 10; // Number of rounds (more rounds = higher confidence in result)
    if (number.millerRabin(k)) {
        cout << "The number is probably prime." << endl;
    } else {
        cout << "The number is composite." << endl;
    }

    return 0;
}
