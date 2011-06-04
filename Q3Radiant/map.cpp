// map.c

#include "stdafx.h"
#include "qe3.h"
#include "PrefsDlg.h"

qboolean	modified;		// for quit confirmation (0 = clean, 1 = unsaved,
							// 2 = autosaved, but not regular saved) 

char		currentmap[1024];

brush_t	active_brushes;		// brushes currently being displayed
brush_t	selected_brushes;	// highlighted

face_t	*selected_face;
brush_t	*selected_face_brush;

brush_t	filtered_brushes;	// brushes that have been filtered or regioned

entity_t	entities;		// head/tail of doubly linked list

entity_t	*world_entity = NULL; // "classname" "worldspawn" !

void AddRegionBrushes (void);
void RemoveRegionBrushes (void);


void DupLists()
{
  DWORD dw = GetTickCount();

}

/*
=============================================================

  Cross map selection saving

  this could fuck up if you have only part of a complex entity selected...
=============================================================
*/

brush_t		between_brushes;
entity_t	between_entities;

bool g_bRestoreBetween = false;

void Map_SaveBetween (void)
{
	if (g_pParentWnd->ActiveXY())
  {
    g_bRestoreBetween = true;
    g_pParentWnd->ActiveXY()->Copy();
  }
}

void Map_RestoreBetween (void)
{
	if (g_pParentWnd->ActiveXY() && g_bRestoreBetween)
    g_pParentWnd->ActiveXY()->Paste();
}

//============================================================================

bool CheckForTinyBrush(brush_t* b, int n, float fSize)
{
  bool bTiny = false;
	for (int i=0 ; i<3 ; i++)
	{
    if (b->maxs[i] - b->mins[i] < fSize)
      bTiny = true;
  }
  if (bTiny)
    Sys_Printf("Possible problem brush (too small) #%i ", n);
  return bTiny;
}

void Map_BuildBrushData(void)
{
	brush_t	*b, *next;

	if (active_brushes.next == NULL)
		return;

	Sys_BeginWait ();	// this could take a while

  int n = 0;
	for (b=active_brushes.next ; b != NULL && b != &active_brushes ; b=next)
	{
		next = b->next;
		Brush_Build( b, true, false, false );
		if (!b->brush_faces || (g_PrefsDlg.m_bCleanTiny && CheckForTinyBrush(b, n++, g_PrefsDlg.m_fTinySize)))
		{
			Brush_Free (b);
			Sys_Printf ("Removed degenerate brush\n");
		}
	}
	Sys_EndWait();
}

entity_t *Map_FindClass (char *cname)
{
	entity_t	*ent;

	for (ent = entities.next ; ent != &entities ; ent=ent->next)
	{
		if (!strcmp(cname, ValueForKey (ent, "classname")))
			return ent;
	}
	return NULL;
}

/*
================
Map_Free
free all map elements, reinitialize the structures that depend on them
================
*/
void Map_Free (void)
{
  g_bRestoreBetween = false;
	if (selected_brushes.next &&
		(selected_brushes.next != &selected_brushes) )
	{
    if (MessageBox(g_qeglobals.d_hwndMain, "Copy selection?", "", MB_YESNO) == IDYES)
		  Map_SaveBetween ();
	}

	QERApp_ActiveShaders_SetInUse( false );
	Pointfile_Clear ();
	strcpy (currentmap, "unnamed.map");
	Sys_SetTitle (currentmap);
	g_qeglobals.d_num_entities = 0;

	if (!active_brushes.next)
	{	
		// first map
		active_brushes.prev = active_brushes.next = &active_brushes;
		selected_brushes.prev = selected_brushes.next = &selected_brushes;
		filtered_brushes.prev = filtered_brushes.next = &filtered_brushes;
		entities.prev = entities.next = &entities;
	}
	else
	{
		// free selected faces array
		g_ptrSelectedFaces.RemoveAll();
    g_ptrSelectedFaceBrushes.RemoveAll();
		while (active_brushes.next != &active_brushes)
			Brush_Free (active_brushes.next);
		while (selected_brushes.next != &selected_brushes)
			Brush_Free (selected_brushes.next);
		while (filtered_brushes.next != &filtered_brushes)
			Brush_Free (filtered_brushes.next);
		while (entities.next != &entities)
			Entity_Free (entities.next);
	}

  if (world_entity)
    Entity_Free(world_entity);
	world_entity = NULL;
}

