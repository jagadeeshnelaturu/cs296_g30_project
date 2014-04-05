.SUFFIXES: .cpp .hpp

# Programs
SHELL 	= bash
CC     	= g++
LD	= ld
RM 	= rm
ECHO	= /bin/echo
CAT	= cat
PRINTF	= printf
SED	= sed
DOXYGEN = doxygen
######################################
# Project Name (generate executable with this name)
TARGET = cs296_30_exe

# Project Paths
PROJECT_ROOT=./
EXTERNAL_ROOT=$(PROJECT_ROOT)/external
SRCDIR = $(PROJECT_ROOT)/src
OBJDIR = $(PROJECT_ROOT)/myobjs
BINDIR = $(PROJECT_ROOT)/mybins
DOCDIR = $(PROJECT_ROOT)/doc
LIBDIR = $(PROJECT_ROOT)/mylibs
EXTER_SRC=$(EXTERNAL_ROOT)/src
FILE_1=./external/include/Box2D/Box2D.h
FILE_2=./external/lib/libBox2D.a

# Library Paths
BOX2D_ROOT=$(EXTERNAL_ROOT)
GLUI_ROOT=/usr
GL_ROOT=/usr/include/

#Libraries
LIBS = -lBox2D -lglui -lglut -lGLU -lGL
SHARED_LIB =FALSE
STATIC_TARGET= $(LIBDIR)/libCS296test.a
DYNAMIC_TARGET = $(LIBDIR)/libCS296test.so

# Compiler and Linker flags
CPPFLAGS =-g -O3 -Wall -fno-strict-aliasing
CPPFLAGS+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include
LDFLAGS+=-L $(BOX2D_ROOT)/lib/ -L $(GLUI_ROOT)/lib

CPPFLAGSd =-pg -Wall -fno-strict-aliasing
CPPFLAGSd+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include
LDFLAGSd+= -pg -L $(BOX2D_ROOT)/lib/ -L $(GLUI_ROOT)/lib

CPPFLAGSr =-pg -O3 -Wall -fno-strict-aliasing
CPPFLAGSr+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include
LDFLAGSr+= -pg -O3 -L $(BOX2D_ROOT)/lib/ -L $(GLUI_ROOT)/lib
######################################

