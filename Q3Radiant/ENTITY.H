    // entity.h
    
    void Eclass_InitForSourceDirectory (char *path);
    eclass_t *Eclass_ForName (char *name, qboolean has_brushes);
    
    //===================================================
    
    // forward declare this one
    class IPluginEntity;
    
    typedef struct entity_s
    {
    	struct entity_s	*prev, *next;
    	brush_t		brushes;					// head/tail of list
     	int			undoId, redoId, entityId;	// used for undo/redo
    	vec3_t		origin;
    	eclass_t	*eclass;
    	epair_t		*epairs;
    	eclass_t  *md3Class;
    	IPluginEntity *pPlugEnt;
      vec3_t vRotation;   // valid for misc_models only
      vec3_t vScale;      // valid for misc_models only
    } entity_t;
    
    char 	*ValueForKey (entity_t *ent, const char *key);
    void	SetKeyValue (entity_t *ent, const char *key, const char *value);
    void 	SetKeyValue (epair_t *&e, const char *key, const char *value);
    void 	DeleteKey (entity_t *ent, const char *key);
    void 	DeleteKey (epair_t *&e, const char *key);
    float	FloatForKey (entity_t *ent, const char *key);
    int		IntForKey (entity_t *ent, const char *key);
    void 	GetVectorForKey (entity_t *ent, const char *key, vec3_t vec);
    
    void		Entity_Free (entity_t *e);
    void		Entity_FreeEpairs(entity_t *e);
    int			Entity_MemorySize(entity_t *e);
    entity_t	*Entity_Parse (qboolean onlypairs, brush_t* pList = NULL);
    void		Entity_Write (entity_t *e, FILE *f, qboolean use_region);
    void		Entity_WriteSelected(entity_t *e, FILE *f);
    void		Entity_WriteSelected(entity_t *e, CMemFile*);
    entity_t	*Entity_Create (eclass_t *c);
    entity_t	*Entity_Clone (entity_t *e);
    void		Entity_AddToList(entity_t *e, entity_t *list);
    void		Entity_RemoveFromList(entity_t *e);
    
    void		Entity_LinkBrush (entity_t *e, brush_t *b);
    void		Entity_UnlinkBrush (brush_t *b);
    entity_t	*FindEntity(char *pszKey, char *pszValue);
    entity_t	*FindEntityInt(char *pszKey, int iValue);
    
    int GetUniqueTargetId(int iHint);
    qboolean Eclass_hasModel(eclass_t *e, vec3_t &vMin, vec3_t &vMax);
    eclass_t* GetCachedModel(entity_t *pEntity, const char *pName, vec3_t &vMin, vec3_t &vMax);
    
    //Timo : used for parsing epairs in brush primitive
    epair_t* ParseEpair(void);
    char *ValueForKey ( epair_t *&e, const char *key);