entity_t *AngledEntity()
{
  entity_t *ent = Map_FindClass ("info_player_start");
  if (!ent)
  {
    ent = Map_FindClass ("info_ut_spawn");
  }
  if (!ent)
  {
		ent = Map_FindClass ("info_player_deathmatch");
  }
  if (!ent)
  {
		ent = Map_FindClass ("info_player_deathmatch");
  }
  if (!ent)
  {
    ent = Map_FindClass ("team_CTF_redplayer");
  }
  if (!ent)
  {
    ent = Map_FindClass ("team_CTF_blueplayer");
  }
  if (!ent)
  {
    ent = Map_FindClass ("team_CTF_redspawn");
  }
  if (!ent)
  {
    ent = Map_FindClass ("team_CTF_bluespawn");
  }
  return ent;
}



/*
================
Map_LoadFile
================
*/
bool  g_bCancel_Map_LoadFile;
void Map_LoadFile (char *filename)
{
    char		*buf;
	entity_t	*ent;
	char         temp[1024];

	Sys_BeginWait ();
	Select_Deselect();
	//SetInspectorMode(W_CONSOLE);

	QE_ConvertDOSToUnixName( temp, filename );
	Sys_Printf ("Map_LoadFile: %s\n", temp );

	Map_Free ();
	//++timo FIXME: maybe even easier to have Group_Init called from Map_Free?
	Group_Init();

	g_qeglobals.d_parsed_brushes = 0;
	strcpy (currentmap, filename);

	if (LoadFile (filename, (void **)&buf) != -1)
	{

		StartTokenParsing (buf);
		g_qeglobals.d_num_entities = 0;

		// Timo
		// will be used in Entity_Parse to detect if a conversion between brush formats is needed
		g_qeglobals.bNeedConvert = false;
		g_qeglobals.bOldBrushes = false;
		g_qeglobals.bPrimitBrushes = false;

    // cancel the map loading process
    // used when conversion between standard map format and BP format is required and the user cancels the process
    //++timo TODO: maybe this can be used if something fails while loading the map
    //++timo FIXME: remove later when mixing BP and standard format 
    g_bCancel_Map_LoadFile = false;

		while (1)
		{
			ent = Entity_Parse (false, &active_brushes);
			if (!ent)
				break;
			if (!strcmp(ValueForKey (ent, "classname"), "worldspawn"))
			{
				if (world_entity)
				{
					Sys_Printf ("WARNING: multiple worldspawn, ");
					// do we have brushes in the worldspawn?
					if (ent->brushes.onext != &ent->brushes)
					{
						Sys_Printf("merging brushes.\n");
						// do a scan to change owner on ent's brushes
						brush_t *b;
						for ( b = ent->brushes.onext; b!=&ent->brushes ; b=b->onext)
							b->owner = world_entity;
						// move brushes from ent to world_entity
						ent->brushes.oprev->onext = world_entity->brushes.onext;
						world_entity->brushes.onext->oprev = ent->brushes.oprev;
						world_entity->brushes.onext = ent->brushes.onext;
						world_entity->brushes.onext->oprev = &world_entity->brushes;
						// unhook from ent
						ent->brushes.onext = ent->brushes.oprev = & ent->brushes;
					}
					else
						Sys_Printf("ignored.\n");
					Entity_Free(ent);
				}
				else
					world_entity = ent;
			}
			else if (!strcmp(ValueForKey (ent, "classname"), "group_info"))
      {
        // it's a group thing!
        Group_Add(ent);
        Entity_Free(ent);
      }
      else
			{
				// add the entity to the end of the entity list
				ent->next = &entities;
				ent->prev = entities.prev;
				entities.prev->next = ent;
				entities.prev = ent;
				g_qeglobals.d_num_entities++;
			}
		}
	}

  free (buf);

  if (g_bCancel_Map_LoadFile)
  {
    Sys_Printf("Map_LoadFile canceled\n");
    Map_New();
    return;
  }

	if (!world_entity)
	{
		Sys_Printf ("No worldspawn in map.\n");
		Map_New ();
		return;
	}

  Sys_Printf ("--- LoadMapFile ---\n");
  Sys_Printf ("%s\n", temp );
  
  Sys_Printf ("%5i brushes\n",  g_qeglobals.d_parsed_brushes );
  Sys_Printf ("%5i entities\n", g_qeglobals.d_num_entities);
  
  Sys_EndWait();
  
  Map_RestoreBetween ();
  
	Sys_Printf ("Map_BuildBrushData\n");
    
  // speed increase: turn querying the texture/shader off, we got it in Map_ImportBuffer
  g_bBuildWindingsNoTexBuild = true;
  Map_BuildBrushData();
  g_bBuildWindingsNoTexBuild = false;

	// reset the "need conversion" flag
	// conversion to the good format done in Map_BuildBrushData
	g_qeglobals.bNeedConvert=false;

	//
	// move the view to a start position
	//
  ent = AngledEntity();

  g_pParentWnd->GetCamera()->Camera().angles[PITCH] = 0;
	if (ent)
	{
		GetVectorForKey (ent, "origin", g_pParentWnd->GetCamera()->Camera().origin);
		GetVectorForKey (ent, "origin", g_pParentWnd->GetXYWnd()->GetOrigin());
		g_pParentWnd->GetCamera()->Camera().angles[YAW] = FloatForKey (ent, "angle");
	}
	else
	{
		g_pParentWnd->GetCamera()->Camera().angles[YAW] = 0;
		VectorCopy (vec3_origin, g_pParentWnd->GetCamera()->Camera().origin);
		VectorCopy (vec3_origin, g_pParentWnd->GetXYWnd()->GetOrigin());
	}

	Map_RegionOff ();

	modified = false;
	Sys_SetTitle (temp);

	Texture_ShowInuse ();
//	Texture_SortTextures();
	QERApp_SortActiveShaders();

//	Sys_EndWait();
	Sys_UpdateWindows (W_ALL);

}

