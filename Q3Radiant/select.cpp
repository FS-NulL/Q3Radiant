// select.c
#include "stdafx.h"
#include "qe3.h"

#define MODEL_COLLISION

// externs
CPtrArray g_SelectedFaces;
CPtrArray g_SelectedFaceBrushes;
CPtrArray& g_ptrSelectedFaces = g_SelectedFaces;
CPtrArray& g_ptrSelectedFaceBrushes = g_SelectedFaceBrushes;

void VectorTransformFast(vec3_t vin, vec3_t vCos, vec3_t vSin, vec3_t translate,vec3_t scale, vec3_t vout)
{
	float x2,y2,z2;

	// Scale
	VectorScale(vin,scale,vout);

	//Rotate
	y2 = vout[1] * vCos[2] - vout[2] * vSin[2];
	z2 = vout[1] * vSin[2] + vout[2] * vCos[2];
	vout[1] = y2;
	vout[2] = z2;

	z2 = vout[2] * vCos[0] - vout[0] * vSin[0];
	x2 = vout[2] * vSin[0] + vout[0] * vCos[0];
	vout[2] = z2;
	vout[0] = x2;

	x2 = vout[0] * vCos[1] - vout[1] * vSin[1];
	y2 = vout[0] * vSin[1] + vout[1] * vCos[1];
	vout[0] = x2;
	vout[1] = y2;

	// Translate
	VectorAdd(vout,translate,vout);
}

/*
==============
Triangle_Intersect

Determins Ray collision and gives distance to collision point
returns 1 on sucess 0 on fail
Moller-Trumbore technique
==============
*/
#define TI_EPSILON 0.00001
int Triangle_Intersect(vec3_t orig, vec3_t direction, vec3_t v0, vec3_t v1, vec3_t v2, float *t)
{
	float u,v,det,inv_det;
	vec3_t edge1, edge2,tvec,pvec,qvec;

	VectorSubtract(v1,v0,edge1);	
	VectorSubtract(v2,v0,edge2);

	CrossProduct(direction,edge2,pvec);

	det = DotProduct(edge1,pvec);

	if (det < TI_EPSILON) return 0;

	VectorSubtract(orig,v0,tvec);

	u = DotProduct(tvec,pvec);

	if (u<0 || u>det) return 0;

	CrossProduct(tvec,edge1,qvec);

	v = DotProduct(direction,qvec);

	if (v<0 || v+u > det) return 0;

	*t = DotProduct(edge2,qvec);

	inv_det = 1.0 / det;

	*t *= inv_det;
}

/*
==============
Model_Ray

Itersects a ray with a brush
Returns the face hit and the distance along the ray the intersection occured at
Returns NULL and 0 if not hit at all
==============
*/
// JONATHAN TODO: Model Selection
bool Model_Ray (entity_t *e,vec3_t origin, vec3_t dir, brush_t *b, float *dist)
{
	// Sanity Checks
	if (!e) return NULL;
	if (!(e->md3Class)) return NULL;
	// Possible Bounding Box Detection first - if we can get the box set properly first
	// Then Face Collision

	// transformed vertexes
	// Rotate about angles or angle then translate to entity origin
	vec3_t t0,t1,t2;
	vec3_t angles;
	char *value;

	// Fetch angles or angle key
	VectorClear(angles);
	GetVectorForKey(e,"angles",angles);
	bool useAngles=false;
    if ( (angles[0] != 0.0f) || (angles[1] != 0.0f) || (angles[2] != 0.0f) ) useAngles = true;
	if (!useAngles)
	{
		value = ValueForKey(e,"angle");
		angles[1] = atof(value);
	}

	value = ValueForKey(e,"modelscale");
	float modelscale = atof(value);
	if (modelscale == 0) modelscale = 1;

	vec3_t modelscale_vec;
	modelscale_vec[0] = modelscale;
	modelscale_vec[1] = modelscale;
	modelscale_vec[2] = modelscale;
	GetVectorForKey(e,"modelscale_vec",modelscale_vec);

	vec3_t vSin;
    vec3_t vCos;
    VectorClear(vSin);
    VectorClear(vCos);

	for (int j = 0; j < 3; j++)
    {
        vSin[j] = sin(angles[j]/180*Q_PI);
        vCos[j] = cos(angles[j]/180*Q_PI);
    }

	
	if (e->md3Class->model)
	{
		entitymodel * model = e->md3Class->model;
		//loop models
		while( model )
		{
			// Loop triangles
			for (int i=0; i < model->nTriCount; i++)
			{
				// Need to translate and rotate vertexes first // TODO:: change Vector transform fast to handle 3 scale arguments, not one!!!
				VectorTransformFast(model->pTriList[i].v[0],vCos,vSin,e->origin,modelscale_vec,t0);
				VectorTransformFast(model->pTriList[i].v[1],vCos,vSin,e->origin,modelscale_vec,t1);
				VectorTransformFast(model->pTriList[i].v[2],vCos,vSin,e->origin,modelscale_vec,t2); 
					

				if (Triangle_Intersect(origin, dir, t0, t2, t1, dist)) return true;
			}
			// Get the next surface in the model
			model = model->pNext;
		}
	}

	return false;
}


