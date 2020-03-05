#include <stdio.h>
#include <time.h>
#include "dataexam.h"


#if 0
/* 試験パターン生成テスト */
u2 tim;
u2 run;
u2 reset;
u2 mode;
u2 runok;
u2 status;
s2 trq;
u2 setok;
u2 modeok;



dataexam::VARDATA varary[] = {
    SETVAR_M(tim, 0.002, time),
    SETVAR(reset, 1.),
    SETVAR(mode, 1.),
    SETVAR(run, 1.),
    SETVAR(trq, 1./8.),
    SETVAR(runok, 1.),
    SETVAR(status, 1.)
};
/* 微分を検出するオプションを付けるか検討 */
void gen_signal(dataexam::generator &gen)
{
    gen.set_value(tim, 10, 1, reset);
    gen.set_value(tim, 15, 0, reset);

    gen.set_value(tim, 20, 3, mode);

    gen.set_value(tim, 25, 1, run);

    gen.set_value(tim, 35, 5.25, trq);

}

void init()
{
    tim = 0;
    run = 0;
    reset = 0;
    mode = 0;
    runok = 0;
    status = 9;
    trq = 0;
    setok = 0;
    modeok = 0;
}

void logic()
{   
    static u2 reseto = 0;
    static u2 runo = 0;
    static u2 runcnt = 0;

    setok = (trq < 50);
    modeok = (mode == 2);
    runok = (status == 0 && setok && modeok);

    if ((status == 9) && (reset == 1 && reseto == 0))
    {
        status = 0;
    }
    
    if ((status == 0) && (run == 1 && runo == 0))
    {
        status = 1;
        runcnt = 0;
    }

    if (status == 1)
    {
        runcnt++;
        
        if (runcnt > 20)
        {
            status = 0;
            run = 0;
        }
    }
    


    runo = run;
    reseto = reset;
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

            rslt.write_line();
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
#endif


/* CSVファイルの読込と書き出しテスト */
#if 1
u2 tim;
u2 run;
u2 sig;
u2 reset;
u2 mode;
u2 runok;
u2 status;
s2 trq;
u2 setok;
u2 modeok;

dataexam::VARDATA varary[] = {
    SETVAR_M(tim, 0.002, time),
    SETVAR(reset, 1.),
    SETVAR_TYPE(mode, 1., dataexam::BOTHEDGE_CYCLE),
    SETVAR_TYPE(run, 1., dataexam::RISEEDGE_CYCLE),
    SETVAR_TYPE(sig, 1., dataexam::FALLEDGE_CYCLE),
    SETVAR(trq, 1./8.),
};
dataexam::VARDATA output[] = {
    SETVAR_M(tim, 0.002, time),
    SETVAR(reset, 1.),
    SETVAR(mode, 1.),
    SETVAR(run, 1.),
    SETVAR(sig, 1.),
    SETVAR(trq, 1./8.),
    SETVAR(runok, 1.),
    SETVAR(setok, 1.),
    SETVAR(modeok, 1.),
    SETVAR(status, 1.)
};

void init()
{
    tim = 0;
    run = 0;
    reset = 0;
    mode = 0;
    runok = 0;
    status = 9;
    trq = 0;
    setok = 0;
    modeok = 0;
}

void logic()
{   
    static u2 reseto = 0;
    static u2 runo = 0;
    static u2 runcnt = 0;

    setok = (trq < 400);
    modeok = (mode == 2);
    runok = (status == 0 && setok && modeok);

    if ((status == 9) && (reset == 1 && reseto == 0))
    {
        status = 0;
    }
    
    if ((status == 0 && runok) && (run == 1 && runo == 0))
    {
        status = 1;
        runcnt = 0;
    }

    if (status == 1)
    {
        runcnt++;
        
        if (runcnt > 20)
        {
            status = 0;
            run = 0;
        }
    }
    
    if (tim == 2 || tim == 5 || tim == 8)
    {
        mode = 0;
        run = 0;
        sig = 0;
        trq = 0;
    }


    runo = run;
    reseto = reset;
}

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
        
        init();

        while (exam.read_line() == dataexam::OK)
        {
            logic();

            rslt.write_line();
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