/*
===========
Map_SaveFile
===========
*/
void Map_SaveFile (char *filename, qboolean use_region )
{
	entity_t	*e, *next;
	FILE		*f;
	char         temp[1024];
	int			count;

  if (filename == NULL || strlen(filename) == 0)
  {
    CFileDialog dlgSave(FALSE, "map", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Map Files (*.map)|*.map||", AfxGetMainWnd());
    if (dlgSave.DoModal() == IDOK)
      filename = strdup(dlgSave.m_ofn.lpstrFile);
    else 
      return;
  }

	Pointfile_Clear ();
	QE_ConvertDOSToUnixName( temp, filename );

	if (!use_region)
	{
		char	backup[1024];

		// rename current to .bak
		strcpy (backup, filename);
		StripExtension (backup);
		strcat (backup, ".bak");
		_unlink (backup);
		rename (filename, backup);
	}

	Sys_Printf ("Map_SaveFile: %s\n", filename);

	f = fopen(filename, "w");

	if (!f)
	{
		Sys_Printf ("ERROR!!!! Couldn't open %s\n", filename);
		return;
	}

	if (use_region)
  {
		AddRegionBrushes ();
  }

	// write world entity first
	Entity_Write (world_entity, f, use_region);

	// then write all other ents
	count = 1;
	for (e=entities.next ; e != &entities ; e=next)
	{
		next = e->next;
		if (e->brushes.onext == &e->brushes)
    {
			Entity_Free (e);	// no brushes left, so remove it
    }
		else
    {
	   	fprintf (f, "// entity %i\n", count);
	  	count++;
			Entity_Write (e, f, use_region);
    }
	}

	// save the group info stuff
	Group_Save(f);

	fclose (f);

	if (use_region)
		RemoveRegionBrushes ();

	Sys_Printf ("Saved.\n");
	modified = false;

	if ( !strstr( temp, "autosave" ) )
		Sys_SetTitle (temp);

	if (!use_region)
	{
		time_t	timer;
		FILE	*f;

		time (&timer);
		MessageBeep (MB_ICONEXCLAMATION);

		// JONATHAN: Why is this even here?
#ifdef _DEBUG
		f = fopen ("c:/tstamps.log", "a");
		if (f)
		{
			fprintf (f, "%s \n", filename);
			//fprintf (f, "%4i : %35s : %s", g_qeglobals.d_workcount, filename, ctime(&timer));
			fclose (f);
			g_qeglobals.d_workcount = 0;
			fclose (f);
		}
#endif
		Sys_Status ("Saved.\n", 0);
	}
	
  //Curve_WriteFile (filename);		//.trinity
  //Patch_WriteFile (filename);
}

/*
===========
Map_New

===========
*/
void Map_New (void)
{
	Sys_Printf ("Map_New\n");
	Map_Free ();

	world_entity = (entity_s*)qmalloc(sizeof(*world_entity));
	world_entity->brushes.onext = 
		world_entity->brushes.oprev = &world_entity->brushes;
	SetKeyValue (world_entity, "classname", "worldspawn");
	world_entity->eclass = Eclass_ForName ("worldspawn", true);

	g_pParentWnd->GetCamera()->Camera().angles[YAW] = 0;
	g_pParentWnd->GetCamera()->Camera().angles[PITCH] = 0;
	VectorCopy (vec3_origin, g_pParentWnd->GetCamera()->Camera().origin);
	g_pParentWnd->GetCamera()->Camera().origin[2] = 48;
	VectorCopy (vec3_origin, g_pParentWnd->GetXYWnd()->GetOrigin());

	Map_RestoreBetween ();

  Group_Init();

	Sys_UpdateWindows (W_ALL);
	modified = false;
}


/*
===========================================================

  REGION

===========================================================
*/
#if 0
qboolean	region_active;
vec3_t	region_mins = {-4096, -4096, -4096};
vec3_t	region_maxs = {4096, 4096, 4096};

brush_t	*region_sides[6];

/*
===========
AddRegionBrushes

a regioned map will have temp walls put up at the region boundary
===========
*/
void AddRegionBrushes (void)
{
	vec3_t	mins, maxs;
	int		i;
	texdef_t	td;

	if (!region_active)
		return;

	memset (&td, 0, sizeof(td));
	//strcpy (td.name, "REGION");
	td.SetName("REGION");

	mins[0] = region_mins[0] - 16;
	maxs[0] = region_mins[0] + 1;
	mins[1] = region_mins[1] - 16;
	maxs[1] = region_maxs[1] + 16;
	mins[2] = -2048;
	maxs[2] = 2048;
	region_sides[0] = Brush_Create (mins, maxs, &td);

	mins[0] = region_maxs[0] - 1;
	maxs[0] = region_maxs[0] + 16;
	region_sides[1] = Brush_Create (mins, maxs, &td);

	mins[0] = region_mins[0] - 16;
	maxs[0] = region_maxs[0] + 16;
	mins[1] = region_mins[1] - 16;
	maxs[1] = region_mins[1] + 1;
	region_sides[2] = Brush_Create (mins, maxs, &td);

	mins[1] = region_maxs[1] - 1;
	maxs[1] = region_maxs[1] + 16;
	region_sides[3] = Brush_Create (mins, maxs, &td);

	mins[0] = region_mins[0];
	maxs[0] = region_maxs[0];
	mins[1] = region_mins[1];
	maxs[1] = region_maxs[1];
	mins[2] = -2048;
	maxs[2] = -2048 + 16;
	region_sides[4] = Brush_Create (mins, maxs, &td);

	mins[2] = 2048;
	maxs[2] = 2048 + 16;
	region_sides[5] = Brush_Create (mins, maxs, &td);

	for (i=0 ; i<6 ; i++)
	{
		Brush_AddToList (region_sides[i], &selected_brushes);
		Entity_LinkBrush (world_entity, region_sides[i]);
		Brush_Build(region_sides[i]);
	}
}

void RemoveRegionBrushes (void)
{
	int		i;

	if (!region_active)
		return;
	for (i=0 ; i<6 ; i++)
		Brush_Free (region_sides[i]);
}

#endif/*
===========================================================

  REGION

===========================================================
*/

qboolean	region_active;
vec3_t	region_mins = {-8192, -8192, -8192};
vec3_t	region_maxs = {8192, 8192, 8192};

brush_t	*region_sides[4];
/*
===========
AddRegionBrushes

a regioned map will have temp walls put up at the region boundary
===========
*/
void AddRegionBrushes (void)
{
	vec3_t	mins, maxs;
	int		i;
	texdef_t	td;

	if (!region_active)
		return;

	memset (&td, 0, sizeof(td));
	//strcpy (td.name, "REGION");
	td.SetName("region");

	mins[0] = region_mins[0] - 16;
	maxs[0] = region_mins[0] + 1;
	mins[1] = region_mins[1] - 16;
	maxs[1] = region_maxs[1] + 16;
	mins[2] = -2048;
	maxs[2] = 2048;
	region_sides[0] = Brush_Create (mins, maxs, &td);

	mins[0] = region_maxs[0] - 1;
	maxs[0] = region_maxs[0] + 16;
	region_sides[1] = Brush_Create (mins, maxs, &td);

	mins[0] = region_mins[0] - 16;
	maxs[0] = region_maxs[0] + 16;
	mins[1] = region_mins[1] - 16;
	maxs[1] = region_mins[1] + 1;
	region_sides[2] = Brush_Create (mins, maxs, &td);

	mins[1] = region_maxs[1] - 1;
	maxs[1] = region_maxs[1] + 16;
	region_sides[3] = Brush_Create (mins, maxs, &td);

	for (i=0 ; i<4 ; i++)
	{
		Brush_AddToList (region_sides[i], &selected_brushes);
		Entity_LinkBrush (world_entity, region_sides[i]);
		Brush_Build( region_sides[i] );
	}
}

void RemoveRegionBrushes (void)
{
	int		i;

	if (!region_active)
		return;
	for (i=0 ; i<4 ; i++)
		Brush_Free (region_sides[i]);
}


qboolean Map_IsBrushFiltered (brush_t *b)
{
	int		i;

	for (i=0 ; i<3 ; i++)
	{
		if (b->mins[i] > region_maxs[i])
			return true;
		if (b->maxs[i] < region_mins[i])
			return true;
	}
	return false;
}

/*
===========
Map_RegionOff

Other filtering options may still be on
===========
*/
void Map_RegionOff (void)
{
	brush_t	*b, *next;
	int			i;

	region_active = false;
	for (i=0 ; i<3 ; i++)
	{
		region_maxs[i] = 8192;
		region_mins[i] = -8192;
	}

	for (b=filtered_brushes.next ; b != &filtered_brushes ; b=next)
		{
		next = b->next;
		if (Map_IsBrushFiltered (b))
			continue;		// still filtered
		Brush_RemoveFromList (b);
		if (active_brushes.next == NULL || active_brushes.prev == NULL)
		{
			active_brushes.next = &active_brushes;
			active_brushes.prev = &active_brushes;
		}
		Brush_AddToList (b, &active_brushes);
		}

	Sys_UpdateWindows (W_ALL);
}

void Map_ApplyRegion (void)
{
	brush_t	*b, *next;

	region_active = true;
	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;
		if (!Map_IsBrushFiltered (b))
			continue;		// still filtered
		Brush_RemoveFromList (b);
		Brush_AddToList (b, &filtered_brushes);
	}

	Sys_UpdateWindows (W_ALL);
}


