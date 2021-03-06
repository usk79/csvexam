#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dataexam.h"

namespace dataexam
{
    exception::exception(const char* msg)
    {
        errmsg = msg;
    }

    const char* exception::err_msg()
    {
        return errmsg;
    }

    csv::csv(const char *filename, VARDATA *ary, const int sizeofary, int md)
    {
        mode = md;
        varnum = sizeofary / sizeof(VARDATA);
        values = NULL;
        varary = ary;

        for (int i = 0; i < varnum; i++) {
            if (varary[i].lsb <= 0) { throw exception("Invalid LSB is set."); }
        }

        switch (mode) {
            case MODE_READ:
                open_csv_r(filename);
                break;
            case MODE_WRITE:
                open_csv_w(filename);
                break;
            default:
                throw exception("Invalid Mode is set.");
        }
    }

    csv::~csv()
    {
        if (values != NULL) delete[] values;
        fclose(csvfile);
    }

    void csv::open_csv_r(const char *filename)
    {   
        char *str;
        int idx = 0;
        errno_t fopen_err;
        char *ctx;

        fopen_err = fopen_s(&csvfile, filename, "r");
        if (fopen_err != 0)
        {
            throw exception("Can not open file.\n");
        }

        fgets(linebuf, LINEBUFSIZE, csvfile);
        
        str = strtok_s(linebuf, ",", &ctx);
        set_varindex(str, idx);
            
        while (str != NULL) {
            idx++;
            str = strtok_s(NULL, ",", &ctx);
            if (str != NULL) {
                set_varindex(str, idx);
            }
        }

        csv_colnum = idx;

        /* Inform inexist variables. */
        for (int i = 0; i < varnum; i++) {
            if (varary[i].idx < 0) {
                printf("variable : \"%s\" is not found.\n", varary[i].name);
            }
        }
        
        values = new CSVDATA[csv_colnum]; 
        memset(values, 0, sizeof(CSVDATA) * csv_colnum); /* initialize values */
    }

    void csv::open_csv_w(const char *filename)
    {
        errno_t fopen_err;

        fopen_err = fopen_s(&csvfile, filename, "w");
        if (fopen_err != 0)
        {
            throw exception("Can not open file.\n");
        }

        memset(linebuf, '\0', LINEBUFSIZE);

        for (int i = 0; i < varnum; i++) {
            strcat_s(linebuf, sizeof(linebuf), varary[i].name);
            if (i < varnum - 1) {
                strcat_s(linebuf, sizeof(linebuf), ",");
            }
        }
        fputs(linebuf, csvfile);
        fputc('\n', csvfile);
    }

    updflg csv::check_edgeflg(double last_data, double data)
    {
        if (last_data < data) {
            return RISEEDGE_CYCLE;
        }
        else if (last_data > data) {
            return FALLEDGE_CYCLE;
        }

        return NOEDGE;
    }

    bool csv::compare_edgeflg(updflg flg, updflg type)
    {
        bool ret;

        switch (type) {
            case BOTHEDGE_CYCLE:
                ret = (flg == FALLEDGE_CYCLE || flg == RISEEDGE_CYCLE);
                break;
            case RISEEDGE_CYCLE:
                ret = (flg == RISEEDGE_CYCLE);
                break;
            case FALLEDGE_CYCLE:
                ret = (flg == FALLEDGE_CYCLE);
                break;
            default:
                ret = true;
        }
        return ret;
    }

    void csv::set_varindex(const char *str, int idx)
    {
        char buf[BUFSIZE];
        int varlen;

        if (strlen(str) > BUFSIZE) {
            throw exception("Variable name is too long.");
        }
        strcpy_s(buf, str);
        
        varlen = strlen(buf);
        if (buf[varlen - 1] == '\n') buf[varlen - 1] = '\0'; 

        for (int i = 0; i < varnum; i++) {
            if (strcmp(buf, varary[i].name) == 0) {
                varary[i].idx = idx;
            }
        }
    }

