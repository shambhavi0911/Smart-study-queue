#include <bits/stdc++.h>
#include "json.hpp"
#include <fstream>
using namespace std;

using json = nlohmann::json;

class StudyItem {
   public:
    int id;                 // unique id
    string title;      // name of study material
    string type;       // "video", "pdf", "note"
    string tag;        // e.g., "DSA", "OS", "DBMS"
    int priority;           // lower = more urgent
    StudyItem* next;        // pointer to next node

    StudyItem(int id, string title, string type, string tag, int priority) { // Node Constructor
      this->id=id;
      this->title = title;
      this->type = type;
      this->tag = tag;
      this->priority = priority;
      this->next = nullptr;
    }

    

};

void displayItem(StudyItem* item) {
  cout << "-->ID: " << item->id << "\nTitle: " << item->title << "\nType: " << item->type << "\nTag: " << item->tag << "\nPriority: " << item->priority << "\n";
}

struct Cmp {
  bool operator()(const StudyItem* a, const StudyItem* b) const {
    if(a->priority == b->priority) return a->id > b->id;
    return a->priority > b->priority; // min-heap by priority
  }
};

class StudyQueue {
  private:
    json jsonData = json::array();
    StudyItem* head;  
    StudyItem* tail;
    unordered_map<string, StudyItem*> searchTable; // for search
    stack<StudyItem*> undoStack; // undo operations

    priority_queue<StudyItem*, vector<StudyItem*>, Cmp> minHeap; // for shuffle
    bool priorityShuffled;

    int count;

  public:
    StudyQueue() { // Queue Constructor
      head = nullptr;
      tail = nullptr;
      count=0;
      priorityShuffled = false;
    }

    void readDataFile() {
      ifstream dataIn("data.json");
      dataIn >> jsonData;
      dataIn.close();
      for (json item : jsonData) {
     
        this->addItem(
          item["title"].get<string>(), 
          item["type"].get<string>(), 
          item["tag"].get<string>(), 
          item["priority"].get<int>()
        );
      }
    }

    void saveToFile() {
      StudyItem* item = head;
      jsonData.clear();
      while(item) {
        json jsonObj = createJsonObj(item->title, item->type, item->tag, item->priority);
        jsonData.push_back(jsonObj);
        item = item->next;
      }
      ofstream dataOut("data.json");
      dataOut << jsonData.dump(4);
      dataOut.close();
    }
    //for each addition of new item
    void writeInDataFile(string title, string type, string tag, int priority) {
      json newJsonItem = createJsonObj(title, type, tag, priority);
      jsonData.push_back(newJsonItem);

      // Writing to json file
      ofstream dataOut("data.json");
      dataOut << jsonData.dump(4);
      dataOut.close();
    }

    json createJsonObj(string title, string type, string tag, int priority) {
      json newItem;
      newItem["title"] = title;
      newItem["type"] = type;
      newItem["tag"] = tag;
      newItem["priority"] = priority;
      return newItem;
    }

    void addItem(string title, string type, string tag, int priority) {     // add item
      StudyItem* newItem = new StudyItem(count, title, type, tag, priority);
      if(tail == nullptr) {
        head = newItem;
        tail = head;
      } else {
        tail->next = newItem;
        tail = tail->next;
      }
      searchTable[title] = newItem;
      minHeap.push(newItem);
      ++count;

      // Setting the shuffled flag to false to enable further shuffle(s) with the new element
      priorityShuffled = false;
    }

    StudyItem* next() {            // show front of queue
      if(isEmpty()) return nullptr;
      
      undoStack.push(head);
      head=head->next;
      saveToFile();
      return undoStack.top();
    }

    StudyItem* undo() {           // undos the next operation
      if(undoStack.empty())
        return nullptr;

      StudyItem* undoItem = undoStack.top();
      undoStack.pop();
      undoItem->next = head;
      head = undoItem;
      saveToFile();
      return undoItem;
    }

    StudyItem* skipItem() {           // dequeue
      return next();
    }

    bool isEmpty() {
      return head == nullptr;
    }

    StudyItem* searchItem(string keyword) {           // search in hashtable
      if(exists(keyword))
        return searchTable[keyword];
      
      return nullptr;
    }


    void shuffleQueue() {        // shuffle as per priority
      if(priorityShuffled) {
        cout << "The queue is already shuffled as per priority\n";
        return;
      }

      if(this->isEmpty()) {
        cout << "The queue is empty\n";
        return;
      }

      head = tail = nullptr;
      auto tempHeap = minHeap;
      while(!minHeap.empty()) {
        if(!head)
          head = tail = minHeap.top();
        else {
          tail->next = minHeap.top();
          tail = tail->next; 
        }
        minHeap.pop();
      }

      tail->next = nullptr;
      priorityShuffled = true;

      // Reinstantiating the minheap for further shuffles on addition of new elements
      minHeap = tempHeap;

      // clearing undoStack as it no longer makes sense immediately after a re-shuffling
      while(!undoStack.empty())
        undoStack.pop();

      cout << "-->Queue Shuffled according to priority Successfully.\n";
    }