/*
========================
Map_RegionSelectedBrushes
========================
*/
void Map_RegionSelectedBrushes (void)
{
	Map_RegionOff ();

	if (selected_brushes.next == &selected_brushes)  // nothing selected
  {
    Sys_Printf("Tried to region with no selection...\n");
    return;
  }
	region_active = true;
	Select_GetBounds (region_mins, region_maxs);

#ifdef _DEBUG
	if (filtered_brushes.next != &filtered_brushes)
		Sys_Printf("WARNING: filtered_brushes list may not be empty in Map_RegionSelectedBrushes\n");
#endif

	if (active_brushes.next == &active_brushes)
	{
		// just have an empty filtered_brushes list
		// this happens if you set region after selecting all the brushes in your map (some weird people do that, ask MrE!)
		filtered_brushes.next = filtered_brushes.prev = &filtered_brushes;
	}
	else
	{
		// move the entire active_brushes list to filtered_brushes
		filtered_brushes.next = active_brushes.next;
		filtered_brushes.prev = active_brushes.prev;
		filtered_brushes.next->prev = &filtered_brushes;
		filtered_brushes.prev->next = &filtered_brushes;
	}

	// move the entire selected_brushes list to active_brushes
	active_brushes.next = selected_brushes.next;
	active_brushes.prev = selected_brushes.prev;
	active_brushes.next->prev = &active_brushes;
	active_brushes.prev->next = &active_brushes;

	// clear selected_brushes
	selected_brushes.next = selected_brushes.prev = &selected_brushes;

	Sys_UpdateWindows (W_ALL);
}


