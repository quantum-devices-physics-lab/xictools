
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
 * WRspice Circuit Simulation and Analysis Tool                           *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

/***************************************************************************
JSPICE3 adaptation of Spice3e2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1992 Stephen R. Whiteley
****************************************************************************/

#ifndef OUTDATA_H
#define OUTDATA_H

#include <stdio.h>


// references
struct sCKT;
struct sJOB;
struct sPlot;
struct sFtCirc;
struct sRunDesc;
struct sOUTdata;
struct wordlist;
struct sDCTprms;
struct sNames;
struct sHtab;

enum OutcType
{
    OutcNormal,     // normal analysis
    OutcCheck,      // margin analysis, save only current point
    OutcCheckSeg,   // margin analysis, save points for current analysis step
    OutcCheckMulti, // margin analysis, save all data
    OutcLoop        // loop command
};

// Base control struct for operating range, Monte Carlo and loop
// analysis.
//
struct sOUTcontrol
{
    sOUTcontrol()
        {
            out_mode       = OutcNormal;
            out_rundesc    = 0;
            out_cir        = 0;
            out_plot       = 0;
        }

    // check.cc
    ~sOUTcontrol();

    OutcType out_mode;          // Control mode for analysis output.
    sRunDesc *out_rundesc;      // Pointer to run desc.
    sFtCirc  *out_cir;          // Pointer to circuit.
    sPlot    *out_plot;         // Pointer to plot.
};

// Interface for asynchronous/remote chained analysis.  This structure
// is used to store parameters used in operating range and Monte Carlo
// analysis.
//
struct sCHECKprms : public sOUTcontrol
{

    // check.cc
    sCHECKprms(bool, bool, bool, bool);
    ~sCHECKprms();

    int parseRng(wordlist**);
    int setup(bool, bool, bool, bool);
    void set_input(double, double);
    void evaluate();
    void findEdge(const char*, const char*);
    void run();
    bool findRange();

    // aspice.cc
    void registerJob();
    void endJob();
    bool processReturn(const char*);
    int nextTask(int*, int*);

    static void setMfilePlotname(const char*, const char*);
    static const char *mfilePlotname(const char*);

    void set_cmd(wordlist *w)   { ch_cmdline = w; }

    int index()                 { return (ch_index); }
    void set_index(int i)       { ch_index = i; }
    int max_index()             { return (ch_max); }

    int cycles()                { return (ch_cycles); }
    const double *points()      { return (ch_points); }
    const char *segbase()       { return (ch_segbase); }

    FILE *outfp()               { return (ch_op); }
    bool ended()                { return (!ch_use_remote && !ch_pause); }

    bool nogo()                 { return (ch_nogo); }
    void set_nogo(bool b)       { ch_nogo = b; }
    bool failed()               { return (ch_fail); }
    void set_failed(bool b)     { ch_fail = b; }

    bool monte()                { return (ch_monte); }

    double val1()               { return (ch_val1); }
    double val2()               { return (ch_val2); }
    double delta1()             { return (ch_delta1); }
    double delta2()             { return (ch_delta2); }
    int step1()                 { return (ch_step1); }
    int step2()                 { return (ch_step2); }

    void set_pflag(int i, int v) { ch_flags[i] = v; }

private:
    // check.cc
    void set_rangevec();
    void plot();
    bool loop();
    int trial(int, int, double, double);
    bool findrange1(double, int, bool, bool);
    bool findrange2(double, int, bool, bool);
    bool findext1(int, double*, double, double);
    bool findext2(int, double, double*, double);
    void addpoint(int, int, bool);