    void displayAll() {         // show entire queue
      StudyItem* node = head;
      int cnt = 0;
      bool f = false;
      while(node) {
        if(!f)
          cout << "\n--------------\n";
        f = true;
        cout << ++cnt << ".\n";
        displayItem(node);
        node = node->next;
      }
    }

    bool exists(string title) {
      if(searchTable.find(title) != searchTable.end())
        return true;
      return false;
    }

} studyQueue;

bool back() {
  while (1) {
    cout << "Go back? (y/n): ";
    string s;
    cin >> s;
    if(s.length() == 1 && tolower(s[0]) == 'y') return true;
    else if (s.length() == 1 && tolower(s[0]) == 'n') return false;
    cout << "Incorrect response.\n";
  }
  return false;
}

void takeInput(string msg, int &n) {
  while(1) {
    cout << msg;
    cin >> n;
    if(n >= 1 && n <= 3) {
      --n;
      return; 
    }
    cout << "Incorrect option\n";
  }
}

bool ADD_ITEM() {
  string title;
  int type, tag;
  int priority;
  string tags[3] = {"DSA", "OS", "DBMS"};
  string types[3] = {"video", "pdf", "note"};

  while(1) {
    cout << "Enter title: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, title);
    if(studyQueue.exists(title)) {
      cout << "Item already exists with this title. Try something else\n";
      continue;
    }
    break;
  }
  takeInput("Enter type\n1. video\n2. pdf\n3. note\nChoose option: ", type);
  takeInput("Enter tag\n1. DSA\n2. OS\n3. DBMS\nChoose option: ", tag);
  cout << "Enter priority (lower = more urgent): ";
  cin >> priority;
  studyQueue.addItem(title, types[type], tags[tag], priority);
  studyQueue.writeInDataFile(title, types[type], tags[tag], priority);  
  cout << "Item added successfully.\n";

  return back();
}

bool VIEW_NEXT() {
  if(studyQueue.isEmpty()) {
    cout << "No items in the study queue.\n";
    return back();
  }
  StudyItem* item = studyQueue.next();
  cout << "\n--------------\n";
  cout << "-->Next item to study:\n";
  displayItem(item);
  
  return back();
}

bool SEARCH_ITEM() {
  string keyword;
  cout << "-->Enter title to search: ";
  cin >> keyword;
  StudyItem* item = studyQueue.searchItem(keyword);
  if(item != nullptr) {
    cout << "-->Item found:\n";
    displayItem(item);
  } else
    cout << "-->Item not found.\n";
  return back();
}

bool UNDO_LAST_ACTION() {
  StudyItem* item = studyQueue.undo();
  if(!item)
    cout << "-->No actions to undo.\n";
  else
    cout << "-->Restored: " << item->title << " at front.\n";
  
  return back();
}

bool SKIP_NEXT_ITEM() {
  StudyItem* skippedItem = studyQueue.skipItem();
  if(!skippedItem)
    cout << "-->No items to skip.\n";
  else
    cout << "-->Skipped item: " << skippedItem->title << "\n";
  
  return back();
}

bool DISPLAY_ALL() {
  if(studyQueue.isEmpty()) {
    cout << "-->No items in the study queue.\n";
    return back();
  }

  studyQueue.displayAll();

  return back();
}

bool SHUFFLE_ORDER() { // shuffling as per priority
  studyQueue.shuffleQueue();
  studyQueue.saveToFile();
  
  return back();
}

void menu() {
  studyQueue.readDataFile();
  bool f = true;
  while(f) {
    cout << "\n--------------\n";
    cout << "1. Add item\n2. View next item\n3. Skip item\n4. Undo last action\n5. Search item\n6. Shuffle order as per priority\n7. Display All\n8. Exit\n";
    int n;
    cin >> n;
    switch(n) {
      case 1:
        f = ADD_ITEM();
        break;
      case 2:
        f = VIEW_NEXT();
        break;
      case 3:
        f = SKIP_NEXT_ITEM();
        break;
      case 4:
        f = UNDO_LAST_ACTION();
        break;
      case 5:
        f = SEARCH_ITEM();
        break;
      case 6:
        f = SHUFFLE_ORDER();
        break;
      case 7:
        f = DISPLAY_ALL();
        break;
      case 8:
        f = false;
        break;
      
      default:
        cout << "-->Incorrect option\n";
    }
  }
}

int main() {
  menu();
  return 0;
}
