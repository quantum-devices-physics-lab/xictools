
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
 * WRspice Circuit Simulation and Analysis Tool:  Device Library          *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1985 Thomas L. Quarles
         1993 Stephen R. Whiteley
****************************************************************************/

#include "resdefs.h"


int 
RESdev::askModl(const sGENmodel *genmod, int which, IFdata *data)
{
    const sRESmodel *model = static_cast<const sRESmodel*>(genmod);
    IFvalue *value = &data->v;
    // Need to override this for non-real returns.
    data->type = IF_REAL;

    switch (which) {
    case RES_MOD_RSH:
        value->rValue = model->RESsheetRes;
        break;
    case RES_MOD_NARROW: 
        value->rValue = model->RESnarrow;
        break;
    case RES_MOD_DL:
        value->rValue = model->RESshorten;
        break;
    case RES_MOD_TC1:
        value->rValue = model->REStempCoeff1;
        break;
    case RES_MOD_TC2:
        value->rValue = model->REStempCoeff2;
        break;
    case RES_MOD_DEFWIDTH:
        value->rValue = model->RESdefWidth;
        break;
    case RES_MOD_DEFLENGTH:
        value->rValue = model->RESdefLength;
        break;
    case RES_MOD_TNOM:
        value->rValue = model->REStnom-CONSTCtoK;
        break;
    case RES_MOD_TEMP:
        value->rValue = model->REStemp-CONSTCtoK;
        break;
    case RES_MOD_NOISE:
        value->rValue = model->RESnoise;
        break;
    case RES_MOD_KF:
        value->rValue = model->RESkf;
        break;
    case RES_MOD_AF:
        value->rValue = model->RESaf;
        break;
    case RES_MOD_EF:
        value->rValue = model->RESef;
        break;
    case RES_MOD_WF:
        value->rValue = model->RESwf;
        break;
    case RES_MOD_LF:
        value->rValue = model->RESlf;
        break;
    default:
        return (E_BADPARM);
    }
    return (OK);
}

