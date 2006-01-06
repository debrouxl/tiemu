/* 
 * Copyright (c) 1993 by Sven Delmas
 * All rights reserved.
 * See the file COPYRIGHT for the copyright notes.
 *
 */

/* renamed from layout.c to tkCanvLayout.c by Will Taylor 09nov95 */
/* converted to Tk4.1 by Will Taylor 05may96 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <tcl.h>
#include "tkCanvLayout.h"

#if defined (__MSVC__) && ! defined (HAVE_RAND)
#define HAVE_RAND
#endif

/*
 * Functions to compute random numbers.  These don't have to be
 * particular good random number generators.
 */
#ifdef HAVE_RANDOM
#define RANDOM random ()
#define SRANDOM srandom
#else /* HAVE_RANDOM */
#ifdef HAVE_DRAND48
#define MY_RAND_MAX 65536
#define RANDOM ((long) (drand48 () * MY_RAND_MAX))
#define SRANDOM srand48
#else /* HAVE_DRAND48 */
#ifdef HAVE_RAND
#define RANDOM rand ()
#define SRANDOM srand
#else
#warning no random number generator specified, default: random, srandom
#define HAVE_RANDOM
#define RANDOM random ()
#define SRANDOM srandom
#endif /* HAVE_RAND */
#endif /* HAVE_DRAND48 */
#endif /* HAVE_RANDOM */

#define LAYOUT_OK TCL_OK
#define LAYOUT_ERROR TCL_ERROR

/*
 * This value is added to the line length in pixels, so the distance
 * between two nodes can be calculated correctly.
 */
#define LINE_INCREMENT 10

/*
 * Turn on/off debugging.
 */
#define DEBUGGING 1
#define DEBUG_LAYOUT 0
#define DEBUG_PLACE 0
#define TESTING 0

/*
 * the datastructures used for layouting.
 */

/*
 * these datas/variables are used by the tree layouter.
 */

struct TreeData {
  double tmpX;                        /* A temporary x position. */
  double tmpY;                        /* A temporary y position. */
};
typedef struct TreeData TreeData;

#define TREE_TMP_X_POS(node)           (node)->treeData.tmpX
#define SET_TREE_TMP_X_POS(node,pos)   (node)->treeData.tmpX = (pos)
#define TREE_TMP_Y_POS(node)           (node)->treeData.tmpY
#define SET_TREE_TMP_Y_POS(node,pos)   (node)->treeData.tmpY = (pos)

#if DEBUGGING
#define DEBUG_PRINT_TREE_NODE_POS(node, s) TkCanvLayoutDebugging(node, s, 1)
#else
#define DEBUG_PRINT_TREE_NODE_POS(node, s)
#endif


/*
 * A topologically ordered node. stored in the global array toplist
 * (0 to toplistnum).
 */
struct Topology {
  struct Nodes *nodePtr;
};
typedef struct Topology Topology;

/*
Define edge information
*/

struct Edge {
	pItem edgeid;		/* edge identifier */
	ItemGeom info;
	struct Nodes* fromNode;	/* A pointer to the ``from'' node struct. */
	struct Nodes* toNode;	/* A pointer to the ``to'' node struct. */
	int ignore;		/* Ignore this edge. */
	int visited;	/* This edge was visited. */
};
typedef struct Edge Edge;

/*
 * A graph node. stored in a global array named nodes
 * (0 to nodenum).
 */
struct Nodes {
  pItem itemPtr;	/* Pointer to the Node dual. */
  ItemGeom info;	/* bounding box of the dual */
  int ignore;		/* Ignore this node. */
  int visited;		/* This node was already */
			/* visited/layouted. */
  double x;		/* The calculated x position. */
  double y;		/* The calculated y position. */
  int parentNum;	/* The number of parent nodes. */
  Edge** parent;	/* The array of parent nodes. */
  int succNum;		/* The number of successor nodes. */
  Edge** succ;		/* The array of successor nodes. */
  struct TreeData treeData; /* temporary tree layout nodes */
#if 0
  char *data;		/* Special data attached to */
			/* this node. The contents */
			/* depend from the layouting */
			/* algorithms. */
#endif
};
typedef struct Nodes Nodes;
typedef struct Nodes Node;

/*
 * Defines that hide internal functionality.
 */

/*
 * Get and set the edge ignore flag. Edges which are ignored will not
 * be traversed. The first parameter is the edge, and the second
 * parameter (if you call the setting) is the new value of the
 * ignore flag.
 */
#define IGNORE_EDGE(edge)             (edge)->ignore
#define SET_IGNORE_EDGE(edge,mode)    (edge)->ignore=mode

/*
 * Get and set the edge visited flag. This flag is usually set to true
 * when the edge was visited. The first parameter is the edge, and the
 * second parameter (if you call the setting) is the new value of the
 * visited flag.
 */
#define VISITED_EDGE(edge)            (edge)->visited
#define SET_VISITED_EDGE(edge,mode)   (edge)->visited=mode

/*
 * Get and set the node ignore flag. Nodes which are ignored will not
 * be traversed, and they are not placed/layouted. The first parameter
 * is the node, and the second parameter (if you call the setting) is
 * the new value of the ignore flag.
 */
#define IGNORE_NODE(node)             (node)->ignore
#define SET_IGNORE_NODE(node,mode)    (node)->ignore=mode
#define RESET_IGNORE_NODE(i)          for (i = 0; i < THIS(nodeNum); i++) nodes[i]->ignore=0;

/*
 * Get and set the node visited flag. This flag is usually set to true
 * when the node was visited. Currently this flag is mainly used for the
 * topological sorting. The first parameter is the node, and the
 * second parameter (if you call the setting) is the new value of the
 * visited flag.
 */
#define VISITED_NODE(node)            (node)->visited
#define SET_VISITED_NODE(node,mode)   (node)->visited=mode
#define RESET_VISITED_NODE(i)         for (i = 0; i < THIS(nodeNum); i++) THIS(nodes)[i]->visited=0;

/*
 * Get and set the number of parent nodes. The first parameter is the
 * node, and the second parameter (if you call the setting) is the new
 * number of parents.
 */
#define PARENT_NUM(node)              (node)->parentNum
#define SET_PARENT_NUM(node,num)      (node)->parentNum=num

/*
 * Get and set the number of successor nodes. The first parameter is
 * the node, and the second parameter (if you call the setting) is the
 * new number of successors.
 */
#define SUCC_NUM(node)                (node)->succNum
#define SET_SUCC_NUM(node,num)        (node)->succNum=num

/*
 * Get and set the node item. This item is the corresponding dual
 * item for this node. The first parameter is the node, and the second
 * parameter (if you call the setting) is the new dual item pointer.
 */
#define NODE_ITEM(node)               (node)->itemPtr
#define SET_NODE_ITEM(node,item)      (node)->itemPtr=item

/* Get and set the node x1,x2,y1, and y2 positions. */
#define NODE_X1_POS(node)              (node)->info.x1
#define NODE_Y1_POS(node)              (node)->info.y1
#define NODE_X2_POS(node)              (node)->info.x2
#define NODE_Y2_POS(node)              (node)->info.y2

#define SET_NODE_X1_POS(node,v)              (node)->info.x1 = (v)
#define SET_NODE_Y1_POS(node,v)              (node)->info.y1 = (v)
#define SET_NODE_X2_POS(node,v)              (node)->info.x2 = (v)
#define SET_NODE_Y2_POS(node,v)              (node)->info.y2 = (v)

/* Get, by calculation, the node's width and height */
#define CALC_NODE_HEIGHT(n)         (NODE_Y2_POS(n) - NODE_Y1_POS(n))
#define CALC_NODE_WIDTH(n)         (NODE_X2_POS(n) - NODE_X1_POS(n))

/* Get/Set the node dual geom */
#define NODE_GEOM(node)      	     (node)->info
#define SET_NODE_GEOM(node,inf)      (node)->info = (inf)

/*
 * Get and set the node x position. This is the value for the final
 * placing of the node. The first parameter is the node, and the
 * second parameter (if you call the setting) is the new x position.
 */
#define NODE_X_POS(node)              (node)->x
#define SET_NODE_X_POS(node,pos)      (node)->x=pos

/*
 * Get and set the node y position. This is the value for the final
 * placing of the node. The first parameter is the node, and the
 * second parameter (if you call the setting) is the new y position.
 */
#define NODE_Y_POS(node)              (node)->y
#define SET_NODE_Y_POS(node,pos)      (node)->y=pos

/*
 * Get and set the nodes width. The parameter specifies the node
 * whose width and height will be returned.
 */
#define NODE_WIDTH(node)              (node)->info.width
#define SET_NODE_WIDTH(node,size)     (node)->info.width=size

/*
 * Get and set the nodes height. The parameter specifies the node
 * whose width and height will be returned.
 */
#define NODE_HEIGHT(node)             (node)->info.height
#define SET_NODE_HEIGHT(node,size)    (node)->info.height=size

#define EDGE_ITEM(edge)               (edge)->edgeid
#define SET_EDGE_ITEM(edge,item)      (edge)->edgeid=item

/*
 * Return the parent/successor edge/node for the specified node. The
 * second parameter is a integer counter that is used as index for the
 * parent/successor array. Usually this index is generated by a
 * FOR_ALL_* macro.
 */
#define PARENT_EDGE(node, i)          (node)->parent[i]
#define PARENT_NODE(node, i)          (node)->parent[i]->fromNode
#define PARENT_ID(node, i)            (node)->parent[i]->edgeid
#define SUCC_EDGE(node, i)            (node)->succ[i]
#define SUCC_NODE(node, i)            (node)->succ[i]->toNode
#define SUCC_ID(node, i)              (node)->succ[i]->edgeid
#define DUMMY_NODE(node)              ((node)->itemPtr == (pItem) NULL)

/* Get and set the edge x1,x2,y1, and y2 positions. */
#define EDGE_X1_POS(edge)              (edge)->info.x1
#define EDGE_X2_POS(edge)              (edge)->info.x2
#define EDGE_Y1_POS(edge)              (edge)->info.y1
#define EDGE_Y2_POS(edge)              (edge)->info.y2

/* Get/Set the edge dual geom */
#define EDGE_GEOM(edge)	             (edge)->info
#define SET_EDGE_GEOM(edge,inf)      (edge)->info = inf

/*
 * Return the node specified by the integer counter passed to this
 * macro. The nodes in the array are topologically ordered (beginning
 * with the index 0. The index is usually created by the macro
 * FOR_ALL_TOP_NODES.
 */
#define TOP_NODE(i)                   THIS(topList)[i]->nodePtr

/*
 * Walk through all nodes that are currently defined. The only
 * parameter is the integer counter that is used to index the array.
 */
#define FOR_ALL_NODES(i)              for (i = 0; i < THIS(nodeNum); i++)

/*
 * Walk through all edges that are currently defined. The only
 * parameter is the integer counter that is used to index the array.
 */
#define FOR_ALL_EDGES(i)              for (i = 0; i < THIS(edgeNum); i++)

/*
 * Walk through all parents/successors of the specified node. The
 * second parameter is the integer counter that is used to index the
 * array.
 */
#define FOR_ALL_PARENTS(node, i)      for (i = 0; i < (node)->parentNum; i++)
#define FOR_ALL_SUCCS(node, i)        for (i = 0; i < (node)->succNum; i++)

