TARGETNAME=test
OUTDIR=.\build
LINK32=link.exe


ALL : $(OUTDIR)\$(TARGETNAME).exe

CLEAN :
        -@erase /Q $(OUTDIR)\*

$(OUTDIR) :
        @if not exist $(OUTDIR) mkdir $(OUTDIR)


CPPFLAGS=\
        /nologo\
        /W3\
        /Fo"$(OUTDIR)\\"\
        /Fd"$(OUTDIR)\\"\
        /c\
        /Zi\
        /D_WIN32_WINNT=0x0600\
        /DUNICODE\
        /D_UNICODE


LINK32_FLAGS=\
        /nologo\
        /subsystem:console\
        /pdb:"$(OUTDIR)\$(TARGETNAME).pdb"\
        /out:"$(OUTDIR)\$(TARGETNAME).exe"\
        /DEBUG


LINK32_OBJS=\
        $(OUTDIR)\main.obj\
        $(OUTDIR)\dataexam.obj

$(OUTDIR)\$(TARGETNAME).exe : $(OUTDIR) $(LINK32_OBJS)
        $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)


.cpp{$(OUTDIR)}.obj:
        $(CPP) $(CPPFLAGS) $<