/*
===========
Map_RegionXY
===========
*/
void Map_RegionXY (void)
{
	Map_RegionOff ();

	region_mins[0] = g_pParentWnd->GetXYWnd()->GetOrigin()[0] - 0.5 * g_pParentWnd->GetXYWnd()->Width() / g_pParentWnd->GetXYWnd()->Scale();
	region_maxs[0] = g_pParentWnd->GetXYWnd()->GetOrigin()[0] + 0.5 * g_pParentWnd->GetXYWnd()->Width() / g_pParentWnd->GetXYWnd()->Scale();
	region_mins[1] = g_pParentWnd->GetXYWnd()->GetOrigin()[1] - 0.5 * g_pParentWnd->GetXYWnd()->Height() / g_pParentWnd->GetXYWnd()->Scale();
	region_maxs[1] = g_pParentWnd->GetXYWnd()->GetOrigin()[1] + 0.5 * g_pParentWnd->GetXYWnd()->Height() / g_pParentWnd->GetXYWnd()->Scale();
	region_mins[2] = -8192;
	region_maxs[2] = 8192;
	Map_ApplyRegion ();
}

/*
===========
Map_RegionTallBrush
===========
*/
void Map_RegionTallBrush (void)
{
	brush_t	*b;

	if (!QE_SingleBrush ())
		return;

	b = selected_brushes.next;

	Map_RegionOff ();

	VectorCopy (b->mins, region_mins);
	VectorCopy (b->maxs, region_maxs);
	region_mins[2] = -8192;
	region_maxs[2] = 8192;

	Select_Delete ();
	Map_ApplyRegion ();
}
/*
===========
Map_RegionBrush
===========
*/
void Map_RegionBrush (void)
{
	brush_t	*b;

	if (!QE_SingleBrush ())
		return;

	b = selected_brushes.next;

	Map_RegionOff ();

	VectorCopy (b->mins, region_mins);
	VectorCopy (b->maxs, region_maxs);

	Select_Delete ();
	Map_ApplyRegion ();
}