/*
 * Walk through all nodes in topological order. The only parameter is
 * the integer counter that is used to index the array. This call
 * requires a call of the topological order function, before it can be
 * used.
 */
#define FOR_ALL_TOP_NODES(i)          for (i = 0; i < THIS(topListNum); i++)

/*
 * DEBUGGING MACROS.
 */
#if DEBUGGING
#define DEBUG_PRINT_NODE_POS(GRAPH, NODE, S) LayoutDebugging(GRAPH, NODE, S, 0)
#define DEBUG_PRINT_STRING(S1, S2)    fprintf(stderr, "%s %s<\n", S2, S1);
#else
#define DEBUG_PRINT_NODE_POS(GRAPH, NODE, S)
#define DEBUG_PRINT_STRING(S1, S2)
#endif


#define THIS(x) This->x

struct Layout_Graph {
	/* Start with user settable configuration items */
	long iconSpaceV;		/* The vertical space between icons. */
	long iconSpaceH;		/*The horizontal space between icons.*/
	int graphOrder;			/* Ordering... 0 = LR, 1 = TD. */
	Node *rootNode;			/* The root node. */
	long xOffset;			/* The x offset for the placing. */
	long yOffset;			/* The y offset for the placing. */

	/*
	 * These datas/variables are used by the random layouter.
	 */
	int keepRandomPositions;	/* Don't change the position of */
					/* already placed icons when */
					/* layouting with the random */
					/* placer. */
	long maxX, maxY;		/* Maximal X and Y coordinates */
					/* for the random placer. */
	/*
	 * Misc. variables used for layouting.
	 */

	int nodeNum;			/* The number of graph nodes. */
	Node** nodes;			/* The list of graph nodes. */
	int edgeNum;			/* The number of graph edges. */
	Edge** edges;			/* The list of graph edges. */
	int topListNum;			/* The current node number. */
	Topology **topList;		/* The sorted nodes. */
	int computeiconsize;		/* Use the biggest icon size. */
	int elementsPerLine;		/* How many elements per line. */
	int hideEdges;			/* make edges zero length (Matrix)*/
	int edgeHeight;			/* The standard height of an edge. */
	int edgeOrder;			/* Set the edges to the layout order.*/
	int edgeWidth;			/* The standard width of an edge. */
	int iconHeight, iconWidth;	/* The standard icon size. */
	double posX1, posY1, posX2, posY2;	/* Coordinates */
	double maxXPosition;		/* Maximal X and Y coordinates */
	double maxYPosition;		/* for the random placer. */
	int layoutTypesNum;		/* The number of layout types. */
	char **layoutTypes;		/* The types of items that will
					   be layouted. */
	int gridlock;			/* avoid using diagnal lines */
	char* errmsg;

#ifdef ignore
	char* graphName
	char *idlist;			/* The list of ids to layout. */
	char *sortcommand;		/* The Tcl procedure called for */
	long rootId;
	char convertBuffer[100];	/* Convert numbers to strings.*/
#endif
};

typedef struct Layout_Graph Layout_Graph;

static	int deleteedge _ANSI_ARGS_((Layout_Graph*,Edge*,int));
static	int deletenode _ANSI_ARGS_((Layout_Graph*,Node*,int));
static	void compress_succ _ANSI_ARGS_((Layout_Graph*,Node*));
static	void compress_parent _ANSI_ARGS_((Layout_Graph*,Node*));

static	void LayoutISISetX _ANSI_ARGS_((Layout_Graph*, Node*));
static	void LayoutISISetY _ANSI_ARGS_((Layout_Graph*, Node*/*, int type*/));
static	void LayoutTreeSetX _ANSI_ARGS_((Layout_Graph*, Node*));
static	void LayoutTreeSetY _ANSI_ARGS_((Layout_Graph*, Node*));
static	int LayoutBuildGraph _ANSI_ARGS_((Layout_Graph*));
static	Node* LayoutGraphRoot _ANSI_ARGS_((Layout_Graph*));
static	void LayoutGraphSortTopological _ANSI_ARGS_((Layout_Graph*, Node*));
static	int LayoutGraphPlaceNodes _ANSI_ARGS_((Layout_Graph*));
static	int LayoutGraphPlaceEdges _ANSI_ARGS_((Layout_Graph*));
static	int LayoutEdgeWidth _ANSI_ARGS_((Layout_Graph*));
static	int LayoutEdge _ANSI_ARGS_((Layout_Graph*, Edge*, Node*, Node*));

#if(defined(__cplusplus) || defined(c_plusplus))
#define AC1(t1,a1) (t1 a1)
#define AC2(t1,a1,t2,a2) (t1 a1, t2 a2)
#define AC3(t1,a1,t2,a2,t3,a3) (t1 a1, t2 a2, t3 a3)
#define AC4(t1,a1,t2,a2,t3,a3,t4,a4) (t1 a1, t2 a2, t3 a3, t4 a4)
#define AC5(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5) (t1 a1, t2 a2, t3 a3, t4 a4, t5 a5)
#else
#define AC1(t1,a1) (a1) t1 a1;
#define AC2(t1,a1,t2,a2) (a1,a2) t1 a1; t2 a2;
#define AC3(t1,a1,t2,a2,t3,a3) (a1,a2,a3) t1 a1; t2 a2; t3 a3;
#define AC4(t1,a1,t2,a2,t3,a3,t4,a4) (a1,a2,a3,a4) t1 a1; t2 a2; t3 a3; t4 a4;
#define AC5(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5) (a1,a2,a3,a4,a5) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5;
#endif

static
void
MY_LayoutISISetY _ANSI_ARGS_((Layout_Graph*, Node*, double));



/*
 *--------------------------------------------------------------
 *
 * LayoutDebugging --
 *
 *	This procedure is invoked to print debugging informations.
 *
 * Results:
 *	None
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

#if DEBUGGING
void
LayoutDebugging(
     Layout_Graph* This,
     Node *currentNode,                /* This is the current node. */
     char *string,
     int type)
{
  double tmpX, tmpY;

  if(THIS(graphOrder)) {
    /* Place nodes top down. */
    tmpX = THIS(xOffset) - NODE_HEIGHT(THIS(rootNode));
    tmpY = THIS(yOffset) - NODE_WIDTH(THIS(rootNode));
  } else {
    /* Place nodes left to right. */
    tmpX = THIS(xOffset) - NODE_WIDTH(THIS(rootNode));
    tmpY = THIS(yOffset) - NODE_HEIGHT(THIS(rootNode));
  }

  if(!DUMMY_NODE(currentNode)) {
    fprintf(stderr, "%-6s Node %-3ld: x=%-g y=%-g x=%-g y=%-g\n",
	    /* string, CANVAS_ITEM_ID(NODE_ITEM(currentNode)), 08nov95 wmt */
	    string, 0L,
	    NODE_X_POS(currentNode) + tmpX,
	    NODE_Y_POS(currentNode) + tmpY,
	    NODE_X_POS(currentNode),
	    NODE_Y_POS(currentNode));
  } else {
    fprintf(stderr, "%-6s Node dummy: x=%-g y=%-g x=%-g y=%-g\n",
	    string, NODE_X_POS(currentNode) + tmpX,
	    NODE_Y_POS(currentNode) + tmpY,
	    NODE_X_POS(currentNode),
	    NODE_Y_POS(currentNode));
  }
  switch (type) {
  case 1:
    fprintf(stderr,
	    "                 x=%-g y=%-g x=%-g y=%-g\n",
	    TREE_TMP_X_POS(currentNode) + tmpX,
	    TREE_TMP_Y_POS(currentNode) + tmpY,
	    TREE_TMP_X_POS(currentNode),
	    TREE_TMP_Y_POS(currentNode));
    break;
  default:
    break; 
  }
}
#endif

/*
 *--------------------------------------------------------------
 *
 * LayoutISISetX --
 *
 *	This procedure is invoked to calculate the x ISI position.
 *
 * Results:
 *	None
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) > (b) ? (b) : (a))
#endif
static
void
LayoutISISetX AC2(Layout_Graph*,This, Node*,currentNode) /* This is the current node. */
{
  int counter, visitedAllChilds = 1;

  if(IGNORE_NODE(currentNode)) {
    return;
  }
  if(VISITED_NODE(currentNode)) {
    return;
  }

  FOR_ALL_SUCCS(currentNode, counter) {
    /* Are there un layouted children ? */
    if(IGNORE_EDGE(SUCC_EDGE(currentNode, counter))) {
      continue;
    }
    if(IGNORE_NODE(SUCC_NODE(currentNode, counter))) {
      continue;
    }
    if(!VISITED_NODE(SUCC_NODE(currentNode, counter))) {
      visitedAllChilds = 0;
    }
  }

/*  SET_VISITED_NODE(currentNode, 1);*/
  if(!visitedAllChilds) {
    FOR_ALL_SUCCS(currentNode, counter) {
      /* Layout the children of this node. */
      if(IGNORE_EDGE(SUCC_EDGE(currentNode, counter))) {
	continue;
      }
      if(IGNORE_NODE(SUCC_NODE(currentNode, counter))) {
	continue;
      }
      if(!VISITED_NODE(SUCC_NODE(currentNode, counter))) {
	LayoutISISetX(This,SUCC_NODE(currentNode, counter));
      }
    }
    if(SUCC_NUM(currentNode) == 1) {
      SET_NODE_X_POS(currentNode,
		     NODE_X_POS(SUCC_NODE(currentNode, 0)));
    }
    else
    {
    
      /* Khamis 8:30 23 Oct 1996 */
    	int i, pingo = 0;
	double x1 = 0.0, x2 = 0.0;
    	FOR_ALL_SUCCS (currentNode, i)
	{
	   if(IGNORE_NODE(SUCC_NODE(currentNode, i)))
	     continue;
	   /*
	   if(! VISITED_NODE(SUCC_NODE(currentNode, i)))
	  	continue;
	    */
  	   if (PARENT_NODE(SUCC_NODE(currentNode, i), 0) !=
			currentNode)
	     continue;
	   if (! pingo)
	   {
	     x1 = NODE_X_POS(SUCC_NODE(currentNode, i));
	     pingo = 1;
	   }
	   x1 = min (x1, NODE_X_POS(SUCC_NODE(currentNode, i)));
	   x2 = max (x2, NODE_X_POS(SUCC_NODE(currentNode, i)));
	}
	if (! pingo)
	{
	  x1 = x2 = NODE_X_POS (SUCC_NODE(currentNode, 0));
	}

#if 1   /* Khamis 8:30 23 Oct 1996 */
        SET_NODE_X_POS(currentNode, x1 + (x2 - x1) / 2.0);
#else   /* original */
        SET_NODE_X_POS(currentNode,
		     NODE_X_POS(SUCC_NODE(currentNode, 0)) +
		     ((NODE_X_POS(SUCC_NODE(currentNode,
					   SUCC_NUM(currentNode)-1)) -
		       NODE_X_POS(SUCC_NODE(currentNode, 0))) / 2));
#endif
    }
  }
  else
  {
    /* Set the x position of the current node. */
    if(THIS(graphOrder)) {
      /* Place nodes top down. */
      SET_NODE_X_POS(currentNode, THIS(maxXPosition));
      THIS(maxXPosition) = NODE_X_POS(currentNode) + THIS(iconSpaceH) +
	THIS(edgeWidth) + NODE_WIDTH(currentNode);
    } else {
      /* Place nodes left to right. */
      SET_NODE_X_POS(currentNode, THIS(maxXPosition));
      THIS(maxXPosition) = NODE_X_POS(currentNode) + THIS(iconSpaceV) +
	THIS(edgeHeight) + NODE_HEIGHT(currentNode);
    }
  }
  SET_VISITED_NODE(currentNode, 1);
}

