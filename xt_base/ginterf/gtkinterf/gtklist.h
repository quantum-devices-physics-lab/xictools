
/*========================================================================*
 *                                                                        *
 *  Whiteley Research Inc, Sunnyvale, CA USA, http://wrcad.com            *
 *  Copyright (C) 2017 Whiteley Research Inc, all rights reserved.        *
 *                                                                        *
 *  Licensed under the Apache License, Version 2.0 (the "License");       *
 *  you may not use this file except in compliance with the License.      *
 *  You may obtain a copy of the License at                               *
 *                                                                        *
 *        http://www.apache.org/licenses/LICENSE-2.0                      *
 *                                                                        *
 *  See the License for the specific language governing permissions       *
 *  and limitations under the License.                                    *
 *                                                                        *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      *
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-        *
 *   INFRINGEMENT.  IN NO EVENT SHALL WHITELEY RESEARCH INCORPORATED      *
 *   OR STEPHEN R. WHITELEY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER     *
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,      *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE       *
 *   USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                        *
 *========================================================================*
 *               XicTools Integrated Circuit Design System                *
 *                                                                        *
 * GtkInterf Graphical Interface Library                                  *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

#ifndef GTKLIST_H
#define GTKLIST_H

namespace gtkinterf {
    struct GTKlistPopup : public GRlistPopup, public gtk_bag
    {
        GTKlistPopup(gtk_bag*, stringlist*, const char*, const char*,
            bool, bool, void*);
        ~GTKlistPopup();

        GtkWidget *list()           { return (ls_list); }

        // GRpopup overrides
        void set_visible(bool visib)
            {
                if (visib)
                    gtk_widget_show(wb_shell);
                else
                    gtk_widget_hide(wb_shell);
            }
        void popdown();

        // GRlistPopup override
        void update(stringlist*, const char*, const char*);
        void update(bool(*)(const char*));
        void unselect_all();

    private:
        void selection_handler(int);

        // GTK signal handlers
        static void ls_quit_proc(GtkWidget*, void*);
        static void ls_apply_proc(GtkWidget*, void*);
        static bool ls_selection_proc(GtkTreeSelection*, GtkTreeModel*,
            GtkTreePath*, bool, void*);
        static bool ls_focus_proc(GtkWidget*, GdkEvent*, void*);

        GtkWidget *ls_list;         // list widget
        GdkPixbuf *ls_open_pb;      // GRonecolPx pixmaps
        GdkPixbuf *ls_close_pb;

        int ls_row;                 // selected row;
        bool ls_no_select;          // treeview focus hack
        bool ls_use_pix;            // display using pixmaps
        bool ls_use_apply;          // use Apply button
    };
}

#endif

