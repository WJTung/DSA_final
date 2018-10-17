# Final Project for Data Structures and Algorithms (NTU, Spring 2015)

This project simulates the requirements for a bank account management system. We designed different data structures to store the information and support several functions efficiently, including
+ *login* : based on a ID and a password. Note that the password cannot be stored in your system in
  its original form.
+ *create* : create an account with some demanded ID. If the ID already exists, some alternative IDs
  must be recommended.
+ *delete* : delete an account
+ *withdraw* : withdraw money from an account
+ *transfer* : transfer money to another account with either
+ *merge* : merge two accounts into one
+ *search* : search the history of transfer

We used the pointer of the *struct History* to store the transferring history in an account so that the history can be modified simultaneously in both accounts when a *merge* operation is processed. We tried *map*, *hash table (unordered_map)*, *trie*, *ternary tree* for storing accounts.