/*
 *--------------------------------------------------------------
 *
 * LayoutISISetY --
 *
 *	This procedure is invoked to calculate the y ISI position. 
 *
 * Results:
 *	None
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

    /* ARGSUSED */
static
void
LayoutISISetY AC2(Layout_Graph*,This, Node*,currentNode) /* This is the current node. */
{
  int counter;
  double tmpMaxY;

  /* Was this node already layouted ? */
  if(IGNORE_NODE(currentNode)) {
    return;
  }
  if(VISITED_NODE(currentNode)) {
    return;
  }

  SET_VISITED_NODE(currentNode, 1);
  
  if(PARENT_NUM(currentNode) != 0) {
    FOR_ALL_PARENTS(currentNode, counter) {
      /* Are there un layouted parents ? */
      if(IGNORE_EDGE(PARENT_EDGE(currentNode, counter))) {
	/*continue;*/
	break;
      }
      if(IGNORE_NODE(PARENT_NODE(currentNode, counter))) {
	/*continue;*/
	break;
      }
      if(!VISITED_NODE(PARENT_NODE(currentNode, counter))) {
	LayoutISISetY(This,PARENT_NODE(currentNode, counter));
      }
      break;
    }
    tmpMaxY = 0;
    FOR_ALL_PARENTS(currentNode, counter) {
      if(THIS(graphOrder)) {
	if(NODE_Y_POS(PARENT_NODE(currentNode, counter)) +
	    NODE_HEIGHT(PARENT_NODE(currentNode, counter)) > tmpMaxY) {
	  tmpMaxY = NODE_Y_POS(PARENT_NODE(currentNode, counter)) +
	    NODE_HEIGHT(PARENT_NODE(currentNode, counter));
	}
      } else {
	if(NODE_Y_POS(PARENT_NODE(currentNode, counter)) +
	    NODE_WIDTH
	    (PARENT_NODE(currentNode, counter)) > tmpMaxY) {
	  tmpMaxY = NODE_Y_POS(PARENT_NODE(currentNode, counter)) +
	    NODE_WIDTH(PARENT_NODE(currentNode, counter));
	}
      }
      break;
    }
    if(THIS(graphOrder)) {
      /* Place nodes top down. */
      SET_NODE_Y_POS(currentNode, tmpMaxY + THIS(edgeHeight) + THIS(iconSpaceV));
    } else {
      /* Place nodes left to right. */
      SET_NODE_Y_POS(currentNode, tmpMaxY + THIS(edgeWidth) + THIS(iconSpaceH));
    }
  } else {
    if(THIS(graphOrder)) {
      /* Place nodes top down. */
      SET_NODE_Y_POS(currentNode, 0);
    } else {
      /* Place nodes left to right. */
      SET_NODE_Y_POS(currentNode, 0);
    }
  }
  
  /*SET_VISITED_NODE(currentNode, 1);*/
}

/*********************************************************/
static
void
MY_LayoutISISetY AC3(Layout_Graph*,This, Node*,currentNode, double, y)
{
  int counter;
  double tmpMaxY;

  /* Was this node already layouted ? */
  if(IGNORE_NODE(currentNode)) {
    return;
  }
  if(VISITED_NODE(currentNode)) {
    return;
  }

  SET_VISITED_NODE(currentNode, 1);
  
  SET_NODE_Y_POS(currentNode, y);
  
  FOR_ALL_SUCCS (currentNode, counter) {
      /* Are there un layouted parents ? */
      if(IGNORE_EDGE(PARENT_EDGE(currentNode, counter))) {
	/*continue;*/
	break;
      }
      if(IGNORE_NODE(PARENT_NODE(currentNode, counter))) {
	/*continue;*/
	break;
      }

      if (PARENT_NODE(SUCC_NODE(currentNode, counter), 0) !=
          currentNode)
	continue;
	
      if(THIS(graphOrder)) {
        tmpMaxY = NODE_Y_POS  (currentNode) + NODE_HEIGHT (currentNode);
		/*+ THIS(edgeHeight) + THIS(iconSpaceV); */
      } else {
	tmpMaxY = NODE_Y_POS(currentNode) +
	          NODE_WIDTH(currentNode) +
		  THIS(edgeWidth) + THIS(iconSpaceH);
      }

      MY_LayoutISISetY(This, PARENT_NODE(currentNode, counter),
                       tmpMaxY);
  }
}
/*********************************************************/



/*
 *--------------------------------------------------------------
 *
 * LayoutTreeSetX --
 *
 *	This procedure is invoked to calculate the x tree position.
 *      The procedure is called for all icons in the topological
 *      order. 
 *
 * Results:
 *	None
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static
void
LayoutTreeSetX AC2(Layout_Graph*,This, Node*,currentNode) /* This is the current node */
{
  int counter;

  /* Was this node already layouted ? */
  if(TREE_TMP_X_POS(currentNode) != -1 || IGNORE_NODE(currentNode)) {
    return;
  }
  if(DUMMY_NODE(currentNode)) {
    return;
  }

  SET_VISITED_NODE(currentNode, 1);
  if(PARENT_NUM(currentNode) > 0 &&
      VISITED_NODE(PARENT_NODE(currentNode, 0))) {
    /* There are parents, and the parent was already visited. This */
    /* means that this node is the first child we layout at this */
    /* level. That means it occurs at the same level as the parent. */
    SET_VISITED_NODE(PARENT_NODE(currentNode, 0), 0);
    SET_TREE_TMP_X_POS(currentNode,
		       TREE_TMP_X_POS(PARENT_NODE(currentNode, 0)));
  } else {
    /* Append the icon to the current maximal x position. It is not */
    /* the first child of the parent. */
    SET_TREE_TMP_X_POS(currentNode, THIS(maxXPosition));
  }

  /* Set the x position of the current node. If the order is top down, */
  /* we use the maximum edge width. */
  if(THIS(graphOrder)) {
    /* Place nodes top down. */
    SET_NODE_X_POS(currentNode, TREE_TMP_X_POS(currentNode));
    /* Do we have a new maximal x position ? */
    if(NODE_X_POS(currentNode) + THIS(iconSpaceH) + THIS(edgeWidth) +
	NODE_WIDTH(currentNode) > THIS(maxXPosition)) {
      THIS(maxXPosition) = NODE_X_POS(currentNode) + THIS(iconSpaceH) +
	THIS(edgeWidth) + NODE_WIDTH(currentNode);
    }
  } else {
    /* Place nodes left to right. */
    SET_NODE_X_POS(currentNode, TREE_TMP_X_POS(currentNode));
    /* Do we have a new maximal x position ? */
    if((NODE_X_POS(currentNode) + THIS(iconSpaceV) + THIS(edgeHeight) +
	 NODE_HEIGHT(currentNode)) > THIS(maxXPosition)) {
      THIS(maxXPosition) = NODE_X_POS(currentNode) + THIS(iconSpaceV) +
	THIS(edgeHeight) + NODE_HEIGHT(currentNode);
    }
  }

  /* Walk through all successors. */
  FOR_ALL_SUCCS(currentNode, counter) {
    /* Layout the children of this node. */
    if(IGNORE_NODE(SUCC_EDGE(currentNode, counter))) {
      continue;
    }
    LayoutTreeSetX(This,SUCC_NODE(currentNode, counter));
  }
}

/*
 *--------------------------------------------------------------
 *
 * LayoutTreeSetY --
 *
 *	This procedure is invoked to calculate the y tree position. 
 *
 * Results:
 *	None
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static
void
LayoutTreeSetY AC2(Layout_Graph*,This, Node*,currentNode) /* This is the current node. */
{
  int counter;
  double tmpMaxY;

  if(IGNORE_NODE(currentNode)) {
    return;
  }
  if(DUMMY_NODE(currentNode)) {
    return;
  }
  if(VISITED_NODE(currentNode)) {
    return;
  }
  
  SET_VISITED_NODE(currentNode, 1);
  tmpMaxY = 0;
  /* Walk through all parents. */
  FOR_ALL_PARENTS(currentNode, counter) {
    /* Find the parent of this node that has the greatest Y. This way */
    /* the graph is always growing to the Y direction. */
    if(IGNORE_NODE(PARENT_EDGE(currentNode, counter)) ||
	IGNORE_NODE(PARENT_NODE(currentNode, counter))) {
      continue;
    }
    if(THIS(graphOrder)) {
      /* Place nodes top down. */
      if(TREE_TMP_Y_POS(PARENT_NODE(currentNode, counter)) +
	  NODE_HEIGHT(PARENT_NODE(currentNode, counter)) +
	  THIS(edgeHeight) + THIS(iconSpaceV) > tmpMaxY) {
	tmpMaxY = TREE_TMP_Y_POS(PARENT_NODE(currentNode, counter)) +
	  NODE_HEIGHT(PARENT_NODE(currentNode, counter)) + THIS(edgeHeight) +
	    THIS(iconSpaceV);
      }
    } else {
      if(TREE_TMP_Y_POS(PARENT_NODE(currentNode, counter)) +
	  NODE_WIDTH(PARENT_NODE(currentNode, counter)) +
	  THIS(edgeWidth) + THIS(iconSpaceH) > tmpMaxY) {
	tmpMaxY = TREE_TMP_Y_POS(PARENT_NODE(currentNode, counter)) +
	  NODE_WIDTH(PARENT_NODE(currentNode, counter)) + THIS(edgeWidth) +
	    THIS(iconSpaceH);
      }
    }
  }
  
  /* Set the y position of the current node. */
  if(THIS(graphOrder)) {
    /* Place nodes top down. */
    SET_NODE_Y_POS(currentNode, tmpMaxY);
    /* Keep the maximal y position, this way we can later calculate the */
    /* correct Y position for children of this widget. */
    SET_TREE_TMP_Y_POS(currentNode, NODE_Y_POS(currentNode));
  } else {
    /* Place nodes left to right. */
    SET_NODE_Y_POS(currentNode, tmpMaxY);
    /* Keep the maximal y position, this way we can later calculate the */
    /* correct Y position for children of this widget. */
    SET_TREE_TMP_Y_POS(currentNode, NODE_Y_POS(currentNode));
  }
}

