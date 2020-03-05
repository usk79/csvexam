typedef unsigned char   u1;
typedef unsigned short  u2;
typedef unsigned int    u4;
typedef signed char     s1;
typedef signed short    s2;
typedef signed int      s4;

#define SETVAR(var, lsb)                {&var, sizeof(var), #var                , lsb, -1, dataexam::NOEDGE}
#define SETVAR_P(var, lsb, pre)         {&var, sizeof(var), #pre ## #var        , lsb, -1, dataexam::NOEDGE}
#define SETVAR_S(var, lsb, suf)         {&var, sizeof(var),         #var ## #suf, lsb, -1, dataexam::NOEDGE}
#define SETVAR_PS(var, lsb, pre, suf)   {&var, sizeof(var), #pre ## #var ## #suf, lsb, -1, dataexam::NOEDGE}
#define SETVAR_M(var, lsb, name)        {&var, sizeof(var), #name               , lsb, -1, dataexam::NOEDGE}

#define SETVAR_TYPE(var, lsb, type)                {&var, sizeof(var), #var                , lsb, -1, type}
#define SETVAR_TYPE_P(var, lsb, type, pre)         {&var, sizeof(var), #pre ## #var        , lsb, -1, type}
#define SETVAR_TYPE_S(var, lsb, type, suf)         {&var, sizeof(var),         #var ## #suf, lsb, -1, type}
#define SETVAR_TYPE_PS(var, lsb, type, pre, suf)   {&var, sizeof(var), #pre ## #var ## #suf, lsb, -1, type}
#define SETVAR_TYPE_M(var, lsb, type, name)        {&var, sizeof(var), #name               , lsb, -1, type}

namespace dataexam
{
    typedef enum {
        NOEDGE,
        BOTHEDGE_CYCLE,
        RISEEDGE_CYCLE,
        FALLEDGE_CYCLE,
        ELEM_NUM
    } updflg;

    const int BUFSIZE        = 64;
    const int LINEBUFSIZE    = 2048;

    typedef struct {
        void* ptr;
        size_t size;
        char name[BUFSIZE];
        double lsb;
        int idx;
        updflg type;
    } VARDATA;

    const int OK            = 1;
    const int FINISH        = -1;
    const int MODE_READ     = 0;
    const int MODE_WRITE    = 1;
    const int UPDATE_ALWAYS = 0;
    const int UPDATE_EDGE   = 1;

    class exception
    {
        const char* errmsg;
    public:
        exception(const char* msg);
        const char* err_msg();
    };

    typedef struct {
        double data;    /* CSVから読んだデータ */    
        updflg edge_flg;  /* 前回値から変化したか */
    } CSVDATA;

    class csv {
        FILE *csvfile;  /* csv file pointer */
        VARDATA *varary;
        int csv_colnum; /* csvファイルの列数 */
        char linebuf[LINEBUFSIZE];
        int varnum;      /* 検索対象の変数の数 */
        int mode;        /* モード： */
        CSVDATA *values;

        updflg check_edgeflg(double last_data, double data);
        bool compare_edgeflg(updflg flg, updflg type);
        void set_varindex(const char *str, int idx);
        void set_dat(void *buf, int dat, size_t size);
        int get_dat(void *buf, size_t size);
        void open_csv_r(const char *filename);
        void open_csv_w(const char *filename);
            
    public:
        csv(const char *filename, VARDATA *ary, const int sizeofary, int md);
        ~csv();

        int read_line(); /* 続きがあれば1, 終了なら-1 */
        void write_line();
    };

    class generator {
        VARDATA *datary;
        int varnum;

        template <class T>
        int get_varidx(T &var);

    public:
        generator(VARDATA *ary, int sizeofary);
        ~generator();

        template <class T>
        void set_value(u4 t, u4 timing, double value, T &var);
    };
}