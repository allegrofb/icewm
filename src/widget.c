/* widget.c */

#include "widget.h"

/* Private data structure */
struct _MyWidgetPrivate {
   GdkWindow *window;
};

/* Internal API */
static void my_cpu_size_allocate(GtkWidget *widget, 
    GtkAllocation *allocation);
static void my_cpu_realize(GtkWidget *widget);
static gboolean my_cpu_expose(GtkWidget *widget, 
    cairo_t          *cr);

static void my_cpu_get_preferred_height(GtkWidget *widget,
                                        gint *minimum_height,
                                        gint *natural_height);
static void my_cpu_get_preferred_width(GtkWidget *widget,
                                       gint *minimum_width,
                                       gint *natural_width);

/* Define type */
G_DEFINE_TYPE(MyWidget, my_widget, GTK_TYPE_WIDGET)

/* Initialization */
static void my_widget_class_init(MyWidgetClass *klass) {
    
   GObjectClass *g_class;
   GtkWidgetClass *w_class;
   GParamSpec *pspec;

   g_class = G_OBJECT_CLASS(klass);
   w_class = GTK_WIDGET_CLASS(klass);

   w_class->realize       = my_cpu_realize;
   // w_class->get_preferred_width  = my_cpu_get_preferred_width;
   // w_class->get_preferred_height  = my_cpu_get_preferred_height;
   // w_class->size_allocate = my_cpu_size_allocate;
   // w_class->scroll_event  = my_cpu_expose;
   w_class->draw = my_cpu_expose;
   

   /* Install property */
   // pspec = g_param_spec_double("percent", "Percent", 
   //     "What CPU load should be displayed", 0, 1, 0, 
   //     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
       
   // g_object_class_install_property(g_class, P_PERCENT, pspec);

   /* Add private data */
   g_type_class_add_private(g_class, sizeof(MyWidgetPrivate));
}

static void my_widget_init(MyWidget *cpu) {
    
   MyWidgetPrivate *priv;
   
   priv = G_TYPE_INSTANCE_GET_PRIVATE(cpu, MY_TYPE_WIDGET, MyWidgetPrivate);

   gtk_widget_set_has_window(GTK_WIDGET(cpu), TRUE);

   /* Set default values */
   // priv->percent = 0;

   /* Create cache for faster access */
   cpu->priv = priv;
}

/* Overriden virtual methods */
static void my_cpu_set_property(GObject *object, guint prop_id,
    const GValue *value, GParamSpec *pspec) {
        
   // MyWidget *cpu = MY_WIDGET(object);

   // switch(prop_id) {
       
   //    case P_PERCENT:
      
   //       my_cpu_set_percent(cpu, g_value_get_double(value));
   //       break;

   //    default:
      
   //       G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   //       break;
   // }
}

static void my_cpu_get_property(GObject *object, guint prop_id,
                GValue *value, GParamSpec *pspec) {
                    
   MyWidget *cpu = MY_WIDGET(object);

   // switch(prop_id) {
       
   //    case P_PERCENT:
   //       g_value_set_double(value, cpu->priv->percent);
   //       break;

   //    default:
   //       G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   //       break;
   // }
}

static void my_cpu_realize(GtkWidget *widget) {
    
   MyWidgetPrivate *priv = MY_WIDGET(widget)->priv;
   GtkAllocation alloc;
   GdkWindowAttr attrs;
   guint attrs_mask;

   gtk_widget_set_realized(widget, TRUE);

   gtk_widget_get_allocation(widget, &alloc);

   attrs.x           = alloc.x;
   attrs.y           = alloc.y;
   attrs.width       = alloc.width;
   attrs.height      = alloc.height;
   attrs.window_type = GDK_WINDOW_CHILD;
   attrs.wclass      = GDK_INPUT_OUTPUT;
   attrs.event_mask  = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK;

   attrs_mask = GDK_WA_X | GDK_WA_Y;

   priv->window = gdk_window_new(gtk_widget_get_parent_window(widget),
               &attrs, attrs_mask);
   gdk_window_set_user_data(priv->window, widget);
   gtk_widget_set_window(widget, priv->window);

   // widget->style = gtk_style_attach(gtk_widget_get_style( widget ),
   //                           priv->window);
   // gtk_style_set_background(widget->style, priv->window, GTK_STATE_NORMAL);
}

static void my_cpu_size_allocate(GtkWidget *widget,
                 GtkAllocation *allocation) {
                     
   MyWidgetPrivate *priv;

   priv = MY_WIDGET(widget)->priv;

   gtk_widget_set_allocation(widget, allocation);

   if (gtk_widget_get_realized(widget)) {
       
      gdk_window_move_resize(priv->window, allocation->x, allocation->y,
          allocation->width, allocation->height);
   }
}


static gboolean my_cpu_expose(GtkWidget *widget, 
    cairo_t          *cr) {
                
   MyWidgetPrivate *priv = MY_WIDGET(widget)->priv;
   gint limit;
   gint i;

   cairo_translate(cr, 0, 7);

   cairo_set_source_rgb(cr, 0, 0, 0);
   cairo_paint(cr);

   // limit = 20 - priv->percent / 5;
   
   for (i = 1; i <= 20; i++) {
       
      if (i > limit) {
         cairo_set_source_rgb(cr, 0.6, 1.0, 0);
      } else {
         cairo_set_source_rgb(cr, 0.2, 0.4, 0);
      }

      cairo_rectangle(cr, 8,  i * 4, 30, 3);
      cairo_rectangle(cr, 42, i * 4, 30, 3);
      cairo_fill(cr);
   }

   return TRUE;
}

/* Public API */
GtkWidget *my_widget_new(void) {
    
   return(g_object_new(MY_TYPE_WIDGET, NULL));
}