/*
 *--------------------------------------------------------------
 *
 * createnode --
 *
 *	This procedure is invoked to create a new node. Optionally the
 *      procedure can be used to create dummy nodes. In that case the
 *      fromNode and toNode parameters are specified.
 *
 * Results:
 *	None
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

int
LayoutCreateNode AC4(Layout_Graph*,This,pItem,itemPtr, pItem,fromNode, pItem, toNode)
{
  int counter1 = 0, counter2 = 0, counter3 = 0, counter4 = 0, found = 0;
  Node *tmpNode;
  Edge *tmpEdge;
  ItemGeom bbox;

  /* see if this item was already added */
  FOR_ALL_NODES(counter1) {
	if(NODE_ITEM(THIS(nodes)[counter1]) == itemPtr) {
	    THIS(errmsg) = "attempt to insert duplicate graph node";
	    return LAYOUT_ERROR; 
	}
  }
  THIS(nodeNum)++;
  if(THIS(nodes) == NULL) {
    THIS(nodes) = (Node **) ckalloc(THIS(nodeNum) * sizeof(Node *));
  } else {
    THIS(nodes) = (Node **) ckrealloc((char *) THIS(nodes),
			      THIS(nodeNum) * sizeof(Node *));
  }
  tmpNode = (Node *) ckalloc(sizeof(Node));
  SET_NODE_ITEM(tmpNode, itemPtr);
  SET_IGNORE_NODE(tmpNode, 0);
  SET_VISITED_NODE(tmpNode, 0);
  SET_NODE_X_POS(tmpNode, 0);
  SET_NODE_Y_POS(tmpNode, 0);
  SET_TREE_TMP_X_POS(tmpNode, -1);
  SET_TREE_TMP_Y_POS(tmpNode, -1);
  bbox.x1 = bbox.y1 = bbox.x2 = bbox.y2 = bbox.width = bbox.height = 0;
  SET_NODE_GEOM(tmpNode,bbox);
  SET_PARENT_NUM(tmpNode, 0);
  tmpNode->parent = (Edge**) NULL;
  SET_SUCC_NUM(tmpNode, 0);
  tmpNode->succ = (Edge**) NULL;
  THIS(nodes)[THIS(nodeNum)-1] = tmpNode;
  
#if 0
  /* create the specific data slot. */
  if(createdatanode != NULL) {
    (*createdatanode)(THIS(nodes)[THIS(nodeNum)-1]);
  }
#endif

  /* insert the dummy node. */
  if(fromNode != (Node*) NULL && toNode != (Node*) NULL) {
    FOR_ALL_NODES(counter1) {
      if(THIS(nodes)[counter1] == fromNode) {
	FOR_ALL_SUCCS(THIS(nodes)[counter1], counter2) {
	  if(SUCC_NODE(THIS(nodes)[counter1], counter2) == toNode) {
	    found++;
	    break;
	  }
	}
	break;
      }
    }
    FOR_ALL_NODES(counter3) {
      if(THIS(nodes)[counter3] == toNode) {
	FOR_ALL_PARENTS(THIS(nodes)[counter3], counter4) {
	  if(PARENT_NODE(THIS(nodes)[counter3], counter4) == fromNode) {
	    found++;
	    break;
	  }
	}
	break;
      }
    }
    if(found == 2) {
      DEBUG_PRINT_NODE_POS(This, THIS(nodes)[counter1], "dummy insert from");
      DEBUG_PRINT_NODE_POS(This, THIS(nodes)[counter3], "dummy insert to");
      SET_PARENT_NUM(tmpNode, 1);
      SET_SUCC_NUM(tmpNode, 1);
      SET_NODE_X_POS(tmpNode, 10);
      SET_NODE_Y_POS(tmpNode, 10);
      THIS(nodes)[THIS(nodeNum)-1]->parent = (Edge**) 
	ckalloc(THIS(nodes)[THIS(nodeNum)-1]->parentNum * sizeof(Edge*));
      tmpEdge = (Edge* ) ckalloc(sizeof(Edge));
      SET_IGNORE_EDGE(tmpEdge, 0);
      SET_VISITED_EDGE(tmpEdge, 0);
      tmpEdge->fromNode = THIS(nodes)[counter1];
      tmpEdge->toNode = THIS(nodes)[counter3];
      THIS(nodes)[THIS(nodeNum)-1]->parent[0] = tmpEdge;
      THIS(nodes)[THIS(nodeNum)-1]->succ = (Edge**) 
	ckalloc(THIS(nodes)[THIS(nodeNum)-1]->succNum * sizeof(Edge* ));
      THIS(nodes)[THIS(nodeNum)-1]->succ[0] = tmpEdge;
      THIS(nodes)[counter3]->parent[counter4]->toNode = tmpNode;
      THIS(nodes)[counter1]->succ[counter2]->fromNode = tmpNode;
    }
  }
  return LAYOUT_OK;
}

int
LayoutDeleteNode AC2(Layout_Graph*,This, pItem,nodeid)
{
    register int i;

    /* find the matching node*/
    FOR_ALL_NODES(i) {
	if(NODE_ITEM(THIS(nodes)[i]) == nodeid) {
	    return deletenode(This,THIS(nodes)[i],i);
	}
    }
    THIS(errmsg) = "node delete: no such node";
    return LAYOUT_ERROR;
}

static
int
deletenode AC3(Layout_Graph*,This, Node*,thisnode, int,index)
{
    register int i;
    /* remove all attached edges */
    FOR_ALL_EDGES(i) {
	register Edge* e = THIS(edges)[i];
	if(e->toNode == thisnode
	   || e->fromNode == thisnode) {
	    deleteedge(This,e,i);
	}
    }

    /* clean up node */
    if(thisnode->parent) ckfree((char*)thisnode->parent);
    if(thisnode->succ) ckfree((char*)thisnode->succ);

    /* free and clear node */
    THIS(nodeNum)--;
    if(THIS(nodeNum) > 0) {
	THIS(nodes)[index] = THIS(nodes)[THIS(nodeNum)];
    }
    ckfree((char*)thisnode);
    return LAYOUT_OK;
}


int
LayoutCreateEdge AC4(Layout_Graph*,This, pItem,edgeid, pItem,fromid, pItem,toid)
{
    register int i;
    register Node* n;
    Node* fromnode = NULL;
    Node* tonode = NULL;
    Edge* tmpEdge;

    /* see if this item was already added */
    FOR_ALL_EDGES(i) {
	if(EDGE_ITEM(THIS(edges)[i]) == edgeid) {
	    THIS(errmsg) = "attempt to insert duplicate graph edge";
	    return LAYOUT_ERROR; 
	}
    }
    /* locate the actual from and to nodes */
    FOR_ALL_NODES(i) {
	n = THIS(nodes)[i];
	if(NODE_ITEM(n) == fromid) {
	    fromnode = n;
	} else if(NODE_ITEM(n) == toid) {
	    tonode = n;
	}
    }
    if(!fromnode || !tonode) {
	THIS(errmsg) = "edge was missing from or to node";
	return LAYOUT_ERROR;
    }

    /* create Edge */
    tmpEdge = (Edge* ) ckalloc(sizeof(Edge));
    if(!tmpEdge) {
	THIS(errmsg) = "malloc failure";
	return LAYOUT_ERROR;
    }
    SET_IGNORE_EDGE(tmpEdge, 0);
    SET_VISITED_EDGE(tmpEdge, 0);
    tmpEdge->edgeid = edgeid;
    tmpEdge->fromNode = fromnode;
    tmpEdge->toNode = tonode;

    THIS(edgeNum)++;
    if(THIS(edges) == NULL) {
	THIS(edges) = (Edge* *) ckalloc(THIS(edgeNum) * sizeof(Edge* ));
    } else {
	THIS(edges) = (Edge* *) ckrealloc((char *) THIS(edges),
			      THIS(edgeNum) * sizeof(Edge* ));
    }
    THIS(edges)[THIS(edgeNum)-1] = tmpEdge;

    /* insert the succ and parent edge structs */
    tonode->parentNum++;
    if(tonode->parent == NULL) {
	tonode->parent = (Edge**) 
		ckalloc(tonode->parentNum * sizeof(Edge*));
    } else {
	tonode->parent = (Edge**) 
		ckrealloc((char *) tonode->parent,
			tonode->parentNum * sizeof(Edge*));
    }
    tonode->parent[tonode->parentNum - 1] = tmpEdge;

    fromnode->succNum++;
    if(fromnode->succ == NULL) {
	fromnode->succ = (Edge**) 
		ckalloc(fromnode->succNum * sizeof(Edge*));
    } else {
	fromnode->succ = (Edge**) 
		ckrealloc((char *) fromnode->succ,
			fromnode->succNum * sizeof(Edge*));
    }
    fromnode->succ[fromnode->succNum-1] = tmpEdge;

    return LAYOUT_OK;
}

static
void
compress_succ AC2(Layout_Graph*,This, Node*,n)
{
    register Edge** p;
    register Edge** q;

    for(p=n->succ,q=p+(n->succNum);p < q;p++) {
	if(*p != NULL) continue;
	/* found a null entry earlier than where q is pointing */
	/* move q down to look for a non-null entry */
	do { --q; } while(q > p && *q == NULL);
	if(q <= p) break; /* p points to last non-null entry */
	*p = *q;
    }
    n->succNum = p - n->succ;
}

static
void
compress_parent AC2(Layout_Graph*,This, Node*,n)
{
    register Edge** p;
    register Edge** q;

    for(p=n->parent,q=p+(n->parentNum);p < q;p++) {
	if(*p != NULL) continue;
	/* found a null entry earlier than where q is pointing */
	/* move q down to look for a non-null entry */
	do { --q; } while(q > p && *q == NULL);
	if(q <= p) break; /* p points to last non-null entry */
	*p = *q;
    }
    n->parentNum = p - n->parent;
}

static
int
deleteedge AC3(Layout_Graph*,This, Edge*,e, int,index)
{
    register int i;
    register int j;
    register int found;

    /* remove all references to this Edge */
    FOR_ALL_NODES(i) {
	register Node* n = THIS(nodes)[i];
	found = 0;
	FOR_ALL_SUCCS(n,j) {
	    if(SUCC_EDGE(n,j) == e) {
		SUCC_EDGE(n,j) = NULL;
		found = 1;
	    }
	}
	if(found) {compress_succ(This,n);}
	found = 0;
	FOR_ALL_PARENTS(n,j) {
	    if(PARENT_EDGE(n,j) == e) {
		PARENT_EDGE(n,j) = NULL;
		found = 1;
	    }
	}
	if(found) {compress_parent(This,n);}	
    }
    /* free and clear Edge*/
    THIS(edgeNum)--;
    if(THIS(edgeNum) > 0) {
	THIS(edges)[index] = THIS(edges)[THIS(edgeNum)];
    }
    ckfree((char*)e);
    return LAYOUT_OK;
}

int
LayoutDeleteEdge AC2(Layout_Graph*,This, pItem,eid)
{
    register int i;

    /* find matching edge object */
    FOR_ALL_EDGES(i) {
	register Edge* e = THIS(edges)[i];
	if(e->edgeid == eid) {
	    return deleteedge(This,e,i);
	}
    }
    THIS(errmsg) = "edge delete: no such edge";
    return LAYOUT_ERROR;
}


/*
 *--------------------------------------------------------------
 *
 * LayoutBuildGraph --
 *
 *	This procedure is invoked to create the internal
 *      graph structure used for layouting.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static
int
LayoutBuildGraph AC1(Layout_Graph*,This)
{
  register int counter;

  /* Walk through all nodes to compute various things */
  FOR_ALL_NODES(counter) {
    register Node* n = THIS(nodes)[counter];
    /* Find the greatest icon dimensions. */
    if(NODE_WIDTH(n) > THIS(iconWidth)) {
      THIS(iconWidth) = (int)NODE_WIDTH(n);
    }
    if(NODE_HEIGHT(n) > THIS(iconHeight)) {
      THIS(iconHeight) = (int)NODE_HEIGHT(n);
    }
  }

  return LAYOUT_OK;
}

/*
 *--------------------------------------------------------------
 *
 * LayoutClearGraph --
 *
 *--------------------------------------------------------------
 */