void UniqueTargetName(CString& rStr)
{
	// make a unique target value
	int maxtarg = 0;
	for (entity_t* e=entities.next ; e != &entities ; e=e->next)
	{
		char* tn = ValueForKey (e, "targetname");
		if (tn && tn[0])
		{
			int targetnum = atoi(tn+1);
			if (targetnum > maxtarg)
				maxtarg = targetnum;
		}
    else
    {
		  tn = ValueForKey (e, "target");
		  if (tn && tn[0])
		  {
			  int targetnum = atoi(tn+1);
			  if (targetnum > maxtarg)
				  maxtarg = targetnum;
		  }
    }
	}
  rStr.Format("t%i", maxtarg+1);
}

//
//================
//Map_ImportFile
// Timo 09/01/99 : called by CXYWnd::Paste & Map_ImportFile
// if Map_ImportFile ( prefab ), the buffer may contain brushes in old format ( conversion needed )
//================
//
void Map_ImportBuffer (char* buf)
{
	entity_t* ent;
	brush_t* b = NULL;
	CPtrArray ptrs;

	Select_Deselect();

	Undo_Start("import buffer");

	g_qeglobals.d_parsed_brushes = 0;
	if (buf)
	{
		CMapStringToString mapStr;
		StartTokenParsing (buf);
		g_qeglobals.d_num_entities = 0;

		// Timo
		// will be used in Entity_Parse to detect if a conversion between brush formats is needed
		g_qeglobals.bNeedConvert = false;
		g_qeglobals.bOldBrushes = false;
		g_qeglobals.bPrimitBrushes = false;

		while (1)
		{

			// use the selected brushes list as it's handy
			//ent = Entity_Parse (false, &selected_brushes);
			ent = Entity_Parse (false, &active_brushes);
			if (!ent)
				break;
			//end entity for undo
			Undo_EndEntity(ent);
			//end brushes for undo
			for(b = ent->brushes.onext; b && b != &ent->brushes; b = b->onext)
			{
				Undo_EndBrush(b);
			}

			if (!strcmp(ValueForKey (ent, "classname"), "worldspawn"))
			{
				// world brushes need to be added to the current world entity

				b=ent->brushes.onext;
				while (b && b != &ent->brushes)
				{
					brush_t* bNext = b->onext;
					Entity_UnlinkBrush(b);
					Entity_LinkBrush(world_entity, b);
					ptrs.Add(b);
					b = bNext;
				}
			}
			else
			{
				// the following bit remaps conflicting target/targetname key/value pairs
				CString str = ValueForKey(ent, "target");
				CString strKey;
				CString strTarget("");
				if (str.GetLength() > 0)
				{
					if (FindEntity("target", str.GetBuffer(0)))
					{
						if (!mapStr.Lookup(str, strKey))
						{
							UniqueTargetName(strKey);
							mapStr.SetAt(str, strKey);
						}
						strTarget = strKey;
						SetKeyValue(ent, "target", strTarget.GetBuffer(0));
					}
				}
				str = ValueForKey(ent, "targetname");
				if (str.GetLength() > 0)
				{
					if (FindEntity("targetname", str.GetBuffer(0)))
					{
						if (!mapStr.Lookup(str, strKey))
						{
							UniqueTargetName(strKey);
							mapStr.SetAt(str, strKey);
						}
						SetKeyValue(ent, "targetname", strKey.GetBuffer(0));
					}
				}
				//if (strTarget.GetLength() > 0)
				//  SetKeyValue(ent, "target", strTarget.GetBuffer(0));

				// add the entity to the end of the entity list
				ent->next = &entities;
				ent->prev = entities.prev;
				entities.prev->next = ent;
				entities.prev = ent;
				g_qeglobals.d_num_entities++;

				for (b=ent->brushes.onext ; b != &ent->brushes ; b=b->onext)
				{
					ptrs.Add(b);
				}
			}
		}
	}

	//::ShowWindow(g_qeglobals.d_hwndEntity, FALSE);
	//::LockWindowUpdate(g_qeglobals.d_hwndEntity);
	g_bScreenUpdates = false; 
	for (int i = 0; i < ptrs.GetSize(); i++)
	{
		Brush_Build(reinterpret_cast<brush_t*>(ptrs[i]), true, false);
		Select_Brush(reinterpret_cast<brush_t*>(ptrs[i]), true, false);
	}
	//::LockWindowUpdate(NULL);
	g_bScreenUpdates = true; 

	ptrs.RemoveAll();

	// reset the "need conversion" flag
	// conversion to the good format done in Map_BuildBrushData
	g_qeglobals.bNeedConvert=false;

	Sys_UpdateWindows (W_ALL);
  //Sys_MarkMapModified();
	modified = true;

	Undo_End();

}