NO_COLOR=\e[0m
OK_COLOR=\e[1;32m
ERR_COLOR=\e[1;31m
WARN_COLOR=\e[1;33m
MESG_COLOR=\e[1;34m
FILE_COLOR=\e[1;37m

OK_STRING="[OK]"
ERR_STRING="[ERRORS]"
WARN_STRING="[WARNINGS]"
OK_FMT="${OK_COLOR}%30s\n${NO_COLOR}"
ERR_FMT="${ERR_COLOR}%30s\n${NO_COLOR}"
WARN_FMT="${WARN_COLOR}%30s\n${NO_COLOR}"
######################################

SRCS := $(wildcard $(SRCDIR)/*.cpp)
INCS := $(wildcard $(SRCDIR)/*.hpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
OBJSd := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%d.o)
OBJSr := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%r.o)
FILES_FOR_LIB :=./myobjs/callbacks.o ./myobjs/cs296_base.o ./myobjs/dominos.o ./myobjs/render.o

.PHONY: all setup doc distclean clean report

all: setup exe $(DYNAMIC_TARGET) $(STATIC_TARGET) exelib 

setup:
	@$(ECHO) "Setting up compilation..."
	@mkdir -p myobjs
	@mkdir -p mybins
	@mkdir -p mylibs
	@echo "Checking If Box2D Is Installed"
	@if  test -e  $(FILE_1) && test -e $(FILE_2) ; then \
	echo "Box2D is installed!!!!"; \
	else \
	echo "Installing......"; \
	cd $(EXTER_SRC); tar -zxf Box2D.tgz; \
	cd Box2D; \
	mkdir -p build296; \
	cd build296; \
	cmake ../; \
	make; \
	make install; \
	fi 
setupd:
	@$(ECHO) "Setting up compilation..."
	@mkdir -p myobjs
	@mkdir -p mybins
	@mkdir -p mylibs
	@echo "Checking If Box2D Is Installed"
	@if  test -e  $(FILE_1) && test -e $(FILE_2) ; then \
	echo "Box2D is installed!!!!"; \
	else \
	echo "Installing......"; \
	cd $(EXTER_SRC); tar -zxf Box2D.tgz; \
	cd Box2D; \
	mkdir -p build296; \
	cd build296; \
	cmake -DCMAKE_BUILD_TYPE=Debug ../; \
	make; \
	make install; \
	fi 

setupr:
	@$(ECHO) "Setting up compilation..."
	@mkdir -p myobjs
	@mkdir -p mybins
	@mkdir -p mylibs
	@echo "Checking If Box2D Is Installed"
	@if  test -e  $(FILE_1) && test -e $(FILE_2) ; then \
	echo "Box2D is installed!!!!"; \
	else \
	echo "Installing......"; \
	cd $(EXTER_SRC); tar -zxf Box2D.tgz; \
	cd Box2D; \
	mkdir -p build296; \
	cd build296; \
	cmake -DCMAKE_BUILD_TYPE=Release ../; \
	make; \
	make install; \
	fi  

exe: setup $(OBJS)
	@$(PRINTF) "$(MESG_COLOR)Building executable:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $(TARGET))"
	@$(CC) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) 2> temp.log || touch temp.err
	@mv $(TARGET) ./mybins/$(TARGET)
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) $(OK_FMT) $(OK_STRING); \
	fi;
	@$(RM) -f temp.log temp.err

-include -include $(OBJS:.o=.d)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp 
	@$(PRINTF) "$(MESG_COLOR)Compiling: $(NO_COLOR) $(FILE_COLOR) %25s$(NO_COLOR)" "$(notdir $<)"
	@$(CC) -fPIC $(CPPFLAGS) -c $< -o $@ -MD 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else printf "${OK_COLOR}%30s\n${NO_COLOR}" "[OK]"; \
	fi;
	@$(RM) -f temp.log temp.err

exed: setupd $(OBJSd)
	@$(PRINTF) "$(MESG_COLOR)Building executable:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $(TARGET))"
	@$(CC) -o $(TARGET) $(LDFLAGSd) $(OBJSd) $(LIBS) 2> temp.log || touch temp.err
	@mv $(TARGET) ./mybins/$(TARGET)
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) $(OK_FMT) $(OK_STRING); \
	fi;
	@$(RM) -f temp.log temp.err

-include -include $(OBJSd:.o=.d)

$(OBJSd): $(OBJDIR)/%d.o : $(SRCDIR)/%.cpp 
	@$(PRINTF) "$(MESG_COLOR)Compiling: $(NO_COLOR) $(FILE_COLOR) %25s$(NO_COLOR)" "$(notdir $<)"
	@$(CC) -fPIC $(CPPFLAGSd) -c $< -o $@ -MD 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else printf "${OK_COLOR}%30s\n${NO_COLOR}" "[OK]"; \
	fi;
	@$(RM) -f temp.log temp.err

exer: setupr $(OBJSr)
	@$(PRINTF) "$(MESG_COLOR)Building executable:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $(TARGET))"
	@$(CC) -o $(TARGET) $(LDFLAGSr) $(OBJSr) $(LIBS) 2> temp.log || touch temp.err
	@mv $(TARGET) ./mybins/$(TARGET)
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) $(OK_FMT) $(OK_STRING); \
	fi;
	@$(RM) -f temp.log temp.err

-include -include $(OBJSr:.o=.d)

$(OBJSr): $(OBJDIR)/%r.o : $(SRCDIR)/%.cpp 
	@$(PRINTF) "$(MESG_COLOR)Compiling: $(NO_COLOR) $(FILE_COLOR) %25s$(NO_COLOR)" "$(notdir $<)"
	@$(CC) -fPIC $(CPPFLAGSr) -c $< -o $@ -MD 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else printf "${OK_COLOR}%30s\n${NO_COLOR}" "[OK]"; \
	fi;
	@$(RM) -f temp.log temp.err

$(STATIC_TARGET) :  $(OBJS)
	@if test "$(SHARED_LIB)" = "FALSE"; then \
	echo "Creating Static Library..." ; \
	ar rcs $@ $(FILES_FOR_LIB); \
	fi;

$(DYNAMIC_TARGET) : $(OBJS)
	@if test "$(SHARED_LIB)" = "TRUE"; then \
	echo "Creating Dynamic Library..."; \
	gcc  -fPIC -shared -o $@ $(FILES_FOR_LIB); \
	fi;

exelib: setup $(STATIC_TARGET) $(DYNAMIC_TARGET)
	@$(ECHO) "Creating Executable Using Library..."
	@cd ./myobjs ; \
	$(CC) -fPIC -o cs296_30_exelib $(LDFLAGS) -L ../mylibs/ -L ../external/lib/ -L ../myobjs main.o -lCS296test $(LIBS)
	@mv myobjs/cs296_30_exelib mybins/

doc:
	@$(ECHO) -n "Generating Doxygen Documentation ...  "
	@$(RM) -rf doc/html
	@$(DOXYGEN) $(DOCDIR)/Doxyfile 2 > /dev/null
	@$(ECHO) "Done"

clean: 
	@$(ECHO) -n "Cleaning up..."
	@$(RM) -rf $(OBJDIR)/* *~ $(DEPS) $(SRCDIR)/*~
	@$(RM) -rf ./mylibs
	@$(RM) -rf ./mybins
	@$(RM) -rf ./myobjs
	@$(RM) -rf ./doc/html
	@$(RM) -rf ./doc/cs296_report_30.pdf
	@$(RM) -rf ./doc/cs296_report_30.aux
	@$(RM) -rf ./doc/cs296_report_30.log
	@$(RM) -rf ./doc/cs296_report_30.blg
	@$(RM) -rf ./doc/cs296_report_30.bbl
	@$(RM) -rf ./data_gnuplot
	@$(RM) -rf ./plots_gnuplot
	@$(RM) -rf ./data_prof
	@$(RM) -rf ./data_matplotlib
	@$(RM) -rf ./plots_matplotlib
	@$(RM) -rf ./scripts_gnuplot/fit.log
	@$(ECHO) "Done"

distclean: clean
	@$(RM) -rf $(BINDIR) $(DOCDIR)/html
	@$(RM) -rf ./mybins
	@$(RM) -rf ./mylibs
	@$(RM) -rf ./myobjs
	@$(RM) -rf ./external/src/Box2D
	@$(RM) -rf ./external/include/*
	@$(RM) -rf ./external/lib/*

report: 
	@ cd ./project_report && pdflatex g30_project_report.tex && bibtex g30_project_report.aux && pdflatex g30_project_report.tex && pdflatex g30_project_report.tex

datag: exe
	@ mkdir -p data_gnuplot
	@ cd ./scripts_gnuplot && ./g30_gen_data.sh
	@ cd ./scripts_gnuplot  && ./g30_gen_csv.sh
	@ cd ./scripts_gnuplot  && ./g30_gen_data_csv.sh
	@ cd ./scripts_gnuplot  && ./g30_gen_data_random.sh

plotg: datag
	@ mkdir -p plots_gnuplot
	@ cd ./scripts_gnuplot && ./gen_avg.sh
	@ cd ./scripts_gnuplot && gnuplot g30_plot01.gpt
	@ cd ./scripts_gnuplot && gnuplot g30_plot02.gpt
	@ cd ./scripts_gnuplot && gnuplot g30_plot03.gpt
	@$(RM) -rf ./data_gnuplot/temp.data
	@ cd ./scripts_gnuplot && ./gen_iteration.sh
	@ cd ./scripts_gnuplot && gnuplot g30_plot04.gpt
	@$(RM) -rf ./data_gnuplot/temp_plot4.data
	@ cd ./scripts_gnuplot && ./gen_avg_random.sh
	@ cd ./scripts_gnuplot && ./gen_avg_data02.sh
	@ cd ./scripts_gnuplot && gnuplot g30_plot05.gpt
	@$(RM) -rf ./data_gnuplot/temp_random.data
	@$(RM) -rf ./data_gnuplot/temp_avg_data02.data

release_prof: distclean setupr $(OBJSr) exer
	@ mkdir -p data_prof
	@ ./mybins/cs296_30_exe 1000
	@ gprof ./mybins/cs296_30_exe gmon.out > ./data_prof/g30_release_prof.dat
	@ #rm -rf gmon.out

debug_prof: distclean setupd $(OBJSd) exed
	@ mkdir -p data_prof
	@ ./mybins/cs296_30_exe 1000
	@ gprof ./mybins/cs296_30_exe gmon.out > ./data_prof/g30_debug_prof.dat
	@ #rm -rf gmon.out

html: exe
	@ mkdir -p data_matplotlib
	@ ./scripts_matplotlib/g30_gen_csv.py
	@ mkdir -p plots_matplotlib
	@ ./scripts_matplotlib/g30_gen_plots.py
	@ ./scripts_matplotlib/g30_gen_html.py