void
LayoutClearGraph AC1(Layout_Graph*,This)
{
  register int counter;
  register Node* n;

  /* Free allocated memory. */
  FOR_ALL_EDGES(counter) {
    ckfree((char *) THIS(edges)[counter]);
  }
  THIS(edgeNum) = 0;
  FOR_ALL_NODES(counter) {
    n = THIS(nodes)[counter];
    if (n->parent != NULL)
    	ckfree((char*)n->parent);
	if (n->succ != NULL)
    	ckfree((char*)n->succ);
    if (n != NULL)
    	ckfree((char *)n);
  }
  THIS(nodeNum) = 0;
  FOR_ALL_TOP_NODES(counter) {
    ckfree((char *) THIS(topList)[counter]);
  }
  THIS(topListNum) = 0;
  if (THIS(topList) != NULL)
  {
	ckfree ((char *) (THIS(topList)));
	THIS(topList) = NULL;
  }
  if (THIS(nodes) != NULL)
  {
	ckfree ((char *) (THIS(nodes)));
	THIS(nodes) = NULL;
  }
  THIS(rootNode) = NULL;
}


/*
 *--------------------------------------------------------------
 *
 * LayoutFreeGraph --
 *
 *	This procedure is invoked to free the graph structures
 *      used for layouting.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

void
LayoutFreeGraph AC1(Layout_Graph*,This)
{
  LayoutClearGraph(This);

  /* now cleanup the Layout Graph structure */
  if (THIS(edges) != NULL)
  {
	ckfree((char *) THIS(edges));
	THIS(edges) = NULL;
  }
  if (THIS(nodes) != NULL)
  {
	ckfree((char *) THIS(nodes));
	THIS(nodes) = NULL;
  }
  if (THIS(topList) != NULL)
  {
	ckfree((char *) THIS(topList));
	THIS(topList) = NULL;
  }
  
  /* free graph layout structure */
  ckfree ((char *) This);
}

/*
 *--------------------------------------------------------------
 *
 * LayoutGraphRoot --
 *
 *	This procedure is invoked to find the root of a graph.
 *
 * Results:
 *	The root node.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static
Node*
LayoutGraphRoot AC1(Layout_Graph*,This)
{
  int optimalRootNum = -10000, minParentNum = -1, maxSuccNum = -1,
      counter, counter1, counter2, counter3, lidx;
  Node *tmprootNode, *node;

  /* Khamis 27-mar-97
   * reorder nodes, so that nodes with not empty subtree displayed
   * first
   */
  lidx = 0;
  FOR_ALL_NODES(counter)
  {
    if (counter == 0 || IGNORE_NODE(THIS(nodes)[counter]))
      continue;
      
    if (SUCC_NUM(THIS(nodes)[counter]) > SUCC_NUM(THIS(nodes)[lidx]))
    {
      node = THIS(nodes)[counter];
      THIS(nodes)[counter] = THIS(nodes)[lidx];
      THIS(nodes)[lidx] = node;
      
      /* search for next node with no subtree */
      while (SUCC_NUM(THIS(nodes)[lidx]) > 0 && lidx < counter)
      {
      	lidx ++;
      }
    }
  }

  tmprootNode = THIS(rootNode);
  
  /* Find a root node. This node has no parents. In case we do not */
  /* have such a node... find the node with the smallest number of */
  /* parents. */

#if 0 /* Zsolt Koppany */
  tmprootNode = THIS(nodes)[0];
  return tmprootNode;
#endif
  if(!tmprootNode) {
    /* We try to find the node with the most children and the least */
    /* parents. This node will become root. */
    FOR_ALL_NODES(counter) {
      if(IGNORE_NODE(THIS(nodes)[counter])) {
	continue;
      }
      if((SUCC_NUM(THIS(nodes)[counter]) > 0 &&
	   optimalRootNum <= (SUCC_NUM(THIS(nodes)[counter]) -
			     PARENT_NUM(THIS(nodes)[counter])) &&
	   (minParentNum > PARENT_NUM(THIS(nodes)[counter]) ||
	    (minParentNum == PARENT_NUM(THIS(nodes)[counter]) &&
	    maxSuccNum < SUCC_NUM(THIS(nodes)[counter])))) ||
	  optimalRootNum == -10000 ||
	    
	  /* khamis: 17-mars-97, root with no parents have more priority */
	  PARENT_NUM(THIS(nodes)[counter]) < PARENT_NUM(tmprootNode)) {
	tmprootNode = THIS(nodes)[counter];

	minParentNum = PARENT_NUM(THIS(nodes)[counter]);
	maxSuccNum = SUCC_NUM(THIS(nodes)[counter]);
	if(SUCC_NUM(THIS(nodes)[counter]) > 0) {
	  optimalRootNum =
	    (SUCC_NUM(THIS(nodes)[counter]) - PARENT_NUM(THIS(nodes)[counter]));
	}
      }
    }
  }
  
  /* No nodes... so abort the search. */
  if(tmprootNode == NULL) {
    return NULL;
  }

  /* There is no node with no parents. So use the node with the */
  /* smallest number of parents, and ignore the edges leading to this */
  /* node. */
  if(PARENT_NUM(tmprootNode) != 0) {
    for (counter1 = 0; counter1 < PARENT_NUM(tmprootNode); counter1++) {
      SET_IGNORE_NODE(PARENT_EDGE(tmprootNode, counter1), 1);
      FOR_ALL_NODES(counter2) {
	/* Ignore dummy nodes. */
	if(DUMMY_NODE(THIS(nodes)[counter2])) {
	  continue;
	}
	if(NODE_ITEM(THIS(nodes)[counter2]) ==
	    NODE_ITEM(PARENT_NODE(tmprootNode, counter1))) {
	  FOR_ALL_SUCCS(THIS(nodes)[counter2], counter3) {
	    if(NODE_ITEM(SUCC_NODE(THIS(nodes)[counter2], counter3)) ==
		NODE_ITEM(tmprootNode)) {
	      SET_IGNORE_NODE(SUCC_EDGE(THIS(nodes)[counter2], counter3), 1);
	    }
	  }
	}
      }
    }
  }
  return tmprootNode;
}

/*
 *--------------------------------------------------------------
 *
 * LayoutGraphSortTopological --
 *
 *	This procedure is invoked to sort a graph topological. 
 *
 * Results:
 *	None
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static
void
LayoutGraphSortTopological AC2(Layout_Graph*,This, Node*,currentNode) /* This is the current node. */
{
  int counter;
  Topology *tmpTopology;
    
  if(VISITED_NODE(currentNode) || IGNORE_NODE(currentNode)) {
    return;
  }

  /* Append the current node to the list of topologically sorted */
  /* nodes. */
  THIS(topListNum)++;
  if(THIS(topList) == NULL) {
    THIS(topList) = (Topology **) ckalloc(THIS(topListNum) * sizeof(Topology *));
  } else {
    THIS(topList) = (Topology **) ckrealloc((char *) THIS(topList),
				    THIS(topListNum) * sizeof(Topology *));
  }
  tmpTopology = (Topology *) ckalloc(sizeof(Topology));
  tmpTopology->nodePtr = currentNode;
  THIS(topList)[THIS(topListNum)-1] = tmpTopology;
  
  SET_VISITED_NODE(currentNode, 1);
  /* Walk through all successors. */
  FOR_ALL_SUCCS(currentNode, counter) {
    if(IGNORE_EDGE(SUCC_EDGE(currentNode, counter))) {
      continue;
    }
    if(IGNORE_NODE(SUCC_NODE(currentNode, counter))) {
      continue;
    }
    if(VISITED_NODE(SUCC_NODE(currentNode, counter))) {
      SET_IGNORE_EDGE(SUCC_EDGE(currentNode, counter), 1);
      continue;
    }
    LayoutGraphSortTopological(This,SUCC_NODE(currentNode, counter));
  }
}

/*
 *--------------------------------------------------------------
 *
 * LayoutGraphPlaceNodes --
 *
 *	This procedure is invoked to actually place the graph nodes.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static
int
LayoutGraphPlaceNodes AC1(Layout_Graph*,This)
{
  int counter;
  double tmpX, tmpY;

  SRANDOM(getpid() + time((time_t *) NULL));

  FOR_ALL_NODES(counter) {
    register Node* n = THIS(nodes)[counter];
    if(IGNORE_NODE(n)) {
      continue;
    }
    if(NODE_X_POS(n) != -1 &&
	NODE_Y_POS(n) != -1) {
      if(THIS(graphOrder)) {
	/* Place nodes top down. */
	tmpX = NODE_X_POS(n);
	tmpY = NODE_Y_POS(n);
      } else {
	/* Place nodes left to right. */
	tmpX = NODE_Y_POS(n);
	tmpY = NODE_X_POS(n);
      }
    } else {
      /* are we allowed to place the icon ? */
      if(THIS(keepRandomPositions) &&
	  NODE_X_POS(n) > 0 &&
	  NODE_Y_POS(n) > 0) {
	continue;
      }
      tmpX = (long) (RANDOM % THIS(maxX)) - NODE_WIDTH(n);
      tmpY = (long) (RANDOM % THIS(maxY)) - NODE_HEIGHT(n);
    }
    if(tmpX < 0) {
      tmpX = 0;
    }
    if(tmpY < 0) {
      tmpY = 0;
    }
    if(!DUMMY_NODE(n)) {
	ItemGeom g;
	g = NODE_GEOM(n);
	/* recalc Item Geom based on our layout */
	g.x1 = tmpX+THIS(xOffset);
	g.y1 = tmpY+THIS(yOffset);
	g.x2 = g.x1 + g.width;
	g.y2 = g.y1 + g.height;
	SET_NODE_GEOM(n,g);
    }
  }
  return LAYOUT_OK;
}

/*
 *--------------------------------------------------------------
 *
 * LayoutGraphPlaceEdges --
 *
 *	This procedure is invoked to relayout all edges.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static
int
LayoutGraphPlaceEdges AC1(Layout_Graph*,This)
{
  register int i;

  /* scan through all edges */
  FOR_ALL_EDGES(i) {
    /* layout edges. */
    LayoutEdge(This,THIS(edges)[i], NULL, NULL);
  }
  return LAYOUT_OK;
}

/*
 *--------------------------------------------------------------
 *
 * LayoutEdgeWidth --
 *
 *	This procedure is invoked to find the widest edge. Widest
 *      means the edge with the maximal x expansion.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static
int
LayoutEdgeWidth AC1(Layout_Graph*,This)
{
#if 1 /* Khamis, 19:00 20 Okt 1996 */
  THIS(edgeHeight) = 0;
  THIS(edgeWidth) = 0;
#else
  register int i;
  if(THIS(edgeWidth) == 0) {
    /* Walk through all edges. */
    FOR_ALL_EDGES(i) {
	if(THIS(edges)[i]->info.height + LINE_INCREMENT > THIS(edgeHeight)) {
	  THIS(edgeHeight) = THIS(edges)[i]->info.height + LINE_INCREMENT;
	}
	if(THIS(edges)[i]->info.width + LINE_INCREMENT > THIS(edgeWidth)) {
	  THIS(edgeWidth) = THIS(edges)[i]->info.width + LINE_INCREMENT;
	}
    }
  }

#endif

  return LAYOUT_OK;
}