    FILE *ch_op;            // Output file pointer.
    char *ch_opname;        // Output file name.
    int ch_graphid;         // Graph id if plotting.
    bool ch_batchmode;      // If true, no user prompts.
    bool ch_no_output;      // Supress output recording.
    bool ch_use_remote;     // Submit analysis to servers.
    bool ch_monte;          // Monte Carlo analysis.
    bool ch_doall;          // Check all points.
    bool ch_pause;          // User requested pause.
    bool ch_nogo;           // Error, abort.
    bool ch_fail;           // Set if the conditionals failed, in
                            // the evaluate function or globally
                            // to stop analysis.  Only used if
                            // mode = OutcCheck*.
    int ch_iterno;          // Iterations for extrema seek.
    int ch_cycles;          // Cell count for OutcCheckMulti.
    int ch_evalcnt;         // Number out_evaluate() calls.
    int ch_index;           // The current index into the array,
                            //  incremented by simulator when the
                            //  running variable exceeds a check
                            //  point.  Only used if mode =
                            //  OutcCheck*.
    int ch_max;             // The size of the check point array,
                            //  set by driver.  Only used if mode =
                            //  OutcCheck*.
    double *ch_points;      // Pointer to an array of running
                            //  variable (e.g, time) points at
                            //  which the output conditionals are
                            //  to be evaluated, loaded by driver. 
                            //  Only used if mode = OutcCheck*.
    const char *ch_segbase; // Segment basename for OutcCheckSeg.
    wordlist *ch_cmdline;   // Command line, wordlist.
    double ch_val1;         // Range specification.
    double ch_val2;
    double ch_delta1;
    double ch_delta2;
    int ch_step1;
    int ch_step2;
    sNames *ch_names;       // Names of special vectors.
    wordlist *ch_devs1;     // Device/parameter lists.
    wordlist *ch_devs2;
    wordlist *ch_prms1;
    wordlist *ch_prms2;
    bool ch_gotval1;        // Flags indicate value set.
    bool ch_gotval2;
    bool ch_gotdelta1;
    bool ch_gotdelta2;
    bool ch_gotstep1;
    bool ch_gotstep2;
    char *ch_flags;         // P/F history.
    FILE *ch_tmpout;        // Redirect stdout server mode.
    char *ch_tmpoutname;    // Redirect stdout filename.

    static sHtab *ch_plotnames; // mplot filename to plotname map
};

// Structure used to store parameters for the sweep command.
//
struct sSWEEPprms : public sOUTcontrol
{
    // sweep.cc
    sSWEEPprms();
    ~sSWEEPprms();

    int sweepParse(wordlist**);
    int setInput();
    int points();

    double start[2];     // starting parameter value
    double stop[2];      // ending parameter value
    double step[2];      // step size
    double state[2];     // internal values saved
    int dims[3];         // dimensions of output vector
    int nestLevel;       // number of levels of nesting
    int nestSave;        // iteration state during pause
    sNames *names;       // names of special vectors, if no device/param
    wordlist *devs[2];   // devices specified
    wordlist *prms[2];   // parameters specified
    int tot_points;      // analysis points
    int loop_count;      // points done
};


// Attribute flags, third arg to IFoutput::setAttrs().
enum OUTscaleType
{
    OUT_SCALE_NONE,
    OUT_SCALE_LIN,
    OUT_SCALE_LOG
};

// Flags for the first argument to IFoutput::error.
enum ERRtype
{
    ERR_NONE,
    ERR_INFO,
    ERR_WARNING,
    ERR_FATAL,
    ERR_PANIC
};

struct sMsg
{
    sMsg(const char *s, ERRtype f)
        {
            string = s;
            flag = f;
        }

    const char *string;
    ERRtype flag;
};

// Structure: IFoutput
//
// This structure provides the simulator with an interface for saving
// results.  Most of the code is in output.cc.
//
struct IFoutput
{
    sRunDesc *beginPlot(sOUTdata*, int = 0, const char* = 0, double = 0.0);
    int appendData(sRunDesc*, IFvalue*, IFvalue*);
    int insertData(sCKT*, sRunDesc*, IFvalue*, IFvalue*, unsigned int);
    int setDims(sRunDesc*, int*, int, bool = false);
    int setDC(sRunDesc*, sDCTprms*);
    int setAttrs(sRunDesc*, IFuid*, OUTscaleType, IFvalue*);
    int initMeasure(sRunDesc*);
    int pauseTest(sRunDesc*);
    void unrollPlot(sRunDesc*);
    void addPlotNote(sRunDesc*, const char*);
    void endPlot(sRunDesc*, bool);
    void endIplot(sRunDesc*);
    double seconds();
    int error(ERRtype, const char*, ...);

    bool endit()            { return (o_endit); }
    void set_endit(bool b)  { o_endit = b; }

    sMsg *msgs()            { return (o_msgs); }

private:
    bool o_endit;           // If nonzero, quit the current analysis as if
                            // finished.
    bool o_shouldstop;      // Tell simulator to stop next time it asks.

    static sMsg o_msgs[];   // Error message prefixes.
};
extern IFoutput OP;

#endif // OUTDATA_H

