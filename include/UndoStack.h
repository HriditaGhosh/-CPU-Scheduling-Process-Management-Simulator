#ifndef UNDO_STACK_H
#define UNDO_STACK_H

#include <vector>
#include "Process.h"

// UndoStack.h  (Lab 2 : Stack)
// ------------------------------
// Manual, linked-list based STACK (LIFO). No STL std::stack used. Records
// every Add / Delete / Update so the last one can be undone.

enum ActionType { ACTION_ADD, ACTION_DELETE, ACTION_UPDATE };

struct UndoAction {
    ActionType type = ACTION_ADD;
    Process beforeState; // valid for DELETE / UPDATE
    Process afterState;  // valid for ADD / UPDATE
    int index = -1;      // DELETE only: position in the process table, so Undo can put it back in the same slot

    // DELETE only: the dependency-graph edges this process had at the
    // moment it was deleted, so Undo can put them back exactly as they
    // were (deleteProcessFromManager wipes them from the graph itself).
    std::vector<int> outgoingEdges; // pids this process must finish before
    std::vector<int> incomingEdges; // pids that had to finish before this one
};

struct UndoNode {
    UndoAction action;
    UndoNode *next = nullptr;
};

struct UndoStack {
    UndoNode *top = nullptr;
    int count = 0;
};

void pushUndo(UndoStack &s, UndoAction action);
bool popUndo(UndoStack &s, UndoAction &outAction);
bool isUndoEmpty(const UndoStack &s);
int undoCount(const UndoStack &s);
void clearUndo(UndoStack &s);

#endif