/*
 *--------------------------------------------------------------
 *
 * LayoutEdge --
 *
 *	This procedure is invoked to adjust the edge to the new
 *      locations of the connected nodes. This algorithm only works
 *      for simple edges.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static
int
LayoutEdge AC4(Layout_Graph*,This,
    Edge*,e,			/* Current edge. */
    Node*,fromNode,			/* Source node. */
    Node*,toNode)			/* Target node. */
{
  int result = LAYOUT_OK;
#if 0
  Node *currentNode;
#endif
  ItemGeom geom;

  if(fromNode == (Node*) NULL && toNode == (Node*) NULL)
  {
    fromNode = e->fromNode;
    toNode = e->toNode;    
  }
#if 0
  else
  {
    /* Place one specific edge... */
    /* WARNING !!! this code is old and not adapted to the new Edge*/
    /* placing. The code is not tested. This stuff will be used to */
    /* display multi point edges.... */
    fromPtr = NODE_ITEM(fromNode);
    while (DUMMY_NODE(toNode) && SUCC_NUM(toNode) > 0) {
      toNode = SUCC_NODE(toNode, 0);
    }
    toPtr = NODE_ITEM(toNode);
    if(itemPtr == (pItem ) NULL) {
      /* Match the numeric id value to a concrete item pointer. */
      FOR_ALL_CANVAS_ITEMS(canvasPtr, itemPtr) {
	if(strcmp(CANVAS_ITEM_TYPE(itemPtr), "edge") == 0) {
	  /* Get "from" id */
	  Tk_ConfigureInfo(canvasPtr->interp, canvasPtr->tkwin,
			   itemPtr->typePtr->configSpecs,
			   (char *) itemPtr, "-from", 0);
	  if(Tcl_SplitList(canvasPtr->interp,
			    canvasPtr->interp->result,
			    &argc2, &argv2) != LAYOUT_OK) {
	    return LAYOUT_ERROR;
	  }
	  fromId = atol(argv2[4]);
	  ckfree((char *) argv2);
	  
	  /* Get "to" id */
	  Tk_ConfigureInfo(canvasPtr->interp, canvasPtr->tkwin,
			   itemPtr->typePtr->configSpecs,
			   (char *) itemPtr, "-to", 0);
	  if(Tcl_SplitList(canvasPtr->interp,
			    canvasPtr->interp->result,
			    &argc2, &argv2) != LAYOUT_OK) {
	    return LAYOUT_ERROR;
	  }
	  toId = atol(argv2[4]);
	  ckfree((char *) argv2);
	
	  if(fromId == CANVAS_ITEM_ID(fromPtr) &&
	      toId == CANVAS_ITEM_ID(toPtr)) {
	    curPtr = itemPtr;
	    FOR_ALL_SUCCS(fromNode, counter1) {
	      Tcl_DStringInit(&positionString);
	      if(fromPtr->x1 > fromPtr->x2) {
		THIS(posX1) = fromPtr->x1 + ((fromPtr->x2 - fromPtr->x1) / 2);
	      } else {
		THIS(posX1) = fromPtr->x2 + ((fromPtr->x1 - fromPtr->x2) / 2);
	      }
	      if(fromPtr->y1 > fromPtr->y2) {
		THIS(posY1) = fromPtr->y1 + ((fromPtr->y2 - fromPtr->y1) / 2);
	      } else {
		THIS(posY1) = fromPtr->y2 + ((fromPtr->y1 - fromPtr->y2) / 2);
	      }
	      if(toPtr->x1 > toPtr->x2) {
		THIS(posX2) = toPtr->x1 + ((toPtr->x2 - toPtr->x1) / 2);
	      } else {
		THIS(posX2) = toPtr->x2 + ((toPtr->x1 - toPtr->x2) / 2);
	      }
	      if(toPtr->y1 > toPtr->y2) {
		THIS(posY2) = toPtr->y1 + ((toPtr->y2 - toPtr->y1) / 2);
	      } else {
		THIS(posY2) = toPtr->y2 + ((toPtr->y1 - toPtr->y2) / 2);
	      }
	      
	      if(fromPtr->y2 <= toPtr->y1) {
		sprintf(convertBuffer, "%d %d ", THIS(posX1), fromPtr->y2);
	      } else {
		if(fromPtr->y1 >= toPtr->y2) {
		  sprintf(convertBuffer, "%d %d ", THIS(posX1), fromPtr->y1);
		} else {
		  if(fromPtr->x2 < toPtr->x1) {
		    sprintf(convertBuffer, "%d %d ", fromPtr->x2, THIS(posY1));
		  } else {
		    sprintf(convertBuffer, "%d %d ", fromPtr->x1, THIS(posY1));
		  }
		}
	      }
	      
	      Tcl_DStringAppend(&positionString, convertBuffer, -1);
	      currentNode = SUCC_NODE(fromNode, counter1);
	      while (DUMMY_NODE(currentNode) && SUCC_NUM(currentNode) > 0) {
		currentNode = SUCC_NODE(currentNode, 0);
		sprintf(convertBuffer, "%g %g ", /*300.0, 300.0*/
			NODE_X_POS(currentNode), NODE_Y_POS(currentNode));
		Tcl_DStringAppend(&positionString, convertBuffer, -1);
	      }
	      if(NODE_ITEM(currentNode) != toPtr) {
		Tcl_DStringFree(&positionString);
		continue;
	      }
	      if(fromPtr->y2 <= toPtr->y1) {
		sprintf(convertBuffer, "%d %d ", THIS(posX2), toPtr->y1);
	      } else {
		if(fromPtr->y1 >= toPtr->y2) {
		  sprintf(convertBuffer, "%d %d ", THIS(posX2), toPtr->y2);
		} else {
		  if(fromPtr->x2 < toPtr->x1) {
		    sprintf(convertBuffer, "%d %d ", toPtr->x1, THIS(posY2));
		  } else {
		    sprintf(convertBuffer, "%d %d ", toPtr->x2, THIS(posY2));
		  }
		}
	      }
	      Tcl_DStringAppend(&positionString, convertBuffer, -1);

	      /* Set new coordinates */
	      if(Tcl_SplitList(canvasPtr->interp, positionString.string,
				&argc2, &argv2) != LAYOUT_OK) {
		return LAYOUT_ERROR;
	      }
	      Tcl_ResetResult(canvasPtr->interp);
	      result = (*curPtr->typePtr->coordProc)
		(canvasPtr, curPtr, argc2, argv2);
	      ckfree((char *) argv2);
	      Tcl_DStringFree(&positionString);
	    }
	    return result;
	  }
	}
      }
    }
  }
#endif /* #if 0 */

  /* Is this a regular edge ? */
  if(fromNode != NULL && toNode != NULL) {
    /* calculate the various node anchors. */
    /* calc center of the from node */
    if(fromNode->info.x1 > fromNode->info.x2) {
      THIS(posX1) = fromNode->info.x1 + ((fromNode->info.x2 - fromNode->info.x1) / 2);
    } else {
      THIS(posX1) = fromNode->info.x2 + ((fromNode->info.x1 - fromNode->info.x2) / 2);
    }
    if(fromNode->info.y1 > fromNode->info.y2) {
      THIS(posY1) = fromNode->info.y1 + ((fromNode->info.y2 - fromNode->info.y1) / 2);
    } else {
      THIS(posY1) = fromNode->info.y2 + ((fromNode->info.y1 - fromNode->info.y2) / 2);
    }
    /* calc center of the to node */
    if(toNode->info.x1 > toNode->info.x2) {
      THIS(posX2) = toNode->info.x1 + ((toNode->info.x2 - toNode->info.x1) / 2);
    } else {
      THIS(posX2) = toNode->info.x2 + ((toNode->info.x1 - toNode->info.x2) / 2);
    }
    if(toNode->info.y1 > toNode->info.y2) {
      THIS(posY2) = toNode->info.y1 + ((toNode->info.y2 - toNode->info.y1) / 2);
    } else {
      THIS(posY2) = toNode->info.y2 + ((toNode->info.y1 - toNode->info.y2) / 2);
    }

    if(THIS(edgeOrder)) {
      /* Place the edges according to the graph order... only along
       * the generale layout direction. */
      if(THIS(graphOrder)) {
	/* Place nodes top down. */
        if(fromNode->info.y2 <= toNode->info.y1) {
	  geom.x1 = THIS(posX1);
	  geom.y1 = fromNode->info.y2;
	  geom.x2 = THIS(posX2);
	  geom.y2 = toNode->info.y1;
        } else {
	  geom.x1 = THIS(posX1);
	  geom.y1 = fromNode->info.y1;
	  geom.x2 = THIS(posX2);
	  geom.y2 = toNode->info.y2;
        }
      } else {
	/* Place nodes left to right. */
        if(fromNode->info.x2 < toNode->info.x1) {
	  geom.x1 = fromNode->info.x2;
	  geom.y1 = THIS(posY1);
	  geom.x2 = toNode->info.x1;
	  geom.y2 = THIS(posY2);
        } else {
	  geom.x1 = fromNode->info.x1;
	  geom.y1 = THIS(posY1);
	  geom.x2 = toNode->info.x2;
	  geom.y2 = THIS(posY2);
        }
      }
    } else {
      /* Place the edges so that they use the shortest distance. */
      if(fromNode->info.y2 <= toNode->info.y1) {
	/* from is above to */
	if(fromNode->info.x2 <= toNode->info.x1) {
	  /* from is left from to */
          if(THIS(graphOrder)) {
	    /* Place nodes top down. */
	    geom.x1 = THIS(posX1);
	    geom.y1 = fromNode->info.y2;
	    geom.x2 = THIS(posX2);
	    geom.y2 = toNode->info.y1;
	  } else {
	    geom.x1 = fromNode->info.x2;
	    geom.y1 = THIS(posY1);
	    geom.x2 = toNode->info.x1;
	    geom.y2 = THIS(posY2);
	  }
	} else {
	  if(fromNode->info.x1 >= toNode->info.x2) {
	    /* from is right from to */
	    if(THIS(graphOrder)) {
	      /* Place nodes top down. */
	      geom.x1 = THIS(posX1);
	      geom.y1 = fromNode->info.y2;
	      geom.x2 = THIS(posX2);
	      geom.y2 = toNode->info.y1;
	    } else {
	      geom.x1 = fromNode->info.x1;
	      geom.y1 = THIS(posY1);
	      geom.x2 = toNode->info.x2;
	      geom.y2 = THIS(posY2);
	    }
	  } else {
	    /* from is at same level as to */
	    geom.x1 = THIS(posX1);
	    geom.y1 = fromNode->info.y2;
	    geom.x2 = THIS(posX2);
	    geom.y2 = toNode->info.y1;
	  }
	}
      } else {
	if(fromNode->info.y1 >= toNode->info.y2) {
	  /* from is below to */
	  if(fromNode->info.x2 <= toNode->info.x1) {
	    /* from is left from to */
	    if(THIS(graphOrder)) {
	      /* Place nodes top down. */
	      geom.x1 = THIS(posX1);
	      geom.y1 = fromNode->info.y1;
	      geom.x2 = THIS(posX2);
	      geom.y2 = toNode->info.y2;
	    } else {
	      geom.x1 = fromNode->info.x2;
	      geom.y1 = THIS(posY1);
	      geom.x2 = toNode->info.x1;
	      geom.y2 = THIS(posY2);
	    }
	  } else {
	    if(fromNode->info.x1 >= toNode->info.x2) {
	      /* from is right from to */
	      if(THIS(graphOrder)) {
		/* Place nodes top down. */
		geom.x1 = THIS(posX1);
		geom.y1 = fromNode->info.y1;
		geom.x2 = THIS(posX2);
		geom.y2 = toNode->info.y2;
	      } else {
		geom.x1 = fromNode->info.x1;
		geom.y1 = THIS(posY1);
		geom.x2 = toNode->info.x2;
		geom.y2 = THIS(posY2);
	      }
	    } else {
	      /* from is at same level as to */
	      geom.x1 = THIS(posX1);
	      geom.y1 = fromNode->info.y1;
	      geom.x2 = THIS(posX2);
	      geom.y2 = toNode->info.y2;
	    }
	  }
	} else {
	  /* from is at same level as to */
	  if(fromNode->info.x2 <= toNode->info.x1) {
	    /* from is left from to */
	    geom.x1 = fromNode->info.x2;
	    geom.y1 = THIS(posY1);
	    geom.x2 = toNode->info.x1;
	    geom.y2 = THIS(posY2);
	  } else {
	    if(fromNode->info.x1 > toNode->info.x2) {
	      /* from is right from to */
	      geom.x1 = fromNode->info.x1;
	      geom.y1 = THIS(posY1);
	      geom.x2 = toNode->info.x2;
	      geom.y2 = THIS(posY2);
	    } else {
	      if(fromNode->info.x1 <= toNode->info.x1) {
		/* from is partially left from to */
		geom.x1 = fromNode->info.x2;
		geom.y1 = THIS(posY1);
		geom.x2 = toNode->info.x1;
		geom.y2 = THIS(posY2);
	      } else {
		/* from is partially right from to */
		geom.x1 = fromNode->info.x1;
		geom.y1 = THIS(posY1);
		geom.x2 = toNode->info.x2;
		geom.y2 = THIS(posY2);
	      }
	    }
	  }
	}
      }
    }
  }    

  /* Set new coordinates on Edge*/ 
  SET_EDGE_GEOM(e,geom);

  return result;
}