/*
===========
Test_Ray
===========
*/
// Jonathan TODO: Change model selection to line-face intersection
#define	DIST_START	999999
trace_t Test_Ray (vec3_t origin, vec3_t dir, int flags)
{
	brush_t	*brush;
	face_t	*face;
	float	dist;
	trace_t	t;

	memset (&t, 0, sizeof(t));
	t.dist = DIST_START;

	if (flags & SF_CYCLE)
	{
		CPtrArray array;
		brush_t *pToSelect = (selected_brushes.next != &selected_brushes) ? selected_brushes.next : NULL;
		// Deselect all brushes when ALT is pressed
		Select_Deselect();

		// go through active brushes and accumulate all "hit" brushes
		for (brush = active_brushes.next ; brush != &active_brushes ; brush=brush->next)
		{
			//if ( (flags & SF_ENTITIES_FIRST) && brush->owner == world_entity)
			//  continue;
		 
			if (FilterBrush (brush))
				continue;

     if (!g_PrefsDlg.m_bSelectCurves && brush->patchBrush)
       continue;

		 if (!g_PrefsDlg.m_bSelectModels && (brush->owner->eclass->nShowFlags & ECLASS_MISCMODEL))
			 continue;

			//if (!g_bShowPatchBounds && brush->patchBrush)
			//  continue;

			face = Brush_Ray (origin, dir, brush, &dist);

			if (face)
				array.Add(brush);
		}

		int nSize = array.GetSize();
		if (nSize > 0)
		{
			bool bFound = false;
			for (int i = 0; i < nSize; i++)
			{
				brush_t *b = reinterpret_cast<brush_t*>(array.GetAt(i));
				// did we hit the last one selected yet ?
				if (b == pToSelect)
				{
					// yes we want to select the next one in the list 
					int n = (i > 0) ? i-1 : nSize-1;
					pToSelect = reinterpret_cast<brush_t*>(array.GetAt(n));
					bFound = true;
					break;
				}
			}
			if (!bFound)
				pToSelect = reinterpret_cast<brush_t*>(array.GetAt(0));
		}
		if (pToSelect)
		{
			face = Brush_Ray (origin, dir, pToSelect, &dist);
			t.dist = dist;
			t.brush = pToSelect;
			t.face = face;
			t.selected = false;
			return t;
		}
	} // End of ALT- Cycle select

	if (! (flags & SF_SELECTED_ONLY) )
	{
		for (brush = active_brushes.next ; brush != &active_brushes ; brush=brush->next)
		{
			if ( (flags & SF_ENTITIES_FIRST) && brush->owner == world_entity)
				continue;
			
      if (FilterBrush (brush))
				continue;

      if (!g_PrefsDlg.m_bSelectCurves && brush->patchBrush)
        continue;

			if (!g_PrefsDlg.m_bSelectModels && (brush->owner->eclass->nShowFlags & ECLASS_MISCMODEL))
				continue;

      //if (!g_bShowPatchBounds && brush->patchBrush)
      //  continue;
			// JONATHAN TODO: Misc_model face selection
			// If its a model do model CD 
			// model if (brush->owner->eclass->nShowFlags & ECLASS_MISCMODEL)
#ifdef MODEL_COLLISION
			if (brush->owner->eclass->nShowFlags & ECLASS_MISCMODEL)
			{
				dist = -0;
				Model_Ray(brush->owner,origin, dir, brush, &dist);
			}
			else 
#endif
				face = Brush_Ray (origin, dir, brush, &dist); // Test Ray again brush (Meat and potatoes of the selection algo)
			if (dist > 0 && dist < t.dist)
			{
				t.dist = dist;
				t.brush = brush;
				t.face = face;
				t.selected = false;
			}
		}
  }


	for (brush = selected_brushes.next ; brush != &selected_brushes ; brush=brush->next)
	{
		if ( (flags & SF_ENTITIES_FIRST) && brush->owner == world_entity)
			continue;

    if (FilterBrush (brush))
			continue;

    if (!g_PrefsDlg.m_bSelectCurves && brush->patchBrush)
      continue;

	  if (!g_PrefsDlg.m_bSelectModels && (brush->owner->eclass->nShowFlags & ECLASS_MISCMODEL))
			continue;
#ifdef MODEL_COLLISION
			if (brush->owner->eclass->nShowFlags & ECLASS_MISCMODEL)
			{
				dist = -0;
				Model_Ray(brush->owner,origin, dir, brush, &dist);
			}
			else 
#endif
		face = Brush_Ray (origin, dir, brush, &dist);
		if (dist > 0 && dist < t.dist)
		{
			t.dist = dist;
			t.brush = brush;
			t.face = face;
			t.selected = true;
		}
	}

	// if entites first, but didn't find any, check regular

	if ( (flags & SF_ENTITIES_FIRST) && t.brush == NULL)
		return Test_Ray (origin, dir, flags - SF_ENTITIES_FIRST);

	return t;

}


/*
============
Select_Brush

============
*/
void Select_Brush (brush_t *brush, bool bComplete, bool bStatus)
{
	brush_t	*b;
	entity_t	*e;

  g_ptrSelectedFaces.RemoveAll();
  g_ptrSelectedFaceBrushes.RemoveAll();
	//selected_face = NULL;
	if (g_qeglobals.d_select_count < 2)
		g_qeglobals.d_select_order[g_qeglobals.d_select_count] = brush;
	g_qeglobals.d_select_count++;

  //if (brush->patchBrush)
  //  Patch_Select(brush->nPatchID);

	e = brush->owner;
	if (e)
	{
		// select complete entity on first click
		if (e != world_entity && bComplete == true)
		{
			for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
				if (b->owner == e)
					goto singleselect;
			for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
			{
        Brush_RemoveFromList (b);
				Brush_AddToList (b, &selected_brushes);
			}
		}
		else
		{
singleselect:
			Brush_RemoveFromList (brush);
			Brush_AddToList (brush, &selected_brushes);
      UpdateSurfaceDialog();
      UpdatePatchInspector();
		}

		if (e->eclass)
		{
			UpdateEntitySel(brush->owner->eclass);
		}
	}
  if (bStatus)
  {
    vec3_t vMin, vMax, vSize;
	  Select_GetBounds (vMin, vMax);
    VectorSubtract(vMax, vMin, vSize);
    CString strStatus;
    strStatus.Format("Selection X:: %.1f  Y:: %.1f  Z:: %.1f", vSize[0], vSize[1], vSize[2]);
    g_pParentWnd->SetStatusText(2, strStatus);
  }
}


/*
============
Select_Ray

If the origin is inside a brush, that brush will be ignored.
============
*/
void Select_Ray (vec3_t origin, vec3_t dir, int flags)
{
	trace_t	t;
	t.face = 0;

	t = Test_Ray (origin, dir, flags);
	if (!t.brush)
		return;

	if (flags == SF_SINGLEFACE)
	{
    int nCount = g_SelectedFaces.GetSize();
    bool bOk = true;
		// NOTE: keep the size check in the loop, we remove stuff inside
    for (int i = 0; i < g_SelectedFaces.GetSize(); i++)
    {
      if (t.face == reinterpret_cast<face_t*>(g_SelectedFaces.GetAt(i)))
      {
        bOk = false;
        // need to remove i'th entry
        g_SelectedFaces.RemoveAt(i, 1);
        g_SelectedFaceBrushes.RemoveAt(i, 1);
      }
    }
    if (bOk)
    {
	  if (!t.face) return;
	  if (!strcmp(t.brush->owner->eclass->name,"misc_model")) return; // Don't select model faces, or hell will ensue 
	  //if (!(t.face->texdef)) return; // Brush faces have texture definitions, other objects do not
      g_SelectedFaces.Add(t.face);
      g_SelectedFaceBrushes.Add(t.brush);
    }
		Sys_UpdateWindows (W_ALL);
		g_qeglobals.d_select_mode = sel_brush;
		// Texture_SetTexture requires a brushprimit_texdef fitted to the default width=2 height=2 texture
	  brushprimit_texdef_t brushprimit_texdef;
	  ConvertTexMatWithQTexture ( &t.face->brushprimit_texdef, t.face->d_texture, &brushprimit_texdef, NULL );
	  Texture_SetTexture ( &t.face->texdef, &brushprimit_texdef, false, GETPLUGINTEXDEF(t.face), false );
	  UpdateSurfaceDialog();
		return;
	}

	// move the brush to the other list

	g_qeglobals.d_select_mode = sel_brush;

	if (t.selected)
	{		
		Brush_RemoveFromList (t.brush);
		Brush_AddToList (t.brush, &active_brushes);
    UpdatePatchInspector();
	} 
  else
	{
		Select_Brush (t.brush, !(GetKeyState(VK_MENU) & 0x8000));
	}

	Sys_UpdateWindows (W_ALL);
}


void Select_Delete (void)
{
	brush_t	*brush;

  g_ptrSelectedFaces.RemoveAll();
  g_ptrSelectedFaceBrushes.RemoveAll();
	//selected_face = NULL;
	
  g_qeglobals.d_select_mode = sel_brush;

	g_qeglobals.d_select_count = 0;
	g_qeglobals.d_num_move_points = 0;
	while (selected_brushes.next != &selected_brushes)
	{
		brush = selected_brushes.next;
    if (brush->patchBrush)
    {
      //Patch_Delete(brush->nPatchID);
      Patch_Delete(brush->pPatch);
    }
		Brush_Free (brush);
	}

	// FIXME: remove any entities with no brushes

	Sys_UpdateWindows (W_ALL);
}

