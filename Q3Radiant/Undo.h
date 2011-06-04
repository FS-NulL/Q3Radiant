//
//
// QERadiant Multilevel Undo/Redo
//
// 

//start operation
void Undo_Start(char *operation);
//end operation
void Undo_End(void);
//add brush to the undo
void Undo_AddBrush(brush_t *pBrush);
//add a list with brushes to the undo
void Undo_AddBrushList(brush_t *brushlist);
//end a brush after the operation is performed
void Undo_EndBrush(brush_t *pBrush);
//end a list with brushes after the operation is performed
void Undo_EndBrushList(brush_t *brushlist);
//add entity to undo
void Undo_AddEntity(entity_t *entity);
//end an entity after the operation is performed
void Undo_EndEntity(entity_t *entity);
//undo last operation
void Undo_Undo(void);
//redo last undone operation
void Undo_Redo(void);
//returns true if there is something to be undone available
int  Undo_UndoAvailable(void);
//returns true if there is something to redo available
int  Undo_RedoAvailable(void);
//clear the undo buffer
void Undo_Clear(void);
//set maximum undo size (default 64)
void Undo_SetMaxSize(int size);
//get maximum undo size
int  Undo_GetMaxSize(void);
//set maximum undo memory in bytes (default 2 MB)
void Undo_SetMaxMemorySize(int size);
//get maximum undo memory in bytes
int  Undo_GetMaxMemorySize(void);
//returns the amount of memory used by undo
int  Undo_MemorySize(void);

