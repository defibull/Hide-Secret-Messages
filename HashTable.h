#ifndef Project_4_HashTable_h
#define Project_4_HashTable_h
//
//  HashTable.h
//  Project 4
//
//  Created by Shashank Khanna  on 3/5/15.
//  Copyright (c) 2015. All rights reserved.
//

#include <iostream>
using namespace std; 
//template<typename KeyType>
//unsigned int computeHash(KeyType key);

template<typename KeyType,typename ValueType>
class HashTable
{
public:
    HashTable(unsigned int numBuckets, unsigned int capacity): m_capacity(capacity), m_numBuckets (numBuckets), m_numAssociations(0),
    tracking_head(nullptr),tracking_tail(nullptr)
    {
    m_dynamic_array = new Node* [m_numBuckets]; // dynamic array gets and array of pointer to nodes
    for (int i = 0; i < m_numBuckets; i++)
        {
        m_dynamic_array[i] = nullptr;
        }
    
    }
    
    
    ~HashTable() 
    {
    for (int i = 0 ; i < m_numBuckets; i++)
        {
        while (m_dynamic_array[i]!= nullptr)
            {
            Node* temp = m_dynamic_array[i];
            m_dynamic_array[i] = m_dynamic_array[i]->next; 
            delete temp;
            }
        }
    }
     
    bool isFull() const
    {
    return m_numAssociations == m_capacity; // returns true if full
    }
    bool set(const KeyType& key, const ValueType& value, bool permanent = false)
    {
    int bucket  = getBucketforKey(key); Node* keyPtr = nullptr; 
    // go through the linked list looking for that key 
    Node* curr = m_dynamic_array[bucket]; //Node* prev = nullptr;
    while(curr != nullptr)
        {
        if (curr->m_key == key) // found the key
            { curr->m_value = value; 
                if (curr->isPermanent) // the association was permanent just update the values
                    return true;
                else{ // association not permanent 
                   curr->pos =  trackTouch(curr->pos);
                    return true; 
                
                }
            
                }
        
      //  prev = curr;     
        curr = curr->next;
        }
    
// if key was not present in the linked list
    if (curr == nullptr) // bucket is empty
    {
        if (isFull())
            return false;
        // insert a Node at the beginning of the list 
        Node* temp = m_dynamic_array[bucket];
        m_dynamic_array[bucket] = new Node; 
        //follow the first pointer and store the stuff in it
        m_dynamic_array[bucket]->m_key = key; m_dynamic_array[bucket]->m_value = value; m_dynamic_array[bucket]->isPermanent = permanent;
        //make the new node's next point to the original first node
        m_dynamic_array[bucket]->next = temp;
    // if permanent put it in the linked list 
                if (!m_dynamic_array[bucket]->isPermanent)
                        addTrackingNode(m_dynamic_array[bucket]);
    m_numAssociations++;
                        return true;
                        }
    return false;
    }
    bool get(const KeyType& key, ValueType& value)const
    {
    int bucket = getBucketforKey(key);
    Node* toGet = m_dynamic_array[bucket];  
    // once we have the bucket we can just look through that linked list for the key
    while(toGet != nullptr)
        {
        if (toGet->m_key == key)
            {    value = toGet->m_value; return true;}
        toGet = toGet->next;
        }
    // key not found 
    return false;
    
    }
    bool touch(const KeyType& key)
    {
    int bucket = getBucketforKey(key);
    Node* toTouch = m_dynamic_array[bucket];
    Node* prev = nullptr;
    while (toTouch != nullptr)
        {
        if (toTouch->m_key == key && !toTouch->isPermanent) // found it in the list
            {
            // if already at the first position when m_key == key return true
            toTouch->pos = trackTouch(toTouch->pos); return true;
            }
        prev = toTouch;
        toTouch = toTouch->next;
        
        }
    return false; // value not found 
    }
    bool discard(KeyType& key, ValueType& value)
    {
    Node* discarded =  trackDiscard(); 
        if (discarded != nullptr)
            
            {
            key = discarded->m_key;
            value = discarded->m_value;
           int bucket =  getBucketforKey(key);
            Node* curr = m_dynamic_array[bucket];
            Node* prev = nullptr; // is this not a pointer to a node? turns out when I make prev null it gives me an argument that it isn't a pointer
            while (curr != nullptr)
                {
                   if (discarded == curr)
                       {
                            if (prev == nullptr) // first element to be deleted
                                {
                                curr->next = m_dynamic_array[bucket];
                                delete curr; 
                                return true;
                                }
                            else{
                                prev->next = curr->next;
                                delete curr;
                                return true;
                            }
                       }
                
                prev = curr; 
                curr = curr->next; 
                }
            }
    return false;
    }
private:
    // We prevent a HashTable from being copied or assigned by declaring 
    // the copy constructor and assignment operator private and not implementing it 
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
    // get bucket 
    unsigned int determineBucketFromHash (unsigned int result)const
    {
    return result%m_numBuckets;
    }
    