// update the workzone to a given brush
void UpdateWorkzone_ForBrush( brush_t* b )
{
  VectorCopy( b->mins, g_qeglobals.d_work_min );
  VectorCopy( b->maxs, g_qeglobals.d_work_max );
  //++timo clean
#if 0
	// will update the workzone to the given brush
	// g_pParentWnd->ActiveXY()->GetViewType()
	// cf VIEWTYPE defintion: enum VIEWTYPE {YZ, XZ, XY};
	// we fit our work zone to the last brush on the list (b)
	int nViewType = g_pParentWnd->ActiveXY()->GetViewType();
  int nDim1 = (nViewType == YZ) ? 1 : 0;
  int nDim2 = (nViewType == XY) ? 1 : 2;
	g_qeglobals.d_work_min[nDim1] = b->mins[nDim1];
	g_qeglobals.d_work_max[nDim1] = b->maxs[nDim1];
	g_qeglobals.d_work_min[nDim2] = b->mins[nDim2];
	g_qeglobals.d_work_max[nDim2] = b->maxs[nDim2];
#endif
}

void Select_Deselect (bool bDeselectFaces)
{
	brush_t	*b;

  Patch_Deselect();

  g_pParentWnd->ActiveXY()->UndoClear();

  g_qeglobals.d_workcount++;
	g_qeglobals.d_select_count = 0;
	g_qeglobals.d_num_move_points = 0;
	b = selected_brushes.next;

	if (b == &selected_brushes)
	{
		if (bDeselectFaces)
		{
			g_ptrSelectedFaces.RemoveAll();
      g_ptrSelectedFaceBrushes.RemoveAll();
      //selected_face = NULL;
		}
 		Sys_UpdateWindows (W_ALL);
		return;
	}

  if (bDeselectFaces)
  {
  	g_ptrSelectedFaces.RemoveAll();
    g_ptrSelectedFaceBrushes.RemoveAll();
	  //selected_face = NULL;
  }

	g_qeglobals.d_select_mode = sel_brush;

	UpdateWorkzone_ForBrush(b);

	selected_brushes.next->prev = &active_brushes;
	selected_brushes.prev->next = active_brushes.next;
	active_brushes.next->prev = selected_brushes.prev;
	active_brushes.next = selected_brushes.next;
	selected_brushes.prev = selected_brushes.next = &selected_brushes;	

	Sys_UpdateWindows (W_ALL);
}

/*
============
Select_Move
============
*/
void Select_Move (vec3_t delta, bool bSnap)
{
	brush_t	*b;
  
 
// actually move the selected brushes
	for (b = selected_brushes.next ; b != &selected_brushes ; b=b->next)
		Brush_Move (b, delta, bSnap);

  vec3_t vMin, vMax;
	Select_GetBounds (vMin, vMax);
  CString strStatus;
  strStatus.Format("Origin X:: %.1f  Y:: %.1f  Z:: %.1f", vMin[0], vMax[1], vMax[2]);
  g_pParentWnd->SetStatusText(2, strStatus);

//	Sys_UpdateWindows (W_ALL);
}

/*
============
Select_Clone

Creates an exact duplicate of the selection in place, then moves
the selected brushes off of their old positions
============
*/
void Select_Clone (void)
{
#if 1
  ASSERT(g_pParentWnd->ActiveXY());
  g_bScreenUpdates = false;  
  g_pParentWnd->ActiveXY()->Copy();
  g_pParentWnd->ActiveXY()->Paste();
  g_pParentWnd->NudgeSelection(2, g_qeglobals.d_gridsize);
  g_pParentWnd->NudgeSelection(3, g_qeglobals.d_gridsize);
  g_bScreenUpdates = true;  
  Sys_UpdateWindows(W_ALL);
#else

	brush_t		*b, *b2, *n, *next, *next2;
	vec3_t		delta;
	entity_t	*e;

	g_qeglobals.d_workcount++;
	g_qeglobals.d_select_mode = sel_brush;

	delta[0] = g_qeglobals.d_gridsize;
	delta[1] = g_qeglobals.d_gridsize;
	delta[2] = 0;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=next)
	{
		next = b->next;
		// if the brush is a world brush, handle simply
		if (b->owner == world_entity)
		{
			n = Brush_Clone (b);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (world_entity, n);
			Brush_Build( n );
			Brush_Move (b, delta);
			continue;
		}

		e = Entity_Clone (b->owner);
		// clear the target / targetname
		DeleteKey (e, "target");
		DeleteKey (e, "targetname");

		// if the brush is a fixed size entity, create a new entity
		if (b->owner->eclass->fixedsize)
		{
			n = Brush_Clone (b);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (e, n);
			Brush_Build( n );
			Brush_Move (b, delta);
			continue;
		}
        
		// brush is a complex entity, grab all the other ones now

		next = &selected_brushes;

		for ( b2 = b ; b2 != &selected_brushes ; b2=next2)
		{
			next2 = b2->next;
			if (b2->owner != b->owner)
			{
				if (next == &selected_brushes)
					next = b2;
				continue;
			}

			// move b2 to the start of selected_brushes,
			// so it won't be hit again
			Brush_RemoveFromList (b2);
			Brush_AddToList (b2, &selected_brushes);
			
			n = Brush_Clone (b2);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (e, n);
			Brush_Build( n );
			Brush_Move (b2, delta, true);
		}

	}
	Sys_UpdateWindows (W_ALL);
#endif
}

//++timo clean
#if 0
/*
============
Select_SetTexture
Timo : bFitScale to compute scale on the plane and counteract plane / axial plane snapping
Timo :	brush primitive texturing
		the brushprimit_texdef given must be understood as a qtexture_t width=2 height=2 ( HiRes )
Timo :  texture plugin, added an IPluginTexdef* parameter
		must be casted to an IPluginTexdef!
		if not NULL, get ->Copy() of it into each face or brush ( and remember to hook )
		if NULL, means we have no information, ask for a default
TTimo - shader code cleanup
  added IShader* parameter
============
*/
void WINAPI Select_SetTexture2 (IShader* pShader, texdef_t *texdef, brushprimit_texdef_t *brushprimit_texdef, bool bFitScale, void* pPlugTexdef )
{
	brush_t	*b;
	int nCount = g_ptrSelectedFaces.GetSize();
	if (nCount > 0)
	{
		Undo_Start("set face textures");
		ASSERT(g_ptrSelectedFaces.GetSize() == g_ptrSelectedFaceBrushes.GetSize());
		for (int i = 0; i < nCount; i++)
		{
			face_t *selFace = reinterpret_cast<face_t*>(g_ptrSelectedFaces.GetAt(i));
			brush_t *selBrush = reinterpret_cast<brush_t*>(g_ptrSelectedFaceBrushes.GetAt(i));
			Undo_AddBrush(selBrush);
			//++timo TODO: propagate the IShader* ..
			SetFaceTexdef (selBrush, selFace, texdef, brushprimit_texdef, bFitScale, static_cast<IPluginTexdef *>(pPlugTexdef) );
			Brush_Build(selBrush, bFitScale);
			Undo_EndBrush(selBrush);
		}
		Undo_End();
	}
	else if (selected_brushes.next != &selected_brushes)
	{
		Undo_Start("set brush textures");
		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
			if (!b->owner->eclass->fixedsize)
			{
				Undo_AddBrush(b);
				Brush_SetTexture2 (b, pShader, texdef, brushprimit_texdef, bFitScale, static_cast<IPluginTexdef *>(pPlugTexdef) );
				Undo_EndBrush(b);
			}
		Undo_End();
	}
	Sys_UpdateWindows (W_ALL);
}
#endif

