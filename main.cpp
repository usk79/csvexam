#include <stdio.h>
#include <time.h>
#include "csvexam.h"

/* 試験パターン生成テスト */
u2 tim;
u2 run;
u2 reset;
u2 mode;
u2 runok;
u2 trq;

dataexam::VARDATA varary[] = {
    SETVAR_M(tim, 0.002, time),
    SETVAR(run, 1.),
    SETVAR(reset, 1.),
    SETVAR(mode, 1.),
    SETVAR(runok, 1.),
    SETVAR(trq, 1./8.)
};

void gen_signal(dataexam::generator &gen)
{
    gen.set_value(tim, 10, 1, reset);
    gen.set_value(tim, 15, 0, reset);

    gen.set_value(tim, 20, 3, mode);

    gen.set_value(tim, 25, 1, run);

    gen.set_value(tim, 35, 5.25, trq);

}

void logic()
{    
    if (mode == 3) runok = 1;
    else runok = 0;
}

int main(int argc, char *argv[])
{
    char *inputfile, *outputfile;
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

        dataexam::csv rslt = dataexam::csv("pattern.csv", varary, sizeof(varary), dataexam::MODE_WRITE);
        dataexam::generator gen = dataexam::generator(varary, sizeof(varary));

        for (tim = 0; tim < 200; tim++)
        {
            gen_signal(gen);

            logic();

            rslt.write_line(varary);
        }

    }
    catch (dataexam::exception e)
    {
        printf("%s\n", e.err_msg());
    }

    end = clock();
    printf("%.2f秒かかりました\n",(double)(end-start)/CLOCKS_PER_SEC);

    return 0;
}



/* CSVファイルの読込と書き出しテスト */
#if 0
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
#endif