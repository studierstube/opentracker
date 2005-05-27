/**
 * @defgroup stbapi Studierstube API
 */
/**
 * @defgroup apps Applications
 */
/**
 * @defgroup extensions Extensions
 *
 * Libraries and nodes in this group are extensions that incorporate
 * functionality from other libraries. They are typically encapsulated
 * into OIV nodes or extensions of Studierstube core interfaces. They
 * are implemented as dynamically loadable modules and can be initialized
 * with the @ref SoClassLoader node.
 *
 * See the @ref extensionguide for details on how to develop extensions.
 */
/**
 * @defgroup tools Tools
 */
/**
 * @defgroup core Core Classes
 * @ingroup stbapi
 * The most important Studierstube classes. This is a good starting point for beginners.
 */
/**
 * @defgroup workspace Workspace & application support
 * @ingroup stbapi
 */
/**
 * @defgroup camera Cameras & rendering
 * @ingroup stbapi
 */
/**
 * @defgroup oldwidgets 3D Widgets (old)
 * @ingroup stbapi
 */
/**
 * @defgroup widgets 3D Widgets
 * @ingroup stbapi
 */
/**
 * @defgroup interaction Interaction
 * @ingroup stbapi
 */
/**
 * @defgroup event 3D Event Handling
 * @ingroup stbapi
 */
/**
 * @defgroup util OpenInventor Utility Classes
 * @ingroup stbapi
 */
/**
 * @defgroup tracking Tracking
 * @ingroup stbapi
 */
/**
 * @defgroup sman Distributed Application Management
 * @ingroup stbapi
 */
/**
 * @defgroup div Distributed Inventor
 * @ingroup stbapi
 */
/**
 * @defgroup context Context-sensitive scenegraph traversal
 * @ingroup stbapi
 *
 * The context sensitive scene graph traversal makes scene graphs parametrizable similar
 * to functions in an ordinary programming language. The context is an additional element 
 * implemented in SoContextElement that is used during all actions. It is modelled as a simple
 * string to string mapping where SbNames are used as keys for performance reasons. 
 *
 * A pair of nodes, SoContext and SoContextReport, allows to set and retrieve the context. 
 * Additionally dedicated SoContextSwitch and SoContextMultiSwitch nodes provide some shortcuts,
 * if you simply want to switch between different scene graphs during traversals. Here are some
 * examples of using context sensitive scene graphs.
 *
 * A second pair of nodes, SoNodeContext and SoNodeContextReport, allows to use nodes and
 * their subgraphs with the context sensitive traversal. SoNodeContext adds one or more
 * node references to the context and SoNodeContextReport either traverses and/or
 * reports these references through a field.
 *
 * A simple graph that allows to set the color of the geometry:
 @code
DEF Graph Separator {
    DEF color SoContextReport {
        index "color"
    }
    Material { 
        diffuseColor = USE color.value
    }
    Sphere {}
}

# set color to green and traverse graph
SoContext {
    index "color"
    value "0 1 0"
}
USE Graph

# set color to red and traverse again
SoContext {
    index "color"
    value "1 0 0"
}
USE Graph
 @endcode
 *
 * The above example has the disadvantage that the permanent change of the node named 'color' will 
 * retrigger render traversals. To avoid passing the notifications which are created during traversals
 * upwards, use a SoContextSeparator node. Here is the same example with the separator node:
 @code
DEF Graph SoContextSeparator { # the separator will avoid constant retriggering
    DEF color SoContextReport {
        index "color"
    }
    Material { 
        diffuseColor = USE color.value
    }
    Sphere {}
}

# set color to green and traverse graph
SoContext {
    index "color"
    value "0 1 0"
}
USE Graph
  
# set color to red and traverse again
SoContext {
    index "color"
    value "1 0 0"
}
USE Graph
 @endcode
 *
 * Note, that if you use context sensitive nodes as parts of a nodekit, the fields containing the parts
 * also propagate notifications. The simplest solution is to turn of notification for these fields and
 * also fields containing parent nodes within the nodekit structure using SoField::enableNotify().
 *
 * Another example shows how to use the SoContextSwitch. It will render only one child from 
 * its list of children depending on the context value:
 *
 @code
DEF Switch SoContextSwitch {
    index "number"
    defaultChild 1          # this child is traversed if the index is not present
    Cone {}
    Sphere {}
}
 @endcode
 *
 * The last example shows the use of SoNodeContext and SoNodeContextReport. A
 * subgraph set with the SoNodeContext node is traversed from the report node. 
 * The first instance will create gray sphere, while the second creates a 
 * moved red sphere.
 *
 @code
DEF Test Separator {
    SoNodeContextReport {  
        index "test"       # traverse any node stored in "test"
        traverse TRUE
        report FALSE
    }
    Sphere { radius 0.5 }
}
SoNodeContext {           # sets the node in "test" to a group with transform and color
    index "test"      
    value Group {
            Transform { translation 1 0 0 }                    
            Material { diffuseColor 1 0 0 }
          }
}
USE Test                  # traverse Test again to have the effects of the above node
@endcode
 */
/**
 * @defgroup deprecated Deprecated Classes
 * Classes in this group should not be used anymore. Their functionality and
 * API is deprecated and superceded by new developments. See the individual
 * classes for details. Please try to not use these classes in new developments
 * and port any old code to the substitutions, if this is feasible. They may be removed
 * at any point without further notice.
 */
/**
 * @defgroup video Video background support
 * @ingroup stbapi
 * This group contains the interface and support classes for enabling and
 * implementing video backgrounds.
 */