/*
============
Select_SetTexture
Timo : bFitScale to compute scale on the plane and counteract plane / axial plane snapping
Timo :	brush primitive texturing
		the brushprimit_texdef given must be understood as a qtexture_t width=2 height=2 ( HiRes )
Timo :  texture plugin, added an IPluginTexdef* parameter
		must be casted to an IPluginTexdef!
		if not NULL, get ->Copy() of it into each face or brush ( and remember to hook )
		if NULL, means we have no information, ask for a default
============
*/
void WINAPI Select_SetTexture (texdef_t *texdef, brushprimit_texdef_t *brushprimit_texdef, bool bFitScale, void* pPlugTexdef )
{
	brush_t	*b;
	int nCount = g_ptrSelectedFaces.GetSize();
	if (nCount > 0)
	{
		Undo_Start("set face textures");
		ASSERT(g_ptrSelectedFaces.GetSize() == g_ptrSelectedFaceBrushes.GetSize());
		for (int i = 0; i < nCount; i++)
		{
			face_t *selFace = reinterpret_cast<face_t*>(g_ptrSelectedFaces.GetAt(i));
			brush_t *selBrush = reinterpret_cast<brush_t*>(g_ptrSelectedFaceBrushes.GetAt(i));
			Undo_AddBrush(selBrush);
			SetFaceTexdef (selBrush, selFace, texdef, brushprimit_texdef, bFitScale, static_cast<IPluginTexdef *>(pPlugTexdef) );
			Brush_Build(selBrush, bFitScale);
			Undo_EndBrush(selBrush);
		}
		Undo_End();
	}
	else if (selected_brushes.next != &selected_brushes)
	{
		Undo_Start("set brush textures");
		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
			if (!b->owner->eclass->fixedsize)
			{
				Undo_AddBrush(b);
				Brush_SetTexture (b, texdef, brushprimit_texdef, bFitScale, static_cast<IPluginTexdef *>(pPlugTexdef) );
				Undo_EndBrush(b);
			}
		Undo_End();
	}
	//++timo FIXME: not necessary in every cases, write a message defering / move one level up
	Sys_UpdateWindows (W_ALL);
}


/*
================================================================

  TRANSFORMATIONS

================================================================
*/

void Select_GetBounds (vec3_t mins, vec3_t maxs)
{
	brush_t	*b;
	int		i;

	for (i=0 ; i<3 ; i++)
	{
		mins[i] = 99999;
		maxs[i] = -99999;
	}

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		for (i=0 ; i<3 ; i++)
		{
			if (b->mins[i] < mins[i])
				mins[i] = b->mins[i];
			if (b->maxs[i] > maxs[i])
				maxs[i] = b->maxs[i];
		}
}


void Select_GetTrueMid (vec3_t mid)
{
	vec3_t	mins, maxs;
	Select_GetBounds (mins, maxs);

  for (int i=0 ; i<3 ; i++)
    mid[i] = (mins[i] + ((maxs[i] - mins[i]) / 2));
}


void Select_GetMid (vec3_t mid)
{
	vec3_t	mins, maxs;
	int		i;

  if (g_PrefsDlg.m_bNoClamp)
  {
    Select_GetTrueMid(mid);
    return;
  }

  Select_GetBounds (mins, maxs);

  for (i=0 ; i<3 ; i++)
		mid[i] = g_qeglobals.d_gridsize*floor ( ( (mins[i] + maxs[i])*0.5 )/g_qeglobals.d_gridsize );

}

vec3_t	select_origin;
vec3_t	select_matrix[3];
qboolean	select_fliporder;

void Select_ApplyMatrix (bool bSnap, bool bRotation, int nAxis, float fDeg)
{
	brush_t	*b;
	face_t	*f;
	int		i, j;
	vec3_t	temp;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			for (i=0 ; i<3 ; i++)
			{
				VectorSubtract (f->planepts[i], select_origin, temp);
				for (j=0 ; j<3 ; j++)
					f->planepts[i][j] = DotProduct(temp, select_matrix[j]) + select_origin[j];
			}
			if (select_fliporder)
			{
				VectorCopy (f->planepts[0], temp);
				VectorCopy (f->planepts[2], f->planepts[0]);
				VectorCopy (temp, f->planepts[2]);
			}
		}

    if(b->owner->eclass->fixedsize)
    {
      if (bRotation && b->owner->md3Class)
      {
        b->owner->vRotation[nAxis] += fDeg;
      }
    }

    Brush_Build(b, bSnap);
		
		if (b->patchBrush)
		{
			//Patch_ApplyMatrix(b->nPatchID, select_origin, select_matrix);
			Patch_ApplyMatrix(b->pPatch, select_origin, select_matrix, bSnap);
		}
	}
}

void ProjectOnPlane(vec3_t& normal,float dist,vec3_t& ez, vec3_t& p)
{
	if (fabs(ez[0]) == 1)
		p[0] = (dist - normal[1] * p[1] - normal[2] * p[2]) / normal[0];
	else if (fabs(ez[1]) == 1)
		p[1] = (dist - normal[0] * p[0] - normal[2] * p[2]) / normal[1];
	else
		p[2] = (dist - normal[0] * p[0] - normal[1] * p[1]) / normal[2];
}

void Back(vec3_t& dir, vec3_t& p)
{
	if (fabs(dir[0]) == 1)
		p[0] = 0;
	else if (fabs(dir[1]) == 1)
		p[1] = 0;
	else p[2] = 0;
}



// using scale[0] and scale[1]
void ComputeScale(vec3_t& rex, vec3_t& rey, vec3_t& p, face_t* f)
{
	float px = DotProduct(rex, p);
	float py = DotProduct(rey, p);
	px *= f->texdef.scale[0];
	py *= f->texdef.scale[1];
  vec3_t aux;
  VectorCopy(rex, aux);
  VectorScale(aux, px, aux);
  VectorCopy(aux, p);
  VectorCopy(rey, aux);
  VectorScale(aux, py, aux);
  VectorAdd(p, aux, p);
}

void ComputeAbsolute(face_t* f, vec3_t& p1, vec3_t& p2, vec3_t& p3)
{
	vec3_t ex,ey,ez;	        // local axis base

#ifdef _DEBUG
	if (g_qeglobals.m_bBrushPrimitMode)
		Sys_Printf("Warning : illegal call of ComputeAbsolute in brush primitive mode\n");
#endif

  // compute first local axis base
  TextureAxisFromPlane(&f->plane, ex, ey);
  CrossProduct(ex, ey, ez);
	    
	vec3_t aux;
  VectorCopy(ex, aux);
  VectorScale(aux, -f->texdef.shift[0], aux);
  VectorCopy(aux, p1);
  VectorCopy(ey, aux);
  VectorScale(aux, -f->texdef.shift[1], aux);
  VectorAdd(p1, aux, p1);
  VectorCopy(p1, p2);
  VectorAdd(p2, ex, p2);
  VectorCopy(p1, p3);
  VectorAdd(p3, ey, p3);
  VectorCopy(ez, aux);
  VectorScale(aux, -f->texdef.rotate, aux);
  VectorRotate(p1, aux, p1);
  VectorRotate(p2, aux, p2);
  VectorRotate(p3, aux, p3);
	// computing rotated local axis base
	vec3_t rex,rey;
  VectorCopy(ex, rex);
  VectorRotate(rex, aux, rex);
  VectorCopy(ey, rey);
  VectorRotate(rey, aux, rey);

  ComputeScale(rex,rey,p1,f);
	ComputeScale(rex,rey,p2,f);
	ComputeScale(rex,rey,p3,f);

	// project on normal plane
	// along ez 
	// assumes plane normal is normalized
	ProjectOnPlane(f->plane.normal,f->plane.dist,ez,p1);
	ProjectOnPlane(f->plane.normal,f->plane.dist,ez,p2);
	ProjectOnPlane(f->plane.normal,f->plane.dist,ez,p3);
};


