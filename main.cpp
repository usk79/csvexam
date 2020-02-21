#include <stdio.h>
#include <time.h>
#include "csvexam.h"

u4 tim;
s2 trq;
s4 rpm;
u2 cnt;
s4 sin;

dataexam::VARDATA varary[] = {
    SETVAR_M(tim, 0.005, time), 
    SETVAR(trq, 1./8.), 
    SETVAR(rpm, 1.), 
    SETVAR(cnt, 1.), 
    SETVAR(sin, 0.00001)
};

dataexam::VARDATA output[] = {
    SETVAR_M(tim, 0.005, time), 
    SETVAR(trq, 1./8.), 
    SETVAR(rpm, 1.), 
    SETVAR(cnt, 1.), 
    SETVAR(sin, 0.00001)
};

int main(int argc, char *argv[])
{
    char *inputfile, *outputfile;
    int debug = 0;
    int cnt = 0;
    clock_t start, end;
    start = clock();

    if (argc != 3)
    {
        printf("Invalid Comanndline Argument\n");
        return -1;
    }

    try {
        inputfile = argv[1];
        outputfile = argv[2];
        
        dataexam::csv exam = dataexam::csv(inputfile, varary, sizeof(varary), dataexam::MODE_READ);
        dataexam::csv rslt = dataexam::csv(outputfile, output, sizeof(output), dataexam::MODE_WRITE);
        
        while (exam.read_line(varary) == dataexam::OK)
        {
            if (cnt == 0)
            {
                rslt.write_line(varary);
            }
            
            debug++;
            cnt = debug & 0x0003;
        }
        printf("finish!\n");
    }
    catch (dataexam::exception e)
    {
        printf("%s\n", e.err_msg());
    }

    end = clock();
    printf("%.2f秒かかりました\n",(double)(end-start)/CLOCKS_PER_SEC);
    
    return 0;
}