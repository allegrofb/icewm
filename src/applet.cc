#include "config.h"
#include "ywindow.h"
#include "applet.h"
#include "yxapp.h"
#include "default.h"
#include "intl.h"

Picturer::~Picturer()
{
}

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, void *data)
{
    MSG((_("draw_cb: %d %d"),
        0,0));    
    cairo_surface_t* surface = (cairo_surface_t*)data;
    cairo_set_source_surface (cr, surface, 0, 0);
    cairo_paint (cr);
    return TRUE;
}

IApplet::IApplet(Picturer *picturer, YWindow *parent) :
    YWindow(1),
    isVisible(false),
    fPicturer(picturer),
    fPixmap(None)
{
    addStyle(wsNoExpose);
    addEventMask(VisibilityChangeMask);
    g_signal_connect(getWidget(), "draw", G_CALLBACK(draw_cb), getKPixmap());
}

IApplet::~IApplet()
{
    freePixmap();
    freeKPixmap();
}

void IApplet::freePixmap()
{
    if (fPixmap) {
        XFreePixmap(xapp->display(), fPixmap);
        fPixmap = None;
    }
}

void IApplet::freeKPixmap()
{
    if (fKPixmap) {
        cairo_surface_destroy(fKPixmap);
        fKPixmap = None;
    }
}

void IApplet::handleVisibility(const XVisibilityEvent& visib)
{
    bool prev = isVisible;
    isVisible = (visib.state != VisibilityFullyObscured);
    if (prev < isVisible)
        repaint();
}

void IApplet::repaint()
{
    MSG((_("IApplet::repaint: %d %d"),
        isVisible, visible()));

    if (isVisible && visible() && fPicturer->k_picture())
        showPixmap();
}

Drawable IApplet::getPixmap()
{
    if (fPixmap == None)
        fPixmap = createPixmap();
    return fPixmap;
}

cairo_surface_t* IApplet::getKPixmap()
{
    if (fKPixmap == None)
        fKPixmap = createKPixmap();
    return fKPixmap;
}

void IApplet::showPixmap() {
    MSG(("IApplet::showPixmap"));

    Graphics g(fKPixmap, width(), height(), depth());
    // Graphics g(fPixmap, width(), height(), depth());
    paint(g, YRect(0, 0, width(), height()));
    gtk_widget_queue_draw (getWidget());     //hyjiang
    // setBackgroundPixmap(fPixmap);
    // clearWindow();


}

void IApplet::configure(const YRect2& r) {
    if (r.resized())
        freePixmap();
    if (None == fPixmap && 1 < r.width() && 1 < r.height())
        repaint();
}