void AbsoluteToLocal(plane_t normal2, face_t* f, vec3_t& p1, vec3_t& p2, vec3_t& p3)
{
	vec3_t ex,ey,ez;

#ifdef _DEBUG
	if (g_qeglobals.m_bBrushPrimitMode)
		Sys_Printf("Warning : illegal call of AbsoluteToLocal in brush primitive mode\n");
#endif

	// computing new local axis base
  TextureAxisFromPlane(&normal2, ex, ey);
  CrossProduct(ex, ey, ez);

  // projecting back on (ex,ey)
	Back(ez,p1);
	Back(ez,p2);
	Back(ez,p3);

	vec3_t aux;
	// rotation
  VectorCopy(p2, aux);
  VectorSubtract(aux, p1,aux);
	
	float x = DotProduct(aux,ex);
	float y = DotProduct(aux,ey);
  f->texdef.rotate = 180 * atan2(y,x) / Q_PI;

	vec3_t rex,rey;
	// computing rotated local axis base
  VectorCopy(ez, aux);
  VectorScale(aux, f->texdef.rotate, aux);
  VectorCopy(ex, rex);
  VectorRotate(rex, aux, rex);
  VectorCopy(ey, rey);
  VectorRotate(rey, aux, rey);

	// scale
  VectorCopy(p2, aux);
  VectorSubtract(aux, p1, aux);
  f->texdef.scale[0] = DotProduct(aux, rex);
  VectorCopy(p3, aux);
  VectorSubtract(aux, p1, aux);
  f->texdef.scale[1] = DotProduct(aux, rey);

	// shift
	// only using p1
	x = DotProduct(rex,p1);
	y = DotProduct(rey,p1);                 
	x /= f->texdef.scale[0];
	y /= f->texdef.scale[1];

  VectorCopy(rex, p1);
  VectorScale(p1, x, p1);
  VectorCopy(rey, aux);
  VectorScale(aux, y, aux);
  VectorAdd(p1, aux, p1);
  VectorCopy(ez, aux);
  VectorScale(aux, -f->texdef.rotate, aux);
  VectorRotate(p1, aux, p1);
	f->texdef.shift[0] = -DotProduct(p1, ex);
	f->texdef.shift[1] = -DotProduct(p1, ey);

	// stored rot is good considering local axis base
	// change it if necessary
	f->texdef.rotate = -f->texdef.rotate;

  Clamp(f->texdef.shift[0], f->d_texture->width);
  Clamp(f->texdef.shift[1], f->d_texture->height);
  Clamp(f->texdef.rotate, 360);

}

void RotateFaceTexture(face_t* f, int nAxis, float fDeg)
{
	vec3_t p1,p2,p3, rota;   
	p1[0] = p1[1] = p1[2] = 0;
	VectorCopy(p1, p2);
	VectorCopy(p1, p3);
	VectorCopy(p1, rota);
	ComputeAbsolute(f, p1, p2, p3);
  
	rota[nAxis] = fDeg;
	VectorRotate(p1, rota, select_origin, p1);
	VectorRotate(p2, rota, select_origin, p2);
	VectorRotate(p3, rota, select_origin, p3);

	plane_t normal2;
	vec3_t vNormal;
	vNormal[0] = f->plane.normal[0];
	vNormal[1] = f->plane.normal[1];
	vNormal[2] = f->plane.normal[2];
	VectorRotate(vNormal, rota, vNormal);
	normal2.normal[0] = vNormal[0];
	normal2.normal[1] = vNormal[1];
	normal2.normal[2] = vNormal[2];
	AbsoluteToLocal(normal2, f, p1, p2 ,p3);

}

void RotateTextures(int nAxis, float fDeg, vec3_t vOrigin)
{
	for (brush_t* b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (face_t* f=b->brush_faces ; f ; f=f->next)
		{
			if (g_qeglobals.m_bBrushPrimitMode)
				RotateFaceTexture_BrushPrimit( f, nAxis, fDeg, vOrigin );
			else
				RotateFaceTexture(f, nAxis, fDeg);
			//++timo removed that call .. works fine .. ???????
//			Brush_Build(b, false);
		}
		Brush_Build(b, false);
	}
}


void Select_FlipAxis (int axis)
{
	int		i;

	Select_GetMid (select_origin);
	for (i=0 ; i<3 ; i++)
	{
		VectorCopy (vec3_origin, select_matrix[i]);
		select_matrix[i][i] = 1;
	}
	select_matrix[axis][axis] = -1;

	select_fliporder = true;
	Select_ApplyMatrix (true, false, 0, 0);
	Sys_UpdateWindows (W_ALL);
}


void Select_Scale(float x, float y, float z)
{
  Select_GetMid (select_origin);
	for (brush_t* b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (face_t* f=b->brush_faces ; f ; f=f->next)
		{
			for (int i=0 ; i<3 ; i++)
			{
        f->planepts[i][0] -= select_origin[0];
        f->planepts[i][1] -= select_origin[1];
        f->planepts[i][2] -= select_origin[2];
        f->planepts[i][0] *= x;
        //f->planepts[i][0] = floor(f->planepts[i][0] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;

        f->planepts[i][1] *= y;
        //f->planepts[i][1] = floor(f->planepts[i][1] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;

        f->planepts[i][2] *= z;
        //f->planepts[i][2] = floor(f->planepts[i][2] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;
        
        f->planepts[i][0] += select_origin[0];
        f->planepts[i][1] += select_origin[1];
        f->planepts[i][2] += select_origin[2];
			}
		}
		Brush_Build(b, false);
    if (b->patchBrush)
    {
      vec3_t v;
      v[0] = x;
      v[1] = y;
      v[2] = z;
      //Patch_Scale(b->nPatchID, select_origin, v);
      Patch_Scale(b->pPatch, select_origin, v);
    }
	}
}