/*
 *--------------------------------------------------------------
 *
 * LayoutISI --
 *
 *	This procedure is invoked to place icons with ISI.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

int
LayoutISI AC1(Layout_Graph*,This)
{
  int counter, result = LAYOUT_OK;

  THIS(maxXPosition) = 0;
  THIS(maxYPosition) = 0;
  if(THIS(topList)) {
    /* free layout specific data slots. */
    for (counter = 0; counter < THIS(topListNum); counter++) {
      ckfree((char *) THIS(topList)[counter]);
    }
    ckfree((char*)THIS(topList));
    THIS(topList) = NULL;
  }
  THIS(topListNum) = 0;
  THIS(topList) = (Topology **) NULL;

  /* find the widest/highest edge. */
  LayoutEdgeWidth(This);

  /* build the internal graph structure. */
  if(LayoutBuildGraph(This) != LAYOUT_OK) {
    return LAYOUT_ERROR;
  }

  /* find root of the graph. */
  if((THIS(rootNode) = LayoutGraphRoot(This)) == NULL) {
    THIS(errmsg) = "no root node";
    return LAYOUT_ERROR;
  }

  /* sort the graph topological. */
  LayoutGraphSortTopological(This,THIS(rootNode));
  FOR_ALL_NODES(counter) {
    if(PARENT_NUM(THIS(nodes)[counter]) == 0) {
      LayoutGraphSortTopological(This,THIS(nodes)[counter]);
    }
  }

  /* Calculate the x position values. */
  RESET_VISITED_NODE(counter);
  FOR_ALL_NODES(counter) {
    if(PARENT_NUM(THIS(nodes)[counter]) == 0) {
      LayoutISISetX(This,THIS(nodes)[counter]);
    }
  }

#if 0
  RESET_VISITED_NODE(counter);
  FOR_ALL_NODES(counter) {
    if(PARENT_NUM(THIS(nodes)[counter]) == 0) {
      MY_LayoutISISetY(This,THIS(nodes)[counter], 0);
    }
  }
#else
  RESET_VISITED_NODE(counter);
  FOR_ALL_NODES(counter) {
    if(SUCC_NUM(THIS(nodes)[counter]) == 0) {
      LayoutISISetY(This,THIS(nodes)[counter]);
    }
  }
#endif

#if 1
  if (! THIS(graphOrder)) {
     while (1) {
	int found;
	found = 0;
  	FOR_ALL_NODES(counter) {
    	  if(PARENT_NUM (THIS(nodes)[counter]) > 0 &&
           NODE_Y_POS (THIS(nodes)[counter]) <
	   NODE_Y_POS (PARENT_NODE(THIS(nodes)[counter], 0)) +
	   NODE_WIDTH (PARENT_NODE(THIS(nodes)[counter], 0)) +
        	    THIS(edgeWidth) + THIS(iconSpaceH)) {
	   SET_NODE_Y_POS(THIS(nodes)[counter],
      
		     NODE_Y_POS (PARENT_NODE(THIS(nodes)[counter], 0)) +
	             NODE_WIDTH (PARENT_NODE(THIS(nodes)[counter], 0)) +
                     THIS(edgeWidth) + THIS(iconSpaceH));
	   found = 1;
          }
     
          if (SUCC_NUM (THIS(nodes)[counter]) == 1 &&
             PARENT_NODE (SUCC_NODE (THIS(nodes)[counter], 0), 0) == THIS(nodes)[counter]) {
     	     SET_NODE_X_POS(SUCC_NODE (THIS(nodes)[counter], 0),
			  NODE_X_POS(THIS(nodes)[counter]));
          }
       }
       if (! found)
         break;
     }
  }
#endif

  /* Place the graph items. */
  if(LayoutGraphPlaceNodes(This) != LAYOUT_OK) {
    result = LAYOUT_ERROR;
  } else if(LayoutGraphPlaceEdges(This) != LAYOUT_OK) {
    result = LAYOUT_ERROR;
  }
  return result;
}

/*
 *--------------------------------------------------------------
 *
 * LayoutMatrix --
 *
 *	This procedure is invoked to place icons as matrix.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

int
LayoutMatrix AC1(Layout_Graph*,This)
{
    int result = LAYOUT_OK, greatestX = 10,
      greatestY = 10, greatestHeight = 0, columnCounter = 0,
      tmpIconWidth = 0, offset = 0, counter;
    ItemGeom geom;

    /* Scan through all canvas items. */
    FOR_ALL_NODES(counter) {
	register Node* n = THIS(nodes)[counter];
	/* Find the greatest icon dimensions. */
	if(THIS(computeiconsize)) {
	    if(NODE_WIDTH(n) > THIS(iconWidth)) {
		THIS(iconWidth) = (int)NODE_WIDTH(n);
	    }
	    if(NODE_HEIGHT(n) > THIS(iconHeight)) {
		THIS(iconHeight) = (int)NODE_HEIGHT(n);
	    }
	}
    }

    /* Walk through the list of NODES */
    FOR_ALL_NODES(counter) {
	register Node* n = THIS(nodes)[counter];
	geom = NODE_GEOM(n);
	if(THIS(iconWidth) == 0) {
	    tmpIconWidth = (int)NODE_WIDTH(n);
	    offset = (int) ((NODE_WIDTH(n) / 2.0) - (NODE_WIDTH(n) / 2.0));
	} else {
	    tmpIconWidth = THIS(iconWidth);
	    offset = (int) ((THIS(iconWidth) / 2.0) - (NODE_WIDTH(n) / 2.0));
	}
	/* Is this the highest icon so far ? */
	if(NODE_HEIGHT(n) > greatestHeight) {
	    greatestHeight = (int) NODE_HEIGHT(n);
	}

	/* Place icon on the current line. */
	if(greatestX + tmpIconWidth < THIS(maxX) &&
	   (THIS(elementsPerLine) == 0 ||
	    columnCounter < THIS(elementsPerLine))) {
	    geom.x1 = offset + greatestX + THIS(xOffset);
	    geom.y1 = greatestY + THIS(yOffset);
	    geom.x2 = geom.x1 + geom.width;
	    geom.y2 = geom.y1 + geom.height;
	    greatestX += (tmpIconWidth + THIS(iconSpaceH));
	    columnCounter++;
	    SET_NODE_GEOM(n,geom);
	} else {
	    /* Place icon on the next line. */
	    if(THIS(iconHeight) > 0) {
		greatestY += (THIS(iconHeight) + THIS(iconSpaceV));
	    } else {
		greatestY += (greatestHeight + THIS(iconSpaceV));
	    }
	    geom.x1 = 10 + offset + greatestX + THIS(xOffset);
	    geom.y1 = greatestY + THIS(yOffset);
	    geom.x2 = geom.x1 + geom.width;
	    geom.y2 = geom.y1 + geom.height;
	    greatestHeight = (int) geom.height;
	    greatestX = 10 + (tmpIconWidth + THIS(iconSpaceH));
	    columnCounter = 1;
	    SET_NODE_GEOM(n,geom);
	}
    }

    if(THIS(hideEdges)) {
	/* make all edges zero length, and place at maxX,MaxY
	   to get them out of the way
	*/
	FOR_ALL_EDGES(counter) {
	    geom.x2 = (geom.x1 = THIS(maxX));
	    geom.y2 = (geom.y1 = THIS(maxY));
	    geom.width = (geom.height = 0);
	    SET_EDGE_GEOM(THIS(edges)[counter],geom);
	}
    } else if(LayoutGraphPlaceEdges(This) != LAYOUT_OK) {
	result = LAYOUT_ERROR;
    }
    return result;
}

/*
 *--------------------------------------------------------------
 *
 * LayoutRandom --
 *
 *	This procedure is invoked to place icons randomly.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

int
LayoutRandom AC1(Layout_Graph*,This)
{
    int result = LAYOUT_OK;
    int counter;

    SRANDOM(getpid() + time((time_t *) NULL));
    /* walk through all nodes */
    FOR_ALL_NODES(counter) {
	register Node* itemptr = THIS(nodes)[counter];
	long tmpx, tmpy;
	ItemGeom geom;

	/* randomly place icons. */
	/* are we allowed to place the icon ? */
	if(THIS(keepRandomPositions) &&
		NODE_X_POS(itemptr) > 0 &&
		NODE_Y_POS(itemptr) > 0) {
	  continue;
	}
	tmpx = (long) (RANDOM % THIS(maxX)) - (long) NODE_WIDTH(itemptr);
	tmpy = (long) (RANDOM % THIS(maxY)) - (long) NODE_HEIGHT(itemptr);
	if(tmpx <= 0) {
	  tmpx = 1;
	}
	if(tmpy <= 0) {
	  tmpy = 1;
	}
	geom = NODE_GEOM(itemptr);
	geom.x1 = tmpx + THIS(xOffset);
	geom.y1 = tmpy + THIS(yOffset);
	geom.x2 = geom.x1 + NODE_WIDTH(itemptr);
	geom.y2 = geom.y1 + NODE_HEIGHT(itemptr);
	SET_NODE_GEOM(itemptr,geom);
    }

    if(LayoutGraphPlaceEdges(This) != LAYOUT_OK) {
	result = LAYOUT_ERROR;
    }
    return result;
}

