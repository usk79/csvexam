typedef unsigned char   u1;
typedef unsigned short  u2;
typedef unsigned int    u4;
typedef signed char     s1;
typedef signed short    s2;
typedef signed int      s4;

#define SETVAR(var, lsb)                {&var, sizeof(var), #var                , lsb, -1}
#define SETVAR_P(var, lsb, pre)         {&var, sizeof(var), #pre ## #var        , lsb, -1}
#define SETVAR_S(var, lsb, suf)         {&var, sizeof(var),         #var ## #suf, lsb, -1}
#define SETVAR_PS(var, lsb, pre, suf)   {&var, sizeof(var), #pre ## #var ## #suf, lsb, -1}
#define SETVAR_M(var, lsb, name)        {&var, sizeof(var), #name               , lsb, -1}

namespace dataexam
{
    const int BUFSIZE        = 64;
    const int LINEBUFSIZE    = 2048;

    typedef struct {
        void* ptr;
        size_t size;
        char name[BUFSIZE];
        double lsb;
        int idx;
    } VARDATA;

    const int OK            = 1;
    const int FINISH        = -1;
    const int MODE_READ     = 0;
    const int MODE_WRITE    = 1;

    class exception
    {
        const char* errmsg;
    public:
        exception(const char* msg);
        const char* err_msg();
    };


    class csv {
        FILE *csvfile;  /* csv file pointer */
        int csv_colnum; /* csvファイルの列数 */
        char linebuf[LINEBUFSIZE];
        int varnum;      /* 検索対象の変数の数 */
        int mode;        /* モード： */
        double *values;

        void set_varindex(const char *str, VARDATA *var, int idx);
        void set_dat(void *buf, int dat, size_t size);
        int get_dat(void *buf, size_t size);
        void open_csv_r(const char *filename, VARDATA *var);
        void open_csv_w(const char *filename, VARDATA *var);
            
    public:
        csv(const char *filename, VARDATA *ary, const int sizeofary, int md);
        ~csv();

        int read_line(VARDATA *ary); /* 続きがあれば1, 終了なら-1 */
        void write_line(const VARDATA *ary);
    };

    class generator {
        VARDATA *datary;
        int varnum;

    public:
        generator(VARDATA *ary, int sizeofary);
        ~generator();
        void set_value(u4 t, u4 timing, double value, u2 &var);
    };
}