void Select_RotateAxis (int axis, float deg, bool bPaint, bool bMouse)
{
	vec3_t	temp;
	int		i, j;
	vec_t	c, s;

	if (deg == 0)
  {
    //Sys_Printf("0 deg\n");
		return;
  }

  if (bMouse)
  {
    VectorCopy(g_pParentWnd->ActiveXY()->RotateOrigin(), select_origin);
  }
  else
  {
	  Select_GetMid (select_origin);
  }

	select_fliporder = false;

	if (deg == 90)
	{
		for (i=0 ; i<3 ; i++)
		{
			VectorCopy (vec3_origin, select_matrix[i]);
			select_matrix[i][i] = 1;
		}
		i = (axis+1)%3;
		j = (axis+2)%3;
		VectorCopy (select_matrix[i], temp);
		VectorCopy (select_matrix[j], select_matrix[i]);
		VectorSubtract (vec3_origin, temp, select_matrix[j]);
	}
	else
	{
		deg = -deg;
		if (deg == -180.0)
		{
			c = -1;
			s = 0;
		}
		else if (deg == -270.0)
		{
			c = 0;
			s = -1;
		}
		else
		{
			c = cos(deg * Q_PI / 180.0);
			s = sin(deg * Q_PI / 180.0);
		}

		for (i=0 ; i<3 ; i++)
		{
			VectorCopy (vec3_origin, select_matrix[i]);
			select_matrix[i][i] = 1;
		}

		switch (axis)
		{
		case 0:
			select_matrix[1][1] = c;
			select_matrix[1][2] = -s;
			select_matrix[2][1] = s;
			select_matrix[2][2] = c;
			break;
		case 1:
			select_matrix[0][0] = c;
			select_matrix[0][2] = s;
			select_matrix[2][0] = -s;
			select_matrix[2][2] = c;
			break;
		case 2:
			select_matrix[0][0] = c;
			select_matrix[0][1] = -s;
			select_matrix[1][0] = s;
			select_matrix[1][1] = c;
			break;
		}
	}

	if (g_PrefsDlg.m_bRotateLock)
		RotateTextures(axis, deg, select_origin);
	Select_ApplyMatrix(!bMouse, true, axis, deg);

	if (bPaint)
		Sys_UpdateWindows (W_ALL);
}

/*
================================================================

GROUP SELECTIONS

================================================================
*/

void Select_CompleteTall (void)
{
	brush_t	*b, *next;
	//int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);
	Select_Delete ();

  int nDim1 = (g_pParentWnd->ActiveXY()->GetViewType() == YZ) ? 1 : 0;
  int nDim2 = (g_pParentWnd->ActiveXY()->GetViewType() == XY) ? 1 : 2;

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;

    if ( (b->maxs[nDim1] > maxs[nDim1] || b->mins[nDim1] < mins[nDim1]) 
      || (b->maxs[nDim2] > maxs[nDim2] || b->mins[nDim2] < mins[nDim2]) )
      continue;

	 	if (FilterBrush (b))
	 		continue;

		Brush_RemoveFromList (b);
		Brush_AddToList (b, &selected_brushes);
#if 0
    // old stuff
    for (i=0 ; i<2 ; i++)
			if (b->maxs[i] > maxs[i] || b->mins[i] < mins[i])
				break;
		if (i == 2)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
#endif
	}
	Sys_UpdateWindows (W_ALL);
}

void Select_PartialTall (void)
{
	brush_t	*b, *next;
	//int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);
	Select_Delete ();

  int nDim1 = (g_pParentWnd->ActiveXY()->GetViewType() == YZ) ? 1 : 0;
  int nDim2 = (g_pParentWnd->ActiveXY()->GetViewType() == XY) ? 1 : 2;

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;

    if ( (b->mins[nDim1] > maxs[nDim1] || b->maxs[nDim1] < mins[nDim1]) 
      || (b->mins[nDim2] > maxs[nDim2] || b->maxs[nDim2] < mins[nDim2]) )
      continue;

	 	if (FilterBrush (b))
	 		continue;

  	Brush_RemoveFromList (b);
		Brush_AddToList (b, &selected_brushes);


#if 0
// old stuff
		for (i=0 ; i<2 ; i++)
			if (b->mins[i] > maxs[i] || b->maxs[i] < mins[i])
				break;
		if (i == 2)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
#endif
	}
	Sys_UpdateWindows (W_ALL);
}

void Select_Touching (void)
{
	brush_t	*b, *next;
	int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;

	 	if (FilterBrush (b))
	 		continue;

		for (i=0 ; i<3 ; i++)
			if (b->mins[i] > maxs[i]+1 || b->maxs[i] < mins[i]-1)
				break;

		if (i == 3)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
	}
	Sys_UpdateWindows (W_ALL);
}

void Select_Inside (void)
{
	brush_t	*b, *next;
	int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);
	Select_Delete ();

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;

	 	if (FilterBrush (b))
	 		continue;

		for (i=0 ; i<3 ; i++)
			if (b->maxs[i] > maxs[i] || b->mins[i] < mins[i])
				break;
		if (i == 3)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
	}
	Sys_UpdateWindows (W_ALL);
}

/*
=============
Select_PushEntity

Move world brushes into an entity
=============
*/
void Select_PushEntity(void)
{
	entity_t *se=0; // Selected entity to push brushes to
	brush_t *sb;

	// Walk selected brush list to find first non worldspawn entity
	for (sb = selected_brushes.next; sb != &selected_brushes; sb = sb->next)
	{
		// Find a non wolrdspawn entity
		if(sb->owner != world_entity)
		{
			se = sb->owner;
			break;
		}
	}
	
	if(se==0) return; // return on entity not found

	if(se->eclass->fixedsize != 0)  // Check that se is not a point entity
	{
		MessageBox( g_qeglobals.d_hwndMain, "I'm not going to move brushes into a point entity you silly human.", "No! No! No!", MB_OK );
		return;
	}

	unsigned int count = 0;
	char *type=0;
	type = ValueForKey (se, "classname");
	if (type == 0) return; // return if the entity doesn't have a classname

	for (sb = selected_brushes.next; sb != &selected_brushes; sb = sb->next)
	{
		if(sb->owner == world_entity)
		{
			// Move world brushes to entity
			Entity_UnlinkBrush (sb);
			Entity_LinkBrush (se, sb);
			Brush_Build( sb );
			sb->owner = se;
			count++;
		}
	}
	
	Sys_Printf( count == 1 ? "Moved %u world brush into %s entity.\n" : "Moved %u world brushes into %s entity.\n",count, type);
}

/*
=============
Select_Ungroup

Turn the currently selected entity back into normal brushes
=============
*/
void Select_Ungroup(void)
{
	int numselectedgroups;
	entity_t	*e;
	brush_t		*b, *sb;

	numselectedgroups = 0;
	for (sb = selected_brushes.next; sb != &selected_brushes; sb = sb->next)
	{
		e = sb->owner;

		if (!e || e == world_entity || e->eclass->fixedsize)
		{
			continue;
		}

		for (b = e->brushes.onext; b != &e->brushes; b = e->brushes.onext)
		{
			//Brush_RemoveFromList (b);
			//Brush_AddToList (b, &active_brushes);
			Entity_UnlinkBrush (b);
			Entity_LinkBrush (world_entity, b);
			Brush_Build( b );
			b->owner = world_entity;
		}
		Entity_Free (e);
		numselectedgroups++;
	}

	if (numselectedgroups <= 0)
	{
		Sys_Printf("No grouped entities selected.\n");
		return;
	}
	Sys_Printf("Ungrouped %d entit%s.\n", numselectedgroups, (numselectedgroups == 1)?"y":"ies");
	Sys_UpdateWindows (W_ALL);
}


/*
====================
Select_MakeStructural
====================
*/
void Select_MakeStructural (void)
{
	brush_t	*b;
	face_t	*f;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		for (f=b->brush_faces ; f ; f=f->next)
			f->texdef.contents &= ~CONTENTS_DETAIL;
	Select_Deselect ();
	Sys_UpdateWindows (W_ALL);
}

void Select_MakeDetail (void)
{
	brush_t	*b;
	face_t	*f;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		for (f=b->brush_faces ; f ; f=f->next)
			f->texdef.contents |= CONTENTS_DETAIL;
	Select_Deselect ();
	Sys_UpdateWindows (W_ALL);
}