/*
 *--------------------------------------------------------------
 *
 * LayoutTree --
 *
 *	this procedure is invoked to place icons as tree.
 *
 * results:
 *	a standard tcl result.
 *
 * side effects:
 *	see the user documentation.
 *
 * 09nov95 wmt: add handling of -hideedges
 *--------------------------------------------------------------
 */

int
LayoutTree AC1(Layout_Graph*,This)
{
  int result = LAYOUT_OK, counter;
  ItemGeom geom;

  THIS(maxXPosition) = 0;
  THIS(maxYPosition) = 0;
  if(THIS(topList)) {
    /* free layout specific data slots. */
    for (counter = 0; counter < THIS(topListNum); counter++) {
      ckfree((char *) THIS(topList)[counter]);
    }
    ckfree((char*)THIS(topList));
  }
  THIS(topListNum) = 0;
  THIS(topList) = (Topology **) NULL;

  /* find the widest/highest edge. */
  LayoutEdgeWidth(This);

  /* build the internal graph structure. */
  if(LayoutBuildGraph(This) != LAYOUT_OK) {
    return LAYOUT_ERROR;
  }

  /* find root of the graph. */
  if((THIS(rootNode) = LayoutGraphRoot(This)) == NULL) {
    THIS(errmsg) = "no root node";
    return LAYOUT_ERROR;
  }
  
  /* sort the graph topological. */
  LayoutGraphSortTopological(This,THIS(rootNode));
  FOR_ALL_NODES(counter) {
    if(PARENT_NUM(THIS(nodes)[counter]) == 0) {
      LayoutGraphSortTopological(This,THIS(nodes)[counter]);
    }
  }

  /* calculate the position values. */
  RESET_VISITED_NODE(counter);
  LayoutTreeSetX(This,THIS(rootNode));
  FOR_ALL_NODES(counter) {
    if(PARENT_NUM(THIS(nodes)[counter]) == 0) {
      LayoutTreeSetX(This,THIS(nodes)[counter]);
    }
  }
  RESET_VISITED_NODE(counter);
  FOR_ALL_TOP_NODES(counter) {
    LayoutTreeSetY(This,TOP_NODE(counter));
  }

  /* place the graph items. */
  if(LayoutGraphPlaceNodes(This) != LAYOUT_OK) {
    result = LAYOUT_ERROR;
  }
  if(THIS(hideEdges)) {
    /* make all edges zero length, and place at maxX,MaxY
       to get them out of the way
       */
    FOR_ALL_EDGES(counter) {
      geom.x2 = (geom.x1 = THIS(maxX));
      geom.y2 = (geom.y1 = THIS(maxY));
      geom.width = (geom.height = 0);
      SET_EDGE_GEOM(THIS(edges)[counter],geom);
    }
  } else if(LayoutGraphPlaceEdges(This) != LAYOUT_OK) {
    result = LAYOUT_ERROR;
  }
  return result;
}

Layout_Graph*
LayoutCreateGraph()
{
    register Layout_Graph* This;

    This = (Layout_Graph*)ckalloc(sizeof(Layout_Graph));
    if(!This) return This;
#if 1 /* multiX */
    memset((char *)This,0,sizeof(Layout_Graph));
#endif
    /* Initialize global data. */
    THIS(graphOrder) = 0;
    THIS(iconSpaceH) = 5;
    THIS(iconSpaceV) = 5;
    THIS(xOffset) = 4;
    THIS(yOffset) = 4;
    THIS(maxX) = 0;
    THIS(maxY) = 0;
    THIS(rootNode) = NULL;

    THIS(keepRandomPositions) = 0;
    THIS(nodeNum) = 0;
    THIS(nodes) = NULL;
    THIS(edgeNum) = 0;
    THIS(edges) = NULL;
    THIS(topListNum) = 0;
    THIS(topList) = NULL;
    THIS(computeiconsize) = 0;
    THIS(elementsPerLine) = 0;
    THIS(hideEdges) = 0;
    THIS(edgeHeight) = 2;
    THIS(edgeOrder) = 0;
    THIS(edgeWidth) = 0;
    THIS(iconWidth) = 0;
    THIS(iconHeight) = 0;
    THIS(posX1) = 0;
    THIS(posY1) = 0;
    THIS(posX2) = 0;
    THIS(posY2) = 0;
	THIS(gridlock) = 0;
#if 0
    THIS(idlist) = NULL;
#endif
    THIS(maxXPosition) = 0.0;
    THIS(maxYPosition) = 0.0;
#if 1
    THIS(layoutTypesNum) = 0;
#else
    THIS(layoutTypesNum) = 1;
    THIS(layoutTypes) = (char **) ckalloc(2);
    THIS(layoutTypes) = (char **) ckalloc(10);
    *THIS(layoutTypes) = '\0';
    *(1+THIS(layoutTypes))) = '\0';
    *THIS(layoutTypes) = (char *) ckalloc(10);
    strcpy(*THIS(layoutTypes), "icon");
#endif
    THIS(errmsg) = (char*)NULL;
    return This;
}

LayoutConfig
GetLayoutConfig(This)
	struct Layout_Graph* This;
{
    LayoutConfig c;
    c.rootnode = THIS(rootNode)?NODE_ITEM(THIS(rootNode)):NULL;
    c.graphorder = THIS(graphOrder);
    c.nodespaceH = (long)THIS(iconSpaceH);
    c.nodespaceV = (long)THIS(iconSpaceV);
    c.xoffset = THIS(xOffset);
    c.yoffset = THIS(yOffset);
    c.computenodesize = THIS(computeiconsize);
    c.elementsperline = THIS(elementsPerLine);
    c.hideedges = THIS(hideEdges);
    c.keeprandompositions = THIS(keepRandomPositions);
    c.maxx = THIS(maxX);
    c.maxy = THIS(maxY);
    c.gridlock = THIS(gridlock);
    return c;
}

void
SetLayoutConfig(This,c)
	struct Layout_Graph* This;
	LayoutConfig c;
{
    register int i;
    THIS(graphOrder) = c.graphorder;
    THIS(iconSpaceH) = c.nodespaceH;
    THIS(iconSpaceV) = c.nodespaceV;
    THIS(xOffset) = c.xoffset;
    THIS(yOffset) = c.yoffset;
    THIS(computeiconsize) = c.computenodesize;
    THIS(elementsPerLine) = c.elementsperline;
    THIS(hideEdges) = c.hideedges;
    THIS(keepRandomPositions) = c.keeprandompositions;
    THIS(maxX) = c.maxx;
    THIS(maxY) = c.maxy;
    THIS(gridlock) = c.gridlock;

    /* rootNode needs special work */
    if(c.rootnode) {
	FOR_ALL_NODES(i) {
	    if(NODE_ITEM(THIS(nodes)[i]) == c.rootnode) {
	        THIS(rootNode) = THIS(nodes)[i];
	    }
	}
    }
}

int
LayoutGetIthNode(This,index,idp)
	struct Layout_Graph* This;
	long index;
	pItem* idp;
{
    if(index < 0 || index >= THIS(nodeNum)) return LAYOUT_ERROR;
    *idp = NODE_ITEM(THIS(nodes)[index]);
    return LAYOUT_OK;
}

int
LayoutGetNodeBBox(This,id,geomp)
	struct Layout_Graph* This;
	pItem id;
	ItemGeom* geomp;
{
    register Node* ip = NULL;
    register int i;

    /* find matching node */
    FOR_ALL_NODES(i) {
		if(NODE_ITEM(THIS(nodes)[i]) == id) {
	   		ip = THIS(nodes)[i];
			break; /* Khamis 11-oct-96 */
		}
    }
    if(!ip) return LAYOUT_ERROR;
    *geomp = NODE_GEOM(ip);
    return LAYOUT_OK;
}

int
LayoutSetNodeBBox(This,id,geom)
	struct Layout_Graph* This;
	pItem id;
	ItemGeom geom;
{
    register Node* ip = NULL;
    register int i;

    /* find matching node */
    FOR_ALL_NODES(i) {
	if(NODE_ITEM(THIS(nodes)[i]) == id) {
	    ip = THIS(nodes)[i];
	    break; /* Khamis 11-oct-96 */
	}
    }
    if(!ip) return LAYOUT_ERROR;
    if(!DUMMY_NODE(ip)) {
	SET_NODE_GEOM(ip,geom);
	SET_NODE_HEIGHT(ip, CALC_NODE_HEIGHT(ip));
	SET_NODE_WIDTH(ip, CALC_NODE_WIDTH(ip));
    } else {
	SET_NODE_HEIGHT(ip, 1);
	SET_NODE_WIDTH(ip, 1);
    }
    return LAYOUT_OK;
}

int
LayoutGetIthEdge(This,index,idp)
	struct Layout_Graph* This;
	long index;
	pItem* idp;
{
    if(index < 0 || index >= THIS(edgeNum)) return LAYOUT_ERROR;
    *idp = EDGE_ITEM(THIS(edges)[index]);
    return LAYOUT_OK;
}

int
LayoutGetEdgeEndPoints(This,id,geomp)
	struct Layout_Graph* This;
	pItem id;
	ItemGeom* geomp;
{
    register Edge* ip = NULL;
    register int i;

    /* find matching edge */
    FOR_ALL_EDGES(i) {
		if(EDGE_ITEM(THIS(edges)[i]) == id) {
		    ip = THIS(edges)[i];
		    break; /* Khamis 11-oct-96 */
		}
    }
    if(!ip) return LAYOUT_ERROR;
    *geomp = EDGE_GEOM(ip);
    return LAYOUT_OK;
}

int
LayoutSetEdgeDim(This,id,geom)
	struct Layout_Graph* This;
	pItem id;
	ItemGeom geom;
{
    register Edge* ip = NULL;
    register int i;

    /* find matching edge */
    FOR_ALL_EDGES(i) {
	if(EDGE_ITEM(THIS(edges)[i]) == id) {
	    ip = THIS(edges)[i];
	    break; /* Khamis 11-oct-96 */
	}
    }
    if(!ip) return LAYOUT_ERROR;
    SET_EDGE_GEOM(ip,geom);
    return LAYOUT_OK;
}

char*
LayoutGetError(This)
	struct Layout_Graph* This;
{
    register char* msg = THIS(errmsg);
    THIS(errmsg) = (char*)0;
    return msg;
}


/* KHAMIS */

void * MY_EdgeFromNode (This, i)
	struct Layout_Graph* This;
	int i;
{
	return THIS(edges)[i]->fromNode;
}

void * MY_EdgeToNode (This, i)
	struct Layout_Graph* This;
	int i;
{
	return THIS(edges)[i]->toNode;
}

int MY_EdgeParentNum (This, i)
	struct Layout_Graph* This;
	int i;
{
	return THIS(edges)[i]->toNode->parentNum;
}

void * MY_EdgeParent (This, i, num)
	struct Layout_Graph* This;
	int i;
	int num;
{
	return THIS(edges)[i]->toNode->parent[num]->fromNode;
}

int MY_EdgeSuccNum (This, i)
	struct Layout_Graph* This;
	int i;
{
	return THIS(edges)[i]->fromNode->succNum;
}

void * MY_EdgeSucc (This, i, num)
	struct Layout_Graph* This;
	int i;
{
	return THIS(edges)[i]->fromNode->succ[num]->toNode;
}

int MY_graphOrder (This)
	struct Layout_Graph* This;
{
	return THIS(graphOrder);
}

/* KHAMIS END */