    int csv::read_line()
    {
        char *str;
        char *ctx;
        int idx = 0;
        double tmpdata;

        if (mode == MODE_WRITE) {
            throw exception("Mode is Inappropriate.");
        }

        fgets(linebuf, LINEBUFSIZE, csvfile);
        if (feof(csvfile) != 0)
        {
            return FINISH;
        }

        /* load a line from csv data */
        str = strtok_s(linebuf, ",", &ctx);
        tmpdata = atof(str);
        
        values[idx].edge_flg = check_edgeflg(values[idx].data, tmpdata);
        values[idx].data = tmpdata;

        while (str != NULL) {
            idx++;
            str = strtok_s(NULL, ",", &ctx);
            if (str != NULL) {
                if (idx >= csv_colnum) throw exception("column size is incorrect!");

                tmpdata = atof(str);

                values[idx].edge_flg = check_edgeflg(values[idx].data, tmpdata);
                values[idx].data = tmpdata;
                        
            }
        }

        /* set data to varary */
        for (int i = 0; i < varnum; i++) {
            VARDATA *dat = &varary[i];
            if (dat->idx >= 0 && compare_edgeflg(values[dat->idx].edge_flg, dat->type))
            {
                double value = values[dat->idx].data;
                int tmp;
                if (value < 0) {
                    if (dat->lsb <= 1) {
                        tmp = int( value / dat->lsb - dat->lsb / 2); /* rounding */
                    }
                    else {
                        tmp = int( value / dat->lsb - 0.5); /* rounding */
                    }
                }
                else {
                    if (dat->lsb <= 1) {
                        tmp = int( value / dat->lsb + dat->lsb / 2); /* rounding */
                    }
                    else {
                        tmp = int( value / dat->lsb + 0.5); /* rounding */
                    }
                }
                set_dat(dat->ptr, tmp, dat->size);
            }
        }
        return OK;
    }

    void csv::write_line()
    {   
        char buf[BUFSIZE];

        memset(linebuf, '\0', LINEBUFSIZE);

        if (mode == MODE_READ) {
            throw exception("Mode is Inappropriate.");
        }

        for (int i = 0; i < varnum; i++) {
            int dat = get_dat(varary[i].ptr, varary[i].size);
            double tmp = (double)dat * varary[i].lsb;

            sprintf_s(buf, sizeof(buf), "%f", tmp);
            if (i < varnum - 1) {
                strcat_s(buf, sizeof(buf), ",");
            }
            strcat_s(linebuf, sizeof(linebuf), buf);
        }
        fputs(linebuf, csvfile);
        fputc('\n', csvfile);
    }

    void csv::set_dat(void *buf, int dat, size_t size)
    {
        unsigned char *ptr = (unsigned char *)buf;
        size_t i;

        for (i = 0; i < size; i++) {
            *ptr++ = (unsigned char)(dat >> (i << 3));
        }

    }

    int csv::get_dat(void *buf, size_t size)
    {
        unsigned char *ptr = (unsigned char *)buf;
        size_t i;
        int ret = 0;

        for (i = 0; i < size; i++) {
            ret += (int)(*ptr++ << (i << 3));
        }

        if ((char)(*--ptr) < 0) {
            ret -= (1 << (i << 3));
        }

        return ret;
    }

    generator::generator(VARDATA *ary, int sizeofary)
    {
        varnum = sizeofary / sizeof(VARDATA);
        datary = ary;
    }

    generator::~generator()
    {

    }

    template <class T>
    int generator::get_varidx(T &var)
    {
        int i = 0;
        int idx = -1;

        while (true) {
            if (datary[i].ptr == &var) {
                idx = i;
                break;
            }

            i++;
            if (i >= varnum) {
                throw exception("generator::set_value() : variable is not found.");
            }
        }

        return idx;
    }

    template <class T>
    void generator::set_value(u4 t, u4 timing, double value, T &var)
    {
        int i = 0;
        int idx;
        double lsb;

        if (t == timing) {
            idx = get_varidx(var);
            lsb = datary[idx].lsb;
            
            var = (int)(value / lsb);
        }
    }

    template int generator::get_varidx<u1>(u1 &var);
    template int generator::get_varidx<u2>(u2 &var);
    template int generator::get_varidx<u4>(u4 &var);
    template int generator::get_varidx<s1>(s1 &var);
    template int generator::get_varidx<s1>(s1 &var);
    template int generator::get_varidx<s1>(s1 &var);

    template void generator::set_value<u1>(u4 t, u4 timing, double value, u1 &var);
    template void generator::set_value<u2>(u4 t, u4 timing, double value, u2 &var);
    template void generator::set_value<u4>(u4 t, u4 timing, double value, u4 &var);
    template void generator::set_value<s1>(u4 t, u4 timing, double value, s1 &var);
    template void generator::set_value<s2>(u4 t, u4 timing, double value, s2 &var);
    template void generator::set_value<s4>(u4 t, u4 timing, double value, s4 &var);
}