void Select_ShiftTexture(int x, int y)
{
	brush_t		*b;
	face_t		*f;

  int nFaceCount = g_ptrSelectedFaces.GetSize();

	if(selected_brushes.next == &selected_brushes && nFaceCount == 0)
		return;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			if (g_qeglobals.m_bBrushPrimitMode)
			{
				// use face normal to compute a true translation
				Select_ShiftTexture_BrushPrimit( f, x, y );
			}
			else
			{
				f->texdef.shift[0] += x;
				f->texdef.shift[1] += y;
			}
		}
		Brush_Build(b);
		if (b->patchBrush)
		{
			//Patch_ShiftTexture(b->nPatchID, x, y);
			Patch_ShiftTexture(b->pPatch, x, y);
		}
	}

	if (nFaceCount > 0)
	{
    for (int i = 0; i < nFaceCount; i++)
    {
      face_t *selFace = reinterpret_cast<face_t*>(g_ptrSelectedFaces.GetAt(i));
      brush_t *selBrush = reinterpret_cast<brush_t*>(g_ptrSelectedFaceBrushes.GetAt(i));
  		if (g_qeglobals.m_bBrushPrimitMode)
	  	{

			  // use face normal to compute a true translation
        // Select_ShiftTexture_BrushPrimit( selected_face, x, y );
			  // use camera view to compute texture shift
			  g_pParentWnd->GetCamera()->ShiftTexture_BrushPrimit( selFace, x, y );
      }
  		else
	  	{
		  	selFace->texdef.shift[0] += x;
			  selFace->texdef.shift[1] += y;
  		}
	  	Brush_Build(selBrush);
    }
	}

	Sys_UpdateWindows (W_CAMERA);
}

void Select_ScaleTexture(int x, int y)
{
	brush_t		*b;
	face_t		*f;

  int nFaceCount = g_ptrSelectedFaces.GetSize();

  if(selected_brushes.next == &selected_brushes && nFaceCount == 0)
	{
		return;
	}

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			if (g_qeglobals.m_bBrushPrimitMode)
			{
				// apply same scale as the spinner button of the surface inspector
				float	shift[2];
				float	rotate;
				float	scale[2];
				brushprimit_texdef_t bp; 
				// compute normalized texture matrix
				ConvertTexMatWithQTexture( &f->brushprimit_texdef, f->d_texture, &bp, NULL );
				// compute fake shift scale rot
				TexMatToFakeTexCoords( bp.coords, shift, &rotate, scale );
				// update
				scale[0]+=static_cast<float>(x)*0.1;
				scale[1]+=static_cast<float>(y)*0.1;
				// compute new normalized texture matrix
				FakeTexCoordsToTexMat( shift, rotate, scale, bp.coords );
				// apply to face texture matrix
				ConvertTexMatWithQTexture( &bp, NULL, &f->brushprimit_texdef, f->d_texture );
			}
			else
			{
				f->texdef.scale[0] += x;
				f->texdef.scale[1] += y;
			}
		}
		Brush_Build(b);
		if (b->patchBrush)
		{
			Patch_ScaleTexture(b->pPatch, x, y);
		}
	}

	if (nFaceCount > 0)
	{
    for (int i = 0; i < nFaceCount; i++)
    {
      face_t *selFace = reinterpret_cast<face_t*>(g_ptrSelectedFaces.GetAt(i));
      brush_t *selBrush = reinterpret_cast<brush_t*>(g_ptrSelectedFaceBrushes.GetAt(i));
		  if (g_qeglobals.m_bBrushPrimitMode)
		  {
			  float	shift[2];
			  float	rotate;
			  float	scale[2];
			  brushprimit_texdef_t bp; 
			  ConvertTexMatWithQTexture( &selFace->brushprimit_texdef, selFace->d_texture, &bp, NULL );
			  TexMatToFakeTexCoords( bp.coords, shift, &rotate, scale );
			  scale[0]+=static_cast<float>(x)*0.1;
			  scale[1]+=static_cast<float>(y)*0.1;
			  FakeTexCoordsToTexMat( shift, rotate, scale, bp.coords );
			  ConvertTexMatWithQTexture( &bp, NULL, &selFace->brushprimit_texdef, selFace->d_texture );
		  }
		  else
		  {
			  selFace->texdef.scale[0] += x;
			  selFace->texdef.scale[1] += y;
		  }
		  Brush_Build(selBrush);
    }
	}

	Sys_UpdateWindows (W_CAMERA);
}

void Select_RotateTexture(int amt)
{
	brush_t		*b;
	face_t		*f;

  int nFaceCount = g_ptrSelectedFaces.GetSize();

  if(selected_brushes.next == &selected_brushes && nFaceCount == 0)
	{
		return;
	}

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			if (g_qeglobals.m_bBrushPrimitMode)
			{
				// apply same scale as the spinner button of the surface inspector
				float	shift[2];
				float	rotate;
				float	scale[2];
				brushprimit_texdef_t bp; 
				// compute normalized texture matrix
				ConvertTexMatWithQTexture( &f->brushprimit_texdef, f->d_texture, &bp, NULL );
				// compute fake shift scale rot
				TexMatToFakeTexCoords( bp.coords, shift, &rotate, scale );
				// update
				rotate += amt;
				// compute new normalized texture matrix
				FakeTexCoordsToTexMat( shift, rotate, scale, bp.coords );
				// apply to face texture matrix
				ConvertTexMatWithQTexture( &bp, NULL, &f->brushprimit_texdef, f->d_texture );
			}
			else
			{
				f->texdef.rotate += amt;
				f->texdef.rotate = static_cast<int>(f->texdef.rotate) % 360;
			}
		}
		Brush_Build(b);
		if (b->patchBrush)
		{
			//Patch_RotateTexture(b->nPatchID, amt);
			Patch_RotateTexture(b->pPatch, amt);
		}
	}
	
	if (nFaceCount > 0)
	{
    for (int i = 0; i < nFaceCount; i++)
    {
      face_t *selFace = reinterpret_cast<face_t*>(g_ptrSelectedFaces.GetAt(i));
      brush_t *selBrush = reinterpret_cast<brush_t*>(g_ptrSelectedFaceBrushes.GetAt(i));
		  if (g_qeglobals.m_bBrushPrimitMode)
		  {
			  float	shift[2];
			  float	rotate;
			  float	scale[2];
			  brushprimit_texdef_t bp; 
			  ConvertTexMatWithQTexture( &selFace->brushprimit_texdef, selFace->d_texture, &bp, NULL );
			  TexMatToFakeTexCoords( bp.coords, shift, &rotate, scale );
			  rotate += amt;
			  FakeTexCoordsToTexMat( shift, rotate, scale, bp.coords );
			  ConvertTexMatWithQTexture( &bp, NULL, &selFace->brushprimit_texdef, selFace->d_texture );
		  }
		  else
		  {
			  selFace->texdef.rotate += amt;
			  selFace->texdef.rotate = static_cast<int>(selFace->texdef.rotate) % 360;
		  }
		  Brush_Build(selBrush);
    }
	}

	Sys_UpdateWindows (W_CAMERA);
}

