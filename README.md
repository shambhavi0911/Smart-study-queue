# Smart Study Queue📚

Smart Study Queue is a C++-based study resource management system that helps students organize, prioritize, and track learning materials efficiently.

The application combines multiple Data Structures and Algorithms—including Linked Lists, Stacks, Hash Maps, and Priority Queues—to provide fast searching, undo functionality, intelligent prioritization, and persistent storage using JSON.

## Overview
Students often struggle with managing scattered study resources across different subjects. Smart Study Queue provides a structured way to store, prioritize, search, and manage study materials efficiently.

Smart Study Queue demonstrates how Linked Lists, Stacks, Hash Maps, and Priority Queues can be combined to build a practical study resource management system.

## Features
- Add study resources with title, type, tag, and priority
- View the next resource in the queue
- Skip resources and revisit them later
- Undo previously skipped or removed resources
- Search resources instantly by title
- Prioritize important resources
- Shuffle the queue based on urgency
- Store data permanently using JSON files
  
## Project Operations

1. Add Resource
2. View Next Resource
3. Skip Resource
4. Undo Last Action
5. Search Resource
6. Shuffle Queue by Priority
7. Display All Resources

## Data Structures Used

### Linked List
Acts as the primary queue structure and stores study resources dynamically.

### Stack
Implements the Undo feature using the LIFO principle.

### Hash Map
Provides fast resource lookup with average O(1) search complexity.

### Priority Queue (Min Heap)
Reorders study resources according to priority so that urgent tasks appear first.

## Technologies Used

- C++
- STL (Stack, Priority Queue, Unordered Map)
- JSON (nlohmann/json)
- File Handling

## Time Complexity

| Operation | Complexity   |
|------------|------------|
| Add Resource | O(log n) |
| Search Resource | O(1) Average |
| Undo Action | O(1) |
| View Next Resource | O(1) |
| Shuffle By Priority | O(n log n) |

## Future Improvements

- GUI based interface
- Deadline reminders
- Progress tracking
- Subject-wise filtering
- Cloud synchronization

## Practical Applications

- Study planning and revision management
- Organizing notes, PDFs, and video resources
- Demonstrating practical applications of Data Structures and Algorithms
- Academic productivity enhancement