//
//================
//Map_ImportFile
//================
//
void Map_ImportFile (char *filename)
{
  char* buf;
	char temp[1024];
	Sys_BeginWait ();
	QE_ConvertDOSToUnixName( temp, filename );
  if (LoadFile (filename, (void **)&buf) != -1)
  {
    Map_ImportBuffer(buf);
    free(buf);
    // speed increase: turn querying the texture/shader off, we got it in Map_ImportBuffer
    g_bBuildWindingsNoTexBuild = true;
    Map_BuildBrushData();
    g_bBuildWindingsNoTexBuild = false;
  }
	Sys_UpdateWindows (W_ALL);
	modified = true;
	Sys_EndWait();
}

//
//===========
//Map_SaveSelected
//===========
//
// Saves selected world brushes and whole entities with partial/full selections
//
void Map_SaveSelected(char* pFilename)
{
	entity_t	*e, *next;
	FILE *f;
	char temp[1024];
	int count;

	QE_ConvertDOSToUnixName(temp, pFilename);
	f = fopen(pFilename, "w");

	if (!f)
	{
		Sys_Printf ("ERROR!!!! Couldn't open %s\n", pFilename);
		return;
	}

	// write world entity first
	Entity_WriteSelected(world_entity, f);

	// then write all other ents
	count = 1;
	for (e=entities.next ; e != &entities ; e=next)
	{
  	fprintf (f, "// entity %i\n", count);
   	count++;
 		Entity_WriteSelected(e, f);
		next = e->next;
	}
	fclose (f);
}


//
//===========
//Map_SaveSelected
//===========
//
// Saves selected world brushes and whole entities with partial/full selections
//
void Map_SaveSelected(CMemFile* pMemFile, CMemFile* pPatchFile)
{
	entity_t	*e, *next;
	int count;
	CString strTemp;
  
	// write world entity first
	Entity_WriteSelected(world_entity, pMemFile);

	// then write all other ents
	count = 1;
	for (e=entities.next ; e != &entities ; e=next)
	{
		MemFile_fprintf(pMemFile, "// entity %i\n", count);
		count++;
 		Entity_WriteSelected(e, pMemFile);
		next = e->next;
	}

  //if (pPatchFile)
  //  Patch_WriteFile(pPatchFile);
}


void MemFile_fprintf(CMemFile* pMemFile, const char* pText, ...)
{
  char Buffer[4096];
  va_list args;
	va_start (args,pText);
  vsprintf(Buffer, pText, args);
  pMemFile->Write(Buffer, strlen(Buffer));
}