    unsigned int getBucketforKey(const KeyType& key) const
    {
    //The computeHash function must be defined
    //for each type of key that we use in some
    // hash table.
    unsigned int computeHash(KeyType); // prototype
    unsigned int result = computeHash(key);
    unsigned int bucketNum = determineBucketFromHash(result);
    return bucketNum;
    }
    

    // doubly linked list - tracking
    struct Node;
    struct TLL_Node
    {
    TLL_Node* next; 
    TLL_Node* prev ;
    Node* address;
    };

    // node for hashTable
    struct Node
    {
    KeyType m_key ; // key value pairs to avoid collision 
    ValueType m_value;
    bool isPermanent; 
    Node* next; 
    TLL_Node* pos;
    };
    
       //hashtable overall
    int m_numBuckets; 
    int m_capacity;   
    int m_numAssociations;
    
    //linked list for tracking Latest Updated/Earliest Updated
    TLL_Node* tracking_head;
    TLL_Node* tracking_tail;
    Node** m_dynamic_array; // pointer to a node pointer
    
    // Tracking functions 
    
    bool addTrackingNode(Node* toAdd)
    {
        // empty list
        if (tracking_head == nullptr && tracking_tail == nullptr)
            {
            tracking_head = new TLL_Node ;
            tracking_head->address = toAdd;
            tracking_head->next = nullptr;
            tracking_head->prev = nullptr;
            tracking_tail = tracking_head; // its next and prev are automatically null
            toAdd->pos = tracking_head;
            return true;}
        // one node 
        else   
        {
            TLL_Node* temp  = tracking_head; 
            tracking_head = new TLL_Node;
            tracking_head->address = toAdd;
            tracking_head->next = temp; 
            temp->prev  = tracking_head;
            toAdd->pos = tracking_head;
            return true;}
    return false;
    }
    
     
    TLL_Node* trackTouch (TLL_Node* moveFront)
    {
    // if moveFront the first item or only one item in the list        
    if (moveFront == tracking_head)
                return tracking_head; 
            else {
   
    moveFront->prev->next = moveFront->next;
    if (moveFront->next != nullptr)
    moveFront->next->prev = moveFront->prev;  //if moveFront is not the last item in the list
    if (moveFront->next == nullptr)
        tracking_tail = moveFront->prev;  // adjust tail pointer if last element 
    TLL_Node* temp = tracking_head;
    tracking_head = moveFront;
    moveFront->next = temp;
    moveFront->prev = nullptr;
    temp->prev = moveFront;
    
                return tracking_head;
            }
    return nullptr;
    }
    
    Node* trackDiscard()
    {
        if (tracking_tail == nullptr)
            return nullptr; // list empty 
    TLL_Node* temp  = tracking_tail;
    tracking_tail = temp->prev;
    Node* discarded = temp->address;
    delete temp;
    return discarded;
    }
    
    
};





#endif
