# Project Overview: Trees, Stacks, and Queues in C

This project implements some basic data structures and their common operations in C that I have made so far for the Graph Theory subject of my Computer Science course.

## Binary Tree

* **Node stuff:** `Node` definition, `createNode`, `printNode`
* **Operations:**

  * Insert node (`insertNode`) — non-recursive
  * Remove node (`removeNode`) — non-recursive
  * Find a node (`findNode`) — non-recursive
* **Traversals:**

  * Pre-order, In-order, Post-order
  * Level-order
* **Utilities:**

  * Find minimum (`findMin`)
  * Find maximum (`findMax`)
  * Get height (`getHeight`)

## Stack

* **Stack stuff:** `createStack`, `printStack`, `freeStack`
* **Operations:** push, pop, check if full/empty

## Queue

* **Queue stuff:** `createQueue`, `printQueue`, `freeQueue`
* **Operations:** enqueue, dequeue, check if full/empty
* **Note:** Converted to **circular queue** so you can reuse slots after dequeues — no more getting stuck when the end of the array is reached