// TTimo modified to handle shader architecture:
// expects shader names at input, comparison relies on shader names .. texture names no longer relevant
void FindReplaceTextures(const char* pFind, const char* pReplace, bool bSelected, bool bForce)
{
	brush_t* pList = (bSelected) ? &selected_brushes : &active_brushes;
	if (!bSelected)
		Select_Deselect();
	
  //++timo BP mode: replacing a texture in BP mode is not that easy, you need to recompute the texture matrix
  // if the size of the replacing texture differs, otherwise you get wrong scaling
  if (g_qeglobals.m_bBrushPrimitMode)
    Sys_Printf("TODO: finalize find/replace code for brush primitives");

	for (brush_t* pBrush = pList->next ; pBrush != pList; pBrush = pBrush->next)
	{
		if (pBrush->patchBrush)
		{
			Patch_FindReplaceTexture(pBrush, pFind, pReplace, bForce);
		}
		
		for (face_t* pFace = pBrush->brush_faces; pFace; pFace = pFace->next)
		{
      if(bForce || strcmpi(pFace->pShader->getName(), pFind) == 0)
			{
			  pFace->pShader->DecRef();
				pFace->pShader = QERApp_Shader_ForName( pReplace );
				pFace->pShader->IncRef();
				pFace->d_texture = pFace->pShader->getTexture();
				pFace->texdef.SetName(pReplace);
			}
		}
		Brush_Build(pBrush);
	}
	Sys_UpdateWindows (W_CAMERA);
}


void Select_AllOfType()
{
	brush_t	*b, *next;
	entity_t	*e;
	//++timo what does the test selected_brushes.next->next != &selected_brushes stands for??
	// FIXME: do we want to select of type only on selected brushes?
	if ( (selected_brushes.next == &selected_brushes)
		|| (selected_brushes.next->next != &selected_brushes) )
	{

    CString strName;
    if (g_ptrSelectedFaces.GetSize() == 0)
    {
      strName = g_qeglobals.d_texturewin.texdef.name;
    }
    else
    {
      face_t *selFace = reinterpret_cast<face_t*>(g_ptrSelectedFaces.GetAt(0));
      strName = selFace->texdef.name;
    }

    Select_Deselect();
	  for (b=active_brushes.next ; b != &active_brushes ; b=next)
    {
		  next = b->next;
    	
      if (FilterBrush (b))
	 	    continue;

      if (b->patchBrush)
      {
        if (strcmpi(strName, b->pPatch->pShader->getName()) == 0)
        {
			    Brush_RemoveFromList (b);
			    Brush_AddToList (b, &selected_brushes);
        }
      }
      else
      {
        for (face_t* pFace = b->brush_faces; pFace; pFace = pFace->next)
        {
          if (strcmpi(strName, pFace->texdef.name) == 0)
          {
			      Brush_RemoveFromList (b);
			      Brush_AddToList (b, &selected_brushes);
          }
        }
      }
    }
    Sys_UpdateWindows(W_ALL);
    return;
  }

  
  b = selected_brushes.next;
	e = b->owner;

  if (e != NULL)
  {
    if (e != world_entity)
    {
      CString strName = e->eclass->name;
      CString strKey, strVal;
      bool bCriteria = GetSelectAllCriteria(strKey, strVal);
      Sys_Printf("Selecting all %s(s)\n", strName);
      Select_Deselect();

	    for (b=active_brushes.next ; b != &active_brushes ; b=next)
    	{
		    next = b->next;
    	 	
        if (FilterBrush (b))
	 	    	continue;

        e = b->owner;
        if (e != NULL)
        {
          if (strcmpi(e->eclass->name, strName) == 0)
          {
            bool doIt = true;
            if (bCriteria) {
              CString str = ValueForKey (e, strKey);
              if (str.CompareNoCase(strVal) != 0) {
                doIt = false;
              }
            }
            if (doIt) {
  		        Brush_RemoveFromList (b);
	  	        Brush_AddToList (b, &selected_brushes);
            }
          }
        }
      }
    }
  }
	Sys_UpdateWindows (W_ALL);

}

void Select_Reselect()
{
  CPtrArray holdArray;
  brush_t* b;
	for (b=selected_brushes.next ; b && b != &selected_brushes ; b=b->next)
	{
    holdArray.Add(reinterpret_cast<void*>(b));
	}

  int n = holdArray.GetSize();
  while (n-- > 0)
  {
    b = reinterpret_cast<brush_t*>(holdArray.GetAt(n));
    Select_Brush(b);
  }
	Sys_UpdateWindows (W_ALL);
}


void Select_FitTexture(int nHeight, int nWidth)
{
	brush_t		*b;

  int nFaceCount = g_ptrSelectedFaces.GetSize();

  if(selected_brushes.next == &selected_brushes && nFaceCount == 0)
		return;

  for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
    Brush_FitTexture(b, nHeight, nWidth);
		Brush_Build(b);
	}

	if (nFaceCount > 0)
	{
    for (int i = 0; i < nFaceCount; i++)
    {
      face_t *selFace = reinterpret_cast<face_t*>(g_ptrSelectedFaces.GetAt(i));
      brush_t *selBrush = reinterpret_cast<brush_t*>(g_ptrSelectedFaceBrushes.GetAt(i));
      Face_FitTexture(selFace, nHeight, nWidth);
	  	Brush_Build(selBrush);
    }
	}

	Sys_UpdateWindows (W_CAMERA);
}

void Select_AxialTexture()
{

}

void Select_Hide()
{
	for (brush_t* b=selected_brushes.next ; b && b != &selected_brushes ; b=b->next)
	{
    b->hiddenBrush = true;
	}
	Sys_UpdateWindows (W_ALL);
}

void Select_ShowAllHidden()
{
  brush_t* b;
	for (b=selected_brushes.next ; b && b != &selected_brushes ; b=b->next)
	{
    b->hiddenBrush = false;
	}
	for (b=active_brushes.next ; b && b != &active_brushes ; b=b->next)
	{
    b->hiddenBrush = false;
	}
	Sys_UpdateWindows (W_ALL);
}

/*
============
Select_Invert
============
*/
void Select_Invert(void)
{
	brush_t *next, *prev;

	Sys_Printf("inverting selection...\n");

	next = active_brushes.next;
	prev = active_brushes.prev;
	if (selected_brushes.next != &selected_brushes)
	{
		active_brushes.next = selected_brushes.next;
		active_brushes.prev = selected_brushes.prev;
		active_brushes.next->prev = &active_brushes;
		active_brushes.prev->next = &active_brushes;
	}
	else
	{
		active_brushes.next = &active_brushes;
		active_brushes.prev = &active_brushes;
	}
	if (next != &active_brushes)
	{
		selected_brushes.next = next;
		selected_brushes.prev = prev;
		selected_brushes.next->prev = &selected_brushes;
		selected_brushes.prev->next = &selected_brushes;
	}
	else
	{
		selected_brushes.next = &selected_brushes;
		selected_brushes.prev = &selected_brushes;
	}

	Sys_UpdateWindows(W_ALL);

	Sys_Printf("done.\n");
}


/* 
===========
Select_Name
===========
*/
void Select_Name(const char *pName)
{
	if (g_qeglobals.m_bBrushPrimitMode)
  {
	  for (brush_t* b=selected_brushes.next ; b && b != &selected_brushes ; b=b->next)
	  {
      Brush_SetEpair(b, "Name", pName);
	  }
  }
}

/* 
=================
Select_AddToGroup
add selected brushes to a group, update the tree
=================
*/
void Select_AddToGroup(const char *pName)
{
	if (g_qeglobals.m_bBrushPrimitMode)
  {
	  for (brush_t* b=selected_brushes.next ; b && b != &selected_brushes ; b=b->next)
	  {
      Brush_SetEpair(b, "group", pName);
			Group_AddToProperGroup(b);
	  }
  }
}
