#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdlib.h>

template <class NODETYPE>
class LinkedList;

template <class NODETYPE>
class Node {

    friend class LinkedList<NODETYPE>; // makes the LinkedList class a friend
    
private:

    NODETYPE info; // info
    Node *prev; // ptr to the previous node or NULL
    Node *next; // ptr to the next node or NULL

public:
    Node ();
    Node ( const NODETYPE & ); // constructor
    NODETYPE getinfo();
};

template <class NODETYPE> class LinkedList {
    
public:

    LinkedList(); // constructor
    ~LinkedList(); // destructor
    void addhead ( const NODETYPE &); // insert at head
    void addtail ( const NODETYPE &); // insert at tail
    int removehead (); // remove from head
    int removetail (); // remove from tail
    int removecurrent (); // remove node at current position
    int removeall (); // remove all elements
    int isempty () const; // checks whether list is empty
    int listend () const; // checks whether reached the end of list
    int gohead(); // go to head
    int gotail(); // go to tail
    int gonext(); // go to next node
    int goprev(); // go to previous node
    Node<NODETYPE> *getnode(); // get current node
    NODETYPE getnodeinfo(); // get current node's info
    void printlist();

private:

    Node<NODETYPE> *head;
    Node<NODETYPE> *tail;
    Node<NODETYPE> *current;

    // getnode returns a pointer to a new, empty node
    Node<NODETYPE> *newnode ( const NODETYPE &);
    
};

// ------- implementation of LinkedList -----------

// constructor for a Node Object
template <class NODETYPE>
Node<NODETYPE> :: Node () { 

    info = NULL;
    next = prev = NULL;
}

template <class NODETYPE>
Node<NODETYPE> :: Node ( const NODETYPE& x) { 

    info = x;
    next = prev = NULL;
}

template <class NODETYPE>
NODETYPE Node<NODETYPE> :: getinfo () { 

    return info;
}

// constructor for the LinkedList object
template <class NODETYPE>
LinkedList <NODETYPE> :: LinkedList(){ 

    head = tail = current = NULL;
}

// destructor
template <class NODETYPE>
LinkedList <NODETYPE> :: ~LinkedList(){ 

    removeall();
}

// newnode
template <class NODETYPE>
Node<NODETYPE> *LinkedList<NODETYPE> :: newnode ( const NODETYPE& x){ 

    Node<NODETYPE> *P = new Node<NODETYPE> ( x );
    // allocate a new Node, initialize it to x
    return P; // return the pointer to the newly allocated node
}

// insert at head
template <class NODETYPE>
void LinkedList<NODETYPE> :: addhead( const NODETYPE& x){ 

    Node<NODETYPE> *P = newnode ( x ); 
    P -> next = head;
    if (head==NULL) {
        tail=P;
        current=head;
    }
    else head->prev=P;
    head = P;

}

// insert at tail
template <class NODETYPE>
void LinkedList<NODETYPE> :: addtail( const NODETYPE& x){ 

    Node<NODETYPE> *P = newnode ( x );
    P->prev = tail;
    if (tail==NULL) {
        head=P;
        current=head;
    }
    else tail -> next = P;
    tail = P;
}

// remove head
template <class NODETYPE>
int LinkedList<NODETYPE> :: removehead (){ 

    if ( isempty() ) return 0; // underflow
    else {
        Node<NODETYPE> *P = head;
        head = P -> next;
        if (head!=NULL) head -> prev=NULL;
        if (current==P) current=head;
        delete P;
        return 1;
    }
}

// remove tail
template <class NODETYPE>
int LinkedList<NODETYPE> :: removetail (){ 

    if ( isempty() ) return 0; // underflow
    else {
        Node<NODETYPE> *P = tail;
        tail = P -> prev;
        if (tail!=NULL) tail -> next =NULL;
        if (current==P) current=tail;
        delete P;
        return 1;
    }
}

// remove current
template <class NODETYPE>
int LinkedList<NODETYPE> :: removecurrent (){ 

    if ( isempty() || current==NULL) return 0; // underflow
    else {
        if (current==head) return removehead();
        else if (current==tail) return removetail();
        else {
            Node<NODETYPE> *P = current;
            P -> prev -> next = P -> next;
            P -> next -> prev = P -> prev;
            if (current==P) current=P->prev;
            delete P;
            return 1;
        }
    }
}

// remove all
template <class NODETYPE>
int LinkedList<NODETYPE> :: removeall (){ 

    if ( isempty() ) return 0; // underflow
    else {
        Node <NODETYPE> *P = head;
        Node <NODETYPE> *tempPtr;

        while ( P != NULL ){ 
            tempPtr = P;
            P = P -> next;
            delete tempPtr;
        }
        return 1;
    }
}

// is empty?
template <class NODETYPE>
int LinkedList<NODETYPE> :: isempty () const { 

    return head == NULL;
}

// end
template <class NODETYPE>
int LinkedList<NODETYPE> :: listend () const { 

    return current==tail;
}

// go to head
template <class NODETYPE>
int LinkedList<NODETYPE> :: gohead(){

    current=head;
    return 1;
}

// go to tail
template <class NODETYPE>
int LinkedList<NODETYPE> :: gotail(){

    current=tail;
    return 1;
}
    
// go to next node
template <class NODETYPE>
int LinkedList<NODETYPE> :: gonext(){

    if (current->next!=NULL) {
        current=current->next;
        return 1;
    }
    else return 0;
}

// go to previous node
template <class NODETYPE>
int LinkedList<NODETYPE> :: goprev(){

    if (current->prev!=NULL) {
        current=current->prev;
        return 1;
    }
    else return 0;
}

// get current node
template <class NODETYPE>
Node<NODETYPE> *LinkedList<NODETYPE> :: getnode(){

    return current;
}

// get current node info
template <class NODETYPE>
NODETYPE LinkedList<NODETYPE> :: getnodeinfo(){

    return current->info;
}

// get current node info
template <class NODETYPE>
void LinkedList<NODETYPE> :: printlist(){

    Node<NODETYPE> *temp = current;
    cout << " current: " << (int)(getnodeinfo().n);
    gohead();
    cout << " head: " << (int)(getnodeinfo().n);
    gotail();
    cout << " tail: " << (int)(getnodeinfo().n);
    cout << " nodeinfos: ";
    gohead();
    do {
        cout << (int)(getnodeinfo().n) << " ";
    }
    while (gonext());
    cout << "\n";
    current=temp;
}

#endif