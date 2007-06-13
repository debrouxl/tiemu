#ifndef LAYOUT_H
#define LAYOUT_H 1

#ifdef __cplusplus
extern "C" {
#endif

/*
Unlike the original code, we assume that the set of nodes
known to this layout graph is always kept 1-1 with the set
of dual nodes.  Similarly with respect to edges.
This implies that every time a dual is created/deleted,
that the corresponding create/delete methods must be
called in the layout graph class.
Additionally, we assume the existence of two ``update''
methods that propagate the correct location and size info
between the layout graph nodes and their duals.
*/

struct Layout_Graph; /* hidden */

/* ptr to user's node info */
typedef void* pItem;

/*
As inputs, we need the following:

For nodes: bbox that just surrounds the node.
For edges: width and height of the text (if any)
	   associated with the edge.

As outputs, we provide the following:

For nodes: new absolute position of the northwest corner
	   of the bbox for the node.
For edges: (x,y) coordinates of the endpoints of the edge.

To avoid proliferation of types,
we define a single struct (ItemGeom)
that is used to carry all the input and output info.

Item type	Direction	ItemGeom Fields Used
---------	---------	--------------------
Node		In		x1,y1,x2,y2
		Out		x1,y1
Edge		In		width,height
		Out		x1,y1,x2,y2

*/

/* All values are in pixels */
struct ItemGeom {
	double	x1,y1;
	double	x2,y2;
	double	width,height;
};
typedef struct ItemGeom ItemGeom;

struct LayoutConfig {
	pItem			rootnode;
	int			graphorder;
	int			nodespaceH;
	int			nodespaceV;
	int			xoffset;
	int			yoffset;
	int			computenodesize;
	int			elementsperline;
	int			hideedges;
	int			keeprandompositions;
	int			maxx;
	int			maxy;
	int			gridlock;
};
typedef struct LayoutConfig LayoutConfig;

extern LayoutConfig GetLayoutConfig _ANSI_ARGS_((struct Layout_Graph*));
extern void SetLayoutConfig _ANSI_ARGS_((struct Layout_Graph*, LayoutConfig));

extern	int LayoutISI _ANSI_ARGS_((struct Layout_Graph*));
extern	int LayoutTree _ANSI_ARGS_((struct Layout_Graph*));
extern	int LayoutMatrix _ANSI_ARGS_((struct Layout_Graph*));
extern	int LayoutRandom _ANSI_ARGS_((struct Layout_Graph*));

#if DEBUGGING
extern	void LayoutDebugging _ANSI_ARGS_((struct Layout_Graph*, struct Node *currentnode, char *string, int type));
#endif

extern struct Layout_Graph* LayoutCreateGraph _ANSI_ARGS_(());
extern void LayoutFreeGraph _ANSI_ARGS_((struct Layout_Graph*));
extern void LayoutClearGraph _ANSI_ARGS_((struct Layout_Graph*));

extern int LayoutCreateNode _ANSI_ARGS_((struct Layout_Graph*,
					  pItem nodeid,
					  pItem from, pItem to));
extern int LayoutDeleteNode _ANSI_ARGS_((struct Layout_Graph*, pItem nodeid));
extern int LayoutCreateEdge _ANSI_ARGS_((struct Layout_Graph*,
					 pItem edgeid,
					 pItem from, pItem to));
extern int LayoutDeleteEdge _ANSI_ARGS_((struct Layout_Graph*, pItem edgeid));

extern int LayoutGetIthNode _ANSI_ARGS_((struct Layout_Graph*, long, pItem*));

extern int LayoutGetIthEdge _ANSI_ARGS_((struct Layout_Graph*, long,  pItem*));

extern int LayoutGetNodeBBox _ANSI_ARGS_((struct Layout_Graph*, pItem, ItemGeom*));
extern int LayoutSetNodeBBox _ANSI_ARGS_((struct Layout_Graph*, pItem, ItemGeom));

extern int LayoutGetEdgeEndPoints _ANSI_ARGS_((struct Layout_Graph*, pItem, ItemGeom*));
extern int LayoutSetEdgeDim _ANSI_ARGS_((struct Layout_Graph*, pItem, ItemGeom));

extern char* LayoutGetError _ANSI_ARGS_((struct Layout_Graph*));

#ifdef __cplusplus
}
#endif

#endif /*LAYOUT_H*/
