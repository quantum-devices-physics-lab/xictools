
/*========================================================================*
 *                                                                        *
 *  Distributed by Whiteley Research Inc., Sunnyvale, California, USA     *
 *                       http://wrcad.com                                 *
 *  Copyright (C) 2017 Whiteley Research Inc., all rights reserved.       *
 *  Author: Stephen R. Whiteley, except as indicated.                     *
 *                                                                        *
 *  As fully as possible recognizing licensing terms and conditions       *
 *  imposed by earlier work from which this work was derived, if any,     *
 *  this work is released under the Apache License, Version 2.0 (the      *
 *  "License").  You may not use this file except in compliance with      *
 *  the License, and compliance with inherited licenses which are         *
 *  specified in a sub-header below this one if applicable.  A copy       *
 *  of the License is provided with this distribution, or you may         *
 *  obtain a copy of the License at                                       *
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
 * Xic Integrated Circuit Layout and Schematic Editor                     *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

#include "config.h"
#include "main.h"
#include "edit.h"
#include "undolist.h"
#include "cd_hypertext.h"
#include "dsp_inlines.h"
#include "layertab.h"
#include "errorlog.h"
#include "select.h"


//
// Command to change layer of objects.
//

// Transfer selected geometry to the current layer.
//
void
cEdit::changeLayer()
{
    CDs *cursd = CurCell();
    if (!cursd)
        return;
    CDol *st = Selections.listQueue(cursd);
    CDmergeInhibit inh(st);
    CDol::destroy(st);

    CDl *new_layer = LT()->CurLayer();
    sSelGen sg(Selections, cursd, "bpwl");
    CDo *od;
    while ((od = sg.next()) != 0) {
        if (change_layer(od, cursd, new_layer, true, true, true))
            sg.remove();
        else {
            Errs()->add_error("change_layer failed");
            Log()->ErrorLog(mh::EditOperation, Errs()->get_error());
        }
    }
}


// Private function for layer changing.
//
bool
cEdit::change_layer(CDo *odesc, CDs* sdesc, CDl *new_layer, bool move,
    bool undoable, bool use_merge)
{
    if (!odesc)
        return (false);
#ifdef HAVE_COMPUTED_GOTO
    COMPUTED_JUMP(odesc)
#else
    CONDITIONAL_JUMP(odesc)
#endif
box:
    {
        CDo *newo;
        if (sdesc->makeBox(new_layer, &odesc->oBB(), &newo) != CDok) {
            Errs()->add_error("makeBox failed");
            return (false);
        }
        if (odesc->prpty_list())
            newo->prptyAddCopyList(odesc->prpty_list());
        if (undoable)
            Ulist()->RecordObjectChange(sdesc, move ? odesc : 0, newo);
        else if (move)
            sdesc->unlink(odesc, false);
        if (use_merge && !sdesc->mergeBoxOrPoly(newo, undoable)) {
            Errs()->add_error("mergeBoxOrPoly failed");
            Log()->ErrorLog(mh::ObjectCreation, Errs()->get_error());
        }
        return (true);
    }
poly:
    {
        int num = ((const CDpo*)odesc)->numpts();
        Poly po(num, Point::dup(((const CDpo*)odesc)->points(), num));
        CDpo *newo;
        if (sdesc->makePolygon(new_layer, &po, &newo) != CDok) {
            Errs()->add_error("makePolygon failed");
            return (false);
        }
        if (odesc->prpty_list())
            newo->prptyAddCopyList(odesc->prpty_list());
        if (undoable)
            Ulist()->RecordObjectChange(sdesc, move ? odesc : 0, newo);
        else if (move)
            sdesc->unlink(odesc, false);
        if (use_merge && !sdesc->mergeBoxOrPoly(newo, undoable)) {
            Errs()->add_error("mergeBoxOrPoly failed");
            Log()->ErrorLog(mh::ObjectCreation, Errs()->get_error());
        }
        return (true);
    }
wire:
    {
        int num = ((const CDw*)odesc)->numpts();
        Wire wire(num, Point::dup(((const CDw*)odesc)->points(), num),
            ((const CDw*)odesc)->attributes());
        CDw *newo;
        if (sdesc->makeWire(new_layer, &wire, &newo) != CDok) {
            Errs()->add_error("makeWire failed");
            return (false);
        }
        if (odesc->prpty_list())
            newo->prptyAddCopyList(odesc->prpty_list());
        if (undoable)
            Ulist()->RecordObjectChange(sdesc, move ? odesc : 0, newo);
        else if (move)
            sdesc->unlink(odesc, false);
        if (use_merge && !sdesc->mergeWire(newo, undoable)) {
            Errs()->add_error("mergeWire failed");
            Log()->ErrorLog(mh::ObjectCreation, Errs()->get_error());
        }
        return (true);
    }
label:
    {
        CDla *newo;
        Label label(((CDla*)odesc)->la_label());
        label.label = hyList::dup(label.label);
        if (sdesc->makeLabel(new_layer, &label, &newo) != CDok) {
            Errs()->add_error("makeLabel failed");
            return (false);
        }
        if (odesc->prpty_list())
            newo->prptyAddCopyList(odesc->prpty_list());
        if (undoable)
            Ulist()->RecordObjectChange(sdesc, move ? odesc : 0, newo);
        else if (move)
            sdesc->unlink(odesc, false);
        if (undoable) {
            if (DSP()->CurMode() == Electrical && move)
                sdesc->prptyLabelUpdate(newo, (CDla*)odesc);
        }
        return (true);
    }
inst:
    Errs()->add_error("Can't change the layer of an instance.");
    return (